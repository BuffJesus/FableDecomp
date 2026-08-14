#include <cstdio>
#include <cstring>

// Behaviour model: base copies whole-ish, sub copies at +0x28
struct Obj {
    int base_field;   // at 0
    char mid[0x24];
    int sub_field;    // at 0x28
};

static int g_base_called_with;
static int g_sub_called_with;

void BaseCopy(Obj* self, const char* src) {
    const Obj* s = (const Obj*)src;
    self->base_field = s->base_field;
    g_base_called_with = s->base_field;
}
void SubCopy(int* self, const int* src) {
    *self = *src;
    g_sub_called_with = *src;
}

void Copy(Obj* self, const char* src) {
    BaseCopy(self, src);
    SubCopy((int*)(((char*)self) + 0x28), (const int*)(src + 0x28));
}

int main() {
    Obj dst; memset(&dst, 0, sizeof(dst));
    Obj src; memset(&src, 0, sizeof(src));
    src.base_field = 111;
    src.sub_field = 222;
    Copy(&dst, (const char*)&src);
    if (dst.base_field == 111 && dst.sub_field == 222 &&
        g_base_called_with == 111 && g_sub_called_with == 222) {
        printf("COPY_OK\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}