# Conversation-line operand audit ? September 12, 2026

The bounded offline audit found no additional operand mismatch. All 36 native
AddLineToConversation calls pass false for the line boolean; all 27 Lua call
expressions forward false through the actual runtime overloads/default. This
boolean is named showSubtitle in Lua but bUnknown in the native typedef. Its
perceptual meaning is not established by this argument-preservation audit.

## Native evidence

`tools/script_recovery/audit_new_oakvale_conversation_lines.py` builds on the
reviewed entity CFGs and archives all 36 call slices, reverse push operands and
context. Villager's two concatenated text arguments require accounting for the
string constructor's eight-byte cleanup and concatenation's four-byte cleanup;
counting the last five pushes blindly gives a wrong result. Affair Man's shared
second call DB19D8 has two participant prefixes joining before the common false,
text and conversation-ID arguments. Both are explicitly recorded; other
unreviewed alternate entries are rejected.

Native-conversation-line-operands-20260912.json contains the slices. Bound actor
register reaching definitions are independently checked at all 36 calls in
conversation-bound-aliases-20260912.json. Bully's bound pointer is saved from
entity+8 through ESI into stack+50 (DBB31C/DBB339), then restored to EDI. It is
not inferred from a dialogue key's actor prefix.

Participant review against the Lua source:

| Native entity | Speaker ? listener |
|---|---|
| Affair Man | Woman ? me and me ? Woman; first pair reverses at index 10 |
| Affair Wife | me ? Hero, empty listener or Man; Man ? me for the optional reply |
| Barrel Man | me ? empty listener for both queued lines |
| Barrel Thug | me ? Hero for all eleven tier branches |
| Book Trader | me ? Hero |
| Bully | me ? Victim, including the VICTIM_REVENGE text key |
| Guard | me ? Hero |
| Teddy Girl | me ? Hero |
| Victim | me ? Bully for both queued lines |
| Villager | me ? Hero |

Non-bound lookup identities are a manual review of native setup/operand locals
and the existing entity inventories, not symbolic execution of every actor lookup.
The surprising Bully/Victim text prefixes do not justify swapping speakers.
Four native near-conversation executions validate both Affair Man orderings and
both early-cancellation branches (native-affair-line-order-20260912.json). The
current Lua order matches the normal branches; this test does not certify its
whole conversation lifecycle or asynchronous cancellation policy.

## Absent-listener ABI

Eight native executions of CGSI AddLine at 890750 verify speaker/listener
predicates and the exact five arguments forwarded to 6E5950. An invalid speaker
queues nothing. An empty listener object forwards listener data zero. Passing a
null C++ listener pointer instead faults at 89076C, reproducing the reason for
the existing empty-CScriptThing wrapper. Evidence:
native-conversation-line-api-20260912.json. Predicates, thing-data getters and the
queue manager are doubles; no actual queue playback or sound is executed.

Barrel Man's absent-listener constructor 6E7B40 sets vtable 1238C8C and zeros data
and ref-info. Wife's running-to-husband path constructs the same empty value
inline at DB3528..DB3555. The current Lua nil handling preserves that contract.

## Actual runtime conversion checks

`work/runtime-conversation-lines-final-20260912/result.json` passed from a fresh
build directory. The test compiles the current GetScriptThingFromSolObject helper,
AddLineToConversation body and registration verbatim with vendor sol/Lua. All 27
source expressions preserve speaker/listener identity, text and false line flag.
Borrowed me, owned Hero/partner, empty nil listener, explicit true, explicit nil
and invalid nil speaker are covered. Dynamic text inputs are fixtures; this
checks forwarding, not authored text selection or branch coverage.

The same clean run also passes all direct Speak expressions/method variants,
17 conversation booleans, shared Maze state/lifetime checks, resource ownership
checks and 14 Meet Sister traces (255 events). Initial harness attempts failed
on a local variable shadowing and an uninitialized test-only text index; both
are corrected and their failed reports retained. No runtime/source port edit
was needed for this audit. The installed v23 build remains untouched.

Remaining audio work is authored macro speech operand review and a real capture
of the affected line/listener/emitter. No loudness, attenuation, physical output
routing or fade visibility fix is claimed by these offline results.
