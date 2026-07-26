struct MA { char pad[0x4c]; char a; };
struct MB { char pad[0x54-0x4c]; char b; };
struct MC { char pad[0x64-0x54]; char c; };
struct Obj {
    char base[0x4c];
    MA ma;   /* +0x4c */
};

extern "C" void __fastcall sub_A(void* p);   /* dtor for +0x64 */
extern "C" void __fastcall sub_B(void* p);   /* dtor for +0x54 */
extern "C" void __fastcall sub_C(void* p);   /* dtor for +0x4c */
extern "C" void __fastcall sub_Base(void* p); /* dtor for this */

void __fastcall Dest_val(char* self)
{
    sub_A(self + 0x64);
    sub_B(self + 0x54);
    sub_C(self + 0x4c);
    sub_Base(self);
}