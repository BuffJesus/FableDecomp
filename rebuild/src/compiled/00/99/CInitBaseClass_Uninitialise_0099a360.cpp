extern int g_count;
struct S { char pad[4]; unsigned char b4; void M(); };
void S::M(){ if(this->b4==1){ --g_count; this->b4=0; } }