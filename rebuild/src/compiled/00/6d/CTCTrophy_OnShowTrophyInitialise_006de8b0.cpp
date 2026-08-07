struct S { char pad[0xC]; char* fC; void OnShowTrophyInitialise(void* arg); void Helper(void* a); };
void S::OnShowTrophyInitialise(void* arg){ (void)arg; this->Helper(this->fC + 0x4c); }