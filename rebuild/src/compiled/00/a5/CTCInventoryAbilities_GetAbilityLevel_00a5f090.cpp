struct S { char pad[376]; int* arr; long GetAbilityLevel(int ab); };
long S::GetAbilityLevel(int ab){ return this->arr[ab]; }