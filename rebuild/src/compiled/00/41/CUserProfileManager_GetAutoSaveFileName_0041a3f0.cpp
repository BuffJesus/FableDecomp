extern const char g_str[];
struct S { int x; void N(const char* s); S* M(); };
S* S::M(){ this->N(g_str); return this; }