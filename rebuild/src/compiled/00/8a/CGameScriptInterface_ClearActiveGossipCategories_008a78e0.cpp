struct Node {
    void* f0;
    void* f4;
    void* f8;
    void* fc;
};

struct List {
    Node* head;             // this object lives at 0x13bae2c
    void Erase(void* arg);  // __fastcall: this in ecx, arg on stack
};

extern List g_obj;      // at 0x13bae2c
extern int  g_flag;     // at 0x13bae30

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