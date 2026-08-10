struct CScriptModuleSelfTerminateBase {
    char pad[0x50];
    int m_threshold; /* +0x50 */
};

extern "C" int __cdecl GetElapsed();

bool __fastcall IsFinished(CScriptModuleSelfTerminateBase* self)
{
    return GetElapsed() > self->m_threshold;
}