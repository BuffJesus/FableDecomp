#include <cstdio>

struct C2DVector {
    float x;
    float y;
    C2DVector() {}
    C2DVector(const C2DVector& o) : x(o.x), y(o.y) {}
};

struct Navigator {
    char pad0[0x0c];
    C2DVector pos;
};

struct NavHolder {
    char pad0[0x60];
    Navigator* nav;
};

struct CGroundNavigatorExternalInterface {
    char pad0[4];
    NavHolder* holder;
    C2DVector PeekNavigatorPosition2D();
};

C2DVector CGroundNavigatorExternalInterface::PeekNavigatorPosition2D()
{
    return this->holder->nav->pos;
}

int main()
{
    Navigator nav;
    nav.pos.x = 12.5f;
    nav.pos.y = -7.25f;

    NavHolder holder;
    holder.nav = &nav;

    CGroundNavigatorExternalInterface iface;
    iface.holder = &holder;

    C2DVector r = iface.PeekNavigatorPosition2D();

    if (r.x == 12.5f && r.y == -7.25f) {
        printf("PEEK_NAV2D_OK\n");
        return 0;
    }
    printf("FAIL x=%f y=%f\n", r.x, r.y);
    return 1;
}