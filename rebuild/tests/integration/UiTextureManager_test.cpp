#include "fable_ui_texture_manager.h"
#include <stdio.h>
#include <string.h>
void* FableUiBaseVtable=reinterpret_cast<void*>(0x70000100);
void* FableUiResourceListVtable=reinterpret_cast<void*>(0x70000103);
void* FableUiResourceVtable=reinterpret_cast<void*>(0x70000104);
void* FableUiPreallocPoolVtable=reinterpret_cast<void*>(0x7000010A);
FableUiGraphicsBankVtable FableUiTextureManagerVtable={};
int main()
{
    __declspec(align(8)) unsigned char storage[sizeof(FableUiTextureManagerView)+16];
    for(unsigned seed=0;seed<256;++seed)
    {
        for(unsigned i=0;i<sizeof(storage);++i) storage[i]=static_cast<unsigned char>(seed+i*17);
        void* p=storage+8;
        if(FableUiConstructTextureManager(p,0)!=p) return 2;
        for(unsigned c=0;c<8;++c) if(storage[c]!=static_cast<unsigned char>(seed+c*17) || storage[sizeof(storage)-8+c]!=static_cast<unsigned char>(seed+(sizeof(storage)-8+c)*17)) return 3;
        printf("CASE%u",seed);
        for(unsigned k=0;k<sizeof(FableUiTextureManagerView);k+=4)
        {
            unsigned word; memcpy(&word,storage+8+k,4);
            unsigned base=reinterpret_cast<unsigned>(p);
            if(word==reinterpret_cast<unsigned>(&FableUiTextureManagerVtable)) word=0x70000109;
            else if(word>=base && word<base+sizeof(FableUiTextureManagerView)) word=0x60000000+word-base;
            printf(":%08x",word);
        }
        printf("\n");
    }
    return 0;
}
