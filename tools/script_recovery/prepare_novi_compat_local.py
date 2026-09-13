"""Assemble local-only coexistence candidate; no installation or game execution."""
import argparse,hashlib,json,shutil
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    p=argparse.ArgumentParser()
    for name in ['original','addon','loader','game','output']:p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args()
    if a.output.exists():raise FileExistsError(a.output)
    if sha(a.game/'Fable.exe')!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10':
        raise ValueError('Unsupported game executable')
    if sha(a.original)!='36ffb32ce4f1689d08cd5deef37829760d9f2a840aa9e801e79c86135fdb95ea':
        raise ValueError('Original DLL does not match the locally compiled upstream baseline')
    a.output.mkdir(parents=True)
    for src,name in [(a.original,'FableScriptExtender.dll'),(a.addon,'NoviCompatibility.dll'),(a.loader,'NoviLocalLauncher.exe')]:
        shutil.copy2(src,a.output/name)
    (a.output/'FSE').mkdir();(a.output/'FSE/quests.lua').write_text('Quests = {}\n')
    profile=a.output/'NoviCompatibility';profile.mkdir()
    (profile/'quests.lua').write_text('Quests = {}\n')
    script_source=ROOT/'refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro'
    shutil.copytree(script_source,profile/'NewOakValeIntro')
    override=(ROOT/'refs/script_recovery/new_oakvale_intro/runtime_playtest/retail_override.lua').read_text()
    assert override.count('enabled = false')==1
    (profile/'retail_override.lua').write_text(override.replace('enabled = false','enabled = true',1))
    shutil.copy2(Path(__file__).with_name('novi_local_test.py'),a.output/'local_test.py')
    shutil.copy2(Path(__file__).with_name('observe_retail_cutscene.py'),a.output/'observe_retail_cutscene.py')
    shutil.copy2(ROOT/'refs/script_recovery/new_oakvale_intro/runtime_evidence/retail-skip-gate-emulation-20260912.json',a.output/'observer-proof.json')
    (a.output/'README.md').write_text('''# Local-only New Oakvale / original FSE coexistence candidate

Not a public release. Original FSE is compiled without source modifications from
upstream b2432e82b2ef1e5fd50b8fac2a8809bd2bd107d6; this is not a maintainer-supplied
release binary. Its bytes are unchanged. The separate add-on contains independent
quest hosts and Lua runtime support, not merely eight replacement API functions.

Preflight (does not launch):

    python local_test.py --game-dir "C:/Programs/Steam/steamapps/common/Fable The Lost Chapters"

Add --launch only for the coordinated local test. The launcher creates the game
suspended, loads original FSE then the add-on, chains original registration, and
resumes only after successful initialization. Both configurations/logs stay in
this folder. Original FSE has an empty custom-quest registry for this first test;
coexistence with other original-FSE custom quests remains untested.

No installer is needed. No DLL or configuration in the game directory is replaced.
Quit the game and use the normal FSE_Launcher.exe to return to installed v23.
Save files are shared with the game: the wrapper backs them up under runs/ before
each launch. Use a fresh disposable profile. Save restoration is a separate step;
merely closing the game does not undo saves written during a test.

The wrapper starts the read-only cutscene/audio observer after launch and before
you enter the intro. It runs until game exit or 30 minutes; output is under runs/.
The observer does not inject hooks, alter input or record microphone audio.

Lua comes from the current reviewed reconstruction (123 fixture cases), including
the four source updates beyond installed v23. This is a new candidate, not the
identical previously playtested v23 package.

First live gates: both initialization logs, one New Oakvale authority, Father
scene and Escape, Bully skip; then deed branches, Theresa, raid and save/reload.
Quiet Father speech remains unresolved. Source/loader checks are not gameplay
validation. No distribution until these local gates are reviewed.
''')
    manifest=dict(local_only=True,game_exe_sha256=sha(a.game/'Fable.exe'),
                  original_source_commit='b2432e82b2ef1e5fd50b8fac2a8809bd2bd107d6',
                  lua_source=str(script_source),
                  files={f.relative_to(a.output).as_posix():sha(f) for f in sorted(a.output.rglob('*')) if f.is_file()})
    (a.output/'manifest.json').write_text(json.dumps(manifest,indent=2))
    print(a.output)
if __name__=='__main__':main()
