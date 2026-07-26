struct CTCHeroExperience { char pad[0xc]; int field_c; };
extern void First_Helper();
extern int Second_Helper();
void __fastcall CTCHeroExperience_AddSuccessfulBlockParry(CTCHeroExperience* self)
{
    First_Helper();
    self->field_c = Second_Helper();
}