struct CTCHeroExperience { char pad[0xd8]; int value; };
extern "C" void __cdecl helper_prep(void);
extern "C" int __cdecl helper_get(void);
void __fastcall CTCHeroExperience_AddSuccessfulBlockParry(CTCHeroExperience* self)
{
    helper_prep();
    self->value = helper_get();
}