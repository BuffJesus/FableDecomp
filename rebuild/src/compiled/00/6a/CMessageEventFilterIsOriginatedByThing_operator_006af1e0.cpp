struct CMessageEvent {
    char pad[0x28];
    unsigned int originatorType;   // +0x28
    unsigned int originatorId;     // +0x2c
};

struct CMessageEventFilter_IsOriginatedByThing {
    unsigned int type;   // +0x00
    unsigned int id;     // +0x04
    bool operator()(CMessageEvent* ev);
};

bool CMessageEventFilter_IsOriginatedByThing::operator()(CMessageEvent* ev)
{
    return ev->originatorType == this->type
        && ev->originatorId == this->id;
}