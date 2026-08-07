struct S { char pad[0x148]; long* p148; long GetAbilityLevel(long i); };
long S::GetAbilityLevel(long i){ return this->p148[i]; }