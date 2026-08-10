struct Arg {
    void Consume(void* p);
};

struct CGameScriptThing {
    char pad[0x24];
    char data;
};

void* __fastcall GetDataString(CGameScriptThing* self, int /*edx*/, void* arg)
{
    ((Arg*)arg)->Consume(&self->data);
    return arg;
}