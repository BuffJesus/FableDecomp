#!/usr/bin/env python3
"""Compare unambiguous native constant rows with reconstructed Lua constants."""
from __future__ import annotations

import json
import re
from pathlib import Path

try:
    from tools.script_recovery.validate_reconstructed_package import strip_lua_comments
except ModuleNotFoundError:
    from validate_reconstructed_package import strip_lua_comments


# Contextual rows describe a split value, use-site operand, shared-module field, or
# differently named Lua constant. Each entry names all executable evidence required.
CONTEXTUAL_ROWS = {
    ("NOVI_AffairMan", "DAT_01375748"): [("@entity", r"PlayAnimation\([^\n]+, true, true, false, false\)")],
    ("NOVI_AffairWife", "TIMER_FIELD"): [
        ("@entity", r"F\.TalkIntermittentTimer"),
        ("fields.lua", r'F\.TalkIntermittentTimer\s*=\s*key\("TalkIntermittentTimer",\s*INT,\s*0x104,')],
    ("NOVI_AffairWife", "DAT_01375748"): [("@entity", r"PlayAnimation\([^\n]+, true, true, false, false\)")],
    ("NOVI_AffairWife", "ZERO_POSITION"): [
        ("@entity", r"local ZERO_POSITION\s*=\s*\{\s*x\s*=\s*0\.0,\s*y\s*=\s*0\.0,\s*z\s*=\s*0\.0\s*\}"),
        ("@entity", r"or ZERO_POSITION")],
    ("NOVI_AffairWoman", "DAT_01375748"): [("@entity", r"PlayAnimation\([^\n]+, true, true, false, false\)")],
    ("NOVI_AffairWoman", "ZERO_POSITION"): [
        ("@entity", r"local ZERO_POSITION\s*=\s*\{\s*x\s*=\s*0\.0,\s*y\s*=\s*0\.0,\s*z\s*=\s*0\.0\s*\}"),
        ("@entity", r"target\s*=\s*ZERO_POSITION")],
    ("NOVI_BarrelMan", "FADE_OUT_SECONDS / FADE_HOLD_SECONDS"): [
        ("@entity", r"local FADE_OUT_SECONDS\s*=\s*1\.0"),
        ("@entity", r"local FADE_HOLD_SECONDS\s*=\s*1\.0"),
        ("@entity", r"FadeScreenOut\(FADE_OUT_SECONDS, FADE_HOLD_SECONDS\)")],
    ("NOVI_BarrelThug", "TEMPT_70 condition"): [
        ("@entity", r"local LAST_TIER_TIMER_MAX\s*=\s*44"),
        ("@entity", r"local LAST_TIER_LAST_MIN\s*=\s*46"),
        ("@entity", r"timer <= LAST_TIER_TIMER_MAX and LastTimeSpoken >= LAST_TIER_LAST_MIN")],
    ("NOVI_BarrelThug", "WELLDONE_40 condition"): [
        ("@entity", r"local LAST_TIER_TIMER_MAX\s*=\s*44"),
        ("@entity", r"local LAST_TIER_LAST_MIN\s*=\s*46"),
        ("@entity", r"timer <= LAST_TIER_TIMER_MAX and LastTimeSpoken >= LAST_TIER_LAST_MIN")],
    ("NOVI_BookTrader", "GiveHeroObject amount"): [("@entity", r"GiveHeroObject\(SWEETS_OBJECT, -1\)")],
    ("NOVI_Bully", "PlayAnimation flag DAT_01375748"): [("@entity", r"PlayAnimation\([^\n]+, true, true, false, false\)")],
    ("NOVI_Guard", "HEAL_LIFE_ABILITY"): [
        ("@entity", r"local EXCLUDED_HIT_ABILITY\s*=\s*14"),
        ("@entity", r"MsgIsHitByHeroSpecialAbility\(EXCLUDED_HIT_ABILITY\)")],
    ("Q_NewOakValeIntro", "GUI_BULLY_HEALTH_COUNTER_INIT"): [
        ("fields.lua", r'F\.GUIBullyHealthCounter\s*=\s*key\("GUIBullyHealthCounter",\s*INT,\s*0x64,\s*-999\)')],
    ("Q_NewOakValeIntro", "DEED_MORALITY_AMOUNT"): [
        ("deeds.lua", r"Deeds\.MORALITY_CHANGE_PER_DEED\s*=\s*0\.001"),
        ("deeds.lua", r"GiveHeroMorality\(-?Deeds\.MORALITY_CHANGE_PER_DEED\)")],
}

CONTEXTUAL_VALUES = {
    ("NOVI_AffairMan", "DAT_01375748"): True,
    ("NOVI_AffairWife", "TIMER_FIELD"): "PARENT+0x104 TalkIntermittentTimer",
    ("NOVI_AffairWife", "DAT_01375748"): True,
    ("NOVI_AffairWife", "ZERO_POSITION"): "DAT_0143e8e0",
    ("NOVI_AffairWoman", "DAT_01375748"): True,
    ("NOVI_AffairWoman", "ZERO_POSITION"): "DAT_0143e8e0",
    ("NOVI_BarrelMan", "FADE_OUT_SECONDS / FADE_HOLD_SECONDS"): [1.0, 1.0],
    ("NOVI_BarrelThug", "TEMPT_70 condition"): "timer <= 44 && LastTimeSpoken >= 46",
    ("NOVI_BarrelThug", "WELLDONE_40 condition"): "timer <= 44 && LastTimeSpoken >= 46",
    ("NOVI_BookTrader", "GiveHeroObject amount"): -1,
    ("NOVI_Bully", "PlayAnimation flag DAT_01375748"): True,
    ("NOVI_Guard", "HEAL_LIFE_ABILITY"): 14,
    ("Q_NewOakValeIntro", "GUI_BULLY_HEALTH_COUNTER_INIT"): -999,
    ("Q_NewOakValeIntro", "DEED_MORALITY_AMOUNT"): 0.001,
}


def _scalar(text: str):
    text = text.strip().rstrip(",")
    if text == "true":
        return True
    if text == "false":
        return False
    if len(text) >= 2 and text[0] == text[-1] and text[0] in "\"'":
        return text[1:-1]
    try:
        return int(text, 0)
    except ValueError:
        try:
            return float(text)
        except ValueError:
            return None


def _declaration(source: str, name: str):
    match = re.search(rf"^local\s+{re.escape(name)}\s*=\s*([^\n]*)", source, re.MULTILINE)
    if not match:
        return None
    first = match.group(1).strip()
    if first.startswith("{") and first.endswith("}"):
        body = first[1:-1]
        indexed = re.findall(r"\[\s*(\d+)\s*\]\s*=\s*([^,}]+)", body)
        if indexed:
            values = {int(key): _scalar(value) for key, value in indexed}
            return [values[key] for key in sorted(values)]
        named = re.findall(r"\b([A-Za-z_]\w*)\s*=\s*([^,}]+)", body)
        if named:
            return {key: _scalar(value) for key, value in named}
        return [_scalar(value) for value in body.split(",") if value.strip()]
    if first != "{":
        return _scalar(first)
    tail = source[match.end():]
    end = re.search(r"^\}", tail, re.MULTILINE)
    if not end:
        return None
    body = tail[:end.start()]
    pairs = []
    for item in re.finditer(r"\{\s*([^,{}]+)\s*,\s*([^,{}]+)\s*\}", body):
        pairs.append([_scalar(item.group(1)), _scalar(item.group(2))])
    return pairs or None


def _equal(native, lua) -> bool:
    if isinstance(native, list) and isinstance(lua, list):
        if len(native) != len(lua):
            return False
        return all(_equal(a, b) for a, b in zip(native, lua))
    if isinstance(native, str) and native.startswith("_") and isinstance(lua, str):
        return lua.endswith(native)
    return native == lua


def audit(entities_dir: Path, lua_root: Path) -> dict:
    errors: list[str] = []
    total = matched = excluded = 0
    seen_exclusions = set()
    for inventory in sorted(entities_dir.glob("*.json")):
        data = json.loads(inventory.read_text(encoding="utf-8-sig"))
        lua_path = (lua_root / "NewOakValeIntro.lua" if inventory.stem == "Q_NewOakValeIntro"
                    else lua_root / "Entities" / f"{inventory.stem}.lua")
        source = strip_lua_comments(lua_path.read_text(encoding="utf-8-sig"))
        for row in data.get("constants", []):
            total += 1
            key = (inventory.stem, row["name"])
            if key in CONTEXTUAL_ROWS:
                excluded += 1
                seen_exclusions.add(key)
                if not _equal(row.get("value"), CONTEXTUAL_VALUES[key]):
                    errors.append(f"{inventory.name}: unexpected contextual value for {row['name']}")
                for relative, pattern in CONTEXTUAL_ROWS[key]:
                    evidence_source = source if relative == "@entity" else strip_lua_comments(
                        (lua_root / relative).read_text(encoding="utf-8-sig"))
                    if not re.search(pattern, evidence_source):
                        errors.append(f"{inventory.name}: contextual evidence missing for {row['name']}: {pattern}")
                continue
            actual = _declaration(source, row["name"])
            if actual is None:
                errors.append(f"{inventory.name}: missing/unparseable Lua constant {row['name']}")
            elif not _equal(row["value"], actual):
                errors.append(
                    f"{inventory.name}: {row['name']} native={row['value']!r} Lua={actual!r}"
                )
            else:
                matched += 1
    missing_exclusions = set(CONTEXTUAL_ROWS) - seen_exclusions
    if set(CONTEXTUAL_ROWS) != set(CONTEXTUAL_VALUES):
        errors.append("contextual evidence/value classifications disagree")
    if missing_exclusions:
        errors.append("stale non-literal classifications: " + repr(sorted(missing_exclusions)))
    return {
        "schema": "new-oakvale-constant-fidelity-audit/0.1",
        "inventoryConstants": total,
        "literalMatched": matched,
        "contextualVerified": excluded,
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    root = Path(__file__).resolve().parents[2]
    print(json.dumps(audit(
        root / "refs/script_recovery/new_oakvale_intro/entities",
        root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro"), indent=2))
