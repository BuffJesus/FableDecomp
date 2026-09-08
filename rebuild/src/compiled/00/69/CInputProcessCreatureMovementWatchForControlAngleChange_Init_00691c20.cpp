struct C2DVector {
    float X;
    float Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct CInputProcessCreatureMovementWatchForControlAngleChange {
    unsigned char Reserved[0x28];
    C2DVector LastJoyDirection;
    void Init();
};
void CInputProcessCreatureMovementWatchForControlAngleChange::Init()
{
    LastJoyDirection = C2DVector(0.0f, 0.0f);
}