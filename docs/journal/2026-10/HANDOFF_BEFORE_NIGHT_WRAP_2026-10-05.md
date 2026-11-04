> Historical handoff: instructions and metrics below belong to their recorded checkpoint. Use [the current handoff](../../HANDOFF.md) before acting on them.

Native queue growth continuation 2026-10-05: visual checkpoint NOT reached.
Latest clean native rebuild:1329 sources compile, zero duplicates,414 unresolved.
work/codex-native-startup/native-gfmain-text-growth-closure-20261005
Native queue overflow009E1750, range copy009E11A0 and clear009E1650 now exact
C++; native-add-text gate852 cases/3444 full24KiB snapshots, seven omissions.
All emitted replay allocator-substituted code/relocations checked unchanged.
Actual insertion/copy/cleanup runs; heap and COM boundaries are recorded.
Next: full ClearRenderTarget009D8CF0 / DrawBufferedPolys009D9C80 and renderer
ctor009D9A30. Read-only Ghidra export completed successfully to
work/parity_addtext/renderer_next_decomp.c (log renderer_decomp.log; exact spans in renderer_next_spans.json).
Remaining native dependencies below; no recreation frame displayed.

Native text queue continuation 2026-10-05: visual checkpoint still NOT reached.
Latest clean native rebuild:1,328 sources compile/zero duplicates/415 unresolved:
work/codex-native-startup/native-gfmain-text-queue-clean-closure-20261005
Accepted native C++ now includes CIMEFont::DrawTextW00405020, actual-to-virtual
coordinates009E21D0, Draw3DScaledText009DBFF0, AddText009DB700, virtual-to-actual
scalars009E1E10/40, GetCurrentAWindow00A0AAA0, queue-record copy009DE890 and
texture copy009F9D00. PDB reference signatures and text/normal record union fixed.
Gates: font pipeline2208 cases/29920 snapshots; coordinates/boxes832/3456;
AddText and copy ownership672/2016. Original frame512, lifecycle180 and visible
area1024 regressions pass. No rasterization: queue growth and downstream render
execution remain missing. Next: actual vector<CQuickDrawTriInfo> overflow009E1750,
then remaining render queue/renderer startup plus IME UI/cursor providers.
No authored frontend, recorded-call test or compilation counts as a native frame.

Native IME continuation 2026-10-05: visual checkpoint still NOT reached.
Latest clean rebuild:1,320 sources compile/zero duplicates/415 unresolved:
work/codex-native-startup/native-gfmain-ime-font-clean-closure-20261005
New accepted C++: IME font height/position/colour, CRT allocation callbacks,
ResetToDefaultSettings, Reset, and Render. Support704 cases/1024 snapshots plus
256 actual allocations; Render192 cases/480 snapshots; font colour2304 cases.
Fixed two native integration bugs: all IME settings now share the actual84-byte
singleton, and EnableIME/CancelOnOKClick use the retail boolean transfers.
Loader76 checks pass; all84 relocations audited and100% instruction similarity.
Initialise,512-case drawing,colour transfer and boolean-parser regressions pass.
Font selection00404970 and measurement00404F60 now pass1248 native/retail
cases/10272 snapshots; see native-ime-font-measure gate.
Next: remaining IME text drawing00405020,
ImeUi_RenderUI00CB38E0 and UI CSprite::DrawWithRenderManager0041A980; then
remaining viewport/render queue and engine startup dependencies below.
No native frame displayed. No authored frontend or recorded-call test counts
as the user's recreation visual checkpoint.

Native continuation 2026-10-05: visual checkpoint NOT reached.
Accepted genuine C++ now includes installer workers/singleton lifetime, native
Draw coordinator0042DF9E, display/core BeginRender/EndRender, GFVBoxToABox,
SetVWindow, visible virtual bounds009BECE0, and UI screen Draw00595222.
Latest native closure: 1,311 sources, zero duplicates, 422 unresolved symbols.
Clean rebuild: all 1,311 sources compile; native link still fails on 422 symbols.
work/codex-native-startup/native-gfmain-native-draw-clean-closure-20261005
Permanent gates: native_frontend_draw (512 cases), native_render_frame (180),
native_virtual_box (448/2304 snapshots), native_visible_area (1024),
native_ui_screen_draw (648/1554 snapshots), plus installer gates below.
Shared C2DBoxF constructors and STLport begin() added; renderer gates rechecked.
Native integer/float viewport candidates remain REJECTED in work/; original
assembly endpoints are not accepted recreation. /QIfist emits qword conversion,
which also differs at invalid/out-of-range values from retail dword FISTP.
Next: remaining real viewport/render queue and CDisplayEngine startup/drawing
providers; no authored frontend, mocked frame, or retail launch is a checkpoint.
See latest native journal section. User requested continued work to a real frame.

Native recreation visual checkpoint requested (2026-10-05): NOT reached.
The authored visual frontend is not this checkpoint. Continue native startup,
resource and renderer reconstruction until the recreation itself produces a
verified visible frame. Latest native closure has 1,298 sources and 423 unresolved
symbols. Previous instruction below to stop at authored visual review is superseded.

Native installer production lifetime checkpoint (2026-10-05): shared polymorphic
base/header and constructor/destructor providers integrated. Permanent gates pass
24 constructor, five failure, 72 destructor replay and 96 real-resource lifetime
cases; deleting destructors retain exact parity. Counted-pointer regressions pass.
Native closure: 1,298 sources, zero duplicates, 423 unresolved (new constructor
dependencies exposed). Next: allocation/globals/singleton release and worker
providers; retire old 009C7900 landscape metadata. Full visual bootstrap now passes after shared-header drift repairs; 123 leaf
fixtures and 182 fixture/integration sources passed their preflights. Stop at the
visual-review checkpoint: close running retail Fable before launching
rebuild/build/bootstrap-Release/FableTLC-Reconstruction-VisualCheckpoint.exe
with --buff-jesus --skip-boot-videos. Live capture was blocked by retail's
single-instance guard; no screenshot or visual parity pass is claimed. See the latest section
of docs/journal/2026-10/NATIVE_RECONSTRUCTION_2026-10-05.md for scope and evidence.

Native installer actualconstructor/destructor lifetime composition PASS (2026-10-05): replay_installer_destructor_containers.py --constructor/FileInstallerLifetime_test.cpp PASS96lifetimes/96fixturethreads/384events/tenrequired-provider omissions. Actualscratchpolymorphicctor creates fiveCRTsentinels, realCS/fourautoresetevents and .biz->.big alias throughrecoveredmapinsertion; compilerderivedvtable. Fixture thenpopulates existinglist/setowners withsharedfilecontrol, completedstring, active uninitialized/deflate/inflatestream andnestedtransfers, actual mainallocator4096/65536 buffers/nullmiddlechunk andGlobalAlloc read/writehandles. ActualfullC++destructor releases containers/strings/streams/buffers/OSresources. Constructorcreatedaliasstrings returnbaseline, trackedstreamallocationszero, fixturefileobjectdestroyedonce, allocatoractivecountzero andall384eventsinvalid. 192constructor threadlaunch callsrecorded,96fixturethreads performevent/CSshutdown; recoveredgameworkerloops deferred. Compressorbasecompletion andproductionCFileInfo destructorbody remainfixtureboundaries; originalallocatorretainedchunksfixture-releasedaftershutdown. Report installer-lifetime.json savedwithproviderhashes. Previously24success/fivefailureconstructorfullstatecases and72destructorfullstatecases remainverified. Next productionsharedsingletonspecialization/installerdeclarations and permanentgates/registration/closure, preservingexistingcountedpointer andcleanupbehavior. Productionclosure1296sources/zero duplicates/419unresolved,index33574; Lua/FMP excluded; ownjobs terminal; native recreation active/incomplete.

Native polymorphic installer constructor failure parity PASS (2026-10-05): check_installer_polymorphic_constructor.py --failure compiles compiler-vtable ctoragainst scratchpolymorphicbase andruns five nullsentinelallocation points. PASS5nativeSEHwritefault-at-zero cases/one required-provider omission; complete32KiBpartialstates, callcounts andrecordedboundaries matchindependentretailfailureoracle. Constructorstill397vs389bytes/18relocations/90.441%normalizedsimilarity; no addedfailurecleanup invented. Successful24case constructorreplay and72native/96fullcleanupdestructorcases remainpreviouslyverified withsameinlinebase. Report polymorphic-constructor-failure.json saved. Next realcombinedconstructor/destructorlifetime, replacingfixturemanualsentinel/eventsetup andcleanup; thenproductionsharedbase/declarations andpermanentgates/registry/closure. Productionclosure1296sources/zero duplicates/419unresolved,index33574; Lua/FMP excluded; ownjobs terminal; native recreation active/incomplete.

Native installer polymorphic scratchconstructor PASS (2026-10-05): check_installer_polymorphic_constructor.py compilesexistingctorbody withscratch CFileInstaller inheritance/virtualdestructor andinlineempty CTBaseSingleton constructor, removing manualvtable extern/store. Compiler-generatedderivedvtable relocation substitutesactual129B43C; constructorretainsaccepted397vs389bytes/18relocations/137vs135instructions/90.441%similarity. PASS24native full32KiBconstructor cases/one required-provider omission againstretailoracle; fixturedestructorrecorded, stableactualvtable canonicalized, identity corroborated byrelocationaudit. Scratchbaseheadernowinlinector; productionlayout untouched. Fullnative destructor replay recheckedPASS72cases andcompletebuffer/container/Windows composition recheckedPASS96scenarios/96fixturethreads/384events aftersameinlinebasechange. Fullbody410bytes/97.260%similarity anddeletingdestructor exact30bytes retained. Next integrateactualconstructor/destructor lifetime andnull-sentinel failure cases, thenproductionsharedbase/declarations andpermanentgates/registry/closure. Productionclosure1296sources/zero duplicates/419unresolved,index33574; Lua/FMP excluded; ownjobs terminal; native recreation active/incomplete.

Native fullinstaller destructor completecleanup composition PASS (2026-10-05): replay_installer_destructor_containers.py --buffers/FileInstallerDestructorBuffers_test.cpp PASS96scenarios/96fixturethreads/384events/ninerequired-provider omissions. ActualfullC++destructor, lock/Windows APIs, recoveredalias/completed/active/counted/string/zlib cleanup nowcomposed withactualClearBuffers/array/range andmain30-classallocatorproviders. Eachscenario owns4096/65536bytebuffers withnullmiddlechunk andactualGlobalAlloc read/writehandles. Afterdestructorarraypointersandglobalhandleszero, mainactiveallocationcount40Czero, stringbaseline restored, trackedstreamsallfreed, sharedfixtureobjectdestroyedonce, eventsinvalid. Fixtureallocator teardown capturesoriginalretainedchunks andexplicitlyreleasesaftershutdown, preservingoriginalproductionretention; HeapValidatepasses. Consistent out-of-linesupportmode usedforallcompositionproviders; missingfixedsizefixtureheader correctedthenPASS. Compressorbasecompletion remainsrecorded, rawstorage bypassesconstructor, fixturethreads replacedeferredgameworkerloops andfixtureCFileInfo destructor. Report destructor-buffers.json savedwithproviderhashes. Next sharedpolymorphicinstaller/base andscratchconstructorintegration, thenpermanentgates/registration/closure. Productionclosure1296sources/zero duplicates/419unresolved,index33574; Lua/FMP excluded; ownjobs terminal; native recreation active/incomplete.

Native fullinstaller destructor realcontainer/stream/Windows composition PASS (2026-10-05): FileInstallerDestructorContainers_test.cpp/replay_installer_destructor_containers.py PASS96scenarios/96fixturethreads/384events/sixrequired-provider omissions. Actual fullC++destructor, fourlockproviders andWin32shutdown plusrecoveredalias/completed/active/countedcleanup, actualCharStringownership, nativezlibinit/end andCRT node/sentinel/control/object frees. Eachscenario hasnonemptyalias/completed trees sharingretainedstring, oneactive node withnestedlists anduninitialized/deflate/inflatestream, oneentryineachcountedlist sharingthree-ownerfixturefilecontrol. Afterdestructionstringsreturnretainedbaseline, alltrackedstreamallocationszero andfixturefileobjectdestroyedexactlyonce afterstreams; all eventsinvalid andthreadshandlesjoined/closed. ClearBuffers/range remainrecorded(emptybuffers); compressorbasecompletionrecorded via test-onlyredirect, rawstorage bypassesconstructor andgameworkerloops stilldeferred. FixturegeneratedfromexistingOS/lifetimefixtures; sourcehashes recorded. Report destructor-containers.json saved. Next actualbuffer/allocatorcomposition andconstructor/baseintegration beforeproductionpromotion. Productionclosure1296sources/zero duplicates/419unresolved,index33574; Lua/FMP excluded; ownjobs terminal; native recreation active/incomplete.

Native installer destructor realWindows synchronization PASS (2026-10-05): FileInstallerDestructorOS_test.cpp/replay_installer_destructor_os.py PASS96scenarios/96fixturethreads/384eventhandles/fiverequired-provider omissions. 32cycles withzero/one/twoworkers unfinished; actual nativefullC++destructor andfour recovered CCriticalSectionLock providers, realWin32CS/autoresetevents/SetEvent/Sleep/CloseHandle/DeleteCriticalSection; fixturethreads wake onevent andsetcompletionflags whileholdinginstallerCS. Destructor mustrelease/yield/relock topermitcompletion; ClearBuffers fixtureverifiesrecursivelock counts1->2->1. Actual malloc/free fivecontainer sentinels; all384eventhandles verifiedinvalid afterreturn, threadsjoined/handlesclosed. ClearBuffers/container/range bodies remainrecorded; rawstorage bypassesconstructor, recoveredgameworkerloops notexecuted. VCtoolchainhasno windows.h inconfiguredinclude; fixture usesexact32bit importprototypes andknownCSrecursionoffset8 likeexistingOSfixtures. No productionobject redirects; fullbody410bytes/97.260%similarity retained. Report destructor-os.json saved. Next realcontainer/allocator ownershipcomposition andscratchconstructor/base integration beforeproductionpromotion. Productionclosure1296sources/zero duplicates/419unresolved,index33574; Lua/FMP excluded; ownjobs terminal; native recreation active/incomplete.

Native fullinstaller destructor native fullstate replay PASS (2026-10-05): FileInstallerDestructor_test.cpp/replay_installer_destructor_native.py compiles actual C++ candidate withscratchpolymorphicinstaller/base declaration; PASS72cases/one required-provider omission, each boundary and finalcomplete32KiBregion compared against independent retail-validated model. Samewait0/1/3,workerinitialflags,treecounts andhandle/head/buffer callbackmutations asretail72cases/1824snapshots. Compilerderived/basevtableidentity normalized fromactualconstructedbase andfirstdestructorboundary; no manualproductionvtablestores. OSimport/free callsites redirected only intestobject; lock/ClearBuffers/containers/range recorded andmemoryretained, no actualWindows/heap/worker claim. Fullbody retains410bytes/23relocations/146instructions/97.260%similarity NEAR; deletingdestructor exact30bytes. Fixture placementnew required<new>; correctedmissinginclude thenPASS. Next actualWindows/ownershipcomposition, scratchconstructorintegration, thenproductionsharedinheritance/gates/registration. Productionclosure1296sources/zero duplicates/419unresolved,index33574. Lua/FMP excluded; ownjobs terminal; native recreation active/incomplete.

Native fullinstaller destructor independent retail replay PASS (2026-10-05): oracle_file_installer_destructor.py executesactual009C5170 against independent fullregion/event model:72cases/1824complete32KiBsnapshots. Waititerations0/1/3,eitherworker initiallyfinished, alias/completed empty/nonempty, callbacksnormal/replaceeventhandles/replacetreeheadandbufferstorage. Checks release-Sleep(0)-relock loop, exitflagbeforeevents, freshsecondsignal/closehandles, finaltreeheadreads/resets/frees, active/counted order, freshbufferfirstafterrange and finalbasevtable129B414 plusstackbalance. Lock/OS/container/range/release boundaries recorded and heapretained; noactualworker/lock/heapcomposition claim. Candidate remainsaccepted410bytes/146instructions/97.260%similarity; generateddeletingdestructor exact30bytes. Console regression originalsession30805 completedPASS compiled16384/comparative16384, validating unchangedgeneric singletonheader extraction. Next native fullstate replay ofdestructorcandidate using sameboundarycontract, thenrealWindows/ownership integration andproductioninheritance/constructor recovery. Production closure remains1296sources/zero duplicates/419unresolved,index33574. Lua/FMP excluded; ownjobs terminal; native recreation active/incomplete.

Native fullinstaller destructor C++ candidate accepted-shape checkpoint (2026-10-05): FileInstallerDestructor_candidate.cpp/check_installer_destructor.py compilesagainst isolated scratchinclude withCFileInstaller inheriting recoveredpolymorphic CTBaseSingleton specialization. ShippingA0size/startupflag81 preserved usingunknown04[7D]; productiondeclaration unchanged. Full009C5170 body410bytes/23relocations/146instructions, zeroinstructiondelta,97.260%normalizedsimilarity NEAR_MATCH; stopbyte tuning. C++ lockscope, ClearBuffers, exitflag, cachedSetEvent/Sleep/CloseHandle, release/yield/relock wait, deleteCS, alias/completed/active/counted cleanup andbuffer-range/free composed; compiler emits derived/basevtablestores. Generatedscalar deletingdestructor009C7900 audited overfull30bytes (initial28 truncatedret4); saved deleting-destructor-parity.json. No fullinstallerbehavior orrealworker shutdownclaim; next retail/native fullstate/callback replay andscratchconstructor integration beforeproductionpromotion. Console regression originalsession30805 stilllive whenpolled; noPASS/restartclaim. Production closure1296sources/zero duplicates/419unresolved,index33574. Lua/FMP excluded; native recreation active/incomplete.

Native installer singleton base behavioral checkpoint (2026-10-05): Isolated specialization moved to FileInstallerSingletonBase_candidate.h forconsistent candidate/fixture use. check_installer_singleton_base.py retains exact31bytes/tworelocations at009C5B60 andpasses128nativecases/one required-provider omission. 32cycles/flags0..3, actualbaseconstructor installscompiler-generatedvtable, scalar deletingdestructor invokedthroughslot0, returnidentity andconditionaloperator-delete argument/vtable checked; test-onlyCOFFdelete redirect recordsrelease, fixtureactualfree afterward. VC7.1 rejectsfreefunctionpointer __thiscall; testfastcall adapter passesECXthis, unusedEDX andstackflags matching actualret4 ABI. Productioninstaller inheritance remainsunmodified while fullconstructor/destructorintegration pending. Console regression gate_console_toggle_event.py originalsession30805 remainslive whenpolled; noPASS claimed orrestartperformed. Standaloneinstallerconstructor previouslyPASS24cases. Production closure remains1296sources/zero duplicates/419unresolved,index33574; Lua/FMP excluded; native recreation active/incomplete.

Native installer singleton base C++ recovery checkpoint (2026-10-05): fable_singleton.h nowowns unchangedgeneric CTBaseSingleton declaration; fable_console.h includes it, avoidingconsolecoupling forinstaller specialization. Isolated FileInstallerSingletonBase_candidate.cpp specializesCTBaseSingleton<CFileInstaller> withvirtualemptydestructor. Compiler-emittedscalar deletingdestructor009C5B60 is full31bytes/tworelocations/11instructions resolved-byteMATCH, includingbasevtable129B414 andconditionalCRTdelete; noasm/literalvtablestores. Initialauditlength29 truncatedret4; correctedretailend009C5B7F gives31. Specialization remainsisolatedscratch pending actualCFileInstaller inheritance and sharedvirtualdestructor integration; noODRmix introduced intoproductioninstaller. Unchangedinstallerconstructorgate PASS24native fullregion cases afterheaderextraction. Next integratepolymorphicinstaller/base and updateconstructor/vtable fixture boundaries, thenfullnative destructor. Production closure remains1296sources/zero duplicates/419unresolved,index33574; Lua/FMP excluded; native recreation active/incomplete.

Native installer full-destructor base/dependency evidence checkpoint (2026-10-05): trace_installer_base_rtti.py reads actual retail PE RTTI for vtables129B414/129B43C; base-rtti-evidence.json identifies CTBaseSingleton<CFileInstaller> as sole directbase atoffsetzero, onevirtualdestructor slot009C5B60. Derived scalar deletingdestructor009C7900 callsfull009C5170 thenconditionalCRTdelete; base slot writes129B414 thenconditionaldelete. Existing shared genericCTBaseSingleton in fable_console.h is nonpolymorphic while CFileInstaller header isopaque/nonvirtual; must reconcile specialization/base ABI beforegenuineC++ destructor composition, avoidingmanualvtable/addressstores. Alias map usesexisting CBasicRetailBankAliasMap at94/size98/compare9C, compatibleowner12 andnode24. gate_bank_manager_containers.py PASS aliasErase70/70,otherErase59/59,clear83/89 andthreeprovideromissions; noaliasproviderrewrite needed. Donor confirmsbasename but itsC4 layout differs fromshippingA0, so shippingoffsets remainauthoritative. Next sharedsingleton/base declaration andinstaller destructor recovery withlock/event wait/container order. Production closure remains1296sources/zero duplicates/419unresolved,index33574. Lua/FMP excluded; native recreation active/incomplete.

Native installer active-file list landed (2026-10-05): InstallerActiveFileList_Clear_009c71a0.cpp registered manifest/artifacts/corrections/symbol_index; old global DataItemList label corrected from retail installer destructor evidence. Three permanentgates registered gate_all: gate_installer_active_file_list.py exact264bytes/7relocations/93instructions and1080native callback cases; oracle_installer_active_file_list.py135distinctretail cases/1071full32KiBsnapshots; gate_installer_active_file_lifetime.py144actualzlib/CRT/sharedownership scenarios/288nodes/144fixtureobjects/two omissions. Base destruction recorded, constructor bypassed and productionCFileInfo body deferred; nofullgate_all claim. Symbolindex33574definitions; freshclosure native-gfmain-installer-active-list-closure-20261005:1296sources/zero duplicates/419unresolved. Closure rechecked aftersymbolindex refreshcompleted. Next fullinstallerdestructor009C5170, base/vtable/deletingdestructor and worker/constructor integration. Lua/FMP excluded; jobs terminal; native recreation active/incomplete.

Native installer active-file list real stream/CRT/shared ownership composition PASS (2026-10-05): InstallerActiveFileLifetime_test.cpp/replay_installer_compressor_lifetime.py --active-list pass144scenarios/288nodes/144fixtureobjects/two required-provider omissions. 16cycles, nodecounts1..3, compressoruninitialized/deflate/inflate modes; nested transferlists0..2 entries. Actual recovered active-list andcompressorcleanup, native zlib1.2.1 streaminit/end, actual calloc/free streamcallbacks, unredirectedCRT malloc/free nested/outer nodes andnew/delete sharedcountedcontrols. Final sharedfixtureobject destroyedexactlyonce afterallstreamsreleased; all trackedstreamallocationszero; totalbasecompletion288; sentinelselflinks and repeatedemptyClear harmless. Base destructor remainsrecorded via test-onlyCOFFredirect; uncalledvirtualslotsabort; rawstorage bypasses fullconstructor; productionCFileInfo destructorbody remainsdeferred. Report active-file-list-lifetime.json includes sourcehashes. Standalone64streamcase replay retained. Next permanentgates/promotion/registries/closure, thenfullinstallerdtor/workerintegration. Production closure unchanged1295sources/zero duplicates/419unresolved,index33569. Lua/FMP excluded; jobs terminal; native recreation active/incomplete.

Native installer compressor real stream lifetime PASS (2026-10-05): InstallerCompressorLifetime_test.cpp/replay_installer_compressor_lifetime.py pass64cases,32cycles eachdeflate/inflate, one required-destructor-provider omission. Actual recovered CCompressorZlib destructor calls actual native zlib1.2.1 init/end providers; allocation callbacks performcalloc/free with liveallocation counts. Each initialized stream has nonzero liveallocations beforedestruction and zero afterward; streamstate becomesnull, initializedflagfalse, decompression resetsnextin/nextout/availin/availout. Raw alignedstorage bypasses compressorconstructor; base destructor redirected only in test object torecordcompletion; uncalled virtualslots abort. Destructor emits its ownvtable, so initial attemptedfixture-vtable substitution collided with actualdefinition; removedsubstitution and suppliedabort-only virtualslots. No productionobject patching, fullactive-node lifecycle or ownworkspace ownership coverage claimed. Report compressor-lifetime.json saved. Next composeactualcompressor streamcleanup with active-filelist and actualCRT ownership, thenpermanentgates/promotion/fullinstallerdtor. Production closure unchanged1295sources/zero duplicates/419unresolved,index33569. Lua/FMP excluded; jobs terminal; native recreation active/incomplete.

Native installer active-file list retail oracle and reference expansion PASS (2026-10-05): oracle_installer_active_file_list.py executes original009C71A0 versus independent event/fullregion model:135distinctcases/1071complete32KiBsnapshots. Outer/nested counts0..2, controlabsent/count0/1/2/ffffffff and threecompressorcallback modes(normal,clearnext,replacefilecontrol). Checks nestedrelease order and sentinelreset beforefree, cachedouternext, freshfilecontrol aftercompressorcallback, countwrapping, payloadzero and finalselflinks, exactstackbalance. Native fixture expanded to same135combinations repeatedeightcycles:1080cases/one required-provider omission PASS; surviving controls validated thenfixture-released. Shared C++ provider remains exact264bytes/sevenrelocations/93instructions. Retail fullregion comparison and native event checks have distinct scopes. Compressor/object destruction and retail releases recorded; no actual zlib/productionfileobject lifetime claim. Reports active-file-list-retail-oracle.json and active-file-list-behavior.json saved. Next actual compressor/zlib/CRT composition beforepermanentgate/promotion and fullinstallerdtor. Production closure unchanged1295sources/zero duplicates/419unresolved,index33569. Lua/FMP excluded; jobs terminal; native recreation active/incomplete.

Native installer active-file list shared layout and native behavior PASS (2026-10-05): fable_file_installer_active_files.h declares shipping owner4/NodeB8/file24/compressor40/input98/output9C with ABI assertions. Shared-header candidate retains exact264bytes/sevenrelocations/93instructions at009C71A0. InstallerActiveFileListClear_test.cpp passes216native cases: eightcycles, outercounts0..2, nestedcounts0..2, normal/callback-cleared-next/callback-replaced-file-reference modes. Checks output theninput releases, compressor thenfile/control/outer release, payloadzero beforeouterfree and finalsentinelselflinks. Nested/outer allocations useactualmalloc/free behindrecordinghooks; compressor and fileobject destruction are recorded boundaries, not actual lifetime coverage. Required-provider omission failslink. Fixture sentinel uses aligned rawstorage because constructing a fullNode accidentally introduces unrelated compressorctor/vtable/destructor dependencies; only sentinel links are accessed. Reports active-file-list-parity.json and active-file-list-behavior.json saved. Next independent retail/fullstate active-list oracle and actual compressor/zlib/CRT composition beforepromotion. Production closure unchanged1295sources/zero duplicates/419unresolved,index33569. Lua/FMP excluded; jobs terminal; native recreation active/incomplete.

Native installer compressor destructor independent retail oracle PASS (2026-10-05): oracle_installer_compressor_destructor.py executes retail00A3ACC0 in Unicorn versus independent model, PASS64cases/192complete32KiBsnapshots at stream-end/free/base-destructor and final state. Initializedfalse/true,compressfalse/true,workspacenull/present,fourcallback modes(normal,replaceworkspace,clearworkspace,mutatestream/flags), stream-end return0/ffffffff ignored. Model verifies originalcachedcompress branch, postcallbackdecompressionzeroes fourfields, initializedzero afterend, fresh workspace read aftercallback, original uninitialized branch skips mutations/end, vtable1245974, base tailcall and exactstackbalance. All bytes outside provenstores preserved in fullregion snapshots. Stream-end/CRTfree/base boundaries recorded, memory not actually reclaimed; no native callback fixture or actual zlib lifetime claim yet. Existing C++ destructor alreadyaudited full91byte/fiverelocation MATCH; active-list candidate exact264bytes remainsunchanged. Report compressor-destructor-retail.json saved. Next native callback replay/realzlib lifecycle and shared active-list layouts before composition/promotion. Clean production closure remains1295sources/zero duplicates/419unresolved,index33569; Lua/FMP excluded; own jobs terminal; native recreation active/incomplete.

Native installer compressor dependency audit checkpoint (2026-10-05): audit_installer_compressor_destructor.py verifies existing CCompressorZlib_CCompressorZlib_00a3acc0.cpp against full retail91bytes/fiverelocations: resolved-byte MATCH,36instructions, actual vtable1245974,z_deflateEnd00C00AE0,z_inflateEnd00C038D0,CRTfree00BFEA14,baseDestructor0099A2E0. Zlib imports are C linkage _z_deflateEnd/_z_inflateEnd; corrected initial audit decorated-target assumption, source unchanged. Retail cleanup resetscompressorvptr, ifinitialized uses compressflag tochooseend; decompression zeroes next_in,next_out,avail_in,avail_out aftercallback; initialised flagzero, fresh OwnWorkspace+10 pointer conditionallyfreed, tailbaseDestructor. No stream-end error handling invented. Existing shipping CCompressorZlib header58bytes/base1C/Stream1C/initialized54 agrees active-node compressor+40/input98/output9C layout. Code audit only: behavior callback mutation, uninitialized streams and genuine zlib stream ownership stillneedtests before full active-list composition. Active-list candidate remains exact264bytes/sevenrelocations; no furtherbyte tuning. Next shared active-list declarations plus recorded compressor lifecycle/native full-state tests and actual zlib/CRT composition. Current clean production closure remains1295sources/zero duplicates/419unresolved,index33569; Lua/FMP excluded; own jobs terminal; native recreation active/incomplete.

Native installer active-file list candidate exact checkpoint (2026-10-05): InstallerActiveFileListClear_candidate.cpp/check_installer_active_file_list.py MATCH264bytes/seven relocations/93instructions/100% at009C71A0. Shipping184byte node recovered from constructor sentinel size and destructor accesses:next/previous0/4, countedfilevalue24/control28,CCompressorZlib40(size58),transferinput98/output9C; unknown spans retained, no donor offsets imported. C++ destroys output theninput transferlists inline (free nodes,selflink sentinel,conditional sentinel free), direct qualified compressor destructor00A3ACC0, authoritative countedpointer destructor and outerfree; finalfreshhead linksreset. Default Node destructor path initially compiled deleting-helper call57bytes; explicit ordered member destruction reproduces original sequence. Unqualified virtual compressor destructor initially compiled indirectvcall and omitted expected relocation; qualified member destructor produces exactretaildirectcall. Accepted exactshape, stopbyte tuning. No active-list behavioral validation or actual compressor composition claimed yet. Existing compressor header shipping58bytes/Stream1C/ZlibInitialised54 agrees member+40 layout; body CCompressorZlib_CCompressorZlib_00a3acc0.cpp exists but must still audit shape/callback/free/base behavior and genuine zlib lifecycle. Next shared active-node/list declarations, independentretail/fullstate replay, native recorded boundary validation and real compressor/CRT composition. Clean production closure remains1295sources/zero duplicates/419unresolved,index33569; Lua/FMP excluded; own jobs terminal; native recreation active/incomplete.

Native installer counted-file list landed (2026-10-05): InstallerCountedFileList_Clear_009c6cc0.cpp registered manifest/artifacts/corrections/symbol_index; incorrect event remove_if label corrected from shipping destructor evidence. Permanent gate_installer_counted_file_list.py MATCH89bytes/two relocations and800native callback cases/oneomission; gate_installer_counted_list_lifetime.py PASS96unredirectedCRTshared scenarios/96fixture objects/oneomission; oracle_installer_counted_file_list.py PASS100distinct retail Unicorn/model cases/310full32KiBsnapshots, report nowcompile-gate. Three gates registered gate_all. Index33569definedsymbols; fresh closure native-gfmain-installer-counted-list-closure-20261005 1295sources/zero duplicates/419unresolved. No fullgate_all run claimed. Native snapshot-hook tests and retailfullregion tests have distinct scopes; actual CFileInfo destructor body remains unrecovered. Next active-file list009C71A0; member+40 destructor00A3ACC0 identified as CCompressorZlib, existing source engine/CCompressorZlib.h and CCompressorZlib_CCompressorZlib_00a3acc0.cpp must be ABI/behavior audited before composition. Then full installer destructor009C5170/vtable/deleting destructor and worker/constructor integration. Lua/FMP excluded; all own jobs terminal; native recreation active/incomplete.

Native installer counted-file list independent retail replay PASS (2026-10-05): oracle_installer_counted_file_list.py executes actual retail009C6CC0 instructions in Unicorn and independently predicts100distinct cases (nodecount0..3,reference absent/count0/1/2/ffffffff,five callback mutation modes). PASS310complete32KiBsnapshots at every objectdestroy/control-delete/node-free boundary and finalstate, exacteventargument/order, fastcall this/return/stack balance checked. Model independently verifies count wrapping, fresh reference aftercallback, cached next beforeobjectcallback, payloadzero aftercontrol-delete mutation, fresh head after node-free replacement and finalselflinks. Release boundaries recorded and memory retained for snapshots; no actual heap reclamation claim in oracle. Native800case mutation fixture covers same100distinct combinations repeated8cycles with actual malloc/free behindrecordinghooks, but does not compare full32KiBnative snapshots; avoid conflating scopes. Separate96unredirectedCRTshared-lifetime scenarios alreadypass. Report counted-list-retail-oracle.json saved; candidate remains exact89bytes/tworelocations, no furtherbyte tuning. Next permanent oracle/native/lifetime gates and promotion/registry/closure, then activefilelist/fullinstaller destructor. Production clean closure remains1294sources/zero duplicates/419unresolved,index33566; Lua/FMP excluded; own jobs terminal; native recreation active/incomplete.

Native installer counted-file list actual CRT lifetime PASS (2026-10-05): InstallerCountedFileListLifetime_test.cpp/replay_installer_counted_list_lifetime_scratch.py PASS96cases/96fixtureobjects/one required-provider omission. 32cycles/shared control counts1/2/3 across separate lists; actual unredirected CRT malloc/free listnodes, new/delete CCPPointerInfo and fixtureobjects, genuine recovered list Clear and shared CCountedPointer destructor. Each intermediate clear decrements live block and preserves object, last owner destroys exactlyonce and releasesblock, sentinel links self-reset, repeated empty clear has no additional destruction. No production CFileInfo destructor claim; fixture object callback records identity and deletes its own object. Report counted-list-lifetime.json includes sourcehashes and exact boundary. Previous exact89byte/two-relocation provider and800native mutation cases remain valid. Next independent retail machine-code replay/model, then permanent gate/promotion/registry; active-file cleanup/full installer destructor stillpending. Clean production closure remains1294sources/zero duplicates/419unresolved,index33566; Lua/FMP excluded; own jobs terminal; native recreation active/incomplete.

Native installer counted-list shared ABI/callback expansion PASS (2026-10-05): fable_file_installer_containers.h now declares InstallerCountedFileList owner4bytes/Node16bytes/value+8 with assertions, authoritative CCountedPointer<const CFileInstaller::CFileInfo> and CCPPointerInfo. Scratch candidate/fixture use shared header. Exact89bytes/two relocations retained; check_installer_counted_file_list.py PASS800native cases/one omission. Five mutation modes:normal, objectcallback replacesreference, objectcallback nulls node next (cached traversal), info-delete callback replaces payload before subsequent zero stores, node-free callback replaces list head and future tail link while traversal continues. Eight cycles/nodecount0..3/nullreference/count0/1/2/ffffffff preserve countwrap, fresh reference delete, clearedData/Info beforefree, final self-links use fresh current head. Actual node/control storage allocated/freed, release boundaries test-only redirected; object bodies recorded. Completed-set shared-header regression permanent gates PASSexact59/64callback and96actualstring/11provider omission cases. Next independent retail replay/model and actual unredirected CRT list lifetimes, then promotion/registration; activefilelist009C71A0/full installer destructor stillpending. Current clean production closure remains1294sources/zero duplicates/419unresolved,index33566; Lua/FMP excluded; own jobs terminal; native recreation active/incomplete.

Native installer counted-file list scratch PASS (2026-10-05): InstallerCountedFileListClear_candidate.cpp/check_installer_counted_file_list.py MATCH89bytes/two relocations/42instructions/100% at009C6CC0. Uses authoritative CCountedPointer<const CFileInstaller::CFileInfo> destructor and CCPPointerInfo; added nested CFileInfo forward declaration in installer header, no object layout change. InstallerCountedFileListClear_test.cpp PASS320native cases/one required-provider omission:nodecount0..3, nullreference/count0/1/2/ffffffff, normal or destructor callback replacesreference, eight cycles. Real malloc node/control blocks; test-only COFF release-symbol redirects snapshot current node/ref then actual free. Callback count0-before-destroy verified, fresh replacement control block passed to delete callback, cached next traversal preserved, node Data/Info zero before free, retained control countwrap0->ffffffff andffffffff->fffffffe, count2->1, final sentinel next/previous self-links. Old control block when callback replacesreference deliberately fixture-cleaned; real CFileInfo destructor not claimed. Source still scratch class/layout; next shared list header, independent retail emulator/model with broader head/next/release callback mutations and actual CRT lifetime composition before promotion. Current clean production closure remains1294sources/zero duplicates/419unresolved,index33566; Lua/FMP excluded; own jobs terminal; native recreation active/incomplete.

Native installer completed-file set eraser landed (2026-10-05): InstallerCompletedFileSet_Erase_004aba70.cpp registered in manifest/artifacts/corrections/symbol_index; old LTextTreeWalkThrough label corrected from installer destructor call and CharString+16 node evidence. Authoritative shipping header fable_file_installer_containers.h Node20/value+16/setowner12. Permanent gate_installer_completed_set_erase.py MATCH59bytes/three relocations and64native callback cases/one omission; gate_installer_completed_set_strings.py PASS96actual ownership cases/11providers/11omissions; both registered gate_all. Real string constructors/copy/default/conversion/destructor/unassign/BasicString allocation, independent/shared/empty keys and fourtree shapes return instancecount baseline; cachedleft survives recorded destructor mutation in separate64case gate. Index refreshed33566symbols. Fresh authoritative closure native-gfmain-installer-completed-set-closure-20261005:1294sources/zero duplicates/419unresolved. Next counted-file list009C6CC0:reuse authoritative CCountedPointer/CCPPointerInfo, preserve fresh reference after callback and zero payload beforefree; then active-list009C71A0/member00A3ACC0 and full installer destructor009C5170. Lua/FMP excluded; own jobs terminal; full native recreation active/incomplete.

Native installer completed-set actual strings checkpoint (2026-10-05): InstallerCompletedSetStrings_test.cpp/replay_installer_completed_set_strings_scratch.py PASS96cases/11actualproviders/11required-provider omissions. Empty/leftchain/rightchain/balanced seven-node trees, eight cycles, three key ownership modes(independent literal,shared copy,empty). Actual production CharString constructors/copy/default/conversion/destructor/UnassignString/AllocStringData and BasicString allocation run; actual CRT calloc/free nodes and new/delete string data/buffers. Instance counts return baseline after every tree/shared-owner scope, shared owners7+1 ->1, retained owner text intact, empty fallback is fixture data only. New authoritative fable_file_installer_containers.h defines shipping completed set Node20bytes/value+16 and owner12bytes/head,size,unknown08 with size/offset assertions. Scratch eraser and both fixtures now include shared declaration, no duplicated class ABI. Exact59byte/three-relocation MATCH and64recorded-destructor callback cases rerun PASS after header migration; actualstrings96 rerun PASS. Initial migration script accidentally stopped at nested struct terminator; corrected actual files and script before checks, no unresolved compile errors. Next promote/register completed set eraser with permanent gates, then counted/active file-list cleanup and complete installer destructor. Production clean closure remains1293sources/zero duplicates/419unresolved,index33564; Lua/FMP excluded; own jobs terminal; native recreation active/incomplete.

Native installer completed-file set eraser scratch PASS (2026-10-05): InstallerCompletedSetErase_candidate.cpp/check_installer_completed_set_erase.py MATCH59bytes/three relocations/25instructions/100% similarity at004ABA70. InstallerCompletedSetErase_test.cpp PASS64native cases/one required-provider omission:empty/allleft/allright/balanced seven-node shapes, eight cycles,two destructor modes(normal or callbackclearleft). Independent right-root-left visit ordering, cachedleft survives destructor mutation; actual calloc/free node storage, CharString destructor recorded, no real string lifetime composition claimed yet. Node shape is shipping20bytes:color,parent,left,right,CCharString at+16; scratch class still to move into shared header and real string composition. Retail destructor-container evidence/disassembly saved by trace_installer_destructor_containers.py: 009C6CC0 clears two-counted-file-pointer lists with fresh reference after destroy callback, zeros payload words beforefree, resets sentinellinks; 009C71A0 clears active files including two nestedlists, member destructor00A3ACC0, counted reference and node cleanup. 004ABA70 old LTextTreeWalkThrough label contradicted by actual CharString node cleanup and installer call; 009C6CC0 old event remove_if label similarly wrong. Destructor imports confirmed SetEvent143FDE0,Sleep143FE1C,CloseHandle143FE30,DeleteCriticalSection143FDE4. Next real string/tree composition and counted/active list recovery before full destructor reconstruction. Production clean closure remains1293sources/zero duplicates/419unresolved, index33564; Lua/FMP excluded; own jobs terminal; native recreation active/incomplete.

Native installer cleanup landed (2026-10-05): seven addresses registered across three provider sources FileInstallerClearBuffers_009c5110.cpp(MATCH85), FileInstallerBufferArray_009c7150.cpp(NEAR76/85bytes93.75/93.939%), FileInstallerDestroyBufferRange_009c6d60.cpp(NEAR119/125bytes95.349%, with exact15/12/12 allocator support methods). Allocator support uses inline noinline C++ COMDAT definitions in calling range unit, coalescing with aligned allocator unit while preserving range out-of-line calls. Standalone strong support source retired from registries/index/seed, file retained. Three prior duplicate definitions resolved without asm, object patches in production, new static data, weakened parity or changed aligned bodies. Index33564definedsymbols. Permanent gates PASS128array/oneomission,320callback-model/oneomission with genuine support code and recorded Windows imports, exact sharedClear/support body checks,70provider/70omission/four actual Windows completeClearBuffers/array/range/deferred-shutdown cycles/one original retainedchunk. Aligned regression gate MATCH187bytes/PASS936independent retail/native states/oneomission. Fresh authoritative closure native-gfmain-installer-cleanup-coalesced-closure-20261005:1293sources/ZEROduplicates/419unresolved. Manifest/artifacts/corrections/symbol-index/gate_all updated, incorrect patch/flash labels corrected. Next installer destructor009C5170 and its set/list/basevtable dependencies, worker member loops and constructor promotion; full native game remains incomplete. Lua/FMP excluded; all own jobs terminal; native recreation active.

Native installer cleanup registry checkpoint (2026-10-05): seven addresses009C5110/6D60/7150/76D0 and009F1F90/2000/2010 registered in manifest, ARTIFACT_INDEX, corrections and four-unique-source symbol_index list; five permanent gates previously registered/pass. Retired three conflicting legacy allocator support candidate catalogue/index entries, source files retained; installer/flash/patch label corrections evidence-based. Index refreshed sequentially FileInstaller6entries then OutOfLineSupport1entry,33564defined symbols. Fresh closure work/codex-native-startup/native-gfmain-installer-cleanup-closure-20261005:1294sources/419unresolved/THREE DUPLICATES, CMemoryAllocator delete/EnterSynchronisation/LeaveSynchronisation emitted by aligned009F2BE0 unit conflict with new strong out-of-line support. Do not claim clean promotion: focused consistent-out-of-line71provider integration passes, default closure requires emission reconciliation. Inline-only support trial work/AllocatorSupport_inline_trial.cpp emits no unreferenced methods in VC7.1, so trivial inline keyword alone is insufficient. Next resolve helper COMDAT versus strong emission while preserving exact aligned187byte bodies and accepted range/helper parity; consider shared inline noinline support emitted by calling range unit with no standalone duplicate strong provider, verify callback fixture linkage and all required-provider gates. No threshold weakening or object/asm production patches. Previous clean baseline remains1290sources/zero duplicates/419unresolved. Own jobs terminal; Lua/FMP excluded; native recreation active/incomplete.

Native installer cleanup permanent gate checkpoint (2026-10-05): Four provider files prepared under rebuild/src/compiled: FileInstallerBufferArray_009c7150.cpp(NEAR76/85bytes93.75/93.939%), FileInstallerDestroyBufferRange_009c6d60.cpp(NEAR119/125bytes95.349%), FileInstallerClearBuffers_009c5110.cpp(MATCH85), CMemoryAllocator_OutOfLineSupport_009f1f90.cpp(MATCH15/12/12). Range source now includes authoritative installer chunk declaration, no duplicate scratch struct. Five permanent gates added to gate_all: gate_installer_buffer_array.py PASS128/oneomission, gate_installer_destroy_buffer_range.py PASS320/oneomission, gate_installer_clear_buffers.py MATCH85, gate_allocator_out_of_line_support.py resolved-fullbody MATCH39total, gate_installer_cleanup_os.py PASS71providers/71omissions/fourrealWindowscleanupcycles/oneretainedchunk. Reports under rebuild/compile-gate/installer-cleanup-*; all gates default to prepared provider files, no scratch source dependencies. No full gate_all rerun claimed. Manifest/artifact/corrections/symbol-index registration and legacy candidate retirement still pending; source files prepared and tested, not yet authoritative closure integration. Next finish seven-address registries, retire conflicting old009F1F90/2000/2010 catalogue sources without deleting files, refresh index sequentially and regenerate closure. Current measured closure unchanged1290sources/zero duplicates/419unresolved. Lua/FMP excluded; own jobs terminal; native recreation active/incomplete.

Native installer shared ClearBuffers composition checkpoint (2026-10-05): InstallerBufferArray_shared_candidate.cpp DeallocateMemory009C76D0 now NEAR_MATCH85bytes/33instructions/93.939%: explicit unsigned element-count>0 comparison reproduces retail SAR/test/JBE; previous implicit truth conversion compiled masked test and rejected86.15%. No threshold lowering, no asm; accepted, stop tuning. Shared array gate now requires both providers near, no rejection catch; PASS128behavior cases/one omission. RemoveRange unchanged76bytes93.75%. fable_file_installer.h declares ClearBuffers member. InstallerClearBuffers_shared_candidate.cpp/check_installer_clear_shared_parity.py MATCH85bytes/fourrelocations with actual shared CFileInstaller/CFileInstallerBufferArray ABI. InstallerClearBuffersSharedOS_test.cpp/replay_installer_clear_shared_os_scratch.py PASS71nativeproviders/71required-provider omissions/fourrealWindows allocation/deferred-shutdown cycles at sizes1/236/4096/65536/one retained original chunk fixture-released. Actual installer offset1C array owns null/live/null chunks; ClearBuffers real lock ctor/dtor, genuine array removal/range destructor/mainallocator shutdown, real malloc/free array storage and GlobalAlloc/GlobalFree read/write handles; array and read/write pointers zero, installer lock balanced. VirtualQuery verifies main and30classallocator objects released; retainedchunk cleanup and processheap validation pass. No array/range/allocator member fixture adapters; consistent opt-in out-of-line allocator support mode. Reports clear-buffers-shared-parity.json/shared-os.json and updated buffer-array-shared-parity/behavior saved. Next permanent gates/provider promotion and registry corrections for installer array/clear/range and allocator support, fresh index/closure; constructor/worker/destructor broader recovery still incomplete. Production source closure remains1290sources/zero duplicates/419unresolved; shared header expanded, scratch candidates not yet registered. Lua/FMP excluded; own jobs terminal; native recreation active/incomplete.

Native installer shared buffer-array composition checkpoint (2026-10-05): fable_file_installer.h now declares shipping InstallerBufferChunk8bytes/allocation+0/unknownword+4 and CFileInstallerBufferArray12bytes/first,last,capacity, with size assertions and range helper declaration; no donor C4 offsets transplanted. InstallerBufferArray_shared_candidate.cpp/check_installer_array_shared.py PASS128behavior cases/one omission, RemoveRange009C7150 remains76bytes93.75% NEAR_MATCH; DeallocateMemory009C76D0 remains86.1538%REJECTED, no production promotion. InstallerBufferArraySharedOS_test.cpp/replay_installer_array_shared_os_scratch.py PASS68actual providers/68required-provider omissions/four realWindows create/deferred-shutdown cycles at sizes1/236/4096/65536/one original retained chunk fixture-released. Actual malloc-backed array has null/live/null owned chunks; shared RemoveRange invokes genuine shared range destructor and recovered mainallocator, DeallocateMemory clears three pointers and frees array storage after range destruction; singleton/30allocator object region release and processheap validate. No array/range member fixture adapters; consistent opt-in allocator support compilation. Passing behavior is insufficient to accept DeallocateMemory code shape; belowthreshold provider remains scratch only. Reports buffer-array-shared-parity.json,shared-behavior.json,shared-os.json saved. Next resolve remaining DeallocateMemory compiler shape, integrate shared ClearBuffers and permanent provider registries/closure. Production source closure unchanged1290sources/zero duplicates/419unresolved; authoritative installer header expanded as described. Lua/FMP excluded; own jobs terminal; native recreation active/incomplete.

Native installer range shared allocator ABI checkpoint (2026-10-05): fable_memory_allocator.h adds typed range friend/forward declaration and opt-in FABLE_MEMORY_ALLOCATOR_OUT_OF_LINE_SUPPORT for existing protected delete/EnterSynchronisation/LeaveSynchronisation declarations; default inline definitions retained. InstallerDestroyBufferRange_shared_candidate.cpp now calls actual protected CMemoryAllocator members directly, no opaque allocator view. check_installer_range_shared.py PASS320callback-model cases/one omission, same119/125bytes95.349% NEAR_MATCH. AllocatorOutOfLineSupport_candidate.cpp/check_allocator_out_of_line_support.py complete resolved-byte MATCH15/12/12bytes at009F1F90/009F2000/009F2010, real VirtualFree/Enter/Leave imports and shared13CAA70 lock, no fabricated data/callback addresses in source. Small-function generic normalized comparison falsely rejects push0 relocation placeholder vs retail address despite identical relocated bytes; support gate enforces complete patched-byte equality directly, stronger than similarity, no threshold lowering. InstallerDestroyBufferRangeSharedOS_test.cpp/replay_installer_range_shared_os_scratch.py PASS67sources/67required-provider omissions/four actual Windows allocation-shutdown cycles/one original retained chunk fixture-cleaned, no member ABI adapters. All composed units compile consistent opt-in out-of-line declarations; mixed inline definitions initially supplied COMDAT alternatives when support omitted, resolved by consistent composition mode rather than weakening omission checks. Other providers in this composition are actual source implementations under this declaration mode, not separately asserted byte parity under changed compilation mode. Reports allocator-support-parity.json,destroy-buffer-range-shared-parity.json,shared-behavior.json,shared-os.json saved. Next move chunk/array declaration to authoritative installer header, resolve rejected array DeallocateMemory shape, actual array/ClearBuffers composition and promotion/registry/closure. Production source closure remains1290sources/zero duplicates/419unresolved; header changed as described, scratch sources not registered. Lua/FMP excluded; own jobs terminal; native recreation active/incomplete.

Native installer range destructor actual allocator composition PASS (2026-10-05): FileInstallerDestroyBufferRangeOS_test.cpp/replay_installer_range_os_scratch.py PASS66native providers/66required-provider omissions, four create/shutdown cycles/sizes1,236,4096,65536/one original retained chunk fixture-released. Real recovered main and30class allocators allocate through aligned API; range destructor processes null/live/null chunks using original rawallocation pointer, preserves all chunk words, frees via recovered InternalDeallocate, reads recovered GetNoAllocatedAreas, executes recovered main destructor/real VirtualFree, clears singleton, then original Leave after real DeleteCriticalSection. VirtualQuery verifies singleton and30allocator object regions released; retained original fixed/variable chunks handled explicitly by fixture, processheap valid. Fixture ABI adapters bridge temporary allocator view methods to protected recovered members and real Win32 imports; no claim of direct production ABI linking or multiple simultaneously-live variable-class shutdown behavior. Fixture generated once via work/build_installer_range_os_fixture.py from existing deferred allocator integration; original fixture unchanged. Report work/file_installer_constructor/destroy-buffer-range-os.json includes provider hashes and boundary. Previous320independent-model native callback cases valid; candidate unchanged NEAR_MATCH119/125bytes95.349%. Next replace scratch allocator/array views with authoritative shared declarations, recover out-of-line enter/leave/delete ABI, resolve DeallocateMemory belowthreshold candidate and compose real array/ClearBuffers before promotion. Production remains1290sources/zero duplicates/419unresolved; Lua/FMP excluded; all own jobs terminal; native recreation active/incomplete.

Native installer buffer-range destructor behavior checkpoint (2026-10-05): FileInstallerDestroyBufferRange_test.cpp/check_installer_destroy_buffer_range.py PASS320native cases/one required-provider omission. Independent expected event model derived from retail disassembly compares every boundary snapshot:all six chunk words,singleton,shutdown,lock depth and call owner/argument/order. Eight null/live chunk masks, singleton absent/present, shutdownfalse/true, count0/1 and five modes(normal,Enter replacesowner/clearscurrentchunk, free replacesowner/enablesshutdown, countqueryclearssingleton, destructor replacessingleton). Cached chunk allocation survives Enter mutation; subsequent chunks read fresh, count callback clearing singleton suppressesdelete safely, destructor replacement does not change cached delete target, singleton cleared before Leave, balancedlocks and unchanged metadata verified. This is native execution against independent instruction-derived model, not retail machine-code emulation; allocator boundaries remain recorded. Code candidate unchanged119vs125bytes/95.349% NEAR_MATCH; stop tuning. Next genuine allocator composition and shared member ABI/out-of-line allocator lock/delete provider reconciliation before promotion. Production remains1290sources/zero duplicates/419unresolved; Lua/FMP excluded; own jobs terminal; native recreation active/incomplete.

Native installer buffer-range destructor candidate checkpoint (2026-10-05): FileInstallerDestroyBufferRange_candidate.cpp/check_installer_destroy_buffer_range.py NEAR_MATCH119vs125bytes/11 relocations/41vs45instructions/8.889%delta/95.349% similarity at009C6D60. Full125byte retail range destructor now extracted by trace_installer_buffer_array.py, ending ret4; prior excerpt omitted final stack cleanup. Genuine C++ fastcall range(first,last,allocator-token) ignores token, skips null chunks, caches chunk pointer before Enter, uses fresh allocator after Enter, frees via InternalDeallocate, checks shutdown then GetNoAllocatedAreas, deletes fresh allocator if count zero, resets singleton, and Leave; chunk fields remain untouched. Opaque scratch allocator member ABI view preserves retail out-of-line destructor/operator delete boundaries; initial derived-access attempt failed protected delete access, inherited inline delete expanded VirtualFree, replaced with explicit view declarations. Do not promote or claim behavioral validation yet: independent callback/full-state replay and actual allocator composition remain required. Next test shutdown/count/singleton mutations across Enter/free/count/destructor/delete/Leave, then compose recovered main allocator and array/ClearBuffers. Accepted RemoveRange unchanged; DeallocateMemory remains rejected86.15%. Also found legacy009F2000 labelled flash destroy but retail lock wrapper;009F2010 correct body with separate legacy global symbol; shared lock ABI reconciliation needed. Production remains1290 sources/zero duplicates/419 unresolved; Lua/FMP excluded; own jobs terminal; native recreation active/incomplete.

Native installer buffer-array scratch checkpoint (2026-10-05): trace_installer_buffer_array.py saves retail009C7150/009C6D60/009C76D0 disassembly in work/file_installer_constructor/buffer-array-disassembly.txt (range destructor excerpt only, full body still to extract). FileInstallerBufferArray_candidate.cpp/check_file_installer_buffer_array.py RemoveRange009C7150 NEAR_MATCH76 bytes/one relocation/32 instructions/93.75% similarity. Source-relative forward copy preserves overlapping erase semantics; accepted, stop tuning RemoveRange. FileInstallerBufferArray_test.cpp PASS128 cases/one required-provider omission:120 exhaustive length0..7/begin/end intervals verify complete ten-element memory image, return/last pointer/tail destruction boundary, plus eight capacities0..7 with actual malloc/free array storage and recorded range destruction. DeallocateMemory009C76D0 rejected for production:86.1538% normalized similarity,32vs33instructions; current native behavior passes but insufficient for promotion. Candidate clears all three array pointers before second range call, zero capacity skips cleanup; actual element-owned engine allocations not yet composed. 009C6D60 range destructor uses recovered main allocator locking/internal free/deferred shutdown and must be recovered next, then genuine array/ClearBuffers composition. Reports buffer-array-parity.json explicitly mark rejection and buffer-array-behavior.json records test boundary. Production remains1290 sources/zero duplicates/419 unresolved; Lua/FMP excluded; all own jobs terminal; native recreation active/incomplete.

Native installer ClearBuffers scratch PASS (2026-10-05): FileInstallerClearBuffers_candidate.cpp/check_file_installer_clear_buffers.py MATCH85 bytes/four relocations/30 instructions/100% similarity at009C5110. FileInstallerClearBuffersOS_test.cpp PASS64 cases/three required-provider omissions with production CCriticalSectionLock ctor/dtor and real Windows critical section/GlobalAlloc/GlobalFree. Null/live read/write handle combinations and array-boundary null-to-live mutations confirm fresh handles after array deallocation, conditional release followed by zero stores; callback observes lock recursion1, final recursion0. Array deallocation009C76D0 remains recorded, no claim of actual array cleanup. Retail destructor009C5170 disassembly saved work/file_installer_constructor/destructor-disassembly.txt: outer lock, ClearBuffers, ExitThread flag7A, SetEvent84/88, yield with lock released/reacquired until both78/79 flags set, close four handles, delete critical section, alias/set/list/array cleanup and final base vtable129B414. Existing catalogue incorrectly labels009C5110 as CPatchTesselationEdgeStrip destructor and009C5170 as its helper; 009C7900 generic T::VecDel source is incorrectly labelled CLandscapeBackgroundPatch although RTTI proves installer. These require evidence-based registry/source replacement when promoting shared installer ABI; legacy generic source remains untouched. Next recover array deallocation and destructor dependencies, replace scratch ABI views with shared class methods, and promote verified providers. Production remains1290 sources/zero duplicates/419 unresolved; Lua/FMP excluded; all own jobs terminal, native recreation active/incomplete.

Native installer worker entry scratch PASS (2026-10-05): FileInstallerWorkers_candidate.cpp/check_file_installer_workers.py MATCH24 bytes/three relocations each at009C5870 and009C4420, 100% normalized instruction similarity; genuine C++ GetCurrentThread/SetThreadPriority calls then thiscall tail-call, priorities1/0. FileInstallerWorkers_test.cpp PASS54 native cases/one provider omission: handle null/1/ffffffff, return0/1/ffffffff, owner null/1/ffffffff, both entries; exact call order/forwarding, ignored priority result, zero actual threads. trace_file_installer_dependencies.py extracts retail evidence into work/file_installer_constructor/dependency-evidence.json: RTTI .?AVCFileInstaller@@ confirms vtable0129B43C slot0=009C7900 deleting destructor, calling destructor009C5170 and CRT delete00BFE9BC. Imports confirm GetCurrentThread143FDCC, SetThreadPriority143FDD0, MSVCR71 malloc1440158 thunk00BFEA0E. Donor Ego_r names distinguish static cdecl BeginThread/BeginDecompressionThread from member loops009C5690/009C4230; donor C4 layout not imported into shipping A0. Scratch ABI view still needs replacement with shared production method declarations and registries. No worker loop or installer destructor recovery claimed. Next recover destructor and loop boundaries or promote exact wrappers with explicit unresolved real member dependencies. Production remains1290 sources/zero duplicates/419 unresolved; Lua/FMP excluded. All own jobs terminal; native recreation active/incomplete.

Native file-installer constructor actual OS composition PASS (2026-10-05): FileInstallerConstructorOS_test.cpp/replay_file_installer_constructor_os_scratch.py PASS32 cycles/128 real auto-reset initially-unsignalled Windows events/20 required-provider omission checks. Genuine CRT malloc allocates five sentinels; production CharString constructors/copy/destructor/assignment and alias-map search/insertion/rebalance/erase execute, with actual string allocation and ownership returning to baseline each cycle. Real critical section recursively enters/leaves; events set/wait/auto-reset/close verified. Retail PE extraction confirms alias .biz (0129B448) -> .big (0129B440); generated fixture literals derive directly from EXE. Only constructor thread relocation redirected; two entry identities/0x3000 stack/owner validated, zero worker launches. Vtable identity, worker bodies and empty narrow-string fallback remain fixture boundaries; explicit fixture cleanup does not claim recovered installer destructor. Accepted constructor candidate unchanged; recorded24 full-state cases and five null-sentinel failures rerun PASS. Report work/file_installer_constructor/os-composition.json includes hashes/source inventory. Next recover production vtable/data/allocator/worker ABI dependencies before constructor promotion. Production remains1290 sources/zero duplicates/419 unresolved; Lua/FMP excluded; native recreation active/incomplete.

Native file-installer constructor sentinel failure PASS (2026-10-05): permanent oracle_file_installer_constructor_failure.py/FileInstallerConstructorFailure_test.cpp/gate_file_installer_constructor_failure.py PASS5independently predictedretail/native null failures,one at eachmalloc sentinel allocation. Retail faults writeaddress0/value0,size4 forfirst3listallocs,size1for2treecolorwrites; nativeSEH confirmswriteAVaddress0 and complete32KiBpartialowner/allocationstates,allboundarysnapshots/callcounts. Treehead/countzero beforefault, earliernodesexact,selflinks preserved;noInitCS/events/strings/threadcalls reached. VolatileSEH-observedfixturecounters ensurecompiler doesn'tfoldacrossexception. Candidate unchanged accepted397bytes90.441%similarity/18relocs;no guards invented. No forcedOSexhaustion claim;mallocboundaryrecorded. Existing24success/callbacknativecasespass. Next genuineCRTmalloc,actualInitializeCriticalSection/CreateEventW,productionCharString/aliasmap composition with recordedthreadentrybeforepromotion;data/vtable/helper ABI stillneedregistry. Production1290sources/zero duplicates/419unresolved unchanged,Lua/FMPexcluded,jobs terminal.

Native file-installer constructor callback expansion PASS (2026-10-05): oracle_file_installer_constructor.py/FileInstallerConstructor_test.cpp/gate_file_installer_constructor.py now24independentretail/native32KiB cases (4eventmasks*2threadreturns*3mutationmodes), candidate unchanged accepted90.441%/397bytes. Allocationcallbacks alterpendingcurrent-readfield74 and untouchedmapcomparison9C;InitCS callback mutatesflags80/81/7B;CreateEventcallbacks mutateflags80/7A,earlierhandle84 and comparison9C;worker-launchcallbacks mutate81. Independent model verifies subsequent original zero/initstores overwriteonlytheoriginalfields and preserveothers. Mutationmode2 firsteventcallback replaces recordedCreateEventW IAT/functionpointer slot with rejectingalternate; allremainingcalls use originalcachedEventCreator, no alternate invoked (bothretail/native). Per-case functionslot restored; no fabricated returnedheap geometry changes. Complete16boundary/final32KiB snapshots pass withoneprovideromission. Allmallocsnonnull;nullmalloc faults and actualheap/string/OS composition with recordedthreads remain next beforepromotion. Production1290sources/zero duplicates/419unresolved unchanged;Lua/FMPexcluded,jobs terminal.

Native file-installer constructor recorded native replay PASS (2026-10-05): permanent FileInstallerConstructor_test.cpp/gate_file_installer_constructor.py passes8independentretail cases/one required-provider omission, full32KiB snapshots at16boundaries+final. Genuine candidate397bytes/137vs135instructions1.48%delta90.441%similarity/18relocations enforced; no tuning. Native recorded malloc/CS/event/string-map/thread boundaries execute typed ABI; validates relocatedvtable before normalization and literal/temporary/map/threadentry argument identities. Five sentinels and all untouched bytes, fourCreateEventWsuccessmasks0/5/10/15 (allzeroargs), temporarystring/aliasvalue writes, two_beginthreadentry/stack3000/thisargs and ignored0/ffffffffreturns validated. Test-only COFF symbol redirects imports and threadroutine, including safe long-name string-table expansion; no worker launched. No nullmallocfailure/callbackmutation/actualheap/OS/stringcomposition yet. Next expand independent cases for callbacks and failure,then genuine allocator/string/map/Windows composition with threadboundaryrecorded beforepromotion/registry. Production1290sources/zero duplicates/419unresolved unchanged;Lua/FMPexcluded,jobs terminal.

Native file-installer constructor near candidate and independent oracle (2026-10-05): replaced chained assignment initialization with sequential stores matching proven retail ordering; candidate397bytes/137vs135instructions1.48%delta/90.441%normalized similarity now satisfies existing near threshold. No threshold adjustment; STOP byte tuning, behavior required. Permanent oracle_file_installer_constructor.py PASS8independent32KiBretail complete boundary/final snapshots, foureventsuccess masks0/5/10/15 and recordedthreadreturn0/ffffffff. Models5mallocsentinels,CSinitialize,4CreateEventW(null/false),temporaryCharString->actualaliasmapinterface->charassignment->tempdestruction,2beginthread(workerpriority entry addresses,3000,this); untouchedobject/block bytes preserved. Containerallocsallnonnull;eventfailures ignored/handles stored,threadreturnsignored. Boundaries recorded (opaqueCS5Apattern/stringdataFE/assignment12345678),no actualthread/OS claim. Full call/arg order135retailinstructions/returnthis/stackcleanup verified independently; native replay needs validated relocation/globalvtable/string/workeraddress normalization and cachedCreateEventIAT behavior. Nullmalloc faults and callbackgeometry/fieldmutations need added cases beforepromotion. Production1290sources/zero duplicates/419unresolved;Lua/FMPexcluded,jobs terminal.

Native file-installer constructor first genuine scratch candidate (2026-10-05): authored work/FileInstallerConstructor_candidate.cpp, using typed relocatedvtable/extensionstrings/worker entry functions, actual CCharString and CBasicRetailBankAliasMap public ABI, inline native list/tree sentinel initialization, real Win32 import declarations and cached EventCreator. FullRetailA0 offsets retained via proven field accesses, no donor-layout transplant. Compiles397bytes/137instructions versus retail389/135(1.48%delta),80.147%ordered similarity, BELOW90%acceptance; no promotion. Detailed18relocations and score in work/file_installer_constructor/initial-parity.json, checker work/check_file_installer_constructor.py. Sentinels declared malloc-helper boundary after BFEA0E inspected as IAT jump1440158 (unlike operatornewBFEA1A); use genuine CRT malloc when composing. Worker entries decoded:009C5870 GetCurrentThread/SetThreadPriority1 then taildispatch009C5690;009C4420 priority0 then009C4230. Avoid treating them as plain donor methods009C5870. Dependencies/string literals still need data/worker ABI recovery and independent constructor oracle; next compare candidate scheduling/sentinel members with retail and predict full callbackstates, then native replay/actual heap plus recordedthread composition before promotion. Constructor source starts real workers only if invoked unredirected; no actual workers launched during current compile. Production1290sources/zero duplicates/419unresolved unchanged;Lua/FMPexcluded,jobs terminal.

Native real file-installer constructor investigation (2026-10-05): decoded entire retail009C5890..009C5A14 (389bytes) into work/file_installer_constructor_disassembly.txt. Retail allocation sizeA0, vtable0129B43C, CS+4, initial vector+1C/+20/+24; five heap sentinel calls to BF EA0E withsizes16/16/184/20/24, stored+38/+3C/+40/+44/+94, lists selflink at+0/+4, RBtree sentinels colorbyte0/root0/selflinks+8/+C and counts+48/+98zero. Verified imports via PE:143FEE4InitializeCriticalSection,143FDC8CreateEventW. Four CreateEventW(allnull/false) calls cached sameIATfunction pointer, handles+84/+88/+8C/+90. Other zero fields/flags sequence recorded; flag+80true,+81false. Temporary CCharString constructor0099EBF0 literal0129B448/-1, actual aliasmapIndex009AC700 atowner+94, assignment0099EFE0 literal0129B440, destructor0099EAE0. Then two _beginthread(BFED3A) calls(entries009C5870/009C4420,stack3000,argthis); do not launchworkers in initial constructor oracle. Current donor DIA evidence: FableWin CFileInstaller sizeDC, Ego_r sizeC4 (work/file_installer_egor_layout.txt), both differ retailA0; map names by proven retailaccesses only, do not transplant layouts. Original sharedheader remains opaqueA0 and startupflag81, no speculative fieldedit. Next inspect allocatorBFEA0E, alias-map/string production ABIs, stringliterals and worker entry evidence; author genuine ctor scratch with typed containers/relocatedvtable/callbacks, recordedOS/thread/fullstate behavior then native composition. Production unchanged1290sources/zero duplicates/419unresolved;Lua/FMPexcluded,jobs terminal.

Native user-profile counted ownership landed (2026-10-05): production UserProfileManagerCountedReset_0040f1b0.cpp NEAR_MATCH103bytes/36instructions94.444%, UserProfileManagerDestroy_0040d830.cpp MATCH11bytes. Shared header typed callback,correct manifest0040F1B0physics label to boot countedassignment;artifacts/corrections/dedicated symbol providers/two gates registered. Obsolete0040D830gapscan catalog/index entry retired, historical source retained. Production gate_user_profile_counted_reset PASS144independent32KiBstates+2actualdestroycases/2omissions;gate_user_profile_counted_lifetime PASS32actualCRTscenarios64objects/controlblocksreleasedonce/2omissions. Independent current user-profile retail validated despite analogous installer structure. Index33,558symbols refreshed sequentially. Fresh combined closure1,290sources/zero duplicates/419unresolved, actual user-profile boot operator= dependency eliminated(previous420). No actual profile constructor/destructor-body claim. Alljobs terminal,Lua/FMPexcluded. Next select genuine remaining boot/native frontier, likely actual constructor lifecycle supporting singletons and engine startup; native game incomplete.

Native user-profile counted ownership scratch verified (2026-10-05): traced CUserProfileManager::Get0040D2A0 -> constructor0040CEC0 and counted reset0040F1B0 (103bytes), typed destroy0040D830 (11bytes). Retail reset structurally same as installer but independently decoded/replayed; evidence user_profile_singleton_disassembly.txt,user_profile_counted_disassembly.txt,user_profile_destroy_disassembly.txt. Genuine shared-header UserProfileManagerCountedReset_candidate.cpp NEAR_MATCH103bytes/36instructions94.444%three relocations; UserProfileManagerDestroy_candidate.cpp MATCH11/sixinstructions/zero relocs. STOP byte tuning. Dedicated oracle_user_profile_counted_reset.py/gate_user_profile_counted_reset.py/UserProfileManagerCountedReset_test.cpp PASS144full32KiBretail/native states+2actualdestroycases/2omissions. Adapted independent allocator/callback/failure/countwrap model validated against this address; no same-objectshortcut/freshobject/reference behaviors retained. gate_user_profile_counted_lifetime.py/UserProfileManagerCountedLifetime_test.cpp PASS32actualCRTownership scenarios64objects/controlblocksreleasedexactlyonce/2omissions. Manualsharedcount2 and real replacement/clear demonstrated;no actual user-profile constructor/destructor-body claim. Next promote/register these two providers and typed header,correct manifest mislabels,productionreplay and fresh closure should remove boot countedassignment frontier. Production1288sources/zero duplicates/420unresolved unchanged;Lua/FMPexcluded,jobs terminal.

Native file-installer counted ownership landed (2026-10-05): production FileInstallerCountedReset_00403db0.cpp NEAR_MATCH103bytes/36instructions94.444%, FileInstallerDestroy_00403630.cpp MATCH11bytes. Corrected00403DB0physics manifest label to shared boot CCountedFileInstaller::operator=; typed helper declaration and two dedicatedproviders/gates registered, artifacts/corrections updated. Obsolete00403630gapscan catalog/index entry retired, source retained. Production replay PASS144independent32KiBstates+2actualdestroy cases/2omissions; realCRTownership PASS32scenarios64objects/controlblocksreleasedexactlyonce/2omissions. Preserved callbackfreshreference/object, wrappedcounts, failure/null/sameinput behavior. Index33,557symbols refreshed sequentially. Fresh combined closure1,288sources/zero duplicates/420unresolved; actual boot operator= dependency removed (previous421). No CFileInstaller constructor/body/destructor claim, still frontier. Jobs terminal;Lua/FMPexcluded. Next native boot ownership/runtime dependencies: user-profile counted assignment/reset and singleton release or real installer constructor evidence; native game recreation incomplete.

Native file-installer counted behavior/lifetime checkpoint (2026-10-05): FileInstallerCountedReset_test.cpp/gate_file_installer_counted_reset.py PASS144independent32KiB retail/native cases+2actual null/polymorphic destroy cases/2omissions; near103byte reset94.444% and exact11byte destroy enforced. Old recording callbacks validated+normalized; newtyped destroy pointer validated before normalization. Covers old countwrap0/ffffffff,1lastowner/2shared, null/same/differentincoming, allocnull/success, destroy changesreference before delete, delete changes fields again, alloc changesobject before freshblockData read, all touched/untouched bytes. New FileInstallerCountedLifetime_test.cpp/gate_file_installer_counted_lifetime.py PASS32realCRTownership scenarios/2omissions;nullclear noalloc, firstobject/block create, manuallyseededsharedcount2, replaceoneowner decrementsoldblock1 preservingoldobject, clearotherowner destroysoldobject/block, clearreplacement destroysnewobject/block;64actualCRTobjects/blocks destroyed/released exactly once. FableOperatorNew/Delete fixture forwards CRT; no actual CFileInstaller body/destructor claim. Next promote/reset and destroy typed declarations, correct manifest00403DB0physics mislabel/register dedicatedproviders/gates/artifacts/corrections, fresh runtimeclosure should resolve operator= frontier. Nativeproduction1286sources/zero duplicates/421unresolved unchanged;Lua/FMPexcluded,jobs terminal.

Native file-installer reset independent oracle/destroy checkpoint (2026-10-05): FileInstallerDestroy_candidate.cpp uses genuine C++ virtual-destructor ABI view/delete, MATCH11bytes/sixinstructions/zero relocs to00403630; report work/file_installer_counted/destroy-parity.json. Permanent oracle_file_installer_counted_reset.py PASS144independently predicted32KiBretail cases (incomingnull/sameoldobject/differentobject; oldreferencenull/present; oldcount0/1/2/ffffffff; newblocknull/success; threecallback modes). Original reference decrement wraps, only1->0 destroys; destroycallback can replace owner.reference with alternateblock and object, subsequent operatorDelete uses fresh alternate reference; deletecallback can mutate again; clear/reference and newinputstore ordering retained. Allocation callback overwrites ownerobject(null/other) and referencepoison; successful block stores owners1,typed destroy403630,fresh callbackobject; failure still preserves object but clearsreference. Same-objectreset has no fastpath. Full boundary/finalstates/callsequence and thiscall stackcleanup match; helper returns recorded/no real destruction claim yet. Reset C++ remains accepted near103bytes94.444%, no byte tuning. Next native replay normalization of recorded oldblock callbacks/newtyped destroy and actual ownership lifecycle before promotion; production1286sources/zero duplicates/421unresolved unchanged. Lua/FMP excluded; jobs terminal.

File-installer reset parity completed: session48935 terminal exit0; candidate passes NEAR_MATCH103bytes/36vs36instructions/94.444%similarity/three relocations, report work/file_installer_counted/reset-parity.json. Stop byte tuning; behavior oracle/destroy thunk/native replay remain next. No jobs live.

Native file-installer counted reset evidence/candidate checkpoint (2026-10-05): traced CFileInstallerSingleton::Get00404440 to real constructor009C5890 and counted reset00403DB0, correcting old physics propagated label. Retail reset103bytes: release old block count (fresh reference reads after decrement/destroy), clear reference/store incoming, nonnull new12-byte block, owners1/destroy00403630/fresh object load; no same-object fastpath. Authored genuine shared-header FileInstallerCountedReset_candidate.cpp using typed callbacks/FableOperatorNew/Delete. Initial parity script caught incorrect test relocation mangling for void FableOperatorDelete and corrected to ?FableOperatorDelete@@YAXPAX@Z. Corrected parity command still LIVE unified exec session48935 at last confirmed poll; no result yet, do not restart on observation delay. Next poll same handle, then inspect destroy00403630, independent callback/failure/state oracle/native lifetime behavior before any promotion. Singleton release00404490 also freshly reads global reference after callback; do not assume cached release owner. Evidence files work/file_installer_singleton_disassembly.txt and file_installer_ownership_disassembly.txt. Production unchanged1286sources/zero duplicates/421unresolved; Lua/FMP excluded.

Native progress ownership landed (2026-10-05): production ProgressCountedConstructor_00403d20.cpp accepted near78bytes/28instructions/92.857%; ProgressDestroy_00403600.cpp exact11bytes. Shared fable_boot declaration, correct member ctor symbol and typed virtual-deleting callback replace old standalone CCountedPtr_ctor/literal403600 provider. Old leaf source/tests retained for history; removed obsolete build_candidates entry/index symbol, replaced manifest/artifacts/corrections; dedicated providers/gates registered. Production gate_progress_counted PASS24full32KiBretail/native states+2actual null/polymorphic destroy cases/2omissions; gate_progress_counted_lifetime PASS32real CRT shared ownershipcycles/4omissions/exactly32objects and blocks destroyed/released. Existing Set/Get and inline copy/destructor composed; no actual texture-bearing display destructor claim. Sequential index33,556symbols. Fresh combined closure1,286sources/zero duplicates/421unresolved, verifies CGame::Initialise constructor frontier eliminated (previous422). Jobs terminal; Lua/FMP excluded. Next trace boot remaining file-installer/user-profile counted ownership wrappers and genuine constructor/runtime frontier from evidence. Native game recreation remains incomplete.

Native progress counted replay/lifetime checkpoint (2026-10-05): ProgressCounted_test.cpp/gate_progress_counted.py PASS24independent32KiB constructor snapshots, constructor/destroy provider omissions2, near78byte constructor and exact11byte destroy enforced. Validates typed callback pointer before normalization; original stale references, failed allocation and object-mutating callbacks preserved. Genuine destroy null and real CRT polymorphic delete cases PASS2. New ProgressCountedLifetime_test.cpp/gate_progress_counted_lifetime.py PASS32scenarios/fourrequired-provider omissions, shared-header ctor/destroy plus existing SetProgressDisplay/GetProgressDisplay and native inline copy/destructor. Reference counts1->2->3->4->3->2->1, singleton clearing and final destruction/controlblockrelease exactly once independently checked;32actual CRT object/controlblock allocations/releases. FableOperatorNew/Delete fixture forwards CRT new/delete, test object has real virtual destructor; no claim about actual texture-bearing CProgressDisplay destructor. Next promote/replace obsolete00403D20 standalone helper with shared-header constructor, register00403600typed thunk/dedicated gates/index/manifest/artifacts/corrections, then fresh runtime closure (actual constructor unresolved should disappear). No further byte tuning. Production remains1284sources/zero duplicates/422unresolved, Lua/FMP excluded; jobs terminal.

Native progress destroy/constructor oracle checkpoint (2026-10-05): genuine scratch ProgressDestroy_candidate.cpp uses an abstract virtual-destructor ABI view and delete, MATCH11bytes/sixinstructions/zero relocations to retail00403600. VC7.1 rejects __thiscall nonmember function pointers, so no asm or calling-convention workaround introduced; C++ virtual deleting destructor naturally generates exact null-check/slot0/flags1. Constructor candidate remains accepted near78bytes/28instructions/92.857%, no tuning. Permanent oracle_progress_counted_ctor.py PASS24independent full32KiB retail states, inputnull/non-null, allocatornull/block, mutationnone/objectcleared/objectreplaced+referencepoison, initialreferencenull/stale. Retail writes incoming object before allocation(12); postcallback object freshly read for controlblock, reference overwritten with newblock/null, nullinputno allocation; original stale reference not released. Correct owners1/destroy403600/object fields and untouched bytes; thiscall return/pop verified. Next MainProgressCounted fixture native replay with typed callback relocation normalization and destroy ABI/callback/null test; actual CRT ownership copy/release composition before promotion/replacing obsolete catalog provider. Production unchanged1284sources/zero duplicates/422unresolved; Lua/FMP excluded; jobs terminal.

Native progress-display ownership constructor selected/recovered (2026-10-05): inspected current422-symbol runtime frontier and CGame::Initialise00413120, which requires CCountedProgressDisplay(CProgressDisplay*). Existing manifest00403D20 leaf uses standalone CCountedPtr_ctor plus literal destroy address403600; does not provide shared-header constructor required by runtime. Retail disassembly work/progress_counted_ctor_disassembly.txt proves78byte ctor, object store before cdecl new(12), null/failed allocation preserves object and clears reference, success owners1/destroy403600/object fresh postcallback load. Shipping destroy403600 is11byte null-check/vtable-slot0 deleting call(flags1), not a plain operator delete. Genuine shared-header scratch ProgressCountedCtor_candidate.cpp relocates typed destroy helper rather than integer address, near accepted78bytes/28vs28instructions/92.857%similarity/tworelocations. STOP byte tuning until behavior gate; no production promotion. Next recover genuine destroy thunk/shared declaration and independent retail/native callback/failure fullstate oracle; validate native ctor with boot ownership/release before registration/replacing obsolete provider, then fresh closure should remove actual boot constructor frontier. Production remains1284sources/zero duplicates/422unresolved. Lua/FMP excluded; jobs terminal.

Native debug allocation landed (2026-10-05): production CMemoryAllocator_InternalAllocateDebug_009f2ac0.cpp (MATCH91/five relocations, compiler-generated /GS cookie) and CMemoryAllocator_AllocateDebug_009f2b90.cpp (MATCH79/eight), total170exact. Registered donor ABI/prototypes in manifest (added missing static entry009f2b90), artifacts/corrections/symbol index and five permanent gates in gate_all. Production focused gates PASS384protected+144static independent full32KiB cases/oneomission each; actual Windows positive and zero/negative gates PASS66providers/66omissions each,12combinedscenarios/1536ops/1554checks/72retainedchunks; actual CRT cookie PASS6cases3match3reject via documented fixture security handler suppressing UI. Original zero/negative clamped allocation versus raw trailer/free-counter discrepancy preserved. Complete compiler GS epilogue, no assembly/naked production code. Index refreshed sequentially33,555symbols. Fresh combined closure1,284sources/zero duplicates/422unresolved; no new dependency frontier. All jobs terminal; Lua/FMP excluded. Native recreation remains active/incomplete. Next assess remaining allocator API/runtime boundaries or next genuine native game frontier from current evidence; no additional byte tuning of accepted providers.

Native debug permanent focused gates checkpoint (2026-10-05): MainDebugStatic_test.cpp/gate_main_debug_static.py PASS144native cases with exact79byte/eight-relocation provider and required-provider omission. Full32KiB snapshots/call order verify lazyCreate, fresh singleton after Enter callback, untouched five args incl invalid file/function/lineffffffff, null/non-null protected return, cached result survives singleton replacement/clear during callbacks; balanced locks. New gate_main_debug_internal.py enforces MATCH91/five and passes384independent full32KiBnative cases/oneomission, rather than scratch behavior-only gate. Both actual Windows66-provider positive and zero/negative compositions and six actual CRT checker cases already pass with /GS. Next promote separate debug sources009F2AC0/009F2B90 and point all permanent gates to production, register index/manifest/artifacts/corrections/gate_all, fresh closure sequentially. No further byte tuning. Production unchanged1282sources/zero duplicates/422unresolved; Lua/FMP excluded; jobs terminal.

Native debug zero/negative OS and static retail oracle checkpoint (2026-10-05): MainDebugEdgeOS_test.cpp/replay_main_debug_edge_os_scratch.py PASS66providers/66omissions6scenarios768ops777checks36retainedchunks. Zero,-1,-2,-17,minsigned inputs plus positive1..65536/categories0/9 execute real static/protected debug candidates with /GS/CRT checker and full native Windows allocation/free. Independent tracking model confirms bool overload clamps<=0 to1 and increments bytes by1, while outer debug trailer stores original signed bit pattern; free subtracts original size, retaining residual tracking counters across two cycles. All payload/metadata/enum/native validation/shutdown checks pass; original discrepancy preserved. New permanent oracle_main_debug_static.py independently verifies144retail static-wrapper cases, sizes0/1/236/maxsigned/minsigned/ffffffff, categories0/9, singleton absent/present, three callback modes, null/non-null recorded protected returns. All five arguments forwarded untouched including invalid file/function and unsigned lineffffffff; lazy Create, fresh singleton after Enter callback, return cached despite Leave clears singleton, balanced recorded lock, full32KiB snapshots/call order and fastcall ret12 verified. Next static native replay then promotion/registration of both exact170-byte debug providers with /GS. No further byte tuning; production closure1282sources/zero duplicates/422unresolved; Lua/FMP excluded, jobs terminal.

Native debug protected wrapper exact candidate checkpoint (2026-10-05): replaced explicit-cookie candidate with genuine12-byte local serialized debug record and sequential4-byte field copies, compiled /O2 /GS /MT. VC7.1 optimizes record storage/copies into direct trailer stores while retaining compiler-generated GS prologue/epilogue. Protected InternalAllocate009F2AC0 now MATCH91bytes/five relocations/33instructions/100%normalized similarity; report work/main_debug_allocate/internal-parity.json. Static caller already MATCH79/eight, total170exact. STOP byte tuning. Previous explicit-cookie source archived work/MainDebugAllocateInternal_explicit_cookie.cpp. All replays now compile /GS:384 native independent32KiBstates PASS, actual Windows66providers/66omissions6scenarios768ops777checks36retainedchunks PASS, real CRT cookie6cases3accepted3rejected PASS. Recording cookie helper updated to return known oracle allocation pointer0x200a000 in EAX: real compiler-inserted checker ABI preserves EAX, unlike ordinary void C++ test hook; actual unmodified CRT checker independently validated by OS/cookie tests. No naked/asm code introduced. Next zero/negative debug trailer/accounting composition and static wrapper callback replay, then promote/register with /GS permanent parity gates and fresh closure. Scratch only, production remains1282sources/zero duplicates/422unresolved. All jobs terminal; Lua/FMP excluded.

Native debug actual CRT cookie checkpoint (2026-10-05): MainDebugCookie_test.cpp and replay_main_debug_cookie_scratch.py PASS6 isolated native subprocesses, three matching cookies0/bb40e64e/ffffffff return with correct trailer; three callback-mutated global cookies are rejected by actual unmodified linked VC7.1 CRT checker. Documented _set_security_error_handler is installed only by fixture: verifies _SECERR_BUFFER_OVERRUN, prints mismatch marker, _exit(3), preventing legacy interactive CRT failure UI. Cookie boundary marker proves both queries completed before rejection; candidate absent link fails. Original default CRT mismatch produced exit3 once and interactive timeout on repeat (timed subprocess killed); reliable handler-based result does not claim default UI exit behavior. Candidate source unchanged. Code similarity still below90% so no promotion; next resolve compiler epilogue and zero/negative debug accounting behavior. Previous384snapshot native cases and66-provider actual Windows768ops/777checks remain valid. Production closure1282sources/zero duplicates/422unresolved; Lua/FMP excluded; jobs terminal.

Native debug allocation actual Windows scratch composition checkpoint (2026-10-05): MainDebugOS_test.cpp and replay_main_debug_os_scratch.py PASS66genuine providers/66omission checks,6scenarios/768allocation-free operations/777invariant checks/36original retained chunks. Executes scratch static five-argument Allocate and protected debug overload with unmodified linked CRT cookie checker, production bool dispatch/metadata/size/free and real30class allocators/Windows memory/locks. Positive requests1/2/3/4/12/13/44/45/235/236/237/255/256/257/4096/65536, categories0/9; file/function deadbeef/baadf00d and unsigned lineffffffff ignored. Full live payload/guard/trailer/counters/row selection/enumeration/validation checked; two cycles/three free orders, genuine live counts defer shutdown then protected free/explicit final shutdown release all object regions. Does not claim zero/negative actual composition, forced-cookie-mismatch rejection or promotion. QFE4035 compiler also yields same93bytes/34vs33instructions/89.552%similarity, fails existing90%threshold; report cookie-qfe.json. Native recorded384cases remain PASS. Next resolve genuine cookie epilogue code match (no threshold weakening), test real cookie mismatch and zero/negative unsanitized trailer semantics. Production closure remains1282sources/zero duplicates/422unresolved. Jobs terminal; Lua/FMP excluded.

Native debug protected wrapper replay checkpoint (2026-10-05): permanent MainDebugInternal_test.cpp and replay_main_debug_internal_scratch.py pass384 native C++ replay cases plus required-provider omission. Complete32KiB snapshots at bool-allocation, first/second main size query, cookie boundary and return match oracle_main_debug_internal.py independent retail model. Actual authored overload executes; bool allocation/main query/checker boundaries recorded. Cookie call is test-only object-symbol redirected to recording helper, genuine CRT cookie global linked; callbacks overwrite global cookie but saved argument remains entry value. This validates state/call behavior without claiming genuine mismatch rejection, real allocator composition, or accepted code similarity. Code parity remains below90% (89.552%,34vs33instructions); no promotion. Next resolve compiler/native cookie epilogue and validate real checker behavior/OS composition; static debug caller exact79bytes remains scratch. Production aligned and full closure unchanged1282sources/zero duplicates/422unresolved. Lua/FMP excluded; jobs terminal.

Native debug protected wrapper independent retail oracle checkpoint (2026-10-05): permanent oracle_main_debug_internal.py passes384 independently predicted retail cases. Six signed-size bit patterns, categories0/9, tracking0/255, capacities20/24/64/256, callback mutationfalse/true, cookies0/bb40e64e. Genuine retail009F2AC0 calls bool InternalAllocate with true, discards first size resultffffffff, uses second capacity, then writes exactly trailer magic/current tracking/original signed size; complete32KiB boundary/final snapshots agree, no per-ID counter/prefix write added by wrapper. Recorded allocation callback changes current tracking and global cookie; query callbacks tracking255 then3; final cookie boundary receives entry-saved cookie despite global change. Ignored file/line/function are invalid-pointer values (never dereferenced). Native replay and real cookie check still pending; oracle records cookie checker rather than claiming mismatch acceptance. Scratch codegen checks O1/O2+Og-/O2+Os/O2+GS all fail existing near threshold (report work/main_debug_allocate/cookie-flags.json); do not loosen threshold. Next native replay of384 states, then resolve semantically justified cookie epilogue. No production promotion, Lua/FMP excluded. Jobs terminal.

Native protected debug allocation candidate checkpoint (2026-10-05): authored work/MainDebugAllocateInternal_candidate.cpp with genuine C++ saved CRT security cookie/check, real bool-overload allocation and two main size queries, trailer writes preserving original signed size/current ID, five-argument donor ABI. Retail wrapper91bytes/33instructions/five relocations. Scratch explicit-cookie candidate currently34instructions/3.03%delta/89.552%ordered opcode similarity, below90% acceptance; NOT promoted and parity gate correctly rejects. Earlier nonvolatile form32instructions/89.231% also rejects. Compiler-call cookie check placement differs from retail compiler-inserted epilogue. Current volatile cookie preserves saved load across callbacks; volatile metadata had no effect on score. Must establish compiler GS treatment or a semantically justified near candidate, then independently replay callback/cookie/null behavior and actual OS composition before registration. Existing static caller remains exact79bytes and production aligned remains1282sources/zero duplicates/422unresolved. No naked/assembly substitution introduced; existing historical global_securitycheckcookie_00bfe9f9.cpp is naked/zero-cookie and cannot be accepted as native runtime support. Next recover/use genuine CRT cookie support, inspect VC7.1/QFE GS codegen and donor stack locals as needed. All jobs terminal; Lua/FMP excluded.

Native debug allocation ABI/static candidate checkpoint (2026-10-05): DIA inspected FableWin CMemoryAllocator methods; protected InternalAllocate(long,unsigned long,const char*,long,const char*) donor RVA02CBD950 and public static fastcall Allocate(unsigned long,unsigned long,const char*,unsigned long,const char*) RVA02CBF980 identify retail009F2AC0/009F2B90. Added matching nonvirtual declarations to authoritative header. Scratch MainDebugAllocateStatic_candidate.cpp compiles MATCH79bytes/eight relocations via check_main_debug_allocate_static.py, report work/main_debug_allocate/static-parity.json. No byte tuning needed; no promotion until behavior replay/composition. Retail protected overload calls InternalAllocate(size,category,true), makes two extra main size queries, writes trailer magic/current tracking ID/original signed size (no extra byte-counter increment), ignores file/line/function in shipping code, and checks saved security cookie at return. Retail null return is unsanitized. Need protected overload genuine C++ with preserved cookie behavior, then callback/fullstate retail oracle and native replay plus actual OS/debug allocation composition. Existing production aligned API remains landed1282sources/zero duplicates/422unresolved. No Lua/FMP. All jobs terminal.

Native aligned API landed (2026-10-05): promoted `rebuild/src/compiled/00/9f/CMemoryAllocator_Aligned_009f2be0.cpp`, genuine static CMemoryAllocator::AllocateAligned(unsigned long,long) and DeallocateAligned(void*), exact79/108bytes. Corrected prior manifest global labels; registered manifest/artifact/correction entries, dedicated symbol provider and four gates in gate_all. Production replays all PASS:936 independent full32KiB retail/native cases,65-provider actual Windows ordinary3scenarios/384ops/387checks/42retainedchunks;65-provider actual deferred4lazy-create/final-free cycles/1retainedchunk;56 retail/native null-return write faults after balanced actual locks. Prefix callback ordering, fresh singleton checks and destructor-before-leave ordering preserved. No OS exhaustion or multithreaded race claim. Index refresh33,550 symbols; both static methods present strong. Fresh combined closure1,282 sources/zero duplicates/422 unresolved, no additional unresolved dependency introduced. Lua/FMP excluded. All jobs terminal. Next recover remaining public main debug allocation wrapper009F2AC0 and static caller009F2B90 from native evidence; security-cookie behavior must be preserved. Broader native game recreation remains incomplete.

Aligned null-return failure checkpoint (2026-10-05): permanent `oracle_main_aligned_failure.py` independently predicts retail call ordering and low-address four-byte zero write faults for56 cases (sizes0/1/236/ffffffff, alignments1/2/4/8/16/256/4096, singleton absent/present). `gate_main_aligned_failure.py` and `MainAlignedFailure_test.cpp` pass all56 with a required-provider omission check; actual Win32 critical sections balance before native SEH catches matching write access violations. InternalAllocate failure is recorded, not forced OS exhaustion. The fixture uses minimal Win32/SEH declarations because VC7.1 environment lacks windows.h; SEH-observed callback/fault counters are volatile to survive optimizer assumptions across exceptional control flow. Production wrapper unchanged: genuine scratch C++ remains exact187 bytes;936 retail/native state cases,65-provider ordinary and deferred actual Windows gates already passed. Next promote and register both static CMemoryAllocator methods, correct old global manifest names/signatures, refresh symbol index and composed closure sequentially. No Lua/FMP work.

Aligned deferred shutdown actual Windows checkpoint (2026-10-05): `gate_main_aligned_deferred_os.py` passes 65 genuine providers/65 omission checks, four lazy-create/shutdown/final-free cycles for payload sizes1/236/4096/65536 and alignments1/16/256/4096. A real live allocation defers shutdown; final public aligned free destroys main and all30 class objects, resets singleton, and executes the original LeaveCriticalSection after actual DeleteCriticalSection. VirtualQuery confirms released object regions; heap validation and fixture cleanup of the one retained fixed chunk pass. Single-threaded actual OS ordering is now verified without reordering or substituted imports. The first fixture run exposed a missing required main count provider reference, fixed by explicit count/total checks; the next run exposed a fixture's incorrect fixed/variable expectation for236+alignment/header, corrected to total request+20<=256. Retail behavior unchanged. Forced-null allocation/fault comparison remains next before production promotion; scratch candidate still exact187 bytes,936 replay cases and ordinary65-provider Windows composition passed.

Aligned API actual Windows composition checkpoint (2026-10-05): `gate_main_aligned_os.py` passes 65 genuine native providers and 65 required-provider omission checks. `MainAlignedOS_test.cpp` completed 3 scenarios, 384 allocation/free operations, 387 invariant checks, and verified/released 42 chunks retained by the original destructor. Actual unmodified Win32 imports execute through aligned wrappers, real main tracked dispatch and all fixed/variable classes. Alignment powers 1..4096, zero through 131072-byte payloads, two cycles and three free orders are covered; original-pointer prefix, payload fit/preservation, guards/trailer, row selection, tracking bytes, counters, enumeration and native validation are checked after each operation. Ordinary explicit shutdown releases allocator objects; deferred shutdown on final aligned release and forced failure remain pending before promotion. The earlier 936 recorded replay cases and exact 187-byte parity remain valid. No Lua/FMP work.

Aligned API native replay checkpoint (2026-10-05): `gate_main_aligned.py` passes both exact retail bodies (79 + 108 = 187 bytes) and 936 native cases (648 allocation, 288 release), with one required-provider omission check. `MainAligned_test.cpp` executes the genuine C++ wrappers against recorded Create/internal allocate/free/destructor and Win32 lock/free boundaries. Complete 32 KiB call and final snapshots match independently predicted retail behavior, including callback singleton replacement, pre-lock prefix capture, post-unlock prefix write, counter wrapping and deferred deletion with a cached owner. This is recorded-boundary evidence; actual aligned Windows allocator composition and failure behavior remain next. Candidates remain in scratch and are not production registered yet.

# Native aligned API retail oracle checkpoint - 2026-10-05

Native recreation remains active until stopped; Lua/FMP excluded. Latest production closure remains1,281 sources/zero duplicates/422 unresolved, main tracked allocation/free already landed below. Current aligned candidates work/MainAligned_candidate.cpp remain exact187 bytes total (AllocateAligned79/DeallocateAligned108), no native behavioral promotion yet.

Added permanent oracle_main_aligned.py, successful independent retail predictions for936 cases:648 allocation/288 release. Aligned sizes0/1/235/236/7fffffff/ffffffff, alignment1/2/4/8/16/32/64/256/4096, returned original pointer offset0/3, initial singleton absent/present, three callback modes. Correct size+alignment+3 forwarded to category0/false internal boundary; lazy Create and fresh singleton after Enter; pointer rounding and original-pointer store occur after Leave. Leave callback overwrites prefix or clears singleton, final returned pointer/store remain original-result based.

Release nullfastpath, saved originalnull/nonzero, absent/present singleton, counters0/1/3, shutdownflagfalse/true, callbacks and VirtualFreeresult0/1. Original pointer captured before Enter callback overwrites prefix. Enter callback switches singleton; InternalFree callback can change singleton/flag/count to request destruction of alternate current owner. Destructor callback changes singleton again; VirtualFree still releases cached destructor owner, then singleton reset precedes Leave. Independent full storage/global/flag/lock-depth/call snapshots all match retail. Main helpers/Create/destructor and imports recorded by oracle; no actual native or OS aligned behavior claim yet. Stop byte tuning at exact candidate.

Next MainAligned_test.cpp/gate_main_aligned.py native replay of same936 recorded boundary cases, candidate-required omissions and full snapshots; hooks mapping in oracle kinds Enter1/Leave2/Create4/InternalAllocate5/InternalFree6/Destructor7/VirtualFree8, arguments11words. Global singleton/flag/depth mirrored at region offsets3000/3004/3008. Then genuine aligned OS composition and failure cases before promotion. User status question answered; no stop request. All jobs terminal. Preserve unrelated work below.

# Native main tracked allocation/free landed - 2026-10-05

Native recreation remains active until stopped; Lua/FMP excluded. Promoted CMemoryAllocator_InternalAllocate_009f2a40.cpp MATCH122 bytes/three relocations and CMemoryAllocator_InternalDeallocate_009f2920.cpp MATCH155/five. Total277 exact bytes, original shared class ABI/fields/metadata methods, no algorithm/assembly substitution. Registered manifest/artifact index/corrections/symbol index and focused/OS gates in gate_all. Focused production replays pass960 allocation and720 free full32-KiB cases (three/two omissions); prior candidate checkpoints below retain detailed independent model/callback evidence. Stop byte tuning at exact pass.

New permanent MainInternalOS_test.cpp/gate_main_internal_os.py production replay passes6 actual Windows scenarios/768 allocation-free operations/777 invariant checks,64 genuine providers/64 omitted-provider failures. Real main Create/constructor/30-row table, all class methods, main InternalAllocate/InitDebugInfo/GetAllocationSize/InternalDeallocate, actual heap/virtual memory and real nested critical sections. Requests[-1,0,1,4,12,13,44,45,235,236,237,255,256,257,4096,65536], categories0/9 and boolfalse/true, forward/reverse/middle frees, two cycles. Independent size-class selection at fixed/variable total256 boundary (requested236/237); headerID/leading guard/trailer magic/storedID/requested size and capacity, per-ID counters/NoAllocations, payload/nonoverlap, class enumeration maps userPointer-4, native statistics/validation checked after each operation. Fixture sets documented currentTracking byte414, wraps protected internal calls in outer synchronization, without inventing public wrapper behavior. Plain user pointer is classData+4, no imposed alignment16 semantics.

Actual main NoAllocations now naturally exercises deferred shutdown with live allocations, without artificial counter. Internal frees bring countzero; explicit second shutdown releases singleton and30 allocator object regions, confirmed MEM_FREE. Original main destructor retains36 empty fixed chunks across six scenarios; fixture confirms and explicitly releases those real OS allocations. Whole heap remains valid. Report main-internal-os.json. No forced OS failure or live engine shutdown-registration claim. All jobs terminal.

Index last output33,543 symbols. Fresh closure1,281 sources/zero duplicates/422 unresolved at work/codex-native-startup/native-gfmain-main-internal-closure-20261005. Broader game recreation incomplete, no manual coverage totals.

Next aligned API recovery: authored scratch work/MainAligned_candidate.cpp and work/check_main_aligned.py, both complete MATCH (AllocateAligned009F2BE0 79 bytes/eight relocations; DeallocateAligned009F2B20 108/eleven), total187 exact. Added public static declarations with real donor ABI AllocateAligned(unsigned long,long) / DeallocateAligned(void*), both fastcall. CORRECTION to older checkpoints: these are CMemoryAllocator static members, not global functions. Manifest propagated _global labels are wrong. FableWin030BF8F0/030BF950 and Ego_r00B469D0/00B468F0 independently identify mangled member names and signed long alignment. No production source/registry promotion for aligned methods yet; behavior fixtures required.

Aligned allocate lazycreates singleton, enterslock, calls actual InternalAllocate(size+alignment+3,category0,false), leaveslock, rounds (original+alignment+3)&~(alignment-1), stores original main pointer at aligned-4. No null/failure sanitization. Aligned free nullfastpath invokes no API; otherwise captures original pointer before enteringlock, conditionally invokes InternalDeallocate if original/singleton nonnull, performs requested zero-count main destruction/VirtualFree and singleton reset while inlock, then leaveslock. Test original capture-before-callback, fresh singleton/flag/counter reads and last-free destruction call order. Record boundaries first, then actual successful aligned OS paths; preserve original DeleteCriticalSection-in-destructor followed by LeaveCriticalSection sequence, do not invent a reordered release. Existing inspector full retail disassembly work/main_allocator_operations_disassembly.txt. New exact candidates should not be byte-tuned. Preserve unrelated work below.

# Native main free dispatch behavior candidate - 2026-10-05

Native recreation remains active until stopped; Lua/FMP excluded. Added permanent oracle_main_internal_deallocate.py, MainInternalDeallocate_test.cpp and gate_main_internal_deallocate.py. Exact155-byte/five-relocation work/MainInternalDeallocate_candidate.cpp passes720 full32-KiB retail/native cases with two required-provider omissions. Real landed main GetAllocationSize110-byte provider executes. Seeds are independently predicted successful retail InternalAllocate metadata states from the960-case allocation oracle, filteredcategory0/max32/debugfalse; no native allocation/free OS claim yet. Report rebuild/compile-gate/main-internal-deallocate.json.

Forty allocation seeds cover signedclamp/boundaries/maxaddition, initial specialrange/byteheader and allocation callback effects; each seed runs normal/callback/invalidID paths and slot3 capacities0/1/4/7/64/256. Total480 valid frees/240 invalidID cases. Independent complete call/final state predictions: genuine main size query twice, first0 ignored, second256 locates stored tracking metadata; subtract storedID/requested value even when CurrentTrackingID callback changes99. Cached allocator retained while callback changes main table pointer for second query. Third direct slot3 query uses original cached context, and callback changes that object's vtable; subsequent slot1 reload invokes alternate free after exact bulk/byte CD fill. Callback free sets NoAllocations0 and currentTracking77, then original decrement wrapsffffffff. Normal free decrements original wrapped0 or8 toffffffff/7; tracked bytes return original seededfffffff0. Invalid header255 with empty special range invokes no callbacks and preserves complete state. Four validated native vtable cells normalized; lock imports/size/free methods recorded, no actual OS free in focused fixture.

Both main dispatch candidates now pass focused behavioral gates: InternalAllocate122 bytes/960 cases (three omissions), InternalDeallocate155 bytes/720 cases (two omissions), total277 exact bytes/1,680 retail/native cases. Neither promoted or registered yet. No source/index/closure changes; latest closure remains1,279 sources/zero duplicates/422 unresolved at work/codex-native-startup/native-gfmain-main-init-debug-closure-20261005. No jobs live. Stop byte tuning, proceed to actual native OS allocation/free composition.

Next implement dedicated MainInternalOS_test.cpp/gate_main_internal_os.py, composing59 mainOS sources plus landed GetAllocationSize/rangeData/InitDebugInfo and two candidates (64 genuine providers). Use real main constructor/table and actual class methods. Suggested requests[-1,0,1,4,12,13,44,45,235,236,237,255,256,257,4096,65536], categories0/9, forward/reverse/middle free orders, two cycles; verify independently chosen rowID based on requested-clamped+20, returned classcapacity>=requested+20, prefix/trailing guards/metadata/tracked ID/requested size, per-ID byte counters and NoAllocations, live payload/nonoverlap, class enumeration maps userPointer-4. CurrentTrackingID may be set through documented byte414 in fixture, no API claim for unlanded setter. Plain InternalAllocate returns classData+4 and need not preserve classalignment16; do not impose aligned-wrapper semantics. Use real outer critical section around internal calls to test nested size-query locking if desired. Real NoAllocations now naturally controls deferred shutdown; no artificial counter needed. If requesting shutdown with live allocations, explicitly free through internal method and call shutdown again, since global public wrapper auto-shutdown is not yet recovered. Main destructor retains empty fixed chunks, verify and explicitly release retained OS storage afterward. Promotion only after composition passes. Preserve unrelated work below.

# Native main allocation dispatch behavior candidate - 2026-10-05

Native recreation remains active until stopped; Lua/FMP excluded. Added permanent oracle_main_internal_allocate.py, MainInternalAllocate_test.cpp and gate_main_internal_allocate.py. Scratch work/MainInternalAllocate_candidate.cpp remains unpromoted pending free-path and actual OS composition. Exact122-byte InternalAllocate009F2A40/three relocations now passes960 full32-KiB retail/native cases with three required-provider omissions. Real landed InitDebugInfo77-byte and GetAllocationSize110-byte providers execute; only class slot0/slot3 and lock imports recorded. Report rebuild/compile-gate/main-internal-allocate.json.

Independent predicted selected index min(unsigned(clampedSize+20),categoryMaximum+1), total argument and full state/events. Distinct allocator context per lookup entry verifies actual selection, not just equivalent return. Sizesffffffff/0/1/12/13/44/45/255/7fffffff/80000000, categories0/9, maximum0/32/64, boolfalse/true, null/success and special-range/byteheader paths;480 successful and480 null results. Raw-context maximum/category variants are not constructor configuration claims. Null preserves counters/metadata apart from explicit callback mutations. Signed-clamped requested sizes and original wrapping addition preserved; large totals recorded, no actual OS success claim.

Allocation callback changes selected entry rowID, replaces owner category lookup pointer and maximum after selection, sets NoAllocations7/currentTracking7, changes range to toggle inside/outside and main table row2 target. Original cached selected lookup cell remains authoritative; stored rowID and special-range check use postcallback values. Successful NoAllocations increments originalffffffff->0 or callback7->8; user pointer advancesfour bytes. Genuine metadata initializer calls actual main size query twice; first size0 ignored, second256 places trailer. Query callback changes tracking255 then3 and table target for second call; latest tracking metadata/per-ID counter write expected, CD/trailing unused payload preserved. Validated/normalized slot0/slot3 native test addresses only; complete prestates/final states and table/range mirrors compared. Fresh replay after unique lookup-context strengthening passes all960 cases.

No production/index/closure additions this turn. Latest production closure remains1,279 sources/zero duplicates/422 unresolved at work/codex-native-startup/native-gfmain-main-init-debug-closure-20261005. No jobs live. Next focused retail/native InternalDeallocate fixture with genuine main size query, seeded guard metadata from successful allocation cases; verify two metadata-size queries then cached allocator slot3, tracked-ID subtraction independent of current ID, complete/partial CD fill, reloaded allocator vtable for slot1, original postcallback NoAllocations decrement. Include invalid headerID/range sentinel cases and counter underflow. Then actual main InternalAllocate/InternalDeallocate OS lifetime with real classes and metadata, followed by promotion/registration only on behavior pass. Candidate work/MainInternalDeallocate_candidate.cpp already exact155 bytes/five relocations. Preserve unrelated work below.

# Native main guard/tracking initialization landed - 2026-10-05

Native recreation remains active until stopped; Lua/FMP excluded. Promoted CMemoryAllocator_InitDebugInfo_009f28d0.cpp: complete MATCH77 retail bytes/two original GetAllocationSize call relocations. Shared protected ABI(void*,bool,long) already corroborated FableWin/Ego_r. Genuine C++ writes original guards/metadata/tracking counter and executes both original size queries; debug bool remains unused. Registered manifest/artifact index/corrections/symbol index; focused/OS gates integrated in gate_all. Stop byte tuning at exact match.

gate_main_init_debug_info.py production replay passes576 full32-KiB retail/native cases/1,152 genuine main size queries/two required-provider omissions. Independent expected prestates/final states compare all owner/storage/table/range bytes and guard/counter writes. First size-query return0/32/ffffffff ignored, second20/24/64/256 determines metadata position. Requested0/1/7/7fffffff/80000000/ffffffff; wrapping counters seededfffffff0, initial tracking0/255, boolfalse/true. First size-class callback changes CurrentTrackingID255 and tablecontext, second sees alternate context then changes tracking7; real initializer reads latest tracking after both calls. Prefix/user-4 sentinelffffffff; metadata at user+capacity-20 hasffffffff/currentID/requestedSize; final four allocation bytes and all other payload unchanged. Real GetAllocationSize provider executes with recorded locks/slot3, validated native vtable pointer normalized once. Report main-init-debug-info.json.

gate_main_init_debug_info_os.py production replay passes6 real main singleton/30-class lifetime scenarios,936 metadata checks,62 genuine providers/62 omissions. Actual InitDebugInfo and main size query operate on real fixed/variable allocations with capacity>=20. Independently predicted complete class payload and whole main owner match actual prefix/trailer/ID/requested/counter writes; actual lock recursion balanced. Fixture deliberately supplies rowID after direct size-class allocations and restores payload after verifying metadata, retaining all prior OS invariants. Existing4,320 main size queries/1,890 operations/1,902 lifetime checks/174 retained chunks pass. No claim yet for main InternalAllocate/InternalDeallocate. Report main-init-debug-info-os.json. MainAllocatorOS_test.cpp has conditional size/metadata modes; default original main OS mode unchanged.

Latest index output33,541 defined symbols. Fresh closure1,279 sources/zero duplicates/422 unresolved at work/codex-native-startup/native-gfmain-main-init-debug-closure-20261005. All jobs terminal; no manual coverage totals. Next permanent candidate-required dispatch/tracking allocation-free fixtures for exact work/MainInternalAllocate_candidate.cpp (122 bytes) and work/MainInternalDeallocate_candidate.cpp (155 bytes). Compose landed main size query/metadata/range data, first record allocation/size/free callbacks with full states, category lookup/byte ID and special range, clamped signed input/overflow/null allocation, metadata and counter transitions; then actual OS main allocation/free. These two methods remain scratch candidates with no production registration. Broader game recreation incomplete. Preserve unrelated work below.

# Native main allocation-size landed checkpoint - 2026-10-05

Native recreation remains active until stopped; Lua/FMP excluded. Promoted CMemoryAllocator_GetAllocationSize_009f2390.cpp, complete MATCH110 bytes/seven relocations. Added AllocatorRangeData_013caaa4.cpp defining address-named special range start/end globals; original donor variable names unidentified. PE section inspection proves both occupy .data zero-filled virtual tail, not file-backed bytes. Direct va_to_off raw-file lookup for these BSS addresses yields unrelated bytes and was replaced with explicit section virtual/raw extent check in OS gate; no fitted production data. Source/data registered in manifest/artifact index/corrections/symbol index; focused and OS gates in gate_all.

gate_main_allocation_size.py production replay passes1,296 full32-KiB retail/native cases,608 valid dispatches/688 null-or-invalid original lock leaks,one provider omission. gate_main_allocation_size_os.py passes6 actual main singleton/30-size-class scenarios,4,320 main size queries (runtime counted/asserted),61 genuine providers/61 omissions. Real size query dispatches fixed/variable slot3 using explicit native area-header rowID supplied by fixture after direct class allocations; special range ID0 path tested with actual mapped pointer. Actual critical-section recursion counts confirm valid calls balance, invalidID255/null calls leakone entry; fixture explicitly releases leaked entries and restores areaID. All existing1,890 operations/1,902 invariant checks/174 destructor-retained chunks still pass. Actual unmodified Win32 APIs, main constructor/table/lifetime/shutdown and recovered class methods execute. No claim yet for main InternalAllocate/InitDebugInfo/InternalDeallocate; direct class fixture deliberately supplies rowID.

Fresh closure1,278 sources/zero duplicates/422 unresolved at work/codex-native-startup/native-gfmain-main-allocation-size-closure-20261005. Symbol index refreshed and all three new provider symbols explicitly verified; last indexing output33,540 rows (index refresh drops missing pre-existing source paths; do not hand-edit coverage). No jobs live. No full-game boot claim.

Next permanent tracking/guard fixtures for remaining exact scratch candidates work/MainInitDebugInfo_candidate.cpp (77), MainInternalAllocate_candidate.cpp (122), MainInternalDeallocate_candidate.cpp (155). Main GetAllocationSize and range globals now real providers available for composition. Original size query capacity includes reserved prefix/trailer; preserve repeated calls and leaked-lock early returns. Need callback mutations/order/full states, counter/metadata extremes and null allocation; then actual main allocation/free OS composition. Stop byte tuning at exact candidates. Existing candidate checkpoint below has source/ABI evidence. Preserve unrelated work below.

# Native main allocation-size behavior checkpoint - 2026-10-05

Native recreation remains active until stopped; Lua/FMP excluded. Added permanent oracle_main_allocation_size.py, MainAllocationSize_test.cpp and gate_main_allocation_size.py. Gate currently requires scratch work/MainAllocationSize_candidate.cpp, no production promotion/registry/closure additions yet. Exact110-byte GetAllocationSize009F2390/seven original relocations now has1,296 passing full32-KiB retail/native behavior cases:608 valid dispatches,688 original early-return lock leaks,one required-source omission. Report rebuild/compile-gate/main-allocation-size.json.

Independent expected ID/lock/call/state model: null pointer, byteheader IDs0/29/30/255, NoAllocators0/1/30, special range IDs0/1/29/255, belowstart/atend/reversedrange; returns0/ffffffff and initial nested lockdepth0/1. Enter callback changes owner count/header/range/table to alternate target, confirming all are read after entry. Recorded slot3 callback changes owner/header/range/table during call; original capacity return and leave order preserved. Valid dispatch passes exact userPointer-4 and current table context; null/invalid returnszero without Leave. Every boundary and final snapshot compares complete owner/storage/table/range state; only one native test vtable function address is validated then normalized. Special range high-address arguments are intentionally passed as values to recorded size method, never dereferenced; no native mapping claim for those pointers. Fixture mirrors real static table/range data into snapshot region; recording only affects lock imports and slot3 boundary after complete original relocation parity.

No real critical-section or actual size-class implementation claim for this focused gate. Existing mainOS integration proves separate real locking/size-class methods but still does not exercise this candidate. Next compose GetAllocationSize with real fixed/variable methods and production range data, then promote/register it; continue full tracking/guard fixtures for three other exact candidates below (InitDebugInfo77/InternalAllocate122/InternalDeallocate155 bytes). No byte tuning required. Production closure unchanged1,276 sources/zero duplicates/422 unresolved at work/codex-native-startup/native-gfmain-fixed-allocation-closure-20261005. No jobs live. Preserve unrelated work below.

# Native main tracking candidates checkpoint - 2026-10-05

Native recreation remains active until stopped; Lua/FMP excluded. Recovered four separate scratch genuine C++ candidates, all complete-body MATCH under VC7.1: work/MainAllocationSize_candidate.cpp GetAllocationSize009F2390 110 bytes/seven relocations; work/MainInitDebugInfo_candidate.cpp InitDebugInfo009F28D0 77 bytes/two relocations; work/MainInternalAllocate_candidate.cpp InternalAllocate009F2A40 122 bytes/three relocations; work/MainInternalDeallocate_candidate.cpp InternalDeallocate009F2920 155 bytes/five relocations. Total464 exact bytes. Commands python work/check_main_allocation_size.py and python work/check_main_tracking_candidates.py passed; reports work/main_allocation_size/parity.json and work/main_tracking/parity.json. No behavior fixture yet, no production-source/manifest/index promotion, no new closure. Stop byte tuning at exact candidates; next implement permanent candidate-required retail/native behavior composition.

Added shared CMemoryAllocator GetAllocationSize public declaration and protected InitDebugInfo(void*,bool,long), InternalAllocate(long,unsigned long,bool), InternalDeallocate(void*) declarations. ABI corroborated FableWin/Ego_r names and complete retail disassembly; no class layout or vtable change. Address-named unsigned globals g_FableAllocatorRangeStart_013CAAA4/g_FableAllocatorRangeEnd_013CAAA8 declared, donor variable names not established and native data provider not yet authored. Candidate fixtures must cover special range and byteheader ID paths; do not assume original variable names. Main size query FableWin030BDD40/Ego_r00B45F00, metadataInit030BD820/00B46420.

GetAllocationSize enters synchronization before testing null or resolving ID. In special range [start,end) ID=(userPointer-start)>>24 truncated uchar; otherwise userPointer[-5]. Invalid ID>=NoAllocators or null returnszero WITHOUT LeaveSynchronisation: original lock-depth leak must be tested/preserved. Valid ID calls original table allocator virtualslot3 with userPointer-4, leaves lock and returns its exact capacity. It does not subtract the prefix or metadata size. This corrects the earlier tentative interpretation that a size query returns user capacity. InitDebugInfo calls main size query twice (including discarded first result), then places metadata at userPointer+capacity-20, writesffffffff/trackingID/requestedSize there and leading userPointer-4 sentinel, increments current tracking bytes. Trailing last four bytes are untouched.

InternalAllocate clamps signed size<1 to1, saves requested size, adds20 and indexes lookup min(unsigned total,categoryMax+1), virtualAllocate(total). Null allocation returnsnull with counters/metadata untouched; successful pointer gets byte allocatorID only outside special range, increments NoAllocations, advancesfour bytes and calls real metadataInit. Original size addition has no overflow guard; test extreme signed inputs against actual compiler/retail body rather than adding sanitization. InternalDeallocate resolves range/headerID and checks unsigned NoAllocators; invalidID returns without changes. Valid ID caches allocator, repeats main size query twice, reads trackingID/requestedSize from metadata and subtracts bytes, asks cached allocator slot3 for whole original class payload capacity, fillsCD, then virtualslot1 deallocates classData=userPointer-4 and decrements NoAllocations. Preserve call order, callback reload behavior and original range/header/tracking quirks.

Fresh main actual OS regression after shared header changes passes6 scenarios/1,890 operations/1,902 checks/174 retained chunks,59 providers/59 omissions; that fixture still uses direct size-class methods and does NOT validate these new main dispatch candidates. Latest production closure remains1,276 sources/zero duplicates/422 unresolved at work/codex-native-startup/native-gfmain-fixed-allocation-closure-20261005. No jobs live. Next: main size query focused recorded lock/virtual-slot fixture with full storage, special range boundaries/truncated ID, null/invalid lock leak and callback owner/header/range/table mutation; then tracking/guard allocation-free composition with real fixed/variable methods and actual OS paths. Promotion only after behavior. Broader game recreation incomplete. Preserve unrelated work below.

# Native main allocator OS integration checkpoint - 2026-10-05

Native recreation remains active until stopped; Lua/FMP excluded. Added permanent MainAllocatorOS_test.cpp/gate_main_allocator_os.py and gate_all entry. Production source providers unchanged, latest closure remains1,276 sources/zero duplicates/422 unresolved at work/codex-native-startup/native-gfmain-fixed-allocation-closure-20261005 (no fixed/variable allocator frontier).

New main allocator OS gate passes6 actual Windows scenarios/1,890 size-class allocate-free operations/1,902 invariant checks,59 genuine sources/59 omission link failures. Explicit provider list in gate (not dependent on generated reports). Actual unmodified Win32 APIs and CRT malloc/free, genuine CMemoryAllocator::Create009F2870, constructor009F20B0, initializer009F1D20,30-row MemoryAllocatorData01396868, real base/fixed/variable constructors and all native vtables, main statistics and destructor009F2310/shutdown009F2D20. Initializer runs in isolated directory without boot.ini, no mutation to user files. Constructor category existence and size lookup boundaries0..257/category9 entries checked independently. Native fixed/variable enumeration/index/count/total/validation methods, payload/alignment/nonoverlap, forward/reverse/permuted frees in two cycles or live payload at shutdown. Original variable GetNoAllocatedAreas returns0, preserved in main aggregation. Direct size-class calls do not update main NoAllocations; test explicitly sets documented counter to1 only for deferred shutdown, then0 to release. Does not claim main static Allocate/Deallocate behavior or engine shutdown registration.

Actual shutdown frees singleton and30 allocator VirtualAlloc object regions, confirmed MEM_FREE. Main destructor does not invoke derived destructors, so chunks/payload remain:174 retained chunks independently verified and explicitly released afterward by fixture through real HeapFree/VirtualFree. Report rebuild/compile-gate/main-allocator-os.json. Both first gate and fresh replay after replacing generated-report dependencies with explicit59-source list pass. No jobs live. No new byte parity or coverage totals claimed.

Next recover main allocation/dispatch/tracking entry points rather than treating direct size-class tests as full memory API recreation. work/inspect_main_allocator_operations.py executed, output work/main_allocator_operations_disassembly.txt. Retail InternalAllocate009F2A40 (122 bytes,ret12) adds20 guard/tracking bytes to signed-clamped size>=1, selects min(total,MaxFixedAllocationSizes[category]+1) lookup, virtualAllocate(total), stores rowID at allocatedData-1 outside special memory range013CAAA4..013CAAA8, increments main NoAllocations, returns allocatedData+4 after real InitDebugInfo009F28D0. Donor bool third parameter evidenced FableWin030BD540/Ego_r00B467F0; bool passed to InitDebugInfo, shipping body does not read it. InitDebugInfo FableWin030BD820 ABI(void*,bool,long): two main GetAllocationSize009F2390 calls, trailing20-byte metadata{ffffffff,currentTrackingID,requestedSize,...}, leading userData-4 sentinelffffffff, increments perTrackingID requested bytes.

InternalDeallocate009F2920 (155 bytes,ret4) chooses rowID from (pointer-rangeStart)>>24 inside special range or userData-5 otherwise; range checks NoAllocators, repeats main GetAllocationSize twice, decrements tracking bytes from trailing metadata, virtualGetAllocationSize(userData-4), fills complete class payloadCD, virtualDeallocate(userData-4), decrements main NoAllocations. Main GetAllocationSize009F2390 itself not yet authored in fable_memory_allocator.h/providers; next inspect its full native body and donor names for special range globals. InitDebugInfo/InternalAllocate/InternalDeallocate likewise no production source yet.

Retail global aligned wrappers009F2BE0/009F2B20 (manifest AllocateAligned/DeallocateAligned) must not be mislabeled as plain static CMemoryAllocator Allocate/Deallocate: AllocateAligned(sizeECX,alignmentEDX) lazycreates singleton, holds critical section around InternalAllocate(size+alignment+3,category0,false), then computes aligned user pointer and stores original main pointer at aligned-4. DeallocateAligned(pointerECX) nullfastpath, loads original pointer at pointer-4 before entering lock, calls InternalDeallocate, may perform deferred destruction and singleton release while holding lock, then leaves lock. Full disassembly preserved, source recovery/behavior still pending. Public static mainAllocate with debugargs009F2B90 routes009F2AC0 wrapper, which includes compiler security-cookie check and duplicate debug metadata setup; do not conflate with aligned wrapper. Broader game recreation incomplete. Preserve unrelated work below.

# Native fixed allocation/lifetime checkpoint - 2026-10-05

Native recreation remains active until stopped; Lua/FMP excluded. Promoted CMemoryAllocatorFixedSize_Allocation_00a662a0.cpp with both remaining native fixed virtuals. Allocate0x00A66450 NEAR_MATCH252/250 bytes,82/81 instructions,0.993865 ordered similarity; Deallocate0x00A662A0 NEAR_MATCH246/242 bytes,86/85 instructions,0.900585 similarity; three original helper relocations each. Genuine C++ shared allocator/chunk/area layout, real original heap/chunk/list providers. Added donor const GetPPreviousArea/GetPParentChunk declarations, corroborated FableWin PDB names033C37C0/033C3700 and retail inlined lowering. Stop byte tuning: passing near match and behavior. Registered manifest/artifact index/corrections/symbol index and both gates in gate_all.

gate_fixed_allocation.py passes54 continuous retail/native sequences/1,668 operations plus18 forced-failure and12 signed-counter cases:1,698 total operations/1,264 heap boundary calls/five provider omissions. Independent membership/state predictions compare every32-KiB prestate/final state, native state persists between operations. Align4/8/16,stride16/64,count1/3/7,initial0/1/3 forward/reversed chunks,null/head/tail current, two cycles/forward-reverse-permuted frees. Size ignored incl0/ffffffff; full chunks move tail, previously-full freed chunks move head; byte links/free-head stale previousID, current selection and empty counter preserved. Counter negative/zero/positive/max/wrap cases match retail signed release decision. Real NewChunk/CreateChunk/DeleteChunk/list helpers and native shipping failure handler execute; only heap imports are recorded. NullHeapAlloc/zeroHeapSize/unsigned64overflow failure cases: absenthead faults at address0x10 via actual native SEH; surviving full head returnsnull and remains current. Direct invalid alignment0 used only to force overflow in zero-slack raw contexts, no constructor support claim. Report fixed-allocation.json.

gate_fixed_os_lifetime.py passes72 real Windows process-heap scenarios/3,465 allocate-free operations/3,537 invariant checks;24 genuine providers and24 required-provider omissions. Actual unmodified HeapAlloc/HeapSize/HeapFree imports, real constructor and complete fixed vtable providers; align4/8/16,areas16/60,minimum128/256,freeordersforward/reverse/permuted,two cycles or live allocations at destruction. Payload preserved/aligned/nonoverlap, OS extents/wholeheap valid, independent area partition and chunk/area links, live counts/free counts/sizes/totals/header overhead/enumeration/index queries, native validation/consolidation and signed empty transitions. Original destructor changes only vtable and preserves allocated chunks and payload:144 retained chunks independently confirmed, explicitly freed by fixture after destruction. GetFragmentation provider linked but its absolute retail constant not exercised. No forced OS failure, deleting-thunk or main singleton construction claim. Report fixed-os-lifetime.json.

Both production gates replay after promotion. Narrow fixed constructor/dtor fixture comments/report now correctly describe five deliberate virtual boundaries rather than unrecovered methods; its30-case/203-byte/10-leaf/16-slot/four-omission regression passes. Index33,562 symbols. Fresh closure1,276 sources/zero duplicates/422 unresolved at work/codex-native-startup/native-gfmain-fixed-allocation-closure-20261005: no fixed or variable allocator frontier remains. Broader world/UI/game recreation incomplete. No jobs live, no manual global coverage edits.

Next native work: compose main CMemoryAllocator constructor/initialization with all now-real fixed and variable providers and actual OS lifetime. Existing gate_main_allocator_constructor.py still uses narrow virtual/chunk boundaries and recorded OS seams; do not claim full singleton boot from individual allocator tests. Main ctor009F20B0, destructor009F2310, InitializeMemorySystem009F1D20 and30-row MemoryAllocatorData01396868 already genuine. Public static CMemoryAllocator Allocate/Deallocate/InternalAllocate/InternalDeallocate have PDB names but no named symbol-index providers located yet; distinguish their recovery from the completed per-class allocators. Preserve original main destructor behavior: class-specific operator delete on allocator objects, without invoking derived destructors. Keep unrelated work below.

# Native fixed allocation candidate checkpoint - 2026-10-05

Native recreation remains active until stopped; Lua/FMP excluded. Latest production checkpoint immediately below remains authoritative: fixed InternalAlloc/NewChunk landed and fully gated; closure1,275 sources/zero duplicates/424 unresolved.

Next candidates are work/FixedAllocation_candidate.cpp, checked by python work/check_fixed_allocation.py. Allocate0x00A66450 NEAR_MATCH252/250 bytes,82/81 instructions,0.993865 ordered similarity/three original helper call relocations. Deallocate0x00A662A0 NEAR_MATCH246/242 bytes,86/85 instructions,0.900585 similarity/three original helper call relocations. Both genuine C++ use original shared allocator/chunk/area layout; added protected donor GetPPreviousArea/GetPParentChunk declarations. Existing GetPNextArea const ABI retained. No production source or registry promotion for these virtuals yet; behavior fixtures required. Stop byte tuning: both clear existing near-match thresholds.

Allocate ignores requested size, selects current/head or real NewChunk, pops free area/prepends allocated ID list, decrements signed empty count only on zero-to-one transition, moves full chunk to tail, resets current to head. Failed NewChunk may leave null head subsequently dereferenced; preserve retail failure behavior. Empty free list with otherwise selected chunk returns null without resetting current. Deallocate derives parent from byte AreaID/stride/header, patches allocated neighbors and prepends free list; freed area's PreviousAreaID intentionally remains stale. A formerly full chunk moves to head if not already there; zero allocation increments signed empty count and deletes when >1. DeleteChunk does not decrement empty count. Need full-state retail/native allocation/free composition and actual OS lifetime before promotion, with required-provider omissions. Reuse the already-landed NewChunk/CreateChunk/DeleteChunk/list providers rather than stand-ins. Native failure seam already has1,080 focused retail cases plus42 OS scenarios. No jobs live. Preserve unrelated work below.

# Native fixed heap acquisition checkpoint - 2026-10-05

Native recreation remains active until stopped; Lua/FMP excluded. Promoted CMemoryAllocatorFixedSize_NewChunk_00a65f40.cpp: InternalAlloc0x00A65F40 MATCH100 bytes/3 original IAT relocations; NewChunk0x00A663D0 MATCH122 bytes/4 relocations. Genuine C++ unsigned64 overflow check and heap acquisition, real recovered CreateChunk, original inlined head insertion. Implementation-only InsertChunkAtListHeadInline avoids strong-helper duplication. Both registered in manifest/artifact index/corrections, symbol index and gate_all; no manual coverage totals.

gate_fixed_new_chunk.py passes1,080 independently predicted full32-KiB retail/native cases and42 real process-heap scenarios/84 allocations and releases, five required-provider omissions. InternalAlloc alignment slack rounded to8; overflow leaves caller output untouched and invokes no APIs; HeapAlloc writes output including null; nonnull allocation returns exact HeapSize including0/ffffffff. GetProcessHeap import cached across calls, tested by replacing IAT after first callback. Heap flags1 and requested bytes checked. Callback owner alignment/shift/header/stride/count/list/empty-counter/singleton mutations honored. NewChunk branches on returned size, not allocated pointer; HeapSize0 invokes failure callback without invented free. Real CreateChunk in both modes; OS mode invokes actual GetProcessHeap/HeapAlloc/HeapSize and native shipping failure handler, then DeleteChunk/HeapFree and whole-heap validation. Complete initialized block and untouched payload independently predicted across align4/8/16, count0/1/3/254/255/256/257, stride16/64 and two-chunk lists. Raw owner context: full fixed virtual Allocate/Deallocate lifetime remains pending. Report rebuild/compile-gate/fixed-new-chunk.json.

Production gate replay passes after promotion. Original fixed allocator lifetime regression passes30 cases/203 bytes/10 leaves/16 slots/four omissions. Index33,557 defined symbols. Fresh closure1,275 sources, zero duplicates,424 unresolved at work/codex-native-startup/native-gfmain-fixed-new-chunk-closure-20261005. No jobs live. Remaining fixed frontier is virtual Allocate0x00A66450 and Deallocate0x00A662A0; proceed there next. Preserve original failure/counter behavior (Allocate can dereference null head after failed NewChunk; DeleteChunk does not decrement empty count). Preserve unrelated work below.

# Native fixed new-chunk candidate checkpoint - 2026-10-05

Native recreation active until stopped; Lua/FMP excluded. Recovered scratch genuine C++ work/FixedNewChunk_candidate.cpp. InternalAlloc0x00A65F40 MATCH100 bytes/three relocations; NewChunk0x00A663D0 MATCH122 bytes/four relocations. VC7.1 check work/check_fixed_new_chunk.py records full parity in work/fixed_new_chunk/parity.json. No production source promotion, manifest registration or closure additions yet: meaningful candidate-required behavior fixture is still required.

Original InternalAlloc: extra=(Alignment-1)&~7UL; unsigned64 addition of extra+size rejects values above ULONG_MAX without touching output pointer or calling heap APIs. Otherwise *memory=HeapAlloc(GetProcessHeap(),1,size+extra); null writes null and returnszero; nonnull calls HeapSize(GetProcessHeap(),1,*memory) and returns its exact value (including0/FFFFFFFF). GetProcessHeap import function pointer is cached between both calls in retail. Original NewChunk: InternalAlloc(ChunkSize,&memory), if returned size nonzero call real CreateChunk(memory), inline original head insertion, increment signed NoEmptyChunks, return chunk. On zero call current singleton OnFailedAllocation and returnnull. No added cleanup/fallback; a nonnull allocation with HeapSizezero follows original failure route. This is distinct from checking allocated pointer.

Added only shared protected declarations InternalAlloc(ulong,void**), NewChunk(), and implementation-only InsertChunkAtListHeadInline to avoid VC7.1 collision with existing strong list helper. NewChunk exact code includes original inline head insertion and real external CreateChunk. Header has no layout/virtual-slot change. Existing fixed lifetime regression remains30 cases/203 bytes/ten leaves/four omissions. Source and PDB ABI match original protected methods. No jobs live.

Next required before promotion: permanent retail/native fixture recording GetProcessHeap/HeapAlloc/HeapSize and OnFailedAllocation, checking args/order/full snapshots/output-pointer preservation and mutation. Cover unsigned overflow thresholds, null allocation, HeapSizeresults0/ordinary/FFFFFFFF, callback owner/alignment/list changes and cached heap-function-pointer behavior. Compose real recovered CreateChunk, then actual process-heap success paths. Register only after passing behavior, index and fresh closure. Latest authoritative production closure remains1,274 sources/zero duplicates/424 unresolved at work/codex-native-startup/native-gfmain-fixed-chunk-lifetime-closed-20261005. Remaining Allocate/Deallocate still unlanded. Preserve independent work below.

# Native fixed chunk lifetime checkpoint - 2026-10-05

Native recreation active until stopped; Lua/FMP excluded. Landed CMemoryAllocatorFixedSize_ChunkLifetime_00a66160.cpp: DeleteChunk0x00A66160 MATCH105 retail bytes/two process-heap import relocations; CreateChunk0x00A661D0 NEAR_MATCH139/137 bytes,49/49 instructions,0.918367 similarity/no relocations. Genuine C++ uses original class/layout and generic aligned pointer/area initialization. Added protected CreateChunk/DeleteChunk/InternalDealloc and static fastcall CMemoryArea::Create(void*,uchar) declarations with donor ABI. Stop byte tuning at passing near match.

gate_fixed_chunk_lifetime.py passes1,344 complete32-KiB retail/native cases:288 creation/1,056 release, required source omission. Alignment4/8/16,unaligned raw pointers,stride16/64,counts0/1/3/7/254/255/256/257; raw memory pointer retained, chunk fields zeroed, four-byte area headers initialized ascending and prepended into reverse-ID free list; ID wrap and ff sentinel preserved. Independent expected writes verify untouched payload and owner storage. DeleteChunk unlinks head/middle/tail/singleton and clears current only if equal, preserves removed links and empty counter. GetProcessHeap/HeapFree callbacks check original heap/flags1/raw-memory argument and complete prestates; callback mutates stored raw pointer/current/empty counter after original argument load; free result ignored. Recorded boundary does not claim actual OS release. Report fixed-chunk-lifetime.json.

Added FixedChunkOS_test.cpp/gate_fixed_chunk_os.py:84 real Windows process-heap scenarios/84 actual releases, two provider omissions. Actual HeapAlloc and recovered CreateChunk/head insertion/DeleteChunk, thin import wrappers validate arguments then invoke real GetProcessHeap/HeapFree. Complete heap-block initialization independently predicted, including ID boundary counts; actual free requires success, whole process heap validated after every release; owner list/current/NoEmptyChunks preserved as original. Raw allocator context, so this does not complete fixed constructor/virtual Allocate/Deallocate lifetime. Report fixed-chunk-os.json. Both gates integrated in gate_all.

First combined link/closure exposed duplicate RemoveChunkFromList emitted by VC7.1 inline lowering alongside dedicated strong helper. Added implementation-only RemoveChunkFromListInline name for the original inlined DeleteChunk body; no assembly/algorithm change. Rechecked exact DeleteChunk, CreateChunk near match and all1,344 cases,84 OS cases. Final index33,553 symbols. Final corrected closure1,274 sources, zero duplicates,424 unresolved at work/codex-native-startup/native-gfmain-fixed-chunk-lifetime-closed-20261005. Older ...fixed-chunk-lifetime-closure... output has one duplicate and is superseded; do not use it. Regression original fixed lifetime still30 cases/203 bytes/ten leaves/four omissions; standalone list76 cases passes. No jobs live; no manual coverage totals.

Source/fixture/oracle/gates indexed and registered in manifest/artifact index/corrections. Next recover InternalAlloc0x00A65F40 and NewChunk0x00A663D0, then virtual Allocate/Deallocate. Existing chunk creation/release now close their original dependencies; native process heap differs from variable allocator VirtualAlloc/VirtualFree. Preserve signed long NoEmptyChunks and original counter/failure behavior. Broader singleton startup/game work remains incomplete. Preserve independent work below.

# Native fixed-size chunk-list checkpoint - 2026-10-05

Native recreation active until stopped; Lua/FMP excluded. Landed CMemoryAllocatorFixedSize_ChunkLists_00a65e70.cpp with three genuine methods: RemoveChunkFromList0x00A65E70 exact69 bytes, InsertChunkAtListHead0x00A65EC0 exact58 bytes, InsertChunkAtListTail0x00A65F00 exact58 bytes;185 total/no relocations. Corrected misleading propagated DeleteChunk/AddWindow labels with Ego_r/FableWin PDB names. Actual removal patches neighbor links and owner head/tail but deliberately retains removed chunk's own Next/Previous pointers. Head/tail insertion overwrites inserted links and sets both endpoints for an empty chain; current allocator chunk and counters are untouched.

Permanent oracle_fixed_chunk_lists.py independently predicts all authorized writes and validates resulting membership/head/tail/reciprocal links; gate_fixed_chunk_lists.py/FixedChunkLists_test.cpp pass76 complete32-KiB retail/native cases and required source omission. Empty/singleton/three/seven chains, forward/reversed orders, every removal position and head/tail insertion; unrelated storage unchanged. Report fixed-chunk-lists.json. Source/fixture/oracle/gate indexed and registered in gate_all/manifest/artifact index/corrections.

Shared NoEmptyChunks corrected unsigned long -> long from Ego_r struct_layouts line165905, original+C48; no layout/virtual-slot change. This matters for deallocation's signed >1 test. Regression gates pass fixed lifetime30 cases/203 bytes/ten leaves/four omissions, enumeration1,800 cases/179 bytes/two omissions, validation252 cases/96 bytes/five omissions. Index completed33,547 defined symbols. Completed closure1,273 sources, zero duplicates,424 unresolved at work/codex-native-startup/native-gfmain-fixed-chunk-lists-closure-20261005. No jobs live; no manual coverage edits.

Next recover actual fixed DeleteChunk0x00A66160 and CreateChunk0x00A661D0, then NewChunk0x00A663D0 and Allocate/Deallocate. Direct retail disassembly obtained this turn: DeleteChunk clears PCurrentAllocatorChunk only when equal, inlines RemoveChunkFromList, then calls HeapFree(GetProcessHeap(),1,chunk->ThisChunkMem). IAT independently resolved with pefile:0x0143FD3C=GetProcessHeap,0x0143FE8C=HeapFree. Fixed chunks use process heap release, NOT the variable allocator's VirtualFree. DeleteChunk does not decrement NoEmptyChunks; preserve original behavior. CreateChunk aligns supplied raw memory using Alignment/AlignmentShift, stores ThisChunkMem and initializes ID-linked free areas at ChunkHeaderSize in ascending ID loop (prepends, so list order reverses). Recover exact original helper ABIs before promotion. Remaining fixed virtual frontier Allocate/Deallocate and native singleton startup/game work still incomplete. Preserve independent work below.

# Native fixed-size validation checkpoint - 2026-10-05

Native recreation active until stopped; Lua/FMP excluded. Landed CMemoryAllocatorFixedSize_Validate_00a66260.cpp with protected Validate(chunk)0x00A66260 exact62 bytes/two relocations and virtual Validate()0x00A663A0 exact34 bytes/one relocation. Both genuine C++ use original shared four-byte area-ID layout and const GetPNextArea helper. Protected chunk overload ABI corrected from generic one-parameter manifest using FableWin PDB0x033C35E0 and Ego_r0x00C08180. Virtual traversal delegates to this protected method; each allocated data pointer calls current singleton InternalValidateArea. Shipping internal validator remains recovered genuine no-op, separate translation unit preserves calls.

Permanent gate_fixed_validate.py passes252 complete32-KiB retail/native cases:84 using actual shipping no-op and168 recorded boundary cases/368 calls, five required-provider omissions (three native/two recorded). Empty/one/three/seven chunks, reversed chunks, empty/single ID254/sparse forward-backward ID chains; whole allocator and selected-chunk entry points. Callback on first call terminates current area chain, skips next chunk and switches singleton, proving actual post-call link/context reloads. Entire call/final snapshots and call this/data/order checked; protected null chunk remains outside retail precondition. Report fixed-validate.json. Oracle reuses independently constructed enumeration seed lists, executes original retail validation bodies and independently predicts callback-adjusted traversal.

Source/fixture/oracle/gate indexed and registered in gate_all/manifest/artifact index/corrections. Header added only protected overload (no virtual slot/layout change). Regression checks: original fixed lifetime gate passes30 cases/203 bytes/ten leaves/sixteen slots/four omissions; fixed enumeration remains1,800 cases/179 exact bytes/two omissions. Index completed33,544 defined symbols. Completed closure1,272 sources, zero duplicates,424 unresolved at work/codex-native-startup/native-gfmain-fixed-validate-closure-20261005. No jobs live; no manual coverage totals.

Remaining fixed virtual frontier Allocate0x00A66450 and Deallocate0x00A662A0. Recover original chunk helpers first: RemoveChunkFromList0x00A65E70, InsertChunkAtListHead0x00A65EC0, InsertChunkAtListTail0x00A65F00 (manifest currently propagated DeleteChunk/AddWindow labels); actual DeleteChunk0x00A66160, CreateChunk0x00A661D0, NewChunk0x00A663D0, InternalAlloc0x00A65F40/InternalDealloc needed. Exact PDB names in Ego_r/FableWin already located. IMPORTANT: Ego_r struct_layouts NoEmptyChunks is long at+C48 (line165905), shared header currently unsigned long; correct signed type before recovering deallocation's signed >1 empty-chunk test and verify dependent parity. Keep original failure behavior; Allocate calls NewChunk then dereferences head without added invented fallback. Saved disassembly work/fixed_operations_disassembly.txt. Native singleton startup and broader game reconstruction remain incomplete. Preserve independent work below.

# Native fixed-size enumeration checkpoint - 2026-10-05

Native recreation active until stopped; Lua/FMP excluded. Landed CMemoryAllocatorFixedSize_Enumeration_00a660a0.cpp: GetAllAllocatedAreas0x00A660A0 exact89 bytes, GetAllocatedArea0x00A66100 exact90 bytes, no relocations. Genuine C++ uses shared candidates/fixed_size_allocator_accessors_compiled.h hierarchy. Added original nested CMemoryArea four-byte layout AreaID/NextAreaID/PreviousAreaID/AllocatorID from Ego_r struct_layouts, and protected const GetPNextArea(CMemoryArea*) from FableWin symbol0x033C3750. Helper returns null for NextAreaID255, otherwise pointer + signed byte-ID delta * StatAreaSize. Backward links supported. Bulk ignores max, always calls virtual GetNoAllocatedAreas before traversing and ignores its return. Index method makes no count call and returns null for missing/negative index.

Permanent gate_fixed_enumeration.py passes1,800 complete32-KiB retail/native cases,188 bulk cases using actual recovered count and two provider omissions. Empty/one/three/seven chunks; empty/single ID254/sparse1->7->3 lists, ff sentinel, forward/backward stride, reversed chunks,32/64 strides, zero/large max, every valid index and negative/out-of-range values. Native ABI-compatible count thunk validates call prestate, uses actual GetNoAllocatedAreas with unrelated wrapping metadata or recorded count return; mutation mode changes owner head/stride before enumeration. Pointer order/output writes/final state all match retail. Only count-vtable binding cell normalized after validating real native thunk address. Report fixed-enumeration.json. Source/fixture/oracle/gate registered in index/gate_all/manifest/artifact index/corrections.

Shared header addition regression: gate_fixed_allocator_lifetime.py still passes30 retail/native cases,203 constructor/destructor/thunk bytes,ten leaves,sixteen slots and four provider omissions. Full native closure recompiled with header:1,271 sources, zero duplicates,425 unresolved at work/codex-native-startup/native-gfmain-fixed-enumeration-closure-20261005. Index completed33,540 defined symbols. No jobs live; no manual coverage edits.

Next three fixed-size frontier methods are Allocate0x00A66450, Deallocate0x00A662A0, Validate0x00A663A0. Retail evidence saved by work/inspect_fixed_operations.py to work/fixed_operations_disassembly.txt. Allocate ignores requested size, obtains/creates a current chunk, moves first free ID-linked area into allocated list, updates NoEmptyChunks and reorders full chunks. Deallocate derives parent chunk from byte AreaID/StatAreaSize/ChunkHeaderSize, unlinks allocated area and prepends free list, reorders first newly-free chunk and deletes excess empty chunks. Validate traverses chunks and calls protected Validate(chunk)0x00A66260. Need recover their exact native chunk/list/create/delete/OS dependencies and PDB ABIs; do not substitute fitted algorithms or sanitize retail failure behavior. Fixed singleton startup and broader engine work remain incomplete. Preserve independent work below.

# Native variable allocator OS lifetime checkpoint - 2026-10-05

Native recreation active until stopped; Lua/FMP excluded. Added permanent VariableOSLifetime_test.cpp and gate_variable_os_lifetime.py, using33 genuine recovered native providers and actual Win32 VirtualAlloc/VirtualQuery/GlobalMemoryStatus/VirtualFree. No recording import replacements or algorithm substitutes. Production source unchanged; gate_all includes this actual-OS integration. Provider set reuses composition source paths via AST parsing without executing the composition CLI, plus constructor/chunk/virtual-entry/destructor/base lifetime sources; report records exact33 sources.

PASS54 scenarios,1,215 allocation/free operations,1,269 independent invariant checks,135 explicitly counted last-allocation/owner release checks, all33 required-provider omissions. All supported constructor alignments4/8/16; empty heap, OS-owned Permanent initial chunk, caller-owned DemandPaged/Permanent initial memory; forward/reverse/permuted frees or destruction with live allocations. Requests0..16,384 force multiple real OS chunks. Actual virtual allocation/deallocation, constructor, Init, lists, split/coalescing, insertion/deletion/fixup/rotations, size/count/totals/enumeration and deleting destructor all composed. Payload bytes preserved and nonoverlapping; area extent bounds/nonoverlap and data/header conservation; list links/counts/extrema; red-black membership/order/parents/colors/black depth; independent live list vs virtual queries.

VirtualQuery confirms committed chunks while live, release after final allocation for nonpermanent OS chunks, owner release after deleting destruction, every recorded OS chunk released at destruction, caller-owned supplied memory retained until explicit caller cleanup. Report variable-os-lifetime.json grade NATIVE_OS_LIFETIME_PASS. This successful read/write OS composition complements separate byte parity and retail replay evidence; it does not claim forced OS-failure paths or main singleton construction/validation/startup complete.

First fixture incorrectly included alignments32/64/128 and timed out. Retail/shared base constructor explicitly supports4/8/16; default AlignmentShift=9999999. Larger alignment with a zero-size request can stall NewChunk retry at zero. Corrected fixture domain to original supported constructor inputs, preserving original code. Earlier standalone query tests with manually supplied valid shifts remain valid but do not expand constructor support. Final run after adding extent checks passes; no jobs live.

Production closure remains1,270 sources/zero duplicates/427 unresolved; no index/closure rebuild needed for fixture-only changes. No manual coverage edits. Next recover five fixed-size allocator methods in the native closure frontier and strengthen singleton startup/lifetime. Broader CWorld/UI/game reconstruction remains incomplete. Preserve independent work below.

# Native destructor tree composition checkpoint - 2026-10-05

Native recreation active until stopped; Lua/FMP excluded. Added --native-tree/FABLE_NATIVE_TREE to the permanent destructor gate/oracle/fixture. Actual recovered RemoveFromTree, RemoveFromTreeFixup and their native rotations now execute inside genuine DeleteChunk during scalar/deleting destruction. No production source changes.

Strong gate passes960 full32-KiB cases/6,984 recorded OS calls, with all seven required native-provider omissions. Retail constructs red-black trees using actual insertion, then executes original deletion/fixup. Initial insertion order alternates forward/reverse; signed/high-bit/equal keys included. Independent oracle checks exact membership, signed(size,address) order, root black, parent links, black depth, no red-red/cycles, removed tree links cleared, and final root empty. Checks run before each removal and at OS boundaries; full linked snapshots match retail at every OS call and final state. Other bytes including list links, ID, data sizes and surrounding storage preserved except explicitly authorized destructor/list/OS-callback changes.

Existing destructor tests remain: empty/one/three/seven chunks, Permanent/DemandPaged combinations, status callback DemandPaged flip and owner/global alignment mutation, once-only alignment initialization, two status calls per chunk sharing buffer, ignored VirtualFree result, scalar and deleting flags0/1/2/3, owner return and optional owner release. Owner vtable pointer validated before normalizing only that cell. Native deletion/fixup retain existing passing near-match grades, no byte tuning. Other unused virtuals abort if invoked.

Report rebuild/compile-gate/variable-destructor-native-tree.json; gate_all requests stronger mode. Legacy recorded-tree mode also passes960 cases/8,584 boundary calls and five provider omissions. No jobs live. OS calls are still recorded, so actual OS lifetime remains unproven. Production closure unchanged1,270 sources/zero duplicates/427 unresolved; no new index or closure rebuild needed for this fixture-only composition.

Next move toward real native allocator lifetime: build a real chunk through recovered allocation/initialization and execute allocation/free/destruction with actual native trees; constrain OS boundary claims to what is genuinely exercised. Fixed-size allocator/native singleton startup and broader engine frontier remain outstanding. Preserve independent work below.

# Native destructor promotion checkpoint - 2026-10-05

Native recreation active until stopped; Lua/FMP excluded. Landed CMemoryAllocatorVariableSize_Destructor_00a65690.cpp as genuine C++: scalar destructor0x00A65690 exact74 bytes/four relocations; compiler deleting thunk0x00A65C60 exact28 bytes/two relocations. Original derived vtable installed, alignment set once, DeleteChunk(PFirstChunk) repeated until empty, including Permanent chunks. Deleting thunk flags&1 releases allocator object via actual native class-specific delete and returns owner; vector-deleting vtable name weak-aliases scalar thunk.

gate_variable_destructor.py passes960 complete32-KiB cases/8,584 recorded boundary calls and five required native-provider omissions. Empty/one/three/seven chunk chains, zero/single/mixed free lists and removal order, alignments4/128, Permanent/DemandPaged combinations, status callback flips DemandPaged and changes owner/global alignment, ignored VirtualFree success/failure, two status calls per chunk sharing a buffer, scalar and deleting flags0/1/2/3. Every call prestate and full final state checked. COFF checks16 vtable bindings; owner pointer must equal linked derived vtable before normalizing only its four-byte cell to original0x0129DB1C.

Real recovered DeleteChunk and class-specific operator delete linked. Tree removal/GlobalMemoryStatus/VirtualFree are recorded seams; no actual OS release or tree mutation claim from this focused destructor gate. Import slot redirection is confined to test object copies after original relocation parity checks. Other virtuals abort if invoked. Permanent fixture VariableDestructor_test.cpp, oracle_variable_destructor.py and report variable-destructor.json.

Source/gate indexed and registered in gate_all/manifest/artifact index/corrections. Corrected misleading CDrawGuildSeal::ClearMessages and CEngineSubPrimitiveGameMesh deleting-destructor labels at these addresses. Existing generic sources remain preserved for unrelated catalog evidence. Actual0x00BE0A50 is unrelated ref-counted container erase and was not used. Both donor PDBs and original vtable/thunk independently establish actual destructor ABI.

Index completed33,537 defined symbols. Completed closure1,270 sources, zero duplicates,427 unresolved at work/codex-native-startup/native-gfmain-variable-destructor-closure-20261005. No remaining CMemoryAllocatorVariableSize symbol in this closure frontier. No jobs live; no manual coverage totals.

Next strengthen destructor with real recovered tree deletion/fixup and live allocated/free chunks, then move through fixed-size allocator/native heap lifetime frontier. This focused gate records tree and OS calls; it does not complete native heap lifetime or the game. Existing nineteen-provider allocation/free/query composition remains passing. Preserve independent work below.

# Native destructor recovery checkpoint - 2026-10-05

Native recreation active until stopped; Lua/FMP excluded. Recovered original variable allocator destructor in scratch genuine C++ work/VariableDestructor_candidate.cpp. Retail destructor is0x00A65690,74 bytes/four relocations: install derived vtable0x0129DB1C, set global alignment, repeatedly DeleteChunk(PFirstChunk) until empty. Permanent does not guard destructor chunk deletion. SetCurrentAlignment occurs once before the loop; subsequent callbacks can mutate globals. Real DeleteChunk0x00A65390 is already recovered and unlinks lists, removes each free area from tree, invokes GlobalMemoryStatus twice, releases non-demand-paged chunks via VirtualFree regardless of Permanent.

Compiler also emits scalar deleting thunk0x00A65C60,28 bytes/two relocations: call destructor, if flags&1 invoke class-specific operator delete0x00A66750, return owner. Vtable slot15 uses weak vector-deleting alias of this scalar thunk. Both candidate bodies MATCH exact retail bytes (102 total); work/check_variable_destructor.py compiles VC7.1, verifies full bodies/relocations and records work/variable_destructor/parity.json. Scalar normalized similarity0.95 despite exact patched bytes (different relocation/raw alignment normalization); grade MATCH authoritative. Both donor PDBs name destructor (Ego_r0x00C07710, FableWin0x033BF300); constructor gate independently proves vtable slot15 retail target0x00A65C60, and retail thunk directly calls0x00A65690.

Misleading propagated labels: manifest0x00A65690 says CDrawGuildSeal::ClearMessages;0x00A65C60 says CEngineSubPrimitiveGameMesh deleting destructor and points to a byte-identical generic source. Actual0x00BE0A50 misleadingly named CMemoryAllocatorVariableSize_Erase_Destructor is an unrelated ref-counted container erase, not allocator destruction. Do not pick this from the label. Evidence: work/inspect_variable_destructor.py disassembles all three actual bodies. None of these label corrections or candidate promotions performed yet.

Next REQUIRED before promotion: add permanent candidate-required retail/native destructor fixture with actual recovered DeleteChunk. Record tree removal and OS seams, compare complete32-KiB call/final states (normalize only validated native owner vtable). Cover empty/one/three/seven chunk chains, free-area removal order, Permanent/DemandPaged combinations, callback flip of DemandPaged and changed globals, VirtualFree failure ignored, two status calls per chunk using same buffer. Exercise generated deleting thunk flags0/1/2/3 (fastcall test adapter ECX owner/unused EDX/stack flags is ABI-compatible), optional allocator-object release and returned owner. Validate vtable16 bindings via COFF and native address before normalization. Then register destructor and thunk addresses/correct labels, index and fresh closure. Existing production closure remains1,269 sources/zero duplicates/428 unresolved; no jobs live. Preserve independent work below.

# Native allocator validation checkpoint - 2026-10-05

Native recreation active until stopped; Lua/FMP excluded. Landed CMemoryAllocatorVariableSize::Validate0x00A65330 (exact96 bytes/six relocations) and typed CMemoryAllocator::InternalValidateArea0x009F1F40 (exact3 bytes, ret4). Public void InternalValidateArea(void*) independently identified by Ego_r PDB0x00B45AC0 and FableWin PDB0x030BF4C0. Added declaration to shared fable_memory_allocator.h, corrected generic E2 manifest/artifact/override to the genuine method. Old generic source remains unused in this closure; preserve unrelated catalog evidence.

Validate sets alignment and walks every allocated area in chunk/list order, calls the current singleton with aligned data pointer, then reloads list links/global alignment and singleton. Shipping InternalValidateArea is genuinely a no-op; its separate translation unit preserves the original calls. Both use genuine C++, no baked instructions.

gate_variable_validate.py passes864 complete32-KiB cases:288 with actual recovered shipping no-op,576 recorded boundary cases/1,104 recorded calls. Six alignments, empty/one/three/seven chunks, empty/single/mixed/reversed lists, stale globals and unrelated counts. Recorded mode checks this/data/order and entire call prestate; first callback mutates owner alignment128/7, globals64/6 and singleton, proving fresh globals/singleton for later calls. Seven required-provider omissions (four native/three recorded). Report variable-validate.json. Source/fixture/oracle/gate indexed and registered in gate_all/manifest/artifact index/corrections.

Optional --validate/FABLE_VALIDATE adds actual native validation after each real allocation/free. Strong mode --free-cycles --size-queries --largest-free --fragmentation --enumeration --validate passes192 sequences/4,068 operations (2,400 allocations/1,668 frees),3,336 size queries,4,068 largest-free/fragmentation/bulk enumeration/validation queries and37,446 indexed lookups. All nineteen native-provider omissions fail linking. Full live heap, globals and singleton preserved. Report variable-composed-validation.json; gate_all requests this mode. Regression mode without --validate also passes4,068 operations and sixteen provider omissions. Raw-storage replay does not cover OS acquisition/destruction or virtual allocation entry.

Index completed33,533 defined symbols. Completed closure1,269 sources, zero duplicates,428 unresolved at work/codex-native-startup/native-gfmain-variable-validate-closure-20261005. Header addition also compiled throughout this closure. No manual coverage totals. No jobs live.

Next recover variable allocator destructor, the remaining unresolved variable allocator method in this closure. Trace its vtable slot/native address and donor ABI, then check native chunk/OS lifetime with required-provider fixtures. Fixed-size allocator and broader CWorld/UI frontier remain incomplete. Preserve independent work below.

# Native enumeration checkpoint - 2026-10-05

Native recreation active until stopped; Lua/FMP excluded. Landed genuine C++ CMemoryAllocatorVariableSize_Enumeration_00a65260.cpp: GetAllAllocatedAreas0x00A65260 exact108 bytes/four relocations and GetAllocatedArea0x00A652D0 exact96 bytes/two relocations. Both set alignment and traverse chunk/allocated-list order. Bulk max argument is unused even when zero; caller must supply actual capacity. Global zero-based lookup returns null for negative/out-of-range indices. Cached counts and sizes do not control enumeration. GetDataStart uses aligned32-byte area header; shared class declarations retained.

Permanent gate_variable_enumeration.py passes3,168 complete32-KiB retail/native cases (1,176 bulk/1,992 indexed), six alignments, empty/one/three/seven chunks, empty/single/mixed lists, reversed order, signed-boundary metadata, zero/insufficient/large max and every valid index plus misses. Exact output writes and untouched surrounding storage; two required-provider omissions. Report variable-enumeration.json. Source/fixture/oracle/gate registered in manifest/artifact index/corrections/index/gate_all.

Added optional --enumeration/FABLE_ENUMERATION to real allocation/free composition. Independently derives expected pointers from actual live lists, executes retail bulk/index bodies, verifies output tail and full heap. Native calls all real recovered methods. Strong mode --free-cycles --size-queries --largest-free --fragmentation --enumeration passes192 sequences/4,068 operations (2,400 allocations/1,668 frees),3,336 size queries,4,068 largest-free,4,068 fragmentation,4,068 bulk enumerations and37,446 indexed queries. All sixteen native-provider omissions fail linking. Report variable-composed-enumeration.json; gate_all now requests this mode. No OS acquisition/destruction or virtual allocation entry claim from this raw-storage replay.

Index completed33,527 defined symbols. Completed closure1,267 sources, zero duplicates,429 unresolved at work/codex-native-startup/native-gfmain-variable-enumeration-closure-20261005. No manual coverage totals.

Regression composition without --enumeration passes4,068 operations and fifteen provider omissions. No jobs live. Next native allocator frontier: Validate0x00A65330 calls singleton CMemoryAllocator at0x009F1F40. This address already has a generic no-op source E2_E2_009f1f40.cpp; establish its PDB method name/ABI and retype that provider before recovering the traversal. Variable allocator destructor also remains unresolved. Retail disassembly evidence in work/inspect_variable_enumeration.py; Validate traverses actual allocated lists and calls main validation with each data pointer, reloading alignment globals after calls. Preserve independent work below.

# Native live fragmentation checkpoint - 2026-10-05

Native recreation remains active; Lua/FMP excluded. Added optional --fragmentation to gate_variable_existing_allocations.py and the retail replay, with FABLE_FRAGMENTATION in the native integration fixture. It installs ABI-compatible vtable thunks that call qualified genuine recovered GetNoFreeAreas/GetNoAllocatedAreas. Both bindings are checked before normalizing just their two pointer cells in full 32-KiB comparisons. Retail executes its actual count entry points directly.

Combined --free-cycles --size-queries --largest-free --fragmentation passes 192 sequences, 4,068 allocation/free operations (2,400 allocations/1,668 frees), 3,336 size queries, 4,068 largest-free queries and 4,068 fragmentation queries. All fifteen required native-provider omissions fail linking. Fragmentation bits match retail and an independent live-list guard; native x87 exception flags and control preservation checked, heap and alignment globals preserved. Report rebuild/compile-gate/variable-composed-fragmentation.json. gate_all requests the strongest mode. Regression mode without --fragmentation also passes all 4,068 operations and thirteen provider omissions. No jobs live. No production source changes; closure remains 1,266 sources/zero duplicates/431 unresolved.

Next recover allocator enumeration: retail GetAllAllocatedAreas0x00A65260 (108 bytes, ret8) enumerates every chunk's allocated list and writes data pointers; its max argument is unused. GetAllocatedArea0x00A652D0 (96 bytes, ret4) walks a global zero-based index, returning data pointer or null (negative indexes miss). Both set alignment. Evidence: work/inspect_variable_enumeration.py disassembles retail directly. Validate0x00A65330 additionally calls native main-allocator validation0x009F1F40 per data pointer; defer until its dependency is recovered. No enumeration implementation or promotion yet. Preserve independent work below.

# Native fragmentation checkpoint - 2026-10-05

Native recreation active until stopped; Lua/FMP excluded. Landed fragmentation-only CMemoryAllocatorVariableSize_GetFragmentation_00a64590.cpp as genuine C++: NEAR_MATCH, 102/102 bytes, 38/38 instructions, 0.973684 similarity, five relocations. Sets alignment, calls both virtual count slots in order even on the zero-result path, returns zero for unsigned free count <=1, otherwise divides float(free-1) by float(allocated). Stop byte tuning at passing near match.

gate_variable_fragmentation.py passes 1,376 complete 32-KiB retail/native cases and all three required-provider omissions. Includes 96 cases executing the genuine recovered count methods, high-bit unsigned conversions, zero denominators, 53/64-bit x87 precision, callback owner/global mutation and fresh second-vtable dispatch. Native float32 results, exception flags and control preservation also compare against the original 102-byte body used strictly as a bounded hardware test oracle. Production contains no baked instructions. Callback pointer normalization validates all four native bindings first. Report: rebuild/compile-gate/variable-fragmentation.json.

Source, integration fixture, oracle and gate registered in symbol index, gate_all, manifest, artifact index and corrections. Index completed with 33,522 defined symbols. Completed closure: 1,266 sources, zero duplicates, 431 unresolved at work/codex-native-startup/native-gfmain-variable-fragmentation-closure-20261005. No manual coverage edits.

Next continue native allocator recovery/composition: add fragmentation queries to the existing real allocation/free cycles, using actual recovered count methods through ABI-compatible vtable bindings. Existing strongest composition remains --free-cycles --size-queries --largest-free: 192 sequences, 4,068 allocation/free operations, 3,336 size queries, 4,068 largest-free queries, thirteen provider omissions. Native heap lifetime/OS integration and broader engine frontier remain outstanding. Preserve independent Lua work below.

# Native largest-free-area checkpoint - 2026-10-05

Native recreation active until stopped; Lua/FMP excluded. Landed GetLargestFreeAreaSize0x00A64500: exact33 retail bytes/no relocations. Traverse PFirstChunk/PNextChunk and take the unsigned maximum of cached chunk LargestFreeAreaSize. It does not update global alignment or walk the tree. High-bit signed metadata compares as large unsigned values, matching retail jbe.

gate_variable_largest_free.py passes144 complete32-KiB retail/linked cases and two required-provider omissions: empty/one/three/seven chunks, six owner alignments, zero/equal/ordinary/high-bit cached sizes, unrelated counts and two fill patterns. Globals start256/8 and remain unchanged; entire storage unchanged. Permanent oracle_variable_largest_free.py and VariableLargestFree_test.cpp; report variable-largest-free.json. Source/gate/address registered in index/gate_all/manifest/artifact index/corrections.

Added optional --largest-free to real allocation/free composition (oracle largest_free=True, fixture FABLE_LARGEST_FREE). Query after every allocation/free, independently comparing against the live free-list maximum and checking storage/global preservation. Strong mode --free-cycles --size-queries --largest-free passes192 sequences/4,068 operations,3,336 allocation-size queries and4,068 largest-free queries; thirteen native providers and all thirteen omissions. Report variable-composed-free-stats.json preserves both query metrics. gate_all now requests this mode. Existing optional modes remain available. No OS acquisition/destruction or virtual allocation entry claim from raw-storage composition.

Fragmentation recovery started but is not landed/indexed: work/VariableAllocatorFreeStats_candidate.cpp and work/check_variable_free_stats.py. GetFragmentation0x00A64590 compiles102/102 bytes,38/38 instructions,0.973684 similarity,five relocations. Original body sets current alignment, always calls virtual slots+0x20/+0x24 in order (GetNoFreeAreas/GetNoAllocatedAreas), returns0.0 when unsigned free count<=1, otherwise casts (free-1) and allocated to floating values and divides via x87. Retail float constants explicitly verified:0x0122DEDC=0.0,0x0122DCB4=4294967296.0. First source spelling free>1/formula/else-zero scored0.842; early free<=1 return-zero matches original branch layout and reaches near-match. Do not tune further once behavior passes.

Next add a meaningful fragmentation fixture before promotion: virtual call order including guard path, unsigned count high bits, zero denominators, x87 results/FPU control and callback mutation/second vptr reload. Use recorded callbacks for the focused virtual seam and real native counts in a stronger composition; note public original GetNoAllocatedAreas returnszero. The scratch file also contains largest-free; when promoting fragmentation, split it into a fragmentation-only source to avoid duplicating the already-landed strong method.

Final index33,518 defined symbols; completed closure1,265 sources, zero duplicates,432 unresolved at work/codex-native-startup/native-gfmain-variable-largest-free-closure-20261005. Session66473 terminal; no jobs live. No manual coverage totals. Preserve independent Lua work below.

# Native allocation-size queries checkpoint - 2026-10-05

Native recreation active until stopped; Lua/FMP excluded. Landed GetAllocationSize0x00A651B0 and GetAllocationSizeIncludingHeader0x00A651E0 in CMemoryAllocatorVariableSize_AllocationSizes_00a651b0.cpp: exact44/46 retail bytes and two original alignment-global relocations each,90 native bytes. Original static fastcall data-pointer conversion and signed GetTotalSize const are real inline source. Queries replace global alignment, convert the data pointer using the aligned32-byte area header, read DataSize and optionally add the header; unsigned return wraps. No new null handling.

gate_variable_allocation_sizes.py passes792 complete32-KiB retail/linked cases and two required-provider omissions: six alignments4..128, three header positions, eleven zero/ordinary/signed-boundary/wrapping sizes, two fill patterns. Independent arithmetic, alignment globals and full unchanged storage checked. Report rebuild/compile-gate/variable-allocation-sizes.json. Permanent oracle_variable_allocation_sizes.py and VariableAllocationSizes_test.cpp. Corrected both new and previous totals fixtures to initialize stale globals to256/8 exactly like their retail oracles; totals5,184 still passes. Tightened the new emulator mapping to the intended128-KiB region.

Extended real production allocation/free composition with optional --size-queries (oracle collect size_queries=True, fixture FABLE_SIZE_QUERIES). Both retail and linked methods run on each real successful split/whole-area allocation's data pointer. 192 sequences/4,068 allocation/free operations plus3,336 queries on all1,668 successful allocations pass; twelve native providers and all twelve omissions. Complete post-query storage remains unchanged. New report variable-composed-size-queries.json, separate from ordinary composition. gate_all.py now requests this stronger mode. Original --free-cycles mode without query extension also passes4,068/eleven providers/omissions after fixture/oracle changes. No OS acquisition/destruction or virtual Allocate/Deallocate claim from this raw-storage composition.

Registered source/gate and both addresses in symbol_index.py/gate_all.py, manifest/functions.tsv, ARTIFACT_INDEX.tsv and corrections/function_overrides.tsv. Final index33,517 defined symbols; completed closure1,264 sources, zero duplicates,433 unresolved at work/codex-native-startup/native-gfmain-variable-allocation-sizes-closure-20261005. Session97472 terminal; no jobs live. No manual global coverage totals.

Next: GetLargestFreeAreaSize0x00A64500 inspected,33-byte body/no relocations: traverse chunk chain and take unsigned maximum of cached LargestFreeAreaSize at+0x10. It deliberately does not set current global alignment and does not walk the tree. Negative cached values compare as large unsigned values. This method is not authored/landed yet. Then fragmentation, allocation enumeration, validation and lifetime. Do not retune accepted Allocate/tree scheduling; keep Lua work separate.

# Native allocator memory totals checkpoint - 2026-10-05

Native recreation remains active until stopped; Lua/FMP excluded. Three original virtual statistics methods landed together in CMemoryAllocatorVariableSize_Totals_00a64db0.cpp: GetTotalMemoryAllocated0x00A64DB0 matches100 bytes, GetHeaderOverhead0x00A64E20 matches74, GetTotalMemoryControlledByAllocator0x00A65210 matches79; two original alignment-global relocations each,253 exact native bytes. Added original PDB-proven nonvirtual/static declarations Chunk::GetAllocatedDataSize const long, protected CalcAllocatedDataSize const long, and static fastcall Chunk::GetHeaderSize unsigned long. GetAllocatedDataSize calls CalcAllocatedDataSize in debug (RVA2FBF800); helpers are genuine inline source, no standalone helper-address claim.

Total allocated walks each allocated list for data, then adds actual chunk NoAllocatedAreas metadata times aligned32-byte area header plus aligned52-byte chunk header. Header overhead omits data. Controlled total sums chunk TotalDataSize plus one aligned area/chunk header per chunk; allocation metadata and ExtraMemUsedByOS are ignored exactly as retail. Every query replaces global alignment first. Using the original global-header/data helpers produced exact original bodies, rather than inventing accounting from the always-zero public allocated-count query.

gate_variable_totals.py passes5,184 complete32-KiB retail/linked cases and two required-provider omissions. Empty/one/three/seven chunks and allocated areas, six alignments4..128, two fill patterns, ordinary/signed-boundary/wrapping data, and metadata intentionally inconsistent with list length. Independent arithmetic checks all returned totals, alignment globals and unchanged full storage. No native boundary mocks. Report rebuild/compile-gate/variable-totals.json. Counts192 and constructor40 gates also pass; original class size/vtable are preserved. Registered all three addresses in manifest/artifact index/corrections and the source/gate in symbol_index.py/gate_all.py.

Final index33,511 defined symbols. Completed closure1,263 sources, zero duplicates,435 unresolved at work/codex-native-startup/native-gfmain-variable-totals-closure-20261005. The first closure ran before controlled-total addition and showed436; after final source/index refresh, rerun completed435. Session96960 is terminal; no jobs live. No manual global coverage totals. Scratch work/VariableAllocatorTotals_candidate.cpp contains only the initial two methods; production source/gate/report are authoritative for all three.

Next: remaining native allocator lifetime, allocation-size/enumeration, largest-free-area, fragmentation and validation methods. GetLargestFreeAreaSize0x00A64500, GetFragmentation0x00A64590, Validate0x00A65330; do not retune accepted Allocate/deletion/fixup scheduling. Preserve independent Lua work below.

# Native allocator counts checkpoint - 2026-10-05

Native recreation active until stopped; Lua/FMP excluded. Landed GetNoFreeAreas0x00A64530 and GetNoAllocatedAreas0x00A64560 together in CMemoryAllocatorVariableSize_Counts_00a64530.cpp. Complete48/43 retail bytes and two original alignment-global relocations each match. Both replace global alignment, then traverse PFirstChunk/PNextChunk. Free count sums NoFreeAreas with unsigned wrap. Original allocated-count method returns zero even with nonzero allocation metadata; preserve this retail behavior.

gate_variable_counts.py passes192 full32-KiB retail/linked cases, two required-provider omissions,91 native bytes. Empty/one/three/seven chunks, four alignments, two fill patterns and ordinary/signed-boundary/wrap counters; allocated metadata deliberately nonzero. Exact returns, current globals and unchanged complete storage checked, with no native boundary mock. Permanent oracle_variable_counts.py and VariableCounts_test.cpp; report rebuild/compile-gate/variable-counts.json. Source and both addresses registered in symbol_index.py/gate_all.py, manifest/functions.tsv, ARTIFACT_INDEX.tsv and corrections/function_overrides.tsv. Earlier scratch-only counts note below is superseded.

Completed index33,503 defined symbols; closure1,262 sources, zero duplicates,438 unresolved at work/codex-native-startup/native-gfmain-variable-counts-closure-20261005. Session29905 terminal; no jobs live. No hand-edited global coverage totals. Prior production allocation/free4,068, deletion2,120+24 guards and virtual allocation2,304 gates remain unchanged.

Next recover remaining native lifetime/statistics methods. Retail GetTotalMemoryAllocated0x00A64DB0 length100 walks allocated lists, sums data, adds aligned area headers multiplied by actual chunk NoAllocatedAreas metadata, plus aligned chunk header per chunk. GetHeaderOverhead0x00A64E20 uses the same metadata/header arithmetic without the data sum. Do not reuse the always-zero public allocated-count query for this arithmetic. These bodies are inspected but not authored/landed. Source scheduling already accepted for Allocate/deletion/fixup should not be revisited. Keep independent Lua work separate.

# Native tree deletion and allocation/free composition checkpoint - 2026-10-05

Native allocator statistics recovery started (2026-10-05): work/VariableAllocatorCounts_candidate.cpp and work/check_variable_allocator_counts.py. GetNoFreeAreas0x00A64530 matches all48 retail bytes/two original global relocations; GetNoAllocatedAreas0x00A64560 matches43/two. Both replace global alignment first. Free count sums chunk NoFreeAreas with unsigned wrap; allocated count really traverses the chain and returns zero, irrespective of the allocation counters. Preserve this observed retail behavior rather than inventing a count implementation. Neither candidate is landed/indexed yet: next add meaningful full-state retail/linked fixtures covering empty/multiple chunks, alignments, signed counter/wrap boundaries and deliberately nonzero allocated counts, then promote exact matches. Last authoritative closure remains1,261/zero duplicates/440 unresolved. No jobs live. Lua/FMP excluded; native recreation active.


Native recreation remains active until stopped, excluding Lua/FMP. Landed RemoveFromTree 0x00A64EF0 as NEAR_MATCH:298/305 bytes,112/114 instructions,0.911504 normalized ordered similarity and the original fixup relocation. Original algorithm/candidate is unchanged from the behavior-tested baseline. Do not continue byte tuning now that near_match and behavior pass.

Found and fixed a genuine grading bug in near_match.py: EBP was always classified as a frame pointer, erasing object member offsets in /Oy bodies. It now masks EBP displacements only after an actual mov ebp,esp frame setup, with no object reassignment or earlier EBP memory accesses. ESP stays a stack base. Added regression checks for EBP object/register equivalence, significant member offsets, actual frame stack slots and ambiguous/reassigned EBP. All16 similarity/verifier/header-grade tests pass. The corrected metric gives RemoveFromTree0.911504 (formerly0.884956) and unchanged production fixup0.990177 (formerly0.935167). This preserves actual member offsets instead of changing acceptance thresholds or source algorithms. Temporary cached-parent variant was reverted.

gate_variable_tree_deletion.py passes104 full retail/linked sequences,2,120 deletions,24 direct guards and two required-provider omissions. Complete32-KiB states, membership/signed size-address order, black depth/root/red-child invariants, removed links and preserved size/list/ID bytes checked; no tree or OS mock. Production fixup direct gate passes1,854 cases and exact84-byte RotateRight; virtual Allocate still passes2,304 cases/four omissions at0.980769. Registered the new deletion source in symbol_index.py/gate_all.py, manifest/functions.tsv, ARTIFACT_INDEX.tsv and corrections/function_overrides.tsv; updated fixup correction to the EBP-aware score.

Moved full existing-chunk heap composition into permanent gate_variable_existing_allocations.py: all providers now production sources. Free-cycle mode passes192 sequences/4,068 operations (2,400 allocations +1,668 frees), eleven providers/omissions; allocation-only mode passes192/2,400 with ten providers/omissions. Real best-fit, splitting, free/allocated lists, coalescing, deletion/fixup/insertion/rotations and alignment globals. Every free sequence restores the original seven extents with zero allocations. Reports variable-allocation-free-cycles.json and variable-existing-allocations.json are COMPOSED_NATIVE_PASS; no OS acquisition/destruction or virtual-entry claim from these raw-storage gates. Free-cycle mode is registered in gate_all.py.

Independent official verify_and_land.parity_of confirms RemoveFromTree NEAR_MATCH298. Broad EBP-change audit recompiles111 existing DIFFER/new near-match candidates at work/near-match-ebp-audit.json;56 currently pass near-match and behavior, and all nine prior accepted near matches remain accepted. Canonical coverage audit/totals are untouched; this is a separate regression audit, not a global coverage increase. Helper work/audit_allocator_ebp_promotion.py checks prior accepted rows and official Remove parity.

Final index33,500 defined symbols. Completed closure1,261 sources, zero duplicates,440 unresolved at work/codex-native-startup/native-gfmain-variable-tree-deletion-closure-20261005. All jobs terminal. Next: native allocator lifetime/statistics and full virtual/OS allocation composition; do not return to now-accepted deletion/fixup/Allocate byte scheduling. Keep independent Lua work separate.

# Native allocator near-match checkpoint - 2026-10-05

Native recreation remains active until stopped, excluding Lua/FMP. Applied the existing 2026-09-26 functional-first standard in CLAUDE.md, CONTRIBUTING.md and docs/modern/MODERNIZATION.md: stop byte tuning at passing near_match, while keeping exact bytes separate. Two additional original providers are landed/indexed and registered in the manifest, artifact index, ABI corrections and gate_all.py.

- Virtual Allocate 0x00A659C0: 140/140 bytes, 52/52 decoded instructions, 0.980769 ordered normalized similarity, eight original relocations; NEAR_MATCH. gate_variable_virtual_allocate.py passes 2,304 complete retail/linked cases and four required-provider omissions. Real best-fit and original strong AllocateMemoryFromExistingChunks retry; recorded split/NewChunk boundaries, original request/call order, unsigned wrap/signed search, stale global replacement, null failures and current-global data header reload. First attempt uses an implementation-only nonvirtual inline helper preserving the original algorithm. This resolved a genuine duplicate caused by combining the inline fallback definition with the separately landed strong retry provider; the final closure has none. Only two field-load instructions differ in order; no exact-byte claim.
- RemoveFromTreeFixup 0x00A64910: 756/757 bytes, 254/255 decoded instructions, 0.935167 similarity, one original RotateRight relocation; NEAR_MATCH. gate_variable_tree_fixup.py passes 1,854 direct full-state cases (1,830 captured real retail deletion calls plus24 guards), with provider omission. Both rotations are genuine; emitted RotateRight additionally matches all84 retail bytes. Retail caller descriptors are copied from stack to a fixed non-node slot for standalone replay, unchanged before/after. No deletion support stub or OS/tree mock runs inside this fixture. Original sequences independently check membership, signed(size,address) ordering, root black, parent links, equal black depth and no red-red/cycles.

Reports: rebuild/compile-gate/variable-virtual-allocate.json and variable-tree-fixup.json. Direct capture added as optional collect(fixup_rows=...) and collect_fixups() in oracle_variable_tree_deletion.py; default deletion oracle behavior remains intact. The virtual oracle/test are now permanent pipeline/integration files. Full real allocation/free composition now links the production fixup and passes192 sequences/4,068 operations (2,400 allocation +1,668 free), eleven provider omissions; only RemoveFromTree remains scratch, so composition stays BEHAVIOR_ONLY_NOT_LANDED. Existing deallocation2,432 and constructor40 gates pass after the implementation helper declaration; class size/vtable remain checked.

RemoveFromTree baseline remains298/305 bytes,112/114 instructions,0.884956 similarity: below90%, not landed. /Oa yields305 bytes but0.881057, also fails. Official verify_and_land.parity_of with installed QFE4035 tried default,/Oa,/GS,/GS+/Oa: same scores/failures, no new compiler remedy. Do not repeat these forms. Default candidate restored; no parent/child-colour scratch edits persist.

Final sequential symbol refresh:33,497 defined symbols. Completed closure:1,260 sources, zero duplicates,441 unresolved at work/codex-native-startup/native-gfmain-variable-allocator-near-matches-closure-20261005. All jobs terminal. No global coverage totals hand-edited; dedicated near-match gates record their own complete-body/behavior evidence. Next: finish RemoveFromTree source scheduling above threshold, then native variable-allocator lifetime/statistics to close the actual main heap. Keep Lua work separate.

# Native virtual deallocation checkpoint - 2026-10-05

Native recreation remains active until stopped, excluding Lua/FMP. Landed original virtual Deallocate 0x00A65A50 (void ABI, 78 bytes/four relocations) and DeleteChunk 0x00A65390 (126 bytes/three relocations). Both complete retail bodies match; original PDB proves static fastcall ConvertDataPointerToMemoryArea and GetHeaderSize. Deallocate updates global alignment, converts the data pointer, runs real FreeArea/coalescing, and deletes only the last nonpermanent allocation's chunk. DeleteChunk removes free areas from the tree, unlinks the chunk, records memory status twice through the same buffer and releases only nondemand-paged storage. It reloads DemandPaged after the first status call and ignores VirtualFree failure exactly as retail does.

gate_variable_deallocation.py passes 2,432 retail/linked cases, 912 dependency bytes, six native providers and six provider omissions. It links real list/free/coalescing providers; tree and OS calls are recorded boundaries with full pre-call states. Cases cover all four chunk positions, remaining allocations, permanence, demand paging, four alignments, free-area adjacency, fill patterns, status mutation of DemandPaged and OS release success/failure. No real OS release or complete heap claim. Report: rebuild/compile-gate/variable-deallocation.json.

Registered both sources in symbol_index.py/gate_all.py, manifest/functions.tsv, ARTIFACT_INDEX.tsv and corrections/function_overrides.tsv. Corrected old DeleteChunk address label and Deallocate return type. Completed index refresh: 33,484 defined symbols. Completed closure: 1,258 sources, zero duplicates, 442 unresolved at work/codex-native-startup/native-gfmain-variable-deallocation-closure-20261005. Index session50265, PDB session27424 and closure93656 are terminal. Selected-process diagnostic approval succeeded after sandbox WMI denied access; those processes had already exited by inspection.

Virtual Allocate now passes 2,304 complete retail/linked behavior cases in work/variable_virtual_allocate_behavior/behavior.json (runner work/check_variable_virtual_allocate_behavior.py). Three native sources and all three required-provider omissions; real best-fit and emitted out-of-line retry dispatcher, recorded AllocAreaFromArea/NewChunk boundaries. Independent assertions cover minimum signed eligible size, unsigned alignment wrap, initial global alignment replacement, original request forwarded to NewChunk, retry only after successful NewChunk, failed split retry, null result, full 32-KiB storage and data-pointer header alignment after a deliberate recorded NewChunk global mutation. The candidate emits its own dispatcher COMDAT, so the fixture excludes the redundant strong dispatcher provider. No OS acquisition, splitting, or complete heap claim from this focused test.

Virtual Allocate remains unpromoted: 140 bytes/eight relocations, only two field-load instructions differ in order. Restored inline GFAlignShift after ordinary definition made no difference; /G5 and /G7 also remain nonmatches, restored default /O2 audit. Debug Base::Align still calls the original fastcall GFAlignShift(size, Alignment, AlignmentShift). Tree deletion scratch remains 298/305 and 756/757, behavior-only; existing real allocation/free cycles pass 4,068 operations. Temporary cached parent and child-colour variants produced 309 and 295 bytes and were reverted to the tested baseline. Debug original and PDB locals prove no separate parent/child-colour locals: this, area_to_remove, replace_with, one-byte replace_with_colour, child_info and child. Full parity rechecked after restoration. All jobs terminal. Next: finish virtual allocation and tree deletion parity. Preserve independent Lua work below.

# Native chunk free/coalescing checkpoint - 2026-10-05

Native recreation remains active until stopped, excluding Lua/FMP. Three original nested chunk methods are landed/indexed in VariableAllocatorChunk_FreeArea_00a658e0.cpp: FreeArea 0x00A658E0 matches79 bytes/two relocations, ConsolidateAdjacentFreeAreas 0x00A65590 matches248/twelve, RemoveAreaFromAllocatedList 0x00A64CE0 matches55/no relocations. 382 new native bytes. gate_variable_chunk_free.py passes 1,152 complete 32-KiB retail/linked states plus all recorded tree boundary arguments/pre-states, four provider omissions and 708 dependency bytes. Tree mutations are recorded in this focused gate. Independent model checks maximal contiguous free run containing the target, merged data including absorbed 32-byte headers, retained lowest address/ID, sorted free sizes, list/count/extrema/free-byte totals and cleared allocated links. Cases cover every free/allocated mask around the target, three list insertion orders, four alignments and two fill patterns.

Debug consolidation body RVA0x2FC1860 length278 resolved code generation: keep a separate active-area local, per-branch Create result, insert that result, reload the free-list head, then assign active to the merged result. Assigning active before Insert or ignoring the first Create return collapsed branch calls and compiled241 rather than248. The source repeatedly scans the size-sorted list and restarts at its head after each physical merge; it does not assume physical address order.

Stronger raw-storage allocation/free composition now passes 192 sequences / 4,068 complete retail/linked operations (2,400 allocations and1,668 frees), using eleven genuine providers with no tree/list/coalescing mock. All eleven provider omissions fail. Nine providers are landed; only the two deletion bodies remain scratch. Free orders ascend/descend/shuffle addresses, forcing both merge directions and repeated joins. After every operation: exact return/full state, tree membership/red-black invariants, lists/counts/extrema, data/header conservation and no overlapping live extents. Every sequence ends with zero allocated nodes and the original seven free extents/sizes restored. This report remains BEHAVIOR_ONLY_NOT_LANDED until deletion byte parity closes: work/variable_allocation_free_cycles_behavior/behavior.json. Runner: work/check_variable_existing_allocations_behavior.py --free-cycles; oracle collect(free_cycles=True), fixture FABLE_FREE_CYCLES mode. Original allocation-only mode remains available.

Corrected original Base::Align to const from FableWin PDB; two promoted inline definitions and their scratch predecessors updated. Existing dispatch2592, NewChunk32, constructor40 and real chunk/tree48 gates retain parity/behavior. Provider index refreshed sequentially;33479 symbols. Base::SetCurrentAlignment is now declared from original PDB. CMemoryArea::GetDataStart const also proved from DIA and declared. No standalone helper-address claim made for these declarations.

Completed closure:1,256 sources, zero duplicates,443 unresolved at work/codex-native-startup/native-gfmain-variable-chunk-free-closure-20261005. Session49564 is terminal; no live jobs. Source omission/parity/new gate and above regressions pass. Deletion remains298 versus305 and756 versus757 with one relocation each, after final baseline parity check. Extra sibling-child local caching did not change fixup bytes; current candidate keeps it and full free cycles still pass. Do not repeat already audited black-result/helper-order/compiler-flag forms without new evidence.

Virtual Allocate recovery also started: work/VariableAllocatorAllocate_candidate.cpp compiles140 bytes/eight relocations at true0x00A659C0, same length as retail. Only instruction order differs at A659DF/E5: candidate loads AlignmentShift then Alignment; retail loads Alignment then Shift. All remaining bytes/relocation targets match. It is unpromoted and has no dedicated virtual-entry behavioral test. Original SetCurrentAlignment sets global shift first then alignment; first existing-chunk allocation is inlined, NewChunk uses original request, retry allocation stays out-of-line, success returns area's data start using current global header alignment. Debug Allocate RVA0x2FBF350 length144 includes a size assertion compiled out in retail. Original Base::Align const calls global fastcall GFAlignShift(size,alignment,shift), proven debug ILT0x141D1ED resolves to RVA0x2FC1F00. Candidate preserves that original helper name and GetDataStart/SetCurrentAlignment inlines. Return arithmetic/source helper variations did not close the two-load ordering. Check work/check_variable_allocate.py and work/variable_chunk_objects/allocate_side_by_side.txt; no asm/volatile workarounds.

Next: finish real deletion and virtual Allocate parity, then original virtual Deallocate (true0x00A65A50,78 bytes, calls newly landed FreeArea and DeleteChunk0x00A65390). DeleteChunk still unrecovered. Whole native heap remains incomplete. Preserve independent Lua notes below.

# Native existing-chunk allocation checkpoint - 2026-10-05

Native recreation remains active until stopped, excluding Lua/FMP. True AllocateMemoryFromExistingChunks 0x00A656E0 is now landed/indexed in CMemoryAllocatorVariableSize_AllocateMemoryFromExistingChunks_00a656e0.cpp: 59 retail bytes and two relocations match. Original public ABI replaces the manifest's generic MemoryAllocator_AllocateFromPool label. gate_variable_allocate_existing.py passes 2,592 retail/linked cases with two native providers and two omissions, real signed best-fit query (55 bytes) and recorded AllocAreaFromArea boundary. Independent assertions verify unsigned alignment/wrap, minimum eligible signed size/node membership, actual parent chunk/area/aligned-size call arguments, exact null/success boundary return propagation and unchanged full 32-KiB state. Splitting/deletion mutations are explicitly outside this dispatch gate.

Stronger full-chain behavior also passes: 192 sequences / 2,400 existing-chunk allocation calls with ten genuine native sources (eight landed plus two scratch deletion bodies), no allocation/tree mock. Real best-fit, splitting, list removal/insertion, tree deletion/fixup/insertion and rotations execute. Oracle begins from retail-built valid free lists/trees in raw test storage, then compares complete 32-KiB state and returns after every call. Independent checks enforce min eligible size and candidate membership, split-tail placement or preserved whole-area size, list links/counts/extrema, exact tree/free-list membership and red-black invariants, data/header conservation, no overlapping live areas, ID preservation and unchanged state on misses/signed split overflow. Two size arrangements include equal-size ties; three insertion orders, four alignments, two fill patterns and four request sequences cover zero/exact/near sizes, exhaustion, alignment wrap and signed split rejection. All ten source omissions fail. This broader report remains BEHAVIOR_ONLY_NOT_LANDED because deletion byte parity is unresolved: work/variable_existing_allocations_behavior/behavior.json. Runner work/check_variable_existing_allocations_behavior.py uses the promoted allocation provider, two scratch deletion candidates and seven other landed providers; oracle/fixture are tools/decomp_pipeline/oracle_variable_existing_allocations.py and rebuild/tests/integration/VariableExistingAllocations_test.cpp.

Full-chain oracle initially calculated best-fit size after mutation; corrected to cache sizes/eligible choices before invoking retail, then full independent checks pass. No production behavior changed for that test correction.

Completed closure: 1,255 sources, zero duplicates, 443 unresolved at work/codex-native-startup/native-gfmain-variable-allocate-existing-closure-20261005. Session 45165 is terminal; no live jobs. New dispatch gate, full chain, existing queries (13,104), and chunk/tree composition (48) pass. Deletion remains scratch: RemoveFromTree 298 versus305 bytes, fixup756 versus757, each one relocation. Baseline parity checker rerun restores current side-by-side objects. Existing 2,120 deletion operations/24 guards previously passed. Do not promote deletion on behavior alone or claim a complete native heap.

Compiler option audit work/check_delete_compiler_forms.py completed (session56528 terminal), report work/variable_chunk_objects/delete_compiler_forms.json. Default/G5/G6/Op keep298/756; G7 gives296/756; Oa/Ow give305/744 but still do not match. Og-/Os/Ob1 introduce unrecognized helper relocations and checker stops for fixup, so those are incomplete audits rather than parity claims. No compiler option was adopted. Original RotateLeft debug RVA0x2FC0470 calls SetTreeRootNode; fixup scratch rotation bodies now use that original inline helper but bytes unchanged. DIA local-type audit confirms all parent/sibling/removed/child pointers are nonconst, saved colour is unsigned byte. GetTreeRoot and IsNodeBlack const qualifiers are already corrected. Original rotation leaf bodies still come from proven 84-byte source. Next: finish deletion parity and real promoted composition, then native allocator Allocate/Deallocate and consolidation/chunk destruction. Preserve independent Lua notes below.

# Native tree deletion behavior checkpoint - 2026-10-05

Native recreation remains active until stopped, excluding Lua/FMP. Scratch real RemoveFromTree and RemoveFromTreeFixup now pass 104 complete deletion sequences / 2,120 full 32-KiB retail/linked post-states plus 24 direct null/inactive descriptor guard cases, without tree mocks. Sequences delete ascending, descending, current root and deterministic random order from all 26 retail-built trees, including equal sizes/address ties and signed extremes. Independent checks enforce exact remaining membership, signed(size,address) order, parent links, root black, equal black depth, no red-red/cycles, cleared removed tree links and preserved original size/list/parent-chunk/ID bytes. Coverage requires both mirrored red-sibling, all-black recolor, inner and outer paths. Two provider omissions fail.

These methods are NOT landed/indexed: work/VariableAllocatorRemoveTree_candidate.cpp currently compiles 298 versus 305 retail bytes at 0x00A64EF0; work/VariableAllocatorDeleteFixup_candidate.cpp compiles 756 versus 757 at 0x00A64910. Both have one original relocation. Fixup control flow and instruction offsets mostly match, but register allocation differs: candidate uses EDI for this, ECX for descriptor and EDX for retained sibling-left pointer, versus retail EBP/EDX/EDI, with an extra retail saved register. Removal register allocation and descriptor Colour-store scheduling differ. Do not promote on behavior alone. work/check_variable_tree_delete.py checks parity and writes current side-by-side disassembly under work/variable_chunk_objects; work/check_variable_tree_delete_behavior.py links genuine scratch sources and existing exact inline rotations. Behavior report: work/variable_tree_delete_behavior/behavior.json, grade BEHAVIOR_ONLY_NOT_LANDED. Oracle/fixture: tools/decomp_pipeline/oracle_variable_tree_deletion.py and rebuild/tests/integration/VariableTreeDeletion_test.cpp, not yet gate_all-registered.

Original global CDeletedNodeChildInfo is size 0xC: bool LeftChild +0, area pointer PParent +4, unsigned byte Colour +8, bool Valid +9 (DIA base types 10/0/7/10). Header now declares this and proven protected fixup/helper methods. DIA objectPointerType.type.constType proves GetTreeRoot and IsNodeBlack are const; SetTreeRootNode is not. These corrected declarations are preserved. Their inline helper definitions live only in scratch sources. Existing insertion gate (530 snapshots/28 direct operations) and splitting gate (528) still pass; scratch deletion/guards rerun pass. No live jobs; broad rg session 50198 is terminal. Latest promoted closure remains 1,254 sources, zero duplicates, 443 unresolved (session 39032 terminal).

Debug FableWin deletion bodies: RemoveFromTree RVA 0x2FC0B10 length555, fixup RVA 0x2FC0DD0 length854. work/disasm_allocator_debug.py accepts RVA/length and optional resolve argument for an ILT E9; IsNodeBlack debug ILT RVA0x143D303 resolves to RVA0x2FC1200, return implements null OR Colour==1. work/pdb_allocator_locals.py lists original local names/types. Actual removal locals: replace_with, replace_with_colour, child_info, child. Debug source assigns info.Colour in child-selection branches then, for nonnull child, sets Valid=false and stores child's parent. Retail compiler hoists old parent into EBP and sinks colour store after that parent write. Do not invent source asm/volatile tricks.

Small black-result forms, rotation/helper definition order and colour-expression/cache forms did not close parity and restore candidates when no exact match. Scripts work/try_delete_black_forms.py, work/try_delete_definition_order.py and work/try_remove_colour_forms.py document those attempts; do not endlessly repeat them. Next: finish deletion byte parity from original inline/helper/local evidence, then promote/register only after exact bytes plus current behavior pass; compose splitting and AllocateMemoryFromExistingChunks with genuine tree deletion. No complete native heap claim yet. Preserve independent Lua notes below.

# Native area splitting checkpoint - 2026-10-05

Native recreation remains active until stopped, excluding Lua/FMP. Original nested chunk AllocAreaFromArea 0x00A65480 (265 bytes/seven relocations), InsertAreaIntoAllocatedList 0x00A64D20 (35 bytes) and RemoveAreaFromFreeList 0x00A65060 (138 bytes/one relocation) are landed, registered and indexed. gate_variable_area_splitting.py passes 528 full 32-KiB retail/linked comparisons with four native source omissions. Real removal/insertion lists execute; tree insertion/removal record original arguments and complete pre-call state. Tree mutation is not claimed by this gate. Independent checks cover signed overflow unchanged, split tail address, whole-area size preservation, counts/byte totals and preserved ID bytes. Dependency parity is 626 bytes including existing free-list insertion. Native sources remain ordinary C++.

Debug FableWin confirms separate checked header-plus-request and total-size calculations, remainder from total area size minus request/header, static fastcall Create returning an area pointer, and split allocation at the remaining area's end. CMemoryArea::GetHeaderSize returns signed long; unsigned caused zero extension instead of retail CDQ. Original inline Create/GetTotalSize and separate locals resolve all scheduling/register differences. Only recovered methods are claimed in metadata; their emitted inline helpers are not separate retail-provider claims.

Completed closure: 1,254 sources, zero duplicates, 443 unresolved at work/codex-native-startup/native-gfmain-variable-area-splitting-closure-20261005. Session 39032 is terminal; no live jobs. The extra unresolved dependency is genuine RemoveFromTree. Variable constructor (40), chunk/tree composition (48), syntax and whitespace checks pass. Initial regression command used a nonexistent gate_variable_constructor.py; corrected gate_variable_allocator_constructor.py passes.

Next: RemoveFromTree 0x00A64EF0 (305 bytes, calls RemoveFromTreeFixup 0x00A64910), then real split/tree composition and scratch AllocateMemoryFromExistingChunks promotion. PDB CDeletedNodeChildInfo is a global 12-byte class: bool LeftChild +0, CMemoryArea* PParent +4, byte Colour +8, bool Valid +9. It is not nested in the allocator. Retail fixup is 757 bytes and uses a descriptor for null children. It loops for real black nonroot nodes OR valid black deleted-child info with a parent, selects parent from node or descriptor, and tests node-left OR descriptor-left. Rotation bodies are already genuine. Preserve original nullable descriptor semantics; do not replace with a generic textbook deletion. Disassemble with work/disasm_allocator_retail.py; PDB tool uses donor addresses only. Tree deletion and whole heap remain unrecovered. Preserve independent Lua notes below.

# Native chunk/tree composition and queries checkpoint - 2026-10-05

Native recreation remains active until stopped, excluding Lua/FMP. New gate_variable_chunk_tree_composed.py links eight genuine providers and passes 48 complete retail/native chunk-to-tree constructions (36 success/12 failure), eight omissions and 1,322 bytes of dependency parity. VirtualAlloc/VirtualQuery are the only recording boundaries; insertion/fixup/rotations execute for real. Existing valid three-/four-node trees force recolor and both inner/outer rotation paths, asserted by retail coverage and full 32-KiB post-state/tree invariants. Earlier recorded 32-case NewChunk gate still passes. oracle_variable_new_chunk.collect(real_tree=True) and fixture FABLE_REAL_TREE mode implement this stronger check.

Three protected const tree queries are landed/indexed in CMemoryAllocatorVariableSize_TreeQueries_00a64c10.cpp: best-fit search 0x00A64C10 (55 bytes), largest 0x00A64C50 (27), smallest 0x00A64C70 (27). gate_variable_tree_queries.py passes 13,104 calls over 531 tree states (530 retail-built insertion states plus empty), independent signed bounds/node-membership/min-max assertions, full unchanged-state checks and one omission. The manifest's incorrect AllocateMemoryFromExistingChunks label at A64C10 is corrected. Native tree/heap operations are still incomplete.

Completed closure: 1,252 sources, zero duplicates, 442 unresolved at work/codex-native-startup/native-gfmain-variable-tree-queries-closure-20261005. Session 90020 is terminal; no live jobs. Syntax/whitespace and query/NewChunk/composed gates pass. Next: real area splitting/removal. work/VariableAllocatorAllocateExisting_candidate.cpp matches all 59 bytes/two relocations at actual AllocateMemoryFromExistingChunks 0x00A656E0, but is scratch/unpromoted and lacks behavioral validation. It calls real best-fit query and nested chunk AllocAreaFromArea 0x00A65480. Header adds proven public declarations. The earlier allocate_existing_retail.txt starts at A656C0 (destructor tail), then includes actual A656E0; do not misidentify the tail as allocation. Use work/check_variable_allocate_existing.py. Recover splitting plus tree/list removal before claiming heap allocation. Preserve independent Lua-thread notes below.

# Native variable tree insertion checkpoint - 2026-10-05

Native recreation remains active until stopped, excluding Lua/FMP. Tree insertion/fixup/rotations are landed and registered: 0x00A64E70/0x00A64740/0x00A64680/0x00A646E0 match 122/464/84/84 retail bytes (754 total). InsertIntoTree source calls the real fixup; fixup source includes exact original inline rotation bodies and emits their original COMDATs. gate_variable_tree_insertion.py passes 26 sequences/530 full 32-KiB snapshots plus 28 direct rotation/fixup operations and two provider omissions, with no tree mock. Independent oracle checks enforce signed size/address ordering, root black, parent links, no cycles/red-red links and equal black depth after every insert. Cases cover all recolor/rotation paths, signed extremes, equal-size address ties and root/parent-root guards.

Completed closure: 1,251 sources, zero duplicates, 442 unresolved at work/codex-native-startup/native-gfmain-variable-tree-insertion-closure-20261005. Session 59685 is terminal; no live jobs. NewChunk (32) and tree gate pass; syntax/whitespace pass. Manifest originally lacked 0x00A64680 and misnamed 0x00A646E0 RotateLeft; true rotation rows/types are corrected from donor/layout/retail evidence. Next: compose real chunk creation with this tree chain, removing recorded InsertIntoTree from a new full-composition test; then recover actual variable allocation/deallocation and tree deletion. Existing recorded chunk gates remain focused parity tests. Preserve independent Lua-thread notes below.

# Native OS-backed chunk promotion checkpoint - 2026-10-05

Native recreation remains active until stopped, excluding Lua/FMP. NewChunk 0x00A65720 is landed/indexed: all 269 retail bytes and nine relocations match. True CMemoryAllocator::OnFailedAllocation 0x009F1F00 matches one byte; obsolete C009f1f00::M source, print-only test and catalog entry are retired. gate_variable_new_chunk.py passes 32 composed retail/linked cases (20 success/12 failure), six genuine native sources and six provider omissions. Native Init/list and failure handler are linked; Win32 acquisition/query and tree insertion are recording boundaries. Failure handler call identity is checked through original relocation/omission, not runtime instrumented. Separate initial aligned size and retry size (declared after memory/actual-size locals), duplicated max expression and original InternalAlloc protection construction close register/scheduling parity. Base Align and variable InternalAlloc have original inline bodies within NewChunk source.

Completed closure: 1,249 sources, zero duplicates, 443 unresolved at work/codex-native-startup/native-gfmain-variable-new-chunk-closure-20261005. Session 43709 is terminal. Main constructor (16), earlier chunk initialization (116), syntax/whitespace pass. No live jobs. Next: actual tree insertion/fixup. work/VariableAllocatorInsertTree_candidate.cpp now matches 122 bytes/three relocations at 0x00A64E70, but is scratch/unpromoted and lacks behavior validation. Equality is a separate branch and pointer address breaks equal-size ties before signed size ordering. It calls InsertFixup 0x00A64740; header adds that proven protected method. Checks/disassembly: work/check_variable_insert_tree.py, work/variable_chunk_objects/tree_side_by_side.txt and insert_tree_retail.txt. Continue toward real tree mutation, replacing the remaining recorded tree boundary. Preserve independent Lua-thread notes below.

# Native OS-backed chunk recovery checkpoint - 2026-10-05

Native recreation remains active, excluding Lua/FMP. NewChunk is still scratch/unpromoted: work/VariableAllocatorNewChunk_candidate.cpp now preserves the debug build's inline InternalAlloc helper, signed halving, real VirtualAlloc/VirtualQuery calls, no-op OnFailedAllocation and real Init/list dependency. Current body 270 bytes versus 269 retail, with register/scheduling differences; check work/check_variable_new_chunk.py and work/variable_chunk_objects/new_side_by_side.txt. No new parity/provider claims. Header adds proven public InternalAlloc and OnFailedAllocation declarations only. The earlier work/tune_variable_new_chunk.py targets the older direct-call skeleton; do not run it unchanged against the inline-helper version.

New tools/decomp_pipeline/oracle_variable_new_chunk.py executes retail NewChunk, real chunk Init/list and retail failure handler, recording only Win32 acquisition/query and tree insertion. 32 scenarios finish (20 successful/12 failed), covering retries, read-only Flags, queried RegionSize, zero-return query with supplied output, signed request/minimum overflow, negative request with eventual success and existing chunk links. It does not yet have a linked C++ comparison. Debug source-structure evidence: work/variable_chunk_objects/new_chunk_debug.txt, FableWin NewChunk RVA 0x02FC00A0 (293 bytes); Inline InternalAlloc writes output pointer and returns queried size. Ego_r optimized local metadata exposes only signed request_size. No live sessions remain (59393 terminal). Completed closure remains 1,247 sources/zero duplicates/444 unresolved. Next: full parity plus linked composed OS retry tests, then recover tree insertion.

# Native variable chunk initialization checkpoint - 2026-10-05

Native recreation remains active until stopped, excluding Lua/FMP. Three new registered native providers: NewChunkDemandPaged 0x00A65830 (108 bytes), nested chunk Init 0x00A65410 (110), InsertAreaIntoFreeList 0x00A650F0 (188). All 406 bytes/nine relocations match retail. gate_variable_chunk_initialization.py passes 116 composed retail/linked cases and four provider omissions. Full 32-KiB memory snapshots before the tree call and after return cover alignment, signed sizes, old chunk links, ordered/equal free areas, preserved fields and canaries. InsertIntoTree is a recording boundary; tree mutations and OS acquisition are not claimed. Header adds original nested/protected methods. Main constructor (16) and variable constructor (40) gates pass.

Completed closure: 1,247 sources, zero duplicates, 444 unresolved at work/codex-native-startup/native-gfmain-variable-chunk-initialization-closure-20261005. Session 30106 is terminal; no live job remains. Next: actual NewChunk 0x00A65720, 269 bytes (donor 268). Disassembly: work/variable_chunk_objects/new_chunk_retail.txt. Retail rounds requested size plus chunk/area headers, applies MinChunkSize and alignment, tries VirtualAlloc(0,size,0x103000,protection), queries actual RegionSize through VirtualQuery, halves retries and initializes/links a successful chunk. Failure calls CMemoryAllocator::OnFailedAllocation at 0x009F1F00, which is retail ret/no-op; donor NewChunk calls proven OnFailedAllocation 0x00B46760. Legacy 00/9f/E2_E2_009f1f00.cpp has the wrong class/mangled symbol and must not be selected as this provider. Tree insertion 0x00A64E70 remains another required dependency. Preserve independent Lua-thread notes below.

# Native main allocator promotion checkpoint - 2026-10-05

Native recreation remains active until the user stops, excluding Lua/FMP. Main constructor 0x009F20B0 is now a genuine registered provider: all 598 retail bytes and sixteen relocations match; sixteen composed cases using twenty-one native sources and eight provider omissions pass. Keeping separate fixed/variable lookup assignment branches (confirmed in debug FableWin constructor) resolved code generation. Heap stays untouched, disabled rows preserve stale allocator pointers, overflow returns with the lock held. OS/CRT boundaries record calls; unrecovered allocator operations abort in the fixture. Gate: tools/decomp_pipeline/gate_main_allocator_constructor.py; report: rebuild/compile-gate/main-allocator-constructor.json.

Completed closure: 1,244 sources, zero duplicates, 444 unresolved at work/codex-native-startup/native-gfmain-main-allocator-constructor-closure-20261005. Session 82219 is terminal. Fixed lifetime (30), variable constructor (40), initializer (16) gates pass. Next: variable initial chunk helpers. work/VariableAllocatorDemandPaged_candidate.cpp now matches 108 retail bytes/five relocations at 0x00A65830, but is scratch only and lacks behavioral validation. work/check_variable_demand_paged.py verifies that candidate; its dependency chunk Init is 0x00A65410, 110 bytes, then InsertAreaIntoFreeList 0x00A650F0. Header now declares the proven chunk Init method. Do not promote the helper on byte parity alone. Preserve independent Lua-thread notes below.

# Native main allocator behavior checkpoint - 2026-10-05

Native recreation stays active until stopped, excluding Lua/FMP. Main allocator construction now passes sixteen full retail/composed comparisons using twenty-one native sources (including its scratch body), eight provider omission checks, real size-class constructors/getters and real boot.ini scanner. Cases cover rounding, categories, lookup holes/padding, stale disabled rows, variable allocation failure and overflow leaving the lock held. Main bytes still differ (596 versus 598 retail); it is unpromoted and not indexed. Report: work/main_allocator_objects/behavior.json (BEHAVIOR_ONLY_NOT_LANDED).

Original nested lookup record type is restored with legacy global typedef; destruction, statistics, fixed/variable lifetimes, creation and composed-save gates pass. Latest completed closure remains 1,243 sources, zero duplicates, 445 unresolved. No live jobs; anneal session 40758 ended with seventy flag/pragma compiles and no source neighbors (constructor traversal unsupported). Next: native main constructor parity. Use work/MemoryAllocatorConstructor_candidate.cpp, work/check_memory_allocator_constructor.py, tools/decomp_pipeline/check_main_allocator_behavior.py and work/native_main_allocator_locals_20261005.txt. Do not upgrade metadata on behavior alone. See journal; preserve independent Lua-thread notes below.

# Native memory initializer checkpoint - 2026-10-05

Native recreation remains active until stopped, excluding Lua/FMP. InitializeMemorySystem 0x009F1D20 now matches 347 retail bytes and passes sixteen linked/retail full call traces. It scans boot.ini but does not apply parsed values; preserves file-size parsing after short/failed reads and signed-character isspace calls. Seven Win32 APIs and isspace are recording boundaries; native imports remain real. Fixed lifetime (30) and variable constructor (40) gates pass.

Latest completed closure: 1,243 sources, zero duplicates, 445 unresolved at work/codex-native-startup/native-gfmain-memory-initializer-closure-20261005. Session 85362 is terminal; no pending job. Next: main constructor 0x009F20B0. Its scratch C++ at work/MemoryAllocatorConstructor_candidate.cpp compiles to 596 bytes versus 598 retail and is unpromoted/unverified behaviorally. Inspect work/main_allocator_objects/side_by_side.txt and work/check_memory_allocator_constructor.py. Preserve untouched Heap field and original early-return overflow path that leaves the lock held. See journal for exact scope, evidence and tests; preserve independent Lua-thread notes below.

# Native variable allocator checkpoint - 2026-10-05

Native recreation stays active until stopped, excluding Lua/FMP. Variable allocator now has its original 0xC38 layout and 16-slot interface, with nested 0x20-byte area/0x34-byte chunk. Constructor 0x00A65930 matches 138 retail bytes; 40 retail/linked cases cover both initial-chunk branches, no-chunk paths, alignment globals, permanent marking and untouched fields. Eight native alignment-state bytes match retail zero-fill. Five omissions fail. Initial chunk helpers are recording boundaries; variable virtual methods/destructor remain unrecovered. Fixed lifetime (30) and base constructor (20) gates also pass.

Latest closure: 1,242 sources, zero duplicates, 445 unresolved at work/codex-native-startup/native-gfmain-variable-allocator-constructor-closure-20261005. Seventeen new unresolved symbols are exposed variable operations/destructor/chunk helpers. Fixed refresh session 44964 and variable closure session 36475 are terminal; no pending job from this checkpoint. Next: main allocator constructor 0x009F20B0, then actual initial chunk helpers and allocator operations. Both real size-class constructors are now available. See journal and work/native_allocator_constructor_chain_20261005.txt; preserve independent Lua-thread notes below.

# Native fixed allocator checkpoint - 2026-10-05

Native recreation remains active until stopped, excluding Lua and FMP analysis. Fixed-size allocator now has its original base inheritance, 16-slot interface, complete 0xC4C object and 0x18-byte nested chunk. Constructor/destructor/deleting thunk match 203 retail bytes; 30 retail/linked object comparisons, vtable checks and four provider omissions pass. Ten corrected virtual getter/no-op candidate builds and base constructor gate pass. Five unrecovered virtual operations remain aborting fixture boundaries; heap/chunk behavior is not claimed.

Latest completed closure: 1,240 sources, zero duplicates, 428 unresolved at work/codex-native-startup/native-gfmain-fixed-allocator-lifetime-closure-20261005. Five newly exposed symbols are the fixed allocator virtual operations; all ten native virtual leaves link. Closure session 31842 is terminal. Provider-index refresh exec session 44964 remains live; poll it before restarting. The verified twelve fixed allocator sources are explicitly seeded, so closure does not depend on that pending refresh. Next: finish these jobs, then variable allocator layout/constructor (donor size 0xC38; work/native_variable_allocator_methods_20261005.txt), then main constructor 0x009F20B0. See journal for exact boundaries and unrelated unfiltered-build failure. Preserve independent Lua-thread notes below.

# Native allocator base checkpoint - 2026-10-05

Native recreation remains active until the user stops, excluding Lua and FMP analysis. Complete base layout is now 0xC1C with original performance/alignment fields; its constructor 0x00A65AD0 matches 113 retail bytes and passes 20 full-object comparisons plus vtable/omission checks. Only the cumulative tracking array is initialized; two tracking arrays remain untouched. Existing native allocator/save gates and six selected callers pass; save composition now uses 43 native providers.

Latest completed closure: 1,228 sources, zero duplicates, 423 unresolved at work/codex-native-startup/native-gfmain-allocator-base-closure-20261005. Earlier creation session 32929 and base session 29802 are terminal. Next: full fixed/variable hierarchy and constructors, then main constructor 0x009F20B0. Fixed allocator has size 0xC4C and its own virtual destructor slot 15; the base has none. Current fixed accessor header omits the actual hierarchy. See work/native_fixed_allocator_methods_20261005.txt, work/native_allocator_constructor_chain_20261005.txt and docs/journal/2026-10/NATIVE_RECONSTRUCTION_2026-10-05.md. Preserve independent Lua-thread notes below.

# Native allocator creation checkpoint - 2026-10-05

Native recreation remains active until the user stops it, excluding Lua and FMP analysis. Create 0x009F2870 and address-identified shutdown helper 0x009F2D20 match 156 retail bytes. Ten retail/linked cases and six provider omissions pass with native destructor, base release, singleton and allocator data linked; the allocator constructor and Win32 APIs remain recording boundaries. The shutdown helper has no established callers/address references, so live exit registration is not claimed. Baseline/shutdown state adds nine verified zero-filled bytes; allocator data gate now covers 873 bytes. Six selected native callers and existing statistics/save gates pass.

Fresh creation closure is live in exec session 32929 at work/codex-native-startup/native-gfmain-allocator-creation-closure-20261005; last inspected object count was 708. Poll that handle before making any restart decision. Last completed closure remains 1,225 sources, zero duplicates, 422 unresolved. Next: actual constructor 0x009F20B0 and coherent base/fixed/variable allocator inheritance/layouts and constructors. Evidence: work/native_allocator_constructor_chain_20261005.txt, work/native_fixed_allocator_methods_20261005.txt. Fixed accessor header currently has correct offsets but no original virtual ancestry; base size is 0xC1C and fixed size 0xC4C.

link_closure.py now supports opt-in --jobs (default remains 1), with flags resolved before parallel compilation and stable link ordering; a real three-source serial/parallel closure check produced identical sources/frontier/duplicates. Use --jobs 8 for the next fresh build, without restarting the confirmed current session solely because it is slow. See docs/journal/2026-10/NATIVE_RECONSTRUCTION_2026-10-05.md. Preserve independent Lua-thread notes below.

# Native allocator data/lifetime checkpoint - 2026-10-05

Native recreation remains active under the user instruction to continue until stopped, excluding Lua and FMP analysis. Recovered 864 bytes of allocator static table/lock storage and four functions totaling 169 exact retail bytes: allocator destructor, class-specific allocation/release and corrected inventory-trade reset. Save composition passes seven cases with 42 native sources; statistics cover all 30 allocator rows. All new dedicated gates and nine selected native callers pass. Shared allocator layout is now 0x49C and its base has the original 15 virtual slots without an invented virtual destructor. False global/allocator aliases and self-mirroring tests are preserved in backlog mislabel directories. Fresh lifetime closure completed: 1,225 sources, zero duplicates, 422 unresolved at work/codex-native-startup/native-gfmain-allocator-lifetime-closure-20261005. Next: native allocator construction/create/destroy and memory-system bootstrap. See docs/journal/2026-10/NATIVE_RECONSTRUCTION_2026-10-05.md. Preserve independent Lua-thread notes below.

# Native allocator reconstruction checkpoint - 2026-10-05

Native recreation remains active under the user instruction to continue until stopped, excluding Lua and FMP analysis. Allocator statistics 0x009F2450/0x009F2550 match 154 retail bytes and pass eighteen retail/linked cases including all 30 size classes. NewFrame 0x009F26D0 replaces a false free-function provider and print-only test, matches 23 bytes and passes three cases plus the selected catalog build. Composed binary saving passes seven cases with 41 native sources and real allocator locking. Latest native closure: 1,220 sources, zero duplicates, 424 unresolved at work/codex-native-startup/native-gfmain-allocator-stats-closure-20261005. The shared table declaration is corrected to 30 entries from retail constructor/table evidence; the closure predates this declaration correction. Next: allocator static data/critical section storage, fresh closure and Clear. See docs/journal/2026-10/NATIVE_RECONSTRUCTION_2026-10-05.md. Preserve independent Lua-thread notes below.

# Native reconstruction continuation - 2026-10-05

This native thread remains active under the user instruction to continue recreation, excluding Lua, until stopped. Twelve compressed-offset sort helpers and four save-data compaction functions now match 1,565 retail bytes; 84 sort, 12 compaction and seven composed-save cases pass. Composed save uses 39 native sources. Fresh compressed-save closure: 1,217 sources, zero duplicates, 427 unresolved; allocator statistics at 0x009F2450/0x009F2550 are the next target. See docs/journal/2026-10/NATIVE_RECONSTRUCTION_2026-10-05.md. Preserve independent Lua-thread notes below.

# Lua recovery - 2026-10-05 (in progress)

Continue Lua until the user stops. Launch only when Fable 2 is not running.
Preserve independent native-thread work and all other dirty lanes.

Promoted: Trader sounds/actor acquisition, Book teacher unpause, and Chicken
Kicking spectator greeting flag. Spectator uses existing reviewed scalar alias
adapter; no shared generator change. Four full-function/adverse tests pass in
both styles; root alias tests9/9; only Spectator.lua differs; exact seven-file
regeneration per style passes. Book full29 only teacher, root lifter90/90.
Audit371 syntax0, diagnostics1008, generic6108, machine temporaries4649.
No outer commit, installation or game launch.

Latest offline bundle:
work/codex_lua_spectator_flag_20261005/chicken-spectator-greeting-candidate-20261005
27 Lua / 32 manifest hashes; e777ce0e DLL. Book bundle remains separately owned.
Owned comparison/regeneration processes completed. Existing Guild/Arena
readable overlays differ from scratch generated baseline and were preserved.

Smoke supports --sidecar-source-dir; explicit incomplete profile rejected,
historical source default unchanged. Generated-stage corpus:232 files,4 findings.
Spectator free global fixed (six-file smoke1 -> 0); two Bordello host-global
warnings are the verified g_PresentedItemName sidecar ABI. Remaining findings:
two empty-mock-vector modulo errors. ScorpionHome's guard belongs to the
deliberately disabled reviewed artifact; generated Guild Training passes37 files.
Smoke now defaults to the same generated readable stage as packaging;
explicit --stage readable still selects the preserved reviewed artifact.
Static/mock execution is not live proof. Reports retained under
work/codex_lua_smoke_profile_20261005.

Latest review: Book GetBookSpecificArgs212 native/draft cases and106 readable
stable-termination cases pass;14 full donation style/path cases pass. No new
lowering change justified for its packed argument bookkeeping. Evidence at
work/codex_lua_book_rewards_20261005/completed_tests.json; no owned worker live.
BookReaction/DoConversation:384 original-native runs and768 draft/readable
comparisons pass, including composed0/1/3-line resource/frame exits. Promoted
root regression2 tests /768 subtests; Unicorn isolated in a subprocess.
Evidence: work/codex_lua_book_reaction_20261005/promoted_validation.json.
No generator change; fixture covers caller/wrapper behavior, not expert internals.
Latest stage-selection proof: work/codex_lua_smoke_generated_stage_20261005.
Populated-vector validation passes18 full Main cases for both affected entities;
counts1/3/7 and random0/17/32767, exact lookup index asserted. Complete unit
reports zero findings under fixtures; default empty-vector diagnostics preserved.
Evidence: work/codex_lua_populated_vectors_20261005/validation.json.
Promoted assassin cleanup: RemoveThing receives me (marker), false,true.
Retail EBP save/reloads/call proved; 42 full Main mock cases, 9 original retail
instruction-window cases, root evidence8 and native1 tests pass. Full29 units
regenerated in both styles; only assassin differs. Exact17-file/style promotion
verified; all other dirty overlays preserved. Corpus21744 and package76634 done.
Offline bundle: work/codex_lua_assassin_cleanup_20261005/trader-cleanup-candidate-20261005
37 Lua /42 manifest hashes pass; e777ce0e DLL. Not installed/game-tested.
Readability audit refreshed, syntax0. Evidence and rejected candidates retained
under work/codex_lua_assassin_cleanup_20261005. No owned worker remains after audit.
Promoted MakeTraderComment E01900: retained speaker/fallback and lookup handles,
both identities, intro swap/clear, reply byte flag, original intro speaker and
comment string lifetime restored. Complete retail helper runs768 cases with
explicit engine/CString boundaries; 3072 result/exact-effect comparisons pass
both styles/scopes. Root25 focused; broader112 +28 subtests, self/stack19 +7,
and18 existing string-return assertions pass with native/generation isolated.
Full29 baseline80894 and after85157 complete: only Trader Escort plus proven
entity alias stores in Bordello/Sick Child differ; 534 Lua parse, inputs/hashes
checked. Eight Lua and eight reports promoted; current audit371 files/syntax0,
unresolved974. Offline bundle: work/codex_lua_trader_comment_20261005/
trader-comment-candidate-20261005, 37 Lua/42 hashes, e777ce0e DLL.
No install/game test. Engine/string doubles and host ownership/runtime boundary
are recorded separately. Current pass workers terminal; rejected candidates and
Windows mixed-engine test failures retained, replaced by fully isolated checks.
Promoted DarkwoodTrader Main actor/refusal fixes: same-actor clear, retained
swap, whole-handle identity and numeric refusal byte. Eight native handle windows,
80 native actor decisions,12 native byte cases and160 generated comparisons pass;
590 full Main mock runs and root62 tests +7 subtests pass. Full29 session26233
terminal0; only DarkwoodTrader changes,534 Lua parse. Two Lua +three reports
promoted. Audit371 files/syntax0, diagnostics973. New offline bundle:
work/codex_lua_trader_actor_swap_20261005/trader-actor-selection-candidate-20261005
37 Lua/42 hashes, e777ce0e DLL; unchanged build/source/ABI provenance checked.
Native engine/CString/GC/host runtime boundaries remain separately qualified.
No installation/game launch. Current pass workers terminal.
Promoted AssassinSpawn/RockTrollTrigger float spill rule and four Lua outputs /
three reports. Native22 x87 cases,30 focused tests, broad175 +28 subtests pass;
88 Assassin comparisons and660 two-invocation RockTroll scenarios pass. Final
corrected-input full29 session15209 terminal0: only two entities change,534 Lua
parse. New offline bundle: work/codex_lua_trader_distance_spills_20261005/
trader-distance-spills-candidate-20261005,37 Lua/42 hashes, e777ce0e DLL.
Audit371 files/syntax0, diagnostics971. Source/DLL/ABI provenance verified.
Encoding correction preserves original text; only14 rule lines added. Current
workers terminal; no install/game launch.
Completed marker-vector lifetime recovery under
work/codex_lua_trader_vector_clear_20261005. Full29 session65806 terminal0;
only TraderEscort master changed each style, other28 unchanged,534 Lua parse.
Promoted two master Lua plus three reports; emitted draft/readable2592 exact
comparisons each. Native full caller240 list cases, erase20/GetAll+erase24/
destroy20/GetPos6; compiled helper144/list72/real-sol16. Focused14, related50
plus2subtests; smoke findings unchanged. Original spawn/host coroutine/game
lifetime remains outside these list fixtures. Scope-owned generic position list
preserves clear-before-refill and every thread exit without extra Lua actor refs.
New offline trader-vector-list-candidate-20261005 contains37 Lua/42 hashes,
DLLd9d7f4b9, isolated commita389811, patch0c11c1c0; nine build sources and four
RemoveThing ABI sources verified. Ledger371/syntax0/diagnostics970, generic6107;
status59 recovery/156 readability/156 human. No install/game launch; current
pass workers terminal. Promoted generic CreateCreature final native flag recovery:
work/codex_lua_trader_spawn_flag_20261005. Call-stack7/flag-window6 and Arena
staged-window18 native cases;85 call sites/14 units audited, all constants0/1.
22 exact compiled method/12 real-sol cases;2592 emitted Trader comparisons/style.
Native flag meaning unclassified; omitted/nil host flag remains false. Optional
signature overlay retains flag; exact Dragon guard updated; generic staged
SUB41 operand recovered from retail push1/GetPos. First corpus4735 failed guard,
retained. Full29v2 session53610 terminal0; twelve units,26 files/style,78 draft
call additions only;534 Lua parse. Readable26 canonical comparisons (versioned
local renames and reviewed immutable grade copy);19 grade fragment cases.
48 smoke runs/24 exact JSON comparisons unchanged. Focused27+5 subtests;
broader65+5, one baseline normal Sister camera failure proved with nonempty
identical sources.88 outputs/reports promoted and hash verified; hand-reviewed
Guild tree preserved. Arena pre-existing two immutable timer-alias reads audited.
New offline trader-create-flag-candidate-20261005:37 Lua/42 manifest hashes,
clean sidecar b05e856, DLL9f096cc9, actual combined patch09fc84a3; nine build
source hashes and retained RemoveThing bodies verified. Ledger371/syntax0,
970 diagnostics;6111 generic temps. final_validation.json records qualifications.
No install, game launch or outer commit. Owned current-pass workers terminal.
Sister camera fixture contract repaired (work/codex_lua_sister_camera_fixture_20261005):
production Speak overload returns Speak_Blocking bool; prior mock returned nil.
Four normal/host-abort/caller-abort/issue-failure cases now check exact camera,
movie/timer/resource cleanup and intended wait path in draft/readable. Camera10
and related58+5 subtests pass,68 total; prior failure resolved. Only test fixture
changed versus full29v2 input hashes; generator/evidence/sidecar/Lua unchanged.
No new package or ledger regeneration needed for the camera fixture pass.
Promoted SetBrainState void(long) recovery E0A510..E0A81A:
work/codex_lua_trader_brain_state_return_20261005. Genuine Ego_r oracle and
retail four RET4/direct caller overwrite prove resultvoid. Generic pinned
reviewed prototype supports existing helper,13 adverse tests pass.504 complete
retail helper runs and504 full emitted/native comparisons each style preserve
ordered mutating calls, native stack colour bytes, fields and term checks;
explicit engine/CString/getter/host boundaries. Broader90 tests pass.
Full29 session92057 terminal0:only DarkwoodTrader changes in both styles,
534 parse; draft delta four stale returns only. Four smoke runs exact before/
after JSON same, existing modulo-zero/free-global findings retained. Five
outputs/reports promoted, hash verified. New offline37-Lua/42-hash bundle:
trader-brain-state-void-candidate-20261005 under this workdir; reused clean
b05e856 sidecar, DLL9f096cc9/patch09fc84a3, nine source hashes verified.
Ledger371/syntax0,970 diagnostics,6093 generic temps. final_validation.json.
No install, game launch or outer commit. Owned pass workers terminal.
Full WatchForSurprisingBalverines spawn caller proof complete at
work/codex_lua_trader_spawn_caller_20261005/final_validation.json:26 native
actor helper cases,2400 complete native caller and2400 ordered comparisons
per Lua style. Actual temporary destructor/IsAlive and list helpers composed;
references/free/stack balance verified. Engine factories/CString/comment/
allocator/base/host doubled; getter traces excluded. Scratch-only changes;
no generator,sidecar,promotion,install or launch. Rejected camera fixture
retained; corrected reset. Generic empty enemy-target binding recovered at
work/codex_lua_trader_empty_target_20261005/final_validation.json:30 native
GSI/wrapper cases show component8 null setter/notify;48 exact direct/real-sol
cases avoid old shared_ptr nil crash.2400 empty-nearest full caller and2400
ordered comparisons/style;90 tests pass. Clean sidecar5a7d2e1,parentb05e856,
DLL4423cdf9/patch1bff1af2;37-Lua/42-hash offline empty-target bundle. Nine
source/66 vendor hashes audited;1829 corpus inputs unchanged. Final build
23391 terminal0 after restoring retained newline bytes. No generator/Lua
promotion/install/game/outer commit. Owned current-pass workers terminal.
Generic empty-relative nearest recovered at
work/codex_lua_trader_empty_relative_20261005/final_validation.json:24
complete original nearest callee/fallback/x87/lifetime cases;72 exact direct/
real-sol owned/borrowed/nil/guard/allocation/output cases.800 failed-create
full caller runs compose original GSI guards;800 ordered comparisons/style
pass. Clean sidecardf094fc,parent5a7d2e1,build9993 terminal0; DLLfda5b7c0/
patchfbc7fd48,new37-Lua/42-hash offline empty-relative bundle.1829 inputs
unchanged; root90-test report reused with hash, new binding compiled/tested.
Engine query/comment/host boundaries doubled. No root generator/Lua changes,
promotion/install/game/outer commit; all owned current-pass workers terminal.
Generic owned Thing output cleanup recovered at
work/codex_lua_trader_actor_ownership_20261005/final_validation.json:stock16
diagnostics find missing zero-count destroy/free and owned-null leaks. Native
scalar flags0 ten/IsAlive16 cases; exact16/adverse33/SDK16/real-sol forcedGC16
pass,81 compiled cases. Current virtual destructor before captured heap free;
pending owner exception/control-block cleanup, borrowed-null copy rules.
Clean sidecarcfdc974,parentdf094fc; one wrapper method,build19159 terminal0.
New37-Lua/42-hash offline actor-owner package;1829 inputs unchanged, prior
root90 test evidence reused; nine source/66 vendor/retained ABI hashes audited.
Native callbacks/base/allocator doubled; eventual forced-GC cleanup only.
No generator/Lua promotion/ledger/install/game/outer commit; owned jobs terminal.
Actor frame ownership gap confirmed at
work/codex_lua_trader_frame_ownership_20261005/final_validation.json:original
full caller releases tocount2 before needed0 frames. Current complete draft/
readable actual-sol coroutines retain count3 across4 frames/style despite
forcedGC; thread end/drop/GC returns2. GSI/factory/resource/host boundaries
doubled; native actor/vector/helpers executed. Scratch proof only, no new
generator/sidecar/Lua/package/install/game/outer commit; probes terminal.
Original99A2E0 composed proof now shows retired vtable01231710; surviving
Lua aliases require empty Thing vtable restoration and no duplicate cleanup.
Deterministic owned-output candidate validated at
work/codex_lua_trader_deterministic_owner_20261005/checkpoint.json:124
compiled SDK/sol/scope/factory cases, DLL build terminal0; clean sidecar 5559d671.
1152 complete native helper copy traces retained separately. No root code/Lua
changes or package/install/launch. Next existing task: reviewed caller lowering
and separate helper local-copy recovery, native-vs-emitted frame proof
with safe surviving aliases and exact actor/nearest/list order, then reviewed
lowering/full29 acceptance. See ownership_requirements.json; do not adopt
borrowed hero/state handles or substitute forcedGC for native release timing.
Next full29 baseline: brain_state_return workdir corpus_final/after and
corpus_final_comparison.json/inputs; completed bundles preserved.
External FableForge processes were observed and left untouched.
Caller deterministic cleanup promoted:full29/534 parse,106 related tests,
5600 traces/style;actor/nearest count2 before4 needed0 frames/style and16
callback-error cases. Initial5142c4b37-Lua bundle superseded for runtime use:
cold-registry first publication error corrected in clean sourcec63bd05;
new DLL build terminal0, two fresh64-case probes pass. Final focused reruns and affected regeneration completed. Registry-safe
37-Lua/42-hash package verified; sourcec63bd05, DLLda9cc957. Current evidence:
deterministic_owner workdir final_validation.json and runtime_plan.md.
No retail visual proof; runner host mismatch remains explicit.
Normalized next full29 baseline:deterministic_owner workdir corpus_final/
validated,validated_corpus_comparison.json; final_inputs.json has current pins.
Latest user explicitly authorizes visual launch while Fable2/xenia is absent.
Original accepted-save load crashed identically with candidate and stock retail
at009E5C63: persisted integer CoreQuestWaiting was FALSE. Scratch-only repair
to0 loaded normal gameplay; original staged profile restored, DLLs unchanged.
Repaired candidate PID18856 loaded gameplay/live TraderEscort host; active
quest/boast9 confirmed, introduction and injured-trader/marshes scenes
played naturally. First combat incomplete: low-health hero resurrected,
SCARED trader killed. Owned game closed/profile restored/DLLs unchanged.
Current state/backup: deterministic_owner/repaired_candidate_visual_session.json.
Full quest completion/sound proof remains outstanding. No install/outer commit.
Current journal: docs/journal/2026-10/LUA_RECOVERY_2026-10-05.md.

---

# Paused for shutdown at user request - 2026-10-04

Native recreation is paused. Resume only when requested; Lua and FMP analysis
remain excluded. This session's closure process has finished.

Latest selected save closure: 1,194 sources, zero duplicates, 436 unresolved;
artifacts: work/codex-native-startup/native-gfmain-definition-save-closure-20261004.

Unregistered completed work: CDefStringTable_WriteTable_009d4a50.cpp and
gate_defstring_write_table.py match all 87 retail bytes after relocation and
pass three linked cases (stream lengths 0, 4, 32). Register the provider, gate
and manifest evidence on resume.

Draft only: CDefStringTable_SaveTable_009d4ce0.cpp has not been compiled or
tested. Next recover its typed string-map erase dependency at 0043020A and
verify disk/output stream lifetime and lock-after-save behavior. The historical
LTextTreeWalkThrough label at that address is incorrect; do not reuse it.

---

# Native definition save checkpoint - 2026-10-04

SaveBinaryDefinitions passes seven linked/retail output and block-boundary
cases, including exact 32 KiB, overflow, oversized and empty defs. Its actual
block helper, context Init and stream Clear pass composition with compressor
input/capacity checks. Cleanup passes four cases; tree erase matches 75 bytes.
Providers and gates are registered. Save closure completed with 1,194 sources,
zero duplicates and 436 unresolved. Native save dependencies and startup
frontier remain for the next requested session. Lua/FMPs excluded.

---

# Native definition sorting checkpoint - 2026-10-04

Insertion auxiliary matches 171 bytes and partial-sort matches 244 bytes
after relocation. Partition passes 29 exact retail order/split comparisons;
61 linked helper cases preserve name/index-vector payloads. Heap fallback
passes 82 linked/retail order cases. Complete sort links eight providers and
passes 27 cases including forced heap fallback and the 16-element boundary.
Latest completed closure before partial-sort: 1,187 sources, zero duplicates,
433 unresolved. Index partial-sort then refresh; continue manager save and
destructor work. Native scope only; Lua/FMP analysis excluded.

---

# Native text persistence checkpoint - 2026-10-04

SaveText and bool output match 97 retail bytes after relocation. Bool input
passes twelve composed linked cases and twelve retail executions. Named-field
LoadText passes six linked and retail cases, preserving parser state and LF
while handling repeated fields, flags and malformed terminators. Gates and
providers are registered. The byte/IME closure is complete: 1,184 sources, zero duplicates and
440 unresolved. Byte binary input and eight-case composed byte text gates pass.
Definition cache and IME alpha globals now match retail data; refresh closure
after indexing these globals. Continue definition-manager sort/save/destructor
dependencies.
Continue native reconstruction; Lua and FMP analysis are excluded.

---

# Native loader checkpoint - 2026-10-04

Main and nested definition compilation, child instantiation and binary LoadDef
now have typed behavioral providers, linked fixtures and retail execution checks.
Main/nested compilation composition links 21 native providers. LoadDef preserves
MRU behavior, progress stages, reference replacement and resource registration;
its typed memory-input destructor matches 11 bytes. Last selected closure:
1,174 sources, zero duplicates, 445 unresolved in the completed loader closure. Continue the remaining persistence text dispatch and
definition-manager dependencies. Work remains native; Lua/FMPs excluded.

---

# Native definition body checkpoint - 2026-10-04

CompileDefinition passes seven linked and retail cases using twenty native
providers. Parent specialization requires Setup, preserves the target Template
bit, and retains copied defaults state. Nested blocks retain LF bytes while other
bytes are removed before field transfer. CompileSubDefinition and LoadDef remain
mocked frontiers. CheckUnknownSymbols separately passes five cases with a real
parser copy and preserves the original parser state. Parser comment-list copy
ownership passes 0/1/3-node cases. Delimiter/whitespace extraction has 48 retail
comparisons; two substring wrappers match 162 bytes total. Typed def-string
conversion and SetChar match another 57 bytes. Continue native reconstruction;
exclude Lua and FMP analysis. Latest authoritative selected closure predates
these providers: 1,161 sources, zero duplicates, 446 unresolved.

---

# Native definition compiler checkpoint - 2026-10-04

Enum symbol parsing, CompileString orchestration and InstantiateDef have linked
fixtures and matching retail Unicorn cases. Integer reads match 111 retail bytes
after relocation; conditional operator consumption passes eight retail comparisons.
The selected startup closure now has 1,157 sources, zero duplicate providers and
447 unresolved symbols (before the latest integer-read and consume providers).
Continue CPersistTraitsSLONG expression input, then CompileDefinition. Preserve
right-associative arithmetic and shifts; archived FMP analysis belongs to another session.

---

# Native parser checkpoint - 2026-10-04

Latest: typed parser Init (55 bytes), ReadAsString (145 bytes),
CalcInQuotes (43 bytes) and Error (170 bytes) match retail after relocations
where required. Symbol scanning, keyword search, whitespace and token dispatch
have behavioral implementations with linked fixtures. The parser callback ABI
is fastcall with a const CCharString reference; the existing constructor fixture
passes after that correction. Identifier and number scanners pass twelve linked cases. Numeric converters
pass nineteen retail differential fixtures at x87 0x027F. Substring assignment
matches 45 bytes; ReadAsSymbol matches 145 bytes. Lookahead preserves retail
success-only cursor restoration. Continue native enum/definition compilation.
Established selected closure checkpoint: 1,150 sources, zero duplicates,
448 unresolved (before the latest lookahead and enum accessor providers).

Recovered CStringParser SetStringPos, SkipUntilWholeString and SkipPastWholeString. Ten position cases and six keyword cases pass; keyword cases also pass under retail Unicorn execution with ASCII CRT boundaries. Behavioral grade. Corrected 0x009B9C00's prior creature-serialization label. Continue ParseStringForSymbols and remaining parser dependencies. Forum definitions are extracted under work/forum-reference-review/defs/Defs; FMP analysis remains excluded.

---

# Native recreation - 2026-10-04

Latest: five wide filename sort helpers recovered with exact retail parity
after relocations, totaling 804 bytes. Linked insertion, introsort,
partition, and fallback cases pass. Selected GFMain closure:
`work/codex-native-startup/native-gfmain-definition-partition-fallback-closure-20261004`,
1,134 sources, zero duplicates, 448 unresolved. Next: the wide-string heap
construction/adjustment/sorting providers and the definition text parsers.
See the [native journal](NATIVE_RECONSTRUCTION_2026-10-04.md#wide-filename-sort-helpers-recovered).

---

# Prior native checkpoint - 2026-10-04

Latest: recovered the native definition symbol and compile path scans plus
the wide filename sort wrapper. A linked six-file fixture passes order,
comment removal, parser arguments, empty lists, and provider omissions.
VC7.1 bodies are 616/625, 589/605, and 112/117 bytes respectively;
these are behavioral recoveries, not byte parity. Selected GFMain closure:
`work/codex-native-startup/native-gfmain-definition-wide-sort-closure-20261004`,
1,129 sources, zero duplicates, 448 unresolved. Next: the three wide-sort
helpers and the two definition text parsers. The community SDK has candidate
hero offsets but no compiler source. See the
[native journal](NATIVE_RECONSTRUCTION_2026-10-04.md#definition-path-scans-and-wide-filename-sort).

---

# Prior native checkpoint - 2026-10-04

Latest: recovered native `GFRemoveComments` at 0x00A60410 for the definition
compiler path. Its eight-case linked fixture and retail Unicorn oracle pass,
including line count preservation and unterminated comments. VC7.1 emits 262 bytes against
245 retail bytes (behavioral recovery; no byte parity). Continue with
`CreateSymbolsFromPathList` and `DoCompilePathList`. See the
[native journal](NATIVE_RECONSTRUCTION_2026-10-04.md#definition-source-comment-removal-recovered).

---

# Prior native checkpoint - 2026-10-04

Latest: recovered the native definition memory shrink path. The typed
`CDefinitionManager::RemoveUnusedMemory` matches 182 retail bytes; its
`CSymbolMap` wrapper matches eight; six array/map shrink and storage
release helpers match another 659 bytes after relocations. Linked
heap-backed compaction and repeat-call fixtures pass. Selected GFMain
closure:
`work/codex-native-startup/native-gfmain-definition-shrink-full-closure-20261004`,
1,125 sources, zero duplicates, 445 unresolved. The three transient
template shrink dependencies are resolved. See the
[native journal](NATIVE_RECONSTRUCTION_2026-10-04.md#definition-memory-shrink-path-recovered).

---

# Prior native checkpoint - 2026-10-04

Latest: replaced the anonymous 0x009AC9E0 stand-in with the typed
`CDefinitionManager::ScrambleDefs(bool)` no-op called by
`CMainGameComponent::Run`. Its three bytes match retail exactly;
linked no-op, provider omission, and selected catalog gates pass.
Selected GFMain closure:
`work/codex-native-startup/native-gfmain-definition-scramble-closure-20261004`,
1,120 sources, zero duplicates, 446 unresolved. See the
[native journal](NATIVE_RECONSTRUCTION_2026-10-04.md#definition-scramble-no-op-retyped).

---

# Prior native checkpoint - 2026-10-04

Latest: corrected retail 0x009ACB10 to
`CDefinitionManager::ClearSymbols`. The typed eight-byte forwarder
loads `PSymbolMap.Data` at +0x58 and tail-calls the real symbol-map
clear method at 0x009E5250. The false particle helper catalog entry and
its stand-in were removed. Retail parity, linked delegation, provider
omission, and selected catalog build pass. Selected GFMain closure:
`work/codex-native-startup/native-gfmain-definition-clear-symbols-closure-20261004`,
1,119 sources, zero duplicates, 447 unresolved. See the
[native journal](NATIVE_RECONSTRUCTION_2026-10-04.md#definition-symbol-clearing-corrected).

---

# Prior native checkpoint - 2026-10-04

Latest: retyped `GFLimitReturn<double>` (0x0041919C), replacing its naked
byte emitter with a VC7.1 template specialization. The 37-byte body has
the same 16 decoded instructions in a different order for the second
comparison; linked edge cases, provider omission, and selected catalog
build pass. The decorated symbol now closes one render dependency.
Selected GFMain closure:
`work/codex-native-startup/native-gfmain-limit-return-double-closure-20261004`,
1,118 sources, zero duplicates, 448 unresolved. See the
[native journal](NATIVE_RECONSTRUCTION_2026-10-04.md#render-double-limit-helper-retyped).

---

# Prior native checkpoint - 2026-10-04

Latest: the bank-info map insertion chain is typed and linked through
`operator[]`. Successor/predecessor iterators, hinted/unhinted
`InsertUnique`, `InsertNode`, and its pair copy helper pass retail parity
and linked behavior gates. The prior alias predecessor address was
corrected to 0x009AA370; bank-info predecessor is 0x009AA320. Selected
GFMain closure:
`work/codex-native-startup/native-gfmain-bank-info-insert-closure-20261004`,
1,117 sources, zero duplicates, 449 unresolved. The seed list in this
closure fixes the old alias filename. See the bank-info insertion section
in the [native journal](NATIVE_RECONSTRUCTION_2026-10-04.md#bank-info-insertion-chain-recovered).

---

# Prior native checkpoint - 2026-10-04

Latest: `CBankFileManager::CBankInfoMap::operator[]` at 0x009AC5D0
is now typed and matches all 302 retail bytes after 21 call relocations.
`CBankInfo` gained the retail default state: four empty `CWideString`
members and a null registered-bank pointer. Linked existing-key and
missing-key cases pass, including constructor/copy/destructor counts
and two provider omissions. The selected GFMain closure is
`work/codex-native-startup/native-gfmain-bank-info-index-closure-20261004`:
1,112 sources, zero duplicates, 450 unresolved. The bank-info hinted
`InsertUnique` is now the remaining bank-map frontier item.

Previous checkpoint: the retail-bank alias insertion chain is typed. Its iterator
advance at 0x009A9860 is a 61/62-byte near match; retreat at 0x009AA320
has identical normalized opcodes across 79/79 bytes. `InsertNode` at
0x009AB2F0 and unhinted `InsertUnique` at 0x009AB3F0 match all 250 and
254 bytes after call relocations. Hinted `InsertUnique` at 0x009ABDB0 is
a 672/668-byte near match (0.956 opcode similarity). The composed gate
links the actual `operator[]`, lower-bound, iterator and insertion
providers; it passes insertion, duplicate and wrong-hint cases with
seven provider-omission checks. The selected GFMain closure is
`work/codex-native-startup/native-gfmain-retail-bank-alias-insert-closure-20261004`:
1,110 sources, zero duplicates, 450 unresolved. Continue from its
`closure_sources.txt` and `frontier.txt`; bank-info `operator[]` remains
on this map frontier.

Previous checkpoint: the contained-bank insertion chain is linked end to end.
`CContainedBankMap::InsertUnique(iterator hint, value)` at 0x009AB870
is a 672/668-byte near match (0.956 normalized opcode similarity).
`InsertUnique(value)` at 0x009AB020 relocation-matches all 254 bytes.
The hint fixture now uses the real unhinted fallback and checks empty,
duplicate, begin, end, interior and wrong-hint paths. The unhinted
fixture checks ordered insertion, duplicate preservation and returned
flags. Both gates pass with provider omissions. The selected GFMain
closure is now
`work/codex-native-startup/native-gfmain-contained-bank-insert-complete-closure-20261004`:
1,105 sources, zero duplicates, 451 unresolved. Continue from its
`closure_sources.txt` and `frontier.txt`; bank-info `operator[]` and
alias-map `InsertUnique` are the remaining bank-map frontier items.

Previous checkpoint: recovered three prerequisites for contained-bank hinted insertion.
`CContainedBankMap::iterator::operator--` at 0x009AA2D0 emits 79/79
bytes with identical normalized opcodes (four register-byte differences);
linked predecessor cases pass. `CreateNode` at 0x009AA9E0 corrects a false
creature-action clone label and relocation-matches all 76 bytes, with
linked allocation and field-copy checks. `InsertNode` at 0x009AAF70
relocation-matches all 173 bytes and passes linked tree-link/rebalance
cases with the recovered node factory. All three have provider-omission
gates and symbol-index entries. The selected GFMain closure is
`work/codex-native-startup/native-gfmain-contained-bank-insert-prereqs-closure-20261004`:
1,099 sources, zero duplicates, 452 unresolved. These providers remain
outside the closure until `InsertUnique` calls them.

Previous checkpoint: `CBasicRetailBankAliasMap::operator[]` at 0x009AC700 is now a
typed 150/149-byte near match (0.945 normalized opcode similarity).
Linked empty-map, existing-key and missing-key cases pass, as do both
provider omissions. Its real `LowerBound` provider enters the GFMain
closure. The contained-bank iterator advance at 0x009A9820 is also typed
as a 61/62-byte near match with linked tree traversal cases; it remains
outside the selected closure until `InsertUnique` is recovered. The
selected closure is now
`work/codex-native-startup/native-gfmain-retail-bank-alias-index-closure-20261004`:
1,099 sources, zero duplicates, 452 unresolved. Alias and contained-bank
`InsertUnique` are both in the frontier.

Previous checkpoint: the three retail bank-map `LowerBound` methods at 0x009AAD60,
0x009AADB0 and 0x009AAE00 are now typed. All are exact 65-byte matches
after masking one comparator call relocation. A linked fixture covers empty
maps and searches before, at, between and after stored keys; three
provider-omission links fail as expected. False STL-map catalog entries and
sources were removed. The selected GFMain closure is now
`work/codex-native-startup/native-gfmain-bank-lower-bound-closure-20261004`:
1,097 sources, zero duplicates, 452 unresolved. Continue from its
`closure_sources.txt` and `frontier.txt`; contained-bank `InsertUnique` and
bank-info/alias `operator[]` remain in the frontier. Retail
`CContainedBankMap::operator[]` at 0x009AC530 calls `InsertUnique` at
0x009AB870, whose current manifest label is still generic.

Previous checkpoint: retail-bank ownership cleanup is now typed. The
`CCountedPointer<NBankFileManager::CRetailBank>::DeleteData` callback at
0x009A90B0 matches all 11 retail bytes and dispatches virtual deletion.
The retail-bank vtable at 0x0129B010 identifies 0x009AC7A0 as its scalar
deleting destructor, which matches all 30 bytes after two call fixups;
0x009AC7C0 is the true `CRetailBank` destructor, a 149/161-byte near match.
The contained-bank map's recursive erase at 0x009AA780 is a 59/59-byte
instruction match. False landscape/text-tree labels and their obsolete
catalog source were corrected. Linked teardown, ownership, recursion and
provider-omission gates pass, as do selected VC7.1 builds and the existing
retail-bank constructor, threaded-file reset and OpenRetailBank fixtures.
The previous selected GFMain closure was
`work/codex-native-startup/native-gfmain-retail-bank-lifetime-closure-20261004`:
1,096 sources, zero duplicates, 453 unresolved. Continue from its
`closure_sources.txt` and `frontier.txt`.

Previous checkpoint: `GFGetBuildNumber2` is now retyped onto genuine `CWideString`,
`CDiskFileWin32`, `CAFile` and `CCharString` constructors, assignment and
destructors. Its old build-number placeholder header was removed. The typed
160/162-byte body has 0.955 normalized instruction similarity after four
retail absolute-address fixups. Linked present/absent-path cases, an
omitted-provider check and the selected VC7.1 build pass. This removes
eight invented frontier symbols and exposes one genuine constructor.
That `CCharString(CAFile*,long)` constructor at 0x0099FA50 is now a 52/52-byte
instruction match with linked forwarding cases. The full-build-version
`CCharString` global at 0x013B85E0 is also typed, with retail `"Unknown"`
initializer and CRT destructor evidence. The selected GFMain closure is
`work/codex-native-startup/native-gfmain-charstring-from-file-closure-20261004`:
1,093 sources, zero duplicates, 455 unresolved. Continue from its
`closure_sources.txt` and `frontier.txt`.

Previous checkpoint: `GFGetBuildNumber2` at 0x00401F30 no longer refers to the
placeholder `CWideString::PathExists`. Its retail call at 0x00401F46
targets the already recovered static `CAFile::PathExists`; the candidate
header, source and fixture now use that symbol. Focused linked and
provider-omission checks and the selected VC7.1 build pass. The body is a
162-byte near match (0.909 normalized opcode similarity); the earlier
`RELOCATION_MATCH` manifest grade was overstated and is corrected. The
selected GFMain closure is now
`work/codex-native-startup/native-gfmain-build-number-path-closure-20261004`:
1,091 sources, zero duplicates, 464 unresolved. Continue from its
`closure_sources.txt` and `frontier.txt`. A scratch append-character
candidate at `work/codex-native-startup/CWideString_AppendCharacter_0099b9a0.candidate.cpp`
is only 150/203 bytes with 0.683 similarity; it was not promoted.

Previous checkpoint: `CAFile`'s cached current-path `CWideString` at 0x013BCA10 now has
a typed data provider. Retail CRT thunks 0x01223D80 and 0x0122A4E0
construct and destroy it; the linked global and omitted-provider gate
passes. The selected GFMain closure is now
`work/codex-native-startup/native-gfmain-cafile-current-path-closure-20261004`:
1,091 sources, zero duplicates, 465 unresolved symbols. Continue from
its `closure_sources.txt` and `frontier.txt`.

Previous checkpoint: GFMain's 0x013CA7DC compiled-definition path is now a typed
`CWideString` global. The 0x01224050/0x0122A6B0 CRT thunks prove its
construction and destruction; its linked data and omitted-provider gate
passes. The 0x013BCA20 wide-string instance counter is one four-byte
object with an alias for an older C symbol, validated by a linked
read/write fixture. The selected GFMain closure is now
`work/codex-native-startup/native-gfmain-wide-count-closure-20261004`:
1,090 sources, zero duplicates, 466 unresolved symbols. Continue from its
`closure_sources.txt` and `frontier.txt`.

Previous checkpoint: the retail global bank manager at 0x013CA79C is now one typed
0x38-byte object under both native symbol aliases. CRT initializer and
destructor thunks identify `CBankFileManager` lifetime methods at 0x009A7AC0
and 0x009A7B70, replacing two false labels. The constructor is a 172-byte
near match, and the destructor is a 229-byte near match; linked fixtures,
provider omissions, and selected VC7.1 builds pass. Its three container
cleanup dependencies are recovered: alias-map erase at 0x009AA830 and
wide-path erase at 0x009AA880 both have identical normalized instruction
sequences to retail; retail-bank
list clear at 0x009AAE50 is an 83/89-byte near match. A composed fixture
checks recursive destruction, shared ownership release and repeat clear.
The selected GFMain closure is now
`work/codex-native-startup/native-gfmain-bank-manager-containers-closure-20261004`:
1,088 sources, zero duplicates, 468 unresolved symbols. Continue from its
`closure_sources.txt` and `frontier.txt`. No full startup execution claimed.

Previous checkpoint: retail 0x009AA7C0 was falsely labeled as a text-tree walker.
The call from `CBankFileManager::Close` and the 100-byte body identify it
as `CBankInfoMap::Erase`. The typed VC7.1 source matches all 100 bytes
after seven relocations. A linked fixture checks right-subtree-first
removal and the five string destructors per node. A composed fixture links
`Close` with the real erase method and checks map cleanup. Two provider
omissions and the selected build pass. The selected GFMain closure is
`work/codex-native-startup/native-gfmain-bank-info-erase-closure-20261004`:
1,082 sources, zero duplicates, 470 unresolved symbols.

Prior checkpoint: `CBankFileManager::Close` at 0x009A7990 is a 132/135-byte near
match with 0.956 normalized opcode similarity. A linked fixture covers
empty state, reverse-order retail-bank release, shared ownership, map
clear, and repeat close. Provider omission and selected VC7.1 build pass.
The selected GFMain closure is now
`work/codex-native-startup/native-gfmain-bank-manager-close-closure-20261004`:
1,081 sources, zero duplicates, 471 unresolved symbols.

Previous checkpoint: `CBankFileManager::ReadQuotedString` at 0x009A77D0 is a
249/255-byte near match with 0.958 normalized opcode similarity. Its
linked fixture covers missing, normal, empty and unterminated quotes plus
the stream overflow path; provider omission and selected VC7.1 build pass.
`ReadCBYTE` is forced inline in the shared stream header to reproduce
retail's caller shape. `OpenIniFile` still compiles to 1,732 bytes with
0.968 opcode similarity after that header change. The latest GFMain
closure is
`work/codex-native-startup/native-gfmain-bank-read-quoted-closure-20261004`:
1,080 sources, zero duplicates, 472 unresolved symbols.

Earlier this turn, `CCharString::operator=(char)` at 0x0099F040 gained an exact
89-byte relocation match, a linked five-case fixture, provider omission,
and a selected candidate build. It is cataloged but is not on the current
GFMain link frontier. `CCharString::RemoveWhiteSpaceBeginEnd` really is at
0x0099F560: the retail bank-file caller passes its string as `this`, and
the 16-byte body calls the leading and trailing trim methods. The former
`_Dest_val` catalog label and source at that address were false. The typed
wrapper now passes exact relocation parity, linked order/receiver behavior,
provider omission, and selected build. Its two trim callees are now
recovered: the leading method is exact at 117 bytes, and the trailing
method is a 134/135-byte near match. Their linked fixtures and a composed
wrapper fixture pass. That checkpoint's selected closure is
`work/codex-native-startup/native-gfmain-charstring-trim-sides-closure-20261004`:
1,079 sources, zero duplicates, 473 unresolved symbols.

Resumed non-Lua GFMain closure. Genuine `CCompressorZlib::InternalCompress`
0x00A3AA40 promoted: full 192-byte relocation match, seven relocations,
16 linked behavior cases plus provider omission, selected candidate and
existing zlib-wrapper gates pass. Focused symbol index refreshed. Closure:
1,076 sources / zero duplicates / 474 unresolved after six typed display
globals, zlib init/end/error-message providers, full C++ deflate and tree
cores, exact 21-byte `z_compressBound`, and 169-byte relocated `z_compress2`.
Five deflate and 16 tree bodies match retail after relocation; the composed
fixture passes seven compression round trips with three provider-omission checks.
The 69-byte CCRC instance method and generated 1,024-byte table also pass
retail and linked checks. Six typed graphics/recording startup settings pass
their data gate; three GFMain runtime flags now share their semantic caller
names and physical storage; the retail-bank flag now has one PDB-named provider
across native callers. No deflate/tree or CRC symbol remains on the focused
GFMain frontier. The real `CDataOutputStream::WriteEBOOL` now matches 28
retail bytes, and `CFrontEndDef::GetSizeofClass` matches six; both link into
the closure. The real `CConsoleCommandBase::GetFormattedName` now matches
26 retail bytes, and its `CCharString` suffix global at 0x013CAA48 has the
retail `"("` initializer and destructor thunk. Both focused gates pass;
operator+ 0x0099F570 now has a 126-byte near retail match (retail 130),
three linked lifetime cases, an omission check, and a selected catalog pass.
The latest selected-source closure is at
`work/codex-native-startup/native-gfmain-bank-info-erase-closure-20261004`;
refresh it with `link_closure.py --seed-file` and its `closure_sources.txt`.
The composed console formatter/operator/suffix fixture passes two linked
cases and three provider omissions. The four-byte zero-filled
`CCharString` instance counter at 0x013BD800 has a typed provider and
focused gate. `CCharString::GetSubString` is retyped onto the shared const
method, 150/148-byte near match, with three linked lifetime cases. Its exact
default-constructor COMDAT replaces the standalone constructor in the
selected closure to avoid a duplicate.
`GFIntToCharString` and its `CCharString::Sprintf` callee match all 116/172
retail bytes; a composed five-integer fixture and two omissions pass. The
console command `GetHelp` at 0x009EA7D0 is now a 313/313-byte,
93/93-instruction near match (opcode similarity 0.903). Three linked
parameter-count cases and a provider-omission check pass. Its retail vtable
call is `GetTypeAsString` at slot 3. The parameter-type helper at 0x009EA740
also matches all 136 retail bytes including the switch table after 16 address
fixups; nine linked enum/default cases and provider omission pass.
Their composed linked fixture passes three parameter counts and two provider
omissions.
`CConsoleVariable::GetHelp` at 0x009EA3C0 is a 206-byte, 62-instruction
near match with five linked type cases. Its `CGenericVarRef::PrintToString`
callee at 0x00A63C00 matches all 180 retail bytes after 17 address fixups,
including the switch table. Nine direct and six composed linked cases pass;
both providers have omission checks.
The classless console callback at 0x00415040 and the
`CConsoleCommand<CConsole>` callback at 0x009EFEB0 match 12 and 15 retail
bytes exactly. Linked fixtures confirm fastcall parameter forwarding, the
bound receiver, and provider omissions. Both leave the GFMain frontier.
`GFGetLastWindowsError` at 0x00A5ED60 matches all 136 retail bytes after ten
address fixups. The linked Win32 fixture covers a known system error and a
missing-message code; provider omission and selected build pass. It leaves
the frontier without a new native dependency.
`CBasicString<char>::operator==(const char*)` at 0x004115A0 is retyped from
a local stand-in to the shared class with the same exact 88 retail bytes.
`CCharStringData` now forwards equality to it inline. Six linked cases and
provider omission pass; the artificial data-wrapper symbol leaves the
frontier.
`CCharString::GetAsUpperCase` at 0x0099EE20 is a 277/278-byte near match
with 0.985 opcode similarity. The linked fixture covers empty, shared,
allocation-failure and extended-byte behavior; provider omission and selected
build pass. The false `ToUpper` manifest label is corrected. It leaves the
frontier without a new native dependency.
String-plus-literal 0x0099F600 is a 126/130-byte near match; literal append
0x0099F100 matches the retail 122-byte instruction sequence after normalized
register allocation. Both linked fixtures, omissions and selected builds pass.
Full startup execution remains open.
Detail: [native journal](NATIVE_RECONSTRUCTION_2026-10-04.md).
Lua recovery has resumed; its current checkpoint is below.

---

# Lua recovery - 2026-10-04 (in progress)

User resumed continuous Lua work. Batch47 MansLover no-letter dialogue state
promoted: native13,824, four-way864, 16 stale/mutation checks, one entity
change in14Lua/style, equal smoke. Batch48 departure optional speech RNG
test-only: original72 branch layouts and96 reachable draft/readable cases;
one draw when LeavingRegion, even roll speaks. Batch49 SickChild jabber
index promoted: original96 transitions, repeated/missing/finish/wrap cases,
8 stale reviews, only SickChild changes in14Lua/style, clean/equal smoke.
Root regressions pass. Shared converter and sidecar source unchanged.

Batch50 SickChildsMother speech table loads promoted: three native-gated
resource-table assignments removed after their paired `me:Speak` restorations.
All29 registered units compared; only Mother changed after fixed-seed Arena
recheck. Related overlay 9,407 passed; root adverse review 10 passed; Mother
smoke equal in both styles and 256 before/after behavior cases unchanged.
Shared member-speech adapter gained the optional reviewed dead-load gate.
Batch51 Witch speech table loads promoted: five more native-gated assignments
removed after paired speech recovery. Full29 fixed-seed comparison changed
only Witch; original first-three speech12 layouts, 480 before/after traces,
10 adverse review checks, equal 13-file smoke, related overlay 9,674 passed.
The shared gate now verifies the owner +0x14 parent-register path.
Batch52 SummoningTheShip FadeInThing promoted: reviewed lowering of two
retail helper calls plus a scratch x86 sidecar binding to 0x00CBE7AA. Native
helper probe measured 30 frames across eight layouts; 11 adverse reviews,
16 reached/early-exit traces, full29 comparison changed only Attacker/Minion,
root review12 and prior Mother/Witch20 passed. Scratch Release DLL built;
new sidecar patch and DLL are packaged offline.
Batch53 SummoningTheShip Briar Rose summoner comments promoted: native-gated
`SummonersAlive` range/key/helper-result recovery; 29-unit fixed-seed
comparison changed only STS_BriarRose, ten Lua behavior cases passed,
root review3, equal eight-file smoke, and exact root regeneration. No sidecar
change beyond the prior FadeInThing candidate DLL.
Batch54 STS_BriarRose wave `else` branch promoted: native CFG gate restores
dead-target reacquisition after wave 1. Full29 comparison changed only Briar
Rose; before/after transition fixtures prove held-wave and changed-wave
behavior, root review3, equal eight-file smoke, exact root regeneration.
Batch55 TraderEscort EndTrader cleanup continuation promoted: syntax-checked
hoisted-region goto restores movie/vector/resource cleanup on a nested
termination path and enables the existing blocking-Speak cancellation exit.
Full29 comparison changed only EndTrader; native tail and behavior fixtures,
root cleanup/native11, blocking-speech18, equal 16-file smoke, exact root
regeneration passed.

Batch56 DragonBossFight Dragon.Main phase `else if` recovery promoted:
retail CFG/source/body-gated split restores phase 1 and phase 2 health
transitions. Full29 fixed-seed comparison changed only Dragon.lua;
seven before/after phase cases, root review3, equal five-file smoke, and
exact six-file root regeneration in both styles passed.

Batch57 DragonBossFight Dragon.Main TargetNumSummoners store promoted:
retail three-source shared write replaces the lost parent alias TODO.
Full29 comparison changed only Dragon.lua; seven Lua behavior cases,
root review3, zero-problem five-file smoke with seven expected writes,
exact six-file regeneration in both styles passed.

Batch58 DragonBossFight SpawnSummoners positions/creature key promoted:
two native slot0x18 calls become indexed GetPos calls; the selected
CREATURE_SUMMONER_01/02 string reaches CreateCreature. Full29 comparison
changed only DragonBossFight.lua; five before/after behavior cases,
root review3, equal zero-problem smoke, exact six-file regeneration passed.

Batch59 BeardyBaldy local IsHeroWearingBeard calls promoted: two native
direct sites now call the already-lifted entity helper, restoring beard
specific phase-3 dialogue/finale. Full29 comparison changed only the man
entity; four full-Main behavior cases, root review3, equal zero-problem
smoke, exact four-file regeneration passed.

Batch60 GameflowAssistance renown messages promoted: native hero-level
array index selects the matching seven message keys rather than always
level seven. Full29 changed only GameflowAssistance.lua; ten level traces,
root review3, equal zero-problem smoke, exact two-file regeneration passed.

Batch61 GameflowAssistance renown temporary-string teardown promoted:
reviewed C++ string-array destruction no longer emits four no-op Lua loops
or three TODOs. Thirty before/after termination traces match; full29 changed
only GameflowAssistance.lua, root review3, equal smoke, exact two-file
regeneration passed.

Batch62 SingingStones EffectList cleanup promoted: four native
KillAllThingsInVector calls now remove live effects through the sidecar and
clear the list. Full29 changed only V_SingingStones.lua in both styles;
region-exit live/dead cases, native gates, equal zero-problem smoke, and exact
eight-file regeneration passed.

Batch63 SingingStones playlist comparisons promoted: retail compares four
YourPersonalPlayList entries with PlayList and RudePlayList. Both lost
comparisons now read their mapped state fields. The full Lua fixture reaches
normal, rude, and other mismatch outcomes; full29 changed only
V_SingingStones.lua, root review2, equal zero-problem smoke, and exact
eight-file regeneration passed.

Batch64 SingingStones personal tune name suffix promoted: the native four
case switch selects A/B/C/D and appends each chosen letter to the message
prefix. Twenty-two valid permutations and two out of range cases pass full
Lua behavior checks; full29 changed only V_SingingStones.lua, root review2,
equal zero-problem smoke, and exact eight-file regeneration passed. The
readable quest Lua now has zero unresolved diagnostics.

Batch65 SingingStone entity hit and playlist path promoted: retail hit
flags, prior-note comparisons, and the selected note store now use mapped
quest/entity state. Hero hit, special hit, excluded heal, no-hit, repeat,
and new-note fixtures pass; full29 changed only SingingStone.lua in both
styles, root review3, zero-problem smoke with the expected no-hit trace
change, and exact eight-file regeneration passed.

Batch66 SingingStone hit-created effect append promoted: native vector
insertion now pushes the created effect into EffectList, replacing C++
vector pointer/refcount TODOs. The entity-to-quest fixture confirms later
region cleanup removes the effect. Full29 changed only SingingStone.lua,
root review2, equal zero-problem smoke, exact eight-file regeneration passed.

Batch67 SingingStone named effects promoted with a new sidecar DLL: both
effect calls pass StonesEffect and z+1.0, using a generic optional script
name argument. Full29 changed only SingingStone.lua; root review2, equal
three-file smoke, exact eight-file regeneration, and x86 Release build pass.
The save-safe game launch reached the chained-host startup marker, but the
frontend driver did not reach Load Game, so there is no quest gameplay proof.
The game closed; all 561 saves and installed DLLs match prelaunch hashes.

Batch68 SingingStone temporary pointer release promoted: a native C++
refcount teardown after Main's frame loop no longer becomes an unreachable
Lua condition and nil receiver call. The return label remains. Six full
Main before/after traces match; full29 changed only SingingStone.lua,
root review2, equal zero-problem smoke, and exact eight-file
regeneration passed.

Batch69 SingingStones camera calls promoted with a new sidecar DLL:
retail slot 0x670 uses CAM_Stones with null offset/rotation, duration -1.0,
and flags 0/-1 at both mismatched-tune branches. The sidecar now accepts
positional nil through sol::object and forwards null native pointers.
Normal/rude/other tune traces, a compiled sol nil/table probe, x86 Release
build, full29 target-only comparison, root review2, equal zero-problem
smoke, and exact eight-file regeneration passed.

Batch70 WaspBoss.Init world-map offset promoted: retail slot 0x440
passes Q_WaspBoss and a 2D coordinate with x=10, y=10. The generated
call's missing y is restored. The full Init fixture changed only that
argument; full29 changed only WaspBoss.lua, root review2, equal ten-file
zero-problem smoke, and exact 11-file draft/readable regeneration passed.

Batch71 TourGuideFollower second FollowThing target promoted: both
native calls reuse the same TourGuideGuide script-thing slot, retained
through the active loop. The full Main fixture shows the second call now
uses the guide instead of nil. Full29 changed only the follower entity;
root review2, exact five-file draft/readable regeneration, and equal
four-file smoke passed. Smoke retains two identical preexisting findings
in other Tour Guide functions for separate review.

Batch72 TraderConflictGood awareness target promoted: retail slot
0x9F0 receives hero and the current inner-loop AllCreatures entry. The
previous nil second argument now uses that entry. Full watcher cases
cover both creature indices, distance short-circuit and awareness true;
full29 changed only TraderConflictGood.lua. Root review2, equal
15-file zero-problem smoke, and exact 16-file draft/readable regeneration
passed.

Latest named offline bundle:
work/codex_lua_trader_awareness_20261004/trader-awareness-candidate-20261004
36 Lua/41 manifest hashes, 07774e98 baseline plus 17cc7399 sidecar patch,
e777ce0e DLL. Audit371 syntax0, diagnostics1017, generic6106, gotos4605.
Installed v124 unchanged; no install or outer commit. The earlier
startup probe did not reach gameplay. User now uses their screen again;
ask before any visual game launch.
[Current journal and next gate](LUA_RECOVERY_2026-10-04.md).

Next: inspect remaining registered-script diagnostics for native-backed recovery.
Preserve other dirty lanes and installed game.
Lua paused at user request for bedtime, 2026-10-04. Trader Escort sound recovery is scratch only: work/codex_lua_trader_sound_20261004. Both entity full-function tests (draft/readable), adverse gates, and controlled 16-file smoke pass. Fixed-seed 29-unit comparison was interrupted on request (40 logs, 38 complete, 2 empty); no source/output promotion or package. No Lua comparison process remains. Resume only on user request: rerun/complete full29, then review and promote if clean. Ask before any visual game launch.


# Lua recovery - 2026-10-02 (in progress)

Resumed the Arena lane on dirty feat/novi-script-recovery (no commits, native lane
untouched). Detail: docs/journal/2026-10/LUA_RECOVERY_2026-10-02.md.

Promoted offline today (each: adverse tests, behavior harness, full 29-unit A/B with
only the target file changing, smoke, gated promote):
- ArenaCellExitGuard speech: hero-listener check generalized (latest GetHero
  assignment, any variable name); 10 Speak / 24 task / 2 Clear recovered.
- New native_wide_assign_receivers.py: restores AssignFromWide's dropped ECX
  destination; ExitGuard SetQuestAsFailed now (name, true, "", true) as native.
- New native_reviewed_arena_enemy.py: ArenaEnemy creature-group loop, yellow health
  bar, TotalCreatures_<group> decrement and DeathScore crowd points.
- Lowering: pause-guard dtor with dropped arg (Roth cleanup no longer re-pauses the
  world), typed dword colours (WhisperAlly bars, FireHeart). New
  native_reviewed_whisper_ally.py: creature totals + wrapper IsAlive.
- Roth: constructor + speech + actor rows (9 lines, cutscene binding); lifter accepts
  byte-split locals; staged vector copies; label pairing falls back to callOrder
  (fixed movie/resource dtor swaps in Madame, GTDI_Maze, ManInLove, Flick/Needle/Shadow).
- Flick/Needle/Shadow constructor + speech rows (23 lines); reaching skips alignment
  filler; lifter: else-arm register kinds, char byte flags (also fixes 9 files in 6
  other units, incl. played quests -- refs only).
- Lifter: string slots re-assigned via CCharString::operator=(slot,"LIT") are variables
  (GivePrizeFund prize lines; ChickenMaster, SickChild traders/Witch lines).
- Arena.lua: CrowdChecker no longer crashes (threshold + sound-handle arrays), Init
  clears ArenaSpawnNeeded, dead base-dtor stubs dropped.
- AnimateCrowd: native-gated audience handle copies; all six animations target the
  real audience. Main Whisper assignments reconciled from reviewed baseline drift.
- PlayWave multi-marker spawns: native radius 1.0 restored, returned handle retained;
  72 cases per style pass. Generic copy rule excludes unresolved constructors.
- PlayWave rewards: token-vector lookup, native rotation, complete xyz, RNG order
  and integer thresholds; 1520 cases/style pass. New sidecar DLL built.
- Readable operator grouping fixed; 16-unit smoke identical. GuildTraining style
  output initially deferred for baseline drift; reconciled below.
Broad suite 1826 passed + 564 subtests; 3 member-alias tests passed. Corpus 371 parse; 81 needs-recovery / 135
needs-readability / 155 human-review; diagnostics 1114 (not completion percentages).

Installed game bundle remains v124. Named offline Arena candidate assembled:
work/codex_lua_arena_melee_20261002/arena-offline-candidate-20261002;
43 Lua parse /48 manifest hashes, exact newly built sidecar. Not installed or
game tested; game not launched.

Main master read restored: 26 focused +82 related tests pass, all29 draft A/B
changes only Arena, smoke identical. Full Main selects Lady Grey dialogue from
WhisperKilledByHero. Unused getter diagnostic retained.
CrowdChecker cycle and FishermansWife delayed-speech return restored; 1541 tests
+564 subtests plus 72 Wife cases pass. All29 A/B changes only these two files;
both styles smoke identical. Ten files promoted offline.
WhisperAlly melee binding complete: 71 compiled Lua/ABI cases +136 focused tests;
all29 changes only WhisperAlly. Four-way full Main traces equal.
Shared-tail inner else repaired: 1826+564 broad, 224+8 CFG/Wife, 128 actual
predicate cases, 26 compatible Bordello adapter cases, 98 Teacher tests. All29
changes six units; 25 files promoted in five, GuildTraining deferred for three
draft and twenty readable baseline drift files. Both styles smoke identical.
Arena backward spawn wrap + null-marker string promoted:1901 focused /103 related
tests;912 cases/style pass, all29 only Arena changes. Smoke adds the native
wrap-path termination check only. Five files promoted; named arena-spawn-candidate
packaged offline with existing melee sidecar,43 Lua/48 manifest hashes.
Arena target handles promoted:512 distinct-handle regressions,16 boundaries,
1826 passed, 564 subtests passed; all29 only Arena changes, both styles smoke equal;6080 reward
cases unchanged. Five refs promoted, named arena-target candidate43 Lua/48hashes.
GuildTraining generated baselines reconciled:28 refs, hand-reviewed tree unchanged.
1289+144 Guild tests,693 native speech cases pass; both smoke styles equal.
Whole Skill864/Melee576/three predicate Main288/KeepTabs288 cases per variant;
PreMelee predicate32/style repaired; native x87 NaN branch8 cases verified.
Named guild-reconciled candidate58 Lua/63 manifest hashes, offline only.
CheckFriendlyAttacks:6144 cases/style pass; FLOW jump preserves true predicate.
No change warranted. MeleeApprentice4224 cases/style pass; resource/movie lifetimes checked.
ApprenticeSpeedTest shown-once byte PROMOTED:5 Lua/reports +generic lowering
and two regressions.154 focused/root +1826 broad/564 subtests pass; all29
only SpeedTest changes; both smoke styles identical. New offline Guild candidate
58 Lua/63 hashes; installed v124 unchanged. See speed-byte owned work/journal.
Race timers PROMOTED: initial/retry use third ID;5 Lua/reports +generic
module/converter, two regressions and one mock fixture update. Root355 pass;
broad1958 +564 subtests. All29 only SpeedTest; both smoke styles equal.
New offline Guild timer candidate58 Lua/63 hashes; installed v124 unchanged.
SkillApprentice1536 cases/style pass,24 native counter windows confirm IDs.
No change warranted. WillApprentice abort-byte copy PROMOTED offline:
270 focused/root,2169 broad+564 subtests pass; all29 only WillApprentice changes.
Both smoke styles equal; full readable1536 cases match file-only traces.
Named Guild will-abort candidate58 Lua/63 hashes; installed v124 unchanged.
Blocking speech cancellation PROMOTED offline:184 refs,65 files/18 units,
converter +generic module +two regressions.466 focused/root;2439 broad+564
subtests pass. Both smoke forms equal across18; all29 compared, no baseline
drift. Whole Will1536/style now identical at native cancellation frame.
Named speech-cancellation candidate185 Lua/190 hashes, same0777 DLL.
Combat score float narrowing PROMOTED offline: 24 refs, nine files/six
units; lifter +generic module +two tests.36 focused/root;2941 broad+564
subtests.100 original native float-store cases; both full Lua forms32 grade
boundaries correct,6912 combat lifecycle traces unchanged. All29 A/B; both
styles smoke identical. Unchanged Arena main alias drift and hand-reviewed
Guild readable preserved. Named float-score-candidate-20261002:
130 Lua/135 hashes, same0777 sidecar source/af5d DLL. Offline only.
Guild Woods Will raw animation byte PROMOTED offline: 24 refs,
seven entity files/five units; converter/lifter +evidence module +two tests.
94 focused/root;3035 broad+564 subtests;24 original native caller/shim and21
exact-source x86 ABI cases.12288 whole WoodsWill lifecycle traces unchanged
at raw1;80 raw-byte cases correct. All29 A/B; zero baseline drift. Twenty
native sites->22 Lua calls includes two proven existing WB branch copies.
Both smoke forms unchanged except intended Wasp animation API trace name.
Named raw-animation-byte-candidate-20261002:114 Lua/119 hashes,
same0777 source/af5d DLL. Corpus371 parse/1114 diagnostics. Offline only.
WoodsDeparture delayed-load draft fix PROMOTED offline: 3 refs,
registered evidence +actual test, existing temporary-cleanup mechanism.
576 focused/root;3611 broad+564 subtests.4096 Maze lifecycle/324 native
predicate/10 native cleanup guard cases;576 whole quest/helper and16 early
cancellation cases. All29 A/B: only three dead draft cleanup lines removed;
readable unchanged, both smoke forms identical, zero baseline drift.
Named woods-departure-cleanup-candidate-20261002:58 Lua/63 hashes, same0777
source/af5d DLL. Corpus371 parse/1114 diagnostics. Offline only.
ArtifactThief movie cleanup PROMOTED offline: 4 refs, one entity
draft/readable; registered evidence +actual lifetime test.192 focused;
1534 broader+549 subtests, unchanged generator/DLL; promoted768 artifact+
departure root tests.4096 full lifecycle traces repair672 leaks per form,
all other events unchanged. Six original native branch cleanup paths proven.
All29 A/B; both smoke forms identical; zero drift. Named artifact-movie-
cleanup-candidate-20261002:58 Lua/63 hashes, same0777 source/af5d DLL.
Corpus371 parse/1114 diagnostics. Offline only.
Earlier guarded animation pairing PROMOTED offline:29 refs,15 native calls,
eight functions/six units. Existing adapter before movie cleanup plus fallback;
216 focused/3925 broader+564 subtests/984 promoted root tests. All29 A/B,
zero drift, both smoke forms identical,4096 thief lifecycle+120 wave traces.
Named early-animation-pairing-candidate-20261002:112 Lua/117 hashes,
same0777 source/af5d DLL. Corpus371 parse/1114 diagnostics;generic temps6290.
SickChild dynamic key remains unresolved nil with original diagnostic.
Installed v124 unchanged. LUA PAUSED FOR TONIGHT at explicit user request.
SickChild sister key scratch work/codex_lua_sick_sister_key_20261002:
baseline72 fail/36 pass ->candidate108 pass;final118 incl10 guards;
96 original native selection/conversion/animation cases pass. Existing0777
sidecar binding suffices. Root converter/evidence unchanged;no promotion.
All29 interrupted intentionally;partial logs preserved,owned processes stopped.
Resume only on user request:complete all29/full readable/smoke/broader checks,
then guarded promotion/audit/package. Exact checkpoint in bedtime_checkpoint.json
and today's Lua journal. Preserve accepted earlier animation pairing promotion.
Owned speech-cancel work; two earlier lifter prototypes discarded.  Owned reward sidecar build completed
(work/codex_lua_arena_rewards_20261002/sidecar_build.json); 22 x86 mocked binding
cases pass. GuildTraining generated refs are reconciled; its hand-reviewed
readable tree remains preserved.
WhisperAlly readable has zero diagnostics. Arena runtime proof remains pending.

---

# Previous Lua handoff (2026-10-01)

Arena InitialiseArenaRounds pending smoke label is hardcoded mock metadata;
v124 owned sidecar source already includes/registers NoviArenaRounds. Actual
Arena runtime proof remains outstanding; do not infer a missing deployed binding.

The prior native shutdown handoff is preserved verbatim below.

---

# Native recreation - active, 2026-10-02

Native lane resumed 2026-10-02 (Lua lane untouched; preserve the dirty `feat/novi-script-recovery` tree, no commits). Detail: [native journal 2026-10-02](NATIVE_RECONSTRUCTION_2026-10-02.md).

Promoted today (dedicated gates in gate_all; native Unicorn oracle, compiled fixture, omission checks, catalog PASS, header audit, candidate build):
- movable-pool policy (8), pool Free chain (3, Free NEAR), CMemoryFile (18, Write NEAR), CDefClassBase own members (12 + 25-slot vtable), input core (25 exact + 3 device vtables; SetHasInputFocus now MATCH), IME settings data (19 globals), CMainGameComponent event-package/resolution de-bakes (4) + static data, CParentDefClassBase sub-def table (all members; RemoveSubDef via the FableWin-evidenced CVectorMap::EraseFromKey) and its map sort/heap/bound family (14 units + 4 COMDAT-covered), the index map's heap helpers (5), CMemoryDataOutputStream SetPosition/GetPosition/Close/destructor (flat stand-ins retired). Flat-stand-in lane: CBankFile notification defaults (6), CACompressor defaults (4), vtable-defaults batch 1 (55; `tools/decomp_pipeline/vtable_defaults.py`, `gate_vtable_defaults.py --batch N`, land with `work/codex-native-startup/land_vtable_defaults.py N`; worklist in the journal); 4 stale asm_bake grades fixed. CDataInputStream readers: ReadNullTerminatedWString, ReadCompressed (LZO), ReadZlibCompressed (de-baked), ReadLine/ReadUntilCharacter/ReadUntilWhiteSpace/ReadWhiteSpace. DataInputStream unit now also holds OpenMemorySource and the CMemoryDataInputStream (data,size)/(CCharString) ctors (full catalog leaves). Worklist: 0x00BF3B60 landing invents a CMemoryDataInputStream (retype it; refresh seeds exclude it). CCompressorZlib Compress/Decompress (block header) landed.
- Repository-wide class-key sweep: `tools/decomp_pipeline/class_keys.py` -> `ghidra_out/class_keys_fablewin.tsv`; generator emits evidenced `class`; 1,264 headers re-keyed.

Closure: 867 sources / 0 duplicates / 579 unresolved (`work/codex-native-startup/native-gfmain-memory-input-closure-20261002`; next refresh seeds from its `closure_objs.rsp`). Full `gate_all.py` after the sweep: sweep fallout fixed (GFMain symbol map, display create, font pointer sharing, OpenIniFile + Initialise boot leaves). Still failing, pre-existing: boot leaves GetWindowTitle 004023F0 and NGameText::CDataBank ctor 009C85A0 (18 bank virtuals unresolved in their fixtures, also with pre-session headers); `header deps` stops on the Defender block (WinError 225) - not retried.

Native recreation paused for the night at user request (2026-10-02). Resume with staged `thing-definition-manager`: typed CThing PeekDefinitionManager004C7990 /SetCurrentThingManager004C74F0 match9/7 relocated bytes; behavior gate, header audit and landing remain. Existing false display/global identities are still canonical. Do not rerun its one-shot probe. Details and next combat-definition lookup evidence are in the native journal pause section. No native jobs remain running.

Latest native continuation: timer data, display leaves/feedback, the real 87-row EventDispatch table, world control/player/GUI actions, six intelligent-pointer ownership/lifetime leaves, five player lookups genuine UnfreezeControls and aggressive toggle/global bool promoted. Dedicated native/compiled gates, catalog checks and selected builds pass; all 123/123 bootstrap leaves pass after player layout changes. Two lookup asm bakes retired; ledger 512. Latest closure: 964 sources / 0 duplicates / 635 unresolved (`work/codex-native-startup/native-gfmain-knockdown-ability-closure-20261002`; refresh with `refresh_knockdown_ability_closure.py`). The table initializer matches 9,567 bytes / 1,477 relocations and explicitly exposes the world action graph. Earlier aggregate header-dependency security block was not retried.

Controlled-creature Get/Peek are promoted (two 8-byte relocation matches, 9,216 native/compiled scenarios, header audit 61/0). CPlayer +2C is a genuine generic intelligent pointer; CThing now models primary CClass/CBaseObject ancestry with retail 4-byte list iterators and CClass size 11. Kill-mode world event 00629570 promoted (34 bytes, 8,192 native/compiled scenarios, header audit 29/0). SetAggressiveMode 00487EB0 parity probes remain scratch/unaccepted. EACreatureCheat 006292A0 is promoted (37 bytes, 6,144 native/compiled cases, header audit 49/0). CThing::Kill(true) and SetUpControlsBasedOnAggressiveMode remain boundaries. Camera-base slots 2-10 promoted: nine matches, 2,304 native/compiled cases, actual vtable slot check and header audit 1/0. Declare const GetCameraManager before mutable because VC7.1 reverses adjacent overload slots. Camera-centre promoted: world event + two manager query overrides match, 1,024 native/compiled cases, actual slots 9/10. RecentreCamera 006978C0 promoted (16-byte match, 1,536 native/compiled cases, header audit 3/0); mode CurrentMode.Data +98 -> Recentre virtual slot 16, preserving Info/refcount. Mode implementations remain dispatch boundaries. CCamera::GetAngleXY 00A0C380 promoted (87-byte relocation match), genuine RightHandedSet and turns calculation; 4,332 native / 722 compiled cases, header audit 14/0. Oracle executes original game MSVCR71.dll _CIfmod, including legacy signed-zero behavior; finite hardware tolerance 2 ULP, zero sign strict/NaN classification. GetAngleYZ 00A0C3E0 promoted (110-byte relocation match), 41,154 native / 6,859 compiled cases, header audit 15/0. GetAngleXZ 00A0C450 promoted (288-byte relocation match), genuine vector-angle/spherical/GF helper bodies, 2,604 native / 434 compiled cases including 42 errno=33 domain errors, three-header audit 133/0. Oracle executes original DLL _CIacos with evidence-backed TLS initialization and Windows API boundaries. MoveForward 00A0C250 promoted (86-byte match), genuine height lock and shared vector operator+=; 5,868 native / 978 compiled cases, two-header audit 134/0. Closure 916 / zero duplicates / 639 unresolved. MoveSideways cross-operator compiler probes remain scratch/unaccepted; EAPlayerRespawn 006294A0 promoted (110-byte relocation match /GS), byte==1 autoload else reset: 8,192 native / 32,768 compiled cases through genuine package/reader, three omissions, header audit 51/0. AutoLoadRespawn / ResetToFrontEnd remain boundaries. EACloseLiveGUI promoted near match plus genuine hero-info getter/Close matches, retiring false scalar identities 00437CD0 / 0066E73D. 8,192 native/compiled cases through real world lookup/pause and typed scoped_ptr; five omissions, header audit 33/0. LEASetExclusiveMode 0085CA80 promoted (103-byte relocation match /GS), byte!=0 and typed world Component reference +08; 8,192 native / 32,768 compiled, three omissions, header audit 53/0. Closure adds existing component mode switch automatically. LEASetDisplayMode 0085CAF0 promoted (132-byte relocation match /GS), 2/2/1 reads and unsigned-short dimensions/byte depth; 7,680 native / 71,680 compiled cases, three omissions, game-header audit 108/0. Component SetDisplayMode 0041641F promoted (125-byte relocation match /Os), 10,304 native/compiled direct + event/package/reader composition and manager-rebinding cases, four omissions, no header changes. Inventory events Map/Status/DeleteMenu 0062C8E0/0062C930/0062C980 and signed component-map LowerBound 0040F020 promoted: 300 bytes, 15,560 native / 30,920 compiled, six provider omissions, 59 header dependents without regressions after merging CTCBase startup static. Corrected initial mislabeled PhotoJournal at0062C930: authoritative dispatch and donor calls show StatusMenu; real PhotoJournal is0062C7A0. Five menu handlers Scoreboard/Skills/Inventory/Options/PhotoJournal promoted (400 bytes), 25,600 native / 51,400 compiled, eight omissions, 59 header dependents without regressions. Subsequent donor-symbol trace corrected GetTC<T> to T::StatGetInterfaceType and CTCInventory static ownership: both gates repeated, 64 dependents without regressions, no emitted byte changes. Four trade events Buy/Sell/Wanted/DeleteTradeActive promoted, 320 bytes, 20,480 native / 41,160 compiled, seven omissions, 64 header dependents without regressions. Trade ID23 and inventory ID17 independent; partial trade primary ancestry only. MSN event0062C6E0 was already accepted; added 20,480 native / 40,960 compiled null/unmapped-result checks, three omissions, source/catalog unchanged. MoveSideways00A0C1B0 promoted NEAR_MATCH152 bytes, 16,584 native / 2,764 compiled, six x87 modes, whole-camera guards, one omission and 18 header dependents without regressions; original Forward cross Up component expression in shared GetRight, no cross operator promoted. SetFOV00A0BED0 promoted48-byte relocation match, 8,664 native / 1,444 compiled, one omission and19 header dependents without regressions: raw angle bits copied, flag3 only for ordered positive vertical. SetView00A0BE30 and AccurateNormalise00A14440 promoted129 relocated bytes, 88,560 native / 14,760 compiled, two omissions, 139 header dependents without regressions. Preserved alias copy/normalization order and unchecked zeros; corrected GetScaled label. Catalog fixtures separated because /Od normalization dependency rounds extreme float inputs differently; composed /O2 gate still uses both genuine providers. PointAt00A0C2B0 promoted208-byte relocation match, 12,192 native / 4,064 compiled, two omissions, 141 header dependents without regressions; four normalization calls, target aliases and vertical lock verified. Const GetTC00436990 / LowerBound004365B0 promoted45/60-byte relocation/exact matches; shared const Find forwards original key reference, 600 native/compiled, two omissions, 395 dependents without regressions. Catalog and selected build PASS. Const map Find00436860 promoted34-byte relocation match, 200 native/compiled, two genuine providers and two omissions; no header changes, catalog/selected build PASS. Free-camera events00629390/00629420 promoted254 relocated bytes /GS, 32,768 native / 2,097,152 compiled, five omissions, 44 header dependents without regressions; exact byte==1 parsing, original CONTROL_CAMERA=19 and flag+220. Current-mode getter00633BE0/default event006298F0 promoted13/64-byte exact/relocation matches, 3,528 native / 221,328 compiled, four omissions, 470 header dependents without regressions. Getter replaces old padded fastcall stand-in/self-contained fixture; real const list back(), original CONTROL_CREATURE=1, forced AddMode only for alive creatures in another mode. HasMode00634160 typed replacement promoted39-byte match, 3,456 native/compiled, one omission, 409 header dependents without regressions; original generic find and actual provider test replace padded local class/self-contained STL test. Photo-capture close0062B4D0 promoted122-byte relocation match, 20,480 native / 81,920 compiled, seven omissions, 85 header dependents without regressions; original unpause/remove45/close/creature/component270/destroy order verified. Console toggle0062A110 promoted103-byte relocation match,16,384 native/compiled,two omissions,243 header dependents without regressions; cached main player, lazy singleton and CONSOLE44 verified with explicit allocator/ctor/reset boundaries. Full component guards and catalog/selected build PASS. In-game menu close00629D60 promoted near57-byte retail body,8,192 native/compiled,five omissions,51 header dependents without regressions. BlockEnd0062B000 near110-byte retail body and typed bool const IsMainPlayer00487520 relocated19 bytes promoted,2,496 native /123,456 compiled,six omissions,88 header dependents without regressions. Both catalog/selected builds PASS. EARemoveCurrentMode0062C4B0 promoted189-byte relocation match /GS,25,920 native /1,658,880 compiled,six omissions,171 header dependents without regressions. Cached creature pointer and late dead-bit checks survive RemoveMode side effects; catalog/selected build PASS. Hero-information close0062B3F0 promoted near214-byte retail body,3,840 native /245,760 compiled,eight omissions,93 header dependents without regressions. Fresh GUI/event/player lookups across RemoveAllPlayerModes side effects verified; global unpause policy and original VIEW_HERO47. Catalog/selected build PASS. Combat UnsetBlocking007338A0 promoted102 relocated bytes,7,680 native /1,966,080 compiled,two omissions,80 header dependents without regressions; component49 required as retail failed GetTC path uses uninitialized local. IsBlocking007331E0 typed const bool replacement promoted4-byte match,1,024 native /262,144 compiled,one omission,81 header dependents without regressions; retired false inventory identity/padded source/print-only test. Catalog/selected builds PASS. Creature-mode RemoveMode0073ABA0 promoted170 relocated bytes,7,680 native /134,400 compiled,three omissions,482 header dependents without regressions; twelve-slot nested mode ABI, original ConstantFPS*30.0f idle deadline and retail __ftol2 oracle. remove_if remains explicit unlink-only boundary without allocation/refcount coverage. Catalog/selected build PASS. Vulnerability007331C0 typed bool const replacement promoted30 relocated bytes,7,392 native /14,784 compiled,one omission,83 header dependents without regressions; original three-second ConstantFPS threshold, six x87 modes and independent integer rounding oracle. Catalog/selected build PASS. Mode-list remove_if0073E790 promoted104 relocated bytes;2,304 native/compiled lifetime and7,680 native /134,400 compiled composed cases,one/four omission links,484 header dependents without regressions. Original generic erase/refcount cleanup/free and cached-next iteration verified with actual helper composition. Catalog/selected build PASS. Knockdown query007331A0/getter00666D10 promoted20/47 relocated bytes,1,536 native /65,536 compiled composed cases,two omissions,246 header dependents without regressions. Actual temporary-definition release precedes flag normalization; callbacks can change flag/parent. Catalog/selected build PASS. Next GetPCombatTypeDef00664D40 donor/native evidence saved: component48 definition+28 or manager lookup by combat type/default name. Field/literal/helper recovery remains, not staged. EventIsWorldEvent0049D8C0 remains scratch52v23 from member-pointer null comparison. The table evidence is now landed in ghidra_out/game_event_dispatch_fablewin.tsv. Remaining named world-handler bodies, HandleMoveHeroToRegionGameEvent 0049EAF0 and UpdateFrame 004A5A40 follow. Deferred: CDefClassInfo map partition/partial_sort/heap helpers (non-POD copy shapes, 0.35-0.96), bank Abort 009D6350 (0.72, do not repeat probes), CThreadedFileManager::RemoveObject 0098EB20 (needs STLport map<ulong,CFileObject*> find/erase modelling). Do not rerun one-shot `land_*.py`; never recursively scan `work/codex-native-startup`; security exclusions 009A5490 / 00A62020 stand.


Lua 2026-10-05 helper ownership checkpoint: MakeTraderComment master/shared scoped copies/assignments/retirement and direct concrete equality promoted through generic shared-helper evidence hook. Full29/534 parse only four intended Lua changes;128 regression tests,5312 actual SDK returning cases,14648 event-ref snapshots,160 isolated native fault observations pass. Smoke findings unchanged;37-Lua/42-hash offline bundle `work/codex_lua_trader_helper_integration_20261005/trader-helper-ownership-candidate-20261005`, sidecar161c39c/DLL967e7dc3. Production host and natural sound/completion remain unproven. Authorized visual sessionPID14652 active, staged backup `scratchpad/save_backup_1234234_20261005-175300`; exact state in integration workdir/repaired_candidate_visual_session.json. Restore staged save/profile on closure. Current details: docs/journal/2026-10/LUA_RECOVERY_2026-10-05.md, section MakeTraderComment ownership and direct equality promoted.


Latest user steering: skip visual testing for now; user wants their screen. Ask before any future game launch or screen/input use. Continue Lua OFFLINE under the original ongoing request. Prior launch authorization is superseded. PID14652 had already exited before cleanup (normal entity/quest teardown is present in log); no relaunch. close_runtime.py completed staged save/Profile.bin restoration and verified all installed DLL hashes unchanged. Archive focused_runtime/validation.json and current_session.log. Four controlled master helper calls returnedtrue: type1 banter, nil-speaker fallback, type2 nonempty equality, and type2 swap. Natural shared helper FOLLOWING and banter/response queues also observed. ForcedGC then reports15hp,quest counter2 and allthree actors alive,quest incomplete; no Lua/autopilot errors in current session. This establishes selected live host paths only, not native refcount balance, audible playback, combat or fullquest equivalence. Initial close-script identity query failed because process had already exited; corrected cleanup handles terminal process and restored successfully. Offline bundle remains valid.


## Offline void return-register store recovery in validation

User reaffirmed continue Lua until asked to stop; screen/game launches remain paused and require asking first. Owned work/codex_lua_void_register_store_20261005 baseline matches prior helper integration1832 inputs. Added generic reviewedVoidRegisterStores pass after scalar operand recovery: independently verifies source/native/PDB void prototype, exact intermediate/output hashes, local integer EAX residue, native producer call and pop-only terminal epilogue (EAX pop/read/branch rejected). Trader SetBrainState E0A80D destructor falls directly through pops to RET4; recovered sole iVar4=extraout_EAX_00 store, no native cleanup/effects removed. Focused26 plus related77 tests terminal0. Both emitted styles504 before/after comparisons pass with poisoned old register and new undefined-register read trap; original native helper504 cases match mutating effects/state/termination eachstyle. Comparison initially missed raw stack float3.0 normalization, rejected scratch retained; corrected explicit IEEE bits normalize3.0 and1.0 only. Smoke draft2->1/readable1->1, sole removed finding is DarkwoodTrader freeglobal; Assassin mock failure unchanged. Full29 session71824 is still running; outputs not promoted.

Independent Assassin diagnosis:96 synthetic vector-shape smoke scenarios, all72 nonempty cases pass; six empty cases exit before selection and18 fail modulozero as expected. Original retail selection block E02EC8..E02F03 independently executes24 Unicorn cases with modeled rand:18 nonempty selected pointers exact,6 empty vectors trap x86 divideerror atE02EED. No invented empty-vector fallback; no full Main/host/cleanup equivalence claim. Reports assassin_scenario_smoke.json and assassin_native_vector.json. No game launch, input, installed files or save modifications.


Void-register recovery complete: full29 worker71824 terminal0,534 Lua parse and only one DarkwoodTrader draft line removed; all other Lua byte-identical. Exact generated draft plus two reports promoted.103 tests,504 native cases and504 emitted comparisons/style pass; current draft/readable smoke each retain only known Assassin fixture modulozero. New offline trader-void-register-candidate-20261005 package37 Lua/42 manifest hashes, same sidecar161c39c/DLL967e7dc3; no install or launch. Readability inventory unchanged because every readable Lua byte is unchanged. Reports in work/codex_lua_void_register_store_20261005.

Next offline issue found by actual SDK termination-boundary probe: shared draft1152 passes, but shared readable drops termination queries at early dead-speaker/timer-positive exits. Initial unhandled C++ assertion fastfailed; diagnostic catch identifies missing snapshot count (kind0/scared/timer2). An initial builder-only fix in scratch still fails because readable_style.normalize_alive and simplify_conditions also delete unused scheduler queries. Both rejected outputs retained. Now allthree rewrites preserve bare query calls while discarding only unused values;50 tests and21 subtests pass, full readable29 worker11040 running with independently hash-verified unchanged completed draft baseline. Root generated readable output not promoted yet; latest candidate must pass actual SDK snapshots1886/form. No game or screen use.


Termination-query fix actual SDK gate complete: probe_sdk_retained_termination_refs.py worker94855 terminal0, four actual generated master/shared draft/readable forms1152cases each and1886 termination-boundary ref snapshots each, total4608cases/7544snapshots. Results/events and pre/post-GC native wrapper/identity reference counts agree with original oracle. Original assignment/equality/getter execute; copy/destructor/engine/termination responses remain modeled, no production host proof. Reports sdk_retained_termination_four_forms_validation.json in helper_assignment and copied sdk_termination_validation.json in termination_queries. Both failed predecessors retained; final root readable builder/style fix has not yet been promoted while full29 worker11040 runs.


## Termination queries retained through readable generation

Completed work/codex_lua_termination_queries_20261005/completion.json. Generic build_readable_unit.drop_unused_alive and readable_style.normalize_alive/simplify_conditions preserve scheduler calls when their result is unused. No special quest pattern. Full29 readable worker11040 terminal0 with hash-verified unchanged draft generation/evidence reused;534 Lua parse.28 changed readable files/13 existing units retain49 query sites, all draft and remaining Lua byte-identical. Reviewed all diffs; structural differences retain early-exit cleanup instead of routing exits through a now-observable final query.50 tests+21 subtests pass. Actual four generated SDK/sol helper forms4608cases/7544 termination reference snapshots match original native oracle with modeled engine/copy/destructor boundaries. No production host/lifetime claim.

All13 affected-unit before/after mock findings, complete nontermination call traces and frame counts agree. Initial scratch collector removed Lua40-call limit but missed Python12-call truncation; WillWhisper false mismatch exposed it. Rejected truncated reports preserved, collector corrected at both layers, source/driver-hashed rerun terminal0.20 focused WillWhisper normal/termination/animation scenarios also preserve exactly one acquired-resource cleanup (zero before acquisition). Promotion first refused GuildTraining hand-reviewed/readable path before any output writes; switched to existing readable_dir resolver and preserved that authored tree, promoting generated readable_converter instead.28 Lua plus13 reports promoted with exact hashes. Readability ledger refreshed371/syntax0; counts6213generic/4611gotos/6357native/4649machine/970unresolved. New focused offline trader-termination-queries-candidate-20261005 package37Lua/42manifest hashes passes, sidecar161c39c/DLL967e7dc3 unchanged. No game, screen input, installation or save changes. Continue offline per user; ask before future launch.


## Helper lookup and Win32 fiber ownership probes complete

Owned work/codex_lua_trader_helper_fibers_20261005/completion.json records terminal probes. Actual SDK/sol allfour generated helper forms4608cases and4768 reference snapshots at nearest/furthest/script-name output lookups match original native oracle (helper_assignment/probe_sdk_lookup_boundary_refs.py worker68153 terminal0). This excludes intentionally hoisted pure hero/state getters and is not a complete every-instruction boundary claim.

Actual C++ WithRetailResources protected callback now also tested on real standalone Win32 fibers, not a substituted Lua scope callback. Fourforms4608cases with432 suspensions atPause; forced LuaGC while C++/Lua stacks are parked retains exact actor/identity counts, resumption matches native Pause/query snapshots/events and final/pre-postGC balances (first56393 and remaining86835 terminal0). A separate432 controlled C++ errors after resumedPause exercise actual protected callback error closure in allfour forms (worker89804 terminal0): exact oracle event prefix throughPause, no subsequent events, native refs balanced before/afterGC. Model scheduler/API/copy/destructor boundaries remain explicit; these probes do not instantiate production quest/entity hosts, region teardown or delete suspended fibers.

Documented stable generic ownership/assignment/direct-equality contracts in docs/scripts/FSE_UPSTREAM_REQUIREMENTS.md. Exact committed cumulative patch exported to tools/script_recovery/sidecar_patches/novi-scoped-thing-outputs-direct-equality.patch, SHA b885fe7f8cd9b36d08d74ddcf1c5fe276fd16ab907d6d56893032909e392dc70, base07774e98 ->161c39c4. Source clean, exported bytes match prior tested patch, reverse git apply check succeeds. Includes prerequisite reviewed camera/position/create changes, not arbitrary-upstream standalone patch. No source or DLL rebuild was needed for scratch probes; generated Lua remains the validated termination-query candidate. No game launch/screen use/install/save changes. User still requests continued offline Lua work until stopped.


## Literal return jumps recovered inside resource callbacks

Completed work/codex_lua_literal_return_jumps_20261005/completion.json. Generic fold_nested_return_jumps now handles bare/nil/true/false returns, keeps non-tail Lua returns in do blocks, rejects variables/calls/strings/multiple values and duplicate labels/named closure bodies. Actual token inspection also prevents string literals blanked by structural analysis from being mistaken for bare returns.73 tests+88 subtests pass; six full-readable-pipeline string/cleanup cases reproduce three prior early-return value-loss cases across a loop and preserve value/arity after correction. Initial straight guard fixture was optimized before the faulty pass and did not reproduce it; retained rejected fixture, switched to cross-loop jump.

Full29 readable worker42422 terminal0 with unchanged draft/evidence hash reuse,534 parse; only master/shared TraderEscort readable helpers change, each16->11 gotos. Scope-independent false returns expose safe local sinking; native calls/cleanup remain. Actual fourforms SDK4608 result/event/termination comparisons and7544 refs pass (worker95612). Current readable forms also pass216 normal Win32 fiber suspensions and216 controlled resumedPause errors, exact Pause/lookup refs and parked/finalGC balances (worker50150); engine/scheduler/copy/destructor doubles, no production-host teardown claim. Full Trader smoke findings, nontermination traces and frames unchanged. Two Lua plus report promoted by exact hashes. Offline trader-literal-return-candidate-20261005 package37 Lua/42 manifest hashes, same sidecar161c39c/DLL967e7dc3. Ledger371/syntax0,6213generic/4601gotos/6341native/4649machine/970unresolved.

Initial baseline snapshot copy overlapped the first edit after hash verification. Guard rejected the copied hash; recovered original function bytes from the prior owned snapshot and required the exact completed-corpus source hash before proceeding. Rejected copy and baseline_recovery.json retained. No acceptance uses the raced copy. No game, screen/input, install or save changes. Continue offline Lua work until user stops; future launch requires asking.


Next offline readability pass in work/codex_lua_dead_bitmask_staging_20261005: generic prune_dead_bitmask_staging removes only a uniquely declared unused local initialized from uint32 literal followed by closed AND-mask guards. No outside reference/capture/global store, unsafe expression, label or diagnostic is removed.81 tests+21 subtests pass. Completed literal-return baseline inputs and exact source snapshot verified before editing. Full readable29 worker74870 running, draft generator/evidence unchanged; native SDK/fiber checks and promotion remain pending. Latest packaged/promoted artifact remains trader-literal-return-candidate-20261005. No game/screen/install/save actions.


Dead-bitmask pass checkpoint: final88 tests+21 subtests terminal0; scope/string/_ENV adverse cases added. Full29 readable worker79282 running against stable final inputs. Previous completed74870 was rejected for scope gaps,75121/51283 explicitly stopped for later guards; rejected copies retained, no promotion yet. Fourforms SDK4608/7544 refs worker31238 and four readable Win32 fiber probes worker63609 completed; verify_helper_reuse.py confirms rebuilt final Trader helper hashes exactly match all probes. Full Trader smoke unchanged, including known Assassin empty-vector failure. Latest accepted package remains literal-return candidate. Record_complete.py is prepared but must run only after final corpus audit, smoke, hash-gated promotion, packaging and ledger audit. Next candidate scratch inventory/prototype:13 unreachable jumps after unconditional do-return blocks;8 return/cleanup/error and7 adverse cases pass; next work/codex_lua_unreachable_return_jumps_20261005/prepare.py requires this pass completion before taking its baseline. Continue offline, user screen reserved, ask before launching.


## Dead private integer-mask staging removed

Completed work/codex_lua_dead_bitmask_staging_20261005/completion.json. Generic prune_dead_bitmask_staging removes a uint32 local initialization and contiguous closed AND-mask guards only when the local has no outside use. Lexical block tokens prove the declaration still encloses the assignment; whole-chunk tokenization rejects mask-shaped string/comment text. The special _ENV local is excluded because it has implicit global-lookup uses. Captures, live values, globals, unsafe operands, diagnostics, labels and ambiguous declarations remain intact.88 tests+21 subtests pass (worker21151 terminal0).

Full29 readable worker79282 completed with534 parsed Lua and unchanged draft/evidence hash reuse. Only master/shared TraderEscort readable helper files change: each loses the unused scratchValue6 declaration and13-line integer-mask chain. No call, cleanup, query, label or goto changes. Full Trader smoke findings, nontermination call traces and frame counts remain identical, including the already traced empty Assassin vector modulo-zero failure.

Actual SDK fourforms4608 cases/7544 termination-reference snapshots pass (worker31238), and current readable helpers pass216 normal plus216 controlled resumed-Pause-error Win32 fiber suspension/parked-GC checks (worker63609). Generated-file hashes prove these completed probes used exactly the final regenerated helper bytes. Engine/scheduler/copy/destructor boundaries are modeled; the ownership-only C++ scope fixture is not the complete production resource class or quest/entity host teardown. Two Lua files and readable report promoted by exact hashes; trader-dead-bitmask-candidate-20261005 package37 Lua/42 manifest hashes, same sidecar161c39c/DLL967e7dc3.

Initial full corpus74870 rejected when late/closed local declarations exposed a global-store bug. Subsequent scratch rebuilds75121 and51283 stopped before completion for multiline-string and _ENV guards; outputs and reports retained in explicitly rejected directories. One sibling-branch test fixture initially inserted twice through broad string replacement; corrected to one insertion before acceptance. Reusing the original after directory was rejected by its existing-destination guard before writes; archived it within the verified task boundary and restarted fresh. No rejected/incomplete run supports acceptance.

Next concrete candidate: thirteen unreachable goto sites directly after unconditional do-return blocks, across BS_Teacher and the two Trader helper forms. Scratch next_jump_prototype.json passes eight return/cleanup/error cases and seven conservative exclusions; it is not integrated or promoted. Continue offline Lua work until user stops; ask before any future game launch. No game/screen/input/install/save changes.

Current ledger summary: {"counts": {"genericTemporaries": 6189, "gotos": 4601, "machineTemporaries": 4649, "nativeNames": 6341, "unresolvedDiagnostics": 970}, "files": 371, "statuses": {"needs-human-review": 156, "needs-readability-work": 156, "needs-recovery": 59}, "syntaxFailures": 0}.


Unreachable-return-jump active checkpoint: work/codex_lua_unreachable_return_jumps_20261005 baseline is completed dead-bitmask candidate.113 tests+21 subtests terminal0 worker14734. New prune_unreachable_return_jumps is now final style cleanup; initial iterative corpus20180 stopped/preserved because it enabled unrelated Teacher frame/query folds. Focused final Teacher preflight proves nine goto-line deletions only. Full29 final worker69605 running; no generator input edits while live. Final Trader generated hashes equal completed actual SDK4608/7544 refs worker59500 and readable Win32 fiber432 suspensions including216 controlled errors worker58069. Both current units' complete smoke traces including termination queries, frame counts and findings unchanged. Pending final corpus audit, verify_exact_diff.py (exact13 deletions across3 files), smoke, gated promotion, two-unit offline package, ledger and record_complete.py. Nothing from this pass promoted yet; latest accepted package is trader-dead-bitmask-candidate-20261005. Next generated-label scratch inventory/prototype has eight behavior/exclusion cases, not integrated. User wants continued offline Lua work; no screen/input/game/install/save actions; ask before launch.


## Unreachable jumps after literal returns removed

Completed work/codex_lua_unreachable_return_jumps_20261005/completion.json. Generic prune_unreachable_return_jumps removes only an immediate same-indent goto after a bare/nil/true/false do-return block. Whole-chunk tokens reject string/comment lookalikes; intervening labels, diagnostics and arbitrary expressions remain. Runs as the last style cleanup so dead-edge removal cannot trigger unrelated executable folds.113 tests+21 subtests pass (worker14734), including return arity, Lua close handlers and their errors, alternate label entry and the complete callback pipeline.

Initial placement inside the repeated optimization loop enabled Teacher frame/query and temporary rewrites. Worker20180 was stopped before completion; rejected_iterative_candidate retains its outputs and smoke. Moved the rule to final cleanup, and focused Teacher regeneration proved exactly nine jump deletions before restarting the full corpus. Worker69605 completed all29 readable units,534 Lua parse, verified unchanged drafts/evidence reuse. Exact diff gate proves only13 goto-line deletions:9 BS_Teacher,2 shared Trader helper,2 master Trader helper. No other source line changes. Full smoke findings and complete API traces (including termination queries) plus frame counts unchanged for both units; Teacher has no smoke findings, Trader retains its known empty Assassin-vector modulo-zero case.

Actual SDK fourforms4608 cases/7544 termination reference snapshots pass (worker59500); current readable forms pass216 normal and216 controlled resumed-Pause-error Win32 fiber suspension/parked-GC checks (worker58069). Helper hashes match final regeneration exactly, permitting reuse after the pass placement change. Ownership-only C++ scope fixture and modeled engine/scheduler/copy/destructor limits remain; not production host teardown. Three Lua files and two reports promoted by hashes. Offline two-unit trader-book-unreachable-return-candidate-20261005 bundle includes TraderEscort and BookCollecting with the existing Oakvale prerequisite, same sidecar161c39c/DLL967e7dc3; syntax and every manifest hash verified. No installation, game launch, screen/input or save actions.

Next concrete investigation: unreferenced generated LAB_/FLOW_ labels remaining after return recovery; next_cleanup_inventory.json is discovery only and has not changed generated files. Continue offline Lua work until stopped; ask before any future launch.

Package: {"bundle": "work/codex_lua_unreachable_return_jumps_20261005/trader-book-unreachable-return-candidate-20261005", "game_tested": false, "installed": false, "overrides": ["Q_NewOakValeIntro", "Q_TraderEscort", "V_BookCollecting"], "parsed_lua": 40, "sidecar_baseline_commit": "07774e98c5675fb66aba7c23b4f1e4453b0434c0", "sidecar_dll_sha256": "967e7dc3b31a5be8523253f31b886b4d06ba1b73e9535354b4b1f0e1cad34a99", "sidecar_patch_sha256": "b885fe7f8cd9b36d08d74ddcf1c5fe276fd16ab907d6d56893032909e392dc70", "sidecar_source_commit": "161c39c4ad167ed23b63704b46afe60e8dd63c35", "units": ["trader_escort", "book_collecting"], "verified_manifest_hashes": 45}.
Ledger: {"counts": {"genericTemporaries": 6189, "gotos": 4588, "machineTemporaries": 4649, "nativeNames": 6328, "unresolvedDiagnostics": 970}, "files": 371, "statuses": {"needs-human-review": 156, "needs-readability-work": 156, "needs-recovery": 59}, "syntaxFailures": 0}.


## Unreferenced generated labels removed

Completed work/codex_lua_unused_generated_labels_20261005/completion.json. Generic prune_unused_generated_labels scans complete file tokens at the end of each style_source round; no function-chunk boundary can hide a jump. Removes only standalone LAB_/FLOW_ labels with no referenced name anywhere, conservatively preserving duplicate names across scopes. User labels, live jumps including comment-separated operands, multiline text and inline diagnostic comments remain.127 tests+21 subtests pass (worker62184), including close-handler behavior and callback traces.

Full29 readable worker10648 terminal0,534 Lua parse, unchanged drafts/evidence hash reuse. Fifteen unused labels removed across9 files/7 units: TraderEscort2files, BanditCampBossBattle, BS_Teacher, ChickenMaster, Global_OpenChest, GuildTrainingPreMelee/TheRealGuildmaster, GuildTrainingMelee/TheRealGuildmaster, STS_BriarRose. The preliminary inventory predates the accepted jump pass; refreshing against the actual baseline adds Teacher's now-unused FLOW label, yielding15. Exact diff initially rejected one extra BanditCamp goto deletion. Removing unused labels exposes a goto immediately after do-return; the previously tested unreachable-jump rule removes it on the second style round. Independent line-index inventory reconstruction plus that single exact bare-return/goto sequence reproduces every final byte; label_interaction_validation.json and revised exact_diff_validation.json accept only those15 labels plus1 unreachable jump. No call, query, assignment, return or diagnostic changed. Seven-unit complete smoke traces including termination queries, frames and findings identical.

Actual SDK fourforms4608 cases/7544 termination reference snapshots pass (worker83683), readable Win32 fiber216 normal plus216 controlled resumed-Pause-error/parked-GC checks pass (worker9932). Exact helper hashes pinned; ownership-only C++ scope fixture and modeled engine/scheduler/copy/destructor limits remain. Nine Lua and seven reports promoted by exact hashes. All affected units packaged offline as unused-generated-labels-candidate-20261005 with existing Oakvale prerequisite:123 parsed Lua/128 manifest hashes, same sidecar161c39c/DLL967e7dc3. Initial package staging rejected a registry-versus-conversion-report ordering difference for Expressions. Verified identical members/no missing or extra scripts, retained staging, and compared sorted membership while preserving actual generated order in the report. No bundle existed at rejection; final bundle independently checked.

Ledger371/syntax0,6189generic/4587gotos/6312native/4649machine/970unresolved. No game launch/screen/input/install/save changes. Next scratch work/codex_lua_conditional_join_20261005 traces retail E01AA9..E01AFB: unequal actors skip query/assignment, equal+terminating cleans up and returns false, equal+alive assigns empty handle before join. Prototype simplifies the corresponding two-jump condition and matches36 truthiness/close-handler cases; not integrated. Continue offline until user stops, ask before future launch.


Conditional-join active checkpoint: work/codex_lua_conditional_join_20261005 baseline is completed unused-label candidate. Generic fold_literal_exit_join integrated late in style_source;207 tests+21 subtests terminal0 worker53558. Full29 worker84982 still running against stable root inputs; most units complete, no root edits while live. Actual SDK4608/7544 refs worker45688, current readable fibers432 suspensions/216 controlled errors worker17508, and additional current readable352 returning empty cases+80 fresh-process native AV boundaries worker76192 all terminal0. Trader complete smoke traces/findings/frames unchanged. Exact visible helper diff matches native E01AA9..E01AFB branch (native_join_window.json). Pending terminal full corpus then audit_outputs.py, verify_join.py, smoke.py, promote.py, package.py, audit_readability, record_complete.py. Latest promoted/package remains unused-generated-labels-candidate-20261005. No game, screen, install or save actions; continue offline until user stops and ask before future launch.


## Native-backed two-jump conditional join recovered

Completed work/codex_lua_conditional_join_20261005/completion.json. Generic fold_literal_exit_join handles exactly a closed two-jump forward join with a literal exit, straight-line prefix/body and no alternate entries/nested control. It evaluates each condition once, preserves prefix scope and LIFO local close order, and moves only bare/nil/boolean returns. Whole-file tokens reject text/diagnostic lookalikes and duplicate or externally reached labels.207 tests+21 subtests pass (worker53558):36 truthiness cases including Lua zero/empty-string truthiness, literal return arity, condition/call errors, close-handler errors, shadowed close locals and complete callback execution.

Retail helper body SHA pinned in native_join_window.json; E01AA9..E01AFB proves unequal actors skip the termination query and assignment, equal+terminating closes owned temporaries and returns false, equal+alive assigns an empty handle before continuing. Exact generated diff gate reproduces the complete output using only this reviewed conditional replacement in master/shared MakeTraderComment: each9->7 helper gotos, two labels total removed, calls/arguments/order outside the branch unchanged. Full29 readable worker84982 terminal0,534 Lua parse, all unrelated and draft Lua byte-identical with unchanged evidence hash reuse. Full Trader smoke findings and complete API traces/frame counts unchanged, including known native-empty Assassin-vector fault.

Actual SDK fourforms4608 cases/7544 termination reference snapshots pass (worker45688); current readable Win32 fibers216 normal plus216 controlled resumed-Pause-error/parked-GC checks pass (worker17508). Additionally both current readable helpers pass352 returning empty-query cases with exact native results/events and pre/postGC references, and80 fresh-process first-chance native AV cases with expected0/0x10 read addresses and zero prior events (worker76192). Fault observer exits immediately; this establishes the failure boundary, not recovery or post-fault cleanup. Ownership-only resource fixture and modeled engine/scheduler/copy/destructor limits remain; no production host teardown or full quest completion claim.

Two Lua and readable report promoted by exact hashes; focused trader-conditional-join-candidate-20261005 package37 Lua/42 manifest hashes verified with unchanged sidecar161c39c/DLL967e7dc3. No game/screen/input/install/save changes. Continue offline Lua work until user stops; ask before any future launch. Next candidate is the kind1 reply-availability flag join (remaining three jumps), requiring native/control and cleanup-order evidence before a generic fold.

Ledger: {"counts": {"genericTemporaries": 6189, "gotos": 4583, "machineTemporaries": 4649, "nativeNames": 6306, "unresolvedDiagnostics": 970}, "files": 371, "statuses": {"needs-human-review": 156, "needs-readability-work": 156, "needs-recovery": 59}, "syntaxFailures": 0}.
