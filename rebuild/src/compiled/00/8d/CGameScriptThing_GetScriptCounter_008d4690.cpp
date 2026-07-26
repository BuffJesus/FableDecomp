int __fastcall CGameScriptThing_GetScriptCounter(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 20);
}