# graphics.big MBANK — where (and whether) the mesh entry is LZO-inflated on the load path

> **★ CORRECTION (2026-07-19, empirical): the "reads RAW / no LZO" verdict below is REFUTED
> for the on-disk format.** The installed `graphics.big` mesh sub-blocks ARE genuinely
> LZO-compressed — measured frame ratios: seagull prim0 vertex buffer `out=5080, clen=2940`
> (58%), bear prim0 `out=35796, clen=28416` (79%); LZO1X-decompressing those exact ranges
> yields geometry whose bbox matches the entry's stored bbox to 0.000 (`scratchpad/
> probe_lzo_framing.py`, `ego_mesh_validate.py`; ported into `fable_core._parse_mesh_primitives`).
> So the engine MUST inflate these blocks on load — this xref pass (via the newly-written
> `XrefTree.java`) MISSED the real caller, most likely a chunked-LZO **wrapper** (frames
> `[u16 clen]` blocks, calls `lzo1x_decompress` internally) reached by indirect/vtable
> dispatch, or a second LZO copy. The call-chain facts below (BuildMesh→GetData→ReadEntryData)
> are accurate as far as they go, but the "no inflate anywhere" CONCLUSION is wrong. Settle the
> exact call site with a live x32dbg breakpoint on the bank read during a mesh load. This does
> NOT affect tooling: the on-disk format is LZO (see `BIG_MESH_GEOMETRY.md`).

*Authored 2026-07-19 (mesh-bank-inflate agent). Method: read-only Ghidra 12.1 headless decompile of
retail `Fable.exe` (project FableTLC, ImageBase `0x400000`). Task: locate the engine function that
LZO-inflates a `graphics.big` mesh entry BEFORE `C3DMesh2::LoadBinary` (`0x00a8ad40`) runs — i.e. what
fills the `CMemoryDataInputStream` that LoadBinary reads. Raw decompiles:
`ghidra_out/decomp_mesh_bank_inflate*.c`, `ghidra_out/decomp_bankfile.c`, `ghidra_out/decomp_asyncdata.c`,
`ghidra_out/decomp_setupbuffer.c`, `ghidra_out/decomp_lzo_named.c`.*

## VERDICT (honest, evidence-first)

**I did NOT find an LZO-inflate stage on the retail `Fable.exe` mesh load path.** The bytes handed to
`C3DMesh2::LoadBinary` are read **RAW** from the bank file into a movable-pool buffer via the async
resource loader; there is no `lzo1x_decompress` (nor zlib) call between the on-disk bank entry and
`LoadBinary`. The one and only caller of `lzo1x_decompress` (`0x00c06b90`) is on the **SAVE/compress**
path (a compress-then-verify roundtrip), and `lzo1x_decompress_safe` (`0x00c08170`) is used **only by the
font glyph bank**. Neither touches `graphics.big` mesh loading.

This **contradicts the task premise** that a bank-layer LZO inflate feeds LoadBinary. Per the "evidence,
not assumption / document the gap" rule, I report what the retail binary actually does rather than
forcing a match. See §5 for how to reconcile this with the EgoCore empirical LZO decode.

## 1. The full mesh load chain in retail `Fable.exe` (cite these)

| Step | Function | Retail addr | What it does with the bytes |
|------|----------|-------------|------------------------------|
| 1 | `C3DMeshLODInfo::OnResourceLoaded` | `0x00a25470` | on load complete → calls `BuildMesh(true,false)` |
| 2 | `C3DMeshLODInfo::BuildMesh` | `0x00a24520` | `pvVar5 = CBankFileAsyncData::GetData(this+0x24)`; `uVar3 = *(this+0x24+0x10)` (size); `CMemoryDataInputStream(local_1c, pvVar5, uVar3)`; then `C3DMesh2::LoadBinary(...)`. **Wraps the async buffer directly — no inflate call.** |
| 3 | `C3DMesh2::LoadBinary` | `0x00a8ad40` | plain deserialize of the in-memory image (prior finding) |
| 4 | `C3DPrimitive2::LoadBinary` | `0x00a92900` | reads via `CMemoryDataInputStream` pointer-bump + `CDataInputStream::ReadWithSrcChunkOverflow` (chunk stitcher, NOT a codec) |
| 5 | `C3DPrimitive2::LoadVertexBuffer` | `0x00a91560` | raw block copy of `nVerts*stride`; copies the 8-dword `CVertexCompressionParams` into VB+0x14 |

### What fills the buffer (the "one layer earlier" the task asked about)

| Function | Retail addr | Behaviour |
|----------|-------------|-----------|
| `CBankFileAsyncData::SetupBuffer` | `0x009d52d0` | allocates the dest buffer sized `CBankFile::Align(dataOffset + dataSize)` from `CMovableResourceMemoryPool::AllocateResource` (or a fallback `CMemoryAllocator`). **Allocate only — no decompress.** |
| `CBankFileAsyncData::OnFileReadCompletion` | `0x009d54a0` | sets the "ready" flag after the threaded file read finishes. **No transform of the bytes.** |
| `CBankFileAsyncData::RelocateData` | `0x009d5670` | rebases embedded pointers (`ptr - oldBase + newBase`) — confirms the entry is a **relocatable serialized image**, exactly what LoadBinary deserializes. |
| `CBankFile::ReadEntryData` | `0x009cc2a0` | seeks (`file vtbl+0x14`) then reads (`file vtbl+0xc`) `size` bytes from the underlying file object at `this+0x7c`. **Raw seek+read, no inflate.** |
| `CBankFile::GetEntryDataSize` | `0x009cc240` | returns the stored entry size (packed-uint array or `[entryTable + 4 + idx*0xc]`). |

So: on-disk bank entry → (threaded raw read via `ReadEntryData`/`SetupBuffer`) → movable-pool buffer →
`GetData()` → `CMemoryDataInputStream` → `LoadBinary`. **No inflate node exists on this path.**

## 2. xref evidence for the two LZO decompressors (the crux)

`FindRefs`/`XrefTree` over both LZO entrypoints (recursive caller walk, depth 4):

**`lzo1x_decompress` @ `0x00c06b90` — exactly ONE caller:**
```
c06b90 _lzo1x_decompress  <-  FUN_00996610 @ 0x996610   (call site 0x009966e5)
FUN_00996610  <-  SaveGameState@4b64e0, TransferBinaryOut@6ba030,
                  SaveBinary@a2eaa0 / a75400 / a89450 / a8fd40 / ac3b00
```
`FUN_00996610` (`ghidra_out/decomp_mesh_bank_inflate_probe.c`) is the **chunked-LZO WRITER**: it calls
`_lzo1x_999_compress(src, size-3, ...)` then immediately `_lzo1x_decompress(...)` to **verify the
roundtrip** of the block it just compressed. Note `size-3` and a trailing `3` — the same "fill size-3 +
3 trailing plain bytes" framing EgoCore's `Utils.h DecompressLZO` describes. **This is the compressor
used when SAVING banks/savegames, not a loader.** Its `lzo1x_decompress` call never sees `graphics.big`.

**`lzo1x_decompress_safe` @ `0x00c08170` — callers are all FONT bank:**
```
c08170 _lzo1x_decompress_safe  <-  CStreamingFontBank ctor @0x9a3c20  (via AcquireFont@0xa5f1c0)
                                <-  GlyphLookup @0x9a1140
                                <-  FindGlyph  @0x9a4160
```
These are the streaming-font glyph decompressor, not the mesh path. (Notably the font bank ctor at
`0x9a3c20` uses the **generic** `CBankFile::GetEntryDataSize` → `malloc` → `CBankFile::ReadEntryData` →
`CMemoryDataInputStream` pattern — the same shape as the mesh loader — and the LZO call is *inside the
font asset*, deeper than the bank read, i.e. the font's own per-glyph payload is LZO, not the bank entry.)

## 3. The "Decompress"-named functions are all ZLIB, and not on the mesh path

`DecompByName Decompress Inflate Uncompress Lzo LZO` → 21 fns (`ghidra_out/decomp_lzo_named.c`). The
real decompressors found:
- `CCompressorZlib::{Decompress,DecompressStream,InternalDecompress,FinishDecompressStream}`
  (`0x00a3a840`..`0x00a3abe0`) — **zlib** (`z_inflate` @ `0x00c02470`).
- `CDefinitionManager::DecompressDefinitionData` (`0x009ad5f0`) → `CCompressorZlib::InternalDecompress`
  — **definition data is zlib**, and it feeds a `CMemoryDataInputStream::OpenMemorySource` (this is the
  DEF path, not mesh).
- `CFileInstaller::ProcessActiveFileForDecompression` (`0x009c3c20`) → `CCompressorZlib::DecompressStream`
  — the **install-time file expander is zlib**, streamed in ≤0x40000 chunks. Operates on installer active
  files, not on a per-mesh-entry basis at draw time.
- `CACompressor::Decompress` (`0x0052adf0`) — empty stub.

None of these zlib decompressors is called by `BuildMesh` / `C3DMesh2::LoadBinary` /
`C3DPrimitive2::LoadBinary` / the `CBankFileAsyncData` fill path.

## 4. Cited snippets

`BuildMesh` (`0x00a24520`) — the buffer is taken raw and wrapped, no inflate:
```c
uVar3  = *(ulong *)(*(CBankFileAsyncData **)(this + 0x24) + 0x10);   // entry size
pvVar5 = CBankFileAsyncData::GetData(*(CBankFileAsyncData **)(this + 0x24)); // raw buffer ptr
CMemoryDataInputStream::CMemoryDataInputStream(local_1c, pvVar5, uVar3);
...
bVar2 = C3DMesh2::LoadBinary(*(C3DMesh2 **)this_00, local_1c, ...);  // reads local_1c directly
```

`FUN_00996610` (`0x00996610`) — the LZO *writer* (compress + verify), the only `lzo1x_decompress` caller:
```c
_lzo1x_999_compress(param_2, puVar5 + -3, ..., &param_3, ...);   // compress src of size (n-3)
_lzo1x_decompress (local_18, param_3, puVar4, &local_1c, 0);     // verify the just-made block
// ... 'puVar5 = 3' trailing bytes handled separately  -> matches EgoCore [.. + 3 trailing plain]
```

`CBankFile::ReadEntryData` (`0x009cc2a0`) — raw seek + read, no codec:
```c
(**(code **)(**(int **)(this + 0x7c) + 0x14))(uVar1 + param_2);      // file->Seek(entryOffset+off)
(**(code **)(**(int **)(this + 0x7c) + 0xc ))(param_2, param_3, 0);  // file->Read(len, dst)
```

## 5. Reconciling with the EgoCore empirical LZO decode

Both facts are real and must coexist:
- **EgoCore** (community re-impl) decodes `graphics.big` mesh vertex/index buffers as **chunked-LZO** and
  the bbox matched to 0.000 — empirically true for the asset bytes EgoCore parsed.
- **Retail `Fable.exe`** reads its mesh bank entries **raw** into `LoadBinary` — no inflate node (this doc).

The consistent explanation is that the **LZO lives one layer BELOW the per-entry mesh read, in the bank
container / install layer**, not in the mesh loader:
1. `lzo1x_decompress` exists in the binary and its **chunked writer** `FUN_00996610` (`size-3` + 3
   trailing bytes) matches EgoCore's `DecompressLZO` framing exactly — so the engine *does* produce this
   LZO framing when it **writes** banks/savedata. The retail on-disk `graphics.big` therefore plausibly
   stores entries in that chunked-LZO frame.
2. But at load time the file-object under `CBankFile` (`this+0x7c`) / the `CFileInstaller`
   (`ProcessActiveFileForDecompression`, zlib) / the threaded read that fills `CBankFileAsyncData` is the
   layer that would expand the container **before** `ReadEntryData` returns plain bytes to `LoadBinary`.
   I traced `ReadEntryData`/`SetupBuffer`/`OnFileReadCompletion` and found them raw — meaning the inflate,
   if it happens for graphics.big at all, is in the **underlying file/stream object** (the vtable at
   `CBankFile+0x7c`, e.g. a `CDiskFileWin32` or a decompressing stream selected at `OpenReadOnly` time),
   which I did not fully resolve in this pass.

**GAP — NOW RESOLVED.** I decompiled `CBankFile::OpenReadOnly` (`0x009d06f0`,
`ghidra_out/decomp_mesh_bank_inflate_openread.c`). The file object stored at `CBankFile+0x7c` is a plain
**`CDiskFileWin32`** (`CCountedPointer<CDiskFileWin32>::operator=(this+0x7c, ...)`), and the bank header is
read through a plain **`CFileDataInputStream`** (`0x4000` buffer). **There is NO decompressing stream class
at `+0x7c`.** Therefore `ReadEntryData`'s `file->Seek/Read` (§4) returns the on-disk bytes verbatim, and
`LoadBinary` receives them raw. Snippet:
```c
CCountedPointer<CDiskFileWin32>::operator=((CCountedPointer<CDiskFileWin32> *)(this + 0x7c), &uStack_40);
CFileDataInputStream::CFileDataInputStream(aCStack_24, *(CAFile **)(this + 0x7c), 0x4000); // plain read
```

**Consequence:** on the retail PC engine, `graphics.big` mesh entries are read **plain** end-to-end — there
is no runtime LZO (or zlib) inflate of mesh geometry at all. This means the EgoCore chunked-LZO decode
corresponds to EITHER (a) the **Xbox/original** asset form (EgoCore targets that layout) rather than the
retail PC `graphics.big`, OR (b) the retail PC `graphics.big` was expanded **once at install time** by
`CFileInstaller::ProcessActiveFileForDecompression` (zlib, `0x009c3c20`) into the plain on-disk bank the
engine mmaps — in which case the *shipped* archive is compressed but the *installed* `graphics.big` the
engine reads is plain. To decide between (a) and (b): hexdump a real installed-`graphics.big` mesh entry
at its table offset and check for the `[u16 clen]...` LZO frame vs a plain relocatable image, and/or set a
live x32dbg breakpoint on `ReadEntryData` (`0x009cc2a0`) during a mesh load.

## 6. One-line verdict

On the retail `Fable.exe` **mesh load path there is no LZO inflate** — `LoadBinary` reads a raw
relocatable image straight from a plain `CDiskFileWin32` bank read (`CBankFile+0x7c` is `CDiskFileWin32`,
confirmed in `OpenReadOnly` @ `0x009d06f0`); the only `lzo1x_decompress` (`0x00c06b90`) is the **save-side
compress-verify** (`FUN_00996610`, single caller) and `lzo1x_decompress_safe` (`0x00c08170`) is
**font-only**. So the EgoCore chunked-LZO mesh decode is NOT what the retail PC engine does at load —
either it reflects the Xbox/original `graphics.big` layout, or the retail archive is expanded once at
install time (`CFileInstaller`, zlib) into the plain bank the engine mmaps.

## 7. Reproduce
```
# Xref proof (both LZO entrypoints):
analyzeHeadless ... -postScript XrefTree.java 4 0xc06b90 0xc08170
analyzeHeadless ... -postScript FindRefs.java c06b90        # -> 1 ref: FUN_00996610

# Decompiles saved:
#   ghidra_out/decomp_mesh_bank_inflate_probe.c      (FUN_00996610 = LZO compress+verify writer)
#   ghidra_out/decomp_mesh_bank_inflate_fontpath.c   (CStreamingFontBank / GlyphLookup / FindGlyph)
#   ghidra_out/decomp_mesh_bank_inflate_buildmesh.c  (OnResourceLoaded + BuildMesh)
#   ghidra_out/decomp_mesh_bank_inflate_loadvb.c     (LoadVertexBuffer + C3DPrimitive2::LoadBinary)
#   ghidra_out/decomp_bankfile.c                     (CBankFile::ReadEntryData / GetEntryDataSize)
#   ghidra_out/decomp_asyncdata.c                    (CBankFileAsyncData fill path)
#   ghidra_out/decomp_setupbuffer.c                  (CBankFileAsyncData::SetupBuffer)
#   ghidra_out/decomp_lzo_named.c                    (all Decompress/Inflate/Lzo-named fns)
```
