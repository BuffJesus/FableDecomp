#include "engine/CTCSpotLight.h"

void __fastcall SetAngle(CTCSpotLight* self, float a)
{
    float* p = (float*)self->PLightInfo_Data;
    self->Overridden = true;
    *p = a;
}
