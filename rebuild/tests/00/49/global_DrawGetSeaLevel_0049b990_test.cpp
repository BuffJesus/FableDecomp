#include <cstdio>
double __fastcall DrawGetSeaLevel(const unsigned char* self)
{
    return *reinterpret_cast<const double*>(self + 40);
}
int main()
{
    unsigned char object[48] = {0};
    *reinterpret_cast<double*>(object + 40) = 123.5;
    if (DrawGetSeaLevel(object) == 123.5) { std::printf("AUTO_TINY_0049b990_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0049b990_TEST FAIL\n");
    return 1;
}