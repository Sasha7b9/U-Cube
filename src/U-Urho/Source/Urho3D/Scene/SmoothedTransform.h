//
// Copyright (c) 2008-2021 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

/// \file

#pragma once

#include "../Scene/Component.h"

namespace Urho3D
{

enum SmoothingType : unsigned
{
    /// No ongoing smoothing.
    SMOOTH_NONE = 0,
    /// Ongoing position smoothing.
    SMOOTH_POSITION = 1,
    /// Ongoing rotation smoothing.
    SMOOTH_ROTATION = 2,
};
URHO3D_FLAGSET(SmoothingType, SmoothingTypeFlags);

/// Transform smoothing component for network updates.
class URHO3D_API SmoothedTransform : public Component
{
    URHO3D_OBJECT(SmoothedTransform, Component);

public:
    /// Construct.
    explicit SmoothedTransform(Context* context);
    /// Destruct.
    ~SmoothedTransform() override;
    /// Register object factory.
    /// @nobind
    static void RegisterObject(Context* context);

    /// Update smoothing.
    void Update(float constant, float squaredSnapThreshold);
    /// Set target position in parent space.
    /// @property
    void SetTargetPosition(const Vector3& position);
    /// Set target rotation in parent space.
    /// @property
    void SetTargetRotation(const Quaternion& rotation);
    /// Set target position in world space.
    /// @property
    void SetTargetWorldPosition(const Vector3& position);
    /// Set target rotation in world space.
    /// @property
    void SetTargetWorldRotation(const Quaternion& rotation);

    /// Return target position in parent space.
    /// @property
    const Vector3& GetTargetPosition() const { return targetPosition_; }

    /// Return target rotation in parent space.
    /// @property
    const Quaternion& GetTargetRotation() const { return targetRotation_; }

    /// Return target position in world space.
    /// @property
    Vector3 GetTargetWorldPosition() const;
    /// Return target rotation in world space.
    /// @property
    Quaternion GetTargetWorldRotation() const;

    /// Return whether smoothing is in progress.
    /// @property
    bool IsInProgress() const { return smoothingMask_ != SMOOTH_NONE; }

protected:
    /// Handle scene node being assigned at creation.
    void OnNodeSet(Node* node) override;

private:
    /// Handle smoothing update event.
    void HandleUpdateSmoothing(StringHash eventType, VariantMap& eventData);

    /// Target position.
    Vector3 targetPosition_;
    /// Target rotation.
    Quaternion targetRotation_;
    /// Active smoothing operations bitmask.
    SmoothingTypeFlags smoothingMask_;
    /// Subscribed to smoothing update event flag.
    bool subscribed_;
};

}