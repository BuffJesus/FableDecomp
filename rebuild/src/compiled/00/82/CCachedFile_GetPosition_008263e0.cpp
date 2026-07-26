int __fastcall CCachedFile_GetPosition(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 4);
}