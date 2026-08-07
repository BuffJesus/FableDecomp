extern int g_a; extern int g_b;
int* AddChildPrimitive(){ g_b = g_a; return &g_a; }