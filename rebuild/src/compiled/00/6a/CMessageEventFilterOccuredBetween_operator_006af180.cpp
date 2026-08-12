struct CMessageEvent {
    int pad[13];
    int timestamp; // +0x34
};

struct CMessageEventFilter_OccuredBetween {
    int minTime;  // +0x00
    int maxTime;  // +0x04

    int operator()(CMessageEvent* ev);
};

int CMessageEventFilter_OccuredBetween::operator()(CMessageEvent* ev)
{
    int t = ev->timestamp;
    if (t > this->minTime && t <= this->maxTime)
        return 1;
    return 0;
}