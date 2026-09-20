#!/usr/bin/env python3
"""cs_lint.py -- static cross-checks for the Oakvale Reborn manifest, .cs macros and Lua.

    python tools/oakvale_reborn/cs_lint.py [--manifest intro.yaml] [--lua <FSE dir>] [--json out.json]

What is checked (each finding names the file and the offending token):
  lines       keys unique and TEXT_OVR_-prefixed; the speaker (line `speaker` or the
              voice's) is a retail NarratorList name; vo: true lines name a speech bank
  cutscenes   exactly one of source/clone_of; the .cs parses into [Macro]/[SkipCond]/
              [SetupCond]; every verb is one of the 184 native verbs (actor verbs are
              `Actor.Verb`); every actor prefix is a declared actor, a RegisterActor /
              Create name, or HERO; every 'TEXT_*' is retail or a manifest line; every
              camera/marker/thing target of UseCamera, NoLoadUseCamera, .Teleport,
              .WalkTo, .RunTo, .LookToThing, Create, CreateEffect, PlaySound is a
              retail StartOakVale TNG thing, a manifest marker, or a scene actor
  lua         every RunMacro("CS_OVR_*") names a manifest cutscene; every "TEXT_OVR_*"
              literal is a manifest line; scenes[].cutscene entries exist

Retail names come from text.big (keys + narrators) and the StartOakVale TNGs pulled
out of FinalAlbion.wad into work/oakvale_reborn/tng_cache (extracted once). Without
the game root the retail-dependent checks are skipped and reported as such.
"""
from __future__ import annotations

import argparse
import json
import pathlib
import re
import subprocess
import sys

import yaml  # noqa: F401

REPO = pathlib.Path(__file__).resolve().parents[2]
sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
import oakvale_manifest  # noqa: E402
AUTHORED = REPO / 'refs/script_recovery/authored/OakvaleReborn'
DEFAULT_MANIFEST = AUTHORED / 'manifest/intro.yaml'
FORGE_TOOLS = pathlib.Path(r'D:\Code\FableForge\build\forge-tools.exe')
VERB_TABLE = REPO / 'ghidra_out/cutscene_verb_args.tsv'
TNG_CACHE = REPO / 'work/oakvale_reborn/tng_cache'
sys.path.insert(0, str(REPO / 'tools'))

TEXT_KEY = re.compile(r"'(TEXT_[A-Z0-9_]+)'")
LUA_TEXT = re.compile(r'"(TEXT_OVR_[A-Z0-9_]+)"')
LUA_MACRO = re.compile(r'RunMacro(?:WithStrings|WithFlags)?\(\s*"(CS_[A-Z0-9_]+)"')
LUA_MACRO_LITERAL = re.compile(r'"(CS_OVR_[A-Z0-9_]+)"')
LUA_TITLE = re.compile(r'"(OBJECT_HERO_TITLE_[A-Z0-9_]+)"')
# verb -> index of the argument that must name a thing / marker / camera point
TARGET_ARGS = {'UseCamera': 0, 'NoLoadUseCamera': 0, '.Teleport': 0, '.WalkTo': 0, '.RunTo': 0,
               '.LookToThing': 0, 'Create': 1, 'CreateEffect': 1, 'PlaySound': 0, 'RemoveExtras': 1,
               'SetDoorOpen': 0}
GLOBAL_ACTORS = {'HERO', 'Hero', 'RETURN', 'NULL'}
RETAIL_TITLES = {'OBJECT_HERO_TITLE_' + n for n in (
    'DUMMY_TEMPLATE BEGINNER_STRENGTH BEGINNER_SKILL BEGINNER_WILL REAPER SHADOWHUNTER MALEFICUS DEATHBRINGER '
    'ASSASSIN NECROMANCER AVATAR PILGRIM LIBERATOR PALADIN DRUID RANGER RUNEMASTER HOOD GLADIATOR SABRE '
    'ARROWDODGER PIEMASTER CHICKEN_CHASER ARSEFACE JACK MAZE SCARLET_ROBE SCYTHE THUNDER WHISPER TWINBLADE '
    'BRIAR_ROSE LADY_GREY GUILDMASTER SCORPION_SLAYER DEATH_BRINGER').split()}


class Lint:
    def __init__(self) -> None:
        self.problems: list[dict] = []
        self.notes: list[str] = []

    def problem(self, where: str, what: str) -> None:
        self.problems.append({'where': where, 'what': what})

    def note(self, text: str) -> None:
        self.notes.append(text)


# --- retail name sources --------------------------------------------------------

def load_verbs() -> dict[str, int | None]:
    verbs: dict[str, int | None] = {}
    for row in VERB_TABLE.read_text(encoding='utf-8').splitlines()[1:]:
        cols = row.split('\t')
        if cols and cols[0]:
            verbs[cols[0]] = int(cols[2]) if len(cols) > 2 and cols[2].isdigit() else None
    return verbs


def retail_text(game: pathlib.Path, lint: Lint):
    big = game / 'data/lang/English/text.big'
    if not big.exists():
        lint.note(f'text.big not found at {big}; text-key and narrator checks skipped')
        return None, None
    import text_build
    bank = text_build.TextBank(str(big))
    return set(bank.by_name), set(bank.narrators)


def retail_tng_names(game: pathlib.Path, lint: Lint) -> set[str] | None:
    wad = game / 'data/Levels/FinalAlbion.wad'
    if not wad.exists() or not FORGE_TOOLS.exists():
        lint.note('FinalAlbion.wad or forge-tools missing; TNG target checks skipped')
        return None
    tngs = sorted(TNG_CACHE.rglob('*.tng'))
    if not tngs:
        TNG_CACHE.mkdir(parents=True, exist_ok=True)
        subprocess.run([str(FORGE_TOOLS), 'wad', 'extract', str(wad), str(TNG_CACHE), 'StartOakVale'],
                       capture_output=True, text=True)
        tngs = sorted(TNG_CACHE.rglob('*.tng'))
        if not tngs:
            lint.note('wad extract produced no StartOakVale TNGs; TNG target checks skipped')
            return None
    names: set[str] = set()
    for tng in tngs:
        out = subprocess.run([str(FORGE_TOOLS), 'tng', 'list', str(tng)], capture_output=True, text=True).stdout
        for row in out.splitlines()[1:]:
            cols = row.split()
            if len(cols) >= 2 and cols[1] != 'NULL':
                names.add(cols[1])
    return names


# --- .cs parsing ------------------------------------------------------------------

def parse_cs(path: pathlib.Path) -> dict[str, list[tuple[int, str]]]:
    sections: dict[str, list[tuple[int, str]]] = {'Macro': []}
    current = 'Macro'
    for n, raw in enumerate(path.read_text(encoding='utf-8').splitlines(), 1):
        line = raw.strip()
        if not line or line.startswith('#'):
            continue
        m = re.fullmatch(r'\[(\w+)\]', line)
        if m:
            current = m.group(1)
            sections.setdefault(current, [])
            continue
        sections[current].append((n, line))
    return sections


def split_command(line: str) -> tuple[str | None, str, list[str]]:
    """'Father.Speak Father,'TEXT_X'' -> ('Father', '.Speak', ['Father', "'TEXT_X'"])"""
    head, _, rest = line.partition(' ')
    actor, verb = None, head
    if '.' in head and not head.startswith('.'):
        actor, _, verb = head.partition('.')
        verb = '.' + verb
    args = [a.strip() for a in rest.split(',')] if rest.strip() else []
    return actor, verb, args


def lint_cutscene(cs: dict, m: dict, lint: Lint, verbs: dict, text_keys: set | None,
                  line_keys: set, tng_names: set | None, marker_names: set) -> None:
    name = cs['name']
    if bool(cs.get('source')) == bool(cs.get('clone_of')):
        lint.problem(name, 'needs exactly one of source / clone_of')
        return
    if cs.get('clone_of'):
        ins = cs.get('insert_before') or {}
        for line in ins.get('lines', []):
            actor, verb, args = split_command(line)
            if verb not in verbs:
                lint.problem(name, f'insert_before: unknown verb {verb!r}')
            for key in TEXT_KEY.findall(line):
                if text_keys is not None and key not in line_keys and key not in text_keys:
                    lint.problem(name, f'insert_before: text key {key} is neither retail nor a manifest line')
        return
    path = m['_dir'] / cs['source']
    if not path.exists():
        lint.problem(name, f'source missing: {cs["source"]}')
        return
    sections = parse_cs(path)
    for extra in set(sections) - {'Macro', 'SkipCond', 'SetupCond', 'Lights', 'LightScene', 'Sound', 'Answer0', 'Answer1'}:
        lint.problem(f'{path.name}', f'unknown section [{extra}]')
    actors = set(cs.get('actors', [])) | GLOBAL_ACTORS
    for section in ('SetupCond', 'Macro', 'SkipCond'):
        for n, line in sections.get(section, []):
            if line == '""':
                continue
            actor, verb, args = split_command(line)
            if verb == 'RegisterActor' and args:
                actors.add(args[0])
            if verb == 'Create' and len(args) >= 3:
                actors.add(args[2])
    for section in ('Macro', 'SkipCond', 'SetupCond'):
        for n, line in sections.get(section, []):
            if line == '""':
                continue
            where = f'{path.name}:{n}'
            actor, verb, args = split_command(line)
            if verb not in verbs:
                lint.problem(where, f'unknown verb {verb!r}')
                continue
            if actor is not None and actor not in actors:
                lint.problem(where, f'actor {actor!r} is not declared (actors: {sorted(actors)})')
            for key in TEXT_KEY.findall(line):
                if key in line_keys or (text_keys is not None and key in text_keys):
                    continue
                if text_keys is None:
                    continue
                lint.problem(where, f'text key {key} is neither retail nor a manifest line')
            idx = TARGET_ARGS.get(verb)
            if idx is not None and len(args) > idx and tng_names is not None:
                target = args[idx]
                if target in tng_names or target in marker_names or target in actors:
                    continue
                if re.fullmatch(r'[A-Z][A-Z0-9_]+', target) and not target.startswith(('SND_', 'MUSIC_', 'CREATURE_')):
                    lint.problem(where, f'{verb} target {target!r} is not a retail StartOakVale thing, manifest marker or actor')


# --- manifest + lua ----------------------------------------------------------------

def run(manifest: pathlib.Path, lua: pathlib.Path | None) -> Lint:
    lint = Lint()
    m = oakvale_manifest.load(manifest)
    game = pathlib.Path(m['game_root'])
    verbs = load_verbs()
    text_keys, narrators = retail_text(game, lint)
    tng_names = retail_tng_names(game, lint)
    voices = m.get('voices') or {}

    line_keys: set[str] = set()
    for ln in m.get('lines') or []:
        key = ln.get('key', '')
        if key in line_keys:
            lint.problem('lines', f'duplicate key {key}')
        line_keys.add(key)
        if not key.startswith('TEXT_OVR_'):
            lint.problem('lines', f'{key}: keys must start with TEXT_OVR_')
        if text_keys is not None and key in text_keys:
            lint.problem('lines', f'{key} already exists in retail text.big')
        if not ln.get('text'):
            lint.problem('lines', f'{key}: empty text')
        voice = ln.get('voice')
        if voice and voice not in voices:
            lint.problem('lines', f'{key}: voice {voice!r} is not in voices')
        speaker = ln.get('speaker') or (voices.get(voice) or {}).get('speaker')
        if not speaker:
            lint.problem('lines', f'{key}: no speaker (line.speaker or voices[voice].speaker)')
        elif narrators is not None and speaker not in narrators:
            lint.problem('lines', f'{key}: speaker {speaker!r} is not a retail NarratorList name')
        if ln.get('vo') and not ln.get('bank'):
            lint.problem('lines', f'{key}: vo: true needs bank (e.g. ScriptDialogue2.lug)')

    marker_names = {mk['name'] for mk in m.get('markers') or []}
    title_names = set()
    for t in m.get('titles') or []:
        if not t['name'].startswith('OBJECT_HERO_TITLE_'):
            lint.problem('titles', f'{t["name"]}: must start with OBJECT_HERO_TITLE_')
        if t['donor'] not in RETAIL_TITLES:
            lint.problem('titles', f'{t["name"]}: donor {t["donor"]} is not a retail title')
        for cat in oakvale_manifest.CATEGORIES:
            missing = [vt for vt in oakvale_manifest.VOICE_TYPES if vt not in (t.get('lines') or {}).get(cat, {})]
            if missing:
                lint.problem('titles', f'{t["name"]} {cat}: no line for voice type(s) {missing}')
        title_names.add(t['name'])
    cutscene_names: set[str] = set()
    for cs in m.get('cutscenes') or []:
        if cs['name'] in cutscene_names:
            lint.problem('cutscenes', f'duplicate cutscene {cs["name"]}')
        cutscene_names.add(cs['name'])
        if not cs['name'].startswith('CS_OVR_'):
            lint.problem('cutscenes', f'{cs["name"]}: names must start with CS_OVR_')
        lint_cutscene(cs, m, lint, verbs, text_keys, line_keys, tng_names, marker_names)

    for sc in m.get('scenes') or []:
        if sc.get('cutscene') and sc['cutscene'] not in cutscene_names:
            lint.problem('scenes', f'{sc.get("id")}: cutscene {sc["cutscene"]} is not a manifest cutscene')

    lua = lua or (m['_dir'] / m.get('lua', 'FSE'))
    if lua.exists():
        for path in sorted(lua.rglob('*.lua')):
            src = path.read_text(encoding='utf-8', errors='replace')
            rel = path.relative_to(lua).as_posix()
            for macro in set(LUA_MACRO.findall(src)) | set(LUA_MACRO_LITERAL.findall(src)):
                if macro.startswith('CS_OVR_') and macro not in cutscene_names:
                    lint.problem(rel, f'"{macro}" is not a manifest cutscene')
            for key in set(LUA_TEXT.findall(src)):
                if key not in line_keys:
                    lint.problem(rel, f'"{key}" is not a manifest line')
            for name in set(LUA_TITLE.findall(src)):
                if name not in title_names and name not in RETAIL_TITLES:
                    lint.problem(rel, f'"{name}" is neither a manifest title nor a retail one')
    else:
        lint.note(f'lua tree {lua} missing; Lua checks skipped')
    return lint


def main() -> int:
    a = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    a.add_argument('--manifest', type=pathlib.Path, default=DEFAULT_MANIFEST)
    a.add_argument('--lua', type=pathlib.Path)
    a.add_argument('--json', type=pathlib.Path)
    args = a.parse_args()
    lint = run(args.manifest, args.lua)
    for note in lint.notes:
        print(f'note: {note}')
    for p in lint.problems:
        print(f'{p["where"]}: {p["what"]}')
    print(f'cs_lint: {len(lint.problems)} problem(s)')
    if args.json:
        args.json.write_text(json.dumps({'problems': lint.problems, 'notes': lint.notes}, indent=1), encoding='utf-8')
    return 1 if lint.problems else 0


if __name__ == '__main__':
    raise SystemExit(main())
