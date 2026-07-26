#include <cstdio>

void* g_last_field = 0;
void* g_last_other = 0;
struct CTransferObject {
    void Accept(void* field) {
        g_last_other = (void*)this;
        g_last_field = field;
    }
};

struct CHairCardDef {
    char pad[0x28];
    int field28;
};

void __fastcall CHairCardDef_Transfer(CHairCardDef* self, int /*edx*/, CTransferObject* other) {
    other->Accept(&self->field28);
}

int main() {
    CHairCardDef def;
    def.field28 = 1234;
    CTransferObject other;
    CHairCardDef_Transfer(&def, 0, &other);
    if (g_last_other == (void*)&other && g_last_field == (void*)&def.field28) {
        std::printf("CHairCardDef_00452b04_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL other=%p field=%p\n", g_last_other, g_last_field);
    return 1;
}