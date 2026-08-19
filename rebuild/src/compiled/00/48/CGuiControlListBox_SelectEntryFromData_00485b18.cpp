#pragma optimize("s",on)
typedef unsigned long ulong;

// External helper at 0x405ba0: __fastcall(ecx=first, edx=second, stack=third)
unsigned long __fastcall ListBoxDataHelper(ulong a, ulong b, ulong c);

struct CGuiControlListBox
{
    void *m_vtbl;      // +0
    ulong m_entry;     // +4

    ulong SelectEntryFromData(ulong arg1, ulong arg2);
};

// this comes in ecx (__fastcall)
ulong CGuiControlListBox::SelectEntryFromData(ulong arg1, ulong arg2)
{
    this->m_entry = ListBoxDataHelper(arg2, this->m_entry, arg1);
    return arg1;
}