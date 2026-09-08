#include "engine/CTCPerceiveThing.h"
#include "engine/CPerceivedThingDef.h"

void __fastcall CTCPerceiveThing_SetSightRadius(CTCPerceiveThing* perception,
                                                float radius)
{
    if (radius != -1.0f)
    {
        perception->ExtendedSightRadius = radius;
        return;
    }

    const CPerceivedThingDef* definition =
        reinterpret_cast<const CPerceivedThingDef*>(perception->PDef_Object);
    perception->ExtendedSightRadius = definition->ExtendedSightRadius;
    if (perception->ExtendedSightRadius < perception->SightRadius)
        perception->ExtendedSightRadius = perception->SightRadius;
}