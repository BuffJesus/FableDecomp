#include "engine/CEngineCamera.h"
#include "fable_engine_camera_desc.h"
#include "fable_camera.h"
#include "fable_virtual_pixels.h"
void CEngineCamera::Process(bool force)
{
    if (DebugScriptRunning) {
        ProcessDebugScript();
        return;
    }
    CEngineCameraDesc desc;
    desc.NearZPlane=0.1f;
    desc.FarZPlane=4000.0f;
    desc.ZBufferStart=ZBUFFER_PARTITION_MAIN_SCENE_START;
    desc.ZBufferEnd=0.99f;
    desc.StateValid=true;
    if (ConsoleOverrideViewPortWidth>0) {
        desc.Window.Initialise(0.0f,0.0f,(float)ConsoleOverrideViewPortWidth,(float)ConsoleOverrideViewPortHeight);
    } else {
        GFVBoxToABox(GameWindow,&desc.Window);
    }
    float horizontal=GameCamera->GetHorizontalFOV()*360.0f;
    horizontal=horizontal*(1.0f/360.0f);
    desc.HorizontalFOV=horizontal*6.2831855f;
    if (GameCamera->GetFOVFlags()&2) {
        desc.Use2DFOV=true;
        float vertical=GameCamera->GetVerticalFOV()*360.0f;
        vertical=vertical*(1.0f/360.0f);
        desc.VerticalFOV=vertical*6.2831855f;
    } else {
        desc.Use2DFOV=false;
        desc.VerticalFOV=0.0f;
    }
    desc.WorldPosition=GameCamera->WorldPosition;
    CRightHandedSet orientation=GameCamera->RHSet;
    orientation.Normalise();
    C3DVector right=orientation.GetRight();
    right.AccurateNormalise();
    desc.RotationMatrix[0]=right.X;
    desc.RotationMatrix[1]=orientation.Up.X;
    desc.RotationMatrix[2]=orientation.Forward.X;
    desc.RotationMatrix[3]=right.Y;
    desc.RotationMatrix[4]=orientation.Up.Y;
    desc.RotationMatrix[5]=orientation.Forward.Y;
    desc.RotationMatrix[6]=right.Z;
    desc.RotationMatrix[7]=orientation.Up.Z;
    desc.RotationMatrix[8]=orientation.Forward.Z;
    desc.RotationMatrix[9]=0.0f;
    desc.RotationMatrix[10]=0.0f;
    desc.RotationMatrix[11]=0.0f;
    SetCameraState(desc,true,force);
}
