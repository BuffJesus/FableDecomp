## PAUSED at user request - concrete text-bank checkpoint (2026-10-23)

The user ended the session: "Let's call it here for now. Update docs etc for next session." Native recreation is paused; do not resume under older ACTIVE entries. No native jobs remain active. No commit, push, install, game launch or cleanup was performed. Preserve all unrelated dirty-worktree changes. Lua was excluded from this native work.

The latest successful experiment is `work/native-recreation/loading/text-bank-concrete/20261023/text-232851554242`. Its `run.json` exits0 with `NATIVE_TEXT_BANK_CONCRETE_SCRATCH_PASS`: two native readers constructed/destructed, one bank constructed/destructed, one metadata entry, one actual37B read, request retirement and publication-before-free checks, owner/data/info/node deletion, and worker/installer joins. This is a single scratch baseline, not a permanent accepted gate.

The fixture now constructs the real native `NGameText::CDataBank` (size0x214) and forwards observed callbacks to native GetDataFormatVersion (106), NotifyBankSize and NotifyEntryAdded. A type0 entry at index3 produces four indexed slots, one TextEntryBuffer element with NarratorIndex -5 and null PText; group and narrator buffers remain empty. Native resource-list constructor defaults and empty-list state are checked. The manager dev-handle route, original two-reader replacement behavior, async ReadFileEx/APC/wait and native teardown remain exercised. Inactive text enumeration/last-modified/resource-list methods remain explicit cold aliases. Payload bytes are still a generated pattern: this does not establish decoded CTextEntry payloads or nonempty resource-list lifetime. No canonical source/header/catalog changes were made for this concrete stage.

Scratch inputs are `Fixture.cpp`, `Manager.cpp`, `sources.json`, `probe.py` in `work/native-recreation/loading/text-bank-concrete/20261023`. Manager.cpp copies the permanent manager bridge. The source profile extends the permanent handle-parsed baseline with native text metadata/lifetime/vector providers. The build receipt reports BUILT_HOST_PASS; receipt SHA256 `7cdd8a027de1476cff42dfccd98d0af10345da21b7d054e3be3d9a7458d137ea`, run SHA256 `835b7bba4c6ead7f3145a1e923ee1b867b79ddd165dd1a0e2ad2d44eab710e6f`. Input pin status is recorded in adjacent `checkpoint.json`.

Resume steps:

1. Read this entry, `checkpoint.json`, scratch Fixture.cpp/probe.py/sources.json and bounded receipt/run summaries. Recheck pinned sources against snapshots before relying on the old build.
2. Audit final native COFF-to-PE bodies and relocations, excluding fixture/manager bridge, and add the three metadata size/offset variants plus original-header/original-dispatcher comparisons using `native_bank_async_handle_parsed_verify.py` patterns. Add schema106, NotifyBankSize/NotifyEntryAdded, narrator index and constructor-default fault controls. Preserve the independently established two-reader behavior.
3. Only after that proof, create a repository-owned permanent gate. Later extend valid text payload decoding, actual resource admission/loading and nonempty list lifetime, group/narrator variants and a concrete retail schema route. Existing retail container parsing/IO is already native and covered; do not recreate another parser.

To rerun the scratch baseline when the user resumes: `python work/native-recreation/loading/text-bank-concrete/20261023/probe.py`. This builds a new isolated output directory; it is not the missing broader verification.

Latest permanent handle-parsed acceptance remains `rebuild/build/native-bank-async-handle-parsed/00/9d/009d56c0/20261023T231210909520Z/acceptance.json`, SHA256 `a37f1353fcffe774d1a2415183b097b47e8fd6f9ddc57707f2858f045fb4166e`:36 processes,3 variants,314 linked symbols/295 bodies,11 faults and1 allocator control. It uses the earlier synthetic schema19/type7 fixture. Retail allocator corrections described below are accepted, with receipt `work/native-recreation/loading/retail-map-allocators/20261023/acceptance.json` SHA256 `e700175a48499a83ff79a94efcc1da0efeda243a82cb650dea4098a955761fb2`. The focused retail integration gate also passed (4banks,293entries,33windows,241providers,332symbols,310bodies). No full gate_all run or historical coverage increase is claimed.

## Retail node allocator audit and corrections accepted

Before extending the retail/schema join, audited sibling map allocators after009AB120's prior fix. Two more canonical helpers called operator new where original calls malloc:alias InsertNode009AB2F0 (callsites009AB332/009AB371 ->00BFEA0E,24B nodes),contained CreateNode009AA9E0 (009AA9E4 ->00BFEA0E,40B nodes). Both native erase helpers use free. Corrected both sources to malloc without header/layout/API changes;existing76B/250B bodies remain exact after independently resolved call targets. No new historical root credit orscore normalization change.

Alias insertion gate formerly mapped the operator-new symbol to the observed malloc address;now requires malloc relocation/import identity and rejects compiled rollback to operator new. Contained-node gate now patches every relocation to actual original targets and compares all76B,verifies PE IAT thunk names malloc,and rejects rollback byboth original-code proof andnative runtime exit80 (explicit wrong-allocator host guard). Positive host allocator hook has byte-identical76B body,renamed malloc call andtyped allocation-size/count/value-copy checks. No OOM/unwind claim. Both aggregate markers now require allocator controls.

Affected insertion regressions pass:contained-node173B/2 omissions,unique254B/4 omissions,hinted672/668B/6 omissions;alias index150/149B/2 omissions;manager teardown containers7 cases/3 omissions. Native retail read-only integration freshly rebuilt241 providers,passes4 synthetic banks/293 entries/33 synchronous+queued/batch/shared windows,332 linked native symbols/310 unique bodies,andindependent wrong-offset rejection. Asset bytes unchanged andallretail input pins unchanged. This existing retail gate already covers actual BIGB parsing andshared async files;its concrete schema getter remains controlled,so the next useful join is the real game-text schema106/notification/resource lifetime rather than inventing another retail parser.

Evidence `D:/Documents/FableTLC/work/native-recreation/loading/retail-map-allocators/20261023/acceptance.json`, SHA256 `e700175a48499a83ff79a94efcc1da0efeda243a82cb650dea4098a955761fb2`. Wrong-allocation source preimages retained in retail-map-allocators/20261023. Python syntax checks pass;full gate_all not run. No manifest/catalog/artifact/coverage count changes;canonical source paths/definitions remain unchanged. All native processes terminal. Lua untouched;no game/installation/save/commit/push/cleanup.

---

## Manager handle -> parsed async worker gate accepted

Permanent `gate_native_bank_async_handle_parsed.py` joins actual manager construction/map registration/lookup/GetDevBankFilePath, CBankFile::OpenReadOnly and CBankFileAsync::OpenReadOnly, native header/entry/runtime metadata parsing, GetAsyncEntry/BeginReadData, Windows ReadFileEx/APC publication,blocking wait/request retirement and owned file/bank/thread teardown.229 native providers plusmanager storage bridge and worker host are freshly compiled with explicit profiles/generated files. Native source-to-final-PE proof verifies314 symbols/295 unique bodies;bridge-defined methods excluded.570 pins unchanged;no scratch/historical build inputs. Original header retains90.62% literal-qualified near score;base dispatcher90.38%,async dispatcher93.81% near without changing grading rules.

Original dev dispatch009D0858 calls vptr+0xC, reaching async OpenFileReadOnly. Outer async handle dispatcher then replaces its threaded reader again. Preserved rather than optimized:two native reader constructions,first physical key1,first successful CloseHandle while its pointer still valid,then final reader key2. The import observer captures closure before numeric handle reuse could make a later stale-handle probe unsafe. Manager registry is destroyed after open;bank-owned handle/path/metadata remain valid during actual async reads. Progress forwarder is native with null display;retail lookup remains explicit fail-fast and UseRetailBanks false for this dev route.

36 positive processes:three metadata size/offset variants timesfour repeats timesthree instruction variants (recreated,original-header,original base+async dispatch+header). Original base385B,async348B andheader1635B execute with live native callees and manager storage;not original whole graph. Sizes37/45/53,metadata offsets0x313/0x413/0x513,payload request offsets0x1000/0x1080/0x1100,alignment8;payload/destination guards and ordered teardown pass. One parsed runtime allocation replaces all adopted runtime/packed metadata. Eleven native exit80 faults reject previous eight plusmissing handle,wrong forwarded flags andsuppressed outer replacement. Node206B full relocated bytes andmalloc import identity checked separately;rollback tooperator new rejected by call-target proof.

Accepted `D:/Documents/FableTLC/rebuild/build/native-bank-async-handle-parsed/00/9d/009d56c0/20261023T231210909520Z/acceptance.json`, SHA256 `a37f1353fcffe774d1a2415183b097b47e8fd6f9ddc57707f2858f045fb4166e`. Aggregate exact marker registered;Python syntax checks pass. Focused regressions:base-handle512 recreated/512original/five faults;async-handle512/512/six faults;manager pathfour exact roots. Prior standalone worker controls remain separate. Full gate_all not run. No production CPP/header/catalog/manifest/coverage edit this step. All owned native jobs terminal.

Scratch link failure handle-230424000310 preserved:overbroad union pulled duplicate string/accessor providers;selected onlyneeded manager units without force-link. Positive scratch handle-230521976082 confirmed new path. Original-dispatch.exe initially failed WinError740 before startup;original-routes.exe ran byte-identical generated image without elevation (hashes checked). No permissions or system settings changed.

Next reduce observed schema/notification boundaries andjoin actual retail-container loading;keep dev double-reader behavior and native exception qualifiers. User's native recreation goal remains active;Lua untouched;no game/installation/save/commit/push/cleanup.

---

## Permanent parsed-async native gate accepted

`gate_native_bank_async_parsed.py` and repository-owned fixture/build/source-profile/verification modules now reproduce parsed metadata through real async read/publication/owned teardown.209 genuine native providers plusone host fixture are all freshly compiled. Source/profiles are explicit and test files are generated from an independent payload pattern/format;no work/historical build/source/object/seed dependencies remain. Host schema/notification vtable,installer synchronization and allocator adapters remain explicit. No canonical production function/header/catalog/manifest/count change this step.

24 positive native processes coverthree size/offset variants/four repeats/two header variants. Twelve use recreated header,twelve use transplanted original header;whole graph is not transplanted. Native entry parsing owns runtime table;real GetAsyncEntry/BeginReadData/ReadFileEx/APC completion/blocking wait/request retirement/owner release/file and bank destruction/worker+installer joins pass.284 native linked symbols/271 unique bodies verified. Sizes37/45/53,entry offsets0x313/0x413/0x513,payload read offsets0x1000/0x1080/0x1100,alignment8;actual parsed size/offset fields determine the requests,full destination guards/payload bytes checked. Hand-adopted runtime/packed metadata remains absent.

Eight explicit exit80 controls:skip entry parsing,wrong parsed size/offset/alignment,skip threaded open,missing completion Finished publication,wrong native request size and leaked native file handle. The additional completion guard fires on worker publication before request retirement;request-size guard fires before releasing manager lock;handle-leak check confirms final invalid handle. Prior standalone worker gate/87 controls remains separate and unchanged;not recharacterized as this composed gate's coverage. Header's unchanged near grader plusindependent literal proof passes90.62%;retains raw88.62%,11 literal-target controls andsemantic swap test. No threshold changes,assembly or native unwind claim.

Acceptance `D:/Documents/FableTLC/rebuild/build/native-bank-async-parsed/00/9d/009d5820/20261023T225848021823Z/acceptance.json`, SHA256 `ca4821490717b1838456af3142f50b4ca18058a7910463aa3b11bcf39ecb2120`;all545 pinned inputs unchanged. Python syntax checks pass;full exact aggregate marker registered in gate_all. Full gate_all not run. All native jobs terminal. Next join bank-manager handle dispatch with the async parsed path,then continue real derived schema/retail-container/loading integration. Lua untouched;no game/installation/save/commit/push/cleanup.

---

## Parsed native metadata reaches real async worker

Work-only async composition removes the old controlled CBankFile::ReadFileInformation body, manually adopted runtime table and handcrafted packed size/offset arrays. Canonical header and actual ReadEntryList allocate/populate the four-entry runtime table from a generated bank file;schema/size/entry/post notifications remain observed host vtable boundaries. Actual CBankFileAsync::OpenFileReadOnly then opens its threaded reader; native GetAsyncEntry/BeginReadData uses the parsed index3,size/offset/alignment; real Windows ReadFileEx/APC publication, blocking wait,request retirement,owner release,file/bank destruction and worker/installer joins pass.

24 native runs:12 recreated-header and12 original-header-transplant processes,three variants timesfour repeats. Parsed sizes37/45/53,metadata offsets0x313/0x413/0x513,alignment8,payload request offsets0x1000/0x1080/0x1100. Independent CLI expectations and full destination guards/payload pattern verify each run. One native48B runtime allocation remains until destruction;one metadata free replaces oldthree fake allocations. No hand-adopted runtime or packed metadata remains. Source-to-final-PE proof checks284 linked native symbols/271 unique bodies. Original comparison transplants only header;whole graph not original/unwind not claimed.

Five new native faults reject explicit exit80:skipped header entry parse,wrong parsed offset,size,alignment and skipped threaded open. Both created threads join on positive runs. 581 current source/header/Python/toolchain/import DLL/parent fixture/case-file pins unchanged;compiled sources/headers independently equal current canonical snapshots. Final receipt `D:/Documents/FableTLC/work/native-recreation/loading/bank-async-parsed/20261023/parsed-225530386020/async-parsed-checkpoint.json`, SHA256 `ecfdf8b994a45be7ad5fc4df123b05c5ee490be06a1977cca97f645333e7d6e3`;summary `D:/Documents/FableTLC/work/native-recreation/loading/bank-async-parsed/20261023/checkpoint.json` SHA256 `566e721cc5ffb6bad726113c6ae4beada42c6433532ba528e139260d65a73ca6`.

Preserved initial failed links and two guarded fixture failures:parsed-224654972468 duplicates wide-string/accessor providers and missing inactive input dependencies;parsed-224807148134 andparsed-224856979109 treated parser heap temporaries as lifetime tree headers in the old worker harness. Corrected provider selection uses existing operator-family bodies;no force-link. Scoped allocator adapters cover old/new malloc/free bodies while preserving explicit initializer/tree allocator hooks,all header free ordering/guards and string accounting. No allocator guard was removed. First end-to-end parsed-225022412862;fresh pinned single-variant parsed-225346349929;final three-variant parsed-225530386020. All native jobs terminal.

This is work-only/unpromoted. Permanent manager and owned-open gates unchanged. Direct async file-open seam only;manager-handle route,retail containers,malformed metadata/native unwind,real derived schema and allocator/installer replacement remain separate gaps. No production CPP/header/catalog/manifest/coverage edit this turn. Next repository-owned parsed-async fixture/build/proof/fault gate,retaining prior worker controls,then join manager handle dispatch. Lua untouched;no game/installation/save/commit/push/cleanup/full gate_all.

---

## Native manager owned-open gate accepted

`gate_native_bank_manager_owned_open.py` is now repository-owned and registered in gate_all with the full exact marker. Fixtures `NativeBankManagerOwnedOpen_test.cpp` and `NativeBankManagerOwnedOpenBridge.cpp` own storage/configuration only; native manager ctor, map registration/lookup/GetDevBankFilePath, handle OpenReadOnly, native file/header/parser/indices/packing/direct ReadData and bank/manager teardown compose. Bridge-defined retail lookup is explicit fail-fast and bridge symbols are excluded from native linked-body counts.

Permanent gate passes9600 dev-handle successes across five path spellings/64 flags,59640 entries/30560 updates per recreated and original-header host;230 native linked symbols/222 unique bodies verified. Prior7 ownership faults/six helper faults retained;four manager faults reject missing handle,wrong forwarded flags,empty dev path and rollback to the wrong node allocator. Existing handle381B near90.38%,dev-path145B exact,corrected node206B allocator-call proof retained. Direct file-open seam separately passes320 missing/1600 malformed attempts;no manager-lookup failure or original whole-graph/unwind/async claim. UseRetailBanks is enabled only when input flag4 forces dev dispatch. Generated schema and notification vtable remain observed fixture boundaries.

Receipt `D:/Documents/FableTLC/rebuild/build/native-bank-manager-owned-open/00/9d/009d06f0/20261023T224158109486Z/receipt.json`, SHA256 `88ef654d9361e1c05ba04595884639e9f348867ca8734cc40b923d95fc31e333`. All495 pinned inputs unchanged,no work-directory dependencies. Manager constructor172B/layout6/omission1,destructor229B/teardown7/omission1 and container7cases/omission3 regressions pass. Python syntax checks pass;full gate_all not run. No new production root/header/catalog/manifest/coverage edit in this step. All native processes terminal.

Next actual async reader/lifecycle composition with this owned metadata path;retain existing native worker request/publication/removal/teardown evidence while replacing its controlled metadata. Lua untouched;no game/installation/save/commit/push/cleanup.

---

## Native manager-to-header integration and allocator correction

Work-only composition now runs real CBankFileManager constructor, map operator[] registration, native alias/map Find/GetDevBankFilePath, CBankFile::OpenReadOnly dev dispatch, actual file/header/parser/query/read path and both bank/manager destruction. Five path spellings9600 opens/59640 entries/30560 updates per recreated and original-header host;320 missing/1600 malformed (these still exercise direct file-open error seam, not manager handle failures).230 native selected linked symbols exclude Manager.cpp host bridge. Generated schema/notifications/storage/configuration remain controlled. UseRetailBanks true only when input flag4 explicitly forces dev dispatch; actual retail-container lookup remains fail-fast. No manager retail/async composition claim.

Guarded allocator exposed genuine source error: CBankInfoMap::InsertNode009AB120 called operator new for40B nodes, Erase called free, while original callsites009AB162/009AB18B callBFEA0E (PE import thunk malloc). Existing gate misleadingly mapped the new symbol to that malloc address. Corrected canonical provider to malloc. Code remains206B exact after expected calls; existing pair79B exact. Gate now verifies malloc import identity, requires _malloc relocation name and explicitly rejects a compiled rollback to operator new. Insertion chain/7 omissions and erase/close composed regression pass. Aggregate marker now requires allocator=malloc controls=1. No metadata or recovered-count change.

Initial missing global-storage external linkage and inactive FindContainedBank dependencies corrected in host only. Then two native runtime failures correctly caught new/free provenance mismatch; no guard relaxation. Preserved manager-223542743609,manager-223609590471,manager-223659038336. First corrected scratch manager-223737197478. Latest uses corrected canonical source manager-223910554041; every native input pin unchanged. Scratch handle381B retains90.38% near, dev-path145B exact, map-node allocator call target independently checked. Four new native faults reject missing handle,wrong forwarded flags,empty dev path and allocator rollback (explicit ALLOC_PAIR got1 wanted0 size40). Prior7 ownership+6 helper controls retained. Manager path exact and bank-handle512 original/recreated/5 controls regressions pass.

Checkpoint `D:/Documents/FableTLC/work/native-recreation/loading/bank-manager-open/20261023/checkpoint.json`, SHA256 `8561d4647517c23051d8d9976e90257061171e2689aef619f6972648a1345fb2`. Manager composition unpromoted, permanent owned gate unchanged; production allocator correction and its regression gate accepted. All owned native processes terminal. Next make manager bridge/fixture/source list/verification repository-owned and compose alias/registration/error cases, then join actual async reader. Lua untouched;no game/installation/save/commit/push/cleanup/full gate_all.

---

## Typed parent-path helpers promoted with permanent native gate

Canonical shared-header providers are now `00/9b/CStringParser_ReadAsStringUntilStringItem_009ba110.cpp` (89B,98.55% near,304 bounded original/recreated cases) and `00/9a/CBasicString_char_SetRepeated_009a0390.cpp` (159B,96.92% near,440 bounded original/recreated cases). Six helper defects reject. No scoring changes or assembly. Historical local-layout/wrong-signature source files and tests retained on disk but their four symbol-index rows (including placeholder global new[]/delete[]) retired from native provider selection. Fresh catalog compilation reproduces each accepted COFF body/relocation set; expected symbols have correct009BA110 strong and009A0390 COMDAT identity. Receipt pin checks gate catalog selection.

Permanent `gate_native_bank_owned_open.py` now includes native parent normalization/parser/string chain: five spellings,9600 successes,59640 entries,30560 updates per recreated and original-header host;320 missing/1600 malformed opens;207 final linked symbols;two exact parent roots plus existing eight selected exact roots. Seven native ownership defects and six helper defects reject. Generated schema/notification vtable and no-throw/error-helper qualifiers remain explicit; original comparison transplants the header root, not the whole graph. Malformed cases use32 distinct no-throw policies repeated across64 input policies. No native unwind, retail-group loading or whole-game claim.

Repository-owned model sources and all470 native inputs pinned/unchanged. A first permanent run passed host/model comparisons then exposed local runner variable reuse after the first helper fault; fixed and rebuilt, failed directory retained20261023T222959991576Z. Accepted receipt `D:/Documents/FableTLC/rebuild/build/native-bank-owned-open/00/9d/009d1090/20261023T223035198774Z/receipt.json`, SHA256 `8cfb96b327872a777e8200f81798a21baf189186e500ab22a3e9c432c3b61ebd`. No scratch path dependencies. All owned native processes terminal.

Affected regressions pass: native narrow and wide absolute-path exact roots,parser delimiter48cases,parser init2cases,basic char copy/truncate/erase/fill/text constructors,string truncate/erase/save. `gate_all` exact aggregate marker updated; full aggregate not run. Manifest and artifact tables update only two existing address rows; symbol table removes four obsolete provider rows and adds two correct providers. `promotion.json` proves every unrelated metadata row byte-identical. Official coverage/readme generators run; recovered address total unchanged18870 because these addresses were already counted under old adapters. No manual coverage edit.

Next: compose actual owned metadata decoding with native bank-handle/manager/async loading callers, reducing schema/notification observer boundaries using recovered concrete classes. Continue native recreation;Lua untouched;no game/installation/save/commit/push/cleanup.

---

## Parent-relative native bank-open checkpoint

Removed the cold parent-normalization boundary in a work-only composition. Actual wide/narrow GetAbsoluteFilePath, CStringParser ctor/init/cursor/item/string chain now opens parent-relative paths, alongside four existing spellings. Scratch passes9600 successes/59640 entries/30560 updates,320 missing/1600 malformed attempts per recreated and original-header host.207 selected linked symbols verified. Narrow786B and wide92B parent helpers independently equal original after relocation; final COFF-to-PE proof retained.

Discovered two needed units were old layout/symbol surrogates: parser item reader009BA110 and basic string repeat setter009A0390. Recovered genuine shared-header C++ candidates in work, without changing those old files. Item89B vs original93B gives98.55% near and passes304 original/recreated bounded full-parser/item-memory/trace/ABI comparisons. Setter159B vs158B gives96.92% near and passes440 original/recreated guarded object/buffer/allocation/ABI comparisons including char high-stack poison and both capacity-bit settings. Six helper fault controls reject: return/type/start, repeated char/length/tail. Existing seven native bank ownership faults also reject. No byte chasing after near+behavior.

Initial scratch links rejected strong/COMDAT string duplicates and missing basic-string leaves; selected operator-family shared providers without /FORCE. Original placeholder SetString source also exported global new[]/delete[] and wrong class signature; excluded it, recovered typed template instead. Failures preserved in parent-222437468632,parent-222503539862,parent-222518806884. First positive parent-222550224605. Model runner bytearray input corrected to bytes before acceptance. No production code change, no new helper promotion, no permanent parent gate claim. Existing four-path permanent gate remains authoritative.

Checkpoint `D:/Documents/FableTLC/work/native-recreation/loading/bank-parent-open/20261023/checkpoint.json`, SHA256 `9c5b3ef843e4fdda458a279b3986de8ca617dea6fe83546c90662ba2daf3a12d`. Original native input pins unchanged; new models/controls/source/object hashes recorded. Next turn make these two helpers and models repository-owned, integrate parent path proof and controls into permanent gate, then register qualified native bodies and run affected parser/string regressions. Native work active;Lua untouched;no game/installation/save/commit/push/cleanup.

---

## Owned bank path and false-return coverage accepted

Extended the permanent owned-open gate from absolute valid files to four spellings: absolute, plain relative, dot-relative and drive-rooted. Each spelling runs the complete 1920 readonly/64 flag corpus, yielding7680 successes,47712 entries,24448 updates per recreated and original-header host. Both hosts additionally pass256 missing-file opens and1280 nonthrow malformed-header opens covering empty files, bad magic, wrong bank version, wrong schema version and interrupted state. Malformed attempts force flag2 to suppress throws:32 distinct effective flag policies, each exercised twice by the64 input flag combinations. Missing-file checks retain all64 distinct policies. No native throw/unwind or original whole-graph execution claim.

Failure assertions check false returns, no opened/entry notifications, retained original filename policy, native constructor defaults and specific HeaderSize/FileSize/Alignment changes on interrupted-state rejection. Actual CloseBankFileImpl releases counted disk ownership before bank destruction. An exclusive writable reopen proves no native read handle remains; native destructor then releases bank headers and all guarded allocations/string instances. Three added controls reject skipped failure cleanup, incorrect missing-file success and omitted OS CloseHandle. Seven controls total; all explicit exit80. Existing183 linked-symbol/eight selected exact-body/literal near proofs remain passing. Full gate_all not run; exact aggregate marker updated.

Receipt `D:/Documents/FableTLC/rebuild/build/native-bank-owned-open/00/9d/009d1090/20261023T222211288608Z/receipt.json`, SHA256 `96a97c66e1f151d0b6f161bda280652da1267fca253390a95f8a0c5e5722fbd7`;447 pinned inputs unchanged. No production C++/header/catalog/manifest/coverage edit in this checkpoint. All native processes terminal. Lua untouched. Next recover/compose parent-relative path helpers and expand original-compatible owned loading beyond observer schema/notification boundaries; preserve explicit native exception qualifier.

---

## Native owned bank open accepted

Permanent `gate_native_bank_owned_open.py` now runs actual CBankFile::OpenFileReadOnly through native PathExists, GetCurrentPath, ExpandPath, ConvertPathToFullPath, disk Open, counted ownership, canonical header, real entry/parser/index/packing/query/string graph, then native bank destruction. Generated valid files exercise 1920 readonly cases, 64 flag combinations, 11928 entries and 6112 updates; recreated and bounded original-header hosts agree. The full generated corpus remains 3840 cases; writable cases are deliberately filtered because this API forces readonly. No original whole-open execution claim.

Direct native ReadData checks a 32-byte header window and surrounding guards. Real counted peers exercise reference counts 1/2/3, live handles retained after bank destruction and final native release/close. Native bank ctor/dtor own tree headers and final capacities. Raw bank layout, schema19 and notification vtable remain observed fixture boundaries. The cached native current-path global has explicit baseline allocation accounting. Absolute path conversion is now native, replacing the initial identity fixture. Relative parent-path helper, error-only dependencies, exception unwinding, payload decode and retail BIGB grouping remain outside this gate.

Fresh linked proof checks 183 selected native symbols/178 unique bodies. Eight selected retail roots are exact after independently resolved relocation, including OpenFileReadOnly204B, counted Reset103B/DeleteData11B, ReadData40B, PathExists111B, ConvertPathToFullPath64B, existing exception ctor33B and disk Open640B. Canonical header retains raw88.62% and independently proven literal90.62% near grade with unchanged grading rules;11 target faults and semantic swap reject. Four new native C++ faults reject with explicit exit80: skipped header, opened callback, filename assignment and reference release.

Receipt `D:/Documents/FableTLC/rebuild/build/native-bank-owned-open/00/9d/009d1090/20261023T221818594037Z/receipt.json`, SHA256 `f7fe7d7188858e022930a41ffc2612d5554b52e7ae9536ea93269beb90de083e`. All pinned inputs rechecked unchanged. Registered exact marker in gate_all; full gate_all was not run. No production source/header, catalog/manifest or coverage count change in this step. Lua untouched. Earlier work-only failures remain preserved: cached-path allocation baseline assertion, virtual dispatch into observer trap and uninitialized observer disk pointer were fixture errors corrected before acceptance; no assembly or forced link workaround.

Next: extend owned-open path/error coverage against original evidence, retaining valid-file/lifetime proof and explicit exception boundaries. Recreation remains active until stopped.

---

## Native owned-open graph in progress

Replacing direct HeaderRead/handle adoption with actual OpenFileReadOnly, native PathExists/ExpandPath/GetCurrentPath/disk Open/counting/header/parser plus opened callback and a guarded native bank ReadData window. Test corpus selects1920 readonly cases because the API forces ReadOnly=true;64 flag modes retained. Genuine peer counted pointers replace synthetic reference-count assignments. Two initial scratch links exposed strong/COMDAT collisions from independently recovered wide-string family units andmissing path/string leaves. Corrected selection uses shared operator-family bodies; no /FORCE or assembly bypass. Failures preserved under bank-native-open/20261023/disk-open-220327528245 anddisk-open-220543074087. Relative absolute-normalization branch stays explicit cold boundary for this absolute-path-only seam; no relative-path claim. Work continues; no success claim yet.

---

## Native bank-header decoder promoted

CBankFile::ReadFileInformation009D0880 is registered as genuine C++ NEAR_MATCH/PASS. Canonical1615B source keeps the existing exception ctor visible in its translation unit and emits its exact33B009D1E70 body. The shared nested CProgressCallback declaration matches PDB five-method/4-byte/log-slot2 evidence without changing stored fields or bank layout. Constructor primary manifest row remains unchanged; alternate provider row has correct009D1E70 address, and duplicate strong ctor linking is explicitly rejected.

Permanent `gate_native_bank_header.py` passes1512 nonthrow/full-bank/ABI cases,30 controlled throw-boundary cases,648 helper mutations,3840 valid native cases per recreated/original header root,64 native disk nonthrow cases,11 native C++ fault controls,11 literal-target controls,one semantic literal-swap control andtwo source-selection controls. Conservative raw score88.6228% is retained. Existing grading API gives90.6188% only with11 independently exact full-literal immediate operands/counts in read-only retail data; no PE relocations or numeric/RTTI masks invented, no thresholds changed.13 original stream bodies/eight active linked bodies/four disk bodies are exact, header bytes/relocations agree across hosts, and emitted ctor is exact. Allmodels/fixtures/helpers are repository-owned; no work-path dependency.

Receipt: `D:/Documents/FableTLC/rebuild/build/native-bank-header/00/9d/009d0880/20261023T214347930311Z/receipt.json`, SHA256 `cee40836f5a51ce70e1cfe0e647ec77278f04a7360a73ddf1ecdaa9b0d5c1e8e`. Catalog compilation independently reproduces both selected bodies/addresses/strengths. Missing header/duplicate ctor controls reject. Bank-entry lifetime regression passes3840 cases per root/59 controls/87 omissions and standalone exception ctor regression passes120 model/20 host/five controls/exact33B. Current gate source/header/Python/toolchain pins remain unchanged; promotion changes only the otherwise-unused source-index metadata pin after validation, recorded in promotion.json. Manifest/artifact/index/gate_all updated only for this recovery; progress reports regenerated with official scripts. No full gate_all run.

Boundaries remain explicit: controlled bank layout/tree headers/manual vector teardown/base/notifications/backing handle adoption, and controlled helpers at throw boundary; native exception unwinding/full Open lifecycle/arbitrary malformed inputs/whole game remain unproven. Initial permanent preflight found a historical cold-alias include name; changed fixture to canonical NativeFileInputStreamColdAliases.h before successful run. Lua, installed files, saves, OS clock, commits/pushes untouched.

Next native seam: compose actual CBankFile::OpenFileReadOnly and owned disk/counting/bank lifetime with this native header and real entry parser on an exclusively generated valid BBBB bank. Remove the earlier controlled ReadFileInformation boundary, keep one ctor provider, independently compare metadata/read windows and native Close/destructor ownership. Preserve error/unwind qualifiers and original return/error policy. Recreation remains active until user stops.

---

## Bank-header context: native composition and operand-proof checkpoint

Same-TU constructor candidate1615B remains scratch/unpromoted. It passes1512 nonthrow/30 throw-boundary/648 helper-mutation comparisons plus fresh native3840 valid-header cases per recreated/original header root (24520 entries/12432 updates each) and64 real disk/header nonthrow policy cases. Native header body/relocations equal the bounded reference. Thirteen original stream bodies/eight active linked bodies andfour disk bodies are exact; emitted exception ctor33B is exact. Eleven native C++ faults detected.

Raw existing score remains88.622754491%,below90%; no threshold or normalization logic changed. Retail haszero PE relocation entries here,so a separate source-symbol proof validates all11 PUSH immediate string targets against full terminated COFF literal bytes in read-only/noncode retail data with exact occurrence counts. Supplying only those proven sites to the existing compare API gives90.618762475% near grade. All11 target-to-PE-header faults reject. A valid-literal target swap deliberately preserves the data multiset; the independent throw-message oracle rejects it. Data proof alone is explicitly insufficient. RTTI/numeric constants/other unproved operands remain unmasked.

Evidence receipt: `work/native-recreation/loading/bank-header/20261023/header-context-checkpoint.json`, SHA256 `a98d5d4b61321246e237606a086a948ac4c7ef5efc99c2dc19084cebbe1f7f0c`. Fresh native valid-chain384 input pins unchanged. Native error-host uses current canonical headers, not the first historical-header run. Existing donor-locals-progress.json and both Ego_r/FableWin XML confirm CBankFile::CProgressCallback nested class,size4,five methods and log slot2 (const CCharString&); typed production interface still pending. Compile attempts: named temporaries/guarded branches/direct magic and /Os,/Ot,/Ob1,/Og- do not improve the raw candidate; class-body constructor placement matches current1615B. Keep best ordinary/inline constructor source; forcing inline is worse.

Next: build a permanent isolated gate from these pinned canonical inputs and proven operand correspondence. Add shared nested progress interface without changing class layout; retain genuine constructor visibility and one selected ctor provider in the graph (new TU currently also emits the already-recovered ctor; exclude duplicate standalone provider). Prove constructor/body selection and native/model controls before source/metadata promotion. Actual exception unwinding, full bank Open/lifecycle and arbitrary malformed-file coverage remain outside these receipts. The103-control loading checkpoint remains accepted; Lua/install/save/OS clock untouched. Allowned jobs terminal, recreation remains active until user stops.

---

## Bank-header constructor context: bounded differential checkpoint

Fresh same-TU constructor candidate1615B passes all existing1512 nonthrow,30 throw-boundary and648 helper-mutation comparisons against original1635B ReadFileInformation009D0880. Signature routing, version/alignment/state handling, error message/type/helper order and GetLength/recovery/entry-reader mutation remain equal under the existing controlled models. Comparison rules unchanged: default opcode similarity88.622754491%,instruction delta0.79523%,complete decode, still below90% near threshold. The1592B current-header baseline reproduces79.879275654%; forcing constructor inline worsens to84.662576687%. No production/parser orcoverage promotion, no native exception unwinding proof. Existing103-control loading checkpoint remains accepted.

Bounded receipt: `work/native-recreation/loading/bank-header/20261023/behavior-212021375288/receipt.json`, SHA256 `1a009095eb03bb901ba20bb12abe833a1efe59c067ab098717641a3729488ae7`. Compile report: `work/native-recreation/loading/bank-header/20261023/context-211916606195/report.json`; best same-tu-ctor.cpp/obj. Initial script root-depth/import and missing callback-interface compile attempts are preserved. Candidate uses canonical current headers and existing genuine constructor definition plus explicit scratch three-slot progress interface matching prior inferred vslot2 boundary; canonical ProgressCallback remains void*,declaration breadth unpromoted. No canonical headers/sources changed. Original span haszero PE relocation entries; no scoring masks or thresholds changed.

Continue native header recovery from this tested source: compare remaining error/throw block placement and stack temporary lifetimes with original, validate any new candidate using the same1512/30/648 models and then native stream/disk closure before promotion. Never replace native routing with fitted algorithms or weaken grading to cross threshold. Lua, installed files, saves and OS clock remain untouched. Allowned jobs terminal; recreation authorization remains active until user stops.

---

## Allocated EOF pending disposal: permanent checkpoint

Complete native gate passes512 loading combinations twice,103 fault controls,46 selected exact roots/four near contexts. Six real cached autoallocated EOF reads (three heap/three pool) use an explicit typed boundary to seed exactly32 owned bytes before OS submission, then hold worker nonalertably. Native discard transfers sole data ownership to DisposalList and pending abort refuses; bank publication/captured UID removal join before independent partial/seeded-tail inspection and drain. Partial13/error0 and0/error38 retain native logical DVD dirty policy. Native heap/pool buffers and bank collections clear; original pool lock accounting is preserved. Flags/callback/forwarder restore through public setters under lock, character counts restore and native allocator shuts down withzero tracked buffers. Four original/recreated worker/Close combinations run twice. Two safe no-drain/no-free controls fail cleanup assertions; no early deletion/UAF control or error-byte decode.

Receipt: `rebuild/build/native-text-eviction/00/9f/009fc170/20261023T211250360126Z/receipt.json`, SHA256 `0b100e8f846e6ee3f0c56fa05d67264008d7c26c294f5a14ba440ea7fc9bae59`. All796 pinned inputs unchanged. gate_all now recognizes verified103-control/allocated_eof=6 marker; full gate_all not run. Native production bodies unchanged; no new whole-project coverage claim. Lua, installed files, saves and OS clock untouched.

Next native gap: CBankFile::ReadFileInformation009D0880 remains functional-only/unpromoted. Fresh scratch context comparison found visible genuine exception constructor improves original1635-byte decoder similarity from79.8793%(1592B) to88.6228%(1615B); marking it force-inline drops to84.6626%,so do not force it. Existing comparison rules unchanged; retail PE haszero relocation entries in this span. Context report: work/native-recreation/loading/bank-header/20261023/context-211916606195/report.json. Same-TU candidate bounded differential verification currently running; no behavior/promotion claim yet. Work only, canonical headers/constructor unchanged. Resume source-context/remaining error-block correspondence and preserve all parser/exception boundaries. Recreation remains active until user stops.

---

## Caller-buffer bank EOF: permanent checkpoint

Complete native gate passes512 loading combinations twice,101 fault controls,46 selected exact roots/four near contexts. Three actual cached bank caller-buffer reads use explicit offsets crossing/at/beyond intact generated archive EOF. Native bank callback publishes Finished/GetData, retains requestedLength32 and descriptorSize32, allocates no internal buffer and enters no restart/disposal/decode path. OS completions13/error0 and0/error38 preserve logical DVD dirty policy. All64 caller bytes are guarded; partial bytes independently match the archive. Captured native UID removal joins after bank publication before releasing caller storage; existing completed UID store race is preserved. Native bank/manager/state and character counts clean up. Four original/recreated worker/Close combinations pass twice. Two safe saved-length/descriptor controls fail explicit assertions; existing63-byte bank completion method is freshly exact with relocations.

Receipt: `rebuild/build/native-text-eviction/00/9f/009fc170/20261023T210631790253Z/receipt.json`, SHA256 `0b511fce28b782fbd3511d630c2400eea372d3dbeb3f182ea1e66806176afcaf`. All796 pinned inputs unchanged. gate_all now expects verified101 controls/bank_eof=3/46 exact; full gate_all not run. Native production bodies unchanged; no new whole-project coverage claim. Lua, installed files, saves and OS clock untouched.

Continue with passing allocated EOF pending-disposal scratch below: six real cached reads (three heap/three pool), explicit seed32 owned bytes, nonalertable post-submission hold, sole-reference disposal retention and native UID join before independent payload/seeded-tail checks/drain. Integrate isolated --bank-allocated-eof-disposal with two safe cleanup controls (expected103/46 exact/four near). Preserve original pool lock accounting; never decode error bytes. Recreation remains active until user stops.

---

## Caller-buffer EOF: permanent integration active

Integrated --bank-caller-eof uses an intact build-owned D: archive. Explicit request offsets cross/meet/exceed EOF; no metadata mutation or invented bounds guard. Captured native UID removal joins after bank publication before caller storage release. Focused integrated fixture passes four original/recreated worker/Close combinations twice, both saved-length/descriptor controls, and the freshly exact63-byte bank completion root. One initial focused negative run omitted --bank-caller-eof and reached unrelated cold normal flow; fixed the isolated child argument before full launch. Complete gate now running with expected101 controls/46 selected exact roots/four near. Accepted99-control checkpoint remains current until completion. Native bodies unchanged; Lua excluded; full gate_all not run.

---

## Allocated EOF pending disposal: scratch checkpoint

Six actual cached native bank autoallocated EOF reads (three heap, three movable pool) pass four original/recreated worker/Close combinations twice. Explicit offsets cross/meet/exceed intact archive EOF. Typed API boundary seeds exactly32 owned bytes before real ReadFileEx then holds worker nonalertably; native discard transfers sole data reference to DisposalList and pending Abort refuses. Completion publishes partial13/error0 or0/error38; fixture joins captured native UID removal before inspecting buffer and draining disposal. Independent partial bytes and explicit seeded unread tail pass. Native drain frees heap/pool and alldata/restart/disposal collections clear; pool original lock accounting is preserved. Flag/callback/forwarder restore through native public setters under manager lock, character/wide counts restore and native allocator shuts down withzero tracked buffers. No decoding of error bytes.

Receipt: `work/native-recreation/loading/text-bank/20261023/bank-allocated-eof-disposal/receipt.json`. Two safe controls (skip disposal drain, skip heap buffer free) fail final cleanup assertions; no early deletion/UAF control. Scratch only. Next integrate isolated --bank-allocated-eof-disposal with generated D: archive andtwo controls (expected103/46 exact/four near). Native production bodies unchanged; no new global coverage, direct-I/O, arbitrary concurrency or failed-submission guarantee. Caller-buffer permanent101-control gate remains running; accepted checkpoint99. Lua excluded.

---

## Native public EOF policies: permanent checkpoint

Complete native gate passes512 loading combinations twice,99 fault controls,45 selected exact roots/four near contexts. Existing native public callback/dirty setters now handle EOF state setup/restoration. Nine cached C: reads cover unset, installed and callback-self-clearing policies with expected callback counts0/2/1; partial13/error0 and at/beyond EOF0/error38 preserve user success, requested32-byte totals, callback pointer mutation, dirtyfalse and independent guard/tail payloads. Native dirty setter true/false transitions pass. Allfour original/recreated worker/Close combinations run twice; original callback/flag restore via native setters under manager lock after ownership joins. Both setter7-byte bodies/one relocation each are freshly exact; ignored-null/dirty-assignment controls fail explicit assertions. Existing provider bodies unchanged; no new whole-project coverage claim.

Receipt: `rebuild/build/native-text-eviction/00/9f/009fc170/20261023T205811200731Z/receipt.json`, SHA256 `d1d434887ca1be184bc52251f28c1e67c359ef4693009080e558c5c5bc38d06a`. All796 pinned inputs unchanged. gate_all now recognizes verified99-control/public_policy=3/45-exact marker; full gate_all not run. Lua, installed files, saves and OS clock untouched.

Continue with passing caller-buffer bank EOF scratch below: isolated --bank-caller-eof child, intact generated D: archive, explicit beyond-entry offsets, native UID removal join before caller storage release, two safe saved-length/descriptor controls and one existing63-byte strict bank completion root (expected46). Do not invent bounds policy or decode EOF/error bytes. Recreation remains active until user stops.

---

## Caller-buffer bank EOF: scratch checkpoint

Three cached reads through native CBankFileAsyncEntry::BeginReadData intentionally use explicit offsets crossing/at/beyond the generated archive EOF, with intact bank metadata. Native CBankFileAsyncData publishes Finished/GetData to the caller pointer, retains requestedLength32/descriptorSize32, allocates no internal buffer and enters no restart/disposal/decode path. OS completions13/error0 and0/error38 preserve native DVD dirty policy. All64 caller bytes are guarded; partial bytes independently match archive and unread tail remains unchanged. Fixture captures native UID at real submission, waits bank publication then native UID removal before releasing caller storage, and deliberately makes no UID-zero assertion (existing publication/store race). Native bank/manager close and character/wide counts restore; flags/callback/forwarder restore under manager lock.

Allfour original/recreated worker/Close combinations pass twice. Two safe completion faults (zero requestedLength, zero saved descriptorSize) fail explicit assertions. Existing63-byte bank completion source freshly matches original with relocation checking; no new whole-project coverage. Receipt: `work/native-recreation/loading/text-bank/20261023/bank-caller-buffer-eof/receipt.json`. Scratch only. Next integrate isolated --bank-caller-eof child using build-owned D: archive, two controls and one strict root (expected46). Do not invent bounds checking or decode EOF/error bytes. Public-policy99-control gate remains running; accepted checkpoint97. Lua excluded; no failed-submission/physical-media claim.

---

## Native public EOF policies: permanent integration active

Existing six-method error-policy provider is linked into native loading composition; EOF setup/restoration now uses genuine public callback/dirty setters through typed fastcall bridges. --eof-public-policy integrates three policies/nine real cached reads, original/recreated worker/Close combinations twice and two safe setter faults. Focused integrated fixture and controls pass. An initial focused negative child omitted its required source environment and rejected at dispatch; fixed by passing the same child-only source environment. Complete gate now running, expecting99 controls/45 selected exact roots/four near contexts. Two strict7-byte/one-relocation setter descriptors reuse threaded_error_policy_oracle; no native provider body change or new whole-project coverage claim. Accepted checkpoint remains97 controls until full completion. Lua excluded; full gate_all not run.

---

## Actual EOF callback peer admission: permanent checkpoint

Complete native gate passes512 loading combinations twice,97 fault controls,43 selected exact roots/four near contexts. A real cached harddisk EOF error callback observes live status2/UID and pending1/32, refuses active removal/priority change and admits a peer32-byte read via native AddObject (pending2/64). Native user completion stays true for EOF and peer; peer auto-removes and original remains finished1/32 until Close. OS completions0/error38 then32/error0, original buffer untouched, independent peer payload/guards and joined file/callback/event ownership pass. State restores under native manager lock. Allfour original/recreated worker/Close combinations run twice. Two safe controls detect active priority mutation and false remove-success without deleting the active object.

Receipt: `rebuild/build/native-text-eviction/00/9f/009fc170/20261023T205117222283Z/receipt.json`, SHA256 `8dceb6ab1db101c64ca98ddf5566b498501614f3134755a187419e324c51dd91`. All794 pinned inputs unchanged. gate_all now expects verified97 controls/eof_peer=1; full gate_all not run. Native bodies unchanged. Lua, installed files, saves and OS clock untouched.

Continue with passing public native EOF policy scratch below: integrate existing policy provider and typed fastcall bridges, native setter state setup/restoration, isolated --eof-public-policy nine-read child with generated C: archive, two controls and two strict7-byte setter roots (expected45 selected). No new whole-project coverage claim. Recreation remains active until user stops.

---

## Public native EOF policy: scratch checkpoint

Nine actual cached C: EOF reads pass three policies: unset callback, installed callback and callback self-clear using the existing native public setter. Partial13/error0 and at/beyond EOF0/error38 retain user success, requested32-byte finished totals and original guard/tail bytes; callback counts are0,2,1 respectively and dirty flag staysfalse. Native dirty setter true/false transitions also pass. Both setter bodies freshly match retail7-byte roots/one relocation each through existing threaded_error_policy_oracle descriptors; six-provider object links genuine C++ without changing source. Native Close joins metadata/files/callbacks/events and restores prior callback/flag using native setters under lock. Allfour original/recreated worker/Close combinations pass twice. Ignored-null-callback and ignored-dirty-assignment controls fail intended assertions.

Receipt: `work/native-recreation/loading/text-bank/20261023/threaded-eof-public-policy/receipt.json`. The first compile used an intentionally incomplete manager view and failed; preserved attempt1-compile.json. Typed fastcall linker bridges now call real public methods without mixing manager/bank header views. Scratch only. Next permanent integration: add existing policy provider, use public setters for EOF state transitions/restoration, integrate isolated --eof-public-policy child with generated C: archive, add two controls and two strict roots (expected45). Native production bodies remain unchanged; no new whole-project coverage or failed-submission claim. Prior95-control checkpoint remains accepted while97-control peer gate runs. Lua excluded.

---

## EOF callback peer admission: permanent integration active

Integrated --eof-callback-peer uses the child-only generated C: archive environment already used by harddisk EOF. Focused integrated child passes allfour original/recreated worker/Close combinations twice. Permanent gate is running with expected97 controls,43 exact roots/four near contexts and peer_admission=1; safe active priority and false remove-success controls are included. Prior95-control checkpoint remains accepted until the complete gate finishes. Native source bodies unchanged; Lua excluded.

---

## Harddisk EOF reads: permanent checkpoint

Complete native gate passes512 loading combinations twice,95 fault controls,43 selected exact roots andfour near contexts. Three actual cached C: archive reads cover partial/at/beyond EOF under native logical harddisk device0. OS results are13/error0 and0/error38; native error callback receives an empty string once for each error38 while request status remains READING2. User completion remains true, requested32-byte finished totals remain, dirty-disc flag stays false, and independent partial payload/guards pass. Callback/dirty flag/forwarder restore under manager lock after native metadata/file/event joins. Child-only FABLE_NATIVE_EOF_HARDDISK_SOURCE is inherited by the four original/recreated worker/Close combinations, each run twice. Generated C: archive path/hash recorded and rechecked. Suppressed harddisk callback fails the intended assertion.

Receipt: `rebuild/build/native-text-eviction/00/9f/009fc170/20261023T204518036832Z/receipt.json`, SHA256 `ea472f572fa6837a23b4a7caef467400a253c6a7cf6e19ac84153a653d9ae9a0`. All794 pinned inputs unchanged. gate_all updated only after verified95-control/harddisk_eof=3 marker; full gate_all not run. Native bodies unchanged. Lua, installed files, saves and OS clock untouched.

Continue with the passing actual EOF callback peer-admission scratch below: isolated child mode, generated C: source/hash, safe active-remove-success/priority controls. Do not stop at this checkpoint; recreation remains active until user stops.

---

## Actual EOF callback peer admission: scratch checkpoint

A real cached harddisk EOF callback now queries its live status2/UID and pending totals1/32, refuses active reprioritization/removal, and submits one peer32-byte read through native AddObject; totals become2/64 inside that callback. Native processing completes both user callbacks true; peer auto-removes and original request remains finished1/32 until native Close. OS callbacks record EOF0/error38 then peer32/error0; all original bytes are untouched and peer payload/guards match independent generated archive bytes. Allfour original/recreated worker/Close combinations pass twice, with two safe faults: active-priority change and false remove-success (without deletion/UAF). Events/files/callback/buffer ownership joins, flags/callbacks restore under manager lock.

Receipt: `work/native-recreation/loading/text-bank/20261023/threaded-eof-callback-peer/receipt.json`. Scratch only; harddisk permanent95-control gate remains running. Next integrate isolated --eof-callback-peer child with generated C: source/hash. Native providers unchanged. No arbitrary concurrency, failed-submission recovery or physical-media claim.

---

## Harddisk EOF: permanent integration active

The current --eof-harddisk child uses FABLE_NATIVE_EOF_HARDDISK_SOURCE supplied only to its subprocess/original oracle. Gate generates an exclusive C: temporary archive, checks logical device0 and pins/rechecks archive bytes. Shared EOF fixture now also checks native harddisk callback invocation while request status remains READING2; DVD EOF retains its dirty flag policy and never calls that leaf. Callback, dirty flag and forwarding slot restore under native manager lock after ownership joins. Focused integrated fixture passes allfour original/recreated worker/Close combinations twice before the complete gate launch. Full gate is active, expected95 controls/three harddisk EOF cases/43 exact/four near. Prior94-control checkpoint remains accepted until it completes. Native bodies unchanged; Lua excluded.

---

## Cached EOF reads: permanent checkpoint

Complete native gate passes512 loading combinations twice,94 fault controls,43 exact roots andfour near contexts. Three actual cached reads on an exclusively generated small archive cover crossing/at/beyond EOF. Captured OS completion is13 bytes/error0 for the partial read and0/error38 at/beyond EOF. Retail still publishes user success and retains requested32-byte finished statistics. Logical DVD dirty-disc flag sets only on error; payload, untouched tail and guards pass. Native Close removes metadata; callbacks/events/files join, and dirty flag restores under the manager lock. Allfour original/recreated worker/Close combinations run twice. Both EOF controls fail explicit assertions.

Receipt: `rebuild/build/native-text-eviction/00/9f/009fc170/20261023T203856939614Z/receipt.json`, SHA256 `5cbb91dbed7138265d0f162de9032e28cc4928c54c9c73e72c702deed5aeb227`. All794 pinned inputs unchanged. gate_all now recognizes the verified94-control/eof=3 marker; full gate_all was not run. Native production bodies unchanged. Lua, installed files, saves and OS clock untouched.

Continue with the already-passing harddisk EOF scratch below: integrate a child-only generated C: archive path/hash and typed error callback boundary, preserving ordinary invocation and explicit state restoration. Recreation remains active until user stops.

---

## Harddisk EOF scratch checkpoint

The next work-only case passes three actual cached reads using an exclusively generated C: temporary archive (native logical device0), all four original/recreated worker/Close combinations twice. Partial EOF returns13/error0; at/beyond EOF return0/error38. A typed harddisk error leaf receives the native empty string exactly once for each error38 while the request is still READING2. Native user completion stays true and retains requested32-byte finished totals; dirty-disc flag stays false. Partial bytes match the independent archive window, all guards and unread tail bytes remain intact. Native Close removes metadata; callbacks/events/file ownership join and callback/dirty flag restore. Suppressing the harddisk error callback fails the explicit assertion.

Receipt: `work/native-recreation/loading/text-bank/20261023/threaded-eof-harddisk/receipt.json`. Generated C: temporary archive path/hash are recorded; no installed data was used. This remains scratch, separate from the ongoing94-control permanent gate. Next integrate through a child-only generated-archive environment path, independently verify its hash and preserve ordinary invocation. No real failed-submission recovery or physical-media claim.

---

## Cached EOF read continuation: integration active

User resumed original-compatible native C++ recreation until asked to stop; Lua remains excluded. Work-only cached EOF probe passes all four original/recreated worker/Close combinations twice. On an exclusively generated small archive, a32-byte request crossing EOF transfers13 bytes/error0; requests at/beyond EOF transfer0/error38. OS error/count are captured before native completion. Native user callbacks still receive true, finished statistics retain requested32 bytes, and logical DVD dirty flag sets on error38. Guards/untransferred buffer bytes remain unchanged. Native Close removes retained UIDs; callbacks/events/files join and dirty flag restores under the manager lock. Ignored dirty flag and false user-completion controls fail explicit assertions.

Scratch: `work/native-recreation/loading/text-bank/20261023/threaded-eof/receipt.json`. Attempt1 preserves the initial case before dirty-flag assertions; initial relative-base include failure is superseded by absolute path resolution. Permanent --eof integration is active, expecting94 controls/three EOF cases/43 exact/four near. The first permanent compile attempt was deliberately interrupted after finding the aggregate control-count list still omitted the two EOF controls; corrected before the replacement run. The next full attempt reached all existing focused children but rejected --eof at line500 because an overly broad edit also added it to the descriptor dispatch branch; fixed to its own branch before the new run. Failed build20261023T203808671821Z is preserved. No native production-body change. No failed-submission recovery, physical-media or latency claim. Prior92-control checkpoint remains accepted until the full focused gate completes. Full gate_all has not run.

---

## Descriptor-only arithmetic: permanent checkpoint

Complete native gate passes512 loading combinations twice,92 controls,43 selected exact roots andfour near contexts. Six descriptor size pairs cover zero, signed boundaries andUINT_MAX. Real global-map object counts retain zero-sized requests; requested-byte totals preserve32-bit wrap/signed return bits. A held begin callback refuses; native Close cancels both requests false once, removes both IDs and leaves small caller buffers unchanged. A typed read-slot guard rejects before any Windows read call and records zero submissions. Each pair passes four original/recreated worker/Close combinations twice. No huge allocation/read/exhaustion claim. Saturating totals and dropping zero-sized objects both fail explicit accounting assertions.

All earlier real read phases/close holds, query/statistics snapshots, callback-peer writes, six device priorities and36 exact archive files/2,304 resource decodes/evictions remain passing.

Receipt: `rebuild/build/native-text-eviction/00/9f/009fc170/20261023T201937672354Z/receipt.json`, SHA256 `8c4a8ca0865a53853838184d03718ace80d00088e7165d9b0ae5bdb7049e9227`. Final check finds no drift among794 pinned inputs. gate_all expects92 controls/descriptor_pairs=6/43 exact/four near; full gate_all was not run. Lua, installed files, saves and OS clock remain untouched.

Continue with a bounded actual cached EOF/partial-read case on an exclusively generated small archive. Capture OS completion error/transfer values before invoking the native callback, then observe original user-completion signal, logical-device dirty flag and retained requested-size totals. Close all metadata and restore flags; do not normalize the original error policy or claim failed-submission recovery.

---

## Descriptor-only32-bit arithmetic: scratch closed, integration active

Six real registered-descriptor size pairs pass, including0/0,1/UINT_MAX,INT_MAX/1,0x80000000/0x80000000,UINT_MAX/2 andUINT_MAX/UINT_MAX. Native pending count includes zero-sized descriptors and pending byte sum preserves32-bit wrap/signed return bits. Main holds the real manager lock during admission; the first begin callback then refuses, Close cancels both false exactly once and removes both IDs. Caller64-byte buffers remain unchanged. A typed read-slot guard rejects before any Windows read API and records zero submissions, so no huge allocation/read/exhaustion behavior is claimed.

Each pair passes allfour original/recreated worker/Close combinations twice with the guarded read forwarder. Clamping bytes and skipping zero-sized objects fail explicit statistic assertions. Scratch receipt: `work/native-recreation/loading/text-bank/20261023/threaded-descriptor-arithmetic/receipt.json`, SHA256 `a71af685f63d44087446478bbe616b278ae635a5e8205a2aaba26cfd3481fd0d`.

Permanent --descriptor-meta integration is active with child-only FABLE_NATIVE_DESCRIPTOR_PAIR, expected92 controls/43 exact/four near and six descriptor cases. The preceding90-control checkpoint remains accepted until completion. Oracle boundary text now distinguishes close-at-read and descriptor-only cases. Full gate_all has not run; Lua, installed files, saves and OS clock remain untouched.

---

## Close during each real read phase: permanent checkpoint

Full native gate passes512 loading combinations twice,90 controls,43 selected exact roots andfour near contexts. Native Close starts while one real cached read is held at begin-locked1, submitted2or native-completed3. In each case Close is observed blocked with manager block count1 and file still open; the existing read advances through its native/user completion before Close removes the UID and closes the handle. Beginning processing already in progress can submit after Close blocks new queue processing; original behavior is preserved. Three phase cases each pass four original/recreated worker/Close combinations twice with independent32-byte payload/guards and joined callback/close/file cleanup. Public Close47B/three relocations is freshly exact in strict checking.

The premature-open-flag control fails its exact intended assertion. Its initial scratch attempt selected an older helper's similar line despite correct fault failure; corrected selector in attempt2 is preserved separately. Earlier all6read states, queries/statistics, callback-peer writes, device priorities and36 archive outputs/2,304 decodes/evictions remain passing. No early object-deletion/UAF mutation, arbitrary concurrency, OS latency or physical cancellation claim.

Receipt: `rebuild/build/native-text-eviction/00/9f/009fc170/20261023T201214336548Z/receipt.json`, SHA256 `7cad858786284ab0cb1117c0ffc429e51a943c9fd99ccba7057aefc9be6cb3d5`. No drift among794 pinned inputs. gate_all expects90 controls/close_read=3/43 exact/four near; full gate_all was not run. Lua, installed files, saves and OS clock remain untouched.

Next prepared scratch: `work/native-recreation/loading/text-bank/20261023/threaded-descriptor-arithmetic/run.py`. Test real registered request descriptors at zero/signed-boundary/UINT_MAX sizes, native32-bit byte-sum wrap and zero-size count policy. Begin callback refuses, Close cancels both requests, and a typed read-slot guard prevents any OS read before Windows is called. No huge buffer, allocation, read or exhaustion claim. Two safe accounting faults.

---

## All real read phases: permanent checkpoint

Complete native gate passes512 loading combinations twice,89 controls,42 selected exact roots andfour near contexts. One actual cached read is observed at native queued0, callback-locked1, submitted2, native-completed3, retained-finished4 and removed phases. Begin, post-OS-submission and post-native-completion holds are explicit nonalertable fixture boundaries. Original worker retains the read import forwarder in all four original/recreated worker/Close combinations twice. Native exists/finished and allfour statistics getters agree under lock;32 independent source bytes and16-byte side guards match. User completion happens once, metadata remains retained until Close, and all file/event/callback/buffer ownership joins cleanly.

The existing native read-completion callback102B/seven relocations now participates in strict matching. Wrong begin status and premature finished status are detected at explicit phase assertions. Earlier writes, callback-peer, retained queries/statistics, six device priorities and36 archive/2,304 decode/eviction comparisons remain passing. No arbitrary concurrency, OS completion latency or physical-media claim.

Receipt: `rebuild/build/native-text-eviction/00/9f/009fc170/20261023T200403577622Z/receipt.json`, SHA256 `17ce7a2837736ad245adfad904dd606886baa07fe9b77b912130858ddbb95c3f`. Final check finds no drift among794 pinned inputs. gate_all expects89 controls/read_phases=6/42 exact/four near; full gate_all was not run. Lua, installed files, saves and OS clock remain untouched.

Next prepared scratch: `work/native-recreation/loading/text-bank/20261023/threaded-close-read-phases/run.py`. Begin native Close during the phase1,2or3 hold, observe manager block count1 and open file while Close waits, advance the existing read through publication, then join close and verify removed UID and independent bytes. Existing phase1 processing may advance into OS submission after Close blocks new queue processing; preserve original behavior. One safe premature-open-flag fault; do not introduce early object deletion/UAF controls.

---

## Native live statistics: permanent checkpoint

Full native gate passes512 original/recreated loading combinations twice,87 controls,41 selected exact roots andfour near contexts. Four existing exact statistics getters (pending/finished objects117B and descriptor bytes118B) now run on the real global UID map under a common outer native lock. Before close, three unfinished requests total12KiB. During the false completion callback, the cancellation descriptor remains in the UID map until callback return and the new peer admission restores the snapshot to3/12KiB. After peer completion only the retained finished request contributes1/4KiB; Close removes it and alltotals returnzero. Counters follow current map status and descriptor sizes. Both statistics faults fail explicit assertions.

All prior global query states, original/recreated worker/resize/Close combinations, reentrant peer writes,12KiB file bytes, device priorities and36 archive output/2,304 decode/eviction checks pass. Sort controls consistently rebuild all26 affected header consumers. Source bodies of the four getters remain their already-recovered exact C++; typed fastcall bridges avoid combining the intentionally separate manager/bank header views.

Receipt: `rebuild/build/native-text-eviction/00/9f/009fc170/20261023T195302245285Z/receipt.json`, SHA256 `d1b34e8763c88542dc2fcf22751ca23b7ab606dd07e818873e0baac37cdebad5`. No drift among794 pinned inputs. gate_all expects87 controls/statistics=4/41 exact/four near; full gate_all was not run. Lua, installed files, saves and OS clock remain untouched.

Next scratch boundary: one actual cached read observed through queued0, begin-callback-locked1, submitted2, native completion3, retained-finished4 and removed states. Use explicit nonalertable begin/API/completion barriers, query/statistic snapshots and independent32-byte payload/guard checks; keep all caller buffers, events and file ownership alive through native joins. Original worker must retain the explicit read forwarding boundary in this oracle; no physical latency or arbitrary concurrency claim.

---

## Real global UID query states: permanent checkpoint

Complete native gate passes512 original/recreated loading combinations twice,85 controls,37 selected exact roots andfour near contexts. The real held write and two queued requests report present/unfinished. File receivers intentionally query the global UID map across the two writers. One peer request uses QueueWhenFinished=true: after native completion/join it remains present/finished, then native Close removes it without another callback. Allfour retired IDs and never-used0 report absent/finished. The two existing exact finished-query methods are now live strict roots (manager95B/file10B), completing allfive exact query bodies in this composition. Existing6,366-case query model still preserves mapped-null fault and API snapshot semantics; no real mapped-null test or new null guard.

Both live-query faults fail explicit assertions. Original/recreated worker/Close combinations, callback-admitted peer work,12KiB independent file bytes, four archive alignments/36 exact files/2,304 decodes and evictions, six device priorities and all prior controls remain passing. Final owned file/UID/thread/buffer cleanup passes.

Receipt: `rebuild/build/native-text-eviction/00/9f/009fc170/20261023T194308805473Z/receipt.json`, SHA256 `2de386093113f1c9e106786c5a16922791c6e45bb2ccfb1319e013e11cfd52fc`. No drift among790 pinned inputs. gate_all expects85 controls/query_states=4/37 exact/four near; full gate_all was not run. Lua, installed files, saves and OS clock remain untouched.

Next scratch: `work/native-recreation/loading/text-bank/20261023/threaded-live-statistics/run.py`, adding the four already-recovered exact pending/finished object/byte getters through typed ABI bridges. Snapshot under an outer native manager lock before Close, during callback admission/cancellation, after retained completion and after removal. Preserve current-map/status/descriptor-byte accounting, including the cancelled descriptor's lifetime before callback return. Two targeted statistics controls. A separate simple Close cursor-operand swap did not change its eight known scheduling bytes; preserved under work/native-recreation/loading/text-bank/20261023/close-cursor-compare, with no canonical Close change.

---

## Exact const UID-find: complete permanent checkpoint

Complete native gate passes512 loading-root combinations twice,83 controls,35 selected exact roots andfour near contexts. Const UID-find now freshly matches original71B with zero relocations; allfive query bodies remain MATCH in the6,366-case standalone gate. The existing const helper declaration and nonconst inline recipe were reused without header/layout changes. Reduced const-iterator view qualification is retained. Earlier device, real write/resize, Close, callback-peer and all36 archive output/2,304 decode/eviction comparisons remain passing.

Receipt: `rebuild/build/native-text-eviction/00/9f/009fc170/20261023T193027819689Z/receipt.json`, SHA256 `08acf06b6ff4f9d058945feda2aaddcd615e1682d3cb52c50e39cab22d62fb26`. All790 pinned inputs pass final no-drift. gate_all expects35 exact/four near; full gate_all was not run. Native source changes in this resumed work are genuine C++ return forms in ResizeFile/ObjectExists and the const lookup helper recipe; no assembly/native byte blobs were introduced. Lua, installed files, saves and OS clock remain untouched.

Continue with `work/native-recreation/loading/text-bank/20261023/threaded-live-query-states/run.py`: real reading/pending queries, one retained finished peer object and removal on native Close, missing/retired IDs, and global receiver-independent semantics. Two targeted query faults. Expected follow-up integration exercises the two already-exact finished-query bodies as live strict roots; preserve all callback/file/buffer joins and original null policy.

---

## Const UID-find integration running; live query-state scratch prepared

Canonical query gate passes allfive bodies as MATCH and all6,366 cases, omissions and const contracts. Const-find source SHA256 `2952ae99d0c9bb40a0058980bcb4ef0ce7aa210fbfd59344428ec6488f58d0d0`; query report SHA256 `3748abc862678631b2b1f5a43f3f4615a5946ba5b9be841a9227697246e42673`. Full gate build20261023T193027819689Z has passed all focused device/write/Close/callback-peer/archive checks and is running the512-combination matrix. Expected35 selected exact roots/four near contexts,83 controls. Do not revise completed34-exact acceptance until it closes.

Prepared next scratch runner: `work/native-recreation/loading/text-bank/20261023/threaded-live-query-states/run.py`. It checks existing global exists/finished APIs on the real active write and two queued requests, keeps one peer object with QueueWhenFinished=true, then observes present/finished before Close and absent/finished after Close. Checks deliberately cross file receivers to preserve their global-UID contract. Two targeted query faults are planned. Actual mapped-null entries remain model-only; no new null guard or policy normalization.

No header/layout, Lua, installed-file, save or OS-clock changes. Full gate_all has not run.

---

## Const UID-find exact C++ improvement: canonical validation active

Const UID find now uses the already-declared const _M_find overload with the existing nonconst inline node-search recipe. No header/layout change. Source `rebuild/src/compiled/00/99/STLThreadedUIDTree_ConstFind_00990320.cpp` compiles byte-exact71B with zero relocations under ordinary and /Gy flags. Allfive query bodies are now MATCH; canonical6,366 original/model cases, five missing-provider checks and const compile contracts pass. Reduced const-iterator view and original missing/null/snapshot semantics remain qualified as before.

Canonical query report SHA256 `3748abc862678631b2b1f5a43f3f4615a5946ba5b9be841a9227697246e42673`. Starting fresh complete native integration with35 selected exact roots/four near contexts and83 controls. The preceding34-exact checkpoint remains last completed until this closes. Earlier16 failed source-form experiments are preserved; only the genuine helper-recipe candidate was promoted. No native byte blob, assembly patch, Lua, installed-file, save or OS-clock changes. Full gate_all has not run.

Next scratch: exercise native global exists/finished queries on active and queued work, keep one peer request after completion, observe retained-finished state, and close it to prove removal. File-wrapper queries intentionally use the global UID map regardless of receiver; retain that original contract.

---

## Exact ObjectExists: complete permanent checkpoint

Complete native loading/device/write/Close/archive gate passes512 combinations twice,83 controls,34 selected exact roots andfour near contexts. Manager/file ObjectExists freshly match original61B/70B, with finished-query95B/10B bodies unchanged. Standalone6,366-case query gate and all original/recreated write/resize/Close/callback-peer/archive compositions pass. All36 archive outputs match hashes and2,304 resource decodes/evictions complete. Original missing/null/lock-snapshot semantics remain preserved.

Receipt: `rebuild/build/native-text-eviction/00/9f/009fc170/20261023T191756085639Z/receipt.json`, SHA256 `8840d19197c2a0401f794bf01be7eb0104d235e3ad6a3c3f876bb1e202149c3d`. Final check finds no drift among790 pinned inputs. gate_all expects34 selected exact roots/four near contexts; full gate_all was not run.

A further const UID-find candidate is now proven in `work/native-recreation/loading/text-bank/20261023/const-find-return-shape/inline-lookup/receipt.json`: use the already-declared const _M_find overload with the same genuine inline C++ recipe already used by nonconst lookup. No header/layout change. Its71B body is byte-exact under ordinary and /Gy flags; allfive query bodies and6,366 guarded original/model cases pass. This follows the preserved unsuccessful16 expression/declaration variants, not a native byte patch. Promote only the const-find source, require MATCH in its query audit and add that live helper to strict selection (expected35 exact/four near); retain the reduced const-iterator view qualification and rerun query/full gates.

Lua, installed files, saves and OS clock remain untouched.

---

## ObjectExists ordinary C++ exact improvement: canonical validation active

One return-form change in `rebuild/src/compiled/00/98/CThreadedFileManager_Queries_0098e590.cpp` makes manager ObjectExists61B and its inlined global file wrapper70B byte-exact. The existing finished-query95B/10B exact bodies remain unchanged. Scratch checks under ordinary and /Gy flags match all four bodies; canonical query gate passes6,366 original/model cases, five missing-provider checks and positive/negative const compile contracts. Missing UID, lock-entry/exit mutation snapshots, ignored file receiver and intentional mapped-null finished-query fault policy remain native behavior. The const UID-find71B helper remains its existing near match and the const-iterator type remains a reduced local view, not original template-name recovery.

Canonical query report: `rebuild/compile-gate/native-threaded-queries.json`, SHA256 `0d0bff531958d507f1dbf2dc158623375b353e45bd69b57bbbffee777c351d8e`. Full native integration is running with expected34 selected exact roots/four near contexts and unchanged83 controls. The preceding32-exact checkpoint remains the last complete integration until it closes. A separate16-variant genuine-C++ const-find declaration/return experiment found no exact candidate; all attempts are preserved under `work/native-recreation/loading/text-bank/20261023/const-find-return-shape`, and no const-find source changed.

Full gate_all has not run. Lua, installed files, saves and OS-clock state remain untouched.

---

## Exact ResizeFile: complete permanent checkpoint

The genuine C++ if/return change now passes full loading/device/write/Close/archive integration:512 loading-root combinations twice,83 controls,32 selected exact roots andfour near contexts. Existing original/recreated worker/resize and worker/Close combinations, callback-submitted peer work, all36 exact archive outputs/2,304 decodes/evictions and all6device priorities remain passing. ResizeFile is freshly byte-exact58B/23instructions/three IAT imports in the strict root gate. Standalone480 model,20 real-Windows and720 open/resize lifetime paths/4,320 calls also pass. Only its boolean return source form changed; original ignored errors and INVALID_FILE_SIZE equality remain unchanged.

Receipt: `rebuild/build/native-text-eviction/00/9f/009fc170/20261023T190510867741Z/receipt.json`, SHA256 `3ff39f898c601805ae41e4ee8bcd67b3b60d2ddcf45e34b928cfd33bc5e06be3`. All790 pinned inputs pass the final no-drift check. gate_all expects the observed32-exact/four-near marker; full gate_all was not run. Lua, installed files, saves and OS-clock state remain outside this work.

Next proven candidate: ordinary if/return in CThreadedFileManager::ObjectExists removes the same four boolean-result bytes in both manager and inlined file-wrapper bodies, matching original61B/70B. The two already-exact finished-query bodies remain95B/10B exact under ordinary and /Gy flags. Scratch `work/native-recreation/loading/text-bank/20261023/query-return-shape/receipt.json` passes all6,366 original/model cases, five omitted-provider checks and guarded lock-entry/exit mutations; const UID-find remains its pre-existing near helper. Promote the one source return change, require MATCH for those two query roots, rerun canonical query gate and full integration. Expected selected exact count34, near count unchanged4.

---

## ResizeFile ordinary C++ exact improvement: canonical validation active

Changed only the final boolean return form in `rebuild/src/compiled/00/98/CThreadedFile_ResizeFile_0098e090.cpp` to an ordinary if/return true/return false sequence. Its compiled body now matches all58 original bytes and three independently checked API imports under both ordinary and /Gy flags, removing the prior four extra return-result bytes. Source SHA256 `8764fd9f772c4725dcfd765d4dcbbcbb9d38b85b358692eb790f675b784601d2`. API order, live handle reloads, ignored seek/end failures, final size storage and INVALID_FILE_SIZE equality remain original behavior.

Canonical standalone480-case model gate,20-case real-Windows gate and720-case open/resize lifetime gate (4,320 resize calls) all pass, including existing omissions and fault controls. Reports: `rebuild/compile-gate/native-threaded-file-resize.json` SHA256 `88167f8b8ee96b08883217a7e37695952b1654f8629afd6e18fbff2e59331c89`; Windows receipt `D:\Documents\FableTLC\rebuild\build\native-file-resize\00\98\0098e090\20261023T190429048780Z/receipt.json`; lifetime receipt `D:\Documents\FableTLC\rebuild\build\native-file-open-resize-lifetime\00\98\0098e000\20261023T190429061551Z/receipt.json`. The lifetime gate now records31 exact helpers/three near helpers plus its pre-existing functional-only body. Auditors retain explicitly checked legacy62-byte near support, while the canonical standalone marker now requires MATCH.

Full loading/device/write/Close/archive integration is running from a fresh frozen build. Expected selection becomes32 exact/four near with unchanged83 controls. The last completed full checkpoint below remains the31-exact/five-near archive checkpoint until this closes. gate_all full suite has not run. This is a genuine C++ source change, not a native byte blob or assembly patch. Lua, installed files, saves and OS clock remain untouched.

---

## Native-written archive round trip: permanent checkpoint

Full permanent gate passes512 original/recreated loading combinations twice,83 controls,31 exact roots andfive near contexts. Earlier device/write/close/callback-peer coverage remains passing. The new isolated --written-archive child writes synthetic BIGB bytes through actual aligned native worker writes, trims padding with public ResizeFile, independently checks every output byte, reopens the result through native bank APIs, decodes all64 text/group resources through public getters, evicts all64 and closes bank/file/allocator ownership cleanly. At each of four archive alignments, the recreated positive plus four original/recreated worker/resize combinations twice produce nine exact files. All36 files match independent source hashes;2,304 decodes and2,304 evictions pass. Source paths are supplied only to relevant child environments, including oracle subprocesses.

Short aligned write and omitted-truncation faults fail explicit byte/resize assertions. The archive is generated test data; this does not recover a bank serializer or claim installed-file writes, saves or physical-media behavior.

Receipt: `rebuild/build/native-text-eviction/00/9f/009fc170/20261023T185507501760Z/receipt.json`, SHA256 `8c2887bdd3f035347d1e4315e1611636937bb6def0c4ec71d07ac44db53406a3`. All790 pinned inputs pass the final no-drift check. gate_all expects the verified83-control/written_archive marker; full gate_all was not run. Native source bodies are unchanged at this checkpoint; Lua remains outside the work.

Next: promote a separately proven genuine-C++ ResizeFile return-shape improvement. `work/native-recreation/loading/text-bank/20261023/resize-return-shape/validate-if-return/receipt.json` proves an ordinary if/return form emits the exact original58bytes/three independently checked imports under both ordinary and /Gy flags.480 original/model cases per build,20 real-Windows cases, two behavioral faults and one missing-provider check pass. Waited for this gate's pinned-input check before changing canonical source. Re-run standalone resize, real-I/O, open/resize lifetime and complete loading gate after promotion; expected selected roots become32 exact/four near, with unchanged83 controls.

---

## Close callback submits peer work: permanent checkpoint

Full permanent gate passes512 original/recreated loading combinations twice,81 controls,31 exact roots andfive near contexts. Device modes retain240 focused windows/1,024 oracle children/61,440 windows. All four original/recreated worker/resize write combinations pass twice. All four worker/CloseAllStreamsForFile combinations also pass twice with12KiB across the closing writer and two surviving peer requests.

The closing writer's false completion callback creates and joins a bounded independent native critical-section probe, verifying that it can acquire the manager lock while the callback runs. The callback then submits another real peer write through native AddObject at block count1. Both initial and callback-submitted peer work survives Close; explicit native KeepUpdating wakes it, completions and UID joins finish before file/buffer cleanup, and independent CRT bytes match both files. A native no-unlock-before-callback fault fails the lock-probe assertion deterministically. Typed callbacks/probe thread remain controlled; no arbitrary-callback, automatic-wake, latency or physical cancellation claim.

Receipt: `rebuild/build/native-text-eviction/00/9f/009fc170/20261023T184519117401Z/receipt.json`, SHA256 `b6e05d73abb5f241df4ba0129561c8ef4ca4b1a040bf26d3826873df2c38642f`. All790 pinned inputs pass the final no-drift check. gate_all expects the verified81-control/callback_peer marker; full gate_all was not run. Existing native C++ source bodies remain unchanged. Only build-owned files are written; no Lua, installed files, saves or OS-clock changes.

Next scratch: `work/native-recreation/loading/text-bank/20261023/threaded-written-archive/run.py`. Write a synthetic BIGB text/group archive through actual native worker writes, trim padded I/O through public ResizeFile, compare every file byte, then reopen through native bank APIs, decode all64 resources and evict/close with final allocator ownership cleanup. Four original/recreated worker/resize combinations run twice for each of four archive alignments. Source archives and outputs remain exclusively build-owned.

---

## Close preserves peer writer: permanent checkpoint

Complete permanent gate passes512 original/recreated loading combinations twice,80 controls,31 exact roots andfive near contexts. Device coverage remains six named priorities/two slice modes/four alignments (240 focused windows,1,024 oracle device runs/61,440 windows). All four original/recreated worker/resize combinations pass twice with8KiB actual writes. All four worker/CloseAllStreamsForFile combinations pass twice with8KiB across the closing and surviving peer writers. Close waits for its held active write, cancels only its queued write, preserves the peer's handle and queued request, then an explicit native KeepUpdating wake/completion/UID join completes the peer. Independent CRT bytes and final ownership cleanup pass. The all-files filter fault is rejected at the peer's expected-success callback.

Receipt: `rebuild/build/native-text-eviction/00/9f/009fc170/20261023T183729773873Z/receipt.json`, SHA256 `62bf648004750215408c5ced9c87f0400efcdb755775b4b3e03c9d47ca838988`. All790 pinned inputs remain unchanged. gate_all expects the verified80-control/write_peer marker; full gate_all was not run. Existing native source bodies remain unchanged. Only build-owned files are written; no Lua, installed files, saves or OS-clock changes.

Next scratch: `work/native-recreation/loading/text-bank/20261023/threaded-close-callback-peer/run.py`, where the closing file's false completion callback submits a further native peer write. A bounded second-thread native lock probe checks that callback execution occurs outside the manager lock before submission. Keep callback behavior explicitly controlled and all file/buffer lifetimes joined.

---

## Close preserves peer writer: scratch proof, permanent integration active

Scratch passes four positive repetitions and all four original/recreated worker/CloseAllStreamsForFile combinations twice. Closing writerA waits for its submitted write and cancels only its queued request. WriterB retains its live handle and queued work; an explicit native KeepUpdating wake, completion event and native UID join finish it. Independent CRT reads match4KiB in each owned file (8KiB total). Source filter fault `first->second->PFile==file` -> true cancels the peer and fails the typed peer-success assertion. Receipt: `work/native-recreation/loading/text-bank/20261023/threaded-close-peer-write/receipt.json`, SHA256 `fc9a5cf5801f93ea97820c7aa0278b942cf88c33a627a6c7da50dde79e9d434a`. Fresh final-link native-body proof also passes in this scratch build.

The permanent --pending-write child now contains the peer case. Focused positives, four original/recreated close/worker combinations twice and all three Close controls pass; full loading/device matrix is active in build20261023T183729773873Z. Permanent acceptance remains the79-control checkpoint until completion. Full gate_all has not run. No native source-body or Lua changes; only build-owned files are written.

Next potential scratch boundary: the closing file's false completion callback reentrantly submits a further write to the surviving peer, preserving callback-unlocked native submission and all peer-buffer/file lifetimes. Callback behavior stays explicitly controlled.

---

## Close during submitted write: permanent checkpoint

Permanent gate passes512 original/recreated loading combinations twice,79 fault controls,31 exact roots andfive near contexts. Device modes retain240 focused windows across six priorities/four alignments and1,024 device children/61,440 payload windows. Four original/recreated worker/ResizeFile combinations pass twice with actual8KiB writes. Four original/recreated worker/CloseAllStreamsForFile combinations also pass twice while one successfully submitted write is held before completion publication and a second remains queued. Close is observed blocked at manager block count1; after release, active completion succeeds, queued completion reportsfalse without submission, both UIDs disappear, block count returns0, and independent CRT bytes contain only the first4KiB. All owned callback/thread/file/buffer cleanup passes.

The test worker changes only ReadFileEx and WriteFileEx import-slot names. Fresh ordinary-worker comparison proves every instruction byte and all other relocations unchanged; full final-link native-body verification passes. New query and const UID-find providers are existing recovered code, with no new global coverage claim. Both new Close controls fail explicit intended assertions. Sort-order controls rebuild all22 now-affected header consumers consistently. Native C++ source bodies remain unchanged.

Receipt: `rebuild/build/native-text-eviction/00/9f/009fc170/20261023T182913617457Z/receipt.json`, SHA256 `d6a0487816f17a3c26b209a586666ef2f5ea0d8cdd514d7750cec463b6ee7441`. Final check found no drift among790 pinned inputs. gate_all now expects the verified79-control/pending-write/31-exact/5-near marker; full gate_all was not run. Only build-owned files are written. No Lua, installed files, saves or OS-clock changes.

Continue with scratch peer-file isolation: closing one writer with active/queued work must preserve another file's queued write and live handle. The survivor receives an explicit native KeepUpdating wake, then completion and native join; no automatic wake, latency or physical cancellation claim. Prepared runner: `work/native-recreation/loading/text-bank/20261023/threaded-close-peer-write/run.py`.

---

## Close during submitted native write: scratch proof and integration

Work-only Close now waits while one real aligned WriteFileEx submission is held outside alertable waits and one further write remains queued. The manager lock exposes block count1 with file/owners/callbacks still live; the Close thread has not returned. Releasing publication completes the active write, sends a false completion for the queued write, removes both native UIDs, restores block count0 and closes the file. Independent CRT bytes match only the first4KiB. Buffers, callback events, close thread and file are released after native joins; string counts restore. Four positive repetitions pass.

All four original/recreated worker/CloseAllStreamsForFile combinations pass twice. Original worker786B/27 adjusted sites retains the explicit WriteFileEx forwarding slot; original Close840B/19 checked sites retains its exact original eight-byte scheduling difference from the near C++ body. Success-on-cancel and omitted close-open-flag controls fail explicit intended assertions. Receipt: `work/native-recreation/loading/text-bank/20261023/threaded-close-pending-write-attempt3/verification-receipt.json`, SHA256 `775316a3652eb76f292f8cfefd218d4e0fa318b381f2dd97891c4d26ecfc3c9c`. Only exclusively build-owned files are written.

Preserved initial scratch link failures: attempt1 lacked the existing ObjectExists query provider; attempt2 added it but lacked its existing const UID-find provider. Attempt3 adds both native providers and passes. No native source-body correction was required. This is lifetime/ownership behavior, not a physical disk cancellation or latency claim.

Permanent integration is running with isolated --pending-write, two optional read/write forwarding slots, four original/recreated close/worker combinations twice and two new controls. Normal loading and existing write child behavior are unchanged. Source-level worker proof compares every instruction byte and all other relocations against a freshly compiled ordinary worker. The accepted permanent report remains the77-control/31-exact/4-near checkpoint below until full integration closes. Full gate_all has not run; Lua remains excluded.

---

## Native worker writes and public resize: permanent checkpoint

The complete permanent gate passes 512 original/recreated loading combinations twice, 77 fault controls, 31 exact roots and four near contexts. Both device modes still pass at all six named priorities: 240 focused windows, plus 1,024 device-child runs/61,440 windows in the resource-list/loading/failure-handler matrix. All four original/recreated worker/ResizeFile write combinations pass twice, independently checking 8,192 written bytes per run. The write child checks completion without read-begin callbacks, native object joins, cached length retention, expansion to12,288/truncation to4,096, overwrite tozero, failed open beneath a regular owned file and string ownership cleanup. Typed callbacks remain explicit boundaries. Only build-owned files are written.

Existing OpenForWrite (143bytes) and FinishAndGetFileHandle (25bytes) now participate in exact-root checking. ResizeFile remains its existing62-byte/58-byte near match, not an exact promotion. The final-link verifier resolves the live private write counter through paired references, writable zero storage and distinction from the read counter, then verifies the entire native body/relocation set. The two independent link-only verifier controls remain separately qualified scratch evidence. Native C++ source bodies remain unchanged.

Receipt: `rebuild/build/native-text-eviction/00/9f/009fc170/20261023T181657442144Z/receipt.json`, SHA256 `3bd88b6153a2e7a7b65983868882a33b4ebfe946d35b07fe538721b5ef806129`. All 787 pinned inputs passed the final no-drift check. `gate_all` expects the observed77-control/31-exact/4-near/write marker; full gate_all was not run. No installed files, saves, OS clock or Lua changes.

Continue with scratch Close during one successfully submitted, publication-held write plus one queued write. Observe native Close blocked under its manager lock, then completion of the submitted write and false completion of the queued write, independent file bytes and complete owned thread/buffer/file cleanup. Prepared runner is `work/native-recreation/loading/text-bank/20261023/threaded-close-pending-write/run.py`.

---

## Permanent write integration: focused checks closed, full gate running

Permanent write-child positives, all four original/recreated worker/resize combinations twice, and all three write controls pass in the fresh integration build. Exact checking passes 31 roots (existing OpenForWrite and FinishAndGetFileHandle now included); ResizeFile retains its separately audited near grade. The existing final-link verifier needed explicit resolution of the live private write sort counter, analogous to its read-counter support. Both references at write-root offsets107/117 must agree on distinct, writable, non-executable, in-range, zero-initialized storage. Full native COFF-to-linked-byte/relocation verification still follows. A separate link-only verifier test accepts the untouched executable and rejects both an aliased read counter and a nonzero initialized write counter: `work/native-recreation/loading/text-bank/20261023/write-counter-verifier/receipt.json`, SHA256 `85ca84a5311c228af2b762b4d64148c0c8ee55119a9c6d640a42439c5cee7f0f`. No fixture executable was launched by those verifier controls.

Preserved integration failures: the scratch-derived oracle initially retained the wrong repository-parent depth and failed on import before building. Build20261023T181407251548Z then invoked the normal child instead of --writes; it failed its loading-path assertion. Build20261023T181453285488Z passed focused writes/controls but the final link verifier rejected unresolved private `_s_NextPhysicalSortKey`. These were fixture/tooling corrections, not native source-body changes. Current complete gate build is20261023T181657442144Z; the accepted report remains the earlier six-priority checkpoint until it closes. Full gate_all has not run.

Next prepared scratch: `work/native-recreation/loading/text-bank/20261023/threaded-close-pending-write`, observing Close while one real OS write is held after submission and another remains queued. Run only against a completed permanent base receipt. No installed files or saves are written; Lua remains outside this work.

---

## Actual worker writes and resizing: work-only checkpoint

Two aligned 4 KiB writes now complete through native worker WriteFileEx and typed completion callbacks, with native joins before freeing buffers or closing handles. Independent CRT reads match all 8,192 written bytes. The cached file length remains zero after writes, then public reopening/ResizeFile observes expansion to 12,288 and truncation to 4,096. Overwrite reopening truncates to zero; opening a child beneath the owned regular file fails without an open flag. Write callbacks correctly skip OnFileReadBegin. String ownership returns to the pre-probe counts. Only experiment-owned output files are written.

Four original/recreated worker/ResizeFile combinations pass twice using privately relocated original 786-byte worker and 58-byte resize bodies. The original OpenForWrite 143-byte body passes the existing exact relocation audit. Read-branch substitution, skipped SetEndOfFile and ignored overwrite each fail their explicit intended assertion. Scratch path: `work/native-recreation/loading/text-bank/20261023/threaded-write-resize-attempt2`. Original-worker receipt SHA256 `00dcdf09679c011dc4772bee96f2edd2a37836da985e68799f6e8b47728c9566`; verification receipt SHA256 `93b6fabc075841091b065fd59661c01f5161d5cd1c2a3dedcbdecc586995527f`.

Preserved failures: initial write scratch omitted the fixture's trailing cold boundary definitions and the FinishAndGetFileHandle provider, so linking failed; attempt2 retains those boundaries and adds the existing provider. The first original-worker relocation assertion incorrectly expected PE base-relocation entries, but the installed reference has none for this root. A second assertion incorrectly assumed the recreated worker's 28 adjustments; decoding all original instructions identifies 27 known adjustments (17 absolute, 10 relative). `original_worker_attempt3.py` checks complete 786-byte decoding, every absolute target and all external calls before running the four combinations. No original installer/error-path or disk latency/fairness claim.

Next: integrate a separate permanent --writes child and four original/recreated worker/resize runs, retaining explicit callback boundaries and exclusively build-owned outputs. Permanent report remains the all-priority device checkpoint until integration passes. Full gate_all has not run; Lua remains outside this work.

---

## All named-priority device modes: permanent checkpoint

The permanent loading gate passes 512 original/recreated combinations twice, 74 fault controls, 29 exact roots and three near contexts. The device child now covers both slice modes at each of the six named priorities, with 240 focused guarded payload windows across four alignments. All 1,024 original/recreated device-child runs also pass (61,440 payload windows). Dynamic per-priority cursor/deadline capture, independent request-order payload output, callback joins and locked restoration remain checked. The final input check found no drift among 782 pinned inputs. No physical-media, latency or fairness claim.

Receipt: `rebuild/build/native-text-eviction/00/9f/009fc170/20261023T180208708574Z/receipt.json`, SHA256 `703ae9bc88fc268fb4cd5b47266ef2de96ed18ac4435167ddbcdb82ebe17eb63`. `gate_all` now expects the observed `devices=60 device_priorities=6` marker; full `gate_all` was not run. Native recreation remains active, excluding Lua.

Next work-only boundary: actual aligned native worker writes into an exclusively build-owned file, independent CRT payload verification, completion/join ownership, cached length retention, and public resize/overwrite/failure branches. The existing ResizeFile source already has separate near-match and real-Windows acceptance; this work adds composition with actual worker writes and does not claim a new exact root.

---

## All named-priority device modes: scratch checkpoint

Work-only tests pass both slice modes at all six named priorities (1 through 6), across four alignments: 240 independently checked, guarded 32-byte windows, native object joins, callback counts, handle cleanup and locked state restoration. Sticky-zero, premature-rotation and ignored-setter controls fail at their intended assertions. Receipt `work/native-recreation/loading/text-bank/20261023/device-priority-levels-attempt2/receipt.json`, SHA256 `4d249dfcf5b51bb4589146316a9b8007e00ee68bf17456eaece9af1ca63f7a75`. Permanent promotion is in progress: the expanded fixture passes focused positives and all three controls; the complete original/recreated matrix remains running. Until that gate closes, the accepted permanent report remains the 40-window device checkpoint below.

---

## Native device time-slice permanent checkpoint - October 23, 2026

Recreation continues at the user's request; Lua remains outside this work. The permanent gate passes all 512 original/recreated resource-list/loading/failure-handler combinations twice, 74 targeted controls, 29 exact roots and three near contexts. Each combination also invokes the isolated `--devices` child twice: 1,024 device runs and 10,240 guarded 32-byte payload windows. Focused tests separately pass both zero-slice alternating and long-slice grouped scheduling at all four archive alignments (40 windows). These are logical C:/D: device categories, not physical-media, latency or fairness guarantees.

The reference path is supplied through `FABLE_NATIVE_DEVICE_REFERENCE` only to device and oracle children. Normal fixture invocation is unchanged. All callbacks/objects join before read handles close. Dynamic initial-device assertions, stable request-order output, independent byte comparisons and locked restoration of slices, 84-byte policy state and process timer input remain intact. The OS clock and installed reference file are unchanged. The public setter freshly matches all 18 original bytes without relocations. Sticky-zero, premature-rotation and ignored-setter faults hit their explicit intended assertions before the full matrix.

Receipt: `rebuild/build/native-text-eviction/00/9f/009fc170/20261023T175421525429Z/receipt.json`, SHA256 `d85edb45728236c0d280060bf0db25803edfaadc7d49cc559e1d73f8bb760407`. All 782 pinned inputs passed the final no-drift check. `gate_all` now expects the verified 74-control/29-exact/device marker; full `gate_all` was not run. Existing native C++ source bodies remain unchanged. The first failed integration build and its fixture forward-declaration correction are recorded below.

Next scratch boundary: both device modes at each of the six named read priorities, with native joins, independent payload checks and the same three scheduling controls. The first scratch compile in `work/native-recreation/loading/text-bank/20261023/device-priority-levels` failed because a broad fixture replacement also changed the earlier file-key helper. Preserved that attempt; attempt2 restricts the replacement to ProbeDevices. Permanent inputs remain frozen during scratch work.

---

## Native recreation resumed: permanent device integration in progress

The user resumed original-compatible native C++ recreation, excluding Lua, on October 23. The paused closeout below is historical. Integration adds an isolated `--devices` fixture child, with the read-only C: reference supplied only through `FABLE_NATIVE_DEVICE_REFERENCE` in child environments. Normal invocation stays unchanged. Both scheduling modes retain the scratch lifetime, guarded payload, dynamic initial-device, fixed output order and locked state-restoration checks. The oracle now explicitly invokes the device child twice for each resource-list/loading/failure-handler combination.

The first integration build `rebuild/build/native-text-eviction/00/9f/009fc170/20261023T175236756344Z` failed compiling Host0 because the copied block omitted `class CThreadedFileManager;` immediately above it in scratch. The missing fixture forward declaration is corrected. No native source body changed. This failed build is preserved; permanent coverage remains the prior 71-control/28-exact receipt until the corrected complete gate passes. Full `gate_all` has not run.

---

## Normal-Z coordination update - October 23, 2026

Native requested a fresh approximately 90-second test-free gap for two 30-second close-view captures after its guard observed particle sound-name PID 7456 and particle attachment PID 13724. Neither PID was present in the subsequent process check. This paused seek/exception session did not launch or stop either process and will launch no tests while paused. No global quiet window is guaranteed for other sessions. Native reports its frontend closed and its own lease released; earlier opening/shadow windows are complete, with no indefinite hold requested. A new capture window still requires current coordination with any active test owners.

---

## Launch coordination cleared - October 23, 2026

Fable2Native reports all D3D/Vulkan captures complete and its owned leases released. No reserved quiet period or D3D extension remains; clear the earlier coordination test hold. Its current work is CPU-only shader contract analysis. Future GPU windows will be coordinated afresh. This update supersedes the earlier pending-window status; it does not resume recreation or tests, which remain paused at the user's request. The earlier queue rejection remains historical evidence, not an active hold.

---

## Session paused at user request - October 23, 2026

The user ended this session and requested documentation for the next session. Recreation and test launches are stopped. Historical instructions below to continue until stopped are superseded by this closeout. Resume the established original-compatible native C++ approach when requested; Lua remains outside this work.

Last permanent checkpoint: 512 original/recreated combinations run twice, 71 fault controls, 28 exact roots and three near contexts. All four archive alignments pass. The final closeout hash check found zero changes among the 781 pinned gate inputs. Full `gate_all` was not run. No tests were launched during closeout.

Permanent receipt: `rebuild/build/native-text-eviction/00/9f/009fc170/20261023T164219060753Z/receipt.json`, SHA256 `33de405cee8459f1e902c565aa69ccfd6e8b8486a7f3d1dc9c7690a80fede70d`. Current report: `rebuild/compile-gate/native-text-eviction.json`.

Device time-slice work remains scratch only: `work/native-recreation/loading/text-bank/20261023/worker-device-slice-modes-170208/receipt.json`, SHA256 `ea3af698198694051a65f44f68c99f1ac29619d74dc33d2fd0382d0bbc246691`. Both zero-slice alternation and long-slice grouping pass across four alignments (40 guarded payload windows); three targeted controls fail as intended. The existing native setter matches 18 original bytes. Process timer input, slice values and policy state restore under the manager lock; the OS clock is unchanged. The installed executable was read only as reference data. No physical-media, latency or fairness guarantee is established.

Next session:

1. Integrate the two scratch device modes into the permanent fixture with an isolated `--devices` child mode. Supply the reference path through a child-only test environment variable, also inherited by the original/recreated oracle. Keep canonical normal invocation unchanged.
2. Add the existing SetDeviceTimeSlice provider and strict byte check; expected exact-root count becomes 29. Add the three device controls; expected control count becomes 74. Update `gate_all` only with the verified marker. Check focused children before the complete matrix.
3. Preserve callback and file lifetimes, independent payload comparisons, dynamic initial-device assertions, stable request-order output and locked state restoration. Detailed evidence and earlier limitations follow below.

Launch coordination: Fable2Native thread `01a168a5-fbee-72f0-87fe-39450caddb64` requested Vulkan/D3D test-free windows. The latest `codex queue` gap notice was rejected because that thread already has 100 queued submissions; delivery is unconfirmed. Do not clear its queue or claim the notice was delivered. Coordinate any renewed launch request before native test executables run. No user game was launched or closed.

---

## Native device time-slice scratch checkpoint - October 23, 2026

Continue established native C++ recreation and verification until asked to stop; no Lua.

Work-only real C:/D: logical-device reads pass both zero-slice alternation and long-slice grouping until the current queue empties. Five guarded32-byte reads per mode, two modes andfour alignments produce40 independent payload windows. The C: reference is the installed original executable opened only as read-only data; its hash is unchanged. D: uses build-owned archives. Typed completion leaves andnative object joins preserve callback/file lifetimes; only fixture-owned read handles close.

The native public SetDeviceTimeSlice method matches original18-byte code withno relocations. Cached deadlines expire through a controlled process-clock input shift while the worker is held outside alertable waits. Both slice floats, the84-byte device/deadline policy state andclock input restore under the manager lock after allcallbacks/objects join. OS clock is unchanged. Initial cursor is checked dynamically, not assumed; payload output uses fixed request order. Sticky-zero, premature-rotation andignored-setter controls each fail explicit assertions.

Receipt: `D:\Documents\FableTLC\work\native-recreation\loading\text-bank\20261023\worker-device-slice-modes-170208\receipt.json` SHA256 `ea3af698198694051a65f44f68c99f1ac29619d74dc33d2fd0382d0bbc246691`.

Next: integrate two device modes in the permanent gate. Supply the original reference path through a child-only test environment variable, including the original/recreated oracle subprocess, andkeep actual first-device assertions independent of incidental empty-queue cursor movement. Last permanent receipt remains the71-control/28-exact independent-handle checkpoint. Cross-device physical-media/latency/fairness guarantees remain outside this evidence. Peer launch-coordination queue has reached100 pending submissions; latest gap notice was rejected. Do not claim delivery, andhonor any incoming window request without closing user games.

---

## Native independent read-handle checkpoint - October 23, 2026

Continue original-compatible C++ recreation and verification until asked to stop; no Lua.

Three independently opened native read handles now establish open-sequence sort-key precedence over file offset. After a held normal-priority read on handle3, queued high-priority admissions are handle3/offset0, handle2/offset8, handle1/offset128; actual selection is handle1,2,3 despite descending offsets. Typed controlled callback leaves check one begin and completion each, four guarded32-byte windows per alignment, and restored string counts. Native waits join object removal only after every completion is captured, then only the fixture-owned read handles close. No bank owner switching. Reversing file-key comparison consistently across20 consumers fails the intended handle assertion.

The permanent gate passes512 original/recreated combinations twice,71 controls,28 exact roots andthree near contexts. Allfour priority/offset/tie scenarios (64 full128-byte windows) andthe independent-handle scenario (16 guarded32-byte windows) pass. Full gate_all was not run.

Receipt: `D:\Documents\FableTLC\rebuild\build\native-text-eviction\00\9f\009fc170\20261023T164219060753Z\receipt.json` SHA256 `33de405cee8459f1e902c565aa69ccfd6e8b8486a7f3d1dc9c7690a80fede70d`.

Next: inspect actual native device time-slice policy before choosing a bounded cross-device case. CThreadedFile::Open classifies D: as native device1 andother drives as device0; these are logical original categories, not claims about physical media. Native read sort keys are open sequence counters, not disk sectors. Keep controlled inputs explicit, restore any changed process state, and preserve callback/file lifetimes. Never launch/close a user game. Coordinate requested D3D gaps with thread01a168a5-fbee-72f0-87fe-39450caddb64.

---

## Native equal-offset request checkpoint - October 23, 2026

Continue original-compatible native C++ recreation and verification until asked to stop; no Lua.

Four scheduling scenarios now pass: priority selection, queued promotion with active-read refusal, equal-priority file-offset sorting, and equal-priority/equal-offset object-address tie-breaking. Three duplicate-offset requests retain independent data ownership and all complete. Object sort keys are captured only while live, stored as integers, never dereferenced after completion, and never printed. All64 full128-byte windows and final ownership cleanup pass across four alignments. Both reversed-offset and reversed-object-tie controls rebuild all20 header consumers consistently and fail explicit assertions.

The permanent gate passes512 original/recreated combinations twice,70 controls,28 exact roots andthree near contexts. Full gate_all was not run.

Receipt: `D:\Documents\FableTLC\rebuild\build\native-text-eviction\00\9f\009fc170\20261023T162905358796Z\receipt.json` SHA256 `d6e4e451013f91375faae376a4aa84e1871a20d2fee18d46d4cd6a36ed8d5196`.

Next: actual ordering across independently opened read handles on one device. Native PC Open assigns a monotonically increasing read sort key; it is not a measured disk-sector location. Use the public CThreadedFile interface and controlled typed completion leaves rather than switching a live bank owner. Preserve callback/file lifetimes through native Close and worker synchronization. Cross-device fairness/time slicing remains unclaimed. Continue coordinating later D3D gaps with Fable2Native thread01a168a5-fbee-72f0-87fe-39450caddb64; never close user games.

---

## Native equal-priority offset checkpoint - October 23, 2026

Continue established native C++ recreation and verification until asked to stop; no Lua.

Three equal-priority requests admitted at entries 4, 3, 2 execute at entries 2, 3, 4, following physical file offsets. Combined with priority selection and queued promotion, all 48 full 128-byte payload windows and native ownership cleanup pass across four alignments. A reversed-offset comparator rebuilt consistently in all 20 consumers fails the explicit offset assertion. A partial single-COMDAT mutation was inconsistent and is preserved as an unsuccessful control-design attempt.

The permanent checkpoint passes all 512 original/recreated combinations twice, 69 controls, 28 exact roots and three near contexts. Full gate_all was not run.

Receipt: `D:\Documents\FableTLC\rebuild\build\native-text-eviction\00\9f\009fc170\20261023T162231634601Z\receipt.json` SHA256 `54f850655e34bc3b60270b66df93e116667f12742795f70d5f1cca50b9277909`.

Next: integrate the already-passing same-priority/same-offset scratch case and its consistent reverse-object-tie control. The scratch captures object addresses only while live, keeps integer sort keys without later dereference, and prints no addresses. Three duplicate-offset requests all complete, payloads and cleanup pass across four alignments, and reversing the final pointer tie-break across 20 providers fails the intended assertion. Active scratch location is recorded in work/native-recreation/loading/text-bank/20261023/active-worker-priority.txt. No cross-device fairness or time-slice claim. Continue coordinating requested D3D launch gaps without closing user games.

---

## Native worker priority integration checkpoint - October 23, 2026

Continue the established original-compatible C++ recreation and verification approach until asked to stop. Lua is outside this work.

Actual worker scheduling now composes with real queued bank reads. Holding the first real OS submission admits three more requests. Baseline runs normal/highest/below-normal/low (3,5,2,1); explicit queued promotion produces 3,5,4,2. An active read refuses reprioritization while its descriptor is safely held alive at the nonalertable worker barrier. IsFinished polling avoids wait-driven priority promotion. All32 full128-byte windows match independent archive bytes over four alignments, and every data/list/heap ownership check passes. The existing native CThreadedFile::SetObjectPriority forwarding provider is linked and its10-byte body/two relocations match original bytes; this is integration of an existing recovered method, not new global coverage credit.

The full checkpoint passes512 original/recreated combinations twice,68 fault controls,28 exact roots andthree near contexts. Scheduling faults use an isolated --priority child mode: reverse order,ignored queued promotion andchanged active priority each fail at explicit assertions. An initial attempt applied reverse order to unrelated loading tests and timed out; preserved under build20261023T161017332964Z. Focused child-mode correction is preserved at work/native-recreation/loading/text-bank/20261023/priority-control-fix-161409.

Receipt: `D:\Documents\FableTLC\rebuild\build\native-text-eviction\00\9f\009fc170\20261023T161416631256Z\receipt.json` SHA256 `9ff14d49f382a41336e169f7175adf7ff3ca9e7b7493e484f74a696cf62cb414`. Full gate_all was not run.

Next: actual equal-priority request ordering with deliberately reversed admission order, observing physical offsets and independent payload windows. Cross-device fairness/time slicing remains unclaimed. Preserve all exception-string,stale UID,bank lifetime andallocator qualifications. Coordinate later D3D gaps with Fable2Native thread01a168a5-fbee-72f0-87fe-39450caddb64; never close user games.

---

## Native worker priority scratch checkpoint - October 23, 2026

Established recreation continues. Work-only priority probe holds the first real OS submission, queues low/highest/below-normal bank reads, then releases the worker. Actual submission order is normal/highest/below-normal/low (3,5,2,1). All sixteen 128-byte payload windows match independent archive bytes over four alignments, and all native data/list/heap ownership clears. Polling IsFinished avoids WaitUntilFinished priority promotion. Reversing the actual worker priority scan fails the probe.

Receipt: `D:\Documents\FableTLC\work\native-recreation\loading\text-bank\20261023\worker-priority-160206\receipt.json` SHA256 `62a313ae85e4645cb474a27f3ee9beb88ba5af141aca256556b892094ecfce3e`. Initial incompatible-header compile attempt is preserved; the observer uses the public descriptor at verified CFileObject offset zero rather than changing production declarations.

Next: cover explicit queued reprioritization and refusal to reprioritize an active read, then promote the scheduling scenario into the loading integration gate. No cross-device fairness/time-slice claim. Latest permanent checkpoint remains seek-exception integration (512 combinations twice, 65 controls, 27 exact roots, three near contexts). Continue coordinating later D3D gaps; never close user games.

---

## Native seek exception checkpoint - October 23, 2026

Continue original-compatible C++ recreation and verification until asked to stop; Lua remains outside this work.

Restored CBBBFileException public inheritance from CExceptionBase, verified against original throw metadata. Six direct consumers retain identical function bytes and normalized relocations. The compile-only metadata gate verifies both catchable types and rejects the old declaration. Legacy throw-info aliases preserve historical object re-verification.

The permanent loading integration gate now uses a separate EH-enabled caller translation unit and isolated --seek-exception child mode. Both invalid relative-seek branches are caught through CExceptionBase; source cursor stays unchanged; subsequent reads match independent archive bytes; original one-string-per-throw retention is asserted. The child exits before general cleanup, which remains unclaimed. Four alignments pass. Removing the base class is detected at runtime.

Full loading checkpoint passed: 512 original/recreated combinations twice, 65 fault controls, 27 exact roots and three near-match contexts. Full gate_all was not run.

Receipt: `D:\Documents\FableTLC\rebuild\build\native-text-eviction\00\9f\009fc170\20261023T155508894351Z\receipt.json` SHA256 `83d933eb0d75d03f140d6045d767f4e87cc87a76900b2580d72640414741e90c`.

The requested 90-second launch window was honored and extended voluntarily. After a two-minute queued scheduling notice, short console probes and this checkpoint resumed; no user game was closed. Coordinate the later D3D gap with Fable2Native thread 01a168a5-fbee-72f0-87fe-39450caddb64 when requested.

Next: investigate actual worker scheduling with multiple queued native bank reads at different priorities, using the existing real-ReadFileEx publication barrier and bounded observations. Preserve single-request and queue ownership semantics, original stale completed UID behavior, and all established cleanup qualifications. Do not tune bank-header 009D0880 blindly; it remains functional-only and unpromoted.

---

## Native file exception inheritance correction - October 23, 2026

Continue the established original-compatible C++ recreation and verification approach; the user explicitly clarified that the comparison with other ports was curiosity and asked work to continue until stopped.

Original throw info 01369380 has two catchable types: CBBBFileException (4 bytes) and CExceptionBase (1 byte). fable_file_exception.h omitted the empty base. Restored public inheritance while preserving the four-byte layout. Updated six fresh-compile address maps from compiler symbol __TI1 to __TI2 at the same original throw-info address.

New gate_native_file_exception_metadata.py passes compile-only metadata checks for MovePosition, SetPosition, Read and Open against the original PE, and rejects a missing-base mutation. Added it to gate_all. MovePosition/SetPosition/Read bodies and normalized relocations remain unchanged (227/159/290 bytes). Full integration and gate_all have not been rerun after this header correction.

Work-only probes at work/native-recreation/loading/text-bank/20261023/seek-exception-153608 compile and link with an EH-enabled caller: fixed base catch, old missing-base control, and specific-type observation. No probes have been launched because the Fable2Native launch hold remains active. Catch dispatch and string ownership remain runtime-unverified. The original seek routines install no local FS exception chain; do not assume outer string temporary cleanup. Scratch probes intentionally exit before general cleanup.

Next: obtain launch-window release from thread 01a168a5-fbee-72f0-87fe-39450caddb64, run bounded exception probes, retain failures, then decide permanent runtime coverage based on actual ownership results. Continue offline work while waiting. Never close a user game.

---

## Native relative-seek checkpoint and launch coordination - October 23, 2026

Native MovePosition now covers seven forward/backward/zero moves and start/end transitions on an independent clone. Three guarded read windows match separate CRT reads and independent archive bytes over four alignments; the source cursor remains unchanged. The 227-byte body matches original bytes and relocations. Bad-seek exception unwinding remains unclaimed.

The integration gate passed: 512 original/recreated combinations twice, 64 mutation controls, 27 exact roots and three near-match contexts. Full gate_all was not run.

Next: continue the established original-compatible C++ recovery and verification approach. The user clarified that the comparison with other ports was curiosity, not a request to change priorities. Investigate bounded disk seek failure paths and native exception/string ownership next; Lua remains outside this work.

Launch coordination: Fable2Native thread 01a168a5-fbee-72f0-87fe-39450caddb64 requested a 90-second test-free Vulkan shadow window and a later D3D window. A codex queue signal was delivered (message 01a16944-c7cb-7dc2-b802-afc5c59e9762). Continue offline source/documentation work; hold further executable tests until the peer reports completion or coordinates the next window. Do not close user games.

Receipt: `D:\Documents\FableTLC\rebuild\build\native-text-eviction\00\9f\009fc170\20261023T145337367658Z\receipt.json` SHA256 `b6998443b4919ddb6681272d124136398935fd8a1a6d14e65400678107db0a9a`.

---

## Native disk clone checkpoint - October 23, 2026

Native Clone now opens an independent read-only handle, starts at cursor zero, preserves source ownership/cursor independence and uses the native metadata getter. Prefix/source-window reads match independent archive bytes; deleting the clone leaves the source usable and restores its saved position. Four clone/alignment cases and eight read windows pass.

All 512 original/recreated combinations pass twice, with 26 exact roots, three near-match contexts and 62 mutation controls. Clone's 70-byte body matches original bytes and relocations. Self-return and unopened-copy faults are detected. Full gate_all was not run.

Continue with relative MovePosition on independent clones: positive/negative/zero deltas within bounds, start/end transitions and guarded reads versus a separate CRT/archive oracle. Bad-seek exception unwinding is outside this next integration claim. Lua, installed files and saves remain untouched.

---

Receipt: `D:\Documents\FableTLC\rebuild\build\native-text-eviction\00\9f\009fc170\20261023T144555781628Z\receipt.json` SHA256 `d6a6e29bade35f271f0ceea10411d1e209ca28ea56e167e1dc20d232b4e85535`.

## Native disk-file information checkpoint - October 23, 2026

Disk-file metadata now uses actual C++ getters, FILETIME conversion and default date construction. Guarded virtual calls compare all three timestamps with before/after direct OS queries, test writable/read-only attributes on the owned archive, restore its original attributes, and return zero dates for a controlled null handle. Original return copying includes unspecified internal padding; only outer guards and defined fields are asserted. Twelve cases pass over four alignments. Original text/disk vtable targets and enumeration literal bytes are now verified inside the permanent gate.

All 512 original/recreated combinations pass twice, with 25 exact roots, three near-match contexts and 60 mutation controls. New exact bodies are 222/84/25 bytes. The earlier padding-assumption failure is preserved. Full gate_all was not run.

Continue with the existing native CDiskFileWin32::Clone provider (70 bytes): actual independent read-only handle, cursor independence, prefix/window byte oracle, virtual metadata dispatch, deletion and surviving source. Lua, installed files and saves remain untouched.

---

Receipt: `D:\Documents\FableTLC\rebuild\build\native-text-eviction\00\9f\009fc170\20261023T143914697348Z\receipt.json` SHA256 `8f13b3d57ae78b03fd12672efb0dc319bb91b16ac12485c5d842dab162685369`.

## Native metadata vtable leaves checkpoint - October 23, 2026

Text-bank vtable slots 18/19 now use actual fixed-date and enumeration C++ providers. Sixty-four guarded cases per leaf cover hidden return storage, raw type values, exact labels/date fields, copied-string lifetime and unchanged bank snapshots. Their prior cold aliases were removed from this fixture. Original primary vtable targets and all three literal byte sequences are independently pinned in work/native-recreation/loading/text-bank/20261023/metadata-leaves-initial/retail-data-proof.json.

All 512 original/recreated combinations pass twice, with 22 exact roots, three near-match contexts and 57 mutation controls. Exact additions are the 82-byte enumeration leaf, 32-byte fixed timestamp leaf and 53-byte date constructor. Full gate_all was not run.

Continue with native CDiskFileWin32::GetFileInformation, FILETIME conversion and the default date constructor, replacing the remaining disk metadata cold boundary. Initial scratch bodies are 222/84/25 bytes. Use guarded virtual returns, actual OS metadata oracles, owned-fixture read-only attribute restoration and a controlled null-handle failure case. Lua, installed files and saves remain untouched.

---

Receipt: `D:\Documents\FableTLC\rebuild\build\native-text-eviction\00\9f\009fc170\20261023T141520443834Z\receipt.json` SHA256 `7045a85308ddf137c775ead281280ff7ecfb22dfa45cd905a714910d97e5aa49`.

## Native mid-queue deadline checkpoint - October 23, 2026

Three actual ready bank reads now exercise deadline expiry between completion callbacks through native GFGetTime and real timer APIs. The first controlled callback shifts this process's start-tick input; native processing stops after one notification. Restoring the input allows the remaining two notifications. Twelve independent payload windows, all reference/list cleanup and restored clock state pass across four alignments. The OS clock is never changed; callback clock input remains a test boundary.

All 512 original/recreated combinations pass twice, with 19 exact roots, three near-match contexts and 54 mutation controls. Caching the clock once per queue-processing call is detected as a behavioral change. Full gate_all was not run.

Continue by replacing the remaining cold text-bank metadata vtable leaves with their existing genuine C++ providers: GetEnumerationType (009C79E0), fixed GetLastModifiedTime (009CB710), and the plain date constructor. Initial scratch compilation confirms 82/32/53-byte bodies; shipping primary vtable slots 19/18 point to those methods. Keep guarded hidden-return, string ownership and unchanged-bank tests explicit. Lua, installed files and saves remain untouched.

---

Receipt: `D:\Documents\FableTLC\rebuild\build\native-text-eviction\00\9f\009fc170\20261023T135436837951Z\receipt.json` SHA256 `d857f4598a69da8d49d3a2d6a21d7f04e2d055782bc14e5157a7c7cbcdb353d1`.

## Native queue-abort/disposal checkpoint - October 23, 2026

A controlled loading consumer stays queued at age one and while the abort global is disabled. At age two with the global enabled, native queue removal invokes its abort notification, which calls the actual static async discard wrapper. The submitted OS read refuses cancellation, so DisposalList retains the sole data reference until callback publication and native drain. Payload bytes and final list/pool/heap cleanup pass at all four alignments. Consumer abort policy remains an explicit test boundary.

All 512 original/recreated combinations pass twice, with 19 exact roots, three near-match contexts and 53 mutation controls. New exact roots are the 16-byte static discard dispatcher and 196-byte data Abort. New faults cover abort-global gating, frame age and notification. Full gate_all was not run.

Continue with a deadline that expires between loading completion callbacks: use the actual GFGetTime routine with a controlled, restored start-tick input, verify one notification before stopping, then finish the remaining ready reads. Lua, installed files and saves remain untouched.

---

Receipt: `D:\Documents\FableTLC\rebuild\build\native-text-eviction\00\9f\009fc170\20261023T134519333105Z\receipt.json` SHA256 `2ad81607459d98f48126f0b577403996ed7051fc9a3b25456284e2813acb550e`.

## Native resource-bank frame checkpoint - October 23, 2026

The actual 15-byte CResourceBank::FrameEnd now advances and resets both nonempty resource/loading lists around a real pending text read. Queue completion moves the controlled consumer, public decoding drops its data reference, and native eviction removes the text resource while the consumer/local handle retain the async buffer. Scope cleanup returns both lists and tracked heap allocation to zero at all four alignments.

The existing retail-shaped frame provider now supplies both frame methods; a fresh dedicated-provider comparison proves the standalone list-frame bytes and relocations are identical. No new production header change was needed. All 512 original/recreated combinations pass twice, with 17 exact roots, three near-match contexts and 50 mutation controls. Full gate_all was not run.

Continue with age/global-policy abort of a loading-queue consumer while its actual OS callback is held, native transfer into the disposal queue, completion and final drain. Keep consumer virtual policy explicit. Lua, installed files and saves remain untouched.

---

Receipt: `D:\Documents\FableTLC\rebuild\build\native-text-eviction\00\9f\009fc170\20261023T133626432015Z\receipt.json` SHA256 `1f7de804353192d8a1a0d49a73b26a25a8189cc72eb746282816e7810ac04bc8`.

## Native pending-Close/reopen checkpoint - October 23, 2026

Native Close now runs twice while a real pool-backed OS read is submitted and its completion callback is held. Base disk pointer and metadata size clear; the bank object, threaded owner, async data and buffer remain alive. Completion bytes match independent archive data, then reopening restores public text decode and eviction. All four logical alignments pass. No bank is destroyed with its own live data.

All 512 original/recreated combinations pass twice, with 16 exact roots, three near-match contexts and 48 mutation controls. Focused pool critical-section, allocation-policy and async teardown gates also pass after the shared-header linkage fix. Full gate_all was not run.

Continue with the actual CResourceBank::FrameEnd coordinator over both nonempty resource/loading lists, a real pending read, completion notification, public decode and final ownership cleanup. Prefer the existing retail-shaped frame provider that emits both frame methods, preserving the standalone method's byte identity. Lua, installed files and saves remain untouched.

---

Receipt: `D:\Documents\FableTLC\rebuild\build\native-text-eviction\00\9f\009fc170\20261023T132411831914Z\receipt.json` SHA256 `6834f39772453b1512f8dcd8a74c56e330d94abaeba6f9e05616d82d7c1b9c71`.

## Native zero-size ownership checkpoint - October 23, 2026

Eight successful zero-size heap cases confirm retail ownership behavior: SetupBuffer allocates a real tracked block but records a zero internal size; native data destruction leaves that one allocation retained. The fixture records the retained count and explicitly frees the orphan through native deallocation, returning tracked allocations to zero. No zero-size OS read is submitted. A normalization fault that frees based on Buffer rather than the original size charge is detected.

All 512 original/recreated combinations pass twice, with 16 exact roots, three near-match contexts and 46 mutation controls. Exact additions include the 348-byte async-data destructor and its 30-byte deleting wrapper. This is an observed original ownership quirk, not a production fix. Full gate_all was not run.

Continue with Close during a submitted pool-backed read, retaining the bank object and threaded owner until callback/data cleanup; then reopen and decode through the public getter. Lua, installed files and saves remain untouched.

---

Receipt: `D:\Documents\FableTLC\rebuild\build\native-text-eviction\00\9f\009fc170\20261023T131814877828Z\receipt.json` SHA256 `a9e8a76b2d907a291ed058693a404ecdee7f6ca4ee4ddbcfc2d8430e41ee6881`.

## Native null-allocation policy checkpoint - October 23, 2026

A typed null-return allocator leaf now exercises the actual InternalAllocate and SetupBuffer routines without issuing any OS read. Across 32 force/length/skip/alignment cases, the native code returns true with null Buffer, sets Data to the raw skip value, and leaves the entire allocator object snapshot unchanged. Restoring the actual leaf permits 24 positive-size retry allocations with balanced release. These observations preserve retail behavior; they do not claim OS exhaustion handling or safe use of a null-backed read buffer.

All 512 original/recreated combinations pass twice, with 14 exact roots, three near-match contexts and 45 mutation controls. New exact roots include SetupBuffer (310 bytes) and InternalAllocate (122 bytes). No successful zero-size buffer was allocated in this checkpoint. Full gate_all was not run.

Next: bound the successful zero-size allocation/destruction ownership path, preserving original accounting and using explicit fixture cleanup if the native owner retains the allocation. No OS read should be submitted by these cases. Lua, installed files and saves remain untouched.

---

Receipt: `D:\Documents\FableTLC\rebuild\build\native-text-eviction\00\9f\009fc170\20261023T131131582428Z\receipt.json` SHA256 `e8b077ec7c117b2468e48dfbf4b01ae1e685fb5c91c2e64fd01106d3effd22fd`.

## Native shared-pool cross-bank checkpoint - October 23, 2026

Two live text banks now share the actual movable pool, loading failure handler and retail threaded-file owner. Both registration directions reclaim the other bank's finished buffer. Updating the requester leaves victim restart work queued; updating the victim resubmits its own read. Sixteen cross-bank/alignment payload windows match independent archive bytes. Peer Close/destruction unregisters only that peer and drops shared-owner counts while a surviving bank's completed data stays readable. No bank is destroyed with its own live external data.

All 512 original/recreated combinations pass twice, with 12 exact roots, three near-match contexts and 42 mutation controls. New faults require scanning the second registered bank and removing the correct peer registration. Full gate_all was not run. The failed first compile used generic counted-pointer field names for the existing threaded-file specialization; corrected to its verified control_ field and preserved the failed attempt.

Next: bounded null-return allocation policy through native InternalAllocate and SetupBuffer without issuing an invalid OS read; preserve original return and pointer/accounting behavior, then verify recovery after restoring the real allocator leaf. Lua, installed files and saves remain untouched.

---

Receipt: `D:\Documents\FableTLC\rebuild\build\native-text-eviction\00\9f\009fc170\20261023T130243124249Z\receipt.json` SHA256 `23c084bbe4a0cadb9305ee05d20e60ac688ed9d8dcae420ceaa1a25f0fab1fd6`.

## Native relocated-pool decode checkpoint - October 23, 2026

Public text and group getters now decode actual bank payloads after their native pool buffers relocate. Text, tags and group values match expected records; getters return shared resource handles, release async data, preserve the original 72-byte stale async-object charge, and native eviction clears owner/list memory. Eight text/group/alignment cases pass. Two normalization faults prove the stale size accounting is intentionally retained.

All 512 original/recreated combinations pass twice, with 12 exact roots, three near-match contexts and 40 mutation controls. Full gate_all was not run. Pending disposal, partial/full relocation, real pool failure/restart and queue scheduling remain covered. The only production change remains the VC71 scoped-helper inline declaration fix with unchanged body/relocation proof.

Continue with two live text banks sharing one pool/failure handler and retail file: reclaim in either registration direction, resubmit on the correct bank, and tear down the peer while a surviving bank retains its completed read. Do not destroy a bank with its own live external data. Lua, installed files and saves remain untouched.

---

Receipt: `D:\Documents\FableTLC\rebuild\build\native-text-eviction\00\9f\009fc170\20261023T125607538161Z\receipt.json` SHA256 `fc61d6d637803eec8669d9f25f4dd7f8185b2305a134ed62d9c76edc5587354e`.

## Native partial-relocation checkpoint - October 23, 2026

Native PartialDefragment now refuses to move an actual pending read while its OS completion callback is held. After publication, the same fragmented pool moves the finished buffer into another free area, updates its handle/resource binding and coalesces enough free space. Independent archive bytes match at alignments 1, 4, 16 and 2048. Finished/lock readiness gates and handle-table updates are covered by targeted faults.

All 512 original/recreated combinations pass twice, with 12 exact roots, three near-match contexts and 38 mutation controls. Exact additions include PartialDefragment (585 bytes), CBankFileAsyncData::CanRelocateData (76 bytes) and movable-pool CanMoveResource (26 bytes). Full gate_all was not run. Prior pending disposal, full relocation, pool shortage/restart, queue scheduling and eviction remain covered.

Continue with public text/group getters decoding payloads from relocated pool buffers, then native eviction and preserved stale async-object size accounting. Lua, installed files and saves remain untouched.

---

Receipt: `D:\Documents\FableTLC\rebuild\build\native-text-eviction\00\9f\009fc170\20261023T125111160990Z\receipt.json` SHA256 `0a67b351b193ef5d5070bf603a89dcce7ab56c90e15d6720820984cd763c64cb`.

## Native completed-read relocation checkpoint - October 23, 2026

A completed pool-backed bank read now moves into a leading free area through native Defragment, actual memmove and CBankFileAsyncData::RelocateData. Buffer and Data keep their skip displacement; handles and resource binding stay correct. A locked earlier read prevents full compaction of a second fragmented read. Both payload windows match independent archive bytes at all four alignments. Pool LockCount retention after freeing a locked allocation is explicitly preserved until Uninitialise.

All 512 original/recreated combinations pass twice; 35 mutation controls pass. Nine exact roots now include Defragment (380 bytes), bank-data relocation (78 bytes), and movable-pool copy/callback dispatch (53 bytes), alongside the prior three near-match contexts. The full gate_all suite was not run. Native source bodies remain unchanged; the earlier scoped-helper inline declaration fix remains the only production header change in this continuation.

Continue with partial defragmentation while real read completion is held, then after publication. Preserve original wait/UID and allocation/lock accounting behavior. Lua, installed files and saves remain untouched.

---

Receipt: `D:\Documents\FableTLC\rebuild\build\native-text-eviction\00\9f\009fc170\20261023T124645104951Z\receipt.json` SHA256 `2bd2fa95e0d9030cc31db56aed56c6ab525a8befa7b3866e6a4ab5db65253e3b`.

## Native pending-disposal checkpoint - October 23, 2026

Pending disposal now passes with a real OS read submitted. A test-only ReadFileEx forwarding slot holds the worker in a nonalertable barrier after actual submission. Native DiscardAsyncData transfers the sole caller reference into DisposalList; native Abort refuses while the worker remains reading, and AsyncUpdatePending retains the data and pool buffer. After callback publication, the independent archive bytes match and native disposal frees the data and allocation. All four logical alignments pass.

A fresh ordinary-worker object proves that the forwarding build changes only the ReadFileEx import-slot name: instruction bytes and every other relocation are identical. The final native link proof resolves that import to the explicit forwarding boundary. All 512 original/recreated root combinations pass twice, with six exact roots, three near-match contexts and 31 mutation controls. A fixture diagnostic flush was added before ExitProcess so mutation failures remain observable without waiting for a held worker; the prior diagnostic-only failure is preserved. Full gate_all was not run.

Next: completed pool-read relocation into a leading free area, real CBankFileAsyncData pointer adjustment, and locked-buffer defragmentation refusal. A work-only candidate is saved at work/native-recreation/loading/text-bank/20261023/pool-relocation-candidate.cpp. Preserve the original retained pool LockCount after freeing a locked allocation until Uninitialise; do not normalize original accounting. Lua, installed files and saves remain untouched.

---

Receipt: `D:\Documents\FableTLC\rebuild\build\native-text-eviction\00\9f\009fc170\20261023T124207396225Z\receipt.json` SHA256 `2e29bd45c1d539cf482cf10c98cdc36d89e5585965ef73ab4bef6b08e46aedce`.

## Native pool reclaim/restart checkpoint - October 23, 2026

Actual CLoadingMemoryFailureHandler registration, finished-buffer reclamation and worker resubmission now pass with the native movable pool. A finished restartable read loses its pool buffer on shortage, enters ReadsToRestart, and is resubmitted by AsyncUpdatePending; both read windows match independent archive bytes after the second buffer releases. All four logical alignments pass with zero tracked heap buffers and zero final pool allocation. Registration and unregistration counts are checked; handler policy in this scenario is native, separate from the earlier controlled refusal/recovery cases.

All 512 original/recreated combinations of eviction, queue and failure-handler/reclaim roots pass twice (1024 processes). Six exact roots, three near-match contexts and 30 mutation controls pass. The focused native failure-handler parity gate also passes all seven roots. The original matrix timeout was increased from 60 to 180 seconds for the expanded process count; the timed-out attempt is preserved. Full gate_all was not run.

Next work: pending disposal ownership while a real OS read has been submitted and its completion callback is held. A candidate fixture is saved in work/native-recreation/loading/text-bank/20261023/pending-disposal-candidate.cpp, not yet applied. Preserve prior completion/UID race semantics and inline-linkage evidence. Lua, installed game files and saves remain untouched.

---

Receipt: `D:\Documents\FableTLC\rebuild\build\native-text-eviction\00\9f\009fc170\20261023T123658932544Z\receipt.json` SHA256 `40c7614395edf5684e63595aa474153ca22673b483d9824fd5af33a0121b0931`.

## Native movable-pool checkpoint - October 23, 2026

Native movable-pool success, nonforced shortage refusal, forced heap fallback and controlled failure-handler recovery now pass alongside eviction, disposal, scheduling and aligned reads. Twelve pool/alignment cases match independent archive bytes and resource binding; pool and heap ownership return to zero. All 128 original/recreated eviction/queue combinations pass twice; 24 mutation controls are detected. Random relocation stress and pool failure-handler policy remain controlled boundaries. Full gate_all was not run.

A shared-header linkage fix adds inline to CScopedCritical constructor/destructor declarations for VC71. Scratch compilation proved all named bodies, offsets and relocations unchanged in both affected providers; the focused pool-allocation and movable-pool gates also pass. Pool integration revealed a fixture assumption error: callback completion may precede the caller storing AddObject's returned UID, leaving a completed UID in retail. A bounded native-accessor publication wait and stable buffer/refcount assertions replace the invalid UID-zero assertion; native wait/assignment semantics were not changed. Failed attempts and the linkage proof are preserved under the build attempts and work/native-recreation/loading/text-bank/20261023/attempt-notes.json.

Next: compose native loading-failure handler registration, finished-buffer reclamation and resubmission with the real pool and worker. Lua, installed files and saves remain untouched.

---

Receipt: `D:\Documents\FableTLC\rebuild\build\native-text-eviction\00\9f\009fc170\20261023T123102799528Z\receipt.json` SHA256 `0c90a495eafba29c55ffe89c74e6a7f99952cfe8d1785064127814de402eadc1`.

## Native aligned-read checkpoint - October 23, 2026

The native text eviction integration now also checks automatic reads at logical alignments 1, 4, 16 and 2048, in cached and noncached arithmetic modes. Each returned window matches independent generated-archive bytes; skip displacement and rounded allocation size are checked. OS handles remain cached, so direct-I/O support is not claimed. All allocated read buffers return to zero.

The combined gate passes 128 original/recreated combinations twice, five exact roots, two near-match contexts and 20 mutation controls. The fixture generator retains its original byte-identical alignment-1 archive by default. Pending-read eviction, completed disposal ownership and controlled loading-queue scheduling remain covered. The full gate_all suite was not run. Next: real movable-pool allocation shortage, forced heap fallback and bounded failure-handler recovery. Lua, installed files and saves remain untouched.

---

## Native loading continuation - October 23, 2026

Pending eviction, completed disposal ownership and loading-queue scheduling now pass together. Two real bank reads remain owned during zero-budget eviction while the submitting thread holds the bank critical section; after completion, both evict without decoding. Three completed allocated reads drain through the native disposal queue; one retained external handle stays readable until released. Two controlled queue consumers use actual bank-read readiness: pending reads remain queued, an expired deadline stops processing, and completion notifications move both consumers into the native resource list exactly once. Consumer virtual callbacks remain test boundaries.

The permanent native text eviction gate freshly compiled 352 providers and verified 510 linked symbols / 471 native bodies. All 128 original/recreated combinations of five eviction roots and two queue roots pass twice (256 processes); 18 mutation controls are detected. Five roots match original bytes with relocation checks; the linked queue context is 255 versus 260 bytes, 96.9388% opcode similarity, and is behavior-tested against the original. The prior unload near-match remains unchanged. The native production bodies were unchanged. `gate_all` recognizes the expanded marker, but its full suite was not run.

Report: [native text eviction](../../../rebuild/compile-gate/native-text-eviction.json). Continue with allocation-failure behavior and wider read alignments. Nonempty pending disposal, pool-backed allocation failures, renderer and bank destruction with live handles remain unclaimed. Preserve stale ResourceSize accounting and allocator class-owned chunk retention. Lua, installed game files and saves were untouched.

---

Build: `D:\Documents\FableTLC\rebuild\build\native-text-eviction\00\9f\009fc170\20261023T121450638663Z`.
Receipt SHA256: `e60a26b24d56b2b854f07f50082094009fffd3f5c3b45c16c9ab25a27e345a83`.

## Native recreation resumed - October 23, 2026

User requested continued recreation work, excluding Lua. Pending-read eviction now passes the permanent native text eviction gate: two actual worker reads are held pending under the bank critical section; zero-budget eviction retains both owner references, resource-list nodes, and size accounting. After the lock is released, both reads finish and eviction removes the resources without forcing decoding. Tracked read-buffer allocations return to zero.

All 32 original/recreated eviction-root combinations pass twice. Four exact roots and the existing near-match unload root remain verified; 349 providers were freshly compiled, with 506 linked symbols / 467 native bodies checked. Fourteen fault controls pass, including separate premature text/group owner-reset controls. The full `gate_all` suite has not been run. No native production-body change was needed.

Receipt: [native text eviction report](../../../rebuild/compile-gate/native-text-eviction.json). Failed and earlier attempts remain preserved. Continue with nonempty disposal/loading queues and scheduling, then allocation failures and wider alignment. Preserve the existing allocator-retention and stale ResourceSize behavior. Lua, installed game files, and saves remain outside this work.

---

Build evidence: `D:\Documents\FableTLC\rebuild\build\native-text-eviction\00\9f\009fc170\20261023T120631339353Z`.

Receipt SHA256: `3ad0ad959dbc42fa59ad90eb4f56926c85af005142a87c79ed7735ebd423e6cd`.

### Additional compile-only coverage

The exception metadata gate now covers all six direct consumers in the disk/memory stream units: MovePosition, SetPosition, Read, Open, Write and CMemoryDataOutputStream::SetPosition. All six generated function bodies and normalized relocations are unchanged by the inheritance correction. Open emits compiler-numbered local labels; their names differ, but each resolves to the same offset in its function section. The compile comparison normalizes those offsets rather than treating label spelling as behavior.

Evidence: `D:\Documents\FableTLC\rebuild\build\native-file-exception-metadata\20261023T154059416501Z\body-stability.json`. Runtime launch hold is still active.

### Exception checkpoint refresh

Latest compile-only receipt: `D:\Documents\FableTLC\rebuild\build\native-file-exception-metadata\20261023T154317715345Z\receipt.json`, SHA256 `b66a57100e15b51ce70b3f4e683699ec8e8c28cd33474ca6bb5db6a4c3e29fd9`. This gate now includes the six-body stability comparison directly, not only in a scratch script. Root sizes are 227, 159, 290, 640, 314 and 46 bytes.

The smaller `bounded-base-catch-fixed.exe` and `bounded-base-catch-old-control.exe` probes in the seek-exception scratch folder are built but unlaunched. They skip the full loading suite and perform only archive/bank open, clone, two invalid relative seeks and two independent-byte-checked reads. A catch-all converts the old missing-base behavior into a bounded diagnostic exit (80), avoiding an unhandled exception dialog. Each probe intentionally exits before general cleanup, so no leak-free cleanup claim is made.

### Historical object compatibility and launch schedule

The six address-map consumers retain the old __TI1 symbol as a legacy alias alongside the corrected __TI2 symbol at original address 01369380. This preserves re-verification of frozen older objects; the new metadata gate independently rejects current builds with only one catchable type. Original-byte auditing of both old and corrected Open objects passes, including original literal/import checks.

After the requested 90-second gap elapsed, queued a two-minute scheduling notice to Fable2Native (message 01a16974-b4a0-7492-a524-514b2e912708). Earliest short-console-probe resume is 2026-10-23 15:52:18 UTC unless an extension is requested. This replaces the earlier open-ended voluntary hold; no user game will be closed.

### Coordination queue limit

The post-checkpoint gap notice after the independent-handle run was rejected: target thread01a168a5-fbee-72f0-87fe-39450caddb64 queue cannot contain more than100 submissions. Do not claim that notice was delivered. Earlier notices were accepted for queuing; their reading/consumption is unconfirmed. Continue authorized native work and honor any incoming launch-window request; do not close user games.
