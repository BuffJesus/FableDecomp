#include "rebuild_abi.h"
#include <cstdio>

struct Obj_00428e0a {
    void* field0;
};

extern Obj_00428e0a* __fastcall sub_00428e0a(Obj_00428e0a* thisptr, int edx, void* value);

int main()
{
    Obj_00428e0a o;
    o.field0 = 0;
    int marker = 0x1234;
    Obj_00428e0a* ret = sub_00428e0a(&o, 0, &marker);
    if (ret == &o && o.field0 == (void*)&marker) {
        printf("OK_428e0a_STORE\n");
    } else {
        printf("FAIL_428e0a\n");
    }
    return 0;
}