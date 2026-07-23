struct Sub { char pad[0x9c]; char flag; };
struct Owner { char pad[0x24]; Sub* sub; };
extern Sub* __fastcall GetSub(Owner* o);

char __fastcall CTCCombat_IsAbleToBeKnockedDown(char* self)
{
    Sub* s = GetSub(*(Owner**)(self + 0x24));
    return s->flag != 0;
}