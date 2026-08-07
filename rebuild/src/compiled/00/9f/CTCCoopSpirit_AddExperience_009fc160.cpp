struct S { char pad[0x2c]; long fv; void AddExperience(long a); };
void S::AddExperience(long a){ this->fv += a; }