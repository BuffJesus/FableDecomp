// Reconstruction of std::fill<ENavigatorType*> at 0x00c13a80
// __fastcall: ecx = first, edx = last, [esp+4] = pointer to value

enum ENavigatorType { NAV_A, NAV_B };

void __fastcall fill_ENavigatorType(ENavigatorType *first, ENavigatorType *last, ENavigatorType *value)
{
    for (; first != last; ++first)
        *first = *value;
}