#include <cstdio>
struct CEnginePrimitiveRenderer2D { float Layer2DToDistance(int l); };
float CEnginePrimitiveRenderer2D::Layer2DToDistance(int l){ return *(float*)((char*)this + l*4 + 0x1d4); }
int main(){ char blk[0x400]; *(float*)(blk+3*4+0x1d4)=1.5f; CEnginePrimitiveRenderer2D* o=(CEnginePrimitiveRenderer2D*)blk;
 if(o->Layer2DToDistance(3)!=1.5f){ std::printf("00b4a6a0_TEST FAIL\n"); return 1;}
 std::printf("00b4a6a0_TEST PASS\n"); return 0;}