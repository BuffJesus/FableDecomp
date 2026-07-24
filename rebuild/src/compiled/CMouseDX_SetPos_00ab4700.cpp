// Byte-exact reconstruction of ?SetPos@CMouseDX@@UAEXPBVC2DVector@@@Z
// retail 0x00ab4700, module _global. VC7.1 /O2 /Oy /W3.
//
// Retail shape notes (see residue bundle 00ab4700.md):
//   * position->X (dword) -> [esi+0x3414] (PositionX) AND [esi+0x3434] (CachedPositionX)
//   * position->Y (dword) -> [esi+0x3418] (PositionY) AND [esi+0x3438] (CachedPositionY)
//   * if [esi+0x343c] (DisableCursorWarp) != 0 -> return.
//   * else: window = ResolveMouseWindowOwner()->[0x94]  (retail: relative `call`,
//     then `mov edi,[eax+0x94]`; a FREE helper, NOT an inlined method chain).
//   * ClientToScreen(window,&pt); GetClientRect(window,&rect);
//     OffsetRect(&rect,pt.x,pt.y);
//   * SetCursorPos(_ftol((float)rect.Left + PositionX),
//                  _ftol((float)rect.Top  + PositionY));
//     The float->int conversions are the VC7.1 `_ftol` helper (retail: `call 0x14a370`).
//
// Self-contained: engine structs inlined, engine helpers/globals extern-declared.

typedef unsigned char u8;

struct C2DVector { float X; float Y; };

struct CMousePoint { long x; long y; };
struct CMouseRect { long left; long top; long right; long bottom; };

struct CMouseWindowOwner
{
    u8   pad00[0x94];
    void* Window;   // +0x94
};

// Free helper: retail emits a relocation-masked relative `call` returning the
// owner whose +0x94 field is the OS window handle used for coordinate mapping.
extern "C" CMouseWindowOwner* __cdecl ResolveMouseWindowOwner(void);

// Engine import-table globals (retail: `call DWORD PTR ds:0x14403xx`).
extern int (__stdcall *FableClientToScreen)(void*, CMousePoint*); // ds:0x1440304
extern int (__stdcall *FableGetClientRect)(void*, CMouseRect*);   // ds:0x1440338
extern int (__stdcall *FableOffsetRect)(CMouseRect*, int, int);   // ds:0x14403a4
extern int (__stdcall *FableSetCursorPos)(int, int);              // ds:0x14403a0

struct CMouseDX
{
    u8    pad000[0x3414];
    float PositionX;        // +0x3414
    float PositionY;        // +0x3418
    u8    pad341C[0x18];
    float CachedPositionX;  // +0x3434
    float CachedPositionY;  // +0x3438
    u8    DisableCursorWarp; // +0x343c
};

// __fastcall(thisptr in ecx, unused edx, position) models the __thiscall member.
void __fastcall CMouseDX_SetPos(CMouseDX* self, void* /*edx*/, const C2DVector* position)
{
    self->PositionX = position->X;
    self->CachedPositionX = position->X;
    self->PositionY = position->Y;
    self->CachedPositionY = position->Y;

    if (self->DisableCursorWarp != 0)
        return;

    void* window = ResolveMouseWindowOwner()->Window;

    CMousePoint pt;
    pt.x = 0;
    pt.y = 0;
    FableClientToScreen(window, &pt);

    CMouseRect rect;
    FableGetClientRect(window, &rect);
    FableOffsetRect(&rect, pt.x, pt.y);

    int x = (int)((float)rect.left + self->PositionX);
    int y = (int)((float)rect.top + self->PositionY);
    FableSetCursorPos(x, y);
}
