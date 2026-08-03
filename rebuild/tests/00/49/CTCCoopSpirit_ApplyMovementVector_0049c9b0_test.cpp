#include <stdio.h>

struct C3DVector { float x, y, z; };

struct CTCCoopSpirit {
    char pad[0x154];
    C3DVector movement;
};

void __fastcall ApplyMovementVector(CTCCoopSpirit* self, int /*edx_unused*/, const C3DVector* v)
{
    self->movement = *v;
}

int main()
{
    CTCCoopSpirit obj;
    obj.movement.x = -1.0f; obj.movement.y = -1.0f; obj.movement.z = -1.0f;

    C3DVector vec;
    vec.x = 3.5f; vec.y = -12.25f; vec.z = 100.0f;

    void* pobj = &obj;
    void* pvec = &vec;
    // __fastcall shim: this=ecx, one stack arg, callee cleans (ret 4).
    __asm {
        mov  ecx, pobj
        push pvec
        call ApplyMovementVector
    }

    char* base = (char*)&obj;
    C3DVector* at = (C3DVector*)(base + 0x154);
    if (obj.movement.x == 3.5f && obj.movement.y == -12.25f && obj.movement.z == 100.0f &&
        at->x == 3.5f && at->y == -12.25f && at->z == 100.0f) {
        printf("OK_0x0049c9b0\n");
        return 0;
    }
    printf("FAIL x=%f y=%f z=%f\n", obj.movement.x, obj.movement.y, obj.movement.z);
    return 1;
}