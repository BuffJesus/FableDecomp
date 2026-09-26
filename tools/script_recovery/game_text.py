"""Game text lookup for the script-recovery tools: a key (`TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_TITLE`) ->
the string the player sees, read from the install's English text.big with the byte-exact text.big codec in
tools/text_build.py (EgoCore CTextParser port). Read-only.

    python -m tools.script_recovery.game_text TEXT_QUEST_BANDIT_CAMP_TITLE
    python -m tools.script_recovery.game_text --find WHITE_BALVERINE
    python -m tools.script_recovery.game_text --title Q_WhiteBalverineKnotholeGlade

The runner uses `quest_title` to match a quest's row on the Guild card table when a config gives no `title`.
"""
from __future__ import annotations

import argparse
import functools
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools'))
TEXT_BIG = Path(r'C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\data\lang\English\text.big')


@functools.lru_cache(maxsize=2)
def _bank(path: str = str(TEXT_BIG)):
    from text_build import TextBank     # noqa: E402 (tools/ on sys.path)
    return TextBank(path)


def text(key: str, path: Path | None = None) -> str | None:
    """the string for a text key, None when the bank has no such (type-0) entry"""
    bank = _bank(str(path or TEXT_BIG))
    if key not in bank.by_name or bank.by_name[key]['type'] != 0:
        return None
    return bank.decode(key).get('content')


def find(substring: str, path: Path | None = None) -> list[tuple[str, str]]:
    """(key, string) for every type-0 key containing `substring` (case-insensitive)"""
    bank = _bank(str(path or TEXT_BIG))
    needle = substring.upper()
    return [(e['name'], bank.decode(e['name']).get('content')) for e in bank.entries
            if e['type'] == 0 and needle in e['name'].upper()]


def quest_key(quest: str) -> str:
    """Q_WhiteBalverineKnotholeGlade -> WHITE_BALVERINE_KNOTHOLE_GLADE (the quest-card text keys' stem)"""
    stem = re.sub(r'^(?:Q|QS|V)_', '', quest)
    return re.sub(r'(?<=[a-z0-9])(?=[A-Z])', '_', stem).upper()


def quest_title(quest: str, path: Path | None = None) -> str | None:
    """the quest card's title (`TEXT_QUEST_<STEM>_TITLE`), e.g. 'White Balverine'"""
    return text(f'TEXT_QUEST_{quest_key(quest)}_TITLE', path)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('key', nargs='?')
    ap.add_argument('--find')
    ap.add_argument('--title', metavar='QUEST')
    ap.add_argument('--text-big', type=Path)
    a = ap.parse_args(argv)
    if a.find:
        for k, v in find(a.find, a.text_big):
            print(f'{k:60} {v!r}')
    elif a.title:
        print(quest_title(a.title, a.text_big))
    elif a.key:
        print(text(a.key, a.text_big))
    else:
        ap.error('give a key, --find or --title')


if __name__ == '__main__':
    main()
