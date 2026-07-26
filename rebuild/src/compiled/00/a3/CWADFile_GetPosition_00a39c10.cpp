int __fastcall CWADFile_GetPosition(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 12);
}