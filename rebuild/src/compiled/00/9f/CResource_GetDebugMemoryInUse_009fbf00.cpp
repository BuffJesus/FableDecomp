int __fastcall CResource_GetDebugMemoryInUse(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 20);
}