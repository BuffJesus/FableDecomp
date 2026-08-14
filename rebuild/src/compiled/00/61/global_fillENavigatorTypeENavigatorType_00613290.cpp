// Byte-exact reconstruction of 0x00613290
// fill(ENavigatorType* first, ENavigatorType* last, const ENavigatorType* val)
// __fastcall: first->ecx, last->edx, val on stack [esp+4]
typedef int ENavigatorType;

void __fastcall fill_ENavigatorType(ENavigatorType* first, ENavigatorType* last, const ENavigatorType* val)
{
    for (; first != last; ++first)
        *first = *val;
}