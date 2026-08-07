#include <cstdio>
struct C { char pad[0x20]; int f20; void Clear(){ this->f20=0x320; } };
int main(){ C o; o.f20=0; o.Clear(); if(o.f20!=0x320){ std::printf("00a14e20_TEST FAIL\n"); return 1;} std::printf("00a14e20_TEST PASS\n"); return 0;}