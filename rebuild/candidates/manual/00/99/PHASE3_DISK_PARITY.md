# GFMain Phase 3 path/file parity

The Phase 3 settings-file path now has exact VC7.1 objects for its existence
probe, disk-file construction, and the directly paired close operation.

All objects were built with:

```powershell
D:\Tools\vc71\bin\cl.exe /nologo /c /O2 /Oy /W3 /Fo<output.obj> <source.cpp>
```

The retail bodies came directly from `Fable.exe` through
`tools/decomp_pipeline/pe_oracle.py`.  Object `.text` and relocation records
were read with the pinned WinLibs `objdump.exe`; each four-byte relocation
field was zeroed in both bodies before comparison.

| Address | Recovered function | Retail | Built | Relocations | Result |
|---|---|---:|---:|---|---|
| `0x00999230` | `CAFile::PathExists` | 111 | 111 | `+0x0B,+0x14,+0x20,+0x2A,+0x33,+0x3C,+0x45,+0x4C,+0x5A,+0x65` | exact |
| `0x0099AD80` | `CDiskFileWin32` constructor | 50 | 50 | `+0x04,+0x0D,+0x12,+0x28` | exact |
| `0x0099A920` | `CDiskFileWin32::Close` | 52 | 52 | `+0x16,+0x20,+0x2A` | exact |

There is no differing byte after relocation normalization for any body.

The fixtures link each retail-shaped source object and prove:

- `PathExists` expands against the current path context, calls
  `GetFileAttributesW`, accepts any result other than
  `INVALID_FILE_ATTRIBUTES`, and destroys all three temporaries on both
  branches.
- the disk constructor performs base construction, installs the retail
  vtable, constructs the pathname member at `+0x18`, forwards access type and
  flags to `Open`, and returns `this`.
- `Close` issues the retail zero-offset `FILE_CURRENT` `SetFilePointer`,
  flushes, closes, and clears `opened`; a second close is idempotent and makes
  no Win32 calls.

Fixture results:

```text
FABLETLC_PATH_EXISTS_00999230 PASS
FABLETLC_DISK_FILE_CTOR_0099AD80 PASS
FABLETLC_DISK_FILE_CLOSE_0099A920 PASS
```
