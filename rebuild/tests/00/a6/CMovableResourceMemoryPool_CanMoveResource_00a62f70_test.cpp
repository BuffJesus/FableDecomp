#include <cstdio>

struct IMovableResource {
    virtual void v0() {}
    virtual void v1() {}
    virtual bool CanMove(long arg) { return false; }
};

bool __stdcall CanMoveResource_forward(IMovableResource *obj, long arg);

struct Impl : IMovableResource {
    long seen;
    Impl() : seen(-1) {}
    void v0() {}
    void v1() {}
    bool CanMove(long arg) { seen = arg; return arg > 10; }
};

int main()
{
    // null obj -> false
    bool r0 = CanMoveResource_forward(0, 99);
    // real obj, arg dispatched to slot 2
    Impl a;
    bool r1 = CanMoveResource_forward(&a, 42);
    bool r2 = CanMoveResource_forward(&a, 5);
    if (r0 == false && r1 == true && r2 == false && a.seen == 5) {
        printf("CANMOVE_OK\n");
        return 0;
    }
    printf("CANMOVE_FAIL r0=%d r1=%d r2=%d seen=%ld\n", r0, r1, r2, a.seen);
    return 1;
}