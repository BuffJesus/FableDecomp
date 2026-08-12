#include <cstdio>

struct BoastList {
    int data;
    void Clear() { data = 0; }
};

static int g_default = 0;
int MakeDefault() { return g_default; }

struct CQuestManager {
    char pad[0x64];
    int  field64;
    BoastList a;
    BoastList b;
    void RemoveAllBoasts();
};

void CQuestManager::RemoveAllBoasts()
{
    this->a.Clear();
    this->b.Clear();
    this->field64 = MakeDefault();
}

int main()
{
    CQuestManager q;
    q.field64 = 0x11223344;
    q.a.data  = 7;
    q.b.data  = 9;
    g_default = 0x55;
    q.RemoveAllBoasts();
    if (q.a.data == 0 && q.b.data == 0 && q.field64 == 0x55) {
        std::printf("BOASTS_CLEARED_OK\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}