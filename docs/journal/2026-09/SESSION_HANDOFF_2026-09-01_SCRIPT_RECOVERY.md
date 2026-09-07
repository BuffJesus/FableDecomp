# ForgeFSE retail-script recovery handoff — 2026-09-01

## Resume point

- Repository: `D:\Documents\FableTLC`
- Branch: `feat/script-recovery-marathon`
- Last pre-session commit: `916dd6c Lift shared aggregate container teardown`
- Forge worktree: `D:\Code\ForgeFSE-retail-shadow`, branch `feat/retail-script-shadow`
- Fable TLC install: `C:\Programs\Steam\steamapps\common\Fable The Lost Chapters`
- No Fable or Steam process was running at shutdown checkpoint.

The full objective remains active. Static/offline conversion work can continue autonomously. A live
world-entry smoke remains pending and must wait for the user to load a disposable/test game
interactively; do not select or mutate a user profile/save automatically.

## Latest completed lift

`CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x0099A380` is now modeled from its complete
retail body. It writes vtable `0x0129A7D8` at offset zero and zeroes the payload at `+4`. Generated
Lua calls one opaque host initializer with the exact `(vtable, payload offset, payload value)` tuple.
The donor label is not used as behavioral evidence.

Authoritative totals after regeneration:

- 161/161 lifecycle IR artifacts
- 28 generated standalone Lua helpers
- 135 native-derived runtime checks, all passing
- 26 resolved helper labels / 992 calls across 160 scripts
- 127 unresolved helper labels / 585 calls
- readiness: 69 `manual-lua-reconstruction`, 92 `map-native-helpers`
- 0 deployment-eligible generated helpers (intentional until parent scripts are complete)
- 615/615 entity bindings
- 1,708 interface calls / 191 methods; 1,707 runtime-callable

## Verification at checkpoint

- `python -m unittest discover -s tools/script_recovery -p 'test_*.py'`: 93/93 pass
- foundation audit: 18/18 pass
- shadow package verification: 19/19 files pass
- offline shadow preflight: 16/16 scripts pass
- deployed transaction verification: 19/19 files byte-intact

The deployed DLL SHA-256 remains
`AF0912549E831AA34929F6CD7B64714C57CCD24044315E6AD6F2E35D375C1C85`.

Deployment transaction:

`C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\FSE\backups\RetailShadowSmoke-20260901T021609Z\deployment.json`

The package remains a mutation-free shadow/instrumentation package. It does not replace retail
script identities. The previous menu-only live smoke attached successfully and exited normally, but
Lua did not initialize because no game world was entered; therefore it proved 0/16 live script
loads and is not a gameplay smoke.

## Next work

Start with the refreshed `refs/script_recovery/native_helper_target_queue.tsv`. The first candidates
are now:

1. `0x00CD4470 CTimer::~CTimer` — 22 calls / 18 scripts
2. `0x00CBFB7D RunCutsceneMacro_Func` — 44 calls / 15 scripts; sole timed-out decompile
3. `0x00CDBFB0 StdMap_Destroy_API` — 34 calls / 15 scripts
4. `0x007E72A0 CCarriedReadableDef::CCarriedReadableDef` — 50 calls / 14 scripts
5. `0x00CDBF70 StdMap_Construct_API` — 28 calls / 13 scripts

Prefer a target with a complete body and exact ownership semantics. Preserve branch and operation
order through typed opaque callbacks, add all meaningful offline branches, regenerate all derived
artifacts, and keep `deploymentEligible` false until complete parent reconstruction.

## Safety invariants

- Do not load, write, or replace user saves/profiles.
- Do not replace retail script identities.
- Do not redeploy merely to include generated helper Lua; it is intentionally not deployment-ready.
- Verify the recorded deployment transaction before any future live smoke.
- Use the transaction's rollback command if the user requests removal; do not delete installed files
  manually.
- The main worktree contains extensive unrelated user changes. Stage only exact script-recovery files.
