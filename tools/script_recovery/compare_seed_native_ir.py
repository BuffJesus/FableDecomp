#!/usr/bin/env python3
"""Compare reconstructed seed literals/bindings/persistence keys with native lifecycle IR."""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path
from typing import Any

try:
    from tools.script_recovery.extract_native_operation_ir import STRING_RE
except ModuleNotFoundError:  # direct script execution
    from extract_native_operation_ir import STRING_RE


SEEDS = {
    "GuardianSisterInfo": "QS_GuardianSisterInfo",
    "GuardianSisterInfo2": "QS_GuardianSisterInfo2_SisterInBanditCamp",
    "MeetSister": "QS_MeetSister",
    "MazeResearch": "V_MazeResearch",
    "HerosOldHouse": "Q_HerosOldHouse",
    "GuardianTrophyDealerInfo": "QS_GuardianTrophyDealerInfo",
}


def lua_facts(package_dir: Path) -> dict[str, Any]:
    texts = [path.read_text(encoding="utf-8-sig") for path in sorted(package_dir.rglob("*.lua"))
             if path.name.lower() != "quests.lua"]
    text = "\n".join(texts)
    literals = {match.group(1) for match in STRING_RE.finditer(text)}
    bindings = set(re.findall(r'AddEntityBinding\s*\(\s*"([^"]+)"', text))
    persisted = set(re.findall(r'PersistTransfer\w+\s*\([^,]+,\s*"([^"]+)"', text))
    return {"literals": literals, "bindings": bindings, "persistenceKeys": persisted}


def compare(corpus_sources: Path, native_ir_dir: Path) -> dict[str, Any]:
    rows = []
    for package, script in SEEDS.items():
        lua = lua_facts(corpus_sources / package)
        native = json.loads((native_ir_dir / f"{script}.json").read_text(encoding="utf-8-sig"))
        native_literals = {value for function in native["lifecycle"] for value in function["strings"]}
        native_persisted = {row["key"] for function in native["lifecycle"]
                            for row in function["persistenceTransfers"] if row["key"]}
        rows.append({
            "package": package, "nativeScript": script,
            "sharedLiterals": sorted(lua["literals"] & native_literals),
            "luaBindingsFoundNative": sorted(lua["bindings"] & native_literals),
            "luaBindingsMissingNativeLifecycle": sorted(lua["bindings"] - native_literals),
            "sharedPersistenceKeys": sorted(lua["persistenceKeys"] & native_persisted),
            "luaPersistenceOnly": sorted(lua["persistenceKeys"] - native_persisted),
            "nativePersistenceOnly": sorted(native_persisted - lua["persistenceKeys"]),
        })
    return {"schema": "fable-seed-native-comparison/0.1", "scripts": rows}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--sources", type=Path, required=True)
    parser.add_argument("--native-ir", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    result = compare(args.sources, args.native_ir)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"scripts": len(result["scripts"]),
                      "sharedLiterals": sum(len(row["sharedLiterals"]) for row in result["scripts"])}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
