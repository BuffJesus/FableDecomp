struct CCard {
    void* Thing; long Name; long Value;
    float InitialRotation; float FinalRotation; float TurnSpeed; float TurnFraction;
    float MoveSpeed; float MoveFraction;
    bool Turning; bool FaceDown; bool Moving;
    unsigned char Tail[49];
    void Turn();
};

void CCard::Turn()
{
    const bool wasFaceDown = FaceDown;
    Turning = true;
    TurnFraction = 0.0f;
    if (wasFaceDown == true) {
        InitialRotation = 0.5f;
        FinalRotation = 0.0f;
    } else {
        InitialRotation = 0.0f;
        FinalRotation = 0.5f;
    }
    FaceDown = !wasFaceDown;
}