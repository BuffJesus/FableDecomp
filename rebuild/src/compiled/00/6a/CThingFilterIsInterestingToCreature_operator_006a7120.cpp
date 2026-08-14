struct CThing {
    char pad[0x10];
    unsigned char kind; // +0x10
};

struct CThingFilter_IsInterestingToCreature {
    CThing* target; // +0x00
    int operator()(CThing* t);
};

int CThingFilter_IsInterestingToCreature::operator()(CThing* t)
{
    if (t != this->target)
    {
        unsigned char k = t->kind;
        if (k == 1 || k == 2)
            return 1;
    }
    return 0;
}

typedef int (CThingFilter_IsInterestingToCreature::*mfp)(CThing*);
mfp force_emit = &CThingFilter_IsInterestingToCreature::operator();