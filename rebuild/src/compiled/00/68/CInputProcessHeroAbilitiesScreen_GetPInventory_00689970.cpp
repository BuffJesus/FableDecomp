int __fastcall CInputProcessHeroAbilitiesScreen_GetPInventory(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 52);
}