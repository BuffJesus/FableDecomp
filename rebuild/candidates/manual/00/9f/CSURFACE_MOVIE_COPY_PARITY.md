# CSurface movie-copy seam parity

These four functions were recovered without starting or querying Ghidra.
Retail bodies came directly from the installed `Fable.exe` PE; names and
prototypes came from the checked-in retail manifest and donor PDB name table.
All objects were compiled with Microsoft Visual C++ 7.1:

```powershell
D:\Tools\vc71\bin\cl.exe /nologo /c /O2 /Oy /W3 /Fo<output.obj> <source.cpp>
```

Object `.text` and relocation records were extracted with `objcopy` and
`objdump -r`. Each four-byte relocation field was zeroed in both bodies before
comparison.

| Address | Function | Retail | Built | Relocations | Normalized result |
|---|---|---:|---:|---:|---|
| `0x009F2D60` | `CSurface::CSurface(const CSurface&)` | 55 | 55 | 1 | exact |
| `0x009F2F10` | `CSurface::InitialiseFromTextureMipmap` | 66 | 66 | 0 | exact |
| `0x009F37E0` | `CSurface::CopyFromSurface` | 446 | 446 | 14 | exact |
| `0x009FA4E0` | `CTexture::CopyFromTexture` | 289 | 289 | 10 | exact |

The first two functions are ordinary recovered C++. The latter two retain
named calls and recovered branch semantics in symbolic naked bodies because
ordinary C++ does not preserve the retail temporary-surface register and stack
allocation.

The catalog's 286-byte size for `CTexture::CopyFromTexture` begins after its
three-byte `sub esp, 4Ch` prologue. Direct PE disassembly establishes the true
body as `0x009FA4E0` through `0x009FA600`, inclusive: 289 bytes ending in
`ret 8`. The following `0xCC` alignment run and jump thunk at `0x009FA610`
belong to neither body.

Behavior fixtures link against the candidate objects and cover:

- copy-constructor conditional mip ownership plus `AddRef`;
- texture-mipmap replacement, old-surface `Release`, and ownership state;
- direct D3DX surface copies with both optional rectangles;
- the signed-destination/unsigned-source path, including temporary image
  creation, recursive copy, lock, signed conversion, unlock, and release.
- source-smaller and destination-smaller mip-count minima plus zero mip levels;
- per-level source/destination acquisition, filter and null-box forwarding;
- initial-wrapper and copied-wrapper reference lifetimes and cleanup order.

```text
FABLETLC_SURFACE_COPY_CONSTRUCTOR_BEHAVIOR PASS
FABLETLC_SURFACE_INITIALISE_TEXTURE_MIPMAP_BEHAVIOR PASS
FABLETLC_SURFACE_COPY_FROM_SURFACE_BEHAVIOR PASS
FABLETLC_TEXTURE_COPY_FROM_TEXTURE_BEHAVIOR PASS
```
