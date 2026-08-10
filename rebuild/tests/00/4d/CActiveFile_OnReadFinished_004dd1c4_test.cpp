
#include <cstdio>
struct CActiveFile { void* member0; };
static int g_helper_called = 0;
static void* g_deleted = (void*)0;
void __fastcall CActiveFile_helper(CActiveFile* self) { g_helper_called++; }
void __cdecl fable_delete(void* p) { g_deleted = p; }
void __fastcall OnReadFinished_3(CActiveFile* self)
{
    CActiveFile_helper(self);
    void* m = self->member0;
    if (m) fable_delete(m);
}
int main(){
    // non-null member -> delete called with member
    CActiveFile a; a.member0 = (void*)0x1234;
    g_helper_called=0; g_deleted=(void*)0;
    OnReadFinished_3(&a);
    if (g_helper_called!=1) { printf("FAIL helper\n"); return 1; }
    if (g_deleted != (void*)0x1234) { printf("FAIL delete\n"); return 1; }
    // null member -> delete NOT called
    CActiveFile b; b.member0 = (void*)0;
    g_helper_called=0; g_deleted=(void*)0xdead;
    OnReadFinished_3(&b);
    if (g_helper_called!=1) { printf("FAIL helper2\n"); return 1; }
    if (g_deleted != (void*)0xdead) { printf("FAIL nulldel\n"); return 1; }
    printf("OK_004dd1c4\n");
    return 0;
}