// CCountedPointer<CWaveFile>::Reset, retail 0x00AADFF0: the per-T COMDAT that
// CWaveLoader::LoadWaveFile (0x00AAD650) calls.
#include "fable_wave_file.h"

template<typename T>
void CCountedPointer<T>::Reset(T* data)
{
    Release();
    Info = 0;
    Data = data;
    if (Data)
        Info = new CCPPointerInfo(&DeleteData, Data);
}

template void CCountedPointer<CWaveFile>::Reset(CWaveFile* data);
