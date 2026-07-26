double __fastcall DrawGetSeaLevel(const unsigned char* self)
{
    return *reinterpret_cast<const double*>(self + 40);
}