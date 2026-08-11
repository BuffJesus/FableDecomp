#include <cstdio>
#include <new>

struct CActiveFile {
    void* m_buffer;
};

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self);

int main()
{
    CActiveFile a;
    a.m_buffer = operator new(64);
    CActiveFile_OnReadFinished(&a);
    if (a.m_buffer != 0) { printf("FAIL nonnull\n"); return 1; }

    // null case: must be a no-op, no crash
    CActiveFile b;
    b.m_buffer = 0;
    CActiveFile_OnReadFinished(&b);
    if (b.m_buffer != 0) { printf("FAIL nullcase\n"); return 1; }

    printf("ONREADFINISHED_OK\n");
    return 0;
}