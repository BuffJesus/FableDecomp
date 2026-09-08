struct CThing { void SetForceFrameUpdate(bool enabled); };

inline const long& Minimum(const long& left, const long& right)
{
    return left < right ? left : right;
}

struct CTCExplodingObject {
    void* __vftable;
    CThing* Thing;
    unsigned char Reserved[0x10];
    long CountdownTimer;
    void SetCountdownTimer(long frames);
};

void CTCExplodingObject::SetCountdownTimer(long frames)
{
    Thing->SetForceFrameUpdate(true);
    if (CountdownTimer > 0)
        CountdownTimer = Minimum(CountdownTimer, frames);
    else
        CountdownTimer = frames;
}