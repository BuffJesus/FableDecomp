enum ENavigatorType { ENAV_0 };

void __fastcall fill_ENavigatorType(ENavigatorType* first, ENavigatorType* last, ENavigatorType* val)
{
    for (; first != last; ++first)
        *first = *val;
}