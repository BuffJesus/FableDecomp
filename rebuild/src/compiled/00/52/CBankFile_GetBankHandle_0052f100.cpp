struct CWideString { void* p; void ctor(const void* src); };
struct CMemoryFile { char pad[0x20]; char member[4]; };
CWideString* __fastcall CMemoryFile_GetPathname(const CMemoryFile* self, void* edx, CWideString* ret)
{
    ret->ctor((const char*)self + 0x20);
    return ret;
}