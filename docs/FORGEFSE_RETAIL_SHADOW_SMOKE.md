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
  --transaction '<game>\FSE\backups\RetailShadowSmoke-<stamp>\deployment.json'
```

This checks every deployed hash and every required rollback backup. It does not launch Fable or
touch saves.

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
