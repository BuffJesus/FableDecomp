struct Helper {
    void Do(const char* s, int n);
};
struct CCharString { char* p; };
extern "C" CCharString* __fastcall GetActionName_9(Helper* self)
{
    self->Do((const char*)0x123e6ec, -1);
    return (CCharString*)self;
}