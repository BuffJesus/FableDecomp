"""Retail enum operands, so the readable output names the numbers retail named.

`me:MoveToPosition(pos, 3.0, 1, false, true)` is `ENTITY_MOVE_RUN`, not `1`. The member values come from
the Ego_r PDB dump (`ghidra_out/ego_r_pdb.xml`), never from this file, and every entry below was checked
against a second source (the FSE headers under
`D:/Code/FQT/SourceFilesToReference/FSE/FableScriptExtender-master/`) before being added.

An entry is (method name, 0-based argument index, enum name). The index is the operand's position in the
LUA call, which is the retail signature minus the receiver -- verified at real call sites, because the
binding argument order is not always the native one (see the GOTCHAS entry on AddLineToConversation).
"""
from __future__ import annotations

import functools
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
PDB_XML = ROOT / 'ghidra_out' / 'ego_r_pdb.xml'

OPERANDS = (
    # ?MoveToPosition@CScriptGameResourceObjectScriptedThingBase@@UAEXABVC3DVector@@MW4EScriptEntityMoveType@@_N2@Z
    ('MoveToPosition', 2, 'EScriptEntityMoveType'),
    ('MoveToThing', 2, 'EScriptEntityMoveType'),
    # ?GiveHeroAbility@CGameScriptInterface@@UBEXW4EHeroAbility@@_N@Z
    ('GiveHeroAbility', 0, 'EHeroAbility'),
    ('MsgIsHitByHeroSpecialAbility', 0, 'EHeroAbility'),
    # ?DisplayTutorial@CGameScriptInterface@@UBE_NW4ETutorialCategory@@@Z
    ('DisplayTutorial', 0, 'ETutorialCategory'),
    ('GiveHeroTutorial', 0, 'ETutorialCategory'),
    # ?EntitySetCutsceneBehaviour@CGameScriptInterface@@UBEXABVCScriptThing@@W4ECutsceneBehaviour@@@Z
    ('EntitySetCutsceneBehaviour', 1, 'ECutsceneBehaviour'),
)


@functools.lru_cache(maxsize=None)
def members(enum: str) -> dict:
    """{value: MEMBER_NAME} for one retail enum, read from the PDB dump. Empty when it is not there."""
    if not PDB_XML.exists():
        return {}
    text = PDB_XML.read_text(encoding='utf-8', errors='replace')
    block = re.search(r'name="' + re.escape(enum) + r'"[^>]*>(.*?)</(?:enum|datatype)>', text, re.S)
    if not block:
        return {}
    out = {}
    for name, value in re.findall(r'name="([A-Z][A-Z0-9_]*)"\s+value="(-?\d+)"', block.group(1)):
        out.setdefault(int(value), name)          # first spelling wins; retail has no duplicate values here
    return out


def split_arguments(text: str):
    """Top-level comma split of a call's argument text (nested calls and tables stay whole)."""
    args, depth, start = [], 0, 0
    for i, ch in enumerate(text):
        if ch in '([{':
            depth += 1
        elif ch in ')]}':
            depth -= 1
        elif ch == ',' and depth == 0:
            args.append((start, i))
            start = i + 1
    args.append((start, len(text)))
    return args
