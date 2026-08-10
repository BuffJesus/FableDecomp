# The cut co-op / multiplayer stuff still sitting in Fable TLC's exe

> **Heads up on authorship:** I want to be upfront — I didn't do this work. The whole
> thing (decompiling the functions, the analysis, and this write-up) was done by an AI
> assistant (Claude) as part of a Fable RE project. I'm just passing it along because it
> answers your question. The findings and addresses are real and mostly byte-verified
> against retail `Fable.exe`, but none of it is my own work and I don't want to take
> credit for it. Where something is community research or interpretation rather than a
> proven fact from the binary, it's marked as such.

So here's what's actually in there. Short version: it's a lot more than "some hints."
There are basically two separate leftovers tangled together — the **local co-op "Alter
Ego" system** (the Hero Spirit thing) and a **networked replication layer** underneath
it. The co-op side is mostly intact. The network side is scaffolded but the sync got
ripped out.

All the addresses below are from the retail Steam `Fable.exe`, ImageBase `0x400000`, so
you can open it in Ghidra/IDA and jump straight to them. Several of these functions were
recompiled and byte-matched against retail (a real parity pass, not eyeballing the
decompiler), so they're not guesses.

One thing up front, to avoid overselling: the write-up keeps "what the binary provably
does" separate from "what old interviews/community asset digs say" and from
"interpretation." Anything that's inference rather than a proven fact from the binary is
flagged.

## The history it lines up with

This part is *public-interview history*, not something read out of the binary — just
setting the stage. Molyneux talked this up back in 2003–04: up to four players on one
machine, a friend plugs in their save and their Hero drops into your world as an **"Alter
Ego,"** no restart. The Alter Ego could fight, gain XP, heal you, trade items. There was
a **score** for being a helpful partner, and the higher your score the easier it was to
take over as the main Hero. "Cooperative but a little bit competitive," his words.

Worth mentioning because a couple of those specific claims line up with things that are
actually in the code — the four slots and the score-triggers-takeover mechanic
especially, called out below. The bits that are *only* interview-sourced (drop-in save
import, healing, trading) have no matching code found, so treat those as history, not
confirmed.

## The "Hero Spirit" — CTCCoopSpirit

This is the class that represents the second player's body when they're not the main
Hero. In the files it's `CTCCoopSpirit` and `CCoopSpiritDef`. The Alter Ego exists as a
spirit bound to the main Hero (the code literally has it chase the Hero around — see
below). Key pieces:

- `CTCCoopSpirit::OnCreate` @ `0x006700F0` — sets up the spirit, zeroes its score, grabs
  its particle emitter and def. (Separately, community asset digs — the Avarice
  cut-content work from 2021, not verified in the binary here — turned up leftover
  multiplayer orb / coloured-particle / lightning-beam effects that would go with a
  spirit form. Flagging that as their finding, not part of this analysis.)
- `CTCCoopSpirit::UpdateAttractionToMaster` @ `0x006701A0` — the "spirit stays near the
  main Hero" logic. It measures the distance from the spirit to its master and, past a
  threshold, applies a physics acceleration pulling it back toward the Hero. That's all
  directly in the decompiled body — genuinely finished code, not a stub.
- `CTCCoopSpirit::UpdateScore` @ `0x00670710` — **the takeover mechanic.** The body
  checks `if (Score > 0x13)` (i.e. score above 19) and then calls `SwapToHero()`. The
  threshold and the call are both literally in the code. It lines up with what Molyneux
  described (score lets you seize the main Hero slot), but to be precise: the binary
  proves "score over 19 triggers a swap," it does *not* prove what raises the score —
  that part is only in the interviews.
- `CTCCoopSpirit::SwapToHero` @ `0x0066FF20` — actually performs the swap: reads the
  spirit's `HeroSlot` byte, calls into the player manager to switch who's driving, and
  repositions. All in the decompiled body.
- `CTCCoopSpirit::AddExperience` @ `0x0066FB00` — an XP-add path on the spirit.
  (Name/signature only; the callers weren't traced.)
- `CWorld::EAMoveSpirit` @ `0x0062C0E0` — a game-event handler that moves a spirit from a
  movement event's payload. The "EA" prefix is the engine's event-action convention, so
  this is event-driven.

There's also input plumbing for controlling one — `CInputProcessControlSpirit` (several
constructors around `0x004D7Dxx`) — and three co-op-specific HUD draw classes in the
disassembly: `CDrawSpiritScores`, `CDrawSpiritsHealth`, and `CDrawSpiritTargetingCursor`.
The class names are what they are, so at minimum a score display, per-spirit health, and
a targeting cursor were coded. (The community also reports leftover scoreboard/leaderboard
*UI assets*; only the code classes are vouched for here.)

## The 4-player manager

`CPlayerManager` handles the player slots. The ones that matter:

- `CPlayerManager::IsMultiplayerGameActive` @ `0x00449D20` — the "is multiplayer on?"
  check, and it's **real data-driven code, not `return false`**. It loops over 4 player
  slots (`slot 0..3`), finds a seated player that isn't the main Hero, and if one's
  active it returns true. So the four-player shape is baked in and the multiplayer-active
  predicate genuinely works.
- `CPlayerManager::GetMultiplayerColour` @ `0x00449B60` — returns a per-player colour, so
  players were meant to be visually distinguished by colour.
- `CPlayerManager_AddPlayerOrAI` @ `0x0044A1A0` — seats a new player (or an AI) into the
  slot vector and inits their character. Fact from the body: player number 4 is given no
  draw environment (`hasDrawEnvironment = playerNumber != 4`), while 0–3 get one.
  Inference (not proven): slot 4 was a non-rendered/AI-ish slot.

There are also `GetSpiritDefName`, `IsThingMainPlayer`,
`GetPlayerNumberFromJoystickDeviceNumber`, `IsPlayerAssociatedWithJoystickDeviceNumber` —
that last pair is straight-up "which controller is which player," i.e. couch co-op
controller assignment.

## The network layer — CNetworkClient

This is the part that would've taken it online, and it's the part that's half-gutted.

- `CNetworkClient::InitialiseAsLocal` @ `0x004AE940` — brings the client up. It hardcodes
  `Host=false`, `LocalGame=true` every single time, so in the shipped build it only ever
  comes up as a local session. It sets an internal enable byte at `+0x2662`.
- `CNetworkClient::Update` @ `0x004AE9D0` — the per-frame tick. First thing it does is
  check the enable byte; if it's 0 the whole function is a no-op. When it's on, it stashes
  the frame's timestamp and **two checksums** that are passed in as arguments. That much
  is straight from the body. Interpretation (not proven from this function alone): two
  per-frame checksums plus a frame counter is the shape of a checksum-based desync check
  — and the fact that `CheckSync` below reads exactly those three values backs that up,
  though it's short of definitively "lockstep."
- The client is embedded inside `CMainGameComponent` at `+0x13AB8` (that offset is
  opcode-proven from the call site), and there are matching `GetGameEventPackageSet` /
  `GetLocalGameEventPackageSet` methods (`0x004AEAA0` / `0x004AEBA0`) that build a package
  set — the names and return types say "collect this frame's events," which reads as the
  outbound-events step.
- The only `CNetworkConnection` method identified so far is `CNetworkConnection_Cleanup` @
  `0x008410D0` — so the connect/send/receive methods haven't turned up in what's been
  recovered, but "totally deleted from the exe" is stronger than what's actually
  confirmed. (For what it's worth the exe does still import Winsock — `__WSAFDIsSet` and
  friends are in the imports.)

## The replication protocol (this part is fully intact)

The wire format for game events is recoverable — both the pack and unpack sides are
present and are exact inverses of each other:

- `CGameEventPackageSet::CompressIntoBuffer` @ `0x009F19A0` packs a set of event packages
  into a byte buffer, and `InitFromCompressedBuffer` @ `0x009F1AC0` unpacks it. "Compress"
  is a bit of a misnomer — reading the body, it's dense byte-packing, no actual
  compression.
- Each individual `CGameEvent` record, from the pack loop: a `u16` header holding a
  15-bit event ID plus a high-bit flag, then a type byte, a payload-length byte, then the
  payload. One caution grounded in the object layout: the `CGameEvent` object is `0x28`
  bytes with the payload occupying 32 of them, and the deserializer does **no** bounds
  check on that length byte — so 32 is the safe ceiling even though the field can encode
  more. `AddEvent` / `AddPackage` / `Clear` are all present too.

So "what goes over the wire" is well recovered — it's a clean per-frame event stream.

## The gutted part — CheckSync

This is the smoking gun for "abandoned mid-build."

`CMainGameComponent::CheckSync` @ `0x004165E8` is supposed to be the desync detector. And
it *almost* is — it walks the events, and for each sync event it reads the three remote
values (checksum1, checksum2, frame) and grabs the local world checksum via
`PeekWorldChecksum`. And then... it throws all of it away. There's no compare, no `if
(mine != theirs)`, no desync flag, no error report. It reads everything it would need and
then does nothing with it. The comparison logic was stripped out, leaving a hollow
function.

That's exactly what you'd expect from a feature that was being actively torn down before
ship — the plumbing that gathers the data is still there, the decision that uses the data
is gone.

## Can you turn it on?

Not really — at least not the naive way. The grounded evidence here is the EgoMP prototype
(a separate community project that does get two independently-controlled player creatures
running): it does that by **bypassing** all of this retail machinery and bolting its own
SLikeNet networking on the side, and per its own notes it deliberately keeps
`IsMultiplayerGameActive` returning false because letting the residual native path
actually run crashes. So: the retail code is real and readable, and the enable gate is
understood, but "flip one flag and you've got co-op" is not it — enough got removed (the
`CheckSync` compare, the connection guts) that activating the leftover path isn't safe.
That crash is the documented prototype's finding, not a fresh live test here.

## TL;DR

- **Local co-op ("Alter Ego"/spirit) system:** substantial and mostly intact in code —
  spirit entity, attraction-to-master, XP path, score-over-19 takeover, role swap,
  controller-to-player assignment, co-op HUD classes (scores/health/targeting).
  `CTCCoopSpirit` @ `0x00670050`+, `CPlayerManager` @ `0x00449xxx`. *(binary-verified)*
- **4-player support:** the slot loop is baked into the player manager, not a leftover
  constant. *(binary-verified)*
- **Network client:** scaffolding present (`CNetworkClient` @ `0x004AE940`+), forced to
  local-only in the shipped build; the connection class is mostly absent from what's been
  recovered. *(binary-verified for the client; "connection deleted" is only "not found
  yet")*
- **Replication protocol:** recovered — pack (`CompressIntoBuffer` @ `0x009F19A0`) and
  unpack (`InitFromCompressedBuffer` @ `0x009F1AC0`) are present and inverse.
  *(binary-verified)*
- **Sync/desync check:** gutted (`CheckSync` @ `0x004165E8`) — it reads the three sync
  values and the local checksum, then compares nothing. *(binary-verified via dataflow)*
- **Turning it on:** documented to crash when the residual native path is activated.
  *(from the EgoMP prototype, not a fresh test)*

Happy to share the actual decompiled `IsMultiplayerGameActive` and `UpdateScore` — the
score>19→SwapToHero one is the most satisfying because it's the exact mechanic Molyneux
described sitting right there in the binary.
