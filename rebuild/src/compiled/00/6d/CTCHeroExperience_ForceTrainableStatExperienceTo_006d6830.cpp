struct CTCHeroExperience {
    char pad[0x18];
    long* stats;   /* +0x18 */
};

/* Retail is __fastcall: this in ecx, category and value both on the stack.
   The harness rewrites __fastcall->__fastcall; an unused edx parameter keeps
   category/value on the stack (esp+4/esp+8) and yields ret 8, matching retail. */
void __fastcall ForceTrainableStatExperienceTo(CTCHeroExperience* self, int edxpad, int category, long value)
{
    if (value < 0)
        value = 0;
    self->stats[category] = value;
}