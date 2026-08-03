typedef void* (__cdecl *FnPtr)(unsigned int);
#define SLOT (*(FnPtr*)0x01440150)

extern "C" void* __cdecl operator_new(unsigned int size)
{
    return SLOT(size);
}