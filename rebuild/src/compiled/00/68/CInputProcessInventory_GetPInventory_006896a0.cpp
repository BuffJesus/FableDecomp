int __fastcall CInputProcessInventory_GetPInventory(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 52);
}