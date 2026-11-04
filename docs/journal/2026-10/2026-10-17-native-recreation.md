# Native recreation - 2026-10-17

Resumed from the 2026-10-16 bedtime stop point (CHandleBasedMemoryPool area helpers drafted, not landed).
Nothing committed or pushed.

## CHandleBasedMemoryPool (lib_handle_based_memory_pool.cpp)

One TU, `rebuild/src/compiled/00/a0/CHandleBasedMemoryPool_Areas_00a01d20.cpp`, now holds the file in
Ego_r's function order (0x00A01D20-0x00A03550). Retail expands the small members (CreateArea,
BeginCritical/EndCritical, ClearFreeLists, ValidateLargestFreeArea, the CScopedCritical pair) into their
callers, so they have to share the TU. The short-lived `CHandleBasedMemoryPool_Growable_00a02020.cpp` was
merged into it and its registration retargeted.

39 exact roots in 5 gates (all in gate_all + symbol_index):

- **pool_areas**: CHeader ctor A01D20, GetHandleFromPointer A01D40, CreateArea A01D50, FindFreeArea
  A01ED0, FindLargestFreeArea A01F50, UpdateLargestFreeArea A01F80, ValidateLargestFreeArea A01FE0.
- **pool_critical**: TestIfAreaIsGrowable A02020, ClearFreeLists A02100, BeginCritical A02140,
  EndCritical A02170 (supersedes the stand-in `?EndCritical@@YIXPAU...` TU, left in place), Uninitialise
  A02190, BeginShutdown A02220, EndShutdown A02370, ResizeArea A02410, CombineAdjacentAreas A02470,
  Validate A024C0, Initialise A02570, ResizePool A02680.
- **pool_defragment**: Defragment A02730, InternalGetResource A028D0, MoveArea A028F0, ValidateResources
  A02AD0.
- **pool_allocate**: GrowFreeArea A02C00, the pool constructor A02D20 and destructor A02DB0 (manifest
  labels there were wrong: CEnginePrimitiveTypeInfo / CTCInventoryItem::OnDie), InternalAllocateHandle
  A03040, InternalAllocate A030D0, and the scalar deleting destructor A03AA0.
- **pool_allocation_policy**: PartialDefragment A02DF0, Allocate(long*, size, flags) A03180,
  Allocate(CScopedMemoryHandle&, size, flags) A032F0, CScopedCritical ctor A034E0 / dtor A03530.

Header additions (`fable_handle_memory_pool.h`): the remaining PDB members, the nested `CScopedCritical`,
`CScopedMemoryHandle` (PDB layout; inline Clear/Initialise), and an inline CHeader ctor unless
`FABLE_HANDLE_POOL_HEADER_OUT_OF_LINE` is defined. The previous session's out-of-line `CHeader()`
declaration had broken `gate_pool_free` and `gate_movable_pool_policy` (their fixtures declare header
arrays); both pass again. New `fable_random.h`: `GFRandom` inline from FableWin's body (018CF370).

Source shapes that mattered:
- FindFreeArea writes the bucket clamp as `q < 255 ? q : 255` (quotient on both arms); retail threads the
  clamped arm straight into the loop. UpdateLargestFreeArea needs the `if (index >= 255) index = 255` form,
  so the clamp is written inline per function, not through one helper.
- Allocate's "no failure handler" bit is `(bool)(~(flags >> 1) & 1)`: retail loads the full dword and
  emits `shr / not bl / test bl,1`. `!(flags & 2)`, `(flags >> 1 & 1) == 0` and friends give `test bl,2`.
- Its failure-handler waits are `while (Largest < size) { if (free >= Max + size || !Handle(size)) break; }`
  (rotated loop; the three-term `while` condition re-tests from the top instead).
- InternalAllocateHandle pushes `CHandleInfo info; info.Header = 0;` (literal zero stores); a
  value-initialised temporary uses a zero register.
- GrowFreeArea / RunRandomRelocationTest: an address-taken local (for `CombineAdjacentAreas(CHeader*&)`) is
  stored where its declaration sits, so `CHeader* neighbour = next;` inside the `if (next)` block.
- `if (x) { ...; return true; } return false;` vs the early-return form changes block order (ResizePool,
  InternalAllocate, Allocate(CScopedMemoryHandle&)).

Near, kept in the TU but not gated:
- InitialiseFromPreallocatedBuffer A02610 (6/38 lines: retail adds buffer + size into the size register;
  11 spellings tried).
- RunRandomRelocationTest A029D0 (32/84: `this`/area register swap; 8 spellings tried).

Not moved: Free A02B30 matches exactly when written with `CScopedCritical critical(*this, false);` in this
TU, but its own TU is linked alone by `gate_pool_free`'s behavior fixture, so it stays near there.
Still open in the range: InternalFreeHandle's out-of-line copy A029A0, the scalar deleting destructor,
CMovableResourceMemoryPool.

## CMemoryAllocator::Deallocate inline corrected

FableWin.exe's debug body (02CB6E30) is `EnterSynchronisation(); if (allocation && Get()) {...}
LeaveSynchronisation();`. `fable_memory_allocator_deallocate.h` had the null test before the lock, which
only matched because its two callers' retail code comes from an outer test:
CEngineLandscapeMemoryManager::FreeVirtual is `if (allocation) { Deallocate(allocation);
TrackAllocationCount(-1); }` in FableWin (02CB6E00), and FreeDisposalList goes through
Delete*BlockWhileBusy -> FreeVirtual. Both TUs now spell the outer `if`; landscape_free,
landscape_lifetime, detail_constructors, landscape_disposal, landscape_pool and map_file_selection pass.

## Tooling

- `native_gate_register.py` now extends a TU that already has a `_verified_roots` entry (new roots,
  companions) and refreshes a regenerated gate's gate_all expectation.
- `header_byte_sweep.py` takes header basenames (`fable_stlport_vector.h`), not paths; a path matches 0
  sources and "passes".
- The interrupted front()/back() check from 2026-10-16 is done: 1,655 includer TUs of
  `fable_stlport_vector.h`, 0 changed.

## lib_chunked_file.cpp (continued)

The rest of the file went into the existing TU `00/aa/CChunkedFileChunk_Base_00aa75d0.cpp` (same-file
helpers are expanded by their callers). New gate **chunked_file_tree**, 30 exact roots:
CChunkedFileLoader ctor/dtor/scalar deleting dtor (AA7510/AA7540/AA8D10), CChunkedFileSaver likewise
(AA7550/AA7580/AA8D30), IsParentRecursive AA7650, CChunkedFileRawDataChunk ctor AA76D0 (now out of line, as
in retail) / dtor AA7710 / GetData AA7750 / SetData AA7760, GetNoSubChunks AA78B0,
FindRegisteredSubChunkType AA7950, LoadChunkedFile AA79C0, SaveChunkedFile AA7C70, CChunkedFile ctor
AA7DB0 / dtor AA7DF0, both GetSubChunkAt (AA7E40/AA7E70), CountSubChunksOfType AA7EA0, the const and
non-const FindFirst/FindNextSubChunkOfType (AA7F40/AA7FF0/AA80E0/AA8190), RemoveSubChunk AA8280,
RemoveSubChunksOfType AA82C0, the private SuperChunk copy ctor AA83B0, RemoveAllSubChunks AA8400,
AddSubChunk AA84F0, RegisterSubChunkType AA87E0.

Header (`fable_chunked_file.h`): CChunkedFile / CChunkedFileLoader / CChunkedFileSaver (Ego_r layouts,
vtables 0x129E4E8/0x129E464/0x129E46C), `CChunkedFileException : CExceptionBase` (throw info 0x1369548),
the sub-chunk query/edit members, `mutable CurrentChunkIndex` (the const finds write it).
`fable_counted_pointer.h` gained the declaration of FableWin's converting
`CCountedPointer<const T>(const CCountedPointer<T>&)`.

Shapes:
- LoadChunkedFile throws with plain `throw CChunkedFileException();` under the canonical flags (no EH frame,
  as retail); header words compare with `memcmp(..., 4)`, the magic is the literal `">>>>"`.
- The Find*SubChunkOfType bodies bind `const CCountedPointer<...>& chunk = SubChunks[index];` and return
  it; indexing twice keeps re-reading the array.
- SaveChunkedFile's comment goes through a scoped named copy (`CCharString comment(file.FileComment);`).
- `erase(first, last)` for counted pointers is an inline explicit specialization in the TU around an
  out-of-line `__copy` (0xAA8F40, folded across CCountedPointer types); single-element erase is out of
  line (0xAA91F0).
- Copy and owning `CCountedPointer` constructors are defined inline in the TU (retail expands them here).

Parked (register allocation only; notes in `work/native-recreation/chunked/NOTES.md`):
SuperChunk::ReadFromFile AA8830 (96/324) and CloneSubChunksOfType AA8540 (93/231).

## Typed sub-chunk finders (lib_3d_mesh_file*.cpp, lib_3d_anim_file*.cpp)

FableWin shows the shape: `CCountedPointer<C3DMeshFileBoneChunk> bone;
bone.SetStaticCast(FindFirstSubChunkOfType(C3DMeshFileBoneChunk::GetStaticChunkType())); return bone;`
(FindNext* pass `previous` through the converting `CCountedPointer<const CChunkedFileChunk>`).

- **mesh_file_finders** (35 roots, `00/ab/C3DMeshFileSubMeshChunk_Finders_00aba4a0.cpp`): SubMesh
  FindTransformation (const), Bone/Primitive/Cloth find-first/next pairs, Primitive Get/Peek Vertices and
  Triangles, Triangles/Group/Cloth finders, Has/Get/Peek SmoothingGroups and UniqueVertices.
- **anim_mesh_file_finders** (63 roots, `00/a9/C3DMeshFileRootChunk_Finders_00a98fa0.cpp`): anim root
  objects/helpers, mesh root sub-meshes/material list/helpers, mesh helpers points/dummies/volumes
  (Ego_r really names these `FindFirstVolumeW`), anim helpers movement vector/events, movement track,
  anim object sequences/bone mask, material list materials/engine materials, material extended
  properties/maps, cloth groups.
- New `fable_counted_pointer_inline.h`: opt-in inline copy/owning/converting constructors, operator=,
  ShareData and SetStaticCast (declared in `fable_counted_pointer.h`; FableWin names cited there). The
  chunked base TU now uses it instead of local definitions. Byte sweep of all 1,046 includers of
  `fable_counted_pointer.h`: only the two TUs that need the new declarations differ.
- `GetStaticChunkType()` (Ego_r `SIPBDXZ`, inline) added to the chunk classes used.
- Has* come in two shapes: 128 bytes through a typed local (`!chunk.IsNull()`), 80 bytes straight off
  the search result. HasBoneMask is the odd one: retail keeps the pointer and tests it after the release,
  which is `const CChunkedFileChunk* chunk = ...Get(); return chunk ? true : false;`.

## Already-written bodies, newly claimed

`automatch.py` (scratchpad) compiles a set of TUs and byte-compares every emitted function against the
unlanded retail function starts of a range (relocation sites masked both sides). Over the chunk lane it
found 27 out-of-line copies that existing TUs already emit exactly (default constructors, destructors,
SetParent, SetMaterialIndex, GetName/GetFilename, CChunkedFile's deleting destructor, ...). They are the
**chunk_lifetimes** gate (27 roots across 20 TUs; `merge_gate.py` builds one gate over several sources).
Skipped as ambiguous: a second CChunkedFileChunk dtor/deleting-dtor copy at AC6AD0/AC6AE0 and a
UniqueVertices WriteToFile copy at AC4F50 (the symbol is already indexed in its own TU).

Wide pass (`automatch2.py`: 2,915 TUs in 00/98..00/af against 14,208 unlanded starts in 0x980000-0xB00000,
gap-size prefiltered): only 19 unique symbol/address pairs. Claimed with the SubMesh/Bone setters
(AB9AA0-AB9AE0, AB9E30-AB9E90; out of line in retail, now defined in their Methods TUs) as the
**out_of_line_copies** gate, 18 roots: CThreadedFileObjectDesc ctor 98DEB0 (emitted by the async-data TU;
symbol_index needed a companions entry and its row filter extended), CAFile::CExpectedTextType::Set
997410, ~CLoadingMemoryFailureHandler 9D5EE0, CVertexCompressionParams ctor 9E27B0,
~C3DAnimFileSequenceChunk A55420, the pool's out-of-line GetFreeListIndex A01D00 and
CScopedMemoryHandle::Initialise A039F0. The rest were TUs whose own rows are missing from the index
(e.g. global_operatordelete_009f1f90.cpp) or invented helper names; left alone.
Not reproducible as roots: retail's out-of-line `GetStaticChunkType` copies (AB9A20-AB9A60) — the
inline bodies are only emitted where something takes them out of line.

## lib_memory_allocator.cpp support and statistics

- **allocator_support** (25 roots): the existing, never-gated `00/9f/CMemoryAllocator_OutOfLineSupport_009f1f90.cpp`
  now holds Ego_r's support members at 9F1E80-9F20A0 in order: GetAllocIDFromPMem (special 16MB-block
  range vs the byte ahead of the header), the debug-header pointer helpers, InitDebugHeader/Record,
  Set/GetCurrentTrackingID, InternalValidate (empty), GetDebugHeader, the class `operator new`
  (VirtualAlloc) / `operator delete` (VirtualFree), GFCalloc/GFMalloc/GFFree/GFRealloc, Init/Uninit/
  Enter/LeaveSynchronisation, PeekAllocatorMemoryControlled/Allocated, IsAllocatorFixedSize and the three
  empty stack-tracker hooks. CDebugHeader/CDebugRecord are nested classes (Ego_r mangles them `V`);
  their field names are ours.
- **allocator_statistics** (8 roots, new `CMemoryAllocator_Statistics_009f2400.cpp`, no out-of-line
  switch since retail inlines the lock): GetTotalMemoryControlledByAllocator, GetTotalFixedSizeMemoryAllocated
  (`MaxAllocSize > -1`; `>= 0` gives `test/jl`), GetNoFreeAreas, GetFragmentation (float sum),
  GetHeaderOverhead, GetTotalMemoryUsedWithTrackingID, Validate (InternalValidate's empty body expanded,
  since it lives in the other TU) and Destroy (inline delete).
- Later added to allocator_support (now 29 roots, compiled with /GS): GetDebugRecord 9F2830 (FableWin
  computes an unused offset with one GetAllocationSize call, then addresses the record with a second;
  a compiled-out tracking branch uses 0x48), GetTrackingDebugRecord 9F2850, CheckChecksums 9F29C0
  (`CDebugRecord* record = &GetDebugRecord(pmem);`) and InitTrackingDebugRecord 9F26F0 (inline
  InitDebugRecord, an unused `_stricmp(file, "Unknown")`, `_splitpath` + `strcat`, 47-char `strncpy`).
  CDebugRecord is 0x14 bytes and CTrackingDebugRecord 0x48 (unknown words kept as Unknown0C/44).
- Open here: the anonymous-namespace parsing helpers 9F2C30-9F2D10 and InitializeMemorySystem.

## Chunk accessors

**chunk_accessors** (27 roots over 7 TUs): C3DMeshFileMaterialChunk setters ABFC40-ABFCC0 (CRGBColour by
value assigns as one dword), C3DMeshFileMapChunk SetMapDescription/SetFilename/GetCRC/SetCRC/SetType
ABFF20-ABFF60, C3DMeshFileMaterialExtChunk Get/SetSelfIllumination and the empty private ResetParams
(ABFDC0/ABFDD0/ABFE10), HelperPoint SetPosition/SetSubMeshIndex/SetBoneIndex/SetName AA9860-AA98A0,
Dummy SetDirection/SetTransform/SetUseLocalOrigin AA9A20-AA9A60, ConvexVolume SetName AA9B10 and
ClearPlanes AAAA40 (`Planes.clear()` through the out-of-line CPlane erase), and the mesh Helpers
constructor AA9650 (confirmed through the gated CreateNewInstance's vtable store; HLPR is shared with the
anim helpers chunk, so the masked bytes alone could not tell them apart).

Cross-gate check: every symbol pinned by today's gates has the same address in every other gate that
pins it (2 pre-existing conflicts elsewhere, none involving these).

**anim_file_root_extras** (5 roots, `00/a9/C3DAnimFileChunks_00a98a70.cpp`): the anim root chunk's
out-of-line constructor A98A30 (confirmed by CreateNewInstance's "ANRT"/vtable stores), SetCyclic A98A50,
SetDuration A98A60, and the new C3DAnimFileSaver (file type "3DAF") constructor A98A00 and SaveAnimFile
A98A20 (a tail call into SaveChunkedFile). C3DAnimFile is modelled as a plain CChunkedFile subclass.

## Anim/mesh file loaders and savers

**chunk_loaders** (48 roots over 5 TUs) covers the loader half of lib_3d_anim_file.cpp and
lib_3d_mesh_file.cpp:
- Anim: Register{Root,Object,Helpers,MovementVector}SubChunkType A98BE0-A98DC0, C3DAnimFile() A98E60 and
  C3DAnimFile(root) A98F40, LoadAnimFile A99420, LoadAnimFileCompressed A99510, ~C3DAnimFileLoader A996D0,
  SaveAnimFileCompressed A99840, the C3DAnimFileLoader ctor A999B0, and the ??_G of the saver, file and
  loader. The ??_G bodies replace three CLandscapeBackgroundPatch stand-ins at A9A2C0/A9A730/A9A7A0.
- Mesh: saver ctor A9A870 and SaveMeshFile A9A890, root chunk ctor A9A8A0 plus CreateNewInstance A9A8C0
  and GetChunkDescription A9C9C0, ~C3DMeshFileLoader A9A900, AddChunkType A9AD00, C3DMeshFile() A9AD80 and
  C3DMeshFile(root) A9AE60, LoadMeshFile A9B640, LoadMeshFileCompressed A9B740, ten Register*SubChunkType
  A9B840-A9BA40, SaveMeshFileCompressed A9BA70, the 3,281-byte C3DMeshFileLoader ctor A9BBE0, and the
  ??_G of the saver, file, loader and CMirroredTextureHighlightsChunk.
  - The three root-chunk functions moved out of `C3DFileContainerChunks_00aa9670.cpp`, because retail
    inlines the ctor into C3DMeshFile(). Their file_container_chunks roots and symbol_index rows moved with
    them.
- Shared: CTemporaryBuffer (new `fable_temporary_buffer.h`, Ego_r lib_temporary_buffer.hpp, kept inline;
  retail's per-unit copies are ctor A98860, dtor A98910 and ??_G A98980), plus CCountedPointer::Reset for
  <CChunkedFileChunk> A9CF40 and <C3DAnimFileRootChunk> A9A420.
- Out-of-line ~C3DMeshFileTrianglesChunk ABC890 and ~C3DMeshFileVerticesChunk ABC7B0. Retail calls them
  from the mesh loader ctor, just as it calls the SmoothingGroups/UniqueVertices/Group dtors.

Shape notes:
- The owning `CCountedPointer(T*)` sets Info on both branches.
- `operator=` is `ShareData(other.Data, other.Info)`, as in FableWin. Both live in
  `fable_counted_pointer_inline.h`, and every gate that includes it passes.
- C3DAnimFile/C3DMeshFile build the comment string as a scoped named local, so the temporary's slot is
  reused.
- LoadMeshFile has an `is.SetPosition(is.GetPosition())` pair, written as two statements; the
  single-expression form caches the vtable.
- The compressed loaders wrap the inflated data with the *uncompressed* size, not the decompressor's output
  size. ">>>>" marks an uncompressed file.
- The compressed savers allocate `size*65/64+128` bytes for output and LZO1X_999_MEM_COMPRESS (0x70000) for
  work memory. This needs `fable_stlport_vector_impl.h` plus `<new>` for the inline zero-filled vector(n).
- CTemporaryBuffer's heap fallback is `new[]`/`delete[]` (retail imports `??_U`/`??_V`), so its header
  includes `<new>`.

Near matches, functional and not gated:
- C3DAnimFileRootChunk::GetNonLoopingDuration A99610: register choice in the second release.
- C3DMeshFileRootChunk::CountVertices A9B460 and CountTriangles A9B550: register allocation of the loop
  pointer (see the correction below).

## Mirrored-texture chunk, primitive list accessors, engine-material extras

- **mirrored_highlights_chunk** (9 roots, /GS; new `00/ac/CMirroredTextureHighlightsChunk_Methods_00ac4ec0.cpp`):
  GetChunkInfo AC4EC0 ("Number of verticies: %u", retail's spelling), GetCount, both GetLists, WriteToFile,
  ClearList, CreateNewInstance, AddVertexIndex and ReadFromFile AC5080.
  - The class is now declared in `fable_chunked_file.h` ("MTEH", vtable 0x129E0E0, CArray<unsigned long>
    at +0x10). Its ctor and GetChunkDescription are inline (.hpp).
  - The gate's literal check showed retail's description is "Mirrored Texture Triangle List". The old
    a9d310 stand-in had invented "MirroredTextureHighlights"; it is corrected and superseded by the typed
    COMDAT in chunk_loaders.
- **chunk_loaders** grew to 50 roots: Reset<C3DMeshFileRootChunk> A9CED0 and the mirrored description
  A9D310.
- **mesh_primitive_lists** (20 roots over 5 TUs, new
  `00/ab/C3DMeshFilePrimitiveSubChunks_Accessors_00abc370.cpp` plus the class Methods TUs):
  - GetCount (Group ABA350, Vertices ABA360), const GetLists (Vertices ABBDD0, Triangles ABBF00) and
    Triangles GetList ABC440, FindHighestUniqueVertexIndex ABC010.
  - The SmoothingGroups/UniqueVertices/Vertices ctors and dtors ABC470/ABC4A0/ABC560/ABC590/ABC780 (bodies
    that already existed but were not roots), ClearList for SmoothingGroups ABC4D0, UniqueVertices ABC5C0,
    Group ABC6B0 and Vertices ABC7E0.
  - AddMaterialIndex ABC740, AddFaceGroupMask ABC940, AddUniqueVertexIndex ABCAB0, AddTriangle ABCEC0.
  - Group AddVertex ABCC20: a repeat of the last vertex index adds its blend weight to that entry instead
    of appending (FableWin `back().SetBlendValue(...)`).
  - Addresses were found by masked matching. Same-shaped bodies were assigned by neighbourhood: each
    class's ctor, dtor, ClearList and Add* sit together, and Add* sits just before that class's
    ReadFromFile. Group's two GetLists stay ungated because three identical bodies exist (ABAEC0, ABC370
    and the ABC420 stand-in).
  - The Group TU now has STLport's element-wise `erase` for C3DVertexBlend2 (inline `__copy` loop).
    Vertices ClearList uses that TU's out-of-line erase.
- **engine_material_extras** (6 roots): the CMapFilenames ctor AC5190, dtor AC6210 and ??_G AC61F0 (an
  existing stand-in, now typed), CalculateMapFlags AC5260 (from the filenames when loaded by name,
  otherwise from the bank indices), WriteToFile AC52F0 (version 4) and RemoveMapFilenames AC5740.
- `fable_counted_pointer_inline.h`: ShareData now calls an inline ReleaseData (release, then clear both
  fields), as in FableWin. All 8 gates that include the header still pass.
- Corrected: the root and sub-mesh CountVertices/CountTriangles near-misses are *not* about frame
  alignment; our build aligns the frame too. Retail keeps the loop pointer's fields in registers (so its
  release is `dec [edi]; jne`) and spills `this` and the count, whereas ours keeps the pointer in memory.
  GetNonLoopingDuration is the same pattern. Still open.

## Wave loader

**wave_loader** (6 roots): new `fable_wave_file.h` holds CWaveFile (0x18 with STLport's CArray: Channels,
SampleLength, SamplingFrequency, AudioData<short>), CWaveLoader, CWaveFormatHeader and an empty
CWaveFileException.
- Gated: GetByteLength AAD120, PeekAudioData AAD130, the CWaveFile ctor AAD320, Initialise AAD590 and
  Reset<CWaveFile> AADFF0 (`00/aa/CCountedPointerCWaveFile_Reset_00aadff0.cpp`).
  - The ctor assigns in its body after the vector, and leaves SamplingFrequency unset.
  - Initialise copies the data, then sets SampleLength = size / Channels.
- SaveWaveFile AAD150 is also gated. It writes RIFF/WAVE/"fmt "/"data" with a PCM format chunk; the four
  tags are scoped named locals.
- Functional, not exact:
  - ConvertToMono AAD5D0: retail caches the new buffer's start in a register.
  - LoadWaveFile AAD650 walks the chunks and accepts 16- or 8-bit PCM; 8-bit samples are
    `(b - 0x80) << 8`. It throws on a bad RIFF/WAVE header, a non-PCM format or other bit depths. Its
    bytes differ in stack-slot layout, a field kept in a register, and one string compare that retail
    expands inline.
- Not written: ConvertSamplingRate AAD340 (16.16 linear interpolation). It uses the inline-asm
  GFFloatToLongFloor/Near helpers (see GOTCHAS), so it can only be functional.
- Added to `fable_data_input_stream.h`: inline ReadUWORD/ReadSWORD/ReadUBYTE and the
  `ReadString(length)` declaration (0x996BE0).

## Cloth and material accessors

- **cloth_chunk_accessors** (6 roots): cloth GetChunkInfo AC62B0 (an empty string), GetName AC62D0,
  SetName AC62F0, SetParticleCode AC6300, SetMaterialIndex AC6310 and the cloth ??_G AC69D0.
  - AC6290 also matched the cloth ??_G byte for byte, but vtable slot 0 shows it belongs to
    C3DMeshFileEngineMaterialChunk, whose dtor is A9D270 (see GOTCHAS).
  - The engine material's own ??_G stays a near-miss: retail calls the implicit dtor out of line, while
    ours inlines it.
- **material_family** (7 roots):
  - Map ctors: (type) ABFE20 and default ABFE50.
  - Other ctors: MaterialList ABFB90, MaterialExt AC02B0 and Material(long) AC0A80. All five existed or
    were trivial; the default ones were pinned but not yet roots.
  - HasExtendedProperties ABFCD0: `CountSubChunksOfType(...) > 0`, the `cmp 0,eax; sbb; neg` idiom. It
    replaces a stand-in.
  - CreateMap AC0C00 lives in the Map Methods TU so the map ctor expands into it, as in retail's single
    lib_3d_mesh_file_material.cpp.
  - GetMap (both) in `00/ac/C3DMeshFileMaterialChunk_Maps_00ac0c00.cpp` is functional, not exact. It has
    the same register-held loop pointer issue as CountVertices.
- `fable_counted_pointer_inline.h`: the owning ctor casts `(void*)Data` so `CCountedPointer<const T>(0)`
  compiles.
- Open (register-held loop pointer; CountVertices/CountTriangles ×4, GetMap ×2, GetNonLoopingDuration):
  - I tried inline-body variants against a full copy of rebuild/include (scratch `sra_try.py`). The
    ShareData and Release forms make no difference.
  - A dtor without the `Data = 0; Info = 0;` stores takes GetNonLoopingDuration from 52 to 19 diff lines
    but worsens GetMap. Retail keeps the zero stores for some locals and drops them for others, so the
    real dtor shape is still unknown.
  - The mesh-loader ctor (3,281 bytes) matches with the current dtor.

## Finishing nearly complete files

- **anim_object_extras** (10 roots):
  - C3DAnimFileObjectChunk setters AB0040-AB0080 and MovementVector SetVector AAE110, each just before its
    class's WriteToFile.
  - The anim Helpers, MovementVector and BoneMask ctors AAE090/AAE2A0/AB0700. The candidate addresses
    were told apart by their chunk-type literal (HLPR/MVEC/AMSK; AA44E0 is XALO).
  - ConvexVolume AddPlane AAAAC0.
- **resource_list_debug** (5 roots): CResourceList GetDebugMemoryInUse(frames) 9FC0B0 (newest first,
  stopping at the first resource unused for longer), ResetDebugStats 9FC100, GetFramesSinceResetDebugStats
  9FC140 and Validate 9FC360 (an empty walk).
  - These four live in the GetUnusedFrames TU so that it expands.
  - CResourceBank::RemoveResource 9FC3C0 has its own TU: in the AddResource TU it broke the frame-data
    fixtures' link.
- CDateAndTime::IsValid (`Month != 0`) is written but not gated: there is no out-of-line retail copy.
- Index oddity, not fixed: `CResourceList_FrameEnd_009fc150.cpp` holds CResourceBank::FrameEnd. Its gate
  checks 9FC430 correctly, but the index row takes 9FC150 from the file name, and 9FC150 is really
  CResourceList::FrameEnd.
- **mesh_x_ctors** (8 roots):
  - X chunk ctors: TextureIndex AA4AD0, MappingInfo AA4BE0, BoundingBox AA5000, TriangleStrip AA5A30 and
    UVTangent AA5AC0. Masked matches were ambiguous; each was assigned by its literal (XTID / XMIF / XBDB /
    XTST / XUVT).
  - XUVTangent ClearTangents AA5AF0: an inline `erase` over STLport's out-of-line element-wise `__copy`
    for C3DVector (0x74CA20), specialised at the top of its TU.
  - C3DMeshFileTransformationChunk(const CMatrix3x4&) AB9BB0 and C3DMeshFileBoneChunk(long, const
    CMatrix3x4&) AB9D50.
- Written but not located, so ungated:
  - XTriangleStrip::Initialise, XUVTangent::AddTangent and XVertexData::SetVertexData (resize, then memcpy
    of count * size bytes).
- Deferred: XAnimatedPrimitiveBlockChunk ("XAPB", 0x60).
  - Its ctor AA5B90, ClearVertices AA5BD0, CreateNewInstance AA5C20 and ReadFromFile AA6C70 need a
    CVertexGroupBlock (CArray<ulong> plus map<ulong,long>; its ctor is out of line at 0xAD9030).

## Vertex group block (lib_vertex_group_block.cpp, start)

New `fable_vertex_group_block.h`: CVertexGroupBlock (0x18 in retail: CArray<ulong> Groups, then
map<ulong,long> GroupToIndexMap at +0x0C). **vertex_group_block** (4 roots): ctor AD9030, ctor(capacity)
AD9070 (reserve), Clear AD90C0 and AddGroup AD91F0 (`push_back`, then `insert(group, size()-1)`).
- The TU specialises STLport's `_M_find` inline for this map (retail expands the lower-bound walk), plus
  `erase(key)` as equal_range plus range erase.
- Near:
  - AddGroupsFrom AD9260: one line; retail re-reads the header for `end()` after the inline find.
  - RemoveLast AD9470: retail inlines `map::erase(key)`, which ours keeps out of line unless the shared
    tree header changes.
  - In IsSubsetOf AD8C30 retail calls an out-of-line `find` (folded at 0x6699C0) while AddGroupsFrom
    inlines it, so the inlining split differs per caller.
- Not started: the CVertexGroupBlockOptimiser algorithms.

## Byte-matcher sweep over the chunk sources

I re-ran the masked matcher with all 105 chunk/wave/mirrored TUs over A98000-AC8000 (763 raw hits; most
are tiny generic bodies).
- **chunk_static_types** (18 roots): out-of-line `GetStaticChunkType` copies. Each is
  `mov eax, literal; ret`, so I assigned it by the tag it returns (scratch `resolve_gsct.py`); "HLPR"
  (mesh and anim helpers) is left out. Examples: AOBJ A98830, SUBM A9A7F0, TRFM AB9A20, MMAP ABFB10.
- **implicit_copies** (2 roots):
  - the implicit ~C3DMeshFileEngineMaterialChunk A9D270, which its ??_G AC6290 calls;
  - CWaveFormatHeader() AAD100, in the wave block.
- Not taken:
  - generic setters (SetFileComment, SetBlendValue);
  - second copies of CMapFilenames ctor/dtor, InitNewInstance and ~CChunkedFileChunk; the owner of
    identical bodies cannot be told.
- **vertex_group_block** grew to 10 roots:
  - ~CVertexGroupBlock A96DA0. It is out of line in retail and is now declared, with an owning
    `map<ulong,long>::~map` specialisation (clear, then free the header), matching map<CCharString,long>.
    Before this, our map model leaked the nodes.
  - The XAPB chunk (new `fable_mesh_file_x_animated_block.h`): ctor AA5B90, ClearVertices AA5BD0
    (element-wise erase for C3DIndexedBoneWeight), CreateNewInstance AA5C20, ??_G AA7460 and dtor AA7480.
  - CVertexGroupBlock's copy operations are declared private because the map model has no deep copy.
    SetGroupBlock, which needs operator=, is not written. Its WriteToFile AA5720 and ReadFromFile AA6C70
    are still open.
- XAPB, continued (vertex_group_block is now 13 roots):
  - WriteToFile AA5720: groups, bones per vertex, vertex indices, then index and weight pairs; weights go
    out through `(unsigned long)(float)`.
  - ReadFromFile AA6C70 (928 bytes): needs the vector impl so both `resize`s expand.
  - GetChunkDescription AA73B0 ("Animated Primitive Block"), replacing a stand-in.
  - AddVertex is written but not located.
- mesh_x_ctors: + XVertexData::SetVertexData AA6640.
  - Retail first checks the 64-bit byte count and returns if it passes 32 bits (FableWin asserts), then
    does resize(n) and memcpy.
- **anim_file_x_loader** (2 roots): C3DAnimFileXLoader ctor AA4710 (XSEQ under Object and MovementVector,
  XALO under Helpers) and its ??_G AA4A20.
  - It sits in the XSequence TU (lib_3d_anim_file_x.cpp).
  - The XALO ctor became inline in the header, as retail expands it. Its own copy at AA44E0 was never
    gated.
- Deferred: the C3DMeshFileXLoader ctor AA5C80 (2,288 bytes). Retail inlines every X chunk ctor and dtor
  from lib_3d_mesh_file_extras.cpp; ours are spread over seven gated TUs, so the X TUs would first need
  merging into one.
- **primitive_chunk_info** (2 roots): SmoothingGroups GetChunkInfo AB9F80 and UniqueVertices GetChunkInfo
  AB9FA0, located through vtable slot 2. Both return "" (literal 0x122D70E). This replaces the AB9F80
  stand-in, which had invented "SmoothingGroups".
- The non-const C3DMeshFileSubMeshChunk::FindTransformation is written. It is not located yet; the first
  search ran while the index was being rebuilt.

## X-format mesh chunks merged into one TU

Retail lib_3d_mesh_file_x.cpp is one unit, so I merged our six X TUs into
`00/aa/C3DMeshFileXChunks_00aa4b50.cpp` (scratch `xmerge.py`).
- The removed TUs were XLeafChunks, XVertexData/XUVTangent/XTriangleStrip Methods and XAPB Methods.
- All 46 gated roots that came from them still MATCH in the merged TU.
- I repointed 8 gates and folded the symbol_index.py `_verified_roots`/companions rows into the merged
  key. The provider rows and the ARTIFACT_INDEX.tsv rows of the removed files are dropped.
- New in the merged TU (mesh_x_ctors is now 14 roots):
  - the C3DMeshFileXLoader ctor AA5C80 (2,144 bytes). It needs every X chunk ctor and dtor to be inline
    here, plus inline counted-pointer bodies;
  - its ??_G AA7440;
  - the ??_G of XVertexData AA7250, XTriangleStrip AA72A0 and XUVTangent AA7370 (confirmed by vtable
    slot 0; the first two replace CParticleEffect stand-ins).
- All 52 chunk-header gates pass.
- Still not located: XTriangleStrip::Initialise, XUVTangent::AddTangent and XAPB::AddVertex.
  Not taken: two masked hits on generic C3DIndexedBoneWeight setters (AA4A40/AA4A50).

## Deleting-destructor sweep

**deleting_dtors** (40 roots over 32 TUs). The scratch `gsweep.py` takes every landed ctor, reads the
last vtable it stores into `this` (the most-derived class), and looks at slot 0. If no `??_GClass` row
exists there, `gtest.py` compiles `??_GClass` from the ctor's TU.
- 56 candidates: 42 MATCH, minus 3 already gated in mesh_x_ctors.
- Classes whose slot 0 collides with another class's are skipped.
- Where a ??_G calls an unlanded dtor, `dsweep.py` tests `??1Class` there: the BoneMask dtor A9A7C0.
- Coverage: most chunk classes, CDiskFileWin32, CSurface, CFrameRateSmoother, CBankFileManager,
  CDefinitionManager, CConsole, the graphic/mesh bank managers and others. Many replace generic
  CLandscapeBackgroundPatch/VecDel stand-ins.
- All 46 gates that touch those TUs pass.
