// `return g * 10;` -- VC7.1 strength-reduces to `lea eax,[eax+eax*4]; add eax,eax`.
extern int g_value;
extern "C" int TimesTen() { return g_value * 10; }