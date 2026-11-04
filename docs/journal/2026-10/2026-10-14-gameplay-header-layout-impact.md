# Gameplay-header seam: compiler layout constants

The [direct-caller audit](2026-10-14-gameplay-header-consumer-impact.md) supplies
123 included syntax passes and an unapplied five-file proposal. This continuation
adds compiler-emitted layout evidence while preserving all earlier inputs.

## Actual layout comparison

The original and staged headers produce identical **479 constants / 1,916 bytes**:

- Sizes and alignment queries for seventeen game-header classes.
- Offsets, member-expression sizes and scalar/array-element categories for all
  135 explicit fields extracted from those class declarations.
- The same properties for the original integer and floating rectangle types,
  including all unsigned coordinate names and signed aliases.

For example, `CMainGameComponent` remains 90,600 bytes with alignment 8 and
`CNewFrontendGameComponent` remains 328 bytes with alignment 8. Both rectangle
types remain 16 bytes with alignment 4. Integer `TLX/TLY/BRX/BRY` produce the
unsigned tag; `left/top/right/bottom` produce the signed tag. The table has no
relocations. No object construction, linker call or native execution occurs.

The local probes add friend declarations to access private/protected members.
Removing those additions yields the exact input header text. Access groups,
fields, methods, inheritance and declaration order remain unchanged. These
instrumented overlays are not production edits or canonical headers.

## Preserved probe failures and qualifications

The first layout probe failed because VC7.1 rejects `__alignof(CGameComponent)`
for the abstract base. A primitive query and a same-size concrete wrapper query
parse successfully; the dereferenced-expression form is unsupported. The revised
probe overrides only the four existing pure slots in a wrapper with no new fields
and a size-equality assertion. Its alignment comparison is explicitly **wrapper
alignment**, not independent direct proof of the abstract base alignment.

The next probes compiled but rejected their own initial code-free-object
assumption: VC7.1 also emits a 48-byte compiler array-construction iterator
(`??_H@YGXPAXIHP6EPAX0@Z@Z`). Replacing standard-library numeric traits with
explicit builtin traits did not remove that helper. These failed verifier
assertions and compiled objects remain preserved. No claim of literally
code-free objects is made.

A subsequent read-only preflight rejected an overly broad no-reference assertion.
Inspection found precisely one helper reference in each object: a 16-byte
`.debug$F` FPO record with an offset-zero SECREL relocation. The amended decoder
records that exact debug reference and requires no runtime-section references.
It does not apply a general symbol or debug normalization rule.

The final comparison reuses the successful original compilation from the
builtin-trait attempt instead of rebuilding it. The staged probe is compiled
once. Both complete compiler-helper bodies match; no engine function body is
emitted and nothing is linked or executed. Newly observed original source/object
hashes are postcreation observations, not retroactively inserted original before
guards. The source overlays are independently reconstructed from the recorded
immutable header/helper inputs and checked byte-identical.

Two parser tests cover forward declarations, comments/literals, access labels,
inline methods, template commas, arrays and exact friend-overlay reversal. The
initial Python import failure was corrected before any layout capsule was frozen.

## Saved result

Packet:
`work/modern/header-seams/20261014/game-header-display-geometry-amended2/layout-constants-helper-qualified-amended1/result.json`.
SHA256:
`769ddc88c6b73bb4ca70566177b57c6b981151adb1fef2b90b25d629cf8d430f`.

Status:
`ORIGINAL_STAGED_LAYOUT_CONSTANTS_EQUAL_WITH_COMPILER_HELPER_QUALIFICATION`.
One newly compiled probe, one retained original probe, identical 479-value
tables, identical complete 48-byte incidental helpers, zero observed input drift,
zero engine-provider recompiles and zero linker/native invocations.

This is a qualified layout fingerprint, not complete field-type identity,
vtable/base-subobject offsets, class construction/lifetime, full historical
profile equality, runtime behavior, retail parity or x64 compatibility. The
abstract base alignment limitation remains explicit. Game-function code-generation
and required behavior/consumer closure checks still precede promotion.

The native frontend/window closure remains unrecorded in its active owner's
directory; its passing fixture was not restarted. The pending menu composition
and existing capped work remain unchanged. Canonical source/header/index/grades,
dashboard, existing native receipts, HANDOFF and ROADMAP are unchanged. No
original-game/GPU launch, Lua work, commit or push occurred.
