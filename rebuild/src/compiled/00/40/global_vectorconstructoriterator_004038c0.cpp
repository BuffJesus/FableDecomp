typedef void (__fastcall *VecCtor)(void* self);

void __stdcall vector_constructor_iterator(void* arr, int size, int count, VecCtor ctor)
{
    int m = count - 1;
    if (m >= 0) {
        char* p = (char*)arr;
        int n = m + 1;
        do {
            ctor(p);
            p += size;
            --n;
        } while (n != 0);
    }
}