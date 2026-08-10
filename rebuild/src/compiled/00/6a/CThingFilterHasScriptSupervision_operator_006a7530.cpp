struct CThing;
struct CThingFilter_HasScriptSupervision { bool m_wanted; };

extern "C" bool __fastcall Thing_IsUnderScriptSupervision(CThing* thing);

bool __fastcall CThingFilter_HasScriptSupervision__op(CThingFilter_HasScriptSupervision* self, void* edx, CThing* thing)
{
    return Thing_IsUnderScriptSupervision(thing) == self->m_wanted;
}