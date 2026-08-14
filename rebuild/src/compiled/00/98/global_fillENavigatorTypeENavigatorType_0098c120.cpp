typedef int ENavigatorType;

void __fastcall fill_ENavigatorType(ENavigatorType *dest, ENavigatorType *dest_end, ENavigatorType *val)
{
    for (; dest != dest_end; ++dest)
        *dest = *val;
}