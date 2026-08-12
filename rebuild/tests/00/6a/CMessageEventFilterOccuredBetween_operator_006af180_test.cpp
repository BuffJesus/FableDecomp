#include <cstdio>

struct CMessageEvent {
    int pad[13];
    int timestamp; // +0x34
};

struct CMessageEventFilter_OccuredBetween {
    int minTime;
    int maxTime;
    int operator()(CMessageEvent* ev);
};

int CMessageEventFilter_OccuredBetween::operator()(CMessageEvent* ev)
{
    int t = ev->timestamp;
    if (t > this->minTime && t <= this->maxTime)
        return 1;
    return 0;
}

int main()
{
    CMessageEventFilter_OccuredBetween f;
    f.minTime = 10;
    f.maxTime = 20;

    CMessageEvent ev;
    ev.timestamp = 10; int r1 = f(&ev); // == min -> false
    ev.timestamp = 11; int r2 = f(&ev); // min+1 -> true
    ev.timestamp = 20; int r3 = f(&ev); // == max -> true
    ev.timestamp = 21; int r4 = f(&ev); // max+1 -> false
    ev.timestamp = 5;  int r5 = f(&ev); // below -> false

    if (!r1 && r2 && r3 && !r4 && !r5) {
        printf("OCCURED_BETWEEN_OK\n");
        return 0;
    }
    printf("FAIL %d %d %d %d %d\n", r1, r2, r3, r4, r5);
    return 1;
}