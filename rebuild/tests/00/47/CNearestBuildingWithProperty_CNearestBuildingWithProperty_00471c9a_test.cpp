#include <cstdio>
struct CNearestBuildingWithProperty {
    int m0, m1, m2, m3, m4, m5;
    CNearestBuildingWithProperty() {}
    CNearestBuildingWithProperty(const CNearestBuildingWithProperty& o);
};
CNearestBuildingWithProperty::CNearestBuildingWithProperty(const CNearestBuildingWithProperty& o)
    : m0(o.m0), m1(o.m1), m2(o.m2), m3(o.m3), m4(o.m4), m5(o.m5) {}
int main() {
    CNearestBuildingWithProperty src;
    src.m0=11; src.m1=22; src.m2=33; src.m3=44; src.m4=55; src.m5=66;
    CNearestBuildingWithProperty dst(src);
    if (dst.m0==11 && dst.m1==22 && dst.m2==33 && dst.m3==44 && dst.m4==55 && dst.m5==66) {
        std::printf("OK_0x00471c9a\n");
        return 0;
    }
    std::printf("FAIL_0x00471c9a\n");
    return 1;
}