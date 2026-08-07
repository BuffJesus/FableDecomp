struct S { char pad[272]; int* arr; long GetAbilityLevel(int ab); };
long S::GetAbilityLevel(int ab){ return this->arr[ab]; }