#include <cstdio>
typedef unsigned long ulong;

// External helper (defined here so the test links standalone).
unsigned long __fastcall ListBoxDataHelper(ulong a, ulong b, ulong c)
{
    // return the third argument so we can observe it flow into m_entry
    return a * 0 + b * 0 + c;
}

struct CGuiControlListBox
{
    void *m_vtbl;      // +0
    ulong m_entry;     // +4

    ulong SelectEntryFromData(ulong arg1, ulong arg2);
};

int main()
{
    CGuiControlListBox box;
    box.m_vtbl = 0;
    box.m_entry = 0x11111111u;

    ulong r = box.SelectEntryFromData(0x42u, 0x99u);

    bool ok = (r == 0x42u) && (box.m_entry == 0x42u);
    if (ok)
        printf("LISTBOX_SELECT_OK\n");
    else
        printf("FAIL r=%lu entry=%lu\n", r, box.m_entry);
    return ok ? 0 : 1;
}