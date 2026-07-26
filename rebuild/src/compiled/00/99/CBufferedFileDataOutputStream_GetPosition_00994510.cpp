int __fastcall CBufferedFileDataOutputStream_GetPosition(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 32);
}