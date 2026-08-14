#include <stdio.h>

struct CMessageEvent {
    char pad[0x28];
    unsigned int originatorType;   // +0x28
    unsigned int originatorId;     // +0x2c
};

struct Filter {
    unsigned int type;   // +0x00
    unsigned int id;     // +0x04
    bool operator()(CMessageEvent* ev);
};

bool Filter::operator()(CMessageEvent* ev)
{
    if (ev->originatorType != this->type)
        return false;
    if (ev->originatorId != this->id)
        return false;
    return true;
}

int main()
{
    Filter f; f.type = 7; f.id = 42;
    CMessageEvent e;
    e.originatorType = 7; e.originatorId = 42;
    bool match = f(&e);

    e.originatorType = 8; e.originatorId = 42;
    bool nomatch1 = f(&e);

    e.originatorType = 7; e.originatorId = 99;
    bool nomatch2 = f(&e);

    if (match && !nomatch1 && !nomatch2)
        printf("PARITY_OK\n");
    else
        printf("PARITY_FAIL\n");
    return 0;
}