#include "fable_surface.h"

CDisplayManager::ECreateDeviceSuccess CDisplayManager::SetDisplayMode(
    const C2DExtentsI& dimensions, unsigned long bitDepth,
    ESurfaceMultisampleType multisample, unsigned long refreshRate)
{
    if (dimensions.X == BackBufferDimensionsStorage_[0] &&
        dimensions.Y == BackBufferDimensionsStorage_[1] &&
        bitDepth == reinterpret_cast<const CPixelFormat*>(&BackBufferFormatStorage_)->GetColourDepth() &&
        refreshRate == static_cast<unsigned long>(RefreshRate) && multisample == MultisampleType)
        return CREATE_DEVICE_SUCCESS;
    CCreateDeviceParameters parameters = LastDeviceCreationParameters;
    parameters.BackBufferDimensions = dimensions;
    if (parameters.BackBufferDimensions.X < 32) parameters.BackBufferDimensions.X = 32;
    if (parameters.BackBufferDimensions.Y < 32) parameters.BackBufferDimensions.Y = 32;
    parameters.BackBufferBitDepth = bitDepth;
    parameters.RefreshRate = refreshRate;
    parameters.MultisampleType = multisample;
    return ResetDevice(parameters);
}

CDisplayManager::ECreateDeviceSuccess CDisplayManager::SetExclusiveMode(bool exclusive)
{
    if (ExclusiveMode != exclusive)
    {
        CCreateDeviceParameters parameters = LastDeviceCreationParameters;
        parameters.ExclusiveMode = exclusive;
        return ResetDevice(parameters);
    }
    return CREATE_DEVICE_SUCCESS;
}
