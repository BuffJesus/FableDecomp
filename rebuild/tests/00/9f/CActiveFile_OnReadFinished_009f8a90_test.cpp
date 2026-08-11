#include <cstdio>

static int g_member_called = 0;
static void* g_freed = 0;

struct CActiveFile
{
    void* m_buffer;   // +0x00
    void OnReadFinished();
};

// Provide the two externs the source references.
void __fastcall sub_9f89b0(void* self)   { (void)self; g_member_called++; }
void __cdecl   sub_bfea14(void* p)        { g_freed = p; }

void CActiveFile::OnReadFinished()
{
    sub_9f89b0(this);
    if (this->m_buffer)
        sub_bfea14(this->m_buffer);
}

int main()
{
    int ok = 1;

    // Case 1: buffer non-null -> member called + freed.
    int dummy = 0x1234;
    CActiveFile a;
    a.m_buffer = &dummy;
    g_member_called = 0; g_freed = 0;
    a.OnReadFinished();
    if (g_member_called != 1) ok = 0;
    if (g_freed != &dummy)    ok = 0;

    // Case 2: buffer null -> member called, nothing freed.
    CActiveFile b;
    b.m_buffer = 0;
    g_member_called = 0; g_freed = (void*)-1;
    b.OnReadFinished();
    if (g_member_called != 1) ok = 0;
    if (g_freed != (void*)-1) ok = 0;

    printf(ok ? "ONREADFINISHED_OK\n" : "ONREADFINISHED_FAIL\n");
    return ok ? 0 : 1;
}