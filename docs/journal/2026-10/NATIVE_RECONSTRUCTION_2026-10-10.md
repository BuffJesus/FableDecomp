# Native recreation — 2026-10-10

## Texture-manager mip reduction

Latest async prerequisite evidence: actual-constructor autoalloc composition independently PASS7,296cases/72,960snapshots per root/model,2,304actualnull72faults; ctor/base/locks/SetupRead execute with bounded tree/Win32/heap/offset/queue. First constructor183B/53ins preliminary94.34% with root-verified E/G weak aliases; first genuine pointer-tree registration180B/78ins100% opcode and exact constructor-call symbol. Original registration independentlyPASS480/1136. Compiled registration/helper verification and constructor relocation identities remain open; no promotion/coverage-count change. Work-only review packets preserve limitations. Lua and archives untouched.

Accepted async auto-allocation root009D6100: genuine235B/93instructions near96.77419355%, eight audited zero-addend fixups and exact11B counted deleter. Fresh permanent gate PASS110772cases/544254full32KiB snapshots, strict root/GetAlignment omission links and current39input/five-toolchain hashes. Genuine7B const GetAlignment replaces mislabeled adapter without extra recovery credit; private constructor/access friendship breadth remains explicitly inferred. Typed metadata/artifacts/corrections/index/gate_all integrated. Fresh canonical closure1796sources/445unresolved/zero duplicates at `work/codex-native-startup/native-autoalloc-clean-20261010`; only new unresolved dependencies are private AsyncData constructor9D5DF0 and SetupRead9D5970. Continuation129exact/38near/6data; cumulative383exact/72near/43data. SetupRead candidate1 parityFAIL74.2424% despite bounded28608case/107280snapshot behaviorPASS; unaccepted, no retune. SetupBuffer original310B evidence under investigation. Full gate_all not run; no actual disk/queue/full-game launch claim. Older pending/count statements below are historical.

Latest permanent composition: successfulallocation gate rootPASS12288cases/24576full16KiB snapshots,nineinstalled/audited/eightexecutedproviders andallninestrict omissions. Actual constructors/accounting/registration execute beneathinitializer andmanagedchildren; allocator anddeletion/ReleaseTexture remainexternal. Expandedrootgate permanentPASS59392/240640 retaining4096faults,originalbaseline54272/215552 separate. Nativeacceptedbodies/countsunchanged. Independent lifecycle/callerABIreview active.

Section-parent9FE780 deferred aftertwofailures:360B/120instructions83.4043percent/O2 plus36caseactualFPcounterexample; unchangedsource/Op357B/119instructions88.8889percent. /Op same36domainzeroFPmismatches doesnot overridefailedparity; no thirdattemptwithoutneworiginalevidence. Source-contextfriendscaffold work-only, canonicalheadersunchanged.

Loader stream prerequisite freshlyread/rerun: memoryReadCompressed995B10 full540B/sixfixups,7200cases/17376full2KiB snapshotpointsPASS. Existinggate substitutesLZO/virtualposition/overflow andexcludesrejectedinitialprefix/uninitializedlocaldomain; not actualdecompression/completepipelineproof. Work-only investigation now targets actualgenuineReadWithSrcChunkOverflow993CA0 plusstreamlifetime beneathcompressedreader; asm-bearing siblingexcluded. Nohelperrecount.

Latest section/ownership checkpoint: first genuine C++ candidates9FCB60 and9FE550 accepted exact after permanent independent original/compiled/model gates. Section57B/21instructions/three original-target relocations,1769472cases/5308416full16KiB snapshots; private ownership208B/76instructions/seven relocations,54272cases/215552snapshots including4096original null-allocation faults. Exact-symbol source omission verified; child interfaces disclosed, no realheap/GPU/frontend claim. Typed manifest/artifacts/overrides, root-only index and gate_all integrated. Fresh canonical closure1794sources/443unresolved/zero duplicates adds only data-section9FCA40 frontier; no scratch seeds. Continuation129exact/37near/6data,cumulative383exact/71near/43data. Recorded initializer input hashes freshly match current files. Full gate_all not run. Historical pending evidence below is superseded for these two roots only.

Work-only actual-child composition reviewed:2250cases/5730full16KiB snapshots, existing-owner partition excludes allocation/constructor. Actual Uninitialise and managed assignment exact; existing managed Release accepted near. Helper upgrading canonical provenance and four-provider omissions, independent reviewer checking recursive header provenance; section-parent original evidence investigation active. No new recovery credit for composition. Archives untouched.

Resumed at the user's request, excluding Lua work. Previous accepted checkpoint remains 1,788 sources / 443 unresolved / zero duplicate definitions. No previous one-time promoter was rerun.

Target: original `CTextureManager::ReduceMipmapLevel`, retail `0x00A69F70`, donor RVA `0x0077D7F0` (VA `0x00B7D7F0`). The donor body is 824 bytes; **the complete retail body is 955 bytes**, ending at the `ret 0xc` at `0x00A6A328`. Retail manifest/signature evidence independently records 955 bytes / 335 instructions. All evidence and candidates are under `work/parity_addtext`.

The method caches the original texture before querying its level count, then reloads frame dimensions. It performs signed division by two for the dimensions, sets the current managed data's `Locked` flag, captures its owner, queries the original pixel format, and allocates a temporary managed texture. Allocation failure clears the live managed data's lock and destroys the temporary.

Success copies the replacement's counted texture, builds a rectangle `(0,0,width,height)`, and copies original mip `level + reduction` into replacement mip `level`. Rectangle dimensions are halved after each copy; source and destination surface temporaries are destroyed in reverse construction order. The surviving mip count and loop are unsigned, including wraparound. Testing bounds execution to finite level counts; no safeguard is added to the source.

The old graphic frame receives a temporary reference before the live frame is released and rebuilt using its original alpha bits. The live original managed-data owner and lock are cleared, then the managed handle is assigned from the replacement. The protected old frame, copied counted texture, and temporary managed handle are released in that order. Child callbacks can change the live owner/handle state; cached original texture, dimensions, mip count, and copied texture ownership must retain native timing.

Original `CGraphicFrame::InitialiseAsSurface(const CCountedPointer<CTexture>&,long,long,bool,bool)` declaration added to the canonical header. Both the FableWin decorated public symbol and Ego_r DIA class-method record confirm this signature. The header changes no layout or existing inline body.

First compiled draft retained out-of-line managed cleanup and assignment: 686 bytes / 240 instructions / 76.8696% normalized similarity, rejected. An older interrupted write subsequently completed and replaced the first scratch file; that file is not used as the verified input. The second candidate restores the original inline cleanup and assignment expressions and corrects copy direction/rectangle arguments: **952 bytes / 333 instructions / 94.6108% normalized similarity**, passing the near threshold. This is a parity candidate, not promoted coverage. Stop byte tuning while the behavior fixture is developed.

`validate_reduce_mipmap_20261010.py` executes the full retail body and compiler-emitted root under bounded x86 emulation against an independently expressed model. It compares callback/final snapshots of all 16 KiB arena bytes and callback scalar arguments. Child methods, C++ deletion, and COM operations remain functional boundaries. Stack addresses of live temporaries are used only to service callbacks and are not logged or compared as persistent identities. No real-heap or GPU claim.

Permanent validation passes 6,912 cases / 120,000 full-arena snapshots against native code and the independent model. The fixture audits all 28 root relocations and proves that omitting the mandatory source fails linkage. The landed root is `rebuild/src/compiled/00/a6/CTextureManager_ReduceMipmapLevel_00a69f70.cpp`; manifest, artifact index, overrides, physical symbol index and `gate_all` registration are integrated.

Integration initially exposed four duplicate inline helper definitions. Scoped inline declarations plus callable emission anchors make the existing managed Release, assignment, destructor and frame UnInitialise providers coalescible. All four canonical provider bodies and relocation maps remain byte-for-byte unchanged; the new root also retains its verified 952-byte body and all 28 fixups. Anchors are excluded from the physical engine symbol index and add no coverage. Evidence: `work/parity_addtext/managed_inline_provider_bodies_20261010.json` and `reduce_mipmap_inline_coalescing_20261010.json`.

Current-provider regressions pass: managed Release 1,152 / 2,704; assignment 73,728 / 207,360; destructor 1,152 / 2,704; frame UnInitialise 124,416 / 289,872 cases/snapshots. Clean closure `work/codex-native-startup/native-reduce-mipmap-clean-20261010` has **1,789 sources / 443 unresolved / zero duplicates**. This adds one accepted near routine: continuation 126 exact / 36 near plus 5 data; cumulative 380 exact / 70 near / 42 data. No new helper coverage is counted.

Next: recover the 57-byte `CGraphicFrame::InitialiseAsSurface` at `0x009FCB20` using its original allocation, inline 27-byte data construction and unconditional data-initializer tailcall. Allocation failure is not a safe-return branch. The nested initializer remains a separate functional boundary until independently recovered. Preserve all prior deferrals and the CPU-only/current-GPU limitations.

## Surface initialization in progress

Original frame helper `0x009FCB20` / donor `0x0076CAB0` has 57 bytes / 21 instructions. First genuine C++ candidate emits the same 57 bytes, exactly matching after auditing its allocator and two tail-jump relocations. Added the donor/native-verified inline packed-data default constructor (reference count 1, counted pointer null, initialization bit clear; all other bytes preserved) and typed data-initializer declaration. The independent allocation/argument/whole-arena test is in progress; no new coverage yet. Current-header frame-debug-memory regression passes 61,440 / 427,520; CPU heap also passes.

The nested data initializer `0x009FC950` has **230 retail bytes**, versus donor 218. First candidate is 231 bytes / 84 instructions versus retail 84, 92.8571% normalized similarity. Stop parity tuning; behavioral validation must now establish counted-owner aliasing, callback reloads, ushort truncation, alpha flags and short-circuit dimension queries before acceptance. These two candidates remain scratch-only until their respective gates and integration pass.

Native and donor independently exposed two semantic mistakes in the initial data-initializer draft: SectionOfTPage must be cleared, and BOTH width and height comparisons read dimension X, not height/Y. Corrected these behaviors without discretionary byte tuning. Final candidate is 230 bytes / 84 instructions, **95.2381% normalized similarity**. Permanent independent proof passes 18,432 cases / 47,104 full-arena snapshots, covering ownership identity/aliasing, incoming capture and live deletion reloads, wraparound, callback mutations, truncation, flags, short-circuit and live texture reload. Counted deletion and texture dimensions remain functional boundaries.

Frame wrapper permanent gate passes **36,864 cases / 110,592 snapshots**, covering every initial flag byte, null/live/owner-alias allocation, placements/fills, raw arguments and raw return bytes. Its three resolved relocation sites reproduce the original 57 bytes exactly. Both gates prove source omission fails linkage. Unicorn host hook/stop behavior caused scratch harness crashes; permanent execution uses stable hooks, explicit stop address and bounded process batches, with no candidate mismatch hidden.

Both sources/tests/metadata/overrides/index/gate_all are integrated. Clean closure `work/codex-native-startup/native-surface-initialise-clean-20261010`: **1,791 sources / 442 unresolved / zero duplicates**. Adds one exact and one near: continuation127exact/37near plus5data; cumulative381exact/71near/42data. No inline constructor, counted helper or preexisting dimension coverage is recounted. Next: compose actual frame/data initialization into the mip-reduction gate, retaining explicit allocator/dimension/deletion boundaries. Current-header mip-reduction6912/120000 and frame-debug-memory61440/427520 regressions pass.

## Progressive mip-chain composition

Scratch original/model/compiled executions each pass 6,912 bounded cases, with all listed source-omission links failing:

- Three sources, actual frame/data initializers: 128,448 full-arena snapshots.
- Four, actual texture dimensions through COM descriptor: 128,448.
- Five, actual frame-data cleanup: 125,568.
- Seven, actual managed texture release and LRU: 127,104.
- Eight, actual manager allocation with size/index/selection/create boundaries: 158,976. Failed counters seeded with 0/1/2/FFFFFFFF; invalid-index, null-pool and create-false branches stay distinct.
- Twelve, actual mip sizing/colour depth/table/private pool lookup: 131,328. Four table rows cover two uncompressed formats and DXT1/DXT5; full 1,692-byte original table is audited.
- Sixteen, actual pool selection/MRU/PC busy and reuse data: 97,536. Bounded singleton resource lists, pool count 0/1/2, busy clearing, locked selection, owner release-leave/release-clear and callbacks execute through native/rebuilt code. Wider list/table coverage remains in the unchanged standalone selection fixture.

Current-header standalone allocation passes 36,864 / 315,936; nine-provider selection chain passes 108,288 / 324,672. These compositions add no function/data coverage. The seventeen-source creator composition and its permanent fixture are running. Deferred GetNextValidDimensions and texture initialization remain functional boundaries at this stage; rounding is supplied by the fixture, not claimed as recovered. Full gate_all and real GPU recreation were not run.

Seventeen-source permanent composition now passes **6,912 cases / 104,448 snapshots**. `gate_native_texture_manager_reduce_mipmap_create.py` / `NativeTextureManagerReduceMipmapCreate_test.py` / `native-texture-manager-reduce-mipmap-create.json` register the complete creator/selection chain. All seventeen source omissions fail; each code provider repeats its relocation-aware parity check, both data providers exactly match the original, and recursive canonical-header hashes accompany source/fixture/original-body hashes. Pool lookup uses an independent linear ceiling model over the case's strictly increasing capacities, not the native binary-search implementation. No old helper or data coverage is recounted. The eighteen-source scratch variant adds actual CTexture::Initialise with explicit system/COM/byte-length boundaries and is running; the deferred rounding helper remains untouched.

Eighteen-source scratch composition passes **6,912 / 122,880**, including actual CTexture::Initialise, mip default calculation before the system callback, dimension/format mutation after that callback, raw COM creation failure with populated texture output, packed allocation-source merging and live accounting callback state. All eighteen omission links fail and provider parity is repeated. Twenty-source scratch composition now adds actual143B texture accounting and50B pixel-format lookup; signed level-count and descriptor/COM operations remain callbacks. Current-header external texture initialization gate passes8,832cases/92,736snapshots. Dirty generated COVERAGE/RUNNABLE dashboards were not overwritten; native checkpoints above remain separate from their older generated metrics.

Twenty-source permanent `gate_native_texture_manager_reduce_mipmap_byte_accounting.py` passes **6,912 cases / 129,792 full-arena snapshots**, all twenty omissions and all provider parity/data checks. Actual texture initializer,143B byte accounting and50B format lookup execute inside the recovered mip/lifetime/selection chain. GetLevelCount0/1/2/FFFFFFFF, descriptor callbacks with populated output despite failed HRESULT, two descriptions per mip, packed allocation bits and callback mutations are covered. Surface operations, owner release, deferred rounding, system accessor, base/deletion/COM remain explicit boundaries; no new helper/data coverage or real GPU claim. Source closure remains1791/442/zero duplicates.

Next loader trace: nested BeginLoading9FDE80 has complete1,262B/437instructions. DIA and the existing donor-layout export both identify CGraphicHeader::CompressedMipmapSizes as `unsigned long[1]` at+24 (not a pointer); original loader9FE202/9FE217 reads the in-object word. Canonical field corrected without layout change (header28B/info44B); no existing compiled source accesses it. Loader also calls9FC440: native16B wrapper forwards CResource to its list's9FC020 RemoveResource, not Ghidra's OnKill label. Existing canonical inline RemoveFromResourceList in the unchanged resource-list ctor TU matches the full16B and its child relocation exactly; existing child matches full80B. Independent proof/metadata correction is next, without retuning or extra helper coverage.

Resource-removal correction complete: `gate_native_resource_remove_from_list.py` executes the unchanged inline16B wrapper and actual80B child against an independent model, **6,144 cases / 12,288 full16KiB snapshots**, two mandatory omissionlinks, no behavioral callbacks. Null/live list, head alias, detached/reciprocal/broken/self/head links, unsigned count/memory/size wrap and untouched age/state are checked. Old OnKill placeholder and mimic test preserved under `rebuild/backlog/mislabels/009fc440`; active catalog entry removed, original metadata/artifact/override/physical provider corrected. No new strong leaf or extra native coverage. Current-header loading69,120/390,912, sprite29,184/115,968, constructor16,000/216,000 and registration composition16,000/152,000 regressions pass after header-array correction.

Fresh removal checkpoint remains **1,791 sources / 442 unresolved / zero duplicates** at `work/codex-native-startup/native-resource-remove-clean-20261010`. Index registers the verified wrapper once despite its shared constructor TU's two catalog roles. Relevant new-file diff checks pass; the broad catalog check reports older whitespace in unrelated existing hunks, which is deliberately preserved. Native totals unchanged127exact/37near plus5data,cumulative381exact/71near/42data. No Lua, commits, old-cache cleanup or real graphics work.

## Loader ABI prerequisites and helper review

`work/parity_addtext/probe_frame_loading_abi_20261010.py` emits a bounded DIA/retail evidence packet. Original ETextureType values0..10/count11 and donor44B PixelFormats array match the retail loader's+288/type*4 access; canonical enum/array and offset assertion added. Donor BlankTextures array88B/count11 is recorded but not added: its retail offset/full bank size remains unclaimed. Original async-entry BeginReadDataAutoAlloc has two ulong arguments and EThreadedFilePriority, returns a counted async-data pointer; WaitUntilFinished returns bool with no explicit args. Typed declarations added, not recovered implementation claims. TextureLoadingPriority retail01396C64 has initial3, donor signedlong4B; provider recovery remains pending.

Current-header regressions pass: frame loading69120/390912, sprite29184/115968, constructor16000/216000, actual registration16000/152000,20-source mip/accounting6912/129792. Native coverage counts unchanged; no new executable provider landed.

User explicitly authorized helpers. Two independent audits caught that the earlier1262B/437instruction loader span ends at stack restoration and omits RET8. Complete body is **1265B/438instructions**, terminal9FE36E RET8, padding begins9FE371. Probe now asserts both terminal return and padding; earlier span statements above are superseded. Audit emphasizes unsigned read-size division, signed/unsigned mip comparisons, cached original Header versus live later bank/index/owner reloads, and two distinct counted texture lifetime guards. Candidate remains pending.

Visual helper reran the existing October9 raster executable successfully on October10: seven images/114688pixels, mip byte size16. This removes the assumption that the historical88760868 device-lost failure is still reproducible, but does not establish current-header rebuild parity or a game/frontend launch. Fresh evidence packet is being prepared in the helper's work-only scope. No plugin install/config change: UltraCodex was found installed as a CLI outside the plugin cache; built-in helpers are currently executing the authorized parallel work.

Fresh current-header raster gate subsequently **PASS**, exit0:17mandatory productionproviders and all omission/parity checks,seven realGPUframes/114688pixels,actual2x2dimensions and16bytelevel accounting,five textured paths byte-identical. Standard generated `native-indexed-raster.json` updated. Historical88760868 is D3DERR_DEVICELOST; historical cause remains unknown,not currently reproducible. Visual scope remains a bounded harness,not actual bank-loaded content/frontend/game boot.

Nested loader genuine firstcandidate written in work-only scope,compiled1117B/388instructions vscomplete1265B/438,80.1453percent similarity/instructiondelta11.4155percent: **first parity failure**, not accepted. Independent source audit finds no definite semantic mismatch; independent model self-check97920cases passes but is explicitly not native/compiled behavior evidence. Helpers are implementing executable original/model/compiled fixture and reviewing emitted-helper/inline context before a secondattempt. Candidatecalls out-of-line TouchResource/async ReleaseData/RemoveResource where original inline/context differs; do not substitute compatibility bodies or bypass the two-attempt cap. No native coverage counts change.

## Second loader attempt and retail-content milestone

Candidate1 scratch differential gate passes3312cases/59748full64KiB snapshots across ordinaryfirst/second/all-fail allocation,reuse,abort/wait,volume forcedformat andunsigned readspan partitions plusfocusedtouch cases. Actual emitted TouchResource and asyncReleaseData COMDATs execute,storeddeletion and callbackmutations still excluded bybaselinecount5. No realIO/GPU/heap/sourcecompleteclaim.

Independent COFF audit establishes genuine originalinline context forTouchResource/asyncReleaseData andstandalone16BRemoveFromResourceList. Scoped canonicalresource macros added,defaultbehaviorunchanged; existingremoval6144/12288 andframeloader69120/390912regressions pass. Candidate2 uses scopedcontext andoriginalexplicitpixel-formatconstructor without unsupporteddefaultwrite. It compiles1249B/434instructions versus1265B/438,**88.9908percent normalizedsimilarity,secondparityFAIL**. Parent9FDE80 is nowDEFERRED, no thirdattemptwithoutneworiginalevidence; no weakeningthreshold or recoveryclaim. Functional secondfixture pending.

Original TextureLoadingPriority01396C64 global independently verified againstDIA andretail,now permanentgenuine `long=3` 4Bstorage plus4state signed/writablefixture andmandatoryexactmissing-symbolomission. Metadataartifact/manualphysicalindex/gate_all integrated; no fictitiousfunctionmanifestrow. Cleanclosure `native-loader-priority-clean-20261010` **1792sources/442unresolved/zero duplicates**. Nativefunctioncountsunchanged127exact/37near; data6continuation/43cumulative (381exact/71near). Generateddashboardsnotmanuallyrewritten.

Permanent realretailasset gate **PASS twice**, includesfreshcanonical17providercompile/parity/omission/realGPU controls eachrun. Originalfrontend.big titleentryid3/FRONTEND_TITLE_01_SPRITE loaded at runtime through existingbankreader/boundedCdecoder. Recoveredrenderer draws256x128spriteinto288x160target:all46080GPU RGBpixels compareexactly with independentPythondecoder/reference,32768asset+13312background. All17productionproviders plusdecoder omissionchecks require exactdecoratedLNK2001/2019symbols. Source/object/canonicalreport hashes recorded. Files `NativeRetailTitleRaster_test.cpp`, `gate_native_retail_title_raster.py`, report `native-retail-title-raster.json`; previewunderworknative-retail-title-raster-20261010. Optionalgate_all --gpu integrated; fullgate_all notrun. Container/decompression/colorconversion/OS/window/device/upload/geometry/readback/clock remainexplicitintegrationboundaries. This is realcontent rendered,NOT recovered9FED30bankloading,originalfrontendboot,presentationloop orplayability. No proprietaryassetbytescommitted.

Nextmissingnativepath includesLoadGraphicFrame9FED30,surface copy/sectionhelpers andstream/LZOboundaries. Later evidence below supersedes the provisional const-async identity; originalsource-paritygoalunchanged,no Lua/UE/newcross-gameimplementation scope.

## Public status publication and native dependency audit

User approved prepare/push and wanted public activity visible. Published five
documentation files to main in commit772d88d753311d7fb0638fd4133c19c765c10e16
from an isolated main-based worktree. No development-branch Lua history,
proprietary assets or scratch outputs included. Public README now distinguishes
authored frontend integration from native-engine boot/playability, labels older
September generated totals, and describes the newer local native checkpoint.
Metrics consistency and311file-link targets pass; remote main SHA verified.
This is status publication only, not a source-checkpoint merge or full CI/gate run.

Loader candidate2 bounded fixture completed:3568cases/67044full64KiB snapshots,
including stored-deleter lifetimes and live-pointer mutation. Still DEFERRED at
88.9908percent parity after two attempts; behavior PASS does not override parity.

Async9D5580 is an original normalized clone of recovered nonconst9D5530,
not an established const overload. Fresh canonical COFF matches both77B bodies
and all three dependency helpers. Root reran actual async/pool composition:
307200cases/1843200full16KiB snapshots PASS,four mandatory source omissions,
current-header hashes refreshed. No new provider or recovery credit.

SurfaceLock9F33E0 already had two October8 failures; the October10 packet only
corroborates old behavior and does not reopen it. Existing CopyFromSurface9F37E0
source freshly compiles445B/144ins against complete446B/146ins,96.5517percent
normalized similarity. Old fixture substitutes Lock/conversion; no new acceptance
or count. Complete RET10 at9F399B and padding through9F399F audited.

Bank-loader evidence identifies a clean nondeferred dependency:
CGraphicFrame::InitialiseAsSurfaceSection9FCB60,true seven-argument donor ABI.
Added declarations to canonical frame/data headers; scratch genuine C++ first
candidate allocates original27B data and forwards every argument unchanged.
Compiler output MATCH57B/21ins/three audited relocations. No parity retuning.
Behavior fixture pending; allocator and unresolved data initializer9FCA40 remain
explicit boundaries,so no section/UV/bank-loading or recovery-count claim yet.
Data initializer's current-mode x87 FISTP rounding is not a truncating C++ cast;
do not introduce invented conversion semantics or asm baking to replace it.

After adding the two section-method declarations, existing ordinary frame
initializer regression PASS36864cases/110592snapshots and data initializer
PASS18432cases/47104snapshots. Independent section-fixture source review agrees
with original null/fresh/owner-alias and rawfloat forwarding semantics; require
COFF-addend audit plus mandatory root-source omission before promotion. The
runtime section fixture is still in progress; no acceptance/count change yet.

## Ownership initializer first candidate

Original CGraphicInfoFrame::InitialiseFrame9FE550 donor ABI verified private,
mutable CGraphicFrame&/CManagedTexture& and signedchar mip; plainchar/public
assumptions would change its mangling. Canonical private declaration added,
no inferred friend declaration. Genuine first source candidate uses existing
intrusive ownership and frame assignment helpers without inlining out-of-line
managed assignment/release. Complete208B/76instructions/sevenrelocations resolves
exactly to original targets. No assembly baking, null-safe shortcut or tuning.

Independent original/model/compiled fixture pending. It must distinguish the
cached graphicFrame slot from live managed/final-mip owner reloads and read
incomingFrameData after old cleanup callbacks. Null allocation retains original
fault behavior rather than invented success. No acceptance/recovery count yet.
