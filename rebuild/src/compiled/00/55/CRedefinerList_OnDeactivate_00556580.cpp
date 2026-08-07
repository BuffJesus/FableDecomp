struct Sub { virtual void d0(); virtual void d1(); virtual void d2(); virtual void d3(); virtual void V4(int); };
struct S { char pad0[4]; Sub sub; char pad1[0x174]; unsigned char b17c; unsigned char b17d; char pad2[2]; float f180; };
extern void __fastcall od_helper(S*, float);
void __fastcall OnDeactivate(S* self){
    self->b17c = 0;
    od_helper(self, 0.2f);
    self->f180 = 0.2f;
    self->b17d = 0;
    self->sub.V4(0x25);
    self->sub.V4(0x24);
}