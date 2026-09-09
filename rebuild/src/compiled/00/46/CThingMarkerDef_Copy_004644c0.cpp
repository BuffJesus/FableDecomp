#pragma optimize("s", on)
#include "engine/CThingMarkerDefRetail.h"

void CThingMarkerDef::Copy(const CDefClassBase* source)
{
    const CThingMarkerDef* marker = static_cast<const CThingMarkerDef*>(source);
    CThingBaseDefRetail::CopyBase(source);
    Graphic = marker->Graphic;
    TriggerType = marker->TriggerType;
    Properties = marker->Properties;
}