int __fastcall CWorldMap_GetGranularity(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 80);
}