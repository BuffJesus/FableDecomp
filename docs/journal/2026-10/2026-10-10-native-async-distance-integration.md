# Native async iterator distance integration

Added the original STLport input-iterator distance declaration in
`rebuild/include/fable_stlport_iterator_distance.h` and its async-pointer
explicit instantiation in
`rebuild/src/compiled/00/9d/STLAsyncPointerTree_Distance_009d7550.cpp`.
No existing tree signatures or callers were migrated. Lua work is excluded.

Freshness preflight checked 1,888 prior pinned inputs with no drift and an
unchanged first scratch object. The separately named canonical first compile
uses VC7.1 `/O2 /MT`: 78 bytes, 35 instructions, no relocations, whole-body
retail equality. The typed caller requires the correct mangled provider;
omission fails and the full control links successfully.

Fresh canonical behavior execution passes 1,112 cases per mode (retail and
compiled separately): independent sorted interval lengths, empty/singleton/
balanced/skew shapes, unsigned high-bit keys, two placements, arena/stack
fills, equal-input aliasing, cdecl stack cleanup and callee saves. All 32 KiB
arena snapshots remain unchanged. This does not prove deletion or whole-engine
startup.

Receipts and immutable first objects:
`work/parity_addtext/async_distance_canonical_first_compile_20261010/` and
`work/parity_addtext/async_distance_canonical_behavior_20261010.json`.

Resume: implement a reusable canonical integration gate and wire its verified
receipt into the symbol-index acceptance path and build catalog. Catalog,
acceptance routing and coverage credit remain pending; source presence alone
is not counted as accepted coverage. Do not rerun immutable first scripts or
retune this passing exact body. Broader historical tree-header migration stays
separate, with capped startup and Lua contexts excluded as before.

## Reusable canonical integration test

Added `rebuild/tests/integration/NativeAsyncDistance_test.py` and its typed
required caller. Unlike the earlier scratch test, this route has no scratch
inventory dependency: it uses the canonical COFF parser, compiles current
canonical sources into a new temporary directory, verifies complete exact
retail bytes and provider omission/control links, and executes the same
independent 1,112-case interval model per mode. It pins headers, toolchain,
source and oracle inputs before/after execution and checks object hashes.
First execution passed; receipt:
`C:/Users/Cornelio/AppData/Local/Temp/native-async-distance-u9jdqu3n/result.json`.
The subsequent source-only guard/boundary-text correction still needs its
own fresh run before acceptance wiring. Catalog/index changes remain pending.

## Fresh acceptance and index route

The guard-corrected test passed separately with receipt in temporary directory
`native-async-distance-7eibm_yw`. Added
`tools/decomp_pipeline/gate_native_async_distance.py`: it runs the current
canonical test, validates its status, exact identity, 1,112 entries per mode,
link controls, input freshness and preserved object hashes.

Registered a dedicated symbol-index recipe with full `/nologo /c /O2 /MT`
flags, excluding inherited `/Oy /GS`. Indexing this source runs the fresh
acceptance gate and independently verifies the indexer's own object against
the complete retail body, then admits only the verified distance root.
Directly exercised `catalog()` plus `one()` in a new temporary directory:
exactly one matching recipe and one COMDAT provider row at `009d7550` passed.
Build directory: `distance-index-route-y6yn8i66`. This diagnostic did not rewrite
the existing symbol-index TSV or other providers.

Resume: safely publish the verified provider into the existing generated index
without dropping unrelated rows, and reconcile any coverage/catalog reporting
through the established generators. No global index refresh or native startup
retry has run; whole-engine visual launch remains unproven.

## Provider publication and closure compilation

Ran fresh acceptance successfully (`native-async-distance-gl45y5vf` receipt)
and added only the verified distance provider row to the generated index via
a targeted patch. Removing that new row reproduces all 36,787 original index
lines exactly in their original order; normalized-text SHA256 remains
`e703c56d85c964c952d4db8e43efdb81b10fc088ea57abf9cfe1d2495f2cbfea`.
The closure reader now resolves the exact symbol to the canonical TU.

Added the matching full `/nologo /c /O2 /MT` profile to `link_closure.flags_for`
for this dedicated provider. Compiling through the actual closure compile
function in a new temporary directory again produced all 78 retail bytes
exactly, with offset zero and no relocations. No existing object was removed.
This is provider/closure availability, not a complete game link or execution.
Leaf catalog/dashboard reporting remains separate and has not been inflated.

Resume: use this canonical provider in the faithful async key-erase dependency
chain, keeping the wider const-bound tree-header migration and capped native
startup work explicit. Existing exact/behavior acceptance requires no tuning.

## Canonical distance in full compiled deletion composition

Added and ran the separately named
`work/parity_addtext/async_keyerase_canonical_distance_chain_20261010.py`.
It freshly compiles the canonical distance TU under `/O2 /MT`, verifies its
whole-body retail equality and current input pins, and substitutes that object
into the existing ten-body/seven-object compiled key-erase composition.
All other providers remain their preserved evidence objects; this is explicitly
not wholesale canonical migration.

Result: `CANONICAL_DISTANCE_COMPILED_KEYERASE_CHAIN_PASS`, 6,000 cases and
15,000 full-arena snapshots per mode, with 3,000 present and 3,000 absent keys.
The original and compiled chains have identical counts across key erase,
equal range, distance, range erase, recursive clear, rebalance, extrema and
rotations. Independent valid-RB construction/physical deletion checks include
raw count mutation and freed-link poisoning. The only substituted execution
boundary is cdecl free. Receipt is the script's separately named `.json`.

Resume: genuine seven-provider omission/control links using the fresh canonical
distance object, then resolve the remaining historical equal-range/key-erase
header ownership before canonical promotion. Existing wrapper NEAR_MATCH and
exact-byte mismatch remain unchanged; do not retune passing code.

## Seven-provider omission/control result

Ran `async_keyerase_canonical_distance_provider_links_20261010.py` with the
exact fresh canonical distance object from the preceding composition receipt.
All seven genuine provider omissions fail with the required missing-symbol
diagnostics, while the complete graph links successfully. Input and object
hashes remain unchanged; no link executable was run. Receipt:
`work/parity_addtext/async_keyerase_canonical_distance_provider_links_20261010/result.json`.

Inspected current tree declarations against the preserved exact equal-range
source: canonical `_M_lower_bound` still returns `_Base_ptr` and is non-const;
the historical source requires const typed `_Link_type` lower/upper bounds.
This remaining declaration/ownership migration is real, not solved by the
distance provider. The key wrapper uses historical category dispatch, also
not yet a canonical shared-header definition. Resume on these source-level
dependencies; keep the passing compiled wrapper body and prior global-header
consumer audit intact rather than fitting instructions or changing algorithms.

## Canonical historical iterator dispatch

Added the five historical iterator typedefs to the canonical tree iterators
(category/difference in the base; value/pointer/reference in the derived
iterator). These add no storage or changed method signatures. Added generic
class-iterator traits and const-reference category dispatch to the canonical
distance header, matching the preserved historical dispatch source. Pointer
specializations and random-access distance remain outside this reconstruction.

Extended the canonical required caller to invoke category dispatch and assert
the iterator remains four bytes. Fresh acceptance passed, retaining exact
78-byte distance code and all 1,112 cases per mode (`native-async-distance-mwrkqggy`).
Also compiled existing algorithm pointer traits together with the new class
traits: both type checks passed and class dispatch requests the exact canonical
distance symbol (`async-distance-traits-mg9crl5g`). Existing pointer-traits
specialization is retained, not replaced or expanded.

Resume: verify reverse include order and representative existing iterator
consumers before treating the additive typedef surface as broadly compatible.
Const typed lower/upper bounds, key-erase declarations and equal-range ownership
are still pending; no whole-tree migration or additional leaf credit is claimed.

## Existing native-consumer regression

Reverse include order also passes under VC7.1: distance header before algorithm
header, existing pointer specialization plus class traits, four-byte iterator,
and the same exact distance provider request. Preserved source:
`async_distance_traits_reverse_order_20261010.cpp`; object directory:
`async-distance-reverse-traits-reylhsi0`.

Ran the existing canonical balanced-registration gate against the current
headers: `NATIVE_ASYNC_BALANCED_REGISTRATION_GATE_PASS 4128 10752`. This freshly
compiles the five-source registration inventory and executes original/compiled
registration with independent RB checks, mandatory provider omissions,
duplicate handling and allocation-failure checks. The gate's normal generated
receipt is refreshed; earlier scratch first objects remain untouched. This is
regression evidence for these consumers, not all tree users or new coverage.

Inspected the frozen historical migration header and both adapted lower-bound
definitions again. Promoting those still changes lower-bound symbol identity
for string/checksum callers; additive iterator metadata alone does not resolve
that owner-graph obligation. Preserve the separate migration audit scope.

## User-requested session stop

Before stopping, fresh existing range-erase gate passed: baseline 4,448 cases /
16,648 snapshots and supplement 80 cases / 704 snapshots, with seven exact
relocation-matched providers, four source omissions and current input hashes.
No further reconstruction changes after the stop request. HANDOFF and ROADMAP
now record the canonical distance/dispatch checkpoint, regression evidence and
remaining const-bound/provider ownership task. Goal paused, not completed;
no processes remain running from this pass and no commit/push was performed.
