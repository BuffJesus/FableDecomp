// CBaseIntelligentPointer::ClearData/GetPItem/SetPItem @ A01B20/A01B50/A01B90.
// FableWin methods and CBaseObjectPointer layout; real compiler inlining
// reproduces all 223 retail bytes, including expired-target refcount cleanup.
#include "fable_intelligent_pointer.h"

void CBaseIntelligentPointer::ClearData() const
{
    if (PData)
    {
        --PData->refcount;
        if (PData->refcount == 0 && PData->object == 0)
            delete PData;
        PData = 0;
    }
}

CBaseObject* CBaseIntelligentPointer::GetPItem() const
{
    if (PData)
    {
        if (PData->object)
            return PData->object;
        ClearData();
    }
    return 0;
}

void CBaseIntelligentPointer::SetPItem(CBaseObject* item)
{
    if (PData && PData->object == item && item)
        return;
    ClearData();
    if (item)
    {
        if (item->IntelligentPointer)
        {
            PData = item->IntelligentPointer;
            ++PData->refcount;
        }
        else
        {
            PData = new CBaseObjectPointer;
            PData->object = item;
            PData->refcount = 1;
            item->IntelligentPointer = PData;
        }
    }
}

CBaseIntelligentPointer::CBaseIntelligentPointer() : PData(0)
{
}

CBaseIntelligentPointer::~CBaseIntelligentPointer()
{
    ClearData();
}
