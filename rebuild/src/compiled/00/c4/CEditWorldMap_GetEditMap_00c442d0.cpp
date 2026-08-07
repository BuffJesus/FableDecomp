struct S { char pad[4]; long long* f4; long long M(int a0); };
long long S::M(int a0){ return this->f4[a0]; }