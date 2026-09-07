# Active task list — decomp, frontend parity, and downstream tools

## ACTIVE — curated decomp promotion pass (2026-08-26)

- **LANDED:** `0x00890FF0 CGameScriptInterface::CameraDefault`. The recovered
  115-byte body is a `RELOCATION_MATCH`. Null/blocked camera targets preserve the
  layer counter; accepted targets resolve the manager, release scripted control
  when the counter is negative, or reset the camera and invoke virtual slot `0x1C`
  exactly `counter + 1` times before storing `-1`. Focused behavior and selected
  catalog build pass.
- **LANDED:** `0x00A0D340 CProcessedInput::AddGameEvent`. The natural aggregate
  assignment reproduces all 71 retail bytes as a raw `MATCH`: set processed type
  `1`, copy one 40-byte event into inline slot `NoGameEvents`, overwrite its player
  byte from the processed-input owner, then increment the byte count. Slot isolation,
  copy fidelity, overwrite behavior, and selected catalog build pass.
- **COMPILER-MODE RESIDUE:** `0x004165E8 CMainGameComponent::CheckSync` is
  behavior-complete at `DIFFER(132v134)`. Retail filters type-`1` events, copies
  each into a 32-byte read buffer, reads three `u32` values, and discards them
  along with the local world checksum—there is genuinely no comparison. Corrected
  helper addresses/ABIs are covered; the residue is retail's optimized frame-pointer
  and register-allocation combination.
- **LANDED:** `0x009F19A0 CGameEventPackageSet::CompressIntoBuffer`. The
  corrected 215-byte body is a raw `MATCH`. It serializes the package-count byte,
  then each `[event-count byte][u32 sequence]` header and dense event records using
  the genuine `Type/Player/Data/EndPos/Replacement` layout. Empty sets/packages,
  replacement bit, partial-word payload, multi-package offsets, and selected build pass.
- **LANDED:** `0x004AEAA0 CNetworkClient::GetLocalGameEventPackageSet`. The
  recovered 252-byte `/GS` body is a `RELOCATION_MATCH`. Only an exact update result
  of `1` builds the type-`1` event containing checksum1, checksum2, and the old frame;
  the incremented frame becomes the local package sequence before output copy and
  local-package clear. Both gate paths, payload ordering, lifecycle, and selected build pass.
- **REGISTER/LOCAL RESIDUE:** `0x0041726D CMainGameComponent::UpdateFromEventPackageSet`
  is behavior-complete and exact-length `DIFFER(189v189)`. It applies only strictly
  newer sequences, derives the caught-up flag before advancing input frame, forwards
  world/display feedback, updates `current=max(current,sequence)`, and sends frame,
  checksum, and seed through the embedded client. Correct `+0x1613C/+0x161E1`
  offsets and all ordering constraints are fixture-covered.
- **LANDED:** `0x009F1810 CGameEvent::CompressIntoBuffer`. The corrected
  stepwise output cursor and intrinsic payload copy reproduce all 82 bytes as a raw
  `MATCH`. Empty/seven-byte payloads, player/type fields, replacement-bit encoding,
  returned length, and selected catalog build pass.
- **LOCAL-SPILL RESIDUE:** `0x009F1870 CGameEvent::InitFromCompressedBuffer`
  is behavior-complete at `DIFFER(118v124)`. It decodes the replacement bit and
  15-bit type, player and payload length, sets valid/end-position state, copies the
  payload intrinsically, and returns bytes consumed. Retail spills player/length and
  zero-loads its type accumulator; forcing those spills expands beyond retail.
- **QUEUE QUARANTINE:** `0x00891940 SetQuestInfoName` has the same poisoned
  text-bank lift family as `SetQuestInfoText`: it invents a morph-entry pair for
  an opaque lookup token and aliases incompatible stack-object/handle lifetimes.
  Both generated copies now require semantic review and byte-led reconstruction.
- **QUEUE QUARANTINE:** `0x00891CA0 GetHero` is not the generated six-level
  predicate/morph `operator&&` chain. Retail resolves a target, obtains a script thing
  or fallback, assigns `this+0x30`, and releases a shared local handle. Both generated
  copies are marked until those genuine helper types are reconstructed.
- **LANDED:** `0x004AE940 CNetworkClient::InitialiseAsLocal`. The recovered
  74-byte body is a `RELOCATION_MATCH`; failure leaves state untouched, while
  success clears replication pointers/frame, stores `CMainGameComponent*` at
  `+0x2678`, and raises `+0x2660/+0x2662`. The selected build passes.
- **LANDED:** `0x00449D20 CPlayerManager::IsMultiplayerGameActive`. Modeling
  `+0x0C..+0x10` as the original vector-like player store reproduces the full
  111-byte scan under relocation normalization. Empty/sparse slots, neutral-player
  exclusion, null/blocked activity, active co-op detection, and selected build pass.
  This restores the retail predicate only; it does not activate incomplete multiplayer.
- **LANDED:** `0x00891720 CGameScriptInterface::ChangeHeroHealthBy`. Retail
  forwards the first boolean to the health vtable slot and uses it to gate the
  stabbed-to-death action, while the second boolean independently gates the GUI
  damage notification. The recovered 201-byte body is a `RELOCATION_MATCH`; the
  independent-flag fixture and selected catalog build pass.
- **CATALOG CORRECTED:** `0x00437940` is `CPlayerGui::AddDamageChange`, not the
  poisoned STL `_Cons_val` identity. Its 11-byte body loads `CDrawDamage` from
  `CPlayerGui + 0x1C0` and tail-forwards `(float,bool)`; exact parity and the new
  forwarding fixture pass.
- **QUEUE REPAIRED:** the generated promotion queue now excludes addresses already
  classified `bounded` by the active parity queue. This removed 61 stale residue
  entries, including `ApplyScriptBrush`, and its report now renders already-qualified
  owner names without duplicating the module. `SetThingAsConscious` is explicitly in
  semantic-review quarantine until its implausible generated action-helper identities
  are recovered from retail.
- **PERMUTER RESIDUE:** `0x0089AF30 RemoveAllCutSceneSkippedMessages` is
  behavior-complete and `DIFFER(182v182)`. Retail's type-`0x32` query, matching
  message unlink/destruct/free path, unmatched preservation, and temporary-list
  cleanup are all covered; only local initialization and cleanup register scheduling
  differ.
- **PERMUTER RESIDUE:** `0x00891170 PlaySoundAtPos` is behavior-complete and
  `DIFFER(164v164)`. Sound-symbol lookup, attached-emitter creation and validity
  gates, all eight `CSoundInit` fields, the genuine `CThingSoundEmitter::PlaySample`
  ABI, success return, and invalid-handle fallback are covered; only free `ECX`/`EDX`
  coloring for the retained sound-global pointer differs.
- **PERMUTER RESIDUE:** `0x00893E70 MsgOnGameSavedManually` is behavior-complete
  and `DIFFER(80v80)`. It uses the distinct world-frame `+0x18` and `+0x1C`
  accessors, builds the type-`0x3E` two-bound visibility filter, and returns whether
  a newest message exists; only three independent filter-local stores are scheduled
  in a different order.
- **QUEUE QUARANTINE:** `0x00891A00 SetQuestInfoText` now carries the established
  semantic-review marker in both generated copies. Its lift conflates text-bank
  lookup with a morph-entry allocator/template and aliases incompatible local
  lifetimes, so it must be reconstructed from retail bytes before promotion.
- **PERMUTER RESIDUE:** `0x008943C0 MsgIsTutorialClickedPast` is behavior-complete
  and `DIFFER(125v125)`. The fixture covers the global tutorial gate, the manager
  `+0x0C` selector, primary `+0x17` and alternate `+0xD7` enable bytes, both
  default-true paths, and the type-`0x12` two-frame newest-message predicate.

- **LANDED:** `0x0088F610 CGameScriptInterface::IsSoundPlaying`. The authored
  11-byte vtable tail-dispatch is `RELOCATION_MATCH`, its focused provider / slot /
  argument / return fixture passes, and the selected catalog build passes.
- **LANDED:** `0x0088F710 EntitySetCombatEnabled`. Its four-byte `CCharString`
  temporary and genuine member-function-pointer dispatch reproduce all 189 bytes
  under relocation normalization; ordered four-state behavior and selected build pass.
- **LANDED:** `0x0088FDC0 SetTimeOfDay`. A direct volatile wrap-threshold read plus
  direct bar-scale read reproduce all 60 bytes under relocation normalization;
  multi-wrap, exact-wrap, negative-value behavior and selected build pass.
- **LANDED:** `0x0088FE90 FastForwardTimeTo`. The adjacent recovered wrap/scale
  shape reproduces all 65 bytes under relocation normalization; wrapped time,
  independent speed forwarding, focused behavior, and selected build pass.
- **PERMUTER RESIDUE:** `0x0088F480 ApplyScriptBrush` is behavior-complete and
  `DIFFER(25v25)`. Retail passes the brush/create flag only to virtual slot `0x30`,
  then invokes a zero-argument notification on the returned quest-info object.
  Current genuine C++ differs only in the `EAX`/`EDX` scratch-register assignment.
- **PERMUTER RESIDUE:** `0x006E7690 DeregisterTimer` is behavior-complete and
  `DIFFER(75v74)`. The timer-map owner, end-sentinel branch, erase/free paths, and
  count decrement are recovered; only the `foundNode = this` local initialization
  spelling remains different.
- **LANDED:** `0x0088FC90 HeroHasExpression`. Modeling both hero resolver stages
  as zero-argument member calls reproduces all 40 bytes under relocation
  normalization; null, blocked, available, unavailable, and selected-build gates pass.
- **LANDED:** `0x008AA010 ClearGossip`. A four-byte key copy, map lookup, complete
  entry-range erase, and both string lifetimes reproduce all 64 bytes under
  relocation normalization. The fixture records the key during lookup (never through
  a destroyed temporary), and its selected build passes.
- **SCHEDULING RESIDUE:** `0x0088FD00 EntitySetInLimbo` is behavior-complete and
  `DIFFER(82v84)`. Retail retains saved `EBX` until the epilogue; VC7.1 restores it
  immediately after testing `inLimbo`. Null/blocked and all auxiliary-flag paths pass.
- **EVALUATION-ORDER RESIDUE:** `0x0088E2A0 IsLevelLoaded` is behavior-complete and
  `DIFFER(87v94)`. Retail preserves the name/map-number argument across its receiver
  getter calls; current C++ resolves the receiver first. Zero-map, absent, pending,
  and proximity-loaded branches pass.
- **BIT-UPDATE RESIDUE:** `0x00894DD0 EntitySetAsMarryable` is behavior-complete and
  `DIFFER(124v131)`. VC7.1 combines the adjacent bit-3/bit-4 assignments while retail
  keeps two update stages and preserves `EBX`; null, blocked, wrong-type, disable,
  add-CTC-wife, and already-present CTC branches pass.
- **REGISTER-ALLOCATION RESIDUE:** `0x008984E0 SetWeaponOutCrimeEnabled` is
  behavior-complete and `DIFFER(91v91)`. Resolver, eligibility, lower-bound,
  end-sentinel, high-key, enable, and disable paths agree; retail assigns target/map
  temporaries to unsaved `EDI` and saved `ESI`, unlike ABI-preserving ordinary C++.
- **REGISTER-ALLOCATION FAMILY RESIDUE:** `0x0089B3F0 SetHeroAsWearing` is
  behavior-complete and `DIFFER(94v96)`. It confirms the same unsaved-`EDI` target /
  saved-`ESI` embedded-map pattern as `SetWeaponOutCrimeEnabled`; lookup, sentinel,
  high-key, and item forwarding paths pass.
- **LANDED ALLOCATOR FAMILY (62):** a masked retail scan of the 22-byte output-pointer
  allocator grammar now has all 62 authoritative instances landed. The family includes
  the original ListNode variants, queue/stack/list initializers, the allocator helpers at
  `0x0042AC52/9F/D10`, 22 closely packed helpers at `0x004DB771..0x004DBCFD`, later
  tree/list helpers through `0x005F895F`, and `0x00CD283C/0x00CD2860`. Every newly
  added member is a size-optimized `RELOCATION_MATCH` with focused behavior and an
  individual selected-build PASS. Retail establishes a three-parameter fastcall ABI
  (`ECX` output, unused `EDX`, one unused stack argument / `ret 4`), compact
  `and [out],0`, and allocation sizes from `0x14` through `0x58` as recorded per member.
- **12-BYTE NODE IDIOM RESIDUE (8):** `0x00429FBE LinkedList_AllocateNode_12bytes`,
  `0x00429FE3 LinkedList_AllocateNode`, `0x0042A008 Std_List_AllocateNode`,
  `0x00493A40 ListNode_AllocateAndInsert`, `0x0053C879 std::list_node::construct`,
  `0x0057F525 LinkedList_AllocNode`, `0x00592AD3 AllocateListNode`, and
  `0x005F807C LinkedList_NewNode` are behavior-complete `DIFFER(26v26)` functions.
  Retail uses `pop ecx; lea ecx,[eax+8]; test ecx`; VC7.1 folds the address test to
  `add eax,8; je` and chooses a different cleanup/register idiom. All eight preserve
  the 12-byte allocation and copy the input dword to offset `+8`.
- **LANDED FREE-POINTER FAMILY (6):** `0x0040E130 Vector_FreeElements`,
  `0x0043F510 FreePointerArray`, `0x0043F560 Vector_FreeElementPointers`,
  `0x0048C820 Array_Free_Elements`, `0x007B3B50 Buffer_FreeAll`, and
  `0x00913690 FreeBufferArray` are each 43-byte `RELOCATION_MATCH` functions.
  Retail corrects the generated prototype to stdcall (`ret 8`); all preserve the
  begin-inclusive/end-exclusive traversal, skip null entries, pass
  empty/null/two-allocation behavior, and pass individual selected builds.
- **LANDED TINY HELPERS (2):** `0x00C61CE0 Getter_TripleDeref` is an exact
  15-byte `MATCH`, and `0x00C62A30 StreamBuffer_AdvancePosition` is an exact
  19-byte `MATCH`. Their three-level pointer load and signed stride/count/2x
  position update fixtures pass, as do individual selected builds.
- **TINY HELPER RESIDUES (2):** `0x00C20EF0 IsStateStreaming_2to4` is
  behavior-complete `DIFFER(46v41)` (stub-call `this` preservation), and
  `0x00E7F7D0 GetFloatTableEntryByIndex` is behavior-complete `DIFFER(34v27)`
  (fixed global-owner/index selection codegen). States 2/3/4 versus all tested
  non-streaming states, and stored indices -1/0/2, are covered respectively.
- **LANDED RB-TREE MAX FAMILY (32):** the original `0x0042A58D`, `0x0042A59E`,
  `0x0042A5AF`, and `0x0042A5C0` plus 28 authoritative RB-tree/max-node twins are
  each 17-byte `RELOCATION_MATCH["s"]` helpers. The additions span `0x0042A5D1`
  through `0x00CD265B`, including the dense iterator groups near `0x005B198D` and
  `0x005BA88B`. Each forwards the iterator's current node to its max-node helper,
  stores the returned maximum, returns the iterator address, passes focused
  forwarding/storage behavior, and passes an individual selected build.
- **LANDED LIST PUSH-FRONT WRAPPER FAMILY (33):** `0x0042B687
  LinkedList_PushFront`, `0x0042B6B8 Std_List_PushFront`, and 31 corrected
  `ListPushFrontWrapper_<address>` twins are 26-byte `RELOCATION_MATCH["s"]`
  member wrappers. Their middle insert argument is a non-trivially copied,
  force-inlined four-byte iterator aggregate, which recovers retail's frame/local-copy
  sequence. All pass focused value/address/iterator forwarding and selected builds.
- **LANDED CIRCULAR LIST/SENTINEL INITIALIZER FAMILY (37):** `0x0042AB01`,
  `0x0042AC0A`, and `0x00450D90` are joined by 34 authoritative 27-byte
  `RELOCATION_MATCH["s"]` twins through `0x0065F2E5`. The recovered fastcall ABI
  uses `ECX` for the output pointer, leaves `EDX` unused, accepts one unused
  callee-clean stack argument (`ret 4`), clears the output, allocates a 12-52 byte
  sentinel, self-links offsets `+0/+4`, publishes the node, and returns the output
  pointer. Two propagated CTC-constructor labels at `0x005792BF` and `0x0057FDA2`
  are corrected to neutral `CircularListInitialize_*` identities. All 37 members
  are behavior-complete and landed; all 34 additions pass individual selected builds.
- **LANDED ACCESSOR/STATE HELPERS (2):** `0x008702D0 Handle_InitInvalid` is an
  exact 17-byte `MATCH` with the recovered handle-pointer return in `EAX`;
  `0x00C44B50 InputEvent_CheckKeyAndGetFrames` is a 34-byte
  `RELOCATION_MATCH` after recovering unsigned key ordering and key forwarding to
  the conditional frames call. Both focused fixtures and selected builds pass.
- **LANDED TREE PAYLOAD ALLOCATION FAMILY (34):** `0x0045C4D2`, `0x00431471`,
  and `0x0045C8A1 TreeNode_AllocData`, plus `0x0042B5A8
  BinarySearchTreeNode_Create`, `0x0042B5CA BinarySearchTreeNode_CreateDuplicate`,
  and `0x0042B5EC BinarySearchTreeNode_CreateCopy`, are each 34-byte
  `RELOCATION_MATCH["s"]` functions. The recovered stdcall contract returns the
  24-byte allocation base in `EAX` (`ret 4`) while copying the input's two dwords
  into the payload at `base + 0x10`. Focused fixtures distinguish the returned base
  from the embedded payload, and every member passes an individual selected build.
  A complete masked retail scan found 28 additional authoritative twins across
  `0x0045F13D..0x005F8C41`; all are landed under the same contract.
- **LANDED LIST UNLINK/FREE FAMILY (7):** `0x00429AB9 DoublyLinkedList_Unlink`,
  `0x00429B02 LinkedList_Unlink_and_Free`, and `0x00429B52
  LinkedList_RemoveNode`, plus `0x0053C7B9 std_list_node_unlink` and the three
  `std_list_node_unlink_and_free` twins at `0x00655125`, `0x006553D0`, and
  `0x006553FD`, are each 32-byte `RELOCATION_MATCH["s"]` functions.
  Retail corrects the generated first member's cdecl/reversed-layout guess to the
  same stdcall (`ret 8`) predecessor/successor layout as its siblings. All seven
  relink both neighbors, free exactly the removed node, update the caller's cursor
  to the predecessor, pass focused behavior, and pass individual selected builds.
- **LANDED ALLOCATE/INITIALIZE WRAPPER:** `0x00433DB5
  STL_AllocateAndInitializeArray` is a 50-byte `RELOCATION_MATCH`. It preserves
  stdcall `ret 12`, allocates `count * 4` bytes only for nonzero counts, forwards
  the source range and destination to `_Uninit_copy`, returns the allocation, and
  passes focused zero/nonzero forwarding behavior plus its selected build.
- **LANDED BUFFER MOVE FAMILY (4):** `0x00436A80 Vector_EraseAndMove`,
  `0x00440550 Buffer_MoveData`, `0x00440590 Buffer_Relocate`, and `0x0040F390
  Buffer_RellocateAndUpdateWritePointer` are 58-byte `RELOCATION_MATCH` functions. Their
  recovered ABI is fastcall with `self` in `ECX`, an unused `EDX` slot, and
  destination/source on the stack (`ret 8`). A register-sized tail local reproduces
  retail's `EAX` source / `ESI` tail schedule. All cover empty-tail retargeting and
  overlapping `memmove`, including returned destination and updated tail, and all
  selected builds pass.
- **LANDED TREE LOOKUP ACCESSOR:** `0x0042CD61 FID_conflict_max_size` is a
  35-byte `RELOCATION_MATCH["s"]`. Retail receives its key by value, passes the
  address of that stack key to a genuine member lookup at `self + 0x144`, returns
  node field `+0x18`, and returns `0xFFFFFFFF` for the header sentinel. Focused
  sentinel/value behavior and its selected build pass.
- **LANDED VECTOR COPY WRAPPER:** `0x00411B00 vector_push_back_copy` is a
  38-byte `RELOCATION_MATCH["s"]`. Its recovered outer ABI uses `ECX` for the
  vector, an unused `EDX` slot, two stack arguments, and `ret 8`; its inner copy
  helper takes first/last in `ECX`/`EDX` plus destination, destination-address,
  and a null tag on the stack. A distinct register result recovers retail's saved
  `EDI` lifetime. Range forwarding, copied values, end update, returned destination,
  and the selected build pass.
- **LANDED VECTOR PUSH WRAPPER FAMILY (2):** `0x004128A0 Vector_PushBack` and
  `0x0044BFF0 std::vector::push_back` are 59-byte `RELOCATION_MATCH` twins. The
  recovered outer ABI uses `ECX` for the vector, an unused
  `EDX` slot, one stack value, and `ret 4`. With spare capacity it copy-constructs
  at the finish and advances by four bytes; at capacity it forwards finish, value,
  value-address, and two unit counts to a genuine member insert. Both paths and the
  selected builds pass.
- **LANDED LIST INSERT FAMILY (33):** `0x0042AB33 LinkedList_Insert_Before`,
  `0x0042AB8A Std_List_Insert`, and `0x0042AADC LinkedList_InsertBefore` are
  joined by 30 authoritative 37-byte `RELOCATION_MATCH` twins through
  `0x006605D5`. Ten already had list-consistent manifest names; 20 poisoned
  Lua/creature-action labels are corrected in `function_overrides.tsv` to neutral,
  address-unique `ListNode_InsertBefore_*` identities.
  All use stdcall outer cleanup (`ret 12`) and a one-value stdcall allocator,
  splice the new node between predecessor and target, publish it through the output
  pointer, return that output pointer, and pass focused link/value behavior plus
  individual selected builds.
- **LANDED LIST-NODE INITIALIZATION FAMILY (62):** `0x0045170F ListNode_Init`,
  variants B/C/D/E/F/G/H/I/J/K/L/M/N/O at `0x00451739`, `0x0045177A`, `0x004517BB`,
  `0x004517E5`, `0x00451835`, `0x004518B7`, `0x00451916`, `0x00451957`,
  `0x00451998`, `0x004519D9`, `0x00451A1A`, `0x00451A5B`, `0x00451876`, and
  `0x00451A9C`, plus `0x004301C9 ListNode_Construct`, `0x0043019F
  LinkedList_Initialize`, and `0x00451ADD CList_Initialize`, plus `0x0042B72B
  CTexture_InitEmpty_56bytes`, `0x0042B76C CTexture_InitEmpty_24bytes`, `0x0042B7EE
  CTexture_InitEmpty_28bytes`, and `0x0042B7AD LinkedList_InitializeNode`, plus 40
  additional authoritative twins from `0x00485AC5` through `0x00CD2D61`, are 42-byte
  `RELOCATION_MATCH` functions. The additions include 19 texture-format handlers,
  container/queue/hash initializers, render-node variants, and the final CList/CTree
  sentinel initializers. Size optimization recovers retail's direct stack
  push into a one-value stdcall initializer. Each zeros owner count, the one-byte
  node state at `+0`, and the dword payload at `+4`, then self-links `+8/+0x0C`.
  Focused state/layout behavior and all individual selected builds pass.
- **BOUNDED LIST PUSH WRAPPER:** `0x00449320 CList_PushBack` is behavior-complete
  `DIFFER(64v64)`. Spare-capacity copy, nullable reference-count propagation,
  eight-byte finish advance, and full-capacity five-argument reallocation forwarding
  pass. Size optimization recovers retail's direct stack pushes but merges the
  epilogue and reloads the source rather than retaining its first dword in `ESI`.
- **LANDED PACKED MEMORY-BLOCK DESTRUCTOR:** `0x004303F0
  CMemoryBlock_Destructor` is a 54-byte `RELOCATION_MATCH`. The true layout is
  one-byte packed with the owned heap pointer at unaligned offset `+0x29`; retail
  conditionally frees it, installs the derived vtable, calls the base destructor,
  conditionally invokes delete for flag bit 0, and returns `this`. Both flag paths
  and the selected build pass.
- **LANDED UNSIGNED-16 RANGE COMPARATOR:** `0x0040F0A0
  Compare_Unsigned16Array` is a 56-byte `RELOCATION_MATCH`. It rejects unequal
  halfword counts before calling the element comparator and returns one only for
  equal content. Equal, length/content mismatch, empty, and high unsigned values
  pass, as does the selected build.
- **LANDED TREE LOOKUP ACCESSOR TWIN:** `0x0042CD3E FID_conflict_max_size` is a
  35-byte `RELOCATION_MATCH["s"]` twin of `0x0042CD61`, with its embedded tree at
  `self + 0x150` rather than `+0x144`. Stack-key address forwarding, sentinel/value
  results, and the selected build pass.
- **BOUNDED INITIALIZER RESIDUE:** `0x00450CCC Buffer_InitializeWithSize` is
  behavior-complete `DIFFER(46v46)`. The recovered member ABI has two stack args
  and `ret 8`; zero/nonzero allocation, all three pointers, end arithmetic, and
  returned `this` pass. Only retail's early `ESI = this` prologue schedule differs.
- **BOUNDED ARRAY INITIALIZER RESIDUE:** `0x00450E3A Array_Initialize` is
  behavior-complete `DIFFER(52v52)`. Its two-stack-argument member ABI (`ret 8`),
  zero/nonzero `count * 4` allocation, begin/current/end values, and returned `this`
  agree; it confirms the same early-`ESI = this` prologue scheduling residue as
  `Buffer_InitializeWithSize`.
- **BOUNDED VERTEX-FILL RESIDUE:** `0x00451583 VertexListInsertVertices` passes
  initialization forwarding, repeated-byte fill, zero/negative counts, end update,
  and returned `this`, but remains compiler-scheduling residue around retaining
  versus reloading the count across the initialization member call.
- **BOUNDED VECTOR COPY RESIDUES:** `0x00440E00 Vector_CopyElements` is
  behavior-complete `DIFFER(58v59)` under both RTM and QFE VC7.1 due register and
  loop-alignment choices. `0x0043212B Vector_AllocateAndCopy` passes zero/nonzero
  allocation and two 12-byte element copies; a typed triple improves parity from
  103 to 88 bytes versus 80 retail, but VC7.1 still folds the retained delta into
  an incremented destination and saves extra registers. Neither is falsely landed.
- **LANDED GUI TREE-PANE DESTRUCTORS (4):** `0x00443BD0/0x00443C10
  CGuiControlTreePane_Destructor_2/3` are 59-byte `RELOCATION_MATCH` twins; they clear the
  tree at `+0x64`, free its storage, invoke base cleanup, conditionally delete for
  flag bit 0, and return `this`. `0x00444E10/0x00444E50
  CGuiControlTreePane_Destructor_4/5` are 60-byte `RELOCATION_MATCH` twins; they are
  null-safe and clear `+0x68` then `+0x64` before
  free/base cleanup/unconditional delete. Ordered/null/flag behavior and individual
  selected builds pass.
- **LANDED POLYMORPHIC RANGE CLEANUP FAMILY (19):** the seven previously cataloged
  `Catch_0047e8cc` bodies from `0x00443540` through `0x00443B20` plus 12 `_gapscan`
  twins now named `PolymorphicRangeCleanup_<address>` are 63-byte
  `RELOCATION_MATCH` functions. Each is null-safe, walks `[begin,end)` in 8-byte
  objects, invokes virtual slot zero with flag 0, frees non-null backing storage,
  and deletes the owner. Null and three-entry behavior plus all selected builds pass;
  the complete masked retail scan is 19/19 landed.
- **BOUNDED BUFFER-ZERO FAMILY (2):** `0x00411910` and `0x00410F30
  Buffer_AllocateAndZero` pass zero and seven-byte allocations, dword/tail-byte
  clearing, pointer endpoints, and returned `this`. Manual loops produce 114 bytes;
  intrinsic `memset` produces the correct `rep stosd`/`rep stosb` core but folds
  retail's separately live allocation/write registers to 69 versus 77 bytes. Neither
  is falsely landed.
- **BOUNDED TREE-PANE DESTRUCTOR 1:** `0x00443840
  CGuiControlTreePane_Destructor_1` is behavior-complete `DIFFER(76v76)`. Null,
  fastcall sort cleanup at `+0x28`, 8-byte virtual range destruction, backing free,
  and owner delete agree; retail keeps owner/end/current in `EBX`/`EDI`/`ESI`, while
  VC7.1 assigns owner/end to `EDI`/`EBX`.
- **BOUNDED BACKWARD-FILL/ERASE RESIDUES:** `0x0040FBF0
  CWideString_FillBackward` passes overlapping backward copy and empty-range behavior
  with its recovered four-parameter fastcall / `ret 8` ABI, but VC7.1 assigns the
  result/count lifetimes differently. `0x00440D70 Vector_EraseRange` is
  behavior-complete `DIFFER(77v77)` after recovering the shared five-argument move
  helper, 8-byte virtual destruction loop, finish update, and returned first iterator;
  only saved-register assignment/store scheduling differs.
- **BITSET CURSOR RESIDUE:** `0x00410D80 bitset_advance_iterator` is
  behavior-complete `DIFFER(50v48)`. Its recovered ABI is eight-argument stdcall:
  state in arguments 1/3/4/5 and three trailing unused arguments (`ret 32`). Retail
  retains `[cursor+4]; cursor+=4`; VC7.1 hoists the increment and loads `[cursor]`.
- **ORACLE BOUNDARY QUARANTINE:** `0x00449B60 GetMultiplayerColour` is not ready
  for promotion from the PE fallback: its 163-byte extracted span contains the switch
  table and a following function. Obtain an authoritative Ghidra boundary first.
- Continue down `rebuild/backlog/PROMOTION_QUEUE.md`, preferring bounded wrappers
  over the 9.5 KiB `GFHandleSystemInitError` until the small CGameScriptInterface
  cluster is exhausted. All work remains offline; runtime and visual gates are deferred.

## ACTIVE — donor-free ForgeTest64 runtime package (2026-08-21)

Shutdown handoff: `docs/SESSION_HANDOFF_2026-08-21_NO_DONOR.md` holds the
authored artifacts, hashes, corrected binary layouts, verification commands, and
dirty-worktree warning. Its runtime state and continuation order stop at stage7
and are SUPERSEDED — see "Runtime stage ledger — stage7 through stage21" below
for the actual frontier, the stage20 reference result, and the crash diagnosis.

Objective: ship the authored 64x64 ForgeTest heightmap as a genuinely new map.
Darkwood-derived LEV/TNG/STB topology is not an acceptable final artifact.

Acceptance gates:

1. **Source identity.** Use `D:\Downloads\penis_terrain_pack.zip` (SHA-256
   `e902e642e4c61bd36e811844ed4c31a064e1125a95c83a5df487a56d1a45eeef`):
   its normalized 64x64 float heightmap and three authored diffuse/bump/cliff
   texture families. The current imported LEV is 64x64 with a 65x65 vertex
   grid, heights 72.000..83.962 and center 81.949. Emit an empty 12-byte
   `ForgeTest64.tng`; the test NPC is
   created only by ForgeFSE after arrival.
2. **World identity.** Slot 399 has one containing region (retail-style strict
   partition), in a dedicated ForgeTest region whose only contained map is
   ForgeTest64. Region 55 remains a diagnostic fallback, not the final owner.
   WLD/BWD/STB bounds remain
   `(3328,2304)..(3392,2368)` and `forge validate` is CLEAN.
3. **No donor package command.** The package build must not accept or invoke
   `--from-donor`, `level create-from-donor`, or a donor map/chunk path.
4. **Fresh STB terrain.** Generate the InfoBlock, texture/material palette,
   foreground patch-header directory, background tree/composed frames, foreground frames, and
   local-detail-empty section from authored inputs. Do not retain donor frame
   allocation, layer membership, indices, textures, scenery, or opaque tails.
5. **Structural gates.** Every emitted frame starts on a 2048-byte boundary;
   all quad AABBs lie inside the map box; all offsets/spans are in range;
   foreground codec round-trip and generated topology pass; STB table offsets
   are monotonic and the common-header name index is sorted.
6. **Runtime gates.** Teleport survives, reports
   `map=ForgeTest64 pos=(3360,2328,...) in_target_bounds=true`, renders the
   authored terrain under the hero, contains no donor ruins/entities, and the
   scripted NPC spawns/interacts successfully.

Execution order:

1. **DONE — background tree/file blocks.** Generate the 4x4 leaf patches,
   parent/root LOD frames, AABBs, band records, and page-aligned file-block
   references entirely from the authored heightfield and inline textures.
2. **DONE — canonical empty local detail.** The native empty root is a 20-byte
   aligned file block (`groupCount=0` plus four absent-child markers), followed
   by the wired 0x2C quadtree header and five-byte empty object palette. The
   12-byte descriptor and every absolute span/offset are now assembled and
   regression-tested by `buildEmptyLocalDetailSection`.
3. **DONE — pure chunk builder.** Add `forge stb create-terrain` consuming the
   final LEV, placement, and nine inline texture blobs without opening a donor
   chunk. Assemble all control, geometry, palette and directory sections.
4. **DONE — offline adversarial gates.** Prove deterministic output, 2048-byte
   frame alignment, in-box AABBs, pointer closure, codec round-trips, generated
   topology, and absence of donor names/chunk bytes.
5. **DONE — dedicated-region installer.** Create slot 399 plus one dedicated
   region whose only contained map is ForgeTest64. No region-55 attachment and
   no dual ownership.
6. **ACTIVE — corrected-control runtime smoke.** Deploy only stage8 through
   the proven rollback harness. Confirm static-map loading survives before
   evaluating region ownership, teleport, rendering, or NPC interaction.
7. **NEXT — smoke automation.** Derive teleport Z from the authored center
   height plus clearance, deploy through the rollback harness, launch, monitor
   logs automatically, and retain screenshot/log/hash evidence.

Live implementation checklist (2026-08-22):

- [x] Recover native background and local-detail header/file-block write order.
- [x] Assemble canonical empty local detail with closed absolute pointers.
- [x] Model the 64x64 background split tree and parent/root LOD payloads. The
  deterministic tree has 31 nodes, 16 leaves, and one full-quality authored
  payload per node; higher bands reuse it through native remap records.
- [x] Lay out and backpatch background root/child file blocks. Tree headers use
  native postorder, all 31 payloads are page-aligned, and recursive reparse
  closes every pointer and child partition.
- [x] Assemble the pure terrain chunk and generated common-header InfoBlock.
  `forge stb create-terrain` now emits the authored chunk and 0x5C InfoBlock;
  ForgeTest64 produces 16 foreground frames, 31 background payloads, and closed
  local-detail/background pointers without opening a donor chunk.
- [~] Run deterministic, alignment, bounds, pointer, codec, and donor-absence
  gates. Double-build hashes, foreground round-trip, 31-node recursive tree
  parse, page alignment, and in-box AABBs pass; explicit donor-byte/name scanning
  remains active.
- [ ] Install dedicated region ownership and run rollback-safe runtime smoke.

Constructor checkpoint: the global texture palette, empty local-detail object
palette, and variable-length background tree-node header now have donor-free
serializers in `forge::stbbake`; focused regression tests pass.

The nine 128x128 custom PNGs compile to DXT1 and native 19-byte-header inline
texture streams with successful self-decode checks. `lev import-heightmap` now
resamples the supplied 64x64 float raster onto Fable's required 65x65 vertex
grid. `lev author-surface` replaced every inherited cell theme, walkability and
preferred-path flag, then navigation was rebuilt against the empty TNG. Current
output: `work/no_donor_terrain_pack/ForgeTest64_final.lev`, SHA-256
`efcb8b52da97d5a146759f064b961ad9f105fb40fc536198bfebc9e4e2abbb46`.
Smoke-test Z must still come from the authored center height rather than 74.

The deterministic section planner and InfoBlock constructor are complete, as is
the minimal 64x64 foreground generator. The authored background path now emits
sixteen fresh 16x16 mesh frames from the supplied heightfield and inline DXT,
LZO-frames them on 2048-byte boundaries, and generates matching retail-form
foreground patch headers/AABBs at directory base `0x800`. Artifact:
`work/no_donor_terrain_pack/ForgeTest64_background.*`;
`stb patchinfo` decodes 16/16 mesh patches at the intended bounds. Remaining STB
work is root control-header/file-block integration and canonical empty local detail.

Retail-oracle correction: the 0x24-byte array begins at `0x800`, not `0x7FC`,
and its records are `CEngineLandscapePatch::SaveHeader` values ordered as
`frameOffset, frameSpan, AABB[6], flags`. All 16 live flags are `0x00C92E01`
and the following terminator is entirely zero. The parser, writer, retarget
rewiring, XY/Z bound mutation, synthetic fixtures, and generated artifact now
use that contract. A background-tree header parser/round-trip gate and the
native 12-byte local-detail root descriptor writer are also complete.

Current clone-based stage4 remains a diagnostic artifact only. It proved the
WAD crash fix, correct hero placement, and the duplicate-region failure mode;
it must not be promoted as the custom-map deliverable. Stage5 was not deployed.

### Offline container checkpoint (2026-08-22)

- [x] Background tree/file blocks and canonical empty local detail.
- [x] Pure `forge stb create-terrain` donor-free chunk builder.
- [x] Determinism, alignment, bounds, pointer, codec, topology, and known
  donor-name absence gates.
- [x] Canonical 0x80-byte common record plus isolated STB append. The stage has
  425 entries / 399 static maps; ForgeTest64 is id 426 at aligned offset
  598276096. Its five rebased stream pointers close exactly, and the complete
  597,979,518-byte source prefix matches except the three intentional archive
  header dwords.
- [x] Assemble slot 399 and a dedicated ForgeTest64-only region, then validate
  WLD/BWD/WAD/STB ownership without deploying. `stage7_region` is CLEAN:
  ForgeTest64 has exactly one owning/visible region, no region-55 attachment,
  authored loose LEV/TNG sources, and zero bounds/reference/static-map issues.

### Runtime stage ledger — stage7 through stage21 (2026-08-22)

The stage7 entry that used to close this section is SUPERSEDED. Fourteen further
runtime packages were built and deployed on 2026-08-22; the notes below are
reconstructed from `work/no_donor_terrain_pack/runtime_evidence/` and the stage
package hashes. Read this before touching the ForgeTest64 lane.

| Stage | STB size | Outcome |
|---|---|---|
| 7-10 | 599,214,598 | `OpenRetailStaticMap(ForgeTest64)` reached; crash at `Fable.exe+0x59EC13` before quest startup. Rollback set `20260822_090312`. |
| 11 | 599,214,598 | `host95` probe. Measured `region_vector_size=142`, `slot_399_owner_region=95`. |
| 12 | 599,214,598 | Retail `EntityTeleportToPosition(3360,2328,82.949)` returned ok=true; crashed post-teleport. |
| 13-15 | (build dirs only) | `*_lodfix_host95` iterations, never packaged for deploy. |
| 16 | 600,218,118 | LOD fix. Background render probes fire, but all are retail-scope. |
| 17 | 600,238,598 | Local-coords fix. Foreground/texture probes fire, retail-scope only. |
| 18 | 600,238,598 | Local detail removed — no change in failure class. |
| 19 | 600,238,598 | Landscape section removed — no change in failure class. |
| 20 | 600,238,598 | **Best result to date.** See below. |
| 21 | 601,668,102 | `edge_cursor`. Differs from stage20 ONLY in the STB. Crash is debugger-heap noise, not terrain — see the diagnosis section. |

**Stage 20 is the reference result.** Its FSE log
(`runtime_evidence/20260822_stage20_wad_registered/FableScriptExtender.log`) shows:

- `[StaticMapProbe] opening 'Data\Levels\FinalAlbion\ForgeTest64.lev'`
- `[StaticMapProbe] opened state=1 bankIndex=426 bounds=(3328,2304,72)..(3392,2368,83.962)`
- `FORGE: arrival verification map=ForgeTest64 in_target_bounds=true`
- `FORGE: spawned ForgeNPC (CREATURE_GHOST_VILLAGER_FEMALE) at (3364.0,2330.0,78.7) -> OK`
- Hero handle acquired and released with clean refcounting.

That satisfies acceptance gates 1-5 and the first clauses of gate 6. **Zero
ForgeTest-scope terrain render probes fired** before the crash, so the authored
terrain has still never been drawn.

**Gate 2 is NOT met by any deployed stage.** Every stage from 11 onward reports
`slot_399_owner_region=95` — the packages host ForgeTest64 in retail region 95,
not the dedicated ForgeTest region validated offline in `stage7_region`. The
dedicated-region installer exists but was not carried into the live packages.

### Crash diagnosis (2026-08-22 session)

**Stage 20 — `Fable.exe+0xb39f25` is `CRangeCompressor::Decompress @ 0x00F39ED0`,
RAW/stored path.**

```
00f39f11  cmp byte ptr [edi],0     ; edi = arg1 = input, 0 => RAW/stored block
00f39f18  shr ecx,2                ; ecx = total/4
00f39f25  rep movsd                ; <-- AV reading 0x09b5e000
```

At the fault `eax = 0x01DCA540` = **31,171,392 bytes** total, and the source
region ends about 1.5 MB in. Some block header declares a 31 MB payload.

Both multiplicands are `[esp+0xC4] * [esp+0xC8]` = arg2 (count) * arg3 (stride).
All 14 call sites of `0x00F39ED0` were enumerated from the retail `.text`:

| Caller | stride | count source |
|---|---|---|
| `Load CLandscapeBackgroundPatch @0xBE7D70` (x2) | 0x10 / 2 | `u16` |
| `LoadVerts<CPatchTesselationEdgeStrip::CFanBaseVertex>` (x4) | 0x14 | `s32` from vector |
| `Load CWaterPatchMesh @0xBEAE50` | 0x42 | literal 0x121 |
| `LoadSea CWaterSeaGenerator @0xBE91E0` (x2) | 0xC / 2 | **`s32` from stream** |
| `LoadCompressedBuffer CEngineWaterRenderer @0xB6D940` | param | **caller-supplied** |
| `Load CEngineWaterBackgroundSubPatch @0xBFD7C0` (x2) | param / 2 | **stream / caller** |
| `Load CWaterSeaStaticMapInfoBlock @0xB6D6E0` | 2 | `s32` |
| `RangeCompressor_Compress @0xF3AE50` | n/a | not a load path |

The `u16`-bounded sites cap out near 1 MB (65535 x 0x14) and **cannot** reach
31 MB. Only the sites reading an `s32` count out of the stream can. Every one of
those is in the **water/sea** path. That is the working hypothesis for stage20:
the sea loader is being entered against a zero or garbage offset rather than
being skipped, and consumes a bogus element count.

Supporting evidence: our generated 0x5C InfoBlock
(`ForgeTest64_terrain_v5.info.bin`) writes `EdgeHeightFilePtr=0`,
`EdgeHeightFileSize=0`, `ShorePointArraySize=0`, `ShorePointArrayStart=0`.

**Stage 21 — `Fable.exe+0x59EC33` is a red herring; do not chase it.**
`0x0099EC33` is the `CCharString` default ctor writing `[esi]=0` with
`this = 0x10`. The `_Buynode` at `0x0042B587` did `operator new(0x18)`, got NULL,
then `lea ecx,[esi+0x10]` and `test ecx,ecx` — the classic MSVC null check that
cannot fail because of the +0x10 offset. The allocation returned NULL despite
1.5 GB of free address space. `!heap -h 01940000 -v` on the dump reports:

```
Free Heap block 163470c0 modified at 163470d4 after it was freed
```

A one-byte use-after-free write. It is only fatal because a process launched
under procdump inherits debug heap flags (tail checking / free checking /
validate parameters — confirmed by the dump's `NtGlobalFlag`; there is **no**
IFEO or gflags entry for `Fable.exe`), which make `RtlAllocateHeap` return NULL
on a corrupt free list instead of ignoring it. Retail without a debugger would
walk past this. Stage21 differs from stage20 only in the STB file, so it
probably got no further than stage20 and its dump says nothing about terrain.

### Recovered grammar — `CLandscapeBackgroundPatch::Load @0x00BE7D70` (2026-08-22)

Read directly from retail `.text`. The body is read in this order:

| # | size | stored at | meaning |
|---|---|---|---|
| 1 | u16 | `[edi+0x110]` | pw |
| 2 | u16 | `[edi+0x112]` | ph |
| 3 | u16 | `[edi+0x10c]` | coord0 |
| 4 | u16 | `[edi+0x10e]` | coord1 |
| 5 | u8  | `[edi+0x11a]` | **gates the mesh** |
| 6 | u8  | `[edi+0x118]` | second flag |

At `0x00BE7FC1` the engine reloads `al` from `[edi+0x11a]` — **stream byte 8** —
and `jne 0x00BE8803` skips all mesh loading. So byte 8 is `isWaterOnly` and byte
9 is `detailMode`, exactly as `forge::stbbake::parsePatchHeader` has it. A
retail-derived frame body (`tmp/Darkwood9_directionmask_rebake.bin` frame 4) and
an authored ForgeTest64 frame both carry `b[8]=0x00, b[9]=0x01`. **Forge's header
byte order is correct — an early "swapped bytes 8/9" hypothesis was tested and
disproved. Do not re-open it.**

Both the mesh path and the water-only path converge on a common tail at
`0x00BE8803`:

```
0x00BE8810  4x  call 0xBFC600   ; CPatchTesselationEdgeStrip::Load, this += 0x2C
0x00BE8872      u8 hasWater     ; if != 0 -> allocate 0x60 (0xBFD160) and
                                ;   CEngineWaterBackgroundSubPatch::Load @0xBFD7C0
```

`CPatchTesselationEdgeStrip::Load @0x00BFC600` reads
`[u16 -> +4][u16 -> +6][u8 -> +0x29]` then calls four `LoadVerts` bodies
(`0xBFC880 / 0xBFCA20 / 0xBFCBC0 / 0xBFCD60`, element stride `0x14`). Each
`LoadVerts` reads one `s32` and passes it to the vector resize at `0x00BFBE40`
(element size `0x14`); **a zero consumes exactly 4 bytes and returns** — the
`newCount == 0` branch at `0x00BFBE56` frees and reports success, then
`[vector+4] <= 0` makes `LoadVerts` return before reading a block.

So forge's 21-byte empty strip (`[u16][u16][u8][4 x s32 zero]`) and its 85-byte
`serializeEmptyPatchTrailer` **do** parse cleanly under the retail grammar, and
an authored frame's `17 + texLen + vbLen + 85` consumes the body exactly. The
128x128 six-mip DXT1 inline texture length also checks out
(8192+2048+512+128+32+8 = 10920, +19-byte header = 10939, matching forge).

**Known cosmetic divergence, cause unproven:** retail strip headers carry real
values — Darkwood frame 4's first strip is `[u16 0x0940 = 2368][u16 0x0010 = 16][u8 1]`,
i.e. world Y and patch size — while forge passes the patch header's
`coord0/coord1`, which is `(0,0)` for ForgeTest64. Those two words are only
stored to `[edi+4]/[edi+6]`; they do not size any read, so they cannot by
themselves shift the cursor.

**ROOT CAUSE FOUND — the inline background texture wrote a full mip chain.**

`CLandscapeBackgroundPatch::Load` allocates the patch texture with a literal
level count of **1** (`push 1` at `0x00BE81D0`, format = the hard-coded `'DXT1'`
fourcc pushed at `0x00BE8197`). `CTexture::LoadFromDataStreamToPreallocatedSurface
@0x009FB750` then reads a 19-byte (`0x13`) header and loops over mips bounded by
the **surface's** level count (vtable `+0x34`, compared at `0x009FBA4D`) — it
never reads back the header's `levels` field. Each level consumes exactly
`bpp*w*h/8` **raw** stream bytes (`0x009FB865`: `imul` width, `imul` height,
`shr eax,3`), copied with `rep movsd` straight off the stream. So the engine
reads mip 0 and stops.

Retail matches that exactly. Darkwood frame 4's inline texture header is
`4000 4000 01 e3040000 ...` — 64x64, **levels=1** — and its texture span is
`19 + 2048 = 2067` bytes, which is mip 0 alone. Every retail patch checked spans
`19 + mip0`.

Forge emitted `8000 8000 06 ...` — 128x128, **levels=6** — spanning
`19 + 10920 = 10939` bytes, while the engine consumes only `19 + 8192 = 8211`.
That leaves **2728 unconsumed bytes** per patch. The stream cursor lands 2728
bytes short of the vertex block, so the loader reads the edge-strip counts and
the `hasWater` EBOOL out of mip-1 texture data. A garbage `hasWater` enters
`CEngineWaterBackgroundSubPatch::Load`, which reads an `s32` element count from
texture bytes and hands it to `CRangeCompressor::Decompress` — the observed
31,171,392-byte `rep movsd` and the stage20 AV.

This also explains why **zero ForgeTest-scope terrain render probes ever fired**:
no authored background patch ever finished loading.

**Fixed in FableForge.** `forge::stbbake::singleLevelBackgroundTexture` reduces a
DXT1 background texture to the single mip the engine reads, and
`buildBackgroundPatchRect` now serializes through it. Non-DXT textures are
returned unchanged rather than guessed at. `forge_tests` passes 1/1.

Regenerated artifact `work/no_donor_terrain_pack/ForgeTest64_terrain_v6.chunk.bin`
(1,216,581 bytes, SHA-256 `170b10306f1e36f8...`, byte-identical across two
builds). Every one of the 31 background payloads now reports
`layout=17+8211+<vb>+0+85`, and a regenerated frame's texture header reads
`8000 8000 01 ...`. The 2728-byte drift is gone chunk-wide.

**Not yet deployed.** This is an offline fix verified offline only; it has never
been run.

### stage22-24 runtime results (2026-08-22) — BOTH BLOCKERS CLEARED

Full evidence under `work/no_donor_terrain_pack/runtime_evidence/20260822_stage2{2,3,4}_*`,
each with a `RESULTS.md`. Run sheet: `work/no_donor_terrain_pack/STAGE22_RUN_SHEET.md`.

**The runtime region vector is hard-capped at 142.**

| stage | BWD regions | runtime vector | owning region | lookup |
|---|---|---|---|---|
| 22 | 143 | 142 | 142 | 0 |
| 23 | 145 | **142** | 142 | 0 |
| 24 | 145 | 142 | **141** | **141** |

stage23 proved the cap by adding two more regions and measuring no change; all
three installed BWD copies were confirmed to hold 145 regions and the WAD holds
no shadow BWD. Region index 142 never answers containment lookups. **A custom
map must be owned by a region at index <= 141** — appending a new region for it
cannot work, an existing under-cap region must be repurposed.

This RETIRES the earlier claim (in this file and in memory) that "the hard-cap
theory is refuted: the FSE probe measured all 142 regions loaded". That rested
on one data point, BWD 143 -> vector 142, misread as "everything loaded". It also
means the `ForgeTerminalSentinel` region added at stage9 never did anything.

stage24 repurposes retail region 141 `Filler_NorthernWastes_02` — inert
(`contains=0 sees=0`, referenced nowhere else in the WLD). It now owns slot 399
and nothing else, so single-map ownership and the strict partition both hold.

**stage24 reached the acceptance gates that had never passed:**

```
slot_399_owner_region=141
arrival verification map=ForgeTest64 in_target_bounds=true
spawned ForgeNPC (CREATURE_GHOST_VILLAGER_FEMALE) at (3364.0,2330.0,78.7) -> OK
```

**The inline-texture mip fix is VALIDATED at runtime.** Background render probes
over the authored patches report `texBytes=8192` (exactly one 128x128 DXT1 mip;
17 of 18 patches, the last a 64x64 at 2048), `water=0` on all 18, `vertices=289`,
`z=(72,83.962)`, and **no access violation** — stage20 faulted at precisely this
point. `water=0` is the direct refutation of the drifted-cursor fault.

**ACTIVE — the terrain renders incorrectly.** Screenshot
`runtime_evidence/20260822_stage24_region141/stage24_screen.png`: the hero is on
ForgeTest64 with a correct minimap, but the surface is a flat featureless grey
plane with black fan-shaped polygon artifacts, and no height relief despite the
patches carrying z=72..83.962. Two leads:

1. **Foreground layer texture IDs are wrong.** `LandscapeTextureProbe` on
   ForgeTest patches reports `values=[1:0,3:0]` / `[1:3,3:3]` — indices **1 and
   3**. Retail patches in the same log report real GBANK_MAIN_PC ids
   (`4185/4304/4192/4307/4226`). Ours look like unresolved local palette symbol
   indices. `missing=0`, so the engine binds whatever 1 and 3 are. Likely cause
   of the flat grey.
2. **Foreground geometry is malformed.** All 16 foreground render probes are
   structurally clean (`layers=1 truncatedOrCycle=0 safe=1`, per-patch bounds
   correct), so the black fans are probably bad vertex/index data inside the
   layer meshes rather than a broken layer list.

### stage30-33 (2026-08-22 later) — terrain DONE, presentation in progress

Deployed now: **stage33** (`work/no_donor_terrain_pack/stage33_mmfill_runtime_package`).
Rollback set `20260822_160324`. Each stage has its own backup set under
`work/terrain_runtime_probe_20260821/backups/`.

**CONFIRMED WORKING IN-GAME** (operator-verified): map streams, hero teleports and
arrives in bounds, terrain renders with correct relief and no holes, surface is
walkable, textures display, NPC spawns reachable (`drop=0.00`), minimap shows the
authored terrain at the correct orientation.

Three background-patch bugs were found and fixed in sequence, each hidden behind
the previous — full detail in `docs/FINDINGS.md`:

1. **Oversized patches** -> the LOD crash. `detailMode==1` makes the loader index a
   shared index-buffer table as `table[pw*17+ph]` (`0x00BE860A`), so pw and ph must
   both be <= 16. Retail Darkwood_3 contains only 16x16 and 16x8 patches. Parents
   must be stride-DECIMATED, never full-resolution.
2. **Row-major vertex order** -> giant spanning wedges. The VB must be in the
   engine's serpentine first-touch strip order (what `buildLayerTopology` already
   produced for foreground layers); verified byte-identical to retail across the
   column-pair seam via the new `forge stb patchverts`.
3. **indexCount as raw triangle count** -> terrain rendered as bands with gaps. It
   is the shared-IB STRIP length: retail 16x16 stores **1085**, not 16*16*2=512.

Two test assertions encoded the wrong values (1 and 3) and were corrected with the
retail evidence in comments so they are not reverted.

**Textures.** The foreground triple must be GBANK_MAIN_PC ids.
`forge stb create-terrain` silently substitutes a placeholder `{1,2,3}` when given
symbol NAMES instead of numeric ids — that was the flat-grey terrain. Custom art
goes into `UNASSIGNED_*` slots via `tools/texture_build.py replace`. Multi-layer
terrain now works via `buildLayeredForeground`: opaque base + slope-blended +
height-blended layers with `mapping=0/2/4` and per-vertex `blend`/`cliffU`/`cliffV`,
matching retail's multi-layer patch composition. Currently
4216 (SPOOKYGROUND) base, 4188 (SPOOKY_TEST03) slope, 4212 (SPOOKYTREES) height.

**Presentation layer** — see `docs/FORGETEST64_UI_AND_REGION.md` for the full
write-up: minimap working, transition screen staged but black, exit signpost
spawn failing silently, minimap zoom unresolved.

**Foliage** — RE done, writer not built. See `docs/FOLIAGE_LOCAL_DETAIL_RE.md`.
The important strategic note there: **author with primitive type 0 only**. It is
fully specified (fixed 92 bytes, one transform, no arrays), so a first writer can
skip the two unpinned 16-byte type-1 instance arrays entirely.

### Next actions

1. **Foliage writer**, type-0 primitives only. Remaining structural RE needed:
   `CObjectCacheGroupCollection::SaveHeader` write path from `0x02E3D6B9`,
   plus `SaveContents` and `GetSaveSize`.
2. **Exit signpost**: restructure the Lua so failures are logged (the current
   pcall swallows them), and try a concrete signpost def rather than
   `OBJECT_SIGNPOST_TEMPLATE`, which is probably abstract.
3. **Transition screen**: add `--worldmap-offset` to `forge bwd set-region-name`
   and give the region a real `WorldMapOffsetX/Y` (currently 0,0; retail values
   are ~(963,1239)).
4. **Minimap zoom**: reverse the minimap render path in `Fable.exe`. Two blind
   attempts have been spent; get the mapping from the code.
5. Region display name / `REGION_FORGETEST64` def, so the transition screen has a
   proper caption instead of a borrowed `REGION_BATTLE`.

### Install state warning (2026-08-22)

The game install is **not** rolled back to retail. `FinalAlbion_RT.stb` is
599,214,598 bytes (the stage7-12 build, mtime 09:13) against retail's
597,979,518, and `FinalAlbion.bwd` is modified (mtime 09:50). Restore before any
measurement that assumes a clean install.

## 2026-08-21 ultracode decomp lane

1. **DONE — focused candidate gating.** Added repeatable `--address` selection and isolated
   `--report-prefix` outputs to `tools/gate_re_agent_candidates.py`. The newest twelve Wave 3
   candidates now gate in about eight seconds without rescanning all 910 candidates or replacing
   the canonical reports. Focused report: `rebuild/compile-gate/wave3-0045-new.tsv`.
2. **DONE — triage newest Wave 3 PASS batch.** All 12 pass source-integrity checks; 8/12 pass the
   host C++20 syntax shim; 0/12 are directly VC7.1-compatible; 0/12 trigger semantic quarantine.
   Agent PASS is therefore treated as structural evidence only.
3. **DONE — tree payload allocation family.** Correcting the generated return type exposed the
   retail contract: return the allocation base while copying the payload at `base + 0x10`. All six
   34-byte family members now pass relocation parity, focused behavior, and selected builds.
4. **NEXT — promote the lowest-friction members of the batch.** Continue with the bounded
   `Copy_MemoryStepped @ 0x0045493E` / `CActiveFile_CopyRange @ 0x0045BCC1` codegen residue, then
   advance to the next small host-syntax-gated wrapper.
5. **NEXT — canonical refresh.** Run the full refresh without `-Force` after a candidate actually
   lands or the Wave queue changes the fingerprint; do not spend a full scan on focused triage.

### 2026-08-21 continuation result

- **LANDED — `CActiveFile_AllocateAndCopy @ 0x0045BCF2`.** Readable VC7.1 source plus retained frame
  pointer and size optimization reproduces all 50 retail bytes after masking the two call
  relocations. The focused behavior fixture passes, the selected canonical build passes, and the
  parity ledger reports `RELOCATION_MATCH` with zero missing oracle rows.
- **BOUNDED RESIDUE — `Copy_MemoryStepped @ 0x0045493E` and `CActiveFile_CopyRange @ 0x0045BCC1`.**
  These are byte-identical 35-byte specializations apart from the `memmove` relocation. Their
  corrected four-parameter fastcall sources have the right `ret 8` behavior, but VC7.1 canonicalizes
  both to the same 30-byte body. Both behavior fixtures pass; neither is falsely landed.
- **TOOLING FIX — transactional oracle validation.** `verify_and_land.py` now validates the required
  TSV schema, hex payloads, and declared lengths before compiling or writing landed artifacts. This
  prevents malformed focused oracles from failing only after source/catalog mutation.

### 2026-08-21 continuation queue 2

1. **LANDED — `CActiveFile::AllocateArray @ 0x00454961`.** Reused the proven allocation/copy source
   shape from `0x0045BCF2`; selected VC7.1 build and behavior fixture pass, with 50/50 retail bytes,
   two masked call relocations, and canonical `RELOCATION_MATCH`.
2. **BOUNDED — `Vector_AllocateAndCopyRGB @ 0x0045BBC5`.** Recovered the retail
   destination-minus-source offset loop and reached 68/68 bytes with behavior PASS. The remaining
   miss is a stable ESI/EDI allocation swap across two source formulations, so it is queued as a
   register-allocation residue rather than falsely landed.
3. **BOUNDED — `CTCCarryable_AssignTreeStructure @ 0x0045C19F`.** The typed overlay and tail-walk
   behavior pass, but the current VC7.1 body is 129 bytes versus retail's 115. Its sibling
   `0x0045C3CF` has the same 115-byte body apart from call relocations, so one future source-shape
   recovery should promote both.
4. **NEXT — template sibling harvesting.** After each exact landing, search the manifest and retail
   disassembly for byte-identical/template-specialized siblings before selecting a new shape. This
   pass turned the `0x0045BCF2` solution directly into the `0x00454961` landing.
5. **NEXT — resume fresh small shapes.** Prefer candidates outside the three bounded residues above;
   return to them only with a new compiler flag, type signature, or register-allocation hypothesis.

### 2026-08-21 continuation queue 3

1. **LANDED — `Math_ComputeByteChunkCount @ 0x00C9CB60`.** The signed divide-by-255 expression
   compiles to an exact 24/24-byte `MATCH`; boundary fixtures cover zero, 254, 255, and -255.
2. **LANDED — fixed-size allocator family.** `Allocator_Alloc_56bytes @ 0x0042AC52`,
   `Allocator_Alloc_24bytes @ 0x0042AC9F`, and `Allocator_Alloc_28bytes @ 0x0042AD10` each reproduce
   all 22 retail bytes after masking only the `malloc` relocation. Their selected behavior and
   canonical builds pass.
3. **LANDED — `ListNode_AllocateNode @ 0x00450E6E`.** Reused the fixed-size allocator shape for
   another 22/22-byte relocation match, with behavior and selected canonical build passing.
4. **NEXT — drain allocator siblings.** Addresses `0x00450EBB` through `0x00451303` contain fifteen
   nearby allocator specializations (mostly 0x18-byte nodes, plus 0x24 and 0x14 variants). Verify
   each retail boundary and hidden stack parameter, then batch-land the byte-identical shapes.
5. **NEXT — return to semantic leaves after the allocator drain.** The current small-shape queue
   begins with RB-tree iterator wrappers and linked-list insertion forwarders; keep their real
   helper declarations rather than adding compile-only stubs.

*Reconciled 2026-08-05 from the canonical rebuild, focused frontend tests, and
strict visual/retail parity gates.*

## Active ForgeTest 64x64 transition lane (2026-08-13)

The game install is currently restored to the stable retail baseline. The next
implementation/probe sequence is deliberately narrow:

1. **Restore the proven native bridge** in ForgeFSE:
   `GetRegionNumberMapIsIn` → `LoadRegion(immediate, force)` → `SetPlayerPos`
   → `ActivateNavMap` → `EntityTeleportToPosition`.
2. **Build the Release Win32 DLL** and verify `git diff --check`.
3. **Run one controlled ForgeTest probe** using the actual runtime BWD at
   `data/Levels/FinalAlbion.bwd`. Slot 399 must resolve to the proven
   `Filler_Darkwood_03` region 95; the region-141 substitution was a bad test
   because the engine reads the nested data copy, not only the game-root copy.
4. **Accept only a real arrival**: FSE must report `map=ForgeTest` and
   `in_target_bounds=true`; a successful API return or unchanged Greatwood
   position is insufficient.
5. **Restore the stable DLL/BWD/config** after the probe and preserve the
   resulting log and hashes under `work/heightmap_test/`.

Current evidence: with the correct nested BWD, the rebuilt bridge resolves slot
399 to region 95, force-loads it, snaps ground Z to `74.0727` (`spawn=75.0727`),
and completes `SetPlayerPos` plus `ActivateNavMap`. The current 64x64 run still
exits after the final `EntityTeleportToPosition` call before arrival verification;
the older 32x32/archived run survives and reports `map=ForgeTest`. The stable
install is restored after each probe. The next engineering target is therefore
the 64x64 final-hero-handoff crash, not BWD ownership or Lua startup.

### 2026-08-13 probe results

- Added and built `GoToMapSlotPrepare`, which performs synchronous region
  preparation but defers hero movement. A source-thread frame yield terminates
  at the region handoff; a destination-owner Lua teleport returns but is a
  no-op and leaves the hero on `GreatwoodTeleport`.
- Added `GoToMapSlotRetailTransition` as a diagnostic wrapper around the exact
  retail `CWorld::SetAsLoadingRegion` primitive. The corrected probe passed the
  supplied target `(3360,2328,74)` and reached the native primitive, but retail
  exits immediately after that call returns from the Lua callback.
- These probes confirm the remaining implementation requirement: schedule the
  retail transition from a safe engine-owned callback/frame, rather than
  synchronously inside the quest callback. The installed game was restored to
  the stable DLL, original Lua, matching root/data BWD hashes, and baseline
  config after each run. Preserved logs are under
  `work/heightmap_test/deferred_prepare_probe_20260813/` and
  `work/heightmap_test/retail_transition_probe_20260813/`.

### 2026-08-13 queued update-boundary implementation

- Added a one-shot `GoToMapSlotRetailTransition` queue. The Lua binding now
  records the live `CWorld*` and target vector and returns; it does not call
  `SetAsLoadingRegion` while the quest callback is still on the stack.
- Added a six-byte trampoline at retail `CMainGameComponent::Update`
  (`0x00418289`). At the next engine-owned update boundary it consumes the
  request and calls the retail `CWorld::SetAsLoadingRegion` state-machine
  primitive, then lets the original update continue.
- Release Win32 build succeeds with zero warnings/errors. A live injection
  probe installed the update hook at `0x418289` and remained responsive, but
  automated frontend input did not reach the gameplay smoke-test thread, so
  this probe is not teleport-arrival evidence.
- The game was restored after the probe to the stable DLL hash
  `80A04F9B3D0C4B341C9964D3A68655BABFE78C36927050F9E05F6F4A859C8BC1`, stable
  nested/root BWD hash
  `7F403163EEDCEA936E0E1FC424757ECBF6E21F468F04765E414EC9FEFA1D74C6`, and
  baseline Lua hash
  `C2B81942CF6E6716E97785B43DC869BA32C93C806C91492D71AA636D621BF766`.

### 2026-08-13 adult-save transition probes

- The queued implementation was exercised from the real adult/free-roam
  `Cornelio\Manual - Save1` through the normal frontend. The first ordering
  armed `SetAsLoadingRegion` before the original main update and terminated
  shortly afterward; no arrival was recorded.
- Changed the detour to run the original `CMainGameComponent::Update` first,
  then arm the queued transition after that update returns. The second run
  survived the arm and remained in the retail loading splash for roughly 90
  seconds before exiting. Its log contains both the queued marker and the
  engine-boundary arm marker, but no `ForgeTest` arrival. This is a real
  improvement and aligns with the reported `Filler_Greatwood` loading screen:
  the retail state machine is advancing farther, not being ignored.
- Preserved probe artifacts under
  `work/heightmap_test/retail_transition_postupdate_probe_20260813/` and
  restored the stable DLL/BWD/config/Lua install afterward.

### 2026-08-13 queued probe follow-up

- Re-ran the queued DLL with a temporary `SetSkipFrontend(TRUE)` config. The
  update hook installed at `0x418289` and the process stayed responsive for
  60 seconds, but no quest host or `ForgeTestEnter` thread was created. This
  is consistent with the documented skip-frontend child/prologue path: it is
  not an adult/free-roam save and cannot produce a valid transition request.
- Restored the normal frontend config, baseline Lua, stable DLL, and verified
  the stable BWD hash. The queued implementation remains source-only until a
  proper adult-save gameplay run can exercise it.

## Current verified state

- Curated reconstruction: **5,795** VC7.1-compiled candidates, with the
  canonical behavior-gated subset tracked by the release dashboard.
- Whole-project verified functional or matching C++: **5,643 / 49,568**
  (**11.38%**), including **3,112 byte-identical** functions (**6.28%**).
- Candidate retail parity: **5,625**
  (`3,096 EXACT + 2,529 RELOCATION_MATCH`), with `42 DIFFER` and
  `128 ORACLE_MISSING`; the whole-project totals additionally include 18
  independently lifted matching/functional functions.
- Auto-RE intake: **885 generated / 871 structural PASS**.
- GFMain evidence/control-flow closure: **257 / 257 direct calls** and
  **10 / 10 authored phases**; the separate modern C++23 startup pass remains
  **0 / 10 phases**.
- Durable source/tests are address-sharded and indexed by `rebuild/ARTIFACT_INDEX.tsv`.
- Custom quest assets are staged offline: nine card defs, 36 custom English strings,
  and one childhood mysterious-NPC TNG addition. Nothing has been deployed to the live game.
- The Release visual checkpoint, retail `frontend.bin` layout oracle, and complete
  Options/Redefine/Quit interaction smoke pass. The scheduled auto-RE queue is
  exhausted apart from five cooldown-deferred hard targets and will retry them
  automatically.
- The profile runtime now follows the recovered retail branches: normal mode
  inserts action `0x125` New Profile before the 30px profile rows, delete mode
  uses the separate 210px list/action `0xD7`, zero profiles enter the authored
  empty-profile branch, and the new-profile editor accepts live text through
  the retail glyph atlas. The adapter also routes the recovered `0xFA`,
  `0x124`, `0x126`, `0x127`, and `0xD6` action boundaries. The exact
  `CreateNewProfile`/profile-manager commit remains an explicit link boundary;
  no synthetic save writer was added.
- The visual checkpoint projects the optional compiled LIVE child out of the
  offline main-menu presentation, preserving the retail 60px gap and atlas
  frames seen in the supplied retail capture. The seven-child `frontend.bin`
  oracle remains unchanged.
- Authored arrow-bearing lists now accept WM_MOUSEWHEEL over their list area
  and left-clicks on the decoded 32x32 up/down arrow components. Detail option
  arrows now also honor the compiled Angle=-0.25, producing the retail
  left/right orientation. Wheel deltas
  and arrow clicks share the recovered `FablePlanUiFrontEndListScroll` path
  and list sound/state transitions; profile, delete, saves, and Redefine
  records preserve their compiled scrolling/wrapping contracts. Main/options
  lists have no authored arrow bindings and serialize `Scrolling=false`, so
  they are not given inferred mouse-wheel ingress. The local debug `FableWin.pdb`
  now supplies authoritative EGameAction enum identifiers for all 31 ActionOrder
  entries, and English text.big resolves every corresponding localized display
  string. The renderer records both layers, so off-page materialization no
  longer depends on guessed labels.
- The delete-confirmation frame is now sourced from the retail definition/text
  pipeline and paired with the normal profile frame in the 640x960 resource
  sheet. `0xD7` enters confirmation and YES dispatches native `0xD6`; the
  checkpoint still stops before native deletion, refresh, and persistence.
- Reference-size Redefine smoke now saves settled captures for the supplied
  `RedefineKeys1-5` states: initial, logical selections 6/15/22, and the
  clamped final page. The alpha-aware comparator reports initial-page RGB MAE
  9.44 and final-page RGB MAE 13.81 against the retail captures after the
  live ENG_ARIAL_12 action/key text migration.
- The scrolled reference captures now exercise the expanded live staging
  queues without dropping to the forest-only fallback. Current state-matched
  RGB MAE is 19.10/17.18/14.89/13.81 for RedefineKeys2-5; these remain
  diagnostic parity measurements, not a final presentation sign-off.
- Focused frontend verification is **50/50** Python tests. The Release
  bootstrap, visual checkpoint, and full reference-size subscreen smoke pass.
  Full decomp/render discovery is **109/109**. Redefine's recovered 44-row
  expanded metadata now has a dedicated 3200x3360 D3D9 atlas/resource and
  native logical-selection page routing, including first-page/final-page
  keyboard, wheel, scrolled-row-hover, and off-page capture/remapping smoke
  coverage; unresolved OEM/reserved labels remain intentionally static.
  Visual sign-off remains open because fullscreen ownership/presentation and
  exact normal-mode retail backdrop timing are not yet signed off, and the runtime is still
  transitional rather than native frontend-manager/component ownership. The
  reference capture gate now provides an exact 1024x768 client window without
  changing the normal 1280x720 smoke default. The
  dynamic profile-qualified title path now uses the recovered `ENG_ARIAL_24`
  atlas and passes a targeted loaded-profile proof. The Video control-atlas
  cursor now follows the authored value groups (`3,4,3,4,4,4,18,2,4,3`), and
  the reference-size smoke emits clean Gameplay/Video/Audio client captures.
  It also emits clean client-area Saved Games captures for the initial AutoSave
  and first manual-save states, ready for state-matched visual diffing. The
  1664-wide Saved Games component cell now leaves all four row names
  transparent; D3D9 submits the recovered ENG_ARIAL_16 labels live at the
  centered `(134,90 + row*30)` origins, with the 1024-wide fallback unchanged.
  `tools/compare_frontend_screenshots.py` and focused tests now provide the
  alpha-aware comparison/heatmap path; the historical first detail baseline
  was Redefine RGB MAE 17.22, Gameplay 16.96, Video 20.42, and Audio 27.36.
   The atlas-vs-retail Redefine comparison measured action-name advances at
   roughly 9/8. The component atlas now leaves those labels transparent, and
   the D3D9 Render2D path emits row labels and key values from the recovered
   ENG_ARIAL_12 atlas/metrics; key-value indices and serialized origins are
   unchanged.
  In reference-size mode those captures use stable decoded retail backdrop
  frames per screen, keeping image comparisons deterministic without changing
  the ordinary animated rendering path.
   Detail Gameplay/Audio/Video row labels now follow the first live text
  component slice: the component atlas leaves those ENG_ARIAL_24 labels
  transparent, and the D3D9 Render2D path emits their glyph quads from the
   recovered title atlas with decoded row origins. The 640x480 fallback remains
   unchanged, and the component recomposition remains pixel-identical.
   Redefine initial and scrolled action labels now follow that same live text
   slice; the focused frontend suite is 50/50 and full Python discovery is
   109/109 after the Release build. The Saved Games title text also follows
   the live ENG_ARIAL_24 glyph path.
  String-valued Gameplay and Video controls now use the same live glyph path;
  arrow sprites and numeric slider/bar tiles remain atlas-backed, while the
  640x480 control sheet retains its original composed values.
  Detail footer labels now follow the live path as well: Cancel, Defaults, and
  Apply are emitted at their decoded helper origins with Apply inheriting the
  disabled gray/active white state. Component hover tiles retain only the ON
  helper glyph, preventing duplicated labels.

Generated queues:

- `rebuild/backlog/promotion_queue.tsv` — ranked uncompiled candidates with semantic hazards.
- `rebuild/backlog/forgefse-binding-queue.tsv` — current binding reconstruction lane.
- `rebuild/backlog/functions.tsv` — canonical reconstruction gaps.

## P0 — frontend runtime and pixel parity

Completed in the current checkpoint:

1. Retail `frontend.bin` now gates the compiled list origins, row steps, child
   order, actions, table offsets, title rules, fonts, and Redefine mouse areas
   used by the Python sheet builders.
2. `CTable::Draw @ 0x00550DC0` ordinary/true transform propagation and generated
   child positioning are promoted into VC7.1 runtime code and behavior tests.
   Main-menu, Options, and Redefine hit regions use that transform chain.
3. Screen headers preserve the serialized left-aligned text origin `(65,y)`
   measured in the retail captures. Their table now uses the exact
   `UI_TABLE_TITLE_WHOLE` mapping: all three horizontal roles reference
   `UI_TEXTBOX_MIDDLE` (#122), producing one continuous retail gold-edged/
   blue-filled rule instead of substituted selected-button pieces. The build
   still prefers the pristine installed `frontend.big`.
4. Plain `--buff-jesus` includes the three retail boot movies; the explicit
   `--skip-boot-videos` developer override and retail/BuffJesus smokes pass.
5. The horizontal/vertical `CTable` line geometry is promoted from retail and
   the PDB-backed FableWin donor: primary placement, axis-specific zoom/repeat
   count, resource-key filtering, cursor advancement, and truncated-output
   accounting have VC7.1 behavior coverage. The installed-retail font test also
   proves the Redefine key-label alpha centre equals its right-slot alpha centre.
6. `CKeyRedefiner` is no longer hover-only in the live checkpoint. PDB-backed
   vtable/body parity identifies `OnLeftClicked @ 0x00557850`,
   `OnLeftUnclicked @ 0x00557AF0`, `ChangeState @ 0x00557C10`, and corrects
   `OnUnhovered @ 0x00557880`. Clicking a row enters the yellow retail
   `PRESS CONTROL` state; keyboard/three-button mouse capture, Escape
   cancellation, duplicate clearing, and Apply/Cancel snapshots are exercised
   by both retail-text and BuffJesus live smokes. Movement action 60 now
   follows `CRedefinerList::RefreshScriptThings @ 0x00556A40`: it expands into
   four W/A/S/D Forward/Left/Backward/Right children before the remaining
   actions,
   rather than occupying one composite display row. Reset actions 284/311 now
   switch those children between arrows and WASD, and both reset controls use
   their complete centered retail ON tables on hover. The initial non-wrapping
   viewport now suppresses its UpArrow and undefined-control warning when the
   nine WASD bindings are defined, matching the supplied retail capture.
7. The exact high-level `ConstructSpritesToDraw` event order is now promoted:
   corners 0/1/2/3, horizontal top/interior/bottom using 4+9, 4+12, 5+8,
   then vertical left/interior/right using 6+10, 6+12, 7+11. Missing optional
   tee/cross components remain null, and missing primary edge components
   suppress their entire separator family. A VC7.1 fixture covers the full
   route, sparse maps, offsets, repeat counts, and truncated output.
8. The exact `ConstructSpritesToDraw` coordinate slice is promoted from retail
   x87 instructions and cross-checked against Ego R: tiled width/height are
   inner extents, corner sizes move the four edge cursors, normalized separator
   tile indices reconstruct interior positions, and repeat/resource offsets
   truncate exactly like retail. Bounded VC7.1 fixtures cover corner anchors,
   line starts, interior rows/columns, and logical generated-child counts.
   The live title rule now uses that inner-span-plus-corners composition before
   the 640-pixel retail viewport clips it.
9. `CList::SetSelectedChild @ 0x005360B1` is promoted from retail and both PDB
   donors. It stores every requested index, leaves visible/invalid selections
   stationary, finds the first contiguous visible window, and emits the exact
   signed 25-pixel-per-row correction for a valid selection outside that
   window. VC7.1 coverage includes above/below, visible, and invalid requests.
10. Frontend lists are proven not to add a hidden centering offset:
    `CFrontEndList::InitialiseOffsets @ 0x0054C480` ignores child count, writes
    interpolation scalar `1.0`, and applies `(0,0)`. Retail, Ego R, and
    FableWin bodies agree, and the VC7.1 fixture locks the override.
11. Frontend list navigation is promoted from retail
    `ScrollUp @ 0x0054C4C0` / `ScrollDown @ 0x0054C810` and cross-checked
    against both PDB donors. Zero/singleton lists and stop-at-end boundaries
    request the invalid action; successful moves wrap when allowed, transition
    the old/new children through states 4/3, rotate row positions by exactly
    `+30`/`-30` pixels, and recompute the optional distance alpha falloff with
    the retail x87 truncation. VC7.1 coverage includes rejection, wrap, state,
    motion, and bounded alpha output.
12. Generic `CList::InitialiseOffsets @ 0x00536B4F` and
    `DoRecomputeOffsets @ 0x0053C332` are promoted and cross-checked against
    Ego R/FableWin. Odd/even centre selection, initial x/y displacement,
    optional zero-origin behavior, states 0/1/4/5/6 position writes, states
    1/4/5 alpha writes, centre reflection, and bounded child output now have
    VC7.1 coverage. The fixture deliberately locks the stored-float alpha
    residue (`126,191,255,191,127`) rather than a symmetric rounded ideal.
13. `CTable::ConstructSpritesToDraw` now crosses from pure plans into owned
    generated components. Definition-bearing sprite prototypes are cloned in
    the recovered corner/horizontal/vertical order, initialised, and receive
    state-0 position/zoom writes. Explicit `CCountedPointer` controls prove the
    retail local-create/vector-copy/local-release sequence, final vector
    ownership, complete destruction, and capacity-failure rollback. The
    20-component fixture and full VC7.1 Release bootstrap pass.
14. The first owned component family now reaches the live Render2D queue.
    Options and all four detail headers construct three definition-122
    `UI_TEXTBOX_MIDDLE` children, map them to the pristine 8x64 retail sprite,
    and emit clipped/scaled quads plus ordinary texture/state records through
    the existing batch and draw-list adapters. Binding/capacity failures are
    transactional. The retained baked sheet and the no-rule component sheet
    recompose pixel-identically across all eight frames; full Release,
    subscreen interaction, and maximized aspect-fit smokes pass.
15. The Options selected-row `UI_BUTTON` table is live. Definitions
    129/130/131 bind to separate pristine `TS_BUTTON_L/R/M` textures, the
    middle child carries the recovered 19-tile zoom, and the owned three-child
    table follows y=`143 + 30*selection` behind the still-baked row text.
    Multi-definition record emission is focused-test covered. Removing both
    this ornament and the title rule from the component sheet still recomposes
    all eight oracle frames pixel-identically. The live D3D9 adapter now
    recognizes the title segment plus all three `TS_BUTTON_L/M/R` texture
    handles; previously those valid Render2D records attached null and made
    the runtime highlight disappear. Retail, BuffJesus, and full subscreen
    smokes now require a visible off-texture highlight-pixel delta.
16. Main-menu selection tables are live in both public variants. Rows 1-6
    reuse the owned 280-pixel vector; Continue Game uses a second owned
    `UI_BUTTON_BIG` vector with the recovered 400-pixel span and 34-tile
    middle zoom. Retail and BuffJesus component sheets omit the ornament and
    recompose all seven baked frames pixel-identically. Both main-menu smokes,
    the complete subscreen smoke, maximized scaling, and the Release bootstrap
    pass.
17. Main-menu labels are seven independent Render2D rows rather than text
    flattened into every selection frame. Retail and BuffJesus component
    atlases retain title-only left frames and one transparent row canvas per
    list child; recomposition is pixel-identical across all 14 frames. The
    retail 0x24-byte `CUIState` layout is compile-locked, and a focused
    `DoRecomputeOffsets` adapter writes the proven position/alpha state masks,
    live state-1 values, and `ScrollingChildrenStates`-style snapshots with
    transactional capacity rejection. The adapter is behavior-gated
    integration code, not whole-function byte parity.
18. The seven live row quads now consume their exact five-state `CUIState`
    maps for position and colour. Main-menu and Options Up/Down input uses the
    recovered frontend-list scroll decision path, including wrap, old/new
    states 4/3, and unchanged-child rejection at a blocked boundary. Retail
    definitions lock both lists to `Wrapping=true`, `Scrolling=false`,
    `AlphaOffset=0`, and `PositionOffsetY=30`; text states 3/4 are immediately
    visible at x=120 with `UpdateTime=-1`. Therefore navigation correctly moves
    only the selected ornament rather than inventing a row translation/fade.
    Retail, BuffJesus, complete subscreen (including Options Down/Up), and
    maximized 2560x1369 smokes pass.
19. Options labels are four independent Render2D row canvases rather than text
    repeated in the four selection frames. The component sheet expands from
    1024x3840 to 1664x3840: its original frame/control/key atlas coordinates
    remain unchanged and x=1024..1664 holds the four row canvases. All four
    quads consume their exact decoded five-state position/colour maps.
    Recomposition against the retained 1024x3840 baked oracle is
    pixel-identical for every Options and detail frame. The full VC7.1 build,
    15 focused subscreen tests, retail interaction, BuffJesus main-menu, and
    maximized smokes pass.
20. Keyboard Enter now dispatches the selected recovered routes instead of
    requiring a synthetic mouse release: press-start action 229, main-menu
    Options/Quit actions 297/314, and Options actions 9/13/12/283. Mouse and
    keyboard share the same transition helpers, keeping detail initialization,
    saved-value snapshots, and D3D state changes aligned. The complete
    subscreen smoke uses Enter for press-start, Options entry, and every detail
    destination; retail and BuffJesus report `keys=enter-up-wrap-down`.
21. Frontend navigation now uses the untouched retail `Frontend.lug` audio
    rather than a synthetic placeholder. `CFrontEndList::ScrollUp/ScrollDown`
    selects `UI_MISC_THINGS_DEF.SoundUpDown` (`CS_GUI_2`, sample 3) after a
    move and `SoundError` (`CS_GUI_5`, sample 7) at a blocked boundary.
    Recovered `CFrontEndManager::Update` transition fields select
    `SoundBack` (`CS_GUI_6`, sample 4) and `SoundForward` (`CS_GUI_7`,
    sample 5). The build extracts and embeds all four RIFFs byte-for-byte,
    the executable resource audit matches their SHA-256 values, and the full
    VC7.1 build plus interaction smokes pass.
22. The first frontend-list bodies now have real object-code proof rather than
    adapter-only evidence. `CFrontEndList::InitialiseOffsets @ 0x0054C480` is
    exact 57/57 bytes with no relocations, and `ScrollUp @ 0x0054C4C0` is
    relocation-normalized 834/834 bytes with zero non-relocation differences.
    Its focused fixture covers reject/wrap, sound selection, states, colours,
    positions, alpha math, and intrusive-reference release. `ScrollDown
    @ 0x0054C810` now has equivalent behavior coverage and a complete
    relocation-normalized 977/977-byte match. Its ABI-explicit recovered body
    also locks the retail x87-truncated alpha ramp (`190,255,190,126,62`).
23. The initial frontend-manager transition/profile seam now has canonical
    object-code proof. `LaunchVirtualKeyboard @ 0x00596917` is a
    relocation-normalized 99/99-byte match, including lookup/routing,
    allocation/construction/storage order, and retail's null-allocation
    behavior. `CreateNewProfile @ 0x0059697A` is a 238/238-byte match covering
    no-space rejection, name-result semantics, defaults, both inversion
    branches, save, menu-definition update, and profile-name propagation.
    Neither body invents direct sound/refcount work; keyboard routing delegates
    that boundary to the already matched `GotoNextScreen`.
    `CreateFrontEndButtonComponent @ 0x00596A68` adds a 614/614-byte match for
    default/custom definition routing, component composition, action setup,
     state widths, and counted-pointer release order. `RefreshAvailableProfiles
     @ 0x00596CCE` is now an 824/824-byte relocation-normalized match with 54
     aligned relocation sites and a passing ownership/dispatch fixture. The
     exact body preserves the inlined string/action/allocator temporary-
     lifetime graph and native vtable dispatches.
    `RefreshAvailableProfilesForDelete` 546/546, `LoadProfile` 130/130,
    `DoPressStart` 227/227, `AddPrefixToTitleBar` 75/75, and
    `CanAcceptOptionChanges` 56/56 further cover delete-list ownership,
    press-start routing, profile publication, title propagation, and the
    option-acceptance gate.
24. Redefine Keys now uses the two different retail row primitives. The
    280-pixel action-name side remains the rounded `FE_SLOT_TEST_L/M/R` table;
    the 220-pixel key-value side is the repeated
    `FE_OPTIONS_HORIZONTAL_BAR_SPRITE` selected by
    `UI_OPTIONS_HORIZONTAL_BAR`. `CText::Draw @ 0x0054EF00` proves both
    `ENG_ARIAL_12` children use their serialized `(0,3)`/`(380,3)` top
    origins: alignment changes x only, y is rounded and forwarded unchanged,
    and the downstream font call receives flags zero. The renderer and focused
    tests now lock that code-derived geometry instead of aesthetically
    centering the text.
25. Continue Game's next screen and dynamic rows are recovered and live in the
    checkpoint. Action 66
    refreshes saves and enters used key `0x08`,
    `UI_FRONTEND_PROFILE_SAVED_GAMES_MENU`. The 1,335-byte refresh body
    replaces `UI_FRONTEND_LIST_FOR_SAVES`, attaches the scrolling text and
    screenshot viewports, then builds autosave-first/ascending-manual rows at
    30-pixel intervals. Each row carries the original filename and chooses
    action `0x11` or invalid-save action `0xDC` from the primary/companion
    validity checks. The runnable now enters the saved-games screen, renders
    the decoded title/list/file-information/Back structure, moves the
    ornamental highlight through the four sample rows with keyboard or mouse,
    and returns through action 86. The 12 logical frames are packed into a
    1664x3840 D3D9-safe atlas; save frames occupy x=1024..1664 at y=1920..3840.
    The full Release build and pixel-level subscreen smoke prove activation,
    highlight movement, and Back. Enter on a row deliberately does not cross
    the still-unrecovered main-game/world-load boundary.

Latest state-matched pass (2026-08-05) corrected four observed presentation
gaps: Redefine's overlapping left/right table now follows the retail draw order,
live detail-font glyphs receive the missing dark halo, and the Saved Games
minimap is alpha-clipped to the decoded ring footprint instead of exposing its
square source texture; the four Saved Games row labels now use the live
ENG_ARIAL_16 glyph path. The File Information header/profile line also uses
the live ENG_ARIAL_24/16 paths; native save-description metadata remains an
explicit ownership boundary. The 4x4 HUD_TEXTBOX_BACK_FE source is also
submitted as a live 640x248 quad from the atlas tail, and the UI_TABLE_TEST_H_T_FE
rule is submitted live at 160x1. The asymmetric UI_TEXT_AREA backdrop now follows
six live atlas quads as well. The UI_TITLE_AREA frame now follows six live atlas
quads too, with the Saved Games title text now live as well. The focused
Python gate is now 50/50, and the Release
bootstrap and full `-VerifyRetailReferenceSize -VerifySubscreens` interaction
smoke pass. These are evidence-based parity corrections, but the saved-games
and keybind screens remain functional checkpoints rather than final
presentation-parity sign-off; fullscreen ownership, exact backdrop timing, and
the remaining retail-capture diffs are still open.

Next work, in order:

1. Extend the live WinMM controller bridge beyond initial navigation. POV and
   analogue Up/Down plus button-1/Start accept and button-2/Back now enter the
   same recovered list-scroll/action routes as keyboard input. Detail screens
   now wrap row focus and apply Left/Right through the same live profile-value
   path as mouse controls. Held direction input now mirrors the recovered
   `CFrontendGameComponent::ChangeSelection @ 0x00494380` state machine:
   immediate direction changes, a 500 ms initial delay, then 100 ms repeats.
   The offline main-menu projection now covers the six visible retail actions;
   connect validated save-row action `0x11` to the recovered main-game load
   chain only after that ownership boundary is ready. Recover any remaining
   mouse-hover and action sound dispatch separately before enabling it.
   The save browser now has an explicit action-boundary resolver: validated
   rows report `0x11`, invalid rows report `0xDC`, and out-of-range selection
   is rejected without fabricating a load or error transition.
2. Capture identical retail and reconstructed Saved Games and Redefine Keys
   states. Produce alpha-aware image diffs and correct measured text,
   highlight, metadata, and background discrepancies.
3. Extend `CKeyRedefiner` beyond the first visible page: the expanded action
   list is now wired through the full 44-row bridge, with retail
   `IsRedefinableKey` filtering and the recovered action-pair coexistence groups
   `{8,31,45}` and `{26,86}`. Keep profile persistence as the explicit
   remaining boundary rather than treating the compact key-name atlas as the
   final renderer.
4. Detail-screen/helper constants are now consolidated into decoded
   definition records: row y/value/default metadata, arrow geometry, footer
   helpers, Redefine reset controls, and the Redefine list mouse/value/hover
   geometry. Runtime hitboxes and both native render paths consume those same
   records. Continue with live retail component ownership after the capture
   and reference-diff work.
5. Move from precomposed 640×480 sheets toward live retail component rendering:
   sprite tables, text components, state transitions, alpha/color inheritance,
   and UI scaling should be emitted through the recovered Render2D path.
6. Replace the remaining dependency boundaries inside the now-callable full
   GFMain coordinator. Phase 10's `CWideString::operator!=` and complete
   registry constructor/write/destructor closure are now exact, independently
   behavior-gated leaves. The former render-manager startup-line call is now
   correctly identified and exact as `CUserProfileManager::SetProfileName`,
   its counted singleton getter is exact as well, and the former destructor
   collision is now exact `GFUninitialise`. `CGame::Play` and its ownership
   graph are exact as well, alongside the full `GFHandleSystemInitError`
   dispatcher. All ten phases execute in Stage 3; Phase 10 is now 21/21
   direct calls proven. Phase 7's final ownership-heavy `GetWindowTitle` leaf
   is exact too, raising that phase to 15/15. Exact CIME initialization raises
   Phase 9's exact CIME and LUG-to-MET coordinators raise it to 10/11 and
   GFMain to 218/257 overall after exact Phase 8 configuration cleanup.
   `CSystemManager::Initialise` is Phase 9's final edge.
   The frontend checkpoint remains a fast visual gate.

Acceptance: decoded layout values and runtime hit regions share one oracle;
headers and row text are centered by recovered transforms/metrics; every visible
state has a deterministic screenshot gate; and the authored checkpoint clearly
distinguishes exact retail data from still-approximate runtime behavior.

## P0 — particles, RSA lighting, sky, and shadows

The viewer-parity lane is now grounded in retail asset and runtime evidence:

1. `tools/report_particle_rendering.py` joins selected `effects.big` emitters
   to `textures.big` descriptors, preserving exact component path, real versus
   allocated dimensions, UV crop, aspect, crossed-sprite count/angles,
   orientation inputs, colours, decoded PDB sprite flags, and exact normal-pass
   D3D9 blend state.
2. The candle-flame defect has a concrete contract: texture 4422 is 32x64,
   the quad aspect is 2.0, and two crossed planes are emitted at base-angle
   offsets 0.125/0.375 turns. The planes are not animation frames.
3. `tools/report_environment_lookup.py` samples the installed 190x21
   `lighting_colours.tga` with the retail `(byte + 0.5) / 255` conversion.
   All rows, including diffuse/ambient/backlight and sky/cloud gradients, are
   decoded from the live `ENVIRONMENT` definition.
4. `docs/PARTICLE_LIGHTING_VIEWER_HANDOFF.md` records the two sprite paths,
   ten-quad batching, padded-texture UV behavior, environment columns,
   lighting SIMD layout, layered sky inputs, and shadow-fade triple.
5. `EEngineSpriteFlag` is recovered through the Ego R PDB. Zero is centered
   `2D_FACE_ME`; the `3D_FACE_ME`, alignment, rotation, lighting, modulation,
   and Z-buffer bits are now decoded in every generated effect report.
6. The batched sprite normal-pass blend tuples are recovered: additive is
   `ONE/ONE`, add-smooth is `ONE/INVSRCCOLOR`, and ordinary alpha is
   `SRCALPHA/INVSRCALPHA`; ADD/SUB/REVSUB blend-op state is also exact.
7. `tools/dump_shader_asm.py` extracts the original shader-model 1.1 token
   streams from `shaders.big` and disassembles them with payload hashes.
   Billboard, crossed, 3D-face-me, stipple, displacement, and ordinary sprite
   shaders are dumped in `work/particle_shaders.asm`.
8. The outer- and inner-sky shader programs are dumped in
   `work/sky_shaders.asm`. The exact two-texture/gradient-alpha outer blend and
   four-texture, two-layer inner/cloud composition are now documented.

Next work, in order:

1. Map names onto every now-disassembled particle shader constant register and
   recover the separate RSA mesh-normal formula.
2. Recover the signed blend-op shader behavior and the special depth/pass
   overrides around the now-exact normal colour pass.
3. Attach semantic names to the inner-sky/cloud constant registers and trace
   each texture stage back to its upper/lower theme field; the GPU composition
   itself is now exact.
4. Promote shadow buffer allocation, depth packing, transforms, and scene
   eligibility into a small documented renderer contract/test fixture.
5. Add effect-specific golden reports for flame, statue waterfall, smoke, and
   self-illuminated particles, then compare viewer captures by camera angle.

## P0 — review the current Quest-wrapper tail

The organized-tree source/parity refresh is complete for its recorded ledger;
the current full bootstrap is still stopped by the strict
`CThreadedFile::Open @ 0x0098E1E0` non-relocation-byte gate. Review these
generated wrappers before any promotion:

1. Reconstruct the failed lookup paths in `DisplayTutorial @ 0x0089E710`,
   `EnableGuards @ 0x00896270`, `EnableVillagerDefTypes @ 0x008962D0`, and
   `GetNumberOfItemsOfTypeInInventory @ 0x00897190`; each currently dereferences an end sentinel.
2. Correct the unrelated-object fallback in the inventory wrapper and the implausible lip-sync
   container identity in `ClearGossip @ 0x008AA010`.
3. Verify the raw slot and null behavior in `GetWaterHeightAtPosition @ 0x0088DF60`.
4. Promote only through VC7.1 compile, focused behavior, signature audit, and retail comparison.

## P0 — make the secret hunt playable

### Registration

1. Register the persistent parent quest `SecretHunt`.
2. Register `MysteryHunt1` through `MysteryHunt9` in ForgeFSE and
   `data/Levels/FinalAlbion.qst`; the companion script currently activates these names.
3. Bind the childhood entity script to `MysteriousChildhoodNPC`.
4. Keep the card, text, TNG, and Lua files in one deployable/rollback unit:
   - `data/CompiledDefs/game.bin`
   - `data/lang/English/text.big`
   - `data/Levels/FinalAlbion/StartOakValeEast.tng`
   - `FSE/SecretHunt/...`

### Region probe

Log `Quest:GetRegionName()` in the intended maps before finalizing the hunt table.
`Greatwood`, `Witchwood`, `Darkwood`, `HookCoast`, and `Oakvale` are plausible;
`Graveyard`, `HobbeCave`, and `Snowspire` are explicitly unconfirmed.

### Runtime smoke order

1. Start a new childhood game and find the NPC at `M_BarrelManHiddenPos`.
2. Confirm talk UI renders the new `TEXT_SECRET_HUNT_NPC_*` strings.
3. Accept the secret; confirm no quest card is created during childhood.
4. Save/reload as a child and confirm the NPC uses the reminder line. This gates
   `OnPersist`/`PersistTransferBool`.
5. Cross the time-skip and confirm `CanOfferAdultSecretHunt()` is true.
6. Activate `MysteryHunt1`; verify F9 shows:
   - title `The Footprint That Vanished`;
   - its custom summary and objective;
   - 200 gold / 50 renown.
7. Complete/deactivate Hunt 1 and activate Hunt 2 to prove distinct cards cycle.
8. Restore all backups after the smoke.

Acceptance: childhood discovery survives save and time-skip, the adult card appears in
F9 with custom text/rewards, and retail restoration hashes match.

## P0 — high-value fse2 decomp candidates (COMPLETE)

These directly support quest/NPC/runtime features. All eight are now cataloged VC7.1
parity landings; the table is retained as the completed product-priority record.

| Address | Bytes | Function | Why next |
|---|---:|---|---|
| `00896F60` | 97 | `SetDeathRecoveryMarkerName` | save/death state wrapper |
| `00896DF0` | 104 | `KickOffCreditsScreen` | screen-flow wrapper |
| `00894300` | 111 | `MsgIsQuestionAnsweredYesOrNo` | mysterious-NPC interaction |
| `0089E4C0` | 120 | `AddScreenTitleMessage` | custom quest messaging |
| `00891220` | 125 | `OverrideMusic` | pairs with verified `StopOverrideMusic` |
| `00896EE0` | 125 | `GetDeathRecoveryMarkerName` | save/death state wrapper |
| `008913F0` | 148 | `AddQuestCard` | central custom-card ABI |
| `0089AD90` | 155 | `IsQuestStartScreenActive` | quest UI state |

Gate for every address:

1. validate the oracle boundary;
2. author C++03-compatible source and a concrete behavior test;
3. require `check_one.py` `MATCH/RELOCATION_MATCH + PASS`;
4. require independent `verify_and_land.py` agreement;
5. land only after both gates.

## P1 — short fse2 candidates (COMPLETE)

The short P1 set is also fully cataloged after landing `00893EC0
MsgOnHeroSlept`. Current unfinished fse2 work begins at P2; use the regenerated
`rebuild/backlog/fse2_remaining_ranked.tsv` rather than this completed table.

| Address | Bytes | Function |
|---|---:|---|
| `00891610` | 97 | `GetMostRecentValidUsedTarget` |
| `008A8E40` | 98 | `EntityDecapitate` |
| `0089D7C0` | 99 | `SetAbilityAvailability` |
| `00893F80` | 100 | `MsgOnHeroCastSpell` |
| `008979C0` | 101 | `IsEntityMarriedToHero` |
| `00897A30` | 101 | `IsEntityMarriable` |
| `00899770` | 101 | `GetHeroRoughExperienceLevel` |
| `008A8EB0` | 102 | `EntitySetAttackThingImmediately` |
| `008A1C50` | 104 | `RepopulateVillage` |
| `0089C8A0` | 104 | `IsDeedWitnessed` |
| `008974F0` | 105 | `IsEntityWieldingWeapon` |
| `00898A50` | 105 | `GiveHeroTutorial` |
| `0089A000` | 105 | `GetHeroMoralityCategory` |
| `00894490` | 106 | `RemoveDeadCreature` |
| `008A0FD0` | 107 | `SetReadableObjectTextTag` |
| `00899390` | 109 | `GetHeroStrengthLevel` |
| `00899400` | 109 | `GetHeroSkillLevel` |
| `00899470` | 109 | `GetHeroWillLevel` |
| `00893EC0` | 110 | `MsgOnHeroSlept` |

Full fse2 ordering, including the 46 larger functions, is in
`rebuild/backlog/fse2_remaining_ranked.tsv`.

### Explicit hold

`008997E0 GetHeroExperienceAvailableToSpend` is behavior `PASS` but best parity is
`DIFFER(101v97)`. It is a documented register-allocation/spill residue. Do not blind
regrind it; retry only with a new liveness/register hypothesis or a different compiler build.

## P1 — pilot a Unicorn retail behavior oracle

Adopt Unicorn only as an optional, local function-level x86 oracle. It must
remain outside the canonical build and cannot change parity totals by itself.
The full design and upstream evidence are in
`docs/UNICORN_EMULATION_PLAN.md`.

Current opportunity:

- 2,390 `MATCH` functions are 2–64 bytes with zero recorded relocations,
  providing a broad known-good harness-validation pool.
- 174 `DIFFER` functions are at most 128 retail bytes, providing a bounded
  second-stage trace-diagnosis pool.
- CPU-side frontend geometry, billboard generation, particle batching, shader
  constant selection, and D3D state arguments can be traced behind registered
  stubs; actual D3D/shader output cannot.

Pilot order:

1. Create a separate Python 3.13 environment and pin `unicorn==2.1.4`; do not
   vendor or link it into reconstructed binaries.
2. Implement `tools/unicorn_oracle.py` around authoritative oracle rows,
   original retail virtual addresses, explicit stack/heap/object fixtures,
   mapped call stubs, a return sentinel, and hard instruction/time bounds.
3. Prove deterministic register, stack-delta, block, memory-write, call-trace,
   and fault JSON on five known exact leaves.
4. Diagnose ten same-length, sub-128-byte `DIFFER` rows. Preserve
   `UNSUPPORTED` separately from `MISMATCH`; feed only proven semantic/ABI
   corrections into the normal author/refine loop.
5. Trace one frontend geometry helper and one particle/RSA/render-state helper
   behind engine/D3D stubs.

Acceptance: repeated outputs are deterministic; existing behavior fixtures
agree; deliberate bad fixtures fail; at least one trace replaces a guessed
contract or repairs a real residue; retail bytes/snapshots remain local; and
every promoted function still passes VC7.1 compile, behavior, and retail-byte
comparison.

## P1 — clear small stalled batches

These are compact cleanup pools after fse2:

1. batch13: **1 remaining**: corrected 39-byte `0045641A OnDie`.
2. batch12: **complete** (130/130 landed).
3. batch11: **1 remaining**: corrected 41-byte `0044FA6D ~CBossDef`.

These are previous non-wins. Validate boundaries before authoring; many tiny failures are
merged-oracle or signature problems rather than difficult source.

## P2 — ForgeFSE binding retries

fse1 has **69** unlanded binding targets. Start with the smallest:

- `0089CD50 CanThingBe_Smelled_ByOtherThing` (20 B)
- `0088E130 GiveHeroWeapon` (23 B)
- `008CFE30 CGameScriptThing::GetHomePos` (24 B)
- `0088F480 ApplyScriptBrush` (25 B)
- `0088E3D0 MiniMapAllowRouteBetweenRegions` (33 B)
- `008A9E30 AddGossipVillage` (34 B)
- `008A9DD0 AddNewRumourToCategory` (34 B)
- `0088F9B0 GetGuildSealRecallPos` (36 B)
- `00891550/80/B0/E0` boast wrappers (36 B)

Because this is a retry lane, record the prior mismatch class before spending another
authoring pass. Prefer oracle repair, ABI correction, or annotated-diff-driven changes.

## P2 — harden FableForge custom text

1. Review and isolate the text-port changes from the already-dirty FableForge worktree.
2. Commit the port as a coherent unit:
   - BIG magic preservation and type histogram;
   - text encode/upsert/import;
   - quest-card custom-text flags;
   - tests and `docs/CUSTOM_QUEST_CARD_TEXT.md`.
3. Add a mod-package/deploy command that treats `game.bin + text.big + TNG + Lua` atomically.
4. Add a rollback manifest containing original and staged hashes.
5. Add additional language manifests only after English runtime validation.

Acceptance: FableForge remains 7/7 tests passing, an imported archive reopens, all untouched
retail records remain byte-identical, and no command edits the install without `--in-place`.

### 2026-08-26 accessor / COM wrapper parity pass

- [x] `0042c48b CTC_FindInterface_0x11` landed as a 75-byte
  `RELOCATION_MATCH`. The bitset gate at `+0x20`, vector-map lower bound at
  `+0x44`, end/sentinel selection, second-dword output, and boolean return all
  pass; its selected VC7.1 build also passes.
- [x] Interface lookup twins landed from the same proven source shape:
  `0042c521 CTC_FindInterface_4` is a 75-byte `RELOCATION_MATCH`, and
  `0042c64d CTCBase_FindInterfaceType_0xe8` is a 76-byte
  `RELOCATION_MATCH`. Both behavior fixtures and selected builds pass.
- [x] Completed the remaining fourteen-member `0042c43f..0042c8f4`
  interface-lookup family. Nine small-key bodies are 75-byte
  `RELOCATION_MATCH` functions; five large-key bodies are 76-byte
  `RELOCATION_MATCH` functions with saved `EDI`. All fourteen focused checks,
  selected builds, and one-entry multiplicity audits pass. Together with
  `0042c48b`, `0042c521`, and `0042c64d`, the recovered family totals seventeen.
- [x] `0042ab01 Std_DoubleLinkedList_CreateNode` landed as a 27-byte
  `RELOCATION_MATCH`. It clears the output, allocates a 12-byte self-linked
  sentinel, republishes it, and returns the output pointer. Behavior, selected
  build, and one-entry multiplicity checks pass.
- [x] `004d55d0 CTCCoopSpirit::Construct` landed as a 29-byte
  `RELOCATION_MATCH`: allocate 0x34 bytes, placement-construct with the incoming
  thing, null fallback. Behavior and selected build pass.
- [x] `006700f0 CTCCoopSpirit::OnCreate` landed as a 162-byte
  `RELOCATION_MATCH`. Retail null-initializes the particle-emitter intelligent
  pointer, calls base `OnCreate`, clears movement/timer/master/counter/score
  state, resolves `this->thing->DefIndex`, extracts the coop-spirit subdefinition
  into `this+0x28`, and releases the temporary definition reference. The
  generated morph-entry/drunkenness/template identities are rejected. Focused
  retained/final-release behavior and the selected build pass.
- [x] `0066ff20 CTCCoopSpirit::SwapToHero` landed as a 175-byte
  `RELOCATION_MATCH`. It acquires the draw environment, saves the spirit thing's
  three-dword position, sign-extends hero slot `+0x90`, performs the player swap,
  creates/adds and destroys the temporary child in retail order, resolves the new
  player, forwards the saved position through virtual slot `0x7c`, and copies
  thing field `+0xb0` to `+0xb4`. Focused ordering/value behavior and the selected
  build pass; speculative generated helper identities were replaced by neutral seams.
- [x] `008a89d0 CGameScriptInterface::GetAllCreaturesExcludingHero` landed as a
  276-byte `RELOCATION_MATCH`. The specialized search seam owns the named
  exclusion policy; this body reserves the caller's output vector, converts each
  returned creature pointer to a 12-byte `CScriptThing`, appends with reference
  acquisition, releases/resets the conversion temporary, frees the search array,
  and returns the output count. Empty and populated ownership behavior plus the
  selected build pass. A documented three-byte loop-alignment NOP preserves retail.
- [x] `008a9c40 CGameScriptInterface::GetAllThingsWithDefNameByDistanceFrom`
  landed as a 358-byte `RELOCATION_MATCH`. It resolves the definition name to a
  search key, obtains the temporary thing-pointer range, returns zero for an empty
  result, reserves output, gets the three-dword origin through `fromThing` virtual
  slot `0x18`, sorts by distance, converts/appends each 12-byte `CScriptThing` with
  balanced reference ownership, frees the pointer range, and returns total output
  size. Empty/populated ordering behavior and the selected build pass.
- [x] `00a76f30 CNavQuadTree::IsAreaBlockedByLines` landed as a 371-byte
  `RELOCATION_MATCH`. It walks the supplied vector of sentinel-headed line lists,
  applies retail's x87 min/max X/Y broad phase with global epsilon
  `0x0129ba3c`, then accepts either contained endpoint or the line/box intersection
  predicate. Empty, broad-phase reject, endpoint hit, intersection-only hit, and
  miss behavior pass, as does the selected build.
- [x] `008a86c0 CGameScriptInterface::GetAllCreaturesInAreaWithScriptName`
  landed as a 452-byte `RELOCATION_MATCH`. Retail converts X/Y/radius through
  `__ftol2` to an integer rough-area box; an empty name selects the potential-
  customer search, while a nonempty name is copied, passed to the named-creature
  search, and destroyed. Results use the proven 12-byte `CScriptThing` append and
  balanced-reference grammar. Search selection, box, empty/populated count, and
  cleanup behavior pass, as does the selected build.
- **QUEUE QUARANTINE:** `0x00401067 entry` is the compiler-owned CRT/SEH startup
  shell, not an ordinary manual lift. It validates PE managed metadata, initializes
  CRT globals/tables, parses the multibyte command line, calls WinMain, and chooses
  `exit` versus `_cexit`; retail also contains unwind/filter tails absent from the
  generated structured body. Per `FULL_DECOMP.md`, exact entry parity remains
  deferred until the GFMain integration boundary is useful. Both generated copies
  now carry `RE_AGENT_SEMANTIC_REVIEW` so offline promotion cannot overclaim it.
- [x] `00a80360 Vector_ConstructInitialized_C2DLineF`, the first missing
  `UpdateLines` dependency, landed as a 122-byte `RELOCATION_MATCH`. It clears
  the vector triple, allocates `count * 16`, copy-initializes every POD slot from
  VC7.1's unspecified 16-byte default seed, publishes identical finish/capacity,
  and returns the vector. Zero/three-element endpoint and copy behavior plus the
  selected build pass.
- **AUDITED NAVIGATION FRONTIER:** `0x00a781a0 CNavQuadTree::UpdateLines` is a
  coherent 1,244-byte grid mutator, not ready for a weak opaque promotion. Retail
  rebuilds clipped cell boxes, clears affected sentinel lists, maps every input
  line to clipped cell ranges with x87 half-cell rounding, tests endpoint/intersection
  coverage, and allocates/splices 24-byte line nodes. Its inclusive temporary-box
  bound cannot reach `count` under the preceding clipped loops; allocation assumes
  success. Continue with phase-isolated fixtures now that `0x00a80360` is landed.
- [x] Four tiny forwarding catches landed as `RELOCATION_MATCH` functions:
  `00447390` (13 bytes), `00449960` (10), `00449970` (10), and `00449980`
  (16, tail-forwarding the resolved player). All fixtures and selected builds
  pass; the combined six-address landing audit is multiplicity-clean.
- [x] `0042a8bd MemoryAllocator_CountAllocatedBlocks` landed as a 35-byte
  `RELOCATION_MATCH`. It follows the allocation chain to the supplied sentinel,
  counts links, and returns the count. Fixture, selected build, and multiplicity
  checks pass.
- [x] `00443540 Catch_0047e8cc` landed as the seventh 63-byte
  `RELOCATION_MATCH` member of the null-safe polymorphic range cleanup/free/
  owner-delete family. Fixture, selected build, and multiplicity checks pass.
- [x] Remaining tree-pane destructor twins landed: `00443c10
  CGuiControlTreePane_Destructor_3` is a 59-byte `RELOCATION_MATCH` twin of
  `_2`, and `00444e10 ..._Destructor_4` is a 60-byte `RELOCATION_MATCH` twin
  of `_5`. Both selected builds and one-entry audits pass; only `_1` remains a
  bounded register-coloring residue.

- [ ] `00ca9c80 Object_AcquireField26` is behavior-complete `DIFFER(53v58)`.
  The member ABI, slot-`0x38` acquire, guards, and slot-`0x14` release agree;
  VC7.1 still folds the retail source-load and release-call schedule.
- [ ] `00ca8610 COM_QueryInterface_012a9a24` is behavior-complete
  `DIFFER(67v70)`. Identity probing, self HRESULT, IID query, output at `+0x9c`,
  and failure-only return agree; preservation/stack scheduling remains.
- [ ] `00c8bae0 Script_GetEntryPointer` is behavior-complete `DIFFER(40v43)`.
  All branches and the 16-byte stride agree; retail reuses `EDX` for the active
  flag while VC7.1 retains a memory compare.
- [ ] `00441c20 TreePane_DestructorHelper` is behavior-complete
  `DIFFER(69v75)`. Tree clear/free, 8-byte callback destruction, and callback
  storage free agree; VC7.1 colors owner/current/end as `ESI/EDI/EBX` instead
  of retail's `EBX/ESI/EDI`.
- [ ] `00441800 Catch_0047e8cc` is behavior-complete `DIFFER(83v99)`.
  Both 8-byte virtual-destruction ranges and ordered backing-store frees pass.
  Retail uses `EDI` for the owner and `ESI` for each iterator and retains two
  alignment NOP regions; VC7.1 colors owner/iterator as `ESI/EDI` and emits the
  compact loop layout.
- [x] `0088fd00 CGameScriptInterface::EntitySetInLimbo` is behavior-complete
  `DIFFER(80v84)`. Entity resolution, inactive rejection, `SetInLimbo`, and
  bit-6 update at `+0x92` pass. A width-preserving volatile read recovers
  retail's saved-`EBX` call setup, but VC7.1 pops it before the flag update
  instead of reusing `BL`.
- [ ] `00caa040 GetBufferOffset_Locked` is exact-length `DIFFER(60v60)`.
  Null-adjusted critical-section selection, imported lock/unlock calls,
  `(write-read)+base`, output, and zero return agree. Only the three arithmetic
  registers differ (`ECX/EAX/EDX` retail versus `ECX/EDX/EAX` VC7.1).

Keep all three out of byte-match totals until these compiler residues close.

`00c942b0 Audio_ComputeLog2WeightedCoeff` was inspected but not authored for
landing: retail consumes undocumented live `ESI`/`EDI` values in addition to
three stack arguments. Preserve it for a custom-ABI lane rather than inventing
ordinary C++ parameters.

`0042cde0 CompareAndCountAllocations` also belongs in the custom-ABI lane. Its
count-helper call pushes three arguments but cleans only eight caller bytes
before constructing two arguments for member cleanup. Recover that mixed
callee-cleanup contract before authoring; do not assume ordinary cdecl.

- [ ] `00ca5e70 CritSec_GetDividedValue_Locked` is behavior-complete
  `DIFFER(90v107)`. Retail divides signed 64-bit `(+0x114 : (+0x110-1))`
  by signed `+0x00`—correcting the lifted operand interpretation—under the
  critical section at interface pointer `-0x64`. VC7.1 merges retail's separate
  failure/success unlock epilogues and uses a compact explicit-helper call.
- [ ] `0045493e Copy_MemoryStepped` and `0045bcc1 CActiveFile_CopyRange` are
  behavior-complete exact-length `DIFFER(35v35)` twins. Empty/overlap/range
  return behavior and the recovered extra unused stack argument (`ret 8`) pass;
  only `mov ESI,EDX; sub ESI,ECX` versus VC7.1's reversed two-instruction
  schedule remains.
- [ ] `0045c71c CActiveFile_AssignVector4` is behavior-complete
  `DIFFER(55v63)`. Allocation and range copying pass; VC7.1 strength-reduces
  retail's retained source/destination delta into two advancing pointers.
- [ ] `0045bbc5 Vector_AllocateAndCopyRGB` is behavior-complete exact-length
  `DIFFER(68v68)`. Zero/nonzero allocation, empty/nonempty ranges, fastcall RGB
  construction, and returned allocation pass; VC7.1 swaps retail's `ESI`
  source / `EDI` delta assignment.
- [ ] `004495d0 SoundPair_MakeHeap` is behavior-complete `DIFFER(84v82)` in
  the independent sweep (a direct base `/O2` compile reaches 82 bytes). Range
  size/12, logarithmic depth, doubled-depth sift-down arguments, comparator
  forwarding, and final make-heap call pass; retail uses three saved registers
  while VC7.1's speed form colors the same lifetimes across four.
- [ ] `00410550 MemoryPool_AdvancePointer` is behavior-complete
  `DIFFER(61v67)`. Fast four-byte advance and slow free/rotate/publish behavior
  pass; VC7.1 merges retail's duplicated current-store/return epilogues.
- [ ] `00410d80 bitset_advance_iterator` is exact-length `DIFFER(48v48)` after
  recovering eight stack slots (`ret 0x20`). VC7.1 hoists `cursor += 4` and uses
  `[cursor]`; retail loads `[cursor+4]` before incrementing. Multi-block behavior
  passes.
- [ ] `0042bf06 CEngineSceneGrid_Initialize` is behavior-complete
  `DIFFER(41v47)`. The one-argument member ABI, stack-tail scratch initializer,
  bounding-box/value/pointer forwarding, and returned receiver pass. Frame
  shape is recovered with `/Oy-`; retail still materializes the three call
  arguments through a different push/stack-slot schedule.
- [ ] `004121d0 Std_Vector_PushBack_WithGrow` is behavior-complete
  `DIFFER(48v50)`: four-byte spare-capacity copy and five-argument growth
  forwarding pass; VC7.1 merges the two retail returns.
- [ ] `004428b0 vector_push_back` is exact-length `DIFFER(58v58)`: eight-byte
  copy/growth behavior passes; retail preserves the first dword in `ESI` and
  uses `EDX` for the slow-path reference while VC7.1 reloads/uses `ECX`.
- [ ] Cleanup variants `00441b90` and `00442e60 Catch_0047e8cc` are behavior-
  complete `DIFFER(60v67)` and `DIFFER(72v78)`. The former adds pre-range heap
  free; the latter adds smart-pointer cleanup and flag-controlled owner delete.
  Both compact through different saved-register/common-epilogue schedules.
- [ ] Cleanup variants `00442190` and `00442280 Catch_0047e8cc` are the paired
  two-range forms. Their recovered offset-specific implementations are behavior-
  complete `DIFFER(83v99)`; retail retains a longer second-loop setup and branch
  layout than VC7.1 emits from the shared C++ shape.
- [ ] Cleanup variants `00442d50` and `004430f0 Catch_0047e8cc` reproduce the
  same `DIFFER(72v78)` shape as `00442e60`: member cleanup at `+0x28`, one
  polymorphic range, optional owner delete, and returned receiver. Only the
  cleanup-call relocation differs, so keep all three in the codegen-residue lane.
- [ ] `00432ff0 Vector_AllocateAndCopyCCharString` is behavior-complete
  `DIFFER(80v71)`. Its three-argument callee-clean ABI, conditional `count * 4`
  allocation, four-byte source walk, placement copy construction, and returned
  allocation are recovered; VC7.1 retains a longer placement/copy schedule.
- [ ] `009f1810 CGameEvent::CompressIntoBuffer` is behavior-complete
  `DIFFER(76v82)` with the intrinsic-copy source shape. It writes the replacement
  bit into the type word, player/end-position header bytes, copies `EndPos`
  payload bytes from object offset `+5`, and returns `EndPos + 4`.
- [x] `00893ec0 CGameScriptInterface::MsgOnHeroSlept` landed as a 110-byte
  `RELOCATION_MATCH`. Keeping the first message-frame limit live across the
  second call recovered retail's saved-`EDI` shape; message lookup, day output,
  true/false returns, focused behavior, selected build, and multiplicity pass.
- [x] Corrected the swallowed-thunk oracle boundaries for `00450a90` and
  `00450a14 _Destroy` from 34/40 bytes to 28 bytes. Both fastcall range loops
  landed as size-optimized `MATCH`, with strides `0x54` and `0x40` respectively.
- [x] `0089b450 CGameScriptInterface::ChangeHeroHairstyle` is behavior-complete
  `DIFFER(116v113)`. Target validation, interface key `0x5e`, definition lookup,
  and positive-index appearance replacement pass; retail shrink-wraps `ESI`
  around the lookup branch while current source saves it at entry.
- [x] `00898ec0 CGameScriptInterface::SetHeroHandLampAsLit` is behavior-complete
  `DIFFER(113v114)`. The null/flag/mode gates, interface key `0x27`, `SetActive`
  call, and retail's raw-value-as-receiver fallback pass; one byte remains in
  the two call-setup/pop schedules after making the raw parameter volatile.
- [x] Corrected `00456010 ~CPatchTesselationEdgeStrip` from a swallowed 61-byte
  oracle row to its true 39-byte body and landed it as size-optimized
  `RELOCATION_MATCH`. Ordered frees at `+0x50`/`+0x3c`, base destruction,
  focused behavior, selected build, and multiplicity pass.
- [x] Corrected `00450b1b _Destroy` from 94 bytes to its true 31-byte body and
  landed the size-optimized fastcall range loop as `MATCH` (stride `0xc4`).
- [ ] The corrected 39-byte `0045641a OnDie` body is behavior-complete
  `DIFFER(46v39)` under both RTM and QFE VC7.1. Cleanup at `+0x34`, embedded
  cleanup at `+0x28`, vtable publication, and fastcall base forwarding pass;
  retail forms a tail jump while both compilers retain call/epilogue code.
- [ ] The corrected 41-byte `0044fa6d ~CBossDef` body remains unlanded. Its
  reverse ordered embedded cleanups at `+0x50`, `+0x4c`, and `+0x28` are
  recovered; the ordinary compiler-generated destructor is `DIFFER(62v41)`
  because retail uses a custom vtable-reset/fastcall tail-destructor shape.
- [x] `00892cf0 CGameScriptInterface::SetCreatureGeneratorsEnabled` is behavior-
  complete `DIFFER(92v117)` for whole-world and named-region updates, but its
  retail null-string path performs one-byte `repe cmpsb` against the empty
  literal using incoming `ESI` without initializing it locally. Route this to
  custom/live-register ABI analysis; do not force an ordinary member model.
- [x] `0089fda0 CGameScriptInterface::CreateRumble` is exact-length
  `DIFFER(120v120)`. The corrected ABI has five callee-clean stack dwords: result,
  position, two scalar factory values, and owned name; the factory receives a
  byte false in `DL`, not a masked 32-bit value. Success/fallback behavior passes.
- [x] `0089fd20 CGameScriptInterface::CreatePhysicalBarrier` is exact-length
  `DIFFER(123v123)`. Null-end substitution, fastcall obstruction creation,
  success binding, fallback result initialization, and owned-name cleanup pass.
  Both creation wrappers differ only in failure-path cleanup-address versus
  vtable-store scheduling and remain outside parity totals.
- [x] `00896e60 CGameScriptInterface::SetPreferredQuickAccessItem` landed as a
  127-byte `RELOCATION_MATCH`. Resolver/capability gates, interface key `0x11`,
  null inventory, add/remove split at slot `-1`, selected build, focused behavior,
  and multiplicity pass. Using literal `0x11` in the returned-key comparison
  removed the final three-byte local-key reload.
- [x] `00898b30 CGameScriptInterface::SetWeaponAsHerosActiveWeapon` landed as a
  127-byte `RELOCATION_MATCH`. Positive definition lookup, hero/interface key
  `0x13`, inventory count gate, null-component activation, build, and audit pass.
- [ ] `0088e3d0 MiniMapAllowRouteBetweenRegions` is exact-length
  `DIFFER(33v33)`. The first provider virtual consumes both region references and
  the allow flag; the returned world-map call takes no arguments. Behavior passes;
  only argument/vtable register coloring differs.
- [ ] `008931b0 MsgIsLevelLoaded` and `00893240 MsgIsLevelUnloaded` are
  exact-length `DIFFER(135v135)` twins. Their pointer-slot parameter ABI permits
  reuse for event codes `0x24`/`0x25`; map lookup, zero-map false path, two frame
  limits, three-pointer filter, and message-to-bool conversion pass. Remaining
  differences are filter-local register and store ordering only.

## P3 — broad backlog

Current remaining staged supply:

- batch14: 317
- batch16: 170
- batch17: 62
- batch15: 15
- batch3: 55
- batch4: 27
- batch5: 40
- retry1: 4

Do not launch these ahead of the product-facing fse queues and small batch11–13 cleanup.
For `retry1` and the old batch3–5 residue, repair merged/over-length oracle rows before
another agent wave.

### 2026-08-26 fse2 expression/follow parity

- [x] Recover and behavior-gate `008953a0 IsHeroPerformingExpression`; bounded
  at `DIFFER(151v136)` with the `0x8f` expression interface/query path intact.
- [x] Recover and behavior-gate `008955c0 IsEntityFollowingHero`; bounded at
  `DIFFER(143v136)` with the `0xbc` followed-component query path intact.
- [x] Recover `00897190 GetNumberOfItemsOfTypeInInventory`; bounded at
  `DIFFER(139v137)` after reproducing both interface and fallback receivers.
- [x] Correct the omitted source-thing ABI in `00896120 TryToRespawnDefNamed`;
  behavior-complete at `DIFFER(146v143)`.
- [x] Recover `0088f570 Play2DSound`; behavior-complete at `DIFFER(150v147)`
  with exact sound globals, parameter block, and virtual-slot semantics.
- [x] Recover `0089c7b0 EntityPostOpinionDeedKeepSearchingForWitnesses`;
  behavior-complete and exact-length at `DIFFER(147v147)` after correcting the
  deed-log flag slots to dword ABI.
- [x] Recover `0089fc80 CreateExplosion`; behavior-complete at
  `DIFFER(150v147)` with owned-vector cleanup proven on both exits.
- [x] Recover `00893b00 MsgOnExpressionPerformed`; behavior-complete at
  `DIFFER(154v151)`, including both distinct world-frame accessors and temporary
  string destruction.
- [x] Recover `00894bf0 IsGiftRomantic`; behavior-complete at
  `DIFFER(184v159)` with exact nested-def and refcount cleanup semantics.
- [x] Independently gate `00894c90 IsGiftFriendly` at `DIFFER(185v158)` and
  `00894d30 IsGiftOffensive` at `DIFFER(184v159)`; both preserve all refcount
  cleanup paths.
- [x] Independently gate recipient opinion-deed wrappers `0089c670` and
  `0089c710`; both are behavior-complete at `DIFFER(167v160)` with post modes
  `1` and `4` respectively.
- [x] Retry `00897150 UnSetThingAndCarriedItemsNotAffectedByScreenFilter`;
  exact-length `DIFFER(64v64)`, with the quarantined sentinel dereference proven
  to be retail's own interface-presence precondition.
- [x] Retry `008a0c10 EntityGetAppearanceSeed`; exact-length
  `DIFFER(73v73)` with output-preservation and retail sentinel semantics proven.
- [x] Retry `006e7690 DeregisterTimer`; corrected its false found-node
  initializer and gated it at `DIFFER(78v74)`.
- [x] Retry `0089c510 OpinionSourceSetAsAttentionGrabbing`; exact-length
  `DIFFER(74v74)` with sentinel semantics validated.
- [x] Retry `008a0bc0 EntitySetAppearanceSeed`; behavior-complete at
  `DIFFER(74v76)`, leaving only retail's explicit seed-load instruction.
- [x] Independently gate the 79-byte opinion setter trio at `0089c330`,
  `0089c380`, and `0089c3d0`; each is behavior-complete at `DIFFER(82v79)`.
- [x] Retry `0089c850 RemoveOpinionDeedStillSearchingForWitnesses`; behavior-
  complete at `DIFFER(77v79)` with sentinel semantics validated.
- [x] Retry `00890820 FadeScreenOut`; behavior-complete at `DIFFER(85v80)`
  with the active/override gate and byte-exact color ordering recovered.
- [x] Bounded `00891070 CameraUseCameraPoint` at `DIFFER(75v83)` with behavior
  PASS. The reviewed source preserves the two null-only gates and exact camera
  component/vtable offsets; the remaining delta is compiler code shape.
- [x] Bounded `008961b0 ClearHeroEnemyOfGuards` at exact-length
  `DIFFER(83v83)` with behavior PASS; the resolve/flag gates, interface `0x22`
  lookup, sentinel precondition, and `+0xd4` clear are validated.
- [x] Bounded `0089e130 EntitySetMaxNumberOfAttackers` at exact-length
  `DIFFER(83v83)` with behavior PASS; the resolve/flag gates, interface `0x36`
  lookup, sentinel precondition, and `+0x44` store are validated.
- [x] Reconciled the stale `0088fd00 EntitySetInLimbo` retry row with its
  existing `DIFFER(80v84)` behavior-complete handoff and bounded it.
- [x] Bounded `00896060 SetVillageLimbo` at `DIFFER(83v85)` with behavior
  PASS; the village interface lookup and bool forwarding are validated.
- [x] Bounded `00896270 EnableGuards` at `DIFFER(83v85)` with behavior PASS;
  its structure matches the adjacent village-interface wrapper.
- [x] Bounded `008978e0 EntitySetAsAbleToRegionFollowWhenMarried` at
  exact-length `DIFFER(86v86)` with behavior PASS.
- [x] Bounded `00898fa0 GetHeroGold` at exact-length `DIFFER(86v86)` with
  behavior PASS; two-hop hero resolution and the gold component read pass.
- [x] Corrected and bounded `00895130 SetTrapAsActive` at exact-length
  `DIFFER(88v88)` with behavior PASS; the generated snapshot incorrectly used
  `this` as the resolver receiver instead of the supplied script thing.
- [x] Bounded `00897880 GetHeroTitle` at exact-length `DIFFER(88v88)` with
  behavior PASS.
- [x] Bounded `0089ef30 SetMoralityChangingAsEnabled` at exact-length
  `DIFFER(88v88)` with behavior PASS.
- [x] Bounded `008a0f70 SetReadableObjectText` at exact-length
  `DIFFER(88v88)` with behavior PASS.
- [x] Bounded `00897c90 GetNumberOfTimesHeroHasHadSex` at exact-length
  `DIFFER(89v89)` with behavior PASS.
- [x] Bounded `008962d0 EnableVillagerDefTypes` at `DIFFER(88v90)` with
  behavior PASS.
- [x] Bounded `00897090 SetWhetherToFailQuestOnDeath` at exact-length
  `DIFFER(91v91)` with behavior PASS.
- [x] Bounded the hero-history setter trio with behavior PASS: `00897cf0`
  count at `+0x154`, `00897d50` bool at `+0x158`, and `00897db0` bool at
  `+0x159`; all are exact-length `DIFFER(91v91)`.
- [x] Reconciled stale `008984e0 SetWeaponOutCrimeEnabled` retry state with
  its existing exact-length `DIFFER(91v91)` behavior-complete handoff.
- [x] Bounded `00898540 SetGuardsIgnoreCrimes` at exact-length
  `DIFFER(91v91)` with behavior PASS.
- [x] Reconciled stale `0088e2a0 IsLevelLoaded` retry state with its existing
  `DIFFER(87v94)` behavior-complete handoff.
- [x] Bounded `008a1650 GetBestScoreBlackjack` at exact-length
  `DIFFER(94v94)` with behavior PASS, including the float-to-long conversion.
- [x] Bounded the remaining score getter trio at exact-length
  `DIFFER(94v94)` with behavior PASS: OakVale `+0x184`, SnowSpire `+0x188`,
  and ShoveHaPenny `+0x18c`.
- [x] Bounded `0088ead0 EntitySetAsDamageable` at `DIFFER(97v95)` with
  behavior PASS, including both notification callbacks.
- [x] Bounded `00899000 GiveHeroExperience` at `DIFFER(93v95)` with behavior
  PASS.
- [x] Landed `008a12a0 GetNumHousesOwned` as a 95-byte
  `RELOCATION_MATCH`; behavior and selected build PASS.
- [x] Reconciled stale `0089b3f0 SetHeroAsWearing` retry state with its
  existing `DIFFER(94v96)` behavior-complete handoff.
- [x] The FSE1 retry queue is exhausted (all 63 remaining rows bounded).
- [x] Audited all 21 FSE2 ranked rows against `HANDOFF.md`; every address has
  an explicit behavior-complete bounded entry. The file is an unlanded ledger,
  not an actionable retry queue.
- [ ] Continue with the first small non-quarantined promotion candidate,
  `0088f480 ApplyScriptBrush`, while preserving the bounded parity ledgers; no
  runtime or visual testing.
  testing.
- [x] Audited and bounded `008a9ae0 MsgOnBoastsMade` at `DIFFER(279v352)` with
  behavior PASS; the two frame bounds, type `0x29`/extra-data query, event and
  string offsets, eight-byte vector append paths, result predicate, and list
  ownership are validated. The residue is old STL cleanup/codegen scheduling.
- [x] Removed poisoned `00896390 OpenChest` from ordinary promotion: its generated
  register shims call unrelated `0040d959`/`00488dc2` targets in place of retail's
  `00449970`/`00487dc0` hero-resolution chain. Both copies are marked for semantic review.
- [x] Bounded `00c93b30 Audio_ApplyGainTableToBuffer` at `DIFFER(250v260)`
  with behavior PASS; clamp, four-wide and scalar processing, indexed gain lookup,
  and zero-tail semantics are validated. The residue is register/pointer scheduling.
- [x] Fixed COFF switch-label parsing consistently across all six verifier,
  audit, residue, and permuter paths; each now extracts the complete 744-byte
  `GFHandleSystemInitError` body, and all modified Python modules compile.
- [x] Reconciled thirteen previously documented codegen residues into the
  structured bounded ledger, reducing the promotion pool to 287 candidates
  (83 bounded exclusions, 62 semantic quarantines) without changing parity claims.
- [x] Bounded `0045d264 CActiveFile_AssignVector8` at `DIFFER(77v68)`
  with behavior PASS; count-times-eight allocation, empty range, two-dword copy,
  null allocation handling, and the three-argument `ret 0x0c` ABI are validated.
- [x] Landed `0045d901 TreeNode_AllocData` as a 34-byte
  `RELOCATION_MATCH["s"]`; 24-byte allocation, two-dword payload at `+0x10`,
  allocation-base return, fixture, and selected build pass.
- [x] Landed `0045e2b1 TreeNode_AllocData_Byte` as a 34-byte
  `RELOCATION_MATCH["s"]`; dword/byte payload at `+0x10/+0x14`, allocation-base
  return, fixture, and selected build pass.
- [x] Landed the additional `0045de2f` and `0045e797
  TreeNode_AllocData_Byte` instances as independent 34-byte
  `RELOCATION_MATCH["s"]` bodies with focused fixtures and selected builds.
- [x] Corrected `0045deba Map_EraseNode`'s oracle boundary from 45 to 47 bytes
  and landed it as `RELOCATION_MATCH["s"]`; four erase-helper arguments,
  conditional free, count decrement, `ret 4`, fixture, and selected build pass.
- [x] Completed the retail-byte sweep for the 34-byte tree-node allocator
  grammar: 27 additional two-dword instances and tagged-byte `005baf06` landed
  as `RELOCATION_MATCH["s"]`. All 28 fixtures, selected builds, and one-per-address
  catalog/oracle/source/test/ledger checks pass.
- [x] Completed the 47-byte erase/free member sweep: all ten retail instances
  are landed after adding nine `RELOCATION_MATCH["s"]` twins. Their fixtures,
  selected builds, and one-per-address artifact checks pass.
- [x] Landed the complete 32-byte list-sentinel default-constructor family:
  all 109 authoritative retail starts are `RELOCATION_MATCH` with focused
  self-link/owner-publication fixtures, individual selected-build passes, and
  one-per-address source/test/catalog/oracle multiplicity. This includes the
  `00a80480` and `00a80520` navigation dependencies. Corrected the initial
  30-byte sweep oracle to include the full `ret 4`, and corrected poisoned
  `004ab4d0` from a propagated vector identity to a neutral sentinel identity.
- [x] Landed `00a57bd0 C2DLineF::IntersectsWith(C2DBoxF const*)`, the remaining
  non-CRT call dependency inside `CNavQuadTree::UpdateLines`, as a 227-byte
  `RELOCATION_MATCH`. Retail constructs bottom/right/top/left box edges, probes
  them in that order through the line/line overload with the global tolerance,
  and short-circuits on the first hit. Edge construction, call order, miss, all
  four hit positions, multiplicity, and selected build pass.
- [x] Promoted `00897bd0 CGameScriptInterface::GetHeroHasChildren` out of the
  semantic-review queue as a 92-byte `RELOCATION_MATCH`. Retail uses resolver
  `00487dc0` (not the generated lift's poisoned `00487dc2`), enforces target
  gates `+0x91 & 1 == 0` and `+0x20 & 0x10 != 0`, lower-bounds interface key 4,
  intentionally falls back to the populated map sentinel, and calls hero-stats
  marriage-state query 4. Gate, direct-entry, sentinel, ordering, multiplicity,
  and selected-build checks pass.
- [x] Landed the 92-byte sibling `00897c30
  CGameScriptInterface::GetHeroHasMurderedWife` as `RELOCATION_MATCH`. It uses
  the identical resolver/gate/key-4/sentinel contract and queries marriage state
  7 instead of state 4. Focused direct/sentinel/gate behavior, multiplicity, and
  selected build pass; semantic quarantines fall to 62.
- [x] Closed the remaining adjacent hero-marriage query family:
  `00897b70 GetHeroHasDivorcedMarriage` is the 92-byte state-5 twin,
  `00897b10 GetHeroHasCurrentMarriage` is an 88-byte no-argument stats-query
  variant, and `00897aa0 GetHeroHasMarried` is a 100-byte marriage-list
  nonempty check at hero-stats `+0x1d8`. All are `RELOCATION_MATCH` with
  direct/sentinel/gate and result-specific fixtures, selected builds, and clean
  multiplicity. Semantic quarantines fall from 62 to 59.
- [x] Promoted `008a9610 CGameScriptInterface::SetThingAsConscious` as a
  162-byte `RELOCATION_MATCH`. Neutral address-specific seams replace poisoned
  GUI/lightning action identities. Validation, creature gates, both action
  branches, reason forwarding, installation/destruction order, and selected
  build pass.
- [x] Promoted `0089b4d0 CGameScriptInterface::RemoveHeroHairstyle` as a
  91-byte `RELOCATION_MATCH`. Retail uses hero resolver `00449970`, not the
  generated `0040d959`; key-`0x5e` direct/sentinel lookup, gates, forwarded
  `false`, behavior, and selected build pass.
- **NEXT OFFLINE RESUME:** refresh the 295-candidate promotion queue and inspect
  `008a1590`, `008a15f0`, and `008a17d0 GetBestTime*`. Keep `00a781a0
  CNavQuadTree::UpdateLines` fixture-first; its bounded dependencies are landed.
  Do not launch the game or claim the pending stage46 runtime gate.
- [x] Landed the complete adjacent `GetBestTime*` family: `008a1590` pairs,
  `008a15f0` sorting, and `008a17d0` guess-the-addition are 93-byte
  `RELOCATION_MATCH` bodies with direct/sentinel/key-too-large and all hero-gate
  behavior covered. Their fields are hero-stats `+0x178/+0x17c/+0x190`; fallback
  is `0122dedc`. Selected and full 18,604-object builds pass; semantic
  quarantines fall from 59 to 56.
- [x] Audited AlbionSecrets' `StatueMaster.zip` against native TLC and PDB-backed
  FableWin script implementations. Recorded the retail `TraderToEscort` cellar
  gate omitted by the Lua port, confirmed the unused Guild branch and remaining
  high-level behavior, and separately reviewed odarenkoas' quest-layer files in
  `docs/STATUEMASTER_LUA_PORT_AUDIT.md`.
- [x] Exhausted the available PC StatueMaster cut-content evidence across three
  gameplay executables, PDB symbols, localization, compiled definitions, and
  extracted level/quest data. Recovered the exact Fire, Steel/Sharpening,
  Silver, Diamond/Piercing, and Lightning category mapping; found no surviving
  reactive entity; documented the Greatwood Piercing chest and two flanking
  unscripted statues. Added the cited report and an explicitly non-parity
  five-seal restoration concept in
  `docs/STATUEMASTER_DEEP_RESEARCH_REPORT.md`.
- [x] Analyzed the locally extracted original Xbox `default.xbe`, including the
  real `S_VSM` overlay. Xbox has the same missing Guild presentation arm and
  orphan five-clue text as PC; see `docs/STATUEMASTER_XBOX_XREF.md`.
- [x] Started the explicitly reconstructed Lookout Point world event under
  `mods/StatueMasterRestoration`: entity id 61 now has a targetable,
  effectively indestructible hit-observing Lua prototype, namespaced seal
  state, and a fail-closed Piercing/Diamond check. ForgeFSE now exposes a
  `MsgGetHitByWeaponAugmentations` diagnostic bridge which copies the matched
  `CEventHitBy` payload instead of inspecting equipped state. All ten verified
  bit values now translate to stable names; the ten-case live probe remains,
  especially projectile/coroutine-window behavior. No reward was invented and
  no DLL was deployed to a game installation.
- [ ] Run `mods/StatueMasterRestoration/RUNTIME_PROBE.md` in game. Preserve the
  complete `SM_HIT_PROBE` rows for primary/same-window/post-yield polling and
  verify plain melee, Flame, Piercing, two augmentations, augmented bow, weapon
  swap, non-hero hit, debounce, yield advancement, and unknown-value handling.
- [x] Added the offline five-seal reconstruction state machine for Flame,
  Sharpening/Steel, Silver, Piercing/Diamond, and Lightning. It is namespaced,
  duplicate-safe, fail-closed, migrates the original `LOOKOUT_SEAL`, and is
  executable-tested. Only Piercing is bound to a shipped world entity so far;
  satellite placement, completion presentation, and reward remain new design.
- [x] Audited AlbionSecrets' 2025-11-05
  `ALL-INTERFACE-FUNCTIONS-FOR-FSE.h`: 947 declarations normalize to 933 unique
  Lua names (88 Entity + 845 Quest) and 14 extra overload declarations. Restored
  the omitted `StartAmbientConversation` and `AddLineToConversation` manifest
  records, regenerated the API/overlay artifacts, and added a repeatable header
  checker plus `docs/FSE_TUTORIAL_API_AUDIT.md`. Tooling validation and the
  458/458 ForgeFSE Quest-binding audit pass.
- [x] Propagated the corrected 933-function manifest into FableForge, updated
  stale 931/843 assertions and quest-node totals, rebuilt the Release CLI/GUI,
  and restored the complete frontend suite to 14/14 passing CTests.
- **NEXT OFFLINE RESUME:** author `00a781a0 CNavQuadTree::UpdateLines`
  fixture-first, or inspect the refreshed non-quarantined promotion head. The
  stage46 runtime gate remains pending and was not exercised.

## Resume rules

- One authoring lane at a time unless rate limits are demonstrably stable.
- Never count self-reported wins; independent VC7.1 verification is mandatory.
- Keep behavior-only `DIFFER` functions out of byte-match totals.
- Preserve automation-owned dirty files and commit scoped changes only.
- Update the three generated TSVs after every landing wave so completed addresses disappear.
### stage45 — native repeated-mesh grass runtime result (2026-08-22, FAILED VISUAL GATE)

- [x] Replaced the failed type-0 grass shortcut with the retail type-1 grammar.
- [x] Recovered paired instance vectors from a real retail batch:
  orientation/scale followed by world position/scale.
- [x] Authored 300 grass instances over ForgeTest64; corrected scanner reports
  300 bound, 0 unbound, X 3329..3391, Y 2306..2367, Z 72..84.
- [x] Fixed the scanner's post-frame four-byte alignment bug.
- [x] Added a byte-level repeated-grass regression test; full test suite passes.
- [x] Assembled `stage45_repeated_grass_bright_runtime_package`; `forge validate`
  reports CLEAN. Terrain chunk hash prefix `F91FAF8D5B2DC2B6`, STB hash prefix
  `9D6EC9E93C109765`.
- [x] Raised local-detail probe retention from 512 to 5000 calls so the custom
  map remains observable after retail startup streaming (probe DLL `C06F9D9F2D1859B2`).
- [x] Deployed and run. ForgeTest64 loads and the hero arrives in bounds, but no
  grass is visible. The installed live log reaches `arrival verification
  map=ForgeTest64 in_target_bounds=true`.
- [x] Offline structure remains internally consistent: the extracted stage45
  chunk scans as 300 bound instances, zero unbound, in X 3329..3391 / Y
  2306..2367 / Z 72..84.
- [ ] **ACTIVE BLOCKER:** the authored collection payload is never opened at
  runtime. The retained hook logs many retail `CObjectTypeCollection::Load`
  calls around the transition, but no custom repeated-mesh stream (decoded size
  approximately 9.7 KiB) after ForgeTest activation. Therefore the failure is
  before primitive parsing/rendering, in local-detail root/group spatial update
  or file-block selection. Do not change the now-proven paired instance arrays
  again; instrument `CLocalDetailCacheMap::StaticUpdate` and
  `CObjectCacheGroupCollection` selection/culling next.
- [ ] **SEPARATE VISUAL BLOCKER:** the stage45 screenshot has near-black terrain
  faces. Textures and relief are present, so treat this as foreground packed-
  normal/environment-lighting input, not as a foliage or missing-texture fault.
  Compare authored foreground normals against a retail patch in the same world
  light before applying another texture-brightness workaround.

### stage46 — local-detail group sphere fix (2026-08-22, READY FOR RUNTIME)

- [x] Recovered the decisive `CObjectCacheGroupCollection::LoadHeader` /
  `StaticUpdate @ 0x00BDEB50` contract directly from retail instructions.
  Group-header bytes `+0x0C..+0x1B` are `(sphereCenterX, sphereCenterY,
  sphereCenterZ, sphereRadius)`, not `(minX,minY,maxX,maxY)`.
- [x] Corrected `buildType0LocalDetailSection` to derive a world-space sphere
  enclosing every authored placement, including its vertical extent. This
  removes the pre-payload cull that prevented stage45's ~9.7 KiB repeated-mesh
  stream from ever reaching `CObjectTypeCollection::Load`.
- [x] Added byte-level assertions for both native type-0 and repeated-mesh
  group spheres. `forge_tests` passes, and the complete 10-target CTest run is
  9/10 with only the pre-existing dirty-worktree `forge_bwd_tests` phase-3
  byte-exact fixture mismatch.
- [x] Built `ForgeTest64_terrain_v22.chunk.bin` (300 bound / zero unbound grass
  instances) and assembled
  `stage46_foliage_spherefix_runtime_package`. `forge validate` is CLEAN; its
  STB SHA-256 is
  `AECB64707DD6234240B082559C2F5557A44E2A0D4670A8F2AF1BAD0609C231B6`.
- [ ] Runtime gate: after the current Fable process is closed, deploy stage46
  through the rollback harness and require both the custom collection-load
  probe and visible grass before promotion.
