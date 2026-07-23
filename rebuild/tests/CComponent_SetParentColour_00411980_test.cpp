#include <cstdio>
struct CComponent {
    int* first;
    int* second;
};
static void copy_range(int* first, int* last, int* dst, CComponent* self)
{
    int n = (int)(last - first);
    while (n > 0) {
        *dst = *first;
        ++first;
        ++dst;
        --n;
    }
    self->second = dst;
}
void __fastcall CComponent_SetParentColour(CComponent* self)
{
    copy_range(self->second, self->second, self->first, self);
}
int main(){
    int buf[4] = {0,0,0,0};
    CComponent c;
    c.first = buf;
    c.second = buf + 2;
    CComponent_SetParentColour(&c);
    if (c.second == buf) {
        std::printf("CComponent_00411980_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL second=%p\n", (void*)c.second);
    return 1;
}