int __fastcall CMemoryDataOutputStream_GetPosition(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 20);
}