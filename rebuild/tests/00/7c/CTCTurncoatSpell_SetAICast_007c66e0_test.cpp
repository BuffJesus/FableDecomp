#include <cstdio>
struct CTCTurncoatSpell { char pad[0x3a]; unsigned char b3a; char pad2[0x11]; long f4c; void SetAICast(long v); };
void CTCTurncoatSpell::SetAICast(long v){ this->b3a=1; this->f4c=v; }
int main(){ CTCTurncoatSpell o; o.b3a=0; o.f4c=0; o.SetAICast(0x1234);
 if(o.b3a!=1||o.f4c!=0x1234){ std::printf("007c66e0_TEST FAIL\n"); return 1;}
 std::printf("007c66e0_TEST PASS\n"); return 0;}