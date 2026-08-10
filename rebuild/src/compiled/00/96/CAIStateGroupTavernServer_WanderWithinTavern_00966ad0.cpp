#pragma optimize("s",on)
struct Sub { int x; };
extern "C" int __fastcall Sub_Get(Sub* s);
struct TavernServer {
    char pad[0x34];
    Sub sub;
    int Act(int a, int b);
};
extern "C" int __fastcall WanderWithinTavern(TavernServer* self)
{
    int r = Sub_Get(&self->sub);
    return self->Act(3, r);
}