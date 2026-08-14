enum ENavigatorType { ENT_A, ENT_B };

void __fastcall fill_ENavigatorType(ENavigatorType* first, ENavigatorType* last, const ENavigatorType* val)
{
    for (; first != last; ++first)
        *first = *val;
}