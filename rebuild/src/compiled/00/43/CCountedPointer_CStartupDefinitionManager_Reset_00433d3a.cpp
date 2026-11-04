#pragma optimize("s", on)
// CCountedPointer<CStartupDefinitionManager>::Reset(CStartupDefinitionManager*)
// @ 0x00433D3A and ::ReleaseData() @ 0x00433CC0 (per-T COMDATs of the /Os
// startup-definition unit; Reset is called by CStartupDefinitionManager::Set
// 0x004336B0, DeleteData is 0x00433CB5).
#include "fable_counted_pointer.h"
#include "fable_definition_manager.h"

// Drop this owner's reference (deleting the object with its last owner) and
// leave the pointer empty.
template<typename T>
void CCountedPointer<T>::ReleaseData()
{
    if (Info)
    {
        --Info->RefCount;
        if (Info->RefCount == 0)
        {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
    Data = 0;
    Info = 0;
}

// Release the current object, then take sole ownership of the new one.
template<typename T>
void CCountedPointer<T>::Reset(T* data)
{
    ReleaseData();
    Data = data;
    if (Data)
    {
        Info = new CCPPointerInfo(&DeleteData, (void*)Data);
    }
}

template class CCountedPointer<CStartupDefinitionManager>;
