int __fastcall CResource_GetUnloadableMemory(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 20);
}