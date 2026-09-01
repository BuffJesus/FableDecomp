# ForgeFSE retail-script shadow smoke test

The generated smoke package evaluates all 16 reconstructed seed scripts in ForgeFSE's isolated
Lua state. It does not register candidates as replacement retail scripts, expose the live game
interface, invoke `OnPersist`, permit mutations, or permit save writes.

Build and verify the package:

```powershell
python tools\script_recovery\build_shadow_smoke_package.py `
  --catalog refs\script_recovery\native_catalog.json `
  --corpus refs\script_recovery\seed_corpus\corpus_index.json `
  --sources refs\script_recovery\seed_corpus\sources `
  --dll D:\Code\ForgeFSE-retail-shadow\Release\FableScriptExtender.dll `
  --output D:\Code\ForgeFSE-retail-shadow\Release\RetailShadowSmoke

python tools\script_recovery\verify_shadow_smoke_package.py `
  D:\Code\ForgeFSE-retail-shadow\Release\RetailShadowSmoke

python tools\script_recovery\run_shadow_preflight.py `
  D:\Code\ForgeFSE-retail-shadow\Release\RetailShadowSmoke `
  --output refs\script_recovery\shadow_offline_preflight.json
```

For a reversible installation, use `deploy_shadow_smoke_package.py deploy`. It refuses to run
while Fable is open, backs up every overwritten file, records newly created files, and emits a
transaction path. Pass that path to its `rollback` command to restore the previous DLL and remove
only unchanged files created by the deployment.

Audit an installed transaction without changing the game directory:

```powershell
python tools\script_recovery\deploy_shadow_smoke_package.py verify `
  --transaction '<game>\FSE\backups\RetailShadowSmoke-<stamp>\deployment.json' `
  --output refs\script_recovery\shadow_deployment_verification.json
```

This checks every deployed hash and every required rollback backup. It does not launch Fable or
touch saves.

The current local transaction is
`RetailShadowSmoke-20260901T021609Z`. Before deploying it, the preceding transaction was rolled back
successfully (one overwritten DLL restored and 18 unchanged package files removed). The rebuilt
package passed 16/16 offline script preflights, and the fresh deployment verifies 19/19 files with
DLL SHA-256 `AF0912549E831AA34929F6CD7B64714C57CCD24044315E6AD6F2E35D375C1C85`.

A fresh menu-only launch with that DLL on `2026-09-01T02:16:51Z` confirmed attachment, API-pointer
initialization, hook installation, a responsive game process, and a normal window close. No profile
or save was loaded. As expected, the menu phase did not initialize Lua, so the gameplay-level
16-script terminal summary remains pending rather than being reported as a successful full smoke.

After manually backing up an existing ForgeFSE installation, copy the package contents into the
game directory so `FableScriptExtender.dll` is beside `Fable.exe` and `FSE/retail_shadow.lua` is
under that same directory. Start the game once, reach the point where ForgeFSE initializes Lua,
then exit normally.

Verify the captured log:

```powershell
python tools\script_recovery\verify_shadow_smoke_log.py `
  --log '<game-directory>\FSE\FableScriptExtender.log' `
  --manifest D:\Code\ForgeFSE-retail-shadow\Release\RetailShadowSmoke\shadow_smoke_manifest.json `
  --output refs\script_recovery\shadow_smoke_result.json
```

Success requires the DLL attach marker, Lua initialization, 16 distinct completion markers,
`passed=16 failed=0`, and no missing-candidate, exception, or lifecycle-failure lines.

A front-end-only launch may stop at verifier phase `dll-attached`: retail initializes ForgeFSE's Lua
systems only after entering gameplay. This proves injection/startup but is not a completed shadow
smoke. Do not automate save selection merely to advance the phase; capture the partial result and
finish the test during an explicitly supervised disposable-save session.
