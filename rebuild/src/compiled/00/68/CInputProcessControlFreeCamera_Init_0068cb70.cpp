static char g_f=0;
struct S { void Init(); };
void S::Init(){ if(!g_f) g_f=1; }