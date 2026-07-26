#include <cstdio>

struct Node {
    void* f0;
    void* f4;
    void* f8;
    void* fc;
};

struct List {
    Node* head;
    void Erase(void* arg);
};

List g_obj;
int  g_flag = 0;

void* g_erase_arg = (void*)0xDEAD;
List* g_erase_this = 0;

void List::Erase(void* arg)
{
    g_erase_this = this;
    g_erase_arg = arg;
}

void __fastcall CGameScriptInterface_ClearActiveGossipCategories()
{
    if (g_flag != 0) {
        Node* p = g_obj.head;
        g_obj.Erase(p->f4);
        g_obj.head->f8 = g_obj.head;
        g_obj.head->f4 = 0;
        g_obj.head->fc = g_obj.head;
        g_flag = 0;
    }
}

int main()
{
    Node n;
    n.f0 = 0; n.f4 = (void*)0x1234; n.f8 = 0; n.fc = 0;
    g_obj.head = &n;

    g_flag = 0;
    CGameScriptInterface_ClearActiveGossipCategories();
    if (n.f4 != (void*)0x1234) { std::printf("FAIL flag0\n"); return 1; }

    g_flag = 1;
    CGameScriptInterface_ClearActiveGossipCategories();
    if (g_erase_arg != (void*)0x1234) { std::printf("FAIL arg\n"); return 1; }
    if (g_erase_this != &g_obj) { std::printf("FAIL this\n"); return 1; }
    if (n.f8 != &n) { std::printf("FAIL f8\n"); return 1; }
    if (n.f4 != 0) { std::printf("FAIL f4\n"); return 1; }
    if (n.fc != &n) { std::printf("FAIL fc\n"); return 1; }
    if (g_flag != 0) { std::printf("FAIL flag\n"); return 1; }

    std::printf("CGameScriptInterface_008a78e0_TEST PASS\n");
    return 0;
}