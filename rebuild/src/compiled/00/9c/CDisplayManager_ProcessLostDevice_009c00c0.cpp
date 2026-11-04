#include "fable_display_viewport.h"

bool CDisplayManager::ProcessLostDevice()
{
    const HRESULT cooperative=D3DDevice->TestCooperativeLevel();
    if (cooperative!=D3DERR_DEVICELOST)
    {
        if (cooperative==0) return true;
        // D3DERR_DEVICENOTRESET, from d3d9.h.
        if (cooperative==static_cast<HRESULT>(0x88760869L))
        {
            ECreateDeviceSuccess result=ResetDevice(LastDeviceCreationParameters);
            if (result==CREATE_DEVICE_DEVICE_LOST) return false;
            if (result==CREATE_DEVICE_SUCCESS) return true;
            CCreateDeviceParameters parameters=LastDeviceCreationParameters;
            if (parameters.MultisampleType!=SURFACE_MULTISAMPLE_NONE)
            {
                parameters.MultisampleType=SURFACE_MULTISAMPLE_NONE;
                result=ResetDevice(parameters);
                if (result==CREATE_DEVICE_DEVICE_LOST) return false;
                if (result==CREATE_DEVICE_SUCCESS) return true;
            }
            if (parameters.BackBufferDimensions.X>1024) parameters.BackBufferDimensions.X=1024;
            if (parameters.BackBufferDimensions.Y>768) parameters.BackBufferDimensions.Y=768;
            result=ResetDevice(parameters);
            if (result==CREATE_DEVICE_DEVICE_LOST) return false;
            return result==CREATE_DEVICE_SUCCESS;
        }
    }
    return false;
}
