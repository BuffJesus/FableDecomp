
unsigned long g_node0;
unsigned long g_node1;
unsigned char g_flagB;
unsigned char g_flagA;
void __fastcall SetBlockedNode(unsigned char a, unsigned char b, unsigned long c, unsigned long d)
{
    g_node0 = c;
    g_node1 = d;
    g_flagA = a;
    g_flagB = b;
}