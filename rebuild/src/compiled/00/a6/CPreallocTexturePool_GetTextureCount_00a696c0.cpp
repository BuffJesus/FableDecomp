int __fastcall CPreallocTexturePool_GetTextureCount(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 60);
}