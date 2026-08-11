#include <cstdio>

static void* g_dispatch_obj = 0;
static void* g_dispatch_arg = 0;
static int   g_dispatch_calls = 0;
static void* g_freed = 0;
static int   g_free_calls = 0;

void __fastcall ActiveFile_Dispatch(void* obj, void* arg)
{
    g_dispatch_obj = obj;
    g_dispatch_arg = arg;
    g_dispatch_calls++;
}

void ActiveFile_Free(void* p)
{
    g_freed = p;
    g_free_calls++;
}

struct CActiveFile
{
    void* field0;
    void* field4;
    void OnReadFinished()
    {
        ActiveFile_Dispatch(this->field0, this->field4);
        if (this->field0)
            ActiveFile_Free(this->field0);
    }
};

int main()
{
    int dummyObj = 111;
    int dummyArg = 222;

    // Case 1: non-null field0 -> dispatch + free called
    CActiveFile a;
    a.field0 = &dummyObj;
    a.field4 = &dummyArg;
    a.OnReadFinished();

    bool ok1 = (g_dispatch_calls == 1) &&
               (g_dispatch_obj == &dummyObj) &&
               (g_dispatch_arg == &dummyArg) &&
               (g_free_calls == 1) &&
               (g_freed == &dummyObj);

    // Case 2: null field0 -> dispatch called, free NOT called
    g_dispatch_calls = 0; g_free_calls = 0;
    g_dispatch_obj = 0; g_dispatch_arg = 0; g_freed = 0;
    CActiveFile b;
    b.field0 = 0;
    b.field4 = &dummyArg;
    b.OnReadFinished();

    bool ok2 = (g_dispatch_calls == 1) &&
               (g_dispatch_obj == 0) &&
               (g_dispatch_arg == &dummyArg) &&
               (g_free_calls == 0);

    if (ok1 && ok2)
    {
        printf("ONREADFINISHED_OK\n");
        return 0;
    }
    printf("FAIL o1=%d o2=%d\n", (int)ok1, (int)ok2);
    return 1;
}