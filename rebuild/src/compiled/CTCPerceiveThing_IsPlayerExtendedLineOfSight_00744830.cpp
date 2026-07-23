struct CPerceiver;
struct CTarget { char pad[0x10]; unsigned char kind; };

extern "C" CTarget* __fastcall Perceiver_GetTarget(CPerceiver* p);

struct CTCPerceiveThing {
    char pad0[0xc];
    char sub[0x21];
    unsigned char flag;
};

int __fastcall CTCPerceiveThing_IsPlayerExtendedLineOfSight(CTCPerceiveThing* self)
{
    CPerceiver* sub = (CPerceiver*)(self->sub);
    CTarget* t = Perceiver_GetTarget(sub);
    if (t) {
        CTarget* t2 = Perceiver_GetTarget(sub);
        if (t2->kind == 2) {
            if (self->flag)
                return 1;
        }
    }
    return 0;
}