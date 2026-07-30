#include <cstdio>

// Function under test: exact reconstruction of CInputTypeXboxPadButtonEvent::GetType
int __fastcall GetType(void *this_ptr)
{
    (void)this_ptr;
    return 2;
}

int main()
{
    // Build a dummy object; this=ecx is supplied by __fastcall calling convention.
    char obj[16];
    for (int i = 0; i < 16; ++i) obj[i] = (char)0xCD;

    int r = GetType(obj);
    if (r != 2) {
        std::printf("FAIL GetType returned %d expected 2\n", r);
        return 1;
    }

    std::printf("OK_0x00445620 GetType=%d\n", r);
    return 0;
}