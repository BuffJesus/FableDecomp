#!/usr/bin/env python3
"""Extract the retail New Oakvale scripted-thing placement/AI baseline from loose TNGs.

This is deliberately a read-only evidence collector.  It does not infer positions from Lua and
does not modify the installed game.  Its output is stable JSON suitable for review and fixtures.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import re
from pathlib import Path


DEFAULT_LEVELS = Path(
    r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\data\Levels\FinalAlbion"
)
MAPS = ("StartOakValeEast.tng", "StartOakValeWest.tng")
SCRIPT_RE = re.compile(r"^(NOVI_[A-Za-z0-9_]+|OVI_DeadFather)$")
BLOCK_RE = re.compile(r"(?ms)^NewThing\s+(?P<kind>[^;]+);\s*(?P<body>.*?)^EndThing;")
DEFAULT_QUESTS = (
    Path(__file__).resolve().parents[2]
    / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/quests.lua"
)


def field(body: str, name: str) -> str | None:
    match = re.search(rf"(?m)^{re.escape(name)}\s+(.+);\r?$", body)
    if not match:
        return None
    value = match.group(1).strip()
    if len(value) >= 2 and value[0] == value[-1] == '"':
        return value[1:-1]
    return value


def number(body: str, name: str) -> int | float | None:
    value = field(body, name)
    if value is None:
        return None
    try:
        return int(value)
    except ValueError:
        return float(value)


def boolean(body: str, name: str) -> bool | None:
    value = field(body, name)
    if value is None:
        return None
    if value == "TRUE":
        return True
    if value == "FALSE":
        return False
    raise ValueError(f"unexpected boolean {name}={value!r}")


def vector(body: str, prefix: str) -> list[float] | None:
    values = [number(body, prefix + axis) for axis in ("X", "Y", "Z")]
    if any(value is None for value in values):
        return None
    return [float(value) for value in values]


def extract(levels: Path, quests_lua: Path | None = None) -> dict[str, object]:
    records: list[dict[str, object]] = []
    sources: list[dict[str, object]] = []
    for map_name in MAPS:
        path = levels / map_name
        data = path.read_bytes()
        text = data.decode("utf-8-sig")
        sources.append(
            {
                "map": map_name,
                "bytes": len(data),
                "sha256": hashlib.sha256(data).hexdigest().upper(),
            }
        )
        for match in BLOCK_RE.finditer(text):
            body = match.group("body")
            script = field(body, "ScriptName")
            if script is None or SCRIPT_RE.fullmatch(script) is None:
                continue
            records.append(
                {
                    "map": map_name,
                    "thingKind": match.group("kind").strip(),
                    "uid": field(body, "UID"),
                    "scriptName": script,
                    "definitionType": field(body, "DefinitionType"),
                    "positionLocal": vector(body, "Position"),
                    "positionInitialWorld": vector(body, "InitialPos"),
                    "forward": vector(body, "RHSetForward"),
                    "health": number(body, "Health"),
                    "overridingBrainName": field(body, "OverridingBrainName"),
                    "targetable": boolean(body, "Targetable"),
                    "hasInformation": boolean(body, "HasInformation"),
                    "wanderWithInformation": boolean(body, "WanderWithInformation"),
                    "continueAIWithInformation": boolean(body, "ContinueAIWithInformation"),
                    "enableCreatureAutoPlacing": boolean(body, "EnableCreatureAutoPlacing"),
                    "allowedToFollowHero": boolean(body, "AllowedToFollowHero"),
                    "respondingToFollowAndWait": boolean(body, "RespondingToFollowAndWait"),
                    "forcedAttitude": number(body, "ForcedAttitude"),
                    "heroOpinionEnemy": boolean(body, "HeroOpinionEnemy"),
                    "friendsWithEverythingFlag": boolean(body, "FriendsWithEverythingFlag"),
                    "factionName": field(body, "FactionName"),
                    "numberOfTimesHit": number(body, "NumberOfTimesHit"),
                    "toleranceToBeingHitOverride": number(body, "ToleranceToBeingHitOverride"),
                    "canBeCourted": boolean(body, "CanBeCourted"),
                    "canBeMarried": boolean(body, "CanBeMarried"),
                    "thingGamePersistent": boolean(body, "ThingGamePersistent"),
                    "thingLevelPersistent": boolean(body, "ThingLevelPersistent"),
                }
            )
    records.sort(key=lambda item: (str(item["map"]), str(item["scriptName"]), str(item["uid"])))
    counts: dict[str, int] = {}
    for record in records:
        name = str(record["scriptName"])
        counts[name] = counts.get(name, 0) + 1
    result: dict[str, object] = {
        "schema": "new-oakvale-retail-tng-npc-baseline/1",
        "evidence": "installed retail loose TNG records; values copied without coordinate conversion",
        "sources": sources,
        "recordCount": len(records),
        "countsByScript": dict(sorted(counts.items())),
        "records": records,
    }
    if quests_lua is not None:
        lua = quests_lua.read_text(encoding="utf-8")
        registered = sorted(
            name
            for name in set(re.findall(r'\{\s*name\s*=\s*"([^"]+)"', lua))
            if SCRIPT_RE.fullmatch(name)
        )
        tng_scripts = set(counts)
        registered_scripts = set(registered)
        result["questRegistrationCrossCheck"] = {
            "source": quests_lua.name,
            "registered": registered,
            "tngNotRegistered": sorted(tng_scripts - registered_scripts),
            "registeredNotInInitialTng": sorted(registered_scripts - tng_scripts),
        }
    return result


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--levels", type=Path, default=DEFAULT_LEVELS)
    parser.add_argument("--quests-lua", type=Path, default=DEFAULT_QUESTS)
    parser.add_argument("--output", type=Path)
    parser.add_argument(
        "--check",
        type=Path,
        help="compare the extracted baseline with an existing JSON fixture and make no changes",
    )
    args = parser.parse_args()
    result = extract(args.levels, args.quests_lua)
    rendered = json.dumps(result, indent=2) + "\n"
    if args.check:
        expected = json.loads(args.check.read_text(encoding="utf-8"))
        if result != expected:
            print(f"baseline differs from {args.check}")
            return 1
        print(f"baseline matches {args.check} ({result['recordCount']} records)")
        return 0
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(rendered, encoding="utf-8")
    else:
        print(rendered, end="")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
