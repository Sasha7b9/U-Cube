#pragma once



class Math
{
public:
    // ���������� 1, ���� �������� ������ ����, -1, ���� ������, � 0, ���� val == 0
    template <class T> static int Sign(T val)
    {
        if(val == T(0))
        {
             return 0;
        }

        return val < 0 ? -1 : 1;
    }
};
