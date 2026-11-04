# Integer viewport genuine C++ candidate and frozen behavior packet

The work-only endpoint candidate at retail `009BEF80` compiles as genuine C++,
alongside the existing genuine shader dirty-bit notification and a C++ empty
notification whose retail target `00A0AAC0` is one RET byte. The legacy naked
integer and float viewport implementations remain excluded from providers.

Complete retail endpoint: 479 bytes, 145 instructions. Candidate: 441 bytes,
134 instructions; corrected own-operand normalization yields 75.9857% opcode
similarity. This is below the 90% threshold and does not qualify for near-match,
reconstruction credit or promotion. Behavioral comparison is independently useful
for the explicit clamp, wrap, offscreen, device and shader boundary.

Independent inspection caught an omitted original operand: the scoped-pointer
boolean conversion tests the getter's address token at root offset 448. The
first inventory and unexecuted fixture remain preserved. The corrected inventory
classifies that token, global pointer and two direct calls. The retail PE has no
base-relocation entries; completeness is semantic/full-decode evidence, not a
HIGHLOW table claim. The first corrected inventory attempt also explicitly
rejected assuming such a table. Retail getter `009C1220` is `8B 01 C3`.

The first fixture compilation failed C4234 because VC7.1 reserves free-function
`__thiscall`. An amended fixture uses a typed fastcall adapter with unused EDX,
ECX receiver and one stack argument. A second fresh amendment binds the boolean
token using the actual current getter member pointer, statically asserted four
bytes. No viewport native invocation has occurred.

Evidence under `work/native-recreation/rendering/009bef80/20261014/`:

- Corrected original inventory: `f201392b2701d496ba56b4fd5c7ddc4ca475af76568539c5ec73d9400481f055`.
- Candidate build: `076b1347a889e5aac896a7b700f1e8574dfcda739bb23b94d2edf617e8b4caef`.
- Fresh amended2 fixture build/link: `48704cec6248140d4fe140bc45822749f7a6a2ddfe565554b522f18950b25600`.
- Complete linked section proof: `133d20dbc5be0129b959677e69783f21f162213211e057285562d5c6b653b917`.
- Frozen review manifest: `b8ed26e0860bea4ef5d7af03654520e889c0e9abfe038ec10c4c278fdc7d0ace`.

The planned finite comparison has 147,456 cases and 294,912 side snapshots:
twelve masked x87 modes, six dimension pairs, sixteen boxes, four prior boxes,
two HRESULTs, four callback mutation modes, two render-pointer states and two
input locations. Complete arena/global/device observations and abstract CRT FP
status/control endpoints compare, with independent per-side outside-write guards.

Ten unique linked code spans, eleven object-section comparisons and four
initialized data sections are checked. The identical three-byte getter COMDAT
is contributed by both caller and provider. Private data locations derive from
actual operand destinations then cross-check payloads/references; this remains
qualified linked self-consistency. BSS is excluded from file payload proof.

Independent whole-source review is required before first execution. All source,
object and tool inputs remain pinned. Borrowed storage lacks constructors and
lifetimes; device slot 47 is a mock; empty notification calls have no live trace.
Different intermediate store ordering and unmasked exceptions are unproved.
No GPU/game launch, canonical changes, coverage, saves/playthrough or x64 claim.

The separate independent readonly findings receipt is
`candidate-independent-readonly-first/independent_candidate_static_findings.json`,
SHA256 `f5f1d218a9ca096f0981d4f748c97278d09612b6bbb3f5c8b3a4edbe2a65b826`.
It preserves the missing-token finding and independently verifies the correction,
retail getter bytes and callback/notification qualifications. Its source-derived
scalar models agree over 307,328 inputs; that is static model comparison, not
execution of either engine body or behavioral acceptance. Independent opcode
normalization also remains below threshold. Full-fixture review is separate.
