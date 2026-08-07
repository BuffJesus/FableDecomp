extern char* g_localseed_base;
unsigned long* GetPLocalSeed()
{
    return reinterpret_cast<unsigned long*>(*reinterpret_cast<char**>(g_localseed_base + 0x1c) + 0x16140);
}