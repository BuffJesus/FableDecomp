#!/usr/bin/env python3
"""Cross-check cutscene and spawned-thread inventory metadata with Lua."""
from __future__ import annotations

import json
import re
from pathlib import Path

try:
    from tools.script_recovery.validate_reconstructed_package import strip_lua_comments
except ModuleNotFoundError:
    from validate_reconstructed_package import strip_lua_comments


def audit(entities_dir: Path, lua_root: Path) -> dict:
    errors: list[str] = []
    cutscenes = named = actor_keys = threads = 0
    for inventory in sorted(entities_dir.glob("*.json")):
        data = json.loads(inventory.read_text(encoding="utf-8-sig"))
        lua_path = (lua_root / "NewOakValeIntro.lua" if inventory.stem == "Q_NewOakValeIntro"
                    else lua_root / "Entities" / f"{inventory.stem}.lua")
        source = strip_lua_comments(lua_path.read_text(encoding="utf-8-sig"))
        for index, row in enumerate(data.get("cutscenes", [])):
            cutscenes += 1
            label = f"{inventory.name} cutscenes[{index}]"
            if isinstance(row, str):
                names = re.findall(r"\bCS_[A-Z0-9_]+\b", row)
                if len(names) != 1 or not re.search(rf'["\']{re.escape(names[0])}["\']', source):
                    errors.append(f"{label}: prose cutscene name does not resolve in Lua")
                else:
                    named += 1
                continue
            if not isinstance(row, dict) or not (row.get("name") or row.get("kind")):
                errors.append(f"{label}: expected named or typed cutscene record")
                continue
            if row.get("name"):
                named += 1
                name = str(row["name"])
                if not re.search(rf'["\']{re.escape(name)}["\']', source):
                    errors.append(f"{label}: named cutscene {name} absent from Lua")
                actors = row.get("actors", {})
                if not isinstance(actors, dict):
                    errors.append(f"{label}: actors must be a mapping")
                else:
                    for actor in actors:
                        actor_keys += 1
                        if not re.search(rf'(?:\b{re.escape(actor)}\s*=|["\']{re.escape(actor)}["\'])', source):
                            errors.append(f"{label}: actor key {actor} absent from Lua setup")
            else:
                if not isinstance(row.get("where"), str) or not row["where"].strip():
                    errors.append(f"{label}: generic movie record lacks where evidence")
                if not re.search(r"\b(?:StartMovieSequence|begin_movie)\s*\(", source):
                    errors.append(f"{label}: no executable movie-sequence acquisition in Lua")
        for index, row in enumerate(data.get("spawnedThreads", [])):
            threads += 1
            label = f"{inventory.name} spawnedThreads[{index}]"
            if set(row) != {"name", "spawnedBy", "lua"}:
                errors.append(f"{label}: expected exactly name/spawnedBy/lua")
                continue
            name = str(row["name"])
            if not re.search(rf"^function\s+{re.escape(name)}\s*\(", source, re.MULTILINE):
                errors.append(f"{label}: thread function {name} absent from Lua")
            call = str(row["lua"])
            if call not in source:
                errors.append(f"{label}: exact CreateThread call absent for {name}")
            spawner = str(row["spawnedBy"]).split(" ", 1)[0]
            if not re.search(rf"^function\s+{re.escape(spawner)}\s*\(", source, re.MULTILINE):
                errors.append(f"{label}: spawning function {spawner} absent from Lua")
    return {
        "schema": "new-oakvale-cutscene-thread-audit/0.1",
        "cutsceneRecords": cutscenes,
        "namedCutscenes": named,
        "actorKeys": actor_keys,
        "spawnedThreads": threads,
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    root = Path(__file__).resolve().parents[2]
    print(json.dumps(audit(
        root / "refs/script_recovery/new_oakvale_intro/entities",
        root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro"), indent=2))
