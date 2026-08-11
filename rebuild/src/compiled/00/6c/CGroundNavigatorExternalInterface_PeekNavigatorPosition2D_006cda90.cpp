struct C2DVector {
    float x;
    float y;
    C2DVector() {}
    C2DVector(const C2DVector& o) : x(o.x), y(o.y) {}
};

struct Navigator {
    char pad0[0x0c];   // 0x00
    C2DVector pos;     // 0x0c
};

struct NavHolder {
    char pad0[0x60];   // 0x00
    Navigator* nav;    // 0x60
};

struct CGroundNavigatorExternalInterface {
    char pad0[4];      // 0x00
    NavHolder* holder; // 0x04
    C2DVector PeekNavigatorPosition2D();
};

C2DVector CGroundNavigatorExternalInterface::PeekNavigatorPosition2D()
{
    return this->holder->nav->pos;
}