#include <cstdio>

static int g_helperCalls = 0;
static void* g_deleted = 0;

// local defs of the externs the source references
void operator_delete_ext(void* p) { g_deleted = p; }

struct CActiveFile {
    void* field0;
    void HelperReadFinished();
    void OnReadFinished();
};

void CActiveFile::HelperReadFinished() { g_helperCalls++; }

void CActiveFile::OnReadFinished()
{
    this->HelperReadFinished();
    void* p = this->field0;
    if (p) {
        operator_delete_ext(p);
    }
}

int main()
{
    int marker = 0;

    void* pmarker = &(marker);

    // case 1: non-null field -> helper called + delete called
    CActiveFile a;
    a.field0 = pmarker;
    g_helperCalls = 0; g_deleted = 0;
    a.OnReadFinished();
    bool ok1 = (g_helperCalls == 1) && (g_deleted == pmarker);

    // case 2: null field -> helper called, no delete
    CActiveFile b;
    b.field0 = 0;
    g_helperCalls = 0; g_deleted = (void*)1;
    b.OnReadFinished();
    bool ok2 = (g_helperCalls == 1) && (g_deleted == (void*)1);

    if (ok1 && ok2) {
        printf("ONREADFINISHED_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}