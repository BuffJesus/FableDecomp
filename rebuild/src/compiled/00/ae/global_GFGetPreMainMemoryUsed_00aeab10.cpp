// Global getter: `mov eax,[g]; ret`. Free function, no `this`.
extern int g_value;
extern "C" int GetGlobal() { return g_value; }