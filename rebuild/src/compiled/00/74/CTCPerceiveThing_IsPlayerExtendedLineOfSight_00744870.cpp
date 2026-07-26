struct CSub {
	unsigned char pad[0x10];
	unsigned char kind; // +0x10
};

struct CTCPerceiveThing {
	unsigned char pad0[0xc];
	unsigned char sub[0x22];
	unsigned char flag; // +0x2e
};

extern CSub* __fastcall Sub_Get(void* self);

int __fastcall CTCPerceiveThing_IsPlayerExtendedLineOfSight(CTCPerceiveThing* self)
{
	void* sub = (char*)self + 0xc;
	if (Sub_Get(sub) != 0) {
		if (Sub_Get(sub)->kind == 2) {
			if (self->flag != 0) {
				return 1;
			}
		}
	}
	return 0;
}