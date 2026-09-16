"""Package a verified stock-FSE + NoviCompatibility sidecar bundle for Discord playtesters.

Input is a bundle produced by build_novi_compat_bundle.py (local-candidate-vN). Output is a zip
that testers extract into the Fable root; nothing in their install is replaced:

    NewOakValeIntro/            stock FableScriptExtender.dll, NoviCompatibility.dll, NoviLocalLauncher.exe,
                                FSE/quests.lua (empty registry), NoviCompatibility/ (readable Lua + override)
    Launch New Oakvale Intro.bat  starts Fable.exe suspended and injects both DLLs from NewOakValeIntro/
    README_DISCORD_TEST.txt
    bundle_manifest.json        sha256 of every packaged file + source bundle manifest

Run-time artifacts (runs/, logs, local_test.py, README.md) are never packaged.
"""
import argparse, datetime, hashlib, json, pathlib, zipfile

REPO = pathlib.Path(__file__).resolve().parents[2]
SKIP_NAMES = {'local_test.py', 'README.md', 'manifest.json', 'FableScriptExtender.log'}
SKIP_DIRS = {'runs'}

LAUNCHER = r'''@echo off
setlocal
set "ROOT=%~dp0"
set "GAME=%ROOT:~0,-1%"
if not exist "%GAME%\Fable.exe" (
    echo This file must sit in the "Fable The Lost Chapters" folder next to Fable.exe.
    pause
    exit /b 1
)
tasklist /FI "IMAGENAME eq Fable.exe" 2>nul | find /I "Fable.exe" >nul
if not errorlevel 1 (
    echo Fable is already running. Close it first.
    pause
    exit /b 1
)
"%GAME%\NewOakValeIntro\NoviLocalLauncher.exe" "%GAME%\Fable.exe" "%GAME%" "%GAME%\NewOakValeIntro\FableScriptExtender.dll" "%GAME%\NewOakValeIntro\NoviCompatibility.dll"
if errorlevel 1 (
    echo Launcher failed with code %errorlevel%. Send NewOakValeIntro\NoviCompatibility\FableScriptExtender.log with your report.
    pause
)
'''

README = '''NEW OAKVALE INTRO - READABLE SCRIPT PLAYTEST (sidecar build {stamp})

What this is
  The childhood "New Oakvale Intro" quest (Q_NewOakValeIntro) re-implemented as readable Lua,
  run by the community FableScriptExtender plus a small compatibility add-on. Your installed
  FSE, Fable.exe and saves are NOT modified: everything loads from the NewOakValeIntro folder.

Requirements
  Retail Fable: The Lost Chapters (Steam). No separate FSE install is needed.

Install
  1. Close Fable.
  2. Extract this archive into the game folder (the one containing Fable.exe). You should get
     "Launch New Oakvale Intro.bat" next to Fable.exe and a NewOakValeIntro folder beside it.
  3. Double-click "Launch New Oakvale Intro.bat". Do NOT use FSE_Launcher.exe or Steam for this test.
  4. Start a NEW game on a fresh, disposable profile. Play through childhood.

Uninstall
  Delete the NewOakValeIntro folder and the .bat. Launch normally through Steam or FSE_Launcher.exe.

Reporting a problem
  Say exactly what you were doing, whether it was a fresh New Game, and attach
  NewOakValeIntro\\NoviCompatibility\\FableScriptExtender.log (and NewOakValeIntro\\FSE\\FableScriptExtender.log
  if present). Known: some locked-camera Father lines play quietly; that is not what we are testing.

This is an offline test build, not a release.
'''


def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()


def collect(bundle):
    for p in sorted(bundle.rglob('*')):
        if not p.is_file() or p.name in SKIP_NAMES: continue
        if SKIP_DIRS & set(p.relative_to(bundle).parts): continue
        if p.suffix == '.log': continue
        yield p


def main():
    a = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    a.add_argument('--bundle', type=pathlib.Path, required=True)
    a.add_argument('--out', type=pathlib.Path, required=True)
    args = a.parse_args()
    bundle = args.bundle.resolve()
    if args.out.exists(): raise SystemExit(f'refusing to overwrite {args.out}')
    for required in ['FableScriptExtender.dll', 'NoviCompatibility.dll', 'NoviLocalLauncher.exe',
                     'FSE/quests.lua', 'NoviCompatibility/retail_override.lua',
                     'NoviCompatibility/NewOakValeIntro/NewOakValeIntro.lua']:
        if not (bundle / required).is_file(): raise SystemExit(f'bundle missing {required}')
    if (bundle / 'FSE/quests.lua').read_text().strip() != 'Quests = {}':
        raise SystemExit('custom-quest registry must be empty; NewOakValeIntro loads through retail_override.lua')
    for p in bundle.rglob('*.lua'):
        if 'PARTY_MODE' in p.read_text(encoding='utf-8', errors='replace') or 'PartyMode' in p.read_text(encoding='utf-8', errors='replace'):
            raise SystemExit(f'Party Mode residue in {p}')
    stamp = datetime.datetime.now(datetime.timezone.utc).strftime('%Y-%m-%d')
    files = []
    args.out.parent.mkdir(parents=True, exist_ok=True)
    with zipfile.ZipFile(args.out, 'w', zipfile.ZIP_DEFLATED) as z:
        for p in collect(bundle):
            rel = 'NewOakValeIntro/' + p.relative_to(bundle).as_posix()
            z.write(p, rel); files.append({'path': rel, 'size': p.stat().st_size, 'sha256': sha(p)})
        z.writestr('Launch New Oakvale Intro.bat', LAUNCHER.replace('\n', '\r\n'))
        z.writestr('README_DISCORD_TEST.txt', README.format(stamp=stamp).replace('\n', '\r\n'))
        manifest = {'schema': 'new-oakvale-discord-playtest/2', 'arrangement': 'stock-fse-plus-novi-sidecar',
                    'built': stamp, 'sourceBundle': str(bundle), 'deploymentPerformed': False,
                    'replacesInstalledFiles': False, 'files': files,
                    'sourceBundleManifest': json.loads((bundle / 'manifest.json').read_text())}
        z.writestr('bundle_manifest.json', json.dumps(manifest, indent=2) + '\n')
    print(json.dumps({'zip': str(args.out), 'files': len(files), 'bytes': args.out.stat().st_size}, indent=2))


if __name__ == '__main__':
    main()
