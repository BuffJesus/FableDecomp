#!/usr/bin/env python3
"""Prove retail 0x00DAEEB0 is NOVI_Victim::Init despite its donor label."""
from __future__ import annotations

import argparse
import json
from pathlib import Path


def _record(archive: str, address: str) -> str:
    needle = f'"address":"{address.lower()}"'
    return next((line for line in archive.splitlines() if line.lower().startswith("{" + needle)), "")


def audit(translation_unit: Path, donor_pdb: Path, donor_layouts: Path,
          victim_evidence: Path, trader_evidence: Path) -> dict:
    archive = translation_unit.read_text(encoding="utf-8-sig")
    victim_alloc = _record(archive, "0x00DAEDE0")
    victim_init = _record(archive, "0x00DAEEB0")
    trader_alloc = _record(archive, "0x00DB3E30")
    trader_init = _record(archive, "0x00DB3F00")
    pdb = donor_pdb.read_text(encoding="utf-8-sig")
    layouts = donor_layouts.read_text(encoding="utf-8-sig")
    victim = json.loads(victim_evidence.read_text(encoding="utf-8-sig"))
    trader = json.loads(trader_evidence.read_text(encoding="utf-8-sig"))
    checks = {
        "victimAllocatorInstallsRetailVtable": all(x in victim_alloc for x in (
            "operator_new(0x20)", "0x012D87B8", "puVar2[5]", "puVar2[6]")),
        "victimVtableInitSlotTargetsDaeeb0":
            '"pointedFrom":[{"site":"0x012D87C0"' in victim_init,
        "victimInitZerosBothVictimFields": all(x in victim_init for x in (
            "this[0x1d]", "this[0x1c]")),
        "donorPdbNamesVictimInitAndMain": all(x in pdb for x in (
            "?Init@CNOVI_Victim@CQ_NewOakValeIntroScript@NScript@@EAEXXZ",
            "?Main@CNOVI_Victim@CQ_NewOakValeIntroScript@NScript@@EAEXXZ")),
        "donorLayoutNamesVictimOffsets1c1d": all(x in layouts for x in (
            "CNOVI_Victim\t32\t28\tbool\tDisplayedGameInfo",
            "CNOVI_Victim\t32\t29\tbool\tDoneThanks")),
        "actualBookTraderUsesSeparateVtableAndInit":
            "0x012D8E80" in trader_alloc
            and '"pointedFrom":[{"site":"0x012D8E88"' in trader_init,
        "actualBookTraderInitOnlyZerosOffset1c":
            "this[0x1c]" in trader_init and "this[0x1d]" not in trader_init,
        "entityInventoriesKeepDistinctAddresses":
            victim["functions"]["Init"]["address"].lower() == "0x00daeeb0"
            and trader["functions"]["Init"]["address"].lower() == "0x00db3f00",
    }
    return {"schema": "victim-init-identity-audit/0.1", "ok": all(checks.values()), "checks": checks}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--translation-unit", type=Path, required=True)
    parser.add_argument("--donor-pdb", type=Path, required=True)
    parser.add_argument("--donor-layouts", type=Path, required=True)
    parser.add_argument("--victim-evidence", type=Path, required=True)
    parser.add_argument("--trader-evidence", type=Path, required=True)
    args = parser.parse_args()
    result = audit(args.translation_unit, args.donor_pdb, args.donor_layouts,
                   args.victim_evidence, args.trader_evidence)
    print(json.dumps(result, indent=2))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
