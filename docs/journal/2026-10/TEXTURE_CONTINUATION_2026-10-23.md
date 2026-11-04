## Stopped at user request - October 23, 2026

User: "Let's call it here for now. Update docs etc for next session."
Implementation, builds, tests and runtime work are stopped. Resume only after a
new user instruction. Historical active-continuation notices below are superseded.

Latest verified app remains **original-restore-dev16**. Latest accepted supplement:
**dev16-deep-workflow-private**, SHA
3d84277d7cec86ecaed4d1d19c3395f90b6b78255ebd474bbb3fe7cdae75e383.
Its extracted46module/373source toolkit passed full shallow/deep UTF-8/BOM
workflow,58ownership/rollback checks,6offline publication controls, DDS36,
terrain28, font identity5, font boundaries and16definition guards.
App/runtime unchanged. No GUI/game/GPU launch. Desktop relocation is unanswered.

**Pending, unpromoted:** build/texture-native-image-pins-20261023.
Both selected-PNG races reproduced: replacement between hash/decode published
wrong-color pixels; replacement after rebuilding went unnoticed. Font additionally
reproduced a previously consumed PNG changing while a later row was processed.
Native font/terrain CLI now rechecks PNG pins after decode and before publication.
Fourteen source-linked ordinary/deep controls pass (font8/terrain6), source SHA
matches this exact frozen candidate. The full targeted CLI build completed while
the stop status was being checked; no subsequent binary tests or promotion ran.
Source373 differs from the accepted workflow kit only in tools/forge-cli/main.cpp.
The next session must distinguish source-linked checks from pending production
binary acceptance. Build artifacts, source manifests and baseline failures remain.

Next session:
1. Read AGENTS and these latest handoffs; use this owning texture checkout.
2. Validate the built image-pin candidate with font identity/boundary and terrain
   checks, then full shallow/deep UTF-8/BOM wrapper regression. Reuse unchanged
   core/ownership findings unless new changes or failures justify repeating them.
3. Finalize a new immutable private toolkit only after those gates pass; retain
   the accepted deep-workflow packet and all rejected candidates.
4. Do not launch the game/GUI/GPU without fresh coordination and the shared lease;
   do not recreate a Desktop kit until the destination is known.

Mutation receipts: build/native-image-race-1tmk5b2u/verification.json (font8),
build/native-image-race-ybe_diov/verification.json (terrain6).
Baselines: build/native-image-race-sh3zvtwd/verification.json (font8),
build/native-image-race-0tmi10l2/verification.json (terrain6).
Checkpoint: dist/development/FableTextures-SESSION-CHECKPOINT-20261023.json.
Latest accepted packet: build/texture-wrapper-deep-paths-20261023e/distribution-verification.json.
Pending CLI SHA256: 1a7e5f586dafba792686a72e89153254784587709dfeb818ec9ffbc3fb33b279. 

## Stopped at user request - final checkpoint

Implementation and runtime work stopped. Final read-only process check found no
texture game/test/build; shared RuntimeLease absent. No native trial queued.
Desktop remains complete original-restore-dev5. Dev6 privately passes 129 process
checks (INI 30, setup 12, restore 47, Unicode 30, portable 10); matching source ZIP
and Desktop/dist promotion remain pending. Dev6 fixes UTF-8 BOM registration,
rejects UTF-16/32/NUL Mods.ini before publishing files, and correctly reads setup
BOM/comments. Two frozen source changes only: import.cpp and textureproject.cpp.
Dev6 ZIP SHA256 65cc34247f9f203daf8d152491d4e6b60b026f3ef6ba2c1022b308cf62b49a15.

Prepared native checkpoint d was never launched. Eleven launcher diagnostic /
cleanup controls pass offline; native snapshot/camera correction remains untested.
Native peer queue rejected latest request at 100 submissions; do not clear peer
queue or reclaim another lane's lease. New user resume and fresh coordination
are required before a launch. No private FSE shipped; no public release.

Authoritative next-session handoff in texture checkout:
C:/Users/Cornelio/AppData/Local/FableForgeDevelopment/texture-project-20261014/
docs/TEXTURES_NEXT_SESSION_20261018.md
Full chronological evidence: docs/TEXTURES_CONTINUATION_20261023.md there.


## Resumed October 23: dev6 source and distribution completed

User resumed ongoing FableTextures work. Reviewed the two-file frozen delta
against dev5 and independently checked all 365 source hashes, packaged EXE and
current receipt EXE identities. Completed source-only ZIP; all source members
match manifest bytes/hashes and ZIP CRC checks pass. Included current package
README and separately labeled inherited GUI/DPI/metadata receipts. No tests
were rerun and no new GUI/native acceptance is claimed.

Source ZIP SHA256:
`c0aa0992e57ea0b53c77a157d8b5c43fa5aaa1d1100442ed887c6f2c8ba6766d`.
Binary ZIP remains
`65cc34247f9f203daf8d152491d4e6b60b026f3ef6ba2c1022b308cf62b49a15`.
Both ZIPs and extracted app copied into `dist/development`; every copied file
hash independently matched. Evidence in `build/texture-mods-ini-20261023/`:
`source-bundle-verification.json` and `distribution-verification.json`.

Desktop promotion is pending: the previously documented testing-kit directory
is no longer present. Bounded Desktop discovery did not locate it; user asked
for its new location. No old notes overwritten or replacement kit invented.
The first finalize attempt stopped on portable receipt schema (no state field);
corrected validation checks its exact ZIP/EXE hashes and ten recorded checks.
Next attempt created verified source ZIP, then refused absent Desktop notes;
no Desktop files changed. Separate distribution-only step completed safely.
The retained finalize.py is a one-shot historical command, not safe to rerun:
source ZIP and dist candidates already exist. Use the receipts/current state.

Shared lease observed owned by Fable2Native thread
`01a169e1-34a9-7971-8cfb-e18b5d84ea27`, token
`4af593dab2c349f48b735e191eed3d9a`; left untouched. No game/GPU/GUI launch,
peer message, installed-game change or publication. Continue offline review
while Desktop destination and runtime coordination remain unresolved.


Offline trial-readiness audit on resume: all 18 prepared d source pins and all
nine saved checkpoint reference PNGs/crops remain exact. Read-only receipts:
`build/texture-route-checkpoint-plan-20261023d/resume-source-audit.json` and
`resume-checkpoint-audit.json`. This does not qualify camera correction or route
coverage; d remains unlaunched and requires fresh coordination.

Added a separate private validation-source supplement to dist/development:
`FableTextures-0.3.0-original-restore-dev6-validation-private-20261023.zip`,
SHA256 `8a070f21e192706e3b5a1c46e8fb586a3dce802d839bb07662b9cd28fcf49186`.
It contains the five current offline suite entry points and their ten local
helper modules, a manifest, instructions and source/binary identity receipts.
It preserves the frozen 365-file app-source ZIP unchanged. Fresh extraction
passes all five entry-point imports/help with no checkout-helper dependency.
The 129 process suites were not rerun by this packaging check. Evidence:
`build/texture-mods-ini-20261023/validation-bundle-verification.json` and
`build/texture-dev6-validation-sources-check-20261023/verification.json`.


Fresh dev6 source-kit extraction and ordinary clean MinGW/CMake app build PASS.
All 365 extracted source hashes match; rebuilt `.text`, `.data`, `.rdata` and
imports section match the original dev6 build exactly. Whole EXE hash differs,
so this is section identity, not a claim of identical PE files. Rebuilt --help
passes with System32-only PATH. No process suite repeat, GUI or game launch.
Evidence `build/texture-dev6-source-kit-validation-20261023/verification.json`
and `rebuilt-executable-verification.json`; copies accompany dist/development
as Restore-dev6-source-kit-build.json / Restore-dev6-rebuilt-executable.json.
Desktop destination remains pending user reply. Latest lease read found it
absent; this does not replace fresh runtime coordination or authorize claiming
a slot while another lane has a queued trial. No native d trial launched.


## Continued after premature milestone stop: font export preservation

User corrected the premature stop and reiterated ongoing work until asked to stop.
Work continues offline. New independent packaged-dev6 witnesses show silent
font output overwrites for same parent names, case-only names, sanitized parent
names, Unicode language names and sanitized bank names. Each affected collection
reports success but one raw original disagrees with its own recorded SHA256.
Duplicate font identities also silently overwrite. Six baseline controls in
`build/texture-font-paths-kloltvef/verification.json` preserve source files.

Candidate collection.cpp disambiguates colliding source-folder/bank labels with
stable source-derived suffixes; ordinary noncolliding paths remain unchanged.
The collection output writer refuses any remaining duplicate output path, using
existing staging cleanup rather than publishing partial output. Isolated frozen
365-file dev6 snapshot differs only collection.cpp; clean app/probe build running
in `build/texture-font-collisions-20261023`. Candidate behavioral pass not yet
claimed. New helper `tools/test_texture_collection_font_paths.py` independently
checks original bytes, source preservation and decoded pixels; duplicate input
must refuse with no completed folder or retained private staging directory.

Expanded existing collection check with --unicode-paths and explicit UTF-8 JSON
reads; packaged dev6 passes mixed main/frontend/animation/volume/signed-vector/
loose-image/static-font pixels and malformed-font controls in Unicode install,
loose basename and long Unicode output paths (`texture-collection-be3_msi1`).
Initial harness-only failure used locale-default cp1252 JSON decoding; actual
UTF-8 manifest and files were correct. No app Unicode regression claimed.
Streaming-font metadata helper now accepts explicit --exe to avoid borrowing a
sibling build; candidate check pending. Package README explains colliding font
folders. No runtime/game/GUI/public/installed-Steam change.


Font-source preservation fix complete in private original-restore-dev7. Ordinary
clean isolated CMake app/probe build PASS. All 365 frozen source files match;
only collection.cpp differs from dev6. Package README is current and separately
included in source ZIP. Current packaged app passes 25 process checks: seven
font collision/refusal controls, ASCII+Unicode collection workflows (four app
processes), streaming metadata (one), extracted portable (ten), help/usage (three).
Independent originals, decoded PNG pixels and all source hashes pass; duplicate
identities refuse with no final output or abandoned private export staging.
Normal noncolliding font output path verified unchanged. Baseline seven-control
receipt: texture-font-paths-t5079ls9; candidate texture-font-paths-ogyj664j.
Other evidence: texture-collection-png_vprf / xfxshwyj,
streaming-font-export-xhn590hp, textures-package-batch-lxa7za9i.

Binary ZIP SHA256 `8a539d83083cf6b5714551765dd33da75dc749286e9fc15248601e144abc6daa`;
EXE `2b59db5bddf098a53cfa1f921d6c85fb821ce68a6b40bf48b57377fafc326624`;
source ZIP `5962e45febb44718dbbd7219b61dd39e4cdc01820c1f5cc48fbc2ce73cb41fcd`;
validation ZIP `8271b178310fae479956aa60b28922634b00dfc566375f209dc9ac09ddb60b58`.
All ZIP CRC/source hashes and full extracted app copies verified in dist/development.
Fifteen-module validation supplement's four entry points import/help standalone
(texture-dev7-validation-extract-sbzxp7af). No new source ZIP clean-build repeat:
the 365 packaged source bytes match the candidate's already clean-built snapshot.
Dev6 INI/setup/restore/Unicode and dev5 GUI evidence is explicitly INHERITED;
not rerun or claimed as current dev7 gates. Both runtime DLLs unchanged. No GUI,
FSE, native game, installed-Steam or public release changes. Desktop remains
pending relocated testing-kit path. Full assembly/source/receipt verification:
`build/texture-font-collisions-20261023/source-and-distribution-verification.json`.
Work continues; this milestone is not a stop request.


## Deep output/project path fix: private original-restore-dev8

Continuing offline after dev7. Actual dev7 export fails at a 351-character output
path even with existing parents; deep image-project review also fails. Six
baseline app controls retained in textures-deep-paths-zu6vtsqz. Candidate uses a
shared native-path helper at export destination, GUI output checks and CLI/GUI
image-project review boundaries. Input display text remains unchanged. Direct
controls caught MinGW fs::absolute converting UNC into a local-drive path;
GetFullPathNameW now preserves the local/UNC form before native prefixing.
Seven actual compiled helper controls pass, including idempotence. No real
network share accessed or accepted.

Actual deep review first refused error123. Preferred image separators alone
were insufficient: resolvedPath's lexical normalization also collapsed an
already extended prefix. Final resolver preserves that prefix and native
spelling before physical handle resolution. Initial failed receipts/logs and
all frozen incremental correction stages are retained; no failing gate erased.
Final five-file overlay relative to dev7: src/resolvedpath.hpp,
src/textureproject.cpp, tools/texture-export/{collection.cpp,collection.hpp,main.cpp}.
All other 360 files unchanged. Isolated clean build + final corrective rebuilds
PASS; source-manifest binds all365 final files.

Final packaged dev8 passes186 process checks: deep paths14, original restore47,
Unicode30, CLI73, extracted portable10, font collision7, collection ASCII/Unicode4,
streaming-font1. Deep controls use >300-character Unicode projects with existing
and missing parents, relative dot/dotdot input, edited image import, repeat no-op,
exact original restore, existing-output refusal and physical junction-escape
refusal. All original bank/backup bytes and edited images preserved. Current
receipts: textures-deep-paths-2a3tru9o, textures-original-restore-_9m0sjjw,
textures-unicode-install-_gfa29ue, textures-cli-arguments-02sttn5i,
textures-package-batch-92d6n3dq and texture-font-paths-21gnp46x.
Authoritative export receipt contains exact ASCII/Unicode/streaming directories;
see build/texture-deep-paths-20261023/export-regression-verification.json.

Binary ZIP `6291599aea6a09a7990a5e1f08e35a2e0ca141ae593f180820eaf5ff229b982f`;
EXE `015e6a1eef4ec87588024b996833408d85a55503ffdf175317f2ae7451a51055`;
source ZIP `c394f23cc9aca74215b04312c93f131d50b87c73229c74e7aab7afc6a89d5ca3`;
validation ZIP `f09ffaae5ffff07278a5f2aea036be86925435bde0fa85a4ce943c73248a3df8`.
Binary/source/validation ZIP CRC/hash and complete extracted app copied and
verified in dist/development. Validation19Python modules + source-linked C++
probe; all eight extracted Python entry points import/help independently
(texture-dev8-validation-extract-zeqkx0_s). Final source ZIP matches exact
built365-file snapshot; no redundant source-kit clean rebuild claimed.

INI/setup evidence inherited from dev6; earlier GUI/DPI and metadata evidence
historical, not dev8 reruns. No new actual GUI, deep game-root, network-share,
native-game or performance acceptance. Runtime DLLs unchanged, private FSE not
shipped, no installed Steam or public release change. Desktop relocation reply
still pending. Complete receipt/source identities:
build/texture-deep-paths-20261023/source-and-distribution-verification.json.
Work remains active; proceed with further offline review until user asks to stop.


## Active deep installation follow-up (candidate not promoted)

Dev8 remains latest verified dist package. Next isolated candidate is
build/texture-deep-install-20261023, frozen365-file dev8 plus five changed files:
src/pendingbanks.hpp, src/textureproject.cpp and tools/texture-export/
{collection.cpp,import.cpp,main.cpp}. Deep game-root baseline: ordinary and
explicit-native export both fail without publishing output; source bank exact
(texture-deep-install-audit-pznu71cv). Nine full baseline controls in
textures-deep-paths-zdobvxxv also preserve source files. New --deep-install test
covers main/frontend image projects, existing/missing output parents and an
explicit native install spelling; positive validation still pending.

Candidate normalizes game roots at program I/O boundaries; joined archive/mod
paths use native separators. PendingBanks retains incoming extended roots,
prepares native paths and uses existing bounded owned-tree cleanup that never
follows reparse points. Normal nonextended constructor branch remains as before.
Initial clean build and corrective rebuilds retained. Actual deep export caught
invalid sourceArchive metadata from fs::absolute on an extended archive path;
metadata now preserves valid spelling. Next actual import caught a remaining
non-native separator in core texture apply; current rebuild fixes that join.
Do not infer a passing full deep-install gate yet.

Cleanup test incident and correction: the standalone MinGW wmain helper's CRT
expanded an explicit \?\ argument as a wildcard, truncating it before main.
Read-only argv-path-diagnostic.json proves plain input intact and native argument
mangled; app uses CommandLineToArgvW independently. Helper created exactly two
small .pending-cleanup-probe directories under newly created C:/root-?? before
its boundary assertion caught the error. Both directories contained only the
known 'owned fixture' bytes; no junction had been created in that failed case.
Strict PowerShell shape/hash/reparse/creation-time checks recovered both into
build/texture-deep-install-20261023/escaped-cleanup-fixtures and removed only
the verified newly created empty C:/root-?? root nonrecursively. Recovery hashes
pass; outside sentinels exact. before-move.json / verification.json retain proof.
No user files or original game/save files were involved.

Helper now receives plain absolute paths, invokes the exact shared native-path
helper internally and validates converted spelling before any writes. Actual
PendingBanks cleanup passes short/deep Unicode roots and staged junctions while
preserving outside sentinel bytes: pending-bank-cleanup-m3fjpi8u/verification.json.
No helper process left running. Test helper executable has a distinct name;
no fake Fable.exe, real game, GUI or GPU process launched. Subsequent game-root
candidate remains private until positive preservation/rollback gates pass.


## Deep installation and collection fixes complete: private dev9c

Latest verified dist app is original-restore-dev9c. Five files differ from the
frozen365-file dev8 source: src/pendingbanks.hpp, src/textureproject.cpp,
tools/texture-export/{collection.cpp,import.cpp,main.cpp}. Corrects deep game
roots, native archive/mod joins, valid sourceArchive metadata and source-relative
collection identities. Extended paths make libstdc++ lexical_relative return
empty even for matching root prefixes; collection now checks native lexical
prefixes at directory boundaries with ordinal Windows comparison, and rejects
empty/rooted/dotdot relative paths. Special/malformed entries use the same rule.
Source indices retain alias spelling; physical image escape checks stay in place.

Current packaged dev9c passes93 app process checks: deep install27, junction
install27, deep project14, font collision7, ASCII/Unicode collection4, streaming
metadata1, extracted portable10, help/usage3. Includes main/frontend editing,
sourceArchive readback, repeat no-op, exact restore, locked frontend/final-metadata
rollback, image-junction refusal and cleanup. Expanded collection/streaming checks
assert source-relative paths identify actual input files. Receipts:
textures-deep-paths-ckshwgc1 / arcvmj_j / kkbnc1cp,
texture-font-paths-u5yzaq53, texture-collection-s3h11f2u / xqor98oo,
streaming-font-export-xqwoywi7, textures-package-batch-ulop69lg.

225 component checks are explicitly INHERITED from the earlier private dev9
binary: restore47, Unicode30, CLI73, legacy8, metadata25, INI30, setup12. That
whole candidate was rejected for collection regressions and never promoted.
Current source is byte-identical to that component-tested snapshot except
collection.cpp; included manifests and EXE pins prove the distinction. Those225
checks are not claimed as dev9c reruns. Rejected dev9/dev9b binaries, source
manifests, collection.cpp revisions and failing process receipts remain intact.

Two actual PendingBanks cleanup cases pass for short/deep Unicode roots and
staged junctions, bound to final header/cleanup-source SHA values. Earlier seven
native path controls bind the unchanged helper header. No real network share,
GUI/DPI, game/driver/performance or native route acceptance added. Runtime DLLs
unchanged; private FSE excluded. No installed Steam or public release writes.

Binary ZIP SHA256 e12396a83ad516b264ec68eb0a55e3e1794d67f2ce7c6343afd28f953c313a61;
EXE 9a69623ef41b83d43e3c2a207a35073e3d9841bcfed313a759b9bd2b12784609;
source ZIP 6020aa8ab87581452b09f14215a80a4c221877145acb271db5a7ec14d32db99f;
validation ZIP 90338b8a54ab179354b7a0cf94c2cecf708a63f4faf4e37a59b559409c77ba24.
All365 source members, ZIP CRC/hash and complete extracted app verified in
 dist/development. Validation24Python modules; thirteen standalone entry points
import/help successfully after extraction (texture-dev9c-validation-extract-
mbmh0vr4). Full identities/limits/current-versus-inherited evidence:
build/texture-deep-install-20261023/dev9c-source-and-distribution-verification.json.
Desktop promotion remains pending relocated kit path; previous packages kept.

Read-only cause control confirms helper CRT wildcard expansion: adding only
int _dowildcard=0 preserves the same explicit native argument before wmain.
argv-no-glob-verification.json retains the actual output. App still uses its own
CommandLineToArgvW parser; no product CRT patch. Earlier two accidental controlled
fixture directories recovered byte-exact into escaped-cleanup-fixtures; empty
newly-created C:/root-?? removed nonrecursively. Independent final read finds
that root absent. No helper remains active. Work continues with offline portable
executable/runtime discovery review; no runtime launch queued.


## Portable runtime discovery complete: private dev10

Latest verified dist app is original-restore-dev10. Only main.cpp/import.cpp
differ from frozen365-file dev9c. Replaces fixed executable-name buffer with
bounded growing lookup, normalizes default/explicit runtime paths, and refuses
empty runtime input before any CWD DLL lookup/copy. Twelve baseline dev9c
failures/workarounds retained in textures-portable-paths-ttapt9ho.

Current175 app/probe process checks PASS: deep executable and explicit runtime10
(including spaces/Unicode), ordinary portable10, CLI73, Unicode portable6,
restore47, deep install27, plus packaged export and actual empty-runtime probe.
Five source-linked API controls cover short/long names, initial/later failure,
and bounded repeated truncation. Actual probe with an inert CWD DLL refuses;
original install/images/trap exact. Source matches compiled snapshot byte-for-byte.
No test provider in app. Primary API references and precise count breakdown in
build/texture-portable-runtime-20261023/api-references.json and
source-and-distribution-verification.json.

Receipts: textures-portable-paths-_3g82j5_, textures-package-batch-v2c10vjw,
textures-cli-arguments-uuzrrdy6, textures-unicode-install-hkg93gmu,
textures-original-restore-ey4uqkow, textures-deep-paths-pzmm_ol5,
texture-runtime-lookup-167sbrh7. Windows launcher control uses explicit
lpApplicationName; no claim that every shell launches deep executables.

Binary ZIP503c0c4d6fac4652374ffba0c8f593b261ed8e389e916174d13e5422f34a2c57;
EXE9379cda5d517e0496cfef743ce50b5ca6afbc4ab776b8e252494b511dc5dcbc0;
source ZIPd6d97142404aa8a378916ed12efb84018ae50a0cecf97d6baf83b41041c6802b;
validation ZIPb09dbfa30d418bf94480702c0894564ae9a7bd642bd4f7c9f60c363c417bbaa1.
All365 source members and ZIP CRC/hash, extracted app and dist copies exact.
Validation26modules; fifteen extracted entry points import/help independently.
Earlier collection/metadata/INI/GUI evidence retained as historical, not rerun.
Runtime DLLs unchanged; private FSE excluded. No GUI/game/GPU/installed-Steam or
public release change. Desktop relocation remains pending. Offline work continues:
review cancellation between private import preparation and live publication.

## Import cancellation boundary under validation

A deterministic source-linked host reproduced cancellation at the final live-root
running-game guard still publishing staged edits. Added one final progress.cancel
check immediately before transactional publication; ordinary import and early/late
cancellation controls pass, with exact install/image preservation and staging
cleanup on refusal. Baseline textures-import-cancel-scja6vai; final three-case
receipt textures-import-cancel-mizv_tpo. Only import.cpp differs from frozen dev10.
Isolated clean build texture-import-cancel-20261023 is in progress; dev10 remains
latest packaged candidate. GUI close currently finishes imports, so this fixes
the existing API boundary rather than adding a GUI cancel action. No game/GUI
launch or runtime DLL change.

## Private dev11 cancellation boundary complete

Latest verified package is original-restore-dev11 in dist/development; dev10 and
all earlier private artifacts preserved. Clean isolated app/probe build passes.
Only import.cpp differs from frozen365-file dev10. Source-linked controls pass
ordinary import plus early and final-gate cancellation; refused imports preserve
all bank/Mods/image files and remove staging. Final-build receipt:
textures-import-cancel-7jualxuw. Final candidate also passes original restore47,
deep-install27 and portable runtime10 process controls (84), plus six export/host
processes for the cancellation controls. GUI close still finishes imports; no
new GUI cancellation claim. Source snapshot, binary/source/validation ZIP hashes,
CRC and exact dist copies pass. Four freshly extracted validation entry points
import/help independently. Runtime DLLs unchanged. No game/GUI/GPU/public release.
Full receipt: build/texture-import-cancel-20261023/source-and-distribution-verification.json.
Binary ZIP dd90795d6522cf95874227e3e3adf96246057074e95ac5181e5cb8d69e657c88;
source ZIP 44dd6fbcdfc620d3a8492ff2cd6a3ead5ec71381d85dd593e4cb50d0bf09e909;
validation ZIP 74881fd3ee0e5c941941dcea5c2acbf1247809af4ddb50ef36cf1f8fa26d6408.
Desktop relocation still pending; continue offline source review.

## Deep supplemental collection scan under validation

A new deep-install variant of the existing collection fixture reproduced silent
omission: expected7 images, actual5, no warnings, only main/frontend sources in
index despite valid loose image/font files. Baseline retained at
build/texture-deep-collection-20261023/baseline-verification.json. The directory
scan joined extended root with forward slash; candidate normalizes that single
join with make_preferred. Only collection.cpp differs from frozen dev11.
Expanded fixture independently checks raw hashes, loose pixels, source integrity
and staging cleanup. Fresh isolated clean build is in progress. Dev11 remains
latest verified package; no game/GUI/GPU launch or runtime DLL changes.

## Private dev12 deep supplemental scan complete

Latest verified private package is original-restore-dev12 in dist/development.
Only collection.cpp differs from frozen365-file dev11. Baseline deep install
silently omitted loose/fonts (5of7 images); Fonts.BIG separately omitted font
(6of7). Preferred-separator-only attempt failed and is retained as rejected.
Read-only comparison proved MinGW directory_iterator on deep extended paths
returned CWD names; native FindFirstFileW returned the selected fixture entries.
Final collection scan uses native enumeration, explicit scan errors, cancellation
checks and no descent through directory reparse points. Font bank discovery is
ASCII case-insensitive. No compiler/CRT test hook in product.

Final57 app processes pass: source-junction2, collection10 (ordinary, Unicode,
deep, mixed-case, combined deep/mixed; each normal+malformed), font collisions7,
streaming metadata1, portable runtime10, deep install27. Independent raw hashes,
pixels, source-relative records, original files and cleanup pass. Actual scan
fixtures include self-cycle and outside image directory junctions in short/deep
roots; outside bytes exact and neither junction traversed. Six freshly extracted
entry points help/import, and extracted scan kit passes two additional app checks.
Initial isolated clean build plus final source-bound scan rebuild recorded.
Binary/source/validation ZIP CRC/hash and exact365 sources/dist copies pass.
Binary ZIP 5044e15ebb466aa2425483d6089d7bc68ca2440060ef21dc95cc9a1f3155487f;
source ZIP 6db9edf3011d7e5b6ea6943f46166be2d0d4978f5d03e565b58aa89fdac9e587;
validation ZIP a22e678a7310b2094e57099113a735d68317bb308c1700e1f061560790bb50f3.
Full receipt build/texture-deep-collection-20261023/source-and-distribution-verification.json.
Earlier dev11/10 retained. Runtime DLLs unchanged. No game/GUI/GPU or public release.
Desktop promotion awaits relocated destination. Offline program work continues.

## Whole-bank collection fingerprint under validation

Controlled actual-source host reproduced successful export with old PNG pixels
and replacement-bank source hash: mutation between ordinary export and existing
collection fingerprint passed both narrow checks. Baseline receipt
texture-collection-race-_08zi4lu. Moved the existing x.source boundary to cover
ordinary project export and supplemental extraction together. Only collection.cpp
differs from frozen dev12. Source-linked normal/mutated main and front-end controls
pass (texture-collection-race-zikwx9pr / 2qiywo5d): source replacement intentionally
retained, refused output absent, all staging removed. Hooks stay in private test
host; shipped app has none. Fresh isolated clean build is in progress at
build/texture-collection-source-20261023. Dev12 remains latest verified package.
No game/GUI/GPU/runtime DLL changes. Continue candidate build/package validation.

## Private dev13 whole-bank source fingerprint complete

Latest verified private package is original-restore-dev13. Clean isolated app/probe
build passes; only collection.cpp differs from frozen365-file dev12. Existing
source fingerprint now spans ordinary project export plus supplemental extraction
as one operation. Controlled baseline retained: old PNG pixels with replacement
source hash incorrectly succeeded. Final controls independently cover main and
front-end normal/mutated banks: normal succeeds; changed source refuses output,
external replacement stays exact, staging removed. Hooks only in private hosts.
Current57 packaged app checks pass plus four source-linked actual-export hosts.
Seven entry points from extracted source/validation kits help/import; extracted
front-end normal/mutation host checks also pass against exact extracted source.
Binary/source/validation ZIP CRC/hash and365source/dist file copies verified.
Binary ZIP c818aff75e6c9c061d0c0cc2548e5207864c40726afce930ac5d8f4d72e24bb5;
source ZIP 57e3db89ce1dcf6a6d2658ab87972c27e1828d57a02b56a8e89e01a35432ac26;
validation ZIP 5678b439bcdbe1bdd10bd3385d6ddb73680fdb0cd0eef51b5aa520ddfffc7be1.
Full receipt build/texture-collection-source-20261023/source-and-distribution-verification.json.
Runtime DLLs unchanged; earlier candidates preserved. No GUI/game/GPU/public release.
Desktop promotion pending relocated destination. Continue offline export review,
including cancellation/error handling in the new native directory scan.

## Dev13 native scan cancellation/error supplement

No app/runtime/source change. Four actual-source controlled Win32 cases pass:
normal, cancellation after first opened search, initial access denial, and
mid-enumeration access denial. Every opened native search closes; all source
files exact, refusal publishes no output and leaves no staging. Receipt
build/texture-scan-controls-wyyldc0_/verification.json binds collection source SHA.
Private test-only Win32 wrappers are absent from product. Dependency-complete
scan-validation supplement ZIP CRC/member/SHA/dist copy and extracted help pass.
Supplement SHA 4c1c9e55d4f5bfed2adcb62c62016157d7321fb8bca63c23ff3dbbedde990098. Full supplement receipt:
build/texture-collection-source-20261023/scan-supplement-verification.json.
Dev13 remains latest private app; no game/GUI/GPU launch. Offline work continues.

## Terrain collection source-label disambiguation under validation

Independent bounded STB fixtures reproduce whole-export refusal for same-stem,
case-folded, sanitized and Unicode source-label collisions; one source succeeds
with exact compressed frame/pixels. Baseline texture-terrain-paths-twry9qp_ is
retained. No data-loss claim: old collision guard refused and removed staging.
Candidate only changes filesystem namespace: colliding terrain source labels gain
a stable relative-source SHA suffix (same pattern as fonts); noncolliding paths
stay unchanged. All STB bytes/fields/formats and runtime DLLs untouched.
Five source-name fixtures will run in ordinary and deep Unicode install/output
paths, with independent compressed-frame/pixel/source/staging checks. Only
collection.cpp differs from frozen365-file dev13. Fresh clean build in progress
at build/texture-terrain-source-paths-20261023; dev13 remains latest verified app.
Applied fableforge-format-io for evidence/byte-preservation discipline; native
runtime loading of these synthetic extraction fixtures is not claimed.

## Private dev14 terrain source paths complete

Latest verified private package original-restore-dev14. Clean app/probe build
passes; only collection.cpp differs from frozen365-file dev13. Package README
updated separately and included as root PACKAGE-README.txt in source ZIP.
Colliding terrain source labels now gain stable relative-source SHA suffixes;
noncolliding paths remain unchanged. No STB format or binary-writer changes.
Current61 app processes pass: terrain10 (five source-name cases in ordinary and
deep Unicode install/output roots), collection4, font7, source-junction2,
streaming1, portable10 and deep-install27. Terrain fixtures independently check
exact compressed frame bytes, distinct source/ID/frame records, known BC1 PNG
colors, relative SHA suffixes, original STB bytes, legacy single-source path and
staging cleanup. These are bounded extraction fixtures, not native-load claims.
Seven freshly extracted entry points help/import; extracted deep terrain kit
passes five more actual app controls. Binary/source/validation CRC/hash, exact365
sources and complete dist copies verified. Previous candidates retained.
Binary ZIP a84808cdb129589f3f7259beccdc22ffaf0737b5c23773388c0e037ac9867f51;
source ZIP ff5cda565305330af7bd0aec819c9ece3fbcf4020eb3d0e4eb86473f9099a846;
validation ZIP 1022c211bc2995062b2601425f81f314fa7a90f1ed0d68b72c4173cad01d630e.
Full receipt build/texture-terrain-source-paths-20261023/source-and-distribution-verification.json.
Earlier race/API controls remain historical. Runtime DLLs unchanged; no GUI/game/
GPU/public release. Desktop relocation pending. Continue offline program review.

## Dev14 late export cancellation/lifecycle supplement complete

Actual source-linked collection host passes five normal/cancellation/error cases,
including cancellation after bank extraction/fingerprinting but before collection
publication. All source bytes exact; late/early cancellation and both enumeration
error cases publish no output and leave no staging. Native search opens/closes
match in every case. Receipt texture-scan-controls-r4a30bur binds exact dev14
collection source. Win32/hash controls exist only in private test host.
No app/runtime change. Dependency-complete lifecycle-validation ZIP CRC/member
SHA/dist copy and extracted entry-point help verified. Supplement SHA
9aa9e555a9bb5390c36d197521708504c2dd2fa8d4b724a823dbb9fe490d717a. Receipt
build/texture-terrain-source-paths-20261023/lifecycle-supplement-verification.json.
Dev14 remains latest private candidate; no GUI/game/GPU/public release. Offline
program review continues; Desktop relocation still pending user reply.

## Packaged DDS diagnostic-path failure under validation

New actual-package sidecar workflow found valid DDS review fails before import:
fs::relative attempted to canonicalize an extended source path solely to format
DDS source text. Baseline textures-deep-dds-kirv828o retained; independent rebuilt
synthetic main/frontend fixtures match install hashes exactly, all exported PNG
hashes exact, no staging/backup writes. Full baseline receipt under
build/texture-dds-project-paths-20261023/baseline-verification.json.
Candidate uses the already validated manifest-relative filename for DDS source
notes and image decode errors, changing extension on DDS selection. Only
src/textureproject.cpp differs from frozen365-file dev14; no binary format/codec,
review-token, physical path guard or transaction changes. New24process workflow
covers shallow/deep Unicode main/frontend DDS with original/absent PNG, repeat,
known-color roundtrip, exact target format/identity/untouched entries/backups,
restore and PNG/DDS ambiguity, DX10, malformed PNG refusals with relative errors.
Clean isolated app/probe build in progress. Dev14 remains latest verified package
while dev15 validates. No game/GUI/GPU/runtime DLL change.

## Private dev15 DDS relative diagnostics complete

Latest verified private package original-restore-dev15. Clean isolated app/probe
build passes; only src/textureproject.cpp differs from frozen365-file dev14.
DDS source notes and image decoding errors use validated manifest-relative names,
with .dds extension when selected. No canonicalization solely for diagnostics;
physical path guards, review tokens, formats/codecs and transaction logic remain.
Current108 app/probe processes pass: shallow/deep main/frontend DDS24, original
restore47, deep PNG/install27, portable runtime10. DDS controls cover original PNG
present/absent, repeat exact bytes, known-color roundtrip, retained native target
format/identity, untouched entries/backups, original restoration, ambiguity, DX10
and malformed PNG refusals with relative filenames and exact install/input state.
Four extracted entry points help/import; actual extracted DDS kit passes24 more
app processes. Binary/source/validation ZIP CRC/hash, exact365 source members and
complete dist copies pass. Package README separately pinned in source ZIP root.
Binary ZIP 2c088bb7def3d58bbafa22e3c8a483e25093cead7511bb97f441169f75a3a0ad;
source ZIP 60439f1b588fc4b958f1fcf424d08debb36c5a503488c727577416c00e45c852;
validation ZIP 5ff6489950c710d3c6d01062582b72f1c61d3c94dc17b5621eda135e91b95e29.
Full receipt build/texture-dds-project-paths-20261023/source-and-distribution-verification.json.
Previous private candidates retained; runtime DLLs unchanged. Earlier collection/
terrain/API/GUI controls are historical, not dev15 reruns. No game/GUI/GPU/public
release. Desktop relocation pending. Continue offline program work.

## Dev15 consumer-tool integration supplement complete

No app/runtime binary change. Three offline tools now read collection/trial JSON
as UTF-8 with optional BOM: prepare_texture_upscale_trial, prepare_static_font_import,
prepare_terrain_texture_import. Inventory excludes only explicitly indexed raw
original files from PNG preview completeness; unindexed PNGs still refuse.
Separate witnesses retained: cp1252 Unicode mojibake inventory refusal, and ASCII
uppercase loose PNG counted twice (2indexed/3files). New integration exports six
actual dev15 previews from Unicode loose/font/terrain sources with colliding
sanitized labels, verifies UTF-8/BOM generation and BOM reuse, exact NN pixels,
correct two-font/two-terrain source hashes/IDs/request PNGs, unchanged inputs and
four prewrite refusals (stale font archive, terrain archive, generated font PNG,
unindexed PNG). Existing static-font9 refusal and terrain full/partial/extent256,
stale/overlap/overwrite checks pass. No native font/STB import invoked; deterministic
nearest-neighbor stress assets are not AI-art or runtime quality evidence.

Dependency-complete15module tooling/source/evidence supplement ZIP CRC/memberSHA/
dist copy and four extracted entry points pass. Extracted actual integration also
passes using the verified dev15 app. Supplement SHA 7244c5170406a877fbad7c11cbeb869ab46c275db833a8e4abb75763b59ea4ec.
Full receipt build/texture-unicode-consumers-20261023/distribution-verification.json.
Dev15 remains latest private app. Earlier artifacts kept; no GUI/game/GPU/public
release. Continue downstream import/verification reader review; other trial tools
are not covered by this three-tool fix. Desktop relocation remains pending.

## Dev15 Unicode loose-import and integrated-verifier supplement complete

Two further offline readers now decode UTF-8/BOM JSON explicitly:
import_loose_texture_trial and verify_texture_integrated_trial. Separate Unicode
baseline failures retained; source delta changes only JSON reads. Actual loose
BMP imports pass UTF-8/BOM with exact requested pixels, unrelated header bytes,
trailer, source hashes and input preservation. Integrated verifier passes two
Unicode/BOM asset/coverage fixtures and four corruption/false-completion refusals;
remaining unsupported coverage stays explicit. Synthetic verifier asset bytes
exercise file identity/coverage checks, not native compatibility.
Existing loose routing source/PNG/duplicate/overlap controls, BMP/TGA template
boundaries and integrated verifier names-length normalization plus12 false-success
refusals pass. No external composer/game invocation or app/runtime binary change.

Initial validation supplement missed its subprocess-invoked verifier dependency;
extracted help caught it before dist promotion. Rejected ZIP retained, corrected
9module archive includes explicit executable-script dependencies. Four freshly
extracted entry points and both full extracted tests now pass. ZIP CRC/memberSHA/
dist exact copy verified. Supplement SHA 39dd4b69a964c5fbcbb87436ce2ee53a0870d6396d4d52b6136b42ec34767915.
Full receipt build/texture-unicode-trial-readers-20261023/distribution-verification.json.
Dev15 remains latest private app; previous artifacts retained. No GUI/game/GPU/
public release. Continue wrapper/streaming JSON reader review; these paths are
not covered by the two-reader fix. Desktop relocation still pending.

## Dev15 streaming-reader supplement complete

Streaming audit and alpha staging JSON reads now use UTF-8/BOM explicitly.
Baseline proved silent mojibake source identities in otherwise successful audit
and stage reports. Actual dev15 export -> deterministic NN stress generation ->
reference-parser audit/alpha staging passes UTF-8/BOM with exact Unicode source,
4096 alpha bytes, logical offsets/sizes, adjustment+padding block, char reference
and odd-scale metadata. NativeImportSupported/gameTested/complete2xImportSupported
remain false. Overwrite and overlap refuse; inputs/reference sources stay exact.
Existing alpha staging exact-pixel/native-cell selection and3 refusal checks pass.
Only two reader files changed; app/runtime and binary layout logic untouched.

Private13module supplement includes three SHA-pinned local reference Python
parsers and fixed inventory generator dependency. ZIP CRC/memberSHA/dist copy,
four extracted entry points and actual fully extracted integration pass using
bundled reference sources. Supplement SHA 103e8c896e90e4fe91e5fe5e3a63daf2bee56aa3fd4aad7a6a656a0c73db3da7.
Full receipt build/texture-unicode-streaming-tools-20261023/distribution-verification.json.
No game/GUI/GPU/native font import/public release. Dev15 remains latest private
app. Continue integrated wrapper JSON/real CLI workflow; no complete wrapper
acceptance claim from reader/staging tests. Desktop relocation pending.

## Integrated wrapper Unicode/BOM validation in progress

Wrapper baseline with a real exported/generated five-image trial refuses BOM JSON
before native commands/output creation (texture-wrapper-1nj8j9kt). Only
import_texture_upscale_trial JSON reads changed to UTF-8/BOM, including captured
native JSON logs and project manifests. Other reader/preparer fixes are separately
verified supplements. No native-writer or coverage-logic change.

Building real forge/forge-tools in private CLI snapshot over exact dev15 app core:
build/texture-wrapper-cli-20261023. Copies six CLI frontend files plus private
CMake target extension, with371file manifest/source delta; no other lane build or
library borrowed. Build remains in progress. New integration fixture contains two
ordinary textures, one static font, one Unicode loose BMP and one streaming page.
Expected real match/review/pack/composition imports4 and leaves streaming1
explicitly pending. Roundtrip pixels, metadata identities, streaming raw bytes,
source/input pins and UTF-8/BOM cases will be checked. No game/native streaming
import acceptance. Dev15 remains latest private app; no GUI/GPU/public release.

## Dev15 real-CLI integrated wrapper supplement complete

Isolated forge/forge-tools build passes over371source files: frozen dev15 app core
plus six CLI frontends/private target extension. Exact binaries/native sources,
build/source/dependency receipts included; no other lane build borrowed.
Wrapper reads UTF-8/BOM JSON and captured native JSON explicitly. Real pipeline
exposed unchanged game.bin recompression and names-header normalization during
texture-only compose. Independent fixture decompression showed game data exact,
composer reported zero definition edits. Wrapper now pins definition inputs before
native commands and retains original bytes for present passthrough outputs after
strict complete no-edit-report validation. Unknown/missing counters, reported edits,
stale inputs or invalid destinations refuse before copy. No definition-writer change.

Sixteen passthrough controls pass. Actual UTF-8/BOM export/generation -> match ->
prepare -> review -> pack -> compose -> native static-atlas import -> loose BMP
import passes:4of5 stress images imported, streaming1 explicitly pending. Pinned
independent Python LZO port verifies native mip pixels; Pillow verifies embedded
TGA/BMP pixels. Unrelated streaming payloads/all entry metadata except physical
offsets remain exact. Source/collection/trial and definition passthrough bytes
exact. Real integrated verifier accepts frozen independently checked output assets
and refuses a tampered output; no normalization remains. No native streaming
import/game compatibility/performance acceptance.

Private19module supplement includes exact tested static CLIs,371sources, reference
LZO decoder, notices, source deltas and evidence. Four freshly extracted entry
points,16guard checks and both full actual extracted pipelines pass. ZIP CRC/member
SHA/dist copy exact. Supplement SHA 2770477004c29be1ea9d82721821cdaae6f2783b582d55f6de436ba74220467d.
Full receipt build/texture-wrapper-cli-20261023/distribution-verification.json.
App EXE remains dev15 1a05536d353f196659f431f3bcc7bd3e3dc09b62cb0712951fa7442bf6bb15b5.
Runtime DLLs unchanged; no GUI/game/GPU/public release. Earlier artifacts retained.
Continue offline static-font writer boundaries on matching CLIs. Desktop relocation
remains pending user reply; it does not block offline work.

## Private dev16 shared path app and corrected native-font toolkit complete

Latest verified app original-restore-dev16. Existing native-path conversion body
moved unchanged into forge/nativepath.hpp; collection imports the same function.
Only collection.hpp plus new header differ from frozen dev15 (366source files).
App/probe frontends rebuild against this lane own source-identical verified core;
reuse/library/source receipts retained. Current132 app/probe checks pass:
collection4/font7/terrain10/source-scan2/streaming1/DDS24/restore47/deep PNG27/
portable10. Seven shared path/idempotence controls pass. Extracted source compiles
and passes those7 controls; nine extracted validation entry points import/help.
Binary/source/validation ZIP CRC/memberSHA/dist copies exact. Binary ZIP
ed4ebc2585b0b798274ad587a7a7d2030992d33b18f413a879c8646b9cb95d7f; source ZIP
53c736ca4feaed0a8182b7b1156498d4fcf10a7d44c5cef13e2a3dc953de2fb7; validation ZIP
2deb548ccb8ef27c46dcadf431bb824ef6091a67136432299668191b758075a6.

Corrected native font CLI uses native I/O spelling plus physical handle-resolved
paths for PNG containment. Relative paths normalize before the extended prefix.
First candidate rejected for breaking valid images/../images input; retained and
never promoted. Final ordinary/deep archive checks pass eight24/32bit TGA
orientation cases each, byte-identical native/Python payloads, colored transparent
texels, logical UV/metric/opaque metadata/ID/footer retention, exact no-op and
normalized-input results,15 refusals per path including lexical/physical junction
escape and overwrite/source protection. Existing native font unit boundaries
pass against unchanged exact core. Actual UTF-8/BOM real-CLI wrapper plus verifier
passes with dev16, keeping definitions exact and streaming1of5 explicitly pending.

Private28module/372native-source toolkit includes exact tested static CLIs,
reference parsers, source delta and receipts. Four extracted entry points, actual
deep font matrix, both full wrapper fixtures and16definition guards pass after
extraction. Toolkit SHA 6b0384efbd796e80a7e3843f6470304b79ef67c59c2fb4041590955d5bfaa4ce.
Full receipts build/texture-shared-native-path-20261023/source-and-distribution-verification.json
and build/texture-native-font-paths-20261023b/distribution-verification.json.
Runtime DLLs unchanged. No GUI/game/GPU/public release/native streaming claim.
Earlier packages retained. Desktop relocation pending; offline work continues.

## Native font source identity guard verified

Requested duplicate source bank names or entry IDs now fail before publication;
unrelated duplicate IDs remain unchanged. Five identity cases pass, including a
valid edit followed by an ambiguous request, with inputs exact and staging clean.
Ordinary/deep eight-orientation native/Python comparisons and15 refusals per path
pass, as does the real dev16 UTF-8/BOM wrapper workflow and16 definition guards.
Extracted private toolkit reruns identity/deep/wrapper/guard checks successfully.
App/runtime unchanged. Toolkit SHA 5b734feedc65ff70c0aa71cf44fd6cda3f79ebdc5faef25bfe3f0e0214c105f1.
Receipt build/texture-native-font-identities-20261023/distribution-verification.json.

Next active investigation: owned MinGW rename fixture confirms filesystem::rename
can replace an existing file. Investigating exclusive publication for native font
output when another writer creates the destination after the initial check.
No game/GUI/GPU launch; Desktop relocation remains pending.

## Exclusive native font publication verified

Font import now publishes a closed staged file with forge::publishNewFile:
Windows MoveFileExW flags0 refuses an existing destination atomically. The initial
existence guard remains for early feedback. Source-linked actual import function
with only a deterministic late-writer hook reproduces old sentinel replacement
and passes six corrected ordinary/deep Unicode normal/file/directory cases.
Inputs exact and temporary directories clean. Native POSIX hard-link fallback
is source included but has not been exercised on a native POSIX host.
Official API: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-movefileexw

Full targeted CLI rebuild passes five identity checks, ordinary/deep font payload
matrices and15 refusals each, real UTF-8/BOM wrapper and16definition guards.
Extracted packet reruns identity/deep/wrapper/guard checks. App/runtime unchanged.
Private30module/373source toolkit SHA
3216feb041fded4ba04aea54e27302208fa4e690489d2592e26fc4eab53cec2d.
Receipt build/texture-native-font-publication-20261023/distribution-verification.json.
Race evidence build/font-publication-uvxp5b98/verification.json and old
build/font-publication-go4ipqe2/verification.json.

Next active work: native terrain importer still uses filesystem::canonical and
rename. Deep path refusal reproduced, but original export-only fixture was not a
valid writer input. Constructed independent valid common-record/background-tree
fixture; ordinary native import passes at build/terrain-native-valid-f1e0u1_z.
Will use this valid control before changing terrain path/publication behavior.
No game/GUI/GPU launch; no new app release; Desktop relocation pending.

## Native terrain paths, containment and publication verified

Native terrain import now uses the same shared native I/O and handle-resolved
physical PNG containment as font import, plus exclusive publication. Baseline
proved deep valid-input failure, junction PNG escape, and late-destination
replacement. Current28 ordinary/deep checks pass: independent constant-green BC1
pixels/LZO framing, exact common records/entry metadata/opaque storage, normalized
relative input, source/PNG pins, unsigned/unknown/duplicate/empty requests,
existing output, lexical and owned-junction escapes,256 extent opt-in and refusal
without opt-in. Extent check is offline storage-only, no runtime acceptance.
Source-linked late-writer controls pass for both font and terrain (12 total).
Native font ordinary/deep matrices, five identity checks and real UTF-8/BOM
wrapper regressions pass on this exact targeted CLI rebuild.

Private31module/373source toolkit SHA
649e7b31c5d69eed0f2a4b4dd6183c8e2b11790a58320b8e0d89b6a651efe59d.
Extracted terrain28, identity5, font deep matrix, full wrapper and16guards pass.
Receipt build/texture-native-terrain-paths-20261023/distribution-verification.json.
App remains dev16, runtime DLLs unchanged. Earlier kits retained.

Next active work: standalone native image encode-dds ordinary8x8 control succeeds;
deep-path input cannot open. Baseline build/native-dds-path-baseline-zzuxrv82.
Will validate the shared native-path and exclusive-publication boundary there.
No game/GUI/GPU launch; Desktop relocation still pending.

## Standalone DDS boundary work active

Standalone native image encode-dds still used ordinary absolute paths and
replacement-capable rename. Ordinary valid fixture succeeds; deep input fails.
Source-linked actual dispatch block reproduces a late destination overwrite.
Current correction applies shared native path conversion to input/output and
publishNewFile to the final staged DDS. Six normal/late-file/late-directory
ordinary/deep Unicode controls pass. Exact isolated CLI frontend build running:
build/texture-native-dds-paths-20261023. Native font/terrain toolkit649e7b31...
remains latest verified supplement until this next candidate passes acceptance.
Pending native DDS matrix covers independently decoded DXT1/DXT3/DXT5/ARGB,
complete mip lengths, cutout alpha, explicit native paths and mixed-depth I/O.
No game/GUI/GPU launch.

## Native DDS path/publication toolkit verified

Standalone DDS now uses native input/output and exclusive publication.36 checks
pass ordinary/deep Unicode, explicit native paths and mixed-depth input/output,
independent Pillow DXT1/DXT3/DXT5/ARGB pixels, exact mip lengths and cutout alpha,
plus invalid format/mips/image and existing-output refusals. Six actual-dispatch
late-writer controls pass. Font/terrain functions and helper byte-identical to
previous accepted implementation;28 terrain,5identity,ordinary/deep font matrices
and full shallow UTF-8/BOM wrapper pass on current binaries. Extracted kit reruns
DDS36/terrain28/identity5/fontdeep/fullwrapper/16guards. Private32module/373source
kit SHA bd9a7323932a34b6eb9ec31806e0296ed314b4d224d0e37912b77220e74947e7. App/runtime unchanged.
Receipt build/texture-native-dds-paths-20261023/distribution-verification.json.

Deep full-wrapper experiment then exposed replacement matching ordinary-path I/O
and MinGW recursive iteration enumerating CWD, safely rejected by alias guard.
First candidate rejected/unpromoted: build/texture-wrapper-deep-paths-20261023.
Current b candidate uses native Windows enumeration with RAII, cancellation,
reparse/alias refusal, file limits and final iteration-error checking; ordinary
paths still drive identity/fingerprints. CLI normalizes joined native paths.
Build active build/texture-wrapper-deep-paths-20261023b. Only two sources change:
src/texturereplacements.cpp and src/cli/textures.cpp. Targeted object rebuild/
owned archive member replacement; all unrelated frontend/core inputs pinned.
No game/GUI/GPU launch; no Desktop promotion.

## Deep wrapper progresses through full composition; broader regression audit active

c candidate: matching/preparation/export/review/subset pack update passes ordinary
and deep Unicode paths. Existing test_texture_replacements now accepts explicit
--forge and --deep-paths; actual deep checks pass all original ambiguity/stale/
ownership/invalid image controls.10 source-linked replacement scan controls pass
early/late cancellation and first/next errors with exact closed-search counts and
stable process handle counts (exception runtime warmed before measurement).

d candidate: complete UTF-8 and BOM deep wrapper passes with independent native
mip/TGA/BMP and verifier checks, definitions and unrelated streaming records exact.
Receipt under build/texture-wrapper-48yqjuij/owned-deep-*/verification.json.
Native composer scanner replaces broken extended-path CWD iteration; physical
overlap/output containment checks use native handle resolution. Native joins are
normalized. Six offline lock/collision/retry checks pass, with inert running-
destination helper explicitly skipped (no process/game launch).
25 core archive members independently extracted/audited: only textureproject.cpp.obj
and texturereplacements.cpp.obj changed; core slice and23 other members exact.
MinGW ar p stdout adds CRLF: audits use ar x real bytes instead.

d remains unpromoted: generic shallow TNG source regression from automatically
adding extended prefixes to every composition source. e narrows conversion to
deep or explicitly extended inputs; ordinary generic sources keep their spelling.
e exact source build active build/texture-wrapper-deep-paths-20261023e.
Ten actual composer helper controls pass ordinary/deep normal/first/next/second-
folder errors and owned-junction refusal, exact relative paths and closed handles.
Receipt build/mod-native-scan-_w2zvqmu/verification.json. No final deep workflow
package promoted yet; latest accepted toolkit remains native-dds bd9a7323...
App stays dev16, runtime DLLs unchanged; no GUI/game/GPU/Desktop promotion.

## Deep offline texture workflow verified and packaged

e candidate resolves d shallow generic TNG regression by retaining ordinary
spelling for ordinary composition sources. Full shallow and deep UTF-8/BOM
workflows pass real matching/preparation/review/pack/compose/static-font/BMP,
independent native mip/TGA/BMP pixels, retained streaming metadata, exact source
definitions and actual verifier/tamper refusal. Streaming1of5 still unsupported.
Original subset-ownership/matching tests pass ordinary/deep with added owned
junction refusal. Ten replacement scan and ten composer scan controls pass.
58 generic ownership/cleanup/rollback/overlap cases and6 offline publication
lock/collision/retry cases pass. Inert running-destination helper skipped.
Standalone DDS36/terrain28/fontidentity5/font matrices15refusals per path pass.
Extracted packet repeats full deep and shallow workflows, deep subset checks,
58ownership/6publication/DDS36/terrain28/fontidentity/deepfont/16definition guards.
Private46module/373native-source toolkit SHA 3d84277d7cec86ecaed4d1d19c3395f90b6b78255ebd474bbb3fe7cdae75e383.
Receipt build/texture-wrapper-deep-paths-20261023e/distribution-verification.json.
App/runtime remain dev16 unchanged. Four source files differ from accepted DDS
kit; archive audit verifies only2of25 core members changed,23others+core slice
exact. All earlier rejected candidates retained and unpromoted.

Next active offline investigation: native specialized font/terrain imports hash
selected PNGs before decoding but do not recheck them during/final import. Will
use a source-linked controlled mutation to verify whether concurrent image
replacement can publish pixels that disagree with manifest pins before changing
behavior. No GUI/game/GPU/Desktop promotion; Desktop destination unanswered.
