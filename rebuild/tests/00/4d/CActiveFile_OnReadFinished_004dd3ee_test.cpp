#include <stdio.h>

static int g_helper_called = 0;
static void* g_deleted = (void*)0;

struct CActiveFile { void* m_ptr; };

void Helper(CActiveFile* self) { g_helper_called++; (void)self; }
void OpDelete(void* p) { g_deleted = p; }

void OnReadFinished_9_model(CActiveFile* self)
{
    Helper(self);
    void* p = self->m_ptr;
    if (p) OpDelete(p);
}

int main()
{
    // non-null member -> delete called
    int dummy = 5;
    CActiveFile a; a.m_ptr = &dummy;
    g_helper_called = 0; g_deleted = (void*)0;
    OnReadFinished_9_model(&a);
    if (g_helper_called != 1) { printf("FAIL helper\n"); return 1; }
    if (g_deleted != &dummy) { printf("FAIL delete\n"); return 1; }

    // null member -> delete skipped
    CActiveFile b; b.m_ptr = (void*)0;
    g_helper_called = 0; g_deleted = (void*)1;
    OnReadFinished_9_model(&b);
    if (g_helper_called != 1) { printf("FAIL helper2\n"); return 1; }
    if (g_deleted != (void*)1) { printf("FAIL skip\n"); return 1; }

    printf("OK_004dd3ee\n");
    return 0;
}