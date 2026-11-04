#pragma once
#include <stddef.h>
#include "fable_display_geometry.h"
#include "fable_2d_vector.h"

// Ego_r CEngineCameraDesc; retail SetCameraState consumes the same 0x6C layout.
struct CEngineCameraDesc {
    C2DBoxF Window;
    float RotationMatrix[12]; // CMatrix3x4: E11,E12,E13,...,E41,E42,E43.
    C3DVector WorldPosition;
    float HorizontalFOV,VerticalFOV;
    bool Use2DFOV;
    float NearZPlane,FarZPlane,ZBufferStart,ZBufferEnd;
    bool StateValid;
};
FABLE_STATIC_ASSERT(sizeof(CEngineCameraDesc)==0x6c);
FABLE_STATIC_ASSERT(offsetof(CEngineCameraDesc,WorldPosition)==0x40);
FABLE_STATIC_ASSERT(offsetof(CEngineCameraDesc,NearZPlane)==0x58);
extern const float ZBUFFER_PARTITION_MAIN_SCENE_START; // retail 01399D44
