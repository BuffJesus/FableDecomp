#!/usr/bin/env python3
"""oakvale_manifest.py -- load manifest/intro.yaml and expand its derived content.

Shared by build_custom_intro.py, cs_lint.py, elevenlabs_vo.py and table_read.py so
every tool sees the same `lines`. Today the only expansion is hero titles:

    titles:
      - name: OBJECT_HERO_TITLE_BUTCHER_OF_OAKVALE     # the new game.bin OBJECT
        donor: OBJECT_HERO_TITLE_DEATHBRINGER          # retail title to clone (keeps its enum)
        key: BUTCHER                                   # TEXT_OVR_TITLE_BUTCHER_*
        display: "Butcher of Oakvale"
        description: "Become known as the Butcher of Oakvale"
        lines:
          greet:   { AF1: "...", AM1: "...", ... }     # villager voice types
          comment: { ... }
          self:    { ... }

expands to: two subtitle-only lines (TEXT_OVR_TITLE_<KEY>, _DESC) and, per voice type
and category, one voiced line TEXT_OVR_TITLE_<KEY>_<CAT>_01_<TYPE> in Dialogue.lug
spoken by that type (the `voices` map must define AF1, AM1, ... with that speaker),
plus three type-1 group entries TEXT_OVR_TITLE_<KEY>_<CAT> listing the members --
exactly the retail shape of TEXT_AI_HEROTITLE_ARSEFACE_GREET_TO_HERO.
"""
from __future__ import annotations

import pathlib

import yaml

REPO = pathlib.Path(__file__).resolve().parents[2]
DEFAULT_MANIFEST = REPO / 'refs/script_recovery/authored/OakvaleReborn/manifest/intro.yaml'
TITLE_BANK = 'Dialogue.lug'
CATEGORIES = {'greet': 'GREET_TO_HERO', 'comment': 'COMMENT_AT_HERO', 'self': 'COMMENT_TO_SELF'}
VOICE_TYPES = ['AF1', 'AF2', 'AF3', 'AM1', 'AM2', 'AM3', 'CF1', 'CM1', 'INKEEP1',
               'LEV1BDT1', 'LEV2BDT1', 'LEV3BDT1']


def title_text_keys(t: dict) -> dict:
    """Keys a title contributes: gui, desc, groups {cat: key}, members {cat: [keys]}."""
    k = f'TEXT_OVR_TITLE_{t["key"]}'
    return {
        'gui': k, 'desc': f'{k}_DESC',
        'groups': {cat: f'{k}_{suffix}' for cat, suffix in CATEGORIES.items()},
        'members': {cat: [f'{k}_{suffix}_01_{vt}' for vt in VOICE_TYPES if vt in (t.get('lines') or {}).get(cat, {})]
                    for cat, suffix in CATEGORIES.items()},
    }


def expand_titles(m: dict) -> list[dict]:
    """The `lines` a manifest's titles imply (not stored in the yaml)."""
    out: list[dict] = []
    for t in m.get('titles') or []:
        keys = title_text_keys(t)
        out.append({'key': keys['gui'], 'speaker': 'NONE', 'vo': False, 'text': t['display'],
                    'title': t['key'], 'beat': 'title'})
        out.append({'key': keys['desc'], 'speaker': 'NONE', 'vo': False, 'text': t['description'],
                    'title': t['key'], 'beat': 'title'})
        for cat, suffix in CATEGORIES.items():
            for vt in VOICE_TYPES:
                text = (t.get('lines') or {}).get(cat, {}).get(vt)
                if not text:
                    continue
                out.append({'key': f'{keys["gui"]}_{suffix}_01_{vt}', 'voice': vt, 'vo': True,
                            'bank': TITLE_BANK, 'text': text, 'direction': t.get('direction', {}).get(vt),
                            'title': t['key'], 'beat': f'title {t["key"]} {cat}'})
    return out


def load(path: pathlib.Path = DEFAULT_MANIFEST) -> dict:
    m = yaml.safe_load(path.read_text(encoding='utf-8'))
    m['_dir'] = path.parent.parent
    m['_path'] = path
    m['lines'] = list(m.get('lines') or []) + expand_titles(m)
    return m
