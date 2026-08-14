struct CMessageEvent {
    char pad[0x20];
    unsigned int thing0;
    unsigned int thing1;
};

struct CMessageEventFilter_IsOriginatedByThing {
    unsigned int a;
    unsigned int b;
    int operator()(CMessageEvent* ev)
    {
        if (ev->thing0 == this->a && ev->thing1 == this->b)
            return 1;
        return 0;
    }
};

int force_emit(CMessageEventFilter_IsOriginatedByThing* self, CMessageEvent* ev)
{
    return (*self)(ev);
}