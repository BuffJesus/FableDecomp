# Lua Gameflow parity and authority audit

## Status

Aeon's downloaded `LUAGameflow.lua` is a structurally strong port, but it is **not safe to deploy as
a second active quest beside retail `Gameflow`**.  This is an evidence gate, not a precaution based
on speculation.

The deterministic audit is:

```powershell
python tools/script_recovery/audit_lua_gameflow.py `
  --lua work/aeon_lua_ports/Gameflow/FSE/LUAGameflow/LUAGameflow.lua `
  --native-cluster refs/script_recovery/native_clusters/Gameflow.json `
  --output work/aeon_lua_ports/Gameflow/gameflow_native_audit.json
```

## Proven stage/resume contract

The retail `CGameflowScript::Main` decompile writes 35 distinct values to
`CQ_SunnyvaleMasterData::PostSavePosition`.  The Lua file declares exactly the same 35 values:

`0, 100, 150, 200, 300, 400, 450, 500, 550, 600, 700, 800, 850, 856, 870, 875, 900, 1000,
1050, 1100, 1200, 1250, 1300, 1450, 1500, 1550, 1600, 1700, 1900, 2100, 2300, 2400, 2500,
2600, 2800`.

Every Lua stage constant has one dispatch-table handler and there are no extra dispatch entries.
Every handler writes exactly its own stage before doing stage work, and its explicit successor is the
next retail stage value; stage 2800 alone has no successor and remains in its free-roam loop.
This proves that a persisted retail stage has a deterministic Lua resume target.  It does not yet
prove that every handler has operation-for-operation runtime parity.

## Childhood-to-Guild handoff

The recovered retail stage-zero code and the Lua port agree on the critical ordering:

1. Give `OBJECT_QUEST_CARD_OAKVALE_INTRO` for `Q_NewOakValeIntro`.
2. Poll `MsgOnQuestCompleted("Q_NewOakValeIntro")`, yielding while false.
3. Activate, without loading resources, `Hook_Fresco_07_OakValeRaid`,
   `Hook_Fresco_09_TimePassing`, `Hook_Fresco_10_UneasyAlliance`, and `Q_GuildTraining`.
4. Move the master `PostSavePosition` to 100 in the training stage.
5. Poll completion of `Q_GuildTraining` before opening the later campaign flow.

Forge binds `MsgOnQuestCompleted` directly to game-interface vtable slot 25; the Lua wrapper does
not emulate completion with `IsQuestCompleted`.  This matches retail's message-polling call at the
stage-zero boundary and preserves event-consumption semantics.

## Persistence fields

Retail `CGameflowScript::OnPersist` transfers four fields:

- `PostSavePosition`
- `CoreQuestWaiting`
- `SavedScriptNames`
- `SavedCardDefNames`

Lua currently transfers the first two.  The two saved vectors are real members (retail script
offsets `+0x60` and `+0x70`; effective decompile offsets `+0x4c` and `+0x58` after its recovered base
layout).  However, neither name nor either object-member offset is referenced by the recovered
retail `Main`; only `OnPersist` references them.  Their persistence-format parity is therefore still
missing, while no campaign behavior may honestly be attributed to them from current evidence.
They must not be invented as Lua string lists until their element-transfer ABI and any external
consumer are identified.

## Native/Lua authority gate

Forge's current loader registers every table entry in `quests.lua`.  Its `EnsureQuestRegistryFile`
then appends `AddQuest("<name>", TRUE)` for every registered custom quest.  The downloaded package's
manifest contains `LUAGameflow`, while retail `Gameflow` remains registered and active.

Running both is invalid because both scripts:

- write the same global `PostSavePosition` master field;
- consume the same quest-completion messages;
- activate the same downstream quests and quest cards; and
- mutate the same gossip/logbook state.

In particular, Lua `Init` writes `PostSavePosition = 0`; merely activating it beside native
Gameflow can rewind the shared campaign authority before any Lua stage handler runs.

The safe activation design must provide exactly one authority.  The evidence-backed implementation
route is an identity-preserving allocator override for native `Gameflow`, using Forge's existing
opt-in retail-override mechanism, after full persistence and handler validation.  A separately named
always-active `LUAGameflow` quest is not an acceptable campaign deployment.

The checked-in authority profile is at
`work/aeon_lua_ports/Gameflow/identity_override/FSE`.  Its `retail_override.lua` replaces only the
allocator on retail's existing `Gameflow` `CScriptInfo`, retaining the native quest name and manager
identity.  Its companion `quests.lua` intentionally contains no `Gameflow` or `LUAGameflow` custom
quest.  The switch is committed as `enabled = false` and is not a deployed game configuration.

Validate that invariant with:

```powershell
python tools/script_recovery/validate_gameflow_authority.py `
  --profile-fse work/aeon_lua_ports/Gameflow/identity_override/FSE `
  --source-fse work/aeon_lua_ports/Gameflow/FSE `
  --output work/aeon_lua_ports/Gameflow/gameflow_authority_validation.json
```

The validator fails if the repository default is enabled, if the native override is missing or
duplicated, if its Lua allocator source is absent, or if the custom quest table introduces a second
`Gameflow`/`LUAGameflow` authority.  The current report passes all of those gates.  It proves static
activation identity and exclusivity; runtime save/load remains deliberately unclaimed.

## Remaining proof gates

- Recover the vector persistence transfer ABI/consumer or prove the members are vestigial outside
  serialization.
- Generate fixtures for every stage handler, including termination while waiting and the true branch
  after its completion message.
- Compare each stage's ordered side effects with retail operation IR, not only API availability.
- Validate save/load at representative stages in a disposable profile.
- Activate Lua gameflow only through a single-authority configuration and verify that native
  `Gameflow` is not simultaneously executing.
