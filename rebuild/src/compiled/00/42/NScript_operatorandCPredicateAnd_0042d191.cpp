#pragma optimize("s",on)
struct S { int x; void pa_h(void* arg); S* M(void* arg); };
S* S::M(void* arg){ this->pa_h(arg); return this; }