#include <cstdio>

// Behaviour model: Copy forwards base copy then member-at-0x28 copy.
struct BaseDef { int base; void Copy(const BaseDef* s){ base = s->base; } };
struct Member28 { int val; void Copy(const Member28* s){ val = s->val; } };

struct Def {
    BaseDef b;
    char pad[0x28 - sizeof(BaseDef)];
    Member28 m28;
    void Copy(const Def* src){
        b.Copy(&src->b);
        m28.Copy(&src->m28);
    }
};

int main(){
    Def a, c;
    a.b.base = 111; a.m28.val = 222;
    c.b.base = 0;   c.m28.val = 0;
    c.Copy(&a);
    if (c.b.base == 111 && c.m28.val == 222 &&
        ((char*)&c.m28 - (char*)&c) == 0x28) {
        printf("PARITY_OK\n");
    } else {
        printf("FAIL %d %d %ld\n", c.b.base, c.m28.val, (long)((char*)&c.m28 - (char*)&c));
    }
    return 0;
}