struct AstmMap { void* lb(int* pKey); };
struct AstmTarget { void d(void* a0, void* a1, void* a2, int z, int one); };
extern void* __fastcall ASTM_Helper_A(void* p_ecx);
extern void* __fastcall ASTM_Helper_B(void* p_ecx);
void __fastcall CGameScriptInterface_AddScreenTitleMessage(
    void* self_ecx, unsigned long, void* a0, void* a1, void* volatile a2)
{
    void* chain = *(void**)((char*)self_ecx + 0x14);
    void* a = ASTM_Helper_A(chain);
    unsigned char* obj = (unsigned char*)ASTM_Helper_B(a);
    if (obj == 0) return;
    if (*(unsigned char*)(obj + 0x91) & 0x01) return;
    AstmTarget* target;
    if (*(unsigned int*)(obj + 0x30) & 0x4000) {
        int key = 0x8e;
        AstmMap* map = (AstmMap*)(obj + 0x44);
        void* node = map->lb(&key);
        void* end = *(void**)((char*)map + 0x4);
        void* pick;
        if (node == end) pick = end;
        else if (*(int*)node > 0x8e) pick = end;
        else pick = node;
        target = *(AstmTarget**)((char*)pick + 0x4);
    } else target = (AstmTarget*)a2;
    if (target == 0) return;
    target->d(a0, a1, a2, 0, 1);
}