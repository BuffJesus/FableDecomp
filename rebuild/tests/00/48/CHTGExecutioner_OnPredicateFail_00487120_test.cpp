#include <stdio.h>

struct Inner {
    char pad[0x79c];
    unsigned char flag;
};
struct CHTG_Executioner {
    char pad0[0xc];
    Inner* inner;
    int lastN;
};

static void helper_model(CHTG_Executioner* self, int n){ self->lastN = n; }

static void OnPredicateFail_model(CHTG_Executioner* self)
{
    helper_model(self, 0x11);
    self->inner->flag = 0;
}

int main()
{
    Inner node; node.flag = 0xAB;
    CHTG_Executioner obj; obj.inner = &node; obj.lastN = -1;
    OnPredicateFail_model(&obj);
    if (obj.lastN != 0x11) { printf("FAIL n=%d\n", obj.lastN); return 1; }
    if (node.flag != 0) { printf("FAIL flag=%d\n", node.flag); return 1; }
    node.flag = 5;
    OnPredicateFail_model(&obj);
    if (node.flag != 0) { printf("FAIL flag2=%d\n", node.flag); return 1; }
    printf("OK_00487120\n");
    return 0;
}