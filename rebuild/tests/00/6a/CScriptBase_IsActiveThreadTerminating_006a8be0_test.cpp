#include <cstdio>

struct SubThread {
    char pad[0xc];
    bool terminating;
};

struct CScriptBase {
    char pad0[0x2c];
    unsigned int flags;
    char pad1[0x5c - 0x2c - 4];
    SubThread* activeThread;
};

bool __stdcall IsActiveThreadTerminating(CScriptBase* self);

int main()
{
    SubThread st;
    st.terminating = true;

    CScriptBase a;
    a.flags = 8;
    a.activeThread = &st;

    CScriptBase b;
    b.flags = 0;          // bit 8 clear -> false
    b.activeThread = &st;

    CScriptBase c;
    c.flags = 8;
    SubThread st2;
    st2.terminating = false;
    c.activeThread = &st2;

    bool ok = IsActiveThreadTerminating(&a) == true
           && IsActiveThreadTerminating(&b) == false
           && IsActiveThreadTerminating(&c) == false;

    if (ok) printf("006a8be0_TEST PASS\n");
    else    printf("FAIL\n");
    return 0;
}