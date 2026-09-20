#!/usr/bin/env python3
"""build_custom_intro.py -- assemble the Oakvale Reborn intro from its manifest.

One manifest (refs/script_recovery/authored/OakvaleReborn/manifest/intro.yaml)
names every piece of custom content; this script turns it into (a) a game-tree
OVERLAY of modified data files and (b) a stock-FSE + NoviCompatibility sidecar
BUNDLE carrying the Lua, then installs / restores the overlay on the live
install. Everything is rebuilt from PRISTINE copies of the touched retail files,
never from a previously staged tree (DEFS.md "Track A" warning).

Subcommands (run in this order; `all` = pristine text defs cutscenes overlay bundle check):
  pristine   copy the touched retail files out of the game root (refuses if a
             copy exists with a different hash -- the install drifted)
  text       manifest `lines` into staged text.big: subtitle-only lines through
             text_build.add_text; vo: true lines through dialogue_pipeline.stage
             --add (each call reads the previous staged tree, so the join
             tables grow consistently) with the wav from work/oakvale_reborn/vo/
             <key>.wav (elevenlabs_vo.py; <out>/vo/) -- a missing wav is an error
  defs       forge title add every manifest title into staged game.bin (donor
             clone; text ids read back from the staged text.big)
  cutscenes  forge script cutscene-set every manifest cutscene into staged
             CompiledDefs (`clone_of` = dump the retail def from pristine and
             re-set it under the new name), then validate + cutscene-roundtrip
  overlay    lay the staged files out as <out>/overlay/<game-relative path>
             with a stage_manifest.json (sha256 + bytes per file)
  bundle     build_novi_compat_bundle.py --skip-build --readable <lua> --bundle
  check      cs_lint.py (manifest / .cs / Lua cross-checks) + the mocked-FSE
             smoke (tools/script_recovery/smoke_run_unit.py --package-dir)
  install    lay the overlay over the live install as its own layer (current
             files -> <file>.ovrbak, install_receipt.json written); the FableForge
             stage manifest is never touched
  restore    put this layer's .ovrbak files back (refuses files that changed
             underneath; --force overrides) -- never `forge unstage`

The Lua source tree is given with --lua (default: the manifest's `lua`). A spike
stage (tools/oakvale_reborn/spike_s1.py) is just another --lua tree.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
import pathlib
import re
import shutil
import subprocess
import sys

import yaml

REPO = pathlib.Path(__file__).resolve().parents[2]
sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
import oakvale_manifest  # noqa: E402
AUTHORED = REPO / 'refs/script_recovery/authored/OakvaleReborn'
DEFAULT_MANIFEST = AUTHORED / 'manifest/intro.yaml'
DEFAULT_OUT = REPO / 'work/oakvale_reborn'
FORGE_TOOLS = pathlib.Path(r'D:\Code\FableForge\build\forge-tools.exe')
BUNDLE_TOOL = REPO / 'tools/script_recovery/build_novi_compat_bundle.py'
SIDECAR_WORK = REPO / 'work/new-oakvale-original-fse-20260912'

# Retail files a build may touch, game-root-relative. Only the ones a manifest
# actually needs are copied/staged.
CUTSCENE_FILES = ['data/CompiledDefs/script.bin', 'data/CompiledDefs/names.bin']
DEFS_FILES = ['data/CompiledDefs/game.bin', 'data/CompiledDefs/names.bin']
TEXT_FILES = ['data/lang/English/text.big']
VO_FILES = ['data/lang/English/dialogue.big']
VO_BANK_FILES = {  # speech bank -> the lut + snds.bin it grows
    'Dialogue.lug': ['data/lang/English/Dialogue.lut', 'data/Defs/dialoguesnds.bin'],
    'Dialogue2.lug': ['data/lang/English/Dialogue2.lut', 'data/Defs/dialoguesnds2.bin'],
    'ScriptDialogue.lug': ['data/lang/English/ScriptDialogue.lut', 'data/Defs/scriptdialoguesnds.bin'],
    'ScriptDialogue2.lug': ['data/lang/English/ScriptDialogue2.lut', 'data/Defs/scriptdialoguesnds2.bin'],
}
SUBTITLE_BANK = 'ScriptDialogue.lug'   # retail subtitle-only quest lines carry this bank and no snds pair
CS_LINT = REPO / 'tools/oakvale_reborn/cs_lint.py'
SMOKE = REPO / 'tools/script_recovery/smoke_run_unit.py'


def sha(p: pathlib.Path) -> str:
    return hashlib.sha256(p.read_bytes()).hexdigest().upper()


def load_manifest(path: pathlib.Path) -> dict:
    return oakvale_manifest.load(path)


def run(cmd: list, check: bool = True) -> subprocess.CompletedProcess:
    print('$', ' '.join(str(c) for c in cmd))
    r = subprocess.run([str(c) for c in cmd], capture_output=True, text=True)
    sys.stdout.write(r.stdout)
    sys.stderr.write(r.stderr)
    if check and r.returncode != 0:
        raise SystemExit(f'command failed ({r.returncode}): {cmd[0]}')
    return r


# --- pristine ---------------------------------------------------------------

def touched_files(m: dict) -> list[str]:
    files: list[str] = []
    lines = m.get('lines') or []
    if lines:
        files += TEXT_FILES
    vo_banks = {ln['bank'] for ln in lines if ln.get('vo')}
    if vo_banks:
        files += VO_FILES
        for bank in sorted(vo_banks):
            files += VO_BANK_FILES[bank]
    if m.get('cutscenes'):
        files += CUTSCENE_FILES
    if m.get('titles'):
        files += [f for f in DEFS_FILES if f not in files]
    return files


def stage_from_pristine(out: pathlib.Path, rels: list[str]) -> None:
    for rel in rels:
        (out / 'staged' / rel).parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(out / 'pristine' / rel, out / 'staged' / rel)


# --- text -------------------------------------------------------------------

def line_speaker(m: dict, ln: dict) -> str:
    voice = (m.get('voices') or {}).get(ln.get('voice'), {})
    speaker = ln.get('speaker') or voice.get('speaker')
    if not speaker:
        raise SystemExit(f'{ln["key"]}: no speaker')
    return speaker


PLACEHOLDER_VO = False


def xbadpcm_placeholder(seconds: float) -> bytes:
    import xbadpcm
    n = int(22050 * seconds)
    return xbadpcm.build_pcm_riff([((i % 64) - 32) for i in range(n)], 1, 22050)   # -60 dBFS hiss


def cmd_text(m: dict, out: pathlib.Path) -> None:
    lines = m.get('lines') or []
    placeholders: list[str] = []
    if not lines:
        print('text: no lines')
        return
    sys.path.insert(0, str(REPO / 'tools'))
    import text_build
    import dialogue_pipeline
    staged = out / 'staged'
    stage_from_pristine(out, [f for f in touched_files(m) if f not in CUTSCENE_FILES])
    text_big = staged / 'data/lang/English/text.big'

    # 1. subtitle-only lines in one text_build pass
    subtitle = [ln for ln in lines if not ln.get('vo')]
    if subtitle:
        bank = text_build.TextBank(str(text_big))
        for ln in subtitle:
            bank.add_text(ln['key'], ln['text'], speaker=line_speaker(m, ln), speechbank=SUBTITLE_BANK)
        tmp = text_big.with_suffix('.big.tmp')
        bank.save(str(tmp))
        os.replace(tmp, text_big)
        print(f'text: {len(subtitle)} subtitle-only line(s) added')

    # 2. voiced lines, one dialogue_pipeline.stage per line, chained through staged/
    for ln in [ln for ln in lines if ln.get('vo')]:
        wav = out / 'vo' / f'{ln["key"]}.wav'
        if not wav.exists():
            if not PLACEHOLDER_VO:
                raise SystemExit(f'{ln["key"]}: vo: true but {wav} is missing (run elevenlabs_vo.py, '
                                 f'or build with --placeholder-vo to stage a silent stand-in)')
            wav = out / 'vo_placeholder' / f'{ln["key"]}.wav'
            if not wav.exists():
                wav.parent.mkdir(parents=True, exist_ok=True)
                # near-silent 1.2 s clip: subtitle + mouth curve exist, nothing audible; swapped out
                # automatically once the real wav appears in vo/
                wav.write_bytes(xbadpcm_placeholder(1.2))
            placeholders.append(ln['key'])
        scratch = out / 'text_stage' / ln['key']
        if scratch.exists():
            shutil.rmtree(scratch)
        args = argparse.Namespace(wav=str(wav), text=ln['text'], speaker=line_speaker(m, ln),
                                  replace=None, add=ln['key'], bank=ln['bank'],
                                  out=str(scratch), install=str(staged))
        dialogue_pipeline.stage(args)
        bank_files = VO_BANK_FILES[ln['bank']]
        moves = {'text.big': 'data/lang/English/text.big', 'dialogue.big': 'data/lang/English/dialogue.big',
                 pathlib.Path(bank_files[0]).name: bank_files[0], pathlib.Path(bank_files[1]).name: bank_files[1]}
        for name, rel in moves.items():
            shutil.copyfile(scratch / name, staged / rel)
        print(f'text: voiced line {ln["key"]} staged')
    # 3. hero-title group entries (type-1 lists of the per-voice-type lines), retail shape
    if m.get('titles'):
        bank = text_build.TextBank(str(text_big))
        for t in m['titles']:
            keys = oakvale_manifest.title_text_keys(t)
            for cat, group in keys['groups'].items():
                members = [bank.by_name[k]['id'] for k in keys['members'][cat]]
                bank.add_group(group, members)
        tmp = text_big.with_suffix('.big.tmp')
        bank.save(str(tmp))
        os.replace(tmp, text_big)
        print(f'text: {3 * len(m["titles"])} title group(s) added')
    n = len(text_build.TextBank(str(text_big)).by_name)
    print(f'text: staged text.big has {n} entries')
    if placeholders:
        (out / 'placeholder_vo.json').write_text(json.dumps(placeholders, indent=1) + '\n', encoding='utf-8')
        print(f'text: {len(placeholders)} line(s) staged with SILENT placeholder audio (see placeholder_vo.json)')


def cmd_pristine(m: dict, out: pathlib.Path) -> None:
    game = pathlib.Path(m['game_root'])
    pristine = out / 'pristine'
    layered = forge_staged_paths(game)
    for rel in touched_files(m):
        src, dst = game / rel, pristine / rel
        if rel in layered:
            print(f'note: {rel} is FableForge-staged on this install; the layer base is that stage, not retail')
        if dst.exists():
            if sha(src) != sha(dst):
                raise SystemExit(f'{rel}: the install differs from the pristine copy; '
                                 f'restore first (or delete {dst} if the pristine copy is stale)')
            print(f'pristine ok   {rel}')
            continue
        dst.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(src, dst)
        print(f'pristine copy {rel}  {sha(dst)[:16]}')


# --- defs (hero titles) -----------------------------------------------------------

def cmd_defs(m: dict, out: pathlib.Path) -> None:
    titles = m.get('titles') or []
    if not titles:
        print('defs: no titles')
        return
    sys.path.insert(0, str(REPO / 'tools'))
    import text_build
    staged = out / 'staged'
    stage_from_pristine(out, [f for f in DEFS_FILES if not (staged / f).exists() or f == 'data/CompiledDefs/game.bin'])
    bank = text_build.TextBank(str(staged / 'data/lang/English/text.big'))
    for t in titles:
        keys = oakvale_manifest.title_text_keys(t)
        ids = {k: bank.by_name[v]['id'] for k, v in (('gui', keys['gui']), ('desc', keys['desc']))}
        ids.update({cat: bank.by_name[g]['id'] for cat, g in keys['groups'].items()})
        cmd = [FORGE_TOOLS, 'title', 'add', staged, t['name'], '--donor', t['donor'],
               '--gui', ids['gui'], '--desc', ids['desc'], '--greet', ids['greet'],
               '--comment', ids['comment'], '--self', ids['self'], '--write']
        if t.get('enum') is not None:
            cmd += ['--enum', t['enum']]
        run(cmd)
    for bak in (staged / 'data/CompiledDefs').glob('*.forgebak'):
        bak.unlink()


# --- cutscenes ----------------------------------------------------------------

def cmd_cutscenes(m: dict, out: pathlib.Path) -> None:
    staged = out / 'staged'
    # names.bin is shared with the defs stage: keep it if titles already grew it
    keep_names = bool(m.get('titles')) and (staged / 'data/CompiledDefs/names.bin').exists()
    stage_from_pristine(out, [f for f in CUTSCENE_FILES if not (keep_names and f.endswith('names.bin'))])
    clones = out / 'cutscene_clones'
    clones.mkdir(parents=True, exist_ok=True)
    for cs in m.get('cutscenes', []):
        if cs.get('clone_of'):
            src = clones / f'{cs["name"]}.cs'
            run([FORGE_TOOLS, 'script', 'cutscene-dump', out / 'pristine', cs['clone_of'], '--cs', src])
            ins = cs.get('insert_before')
            if ins:
                # splice new commands in front of a retail one (a Maze line before "We must leave")
                text = src.read_text(encoding='utf-8')
                anchor = ins['match'] + '\n'
                if text.count(anchor) != 1:
                    raise SystemExit(f'{cs["name"]}: insert_before match not unique in {cs["clone_of"]}: {ins["match"]!r}')
                text = text.replace(anchor, '\n'.join(ins['lines']) + '\n' + anchor)
                src.write_text(text, encoding='utf-8', newline='\n')
        else:
            src = m['_dir'] / cs['source']
        run([FORGE_TOOLS, 'script', 'cutscene-set', staged, cs['name'], src, '--write'])
    # forge backs the staged copies up as *.forgebak on first write; they are
    # the pristine bytes again, so drop them to keep the overlay clean.
    for bak in (staged / 'data/CompiledDefs').glob('*.forgebak'):
        bak.unlink()
    run([FORGE_TOOLS, 'script', 'cutscene-roundtrip', staged])
    r = run([FORGE_TOOLS, 'script', 'validate', staged, 'CS_OVR'], check=False)
    if 'unknown' in r.stdout.lower() and 'unknown verbs: 0' not in r.stdout.lower():
        print('validate output above -- check it')


# --- overlay ------------------------------------------------------------------

def cmd_overlay(m: dict, out: pathlib.Path) -> None:
    overlay = out / 'overlay'
    if overlay.exists():
        shutil.rmtree(overlay)
    rows = []
    for rel in touched_files(m):
        src = out / 'staged' / rel
        if not src.exists():
            raise SystemExit(f'{rel} not staged yet')
        dst = overlay / rel
        dst.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(src, dst)
        rows.append({'path': rel, 'sha256': sha(dst), 'bytes': dst.stat().st_size})
    (overlay / 'stage_manifest.json').write_text(
        json.dumps({'schema': 'fableforge.stage_overlay.v1', 'project': m['project'],
                    'files': rows}, indent=2) + '\n', encoding='utf-8')
    print(f'overlay: {len(rows)} files -> {overlay}')


# --- bundle -------------------------------------------------------------------

def cmd_bundle(m: dict, out: pathlib.Path, lua: pathlib.Path, tag: str) -> None:
    bundle = out / f'bundle-{tag}'
    run([sys.executable, BUNDLE_TOOL, '--skip-build', '--readable', lua,
         '--template', SIDECAR_WORK / m.get('bundle_template', 'local-candidate-v4'),
         '--out-source', SIDECAR_WORK / m.get('sidecar_source', 'sidecar-abi-v2'),
         '--bundle', bundle])
    print(f'bundle: {bundle}')
    print(f'launch: python "{bundle / "local_test.py"}" --game-dir "{m["game_root"]}" '
          f'--launch --save-dir "<saves>"')


# --- check --------------------------------------------------------------------

def cmd_check(m: dict, out: pathlib.Path, lua: pathlib.Path, manifest: pathlib.Path) -> None:
    lint = run([sys.executable, CS_LINT, '--manifest', manifest, '--lua', lua], check=False)
    smoke = run([sys.executable, SMOKE, '--package-dir', lua, '--frames', '60',
                 '--json', out / 'smoke_report.json'], check=False)
    if lint.returncode != 0:
        raise SystemExit('check: cs_lint reported problems')
    if smoke.returncode != 0:
        raise SystemExit('check: smoke runner failed')
    report = json.loads((out / 'smoke_report.json').read_text(encoding='utf-8'))
    # the mocked runtime knows nothing about the sidecar's real return shapes, so only
    # load errors and unregistered methods are gates; runtime errors are listed above
    gate = 0
    for rel, entry in report.items():
        if 'load_error' in entry:
            gate += 1
        for fname, r in entry.items():
            if isinstance(r, dict) and r.get('unknownMethods'):
                print(f'check: {rel}:{fname} calls unregistered {r["unknownMethods"]}')
                gate += 1
    print(f'check: {gate} gating problem(s) (smoke report {out / "smoke_report.json"})')
    if gate:
        raise SystemExit(1)


# --- install / restore ----------------------------------------------------------
#
# The overlay is a LAYER over whatever the install currently holds -- today that
# is retail plus the FableForge world stage (text.big, wad, wld, qst, stb,
# FSE_Master.lua in forge_stage_manifest.json). `pristine` therefore means "the
# file as installed when this build started", and install/restore keep their own
# <file>.ovrbak backups + install_receipt.json and never edit the forge stage
# manifest: `forge unstage` stays FableForge's, `restore` stays ours. Restore
# refuses a file that changed since we installed it (someone unstaged or
# verified underneath us) unless --force.

BACKUP_SUFFIX = '.ovrbak'


def stage_manifest_path(game: pathlib.Path) -> pathlib.Path:
    return game / 'forge_stage_manifest.json'


def forge_staged_paths(game: pathlib.Path) -> set[str]:
    mp = stage_manifest_path(game)
    if not mp.exists():
        return set()
    return {x['path'] for x in json.loads(mp.read_text(encoding='utf-8')).get('files', [])}


def cmd_install(m: dict, out: pathlib.Path) -> None:
    game = pathlib.Path(m['game_root'])
    overlay = out / 'overlay'
    rows = json.loads((overlay / 'stage_manifest.json').read_text(encoding='utf-8'))['files']
    rp = out / 'install_receipt.json'
    if rp.exists():
        raise SystemExit(f'already installed ({rp}); restore first')
    layered = forge_staged_paths(game)
    receipt = []
    for row in rows:
        rel = row['path']
        src, dst = overlay / rel, game / rel
        bak = pathlib.Path(str(dst) + BACKUP_SUFFIX)
        had = dst.exists()
        if had:
            if sha(dst) != sha(out / 'pristine' / rel):
                raise SystemExit(f'{rel}: the install no longer matches pristine/ -- rebuild from a fresh '
                                 f'`pristine` (delete the stale copy) before installing')
            shutil.copyfile(dst, bak)
        dst.parent.mkdir(parents=True, exist_ok=True)
        tmp = dst.with_name(dst.name + '.ovr-tmp')
        shutil.copyfile(src, tmp)
        os.replace(tmp, dst)
        if sha(dst) != row['sha256']:
            raise SystemExit(f'post-copy hash mismatch: {rel}')
        receipt.append({'path': rel, 'sha256': row['sha256'], 'had_original': had,
                        'backup': str(bak) if had else None, 'over_forge_stage': rel in layered})
        print(f'installed {rel}{"  (layered over the FableForge stage)" if rel in layered else ""}')
    rp.write_text(json.dumps({'schema': 'oakvale_reborn.install_receipt.v2', 'game_root': str(game),
                              'files': receipt}, indent=2) + '\n', encoding='utf-8')
    print(f'{len(receipt)} file(s) installed; receipt {rp}')


def cmd_restore(m: dict, out: pathlib.Path, force: bool = False) -> None:
    game = pathlib.Path(m['game_root'])
    rp = out / 'install_receipt.json'
    if not rp.exists():
        raise SystemExit('nothing installed (no install_receipt.json)')
    receipt = json.loads(rp.read_text(encoding='utf-8'))
    for r in receipt['files']:
        dst = game / r['path']
        if dst.exists() and sha(dst) != r['sha256'] and not force:
            raise SystemExit(f'{r["path"]}: changed since install (not our bytes); '
                             f'inspect it, then `restore --force` to put the backup back anyway')
        if r['had_original']:
            os.replace(pathlib.Path(r['backup']), dst)
            print(f'restored {r["path"]}')
        elif dst.exists():
            dst.unlink()
            print(f'removed  {r["path"]}')
    rp.unlink()
    print('restore complete')


# --- main ---------------------------------------------------------------------

def main() -> int:
    a = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    a.add_argument('command', choices=['pristine', 'text', 'defs', 'cutscenes', 'overlay', 'bundle', 'check',
                                       'install', 'restore', 'all'])
    a.add_argument('--manifest', type=pathlib.Path, default=DEFAULT_MANIFEST)
    a.add_argument('--out', type=pathlib.Path, default=DEFAULT_OUT)
    a.add_argument('--lua', type=pathlib.Path, help='Lua tree to bundle (default: manifest `lua`)')
    a.add_argument('--tag', default='v1', help='bundle folder suffix')
    a.add_argument('--force', action='store_true', help='restore: put backups back even if the installed bytes changed')
    a.add_argument('--placeholder-vo', action='store_true', help='text: stage a silent clip for any voiced line without a wav')
    args = a.parse_args()
    m = load_manifest(args.manifest)
    global PLACEHOLDER_VO
    PLACEHOLDER_VO = args.placeholder_vo
    lua = args.lua or (m['_dir'] / m.get('lua', 'FSE'))
    args.out.mkdir(parents=True, exist_ok=True)
    steps = {'pristine': lambda: cmd_pristine(m, args.out),
             'text': lambda: cmd_text(m, args.out),
             'defs': lambda: cmd_defs(m, args.out),
             'cutscenes': lambda: cmd_cutscenes(m, args.out),
             'overlay': lambda: cmd_overlay(m, args.out),
             'bundle': lambda: cmd_bundle(m, args.out, lua, args.tag),
             'check': lambda: cmd_check(m, args.out, lua, args.manifest),
             'install': lambda: cmd_install(m, args.out),
             'restore': lambda: cmd_restore(m, args.out, args.force)}
    order = ['pristine', 'text', 'defs', 'cutscenes', 'overlay', 'bundle', 'check'] if args.command == 'all' else [args.command]
    for step in order:
        print(f'== {step}')
        steps[step]()
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
