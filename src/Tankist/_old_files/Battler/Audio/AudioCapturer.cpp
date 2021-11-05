/* (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by */

#ifdef WIN32

#include <stdafx.h>
#pragma warning(push, 0)
#include <bass.h>
#include <opus.h>
#pragma warning(pop)
#include "AudioCapturer.h"

#ifdef WIN32
#pragma comment(lib, "bass.lib")
#pragma comment(lib, "opus.lib")
#endif



#define SAMPLERATE 8000
//#define ADJUSTRATE      // Adjust the output rate (in case input and output devices are going at slightly different speeds)

static HSTREAM chan = 0;
static DWORD prebuf = 0;
#ifdef WIN32
static DWORD targbuf = 0;
#endif
static DWORD rate = 0;
static HRECORD rchan = 0;


struct OpusEncoder *enc = nullptr;
struct OpusDecoder *dec = nullptr;

static void CreateEncodeDecode();
// Encode voice data. bufIn - input buffer, sizeInOut - length input/output buffer. Return output buffer. If data not ready, return 0.
static void* OPUS_Encode(void *bufIn, int *sizeInOut);



void DisplayDeviceInfo(BASS_DEVICEINFO *di)
{
    URHO3D_LOGINFOF("%s\n\tdriver: %s\n\ttype: ", di->name, di->driver);
    switch(di->flags&BASS_DEVICE_TYPE_MASK)
    {
        case BASS_DEVICE_TYPE_NETWORK:
            URHO3D_LOGINFO("Remote Network");
            break;
        case BASS_DEVICE_TYPE_SPEAKERS:
            URHO3D_LOGINFO("Speakers");
            break;
        case BASS_DEVICE_TYPE_LINE:
            URHO3D_LOGINFO("Line");
            break;
        case BASS_DEVICE_TYPE_HEADPHONES:
            URHO3D_LOGINFO("Headphones");
            break;
        case BASS_DEVICE_TYPE_MICROPHONE:
            URHO3D_LOGINFO("Microphone");
            break;
        case BASS_DEVICE_TYPE_HEADSET:
            URHO3D_LOGINFO("Headset");
            break;
        case BASS_DEVICE_TYPE_HANDSET:
            URHO3D_LOGINFO("Handset");
            break;
        case BASS_DEVICE_TYPE_DIGITAL:
            URHO3D_LOGINFO("Digital");
            break;
        case BASS_DEVICE_TYPE_SPDIF:
            URHO3D_LOGINFO("SPDIF");
            break;
        case BASS_DEVICE_TYPE_HDMI:
            URHO3D_LOGINFO("HDMI");
            break;
        case BASS_DEVICE_TYPE_DISPLAYPORT:
            URHO3D_LOGINFO("DisplayPort");
            break;
        default:
            URHO3D_LOGINFO("Unknown");
    }
    URHO3D_LOGINFO("\n\tflags:");
    if(di->flags&BASS_DEVICE_ENABLED) URHO3D_LOGINFO(" enabled");
    if(di->flags&BASS_DEVICE_DEFAULT) URHO3D_LOGINFO(" default");
    URHO3D_LOGINFOF(" (%x)\n", di->flags);
}


AudioCapturer::AudioCapturer()
{
    /*
    BASS_DEVICEINFO di;
    int a;
    LOG_INFO("Output Devices\n");
    for(a = 1; BASS_GetDeviceInfo(a, &di); a++)
    {
        LOG_INFOF("%d: ", a);
        DisplayDeviceInfo(&di);
    }
    LOG_INFO("\nInput Devices\n");
    for(a = 0; BASS_RecordGetDeviceInfo(a, &di); a++)
    {
        LOG_INFOF("%d: ", a);
        DisplayDeviceInfo(&di);
    }
    */

    if(HIWORD(BASS_GetVersion()) != BASSVERSION)
    {
        URHO3D_LOGERRORF("An incorrect version of bass.dll. Required %d, available %d", BASSVERSION, BASS_GetVersion());
    }

    CreateEncodeDecode();
}


void CreateEncodeDecode()
{
    int error = 0;

    enc = opus_encoder_create(8000, 2, OPUS_APPLICATION_AUDIO, &error);

    opus_encoder_ctl(enc, OPUS_SET_COMPLEXITY(1));
    opus_encoder_ctl(enc, OPUS_SET_BITRATE(6000));
    opus_encoder_ctl(enc, OPUS_SET_SIGNAL(OPUS_SIGNAL_MUSIC));

    dec = opus_decoder_create(8000, 2, &error);

    //opus_decoder_ctl(dec, OPUS_SET_COMPLEXITY(5));
    //opus_encoder_ctl(enc, OPUS_SET_BITRATE(32000));
    //opus_decoder_ctl(dec, OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE));
}


void* AudioCapturer::OPUS_Decode(void *bufIn, int *sizeInOut)
{
    static uint8 buffer[10000];

    int numBytes = opus_decode(dec, (uint8*)bufIn, *sizeInOut, (opus_int16*)buffer, 2000, 0);

    if(numBytes < 0)
    {
        URHO3D_LOGERRORF("Error decode with code %d", numBytes);
    }
    
    *sizeInOut = numBytes * 4;
    return buffer;
}


void* OPUS_Encode(void *buffIn, int *sizeInOut)
{
    static const int SIZE_BUFFER = 320;

    static uint8 bufferIn[10000];
    static uint8 bufferOut[10000];

    static int bytesInInput = 0;
    uint8 *pointerIn = bufferIn;

    memcpy(bufferIn + bytesInInput, buffIn, (size_t)*sizeInOut);
    bytesInInput += *sizeInOut;

    int encodeBytes = 0;

#define LENGTH (SIZE_BUFFER * 4)

    while(bytesInInput >= LENGTH)
    {
        int nextBytes = opus_encode(enc, (opus_int16*)pointerIn, SIZE_BUFFER, bufferOut + encodeBytes, 5000);
        if(nextBytes < 0)
        {
            URHO3D_LOGERRORF("Error encode %d", nextBytes);
        }
        encodeBytes += nextBytes;
        pointerIn += LENGTH;
        bytesInInput -= LENGTH;
    }
    
    if(encodeBytes <= 0)
    {
        return 0;
    }

    if(bytesInInput)
    {
        memcpy(bufferIn, pointerIn, (size_t)bytesInInput);
    }

    *sizeInOut = encodeBytes;

    return bufferOut;
}


static BOOL CALLBACK RecordingCallback(HRECORD /*handle*/, const void *buffer, DWORD length, void *user)
{
    bool *pause = (bool*)user;
    if(*pause)
    {
        return true;
    }

    static float prevTime = 0.0f;
    static int allBytes = 0;
    static int recvBytes = 0;

    if(TheTime->GetElapsedTime() - prevTime >= 1.0f)
    {
        //LOG_INFOF("bytes : all - %d, transmitted - %d", allBytes, recvBytes);
        prevTime = TheTime->GetElapsedTime();
        allBytes = 0;
        recvBytes = 0;
    }

    allBytes += length;

    if(TheChat)
    {
        uint8 *pointer = (uint8*)buffer;

        do
        {
            uint sendBytes = length;
            if(sendBytes > 1000)
            {
                sendBytes = 1000;
            }

            int numBytes = (int)sendBytes;

            void* data = OPUS_Encode(pointer, &numBytes);

            recvBytes += numBytes;

            if(data)
            {
                TheChat->SendAudioData(data, (uint)numBytes);
            }

            pointer += sendBytes;
            length -= sendBytes;

        } while(length);
    }

    return true;
}


void AudioCapturer::PlayData(void *buffer, uint length)
{
    DWORD bl;

    BASS_StreamPutData(chan, buffer, length);

    bl = BASS_ChannelGetData(chan, NULL, BASS_DATA_AVAILABLE);

    if(prebuf)
    {
        if(bl >= prebuf + length)
        {
            prebuf = 0;
            BASS_ChannelPlay(chan, FALSE);
        }
    }
}


bool AudioCapturer::Start()
{
    Init();

    if(TheTypeApplication == Type_Client)
    {
        if(!BASS_RecordInit(-1))
        {
            URHO3D_LOGERRORF("Can't initialize recording with error %d", BASS_ErrorGetCode());
            BASS_RecordFree();
            BASS_Free();
            return false;
        }

#pragma warning(push, 0)
        rchan = BASS_RecordStart(SAMPLERATE, 2, MAKELONG(0, 10), RecordingCallback, &pause);
#pragma warning(pop)

        if(rchan == 0)
        {
            URHO3D_LOGERRORF("Can't initialize recording with error %d", BASS_ErrorGetCode());
            BASS_RecordFree();
            BASS_Free();
            return false;
        }
    }

    return true;
}


bool AudioCapturer::Init()
{
    BASS_INFO bi;

    BASS_SetConfig(BASS_CONFIG_VISTA_TRUEPOS, 0);       // Allosw lower latency on Vista and newer

    if(!BASS_Init(-1, SAMPLERATE, BASS_DEVICE_LATENCY, 0, NULL))
    {
        URHO3D_LOGERROR("Can't initialize audio output");
        return false;
    }

    BASS_GetInfo(&bi);

    chan = BASS_StreamCreate(SAMPLERATE, 2, 0, STREAMPROC_PUSH, 0);

    if(chan == 0)
    {
        URHO3D_LOGERRORF("Can't create stream with error code %d", BASS_ErrorGetCode());
        return false;
    }

    rate = SAMPLERATE;

    prebuf = (DWORD)BASS_ChannelSeconds2Bytes(chan, bi.minbuf / 1000.0f);  // prebuffer at least "minbuf" worth of data

    if(prebuf == (uint)-1)
    {
        URHO3D_LOGERRORF("Error %d", BASS_ErrorGetCode());
        return false;
    }

    return true;
}


void AudioCapturer::Stop()
{
    BASS_RecordFree();
    BASS_Free();
}


void AudioCapturer::Pause(bool pause_)
{
    this->pause = pause_;
}


bool AudioCapturer::InPause()
{
    return pause;
}

#endif

