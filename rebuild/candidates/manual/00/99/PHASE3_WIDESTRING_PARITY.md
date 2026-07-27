# GFMain Phase 3 wide-string operator parity

VC7.1 command line used for both source objects:

```powershell
D:\Tools\vc71\bin\cl.exe /nologo /c /O2 /Oy /W3 /Fo<output.obj> <candidate.cpp>
```

The object `.text` bodies were extracted with the pinned WinLibs `objdump.exe`.
Relocation offsets came from `objdump -h -r`; each four-byte relocation field
was zeroed in both the retail oracle and built body before comparison.

| Address | Retail | Built | Raw first difference | Normalized result |
|---|---:|---:|---|---|
| `0x0099BF30` | 177 bytes | 177 bytes | `+0x04`, absolute global relocation | exact |
| `0x0099BFF0` | 134 bytes | 134 bytes | `+0x0B`, relative call relocation | exact |

There is no first differing byte after relocation normalization for either
body. Ordinary recovered C++ was also tested under `/O1`, `/O2`, `/Ox`,
`/Ob2`, `/Os`, and `/Ot`. It retained the right ownership behavior but emitted
different register allocation and lengths (the closest `/O2` bodies were 185
and 146 bytes). The checked-in symbolic naked bodies are therefore the same
fallback already used by other exact catalog entries: branch and ownership
logic remain named, and only the retail instruction allocation is explicit.

Behavior was linked against the actual candidate object, not a test-only
semantic duplicate:

```powershell
D:\Tools\vc71\bin\cl.exe /nologo /c /Od /W3 /Fo<test.obj> <test.cpp>
D:\Tools\vc71\bin\link.exe /nologo /out:<test.exe> <source.obj> <test.obj>
```

Results:

```text
FABLETLC_WIDE_STRING_ADD_RIGHT PASS
FABLETLC_WIDE_STRING_ADD_LEFT PASS
```
