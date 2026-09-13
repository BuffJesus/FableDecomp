#!/usr/bin/env python3
"""Certify the installed native chain from barrel inventory insertion to world pickup creation."""
from __future__ import annotations

import hashlib
import json
import struct
from pathlib import Path

try:
    from tools.script_recovery.audit_barrel_reward_component import audit as audit_barrel_component
except ModuleNotFoundError:
    from audit_barrel_reward_component import audit as audit_barrel_component


EXE = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
EXE_SHA256 = "41DC91090AE853715AC06D2E9FC96E5D545381D197ED55D624C642F34509AC10"

ADD_ITEM = 0x0089E780
INSERT_ITEM = 0x008ED1F0
REWARD_ON_DIE = 0x0074F080
CREATE_PICKUP = 0x0074EC50
RELEASE_ITEMS = 0x0074E6E0


class PEImage:
    def __init__(self, path: Path):
        self.data = path.read_bytes()
        pe = struct.unpack_from("<I", self.data, 0x3C)[0]
        self.section_count = struct.unpack_from("<H", self.data, pe + 6)[0]
        optional_size = struct.unpack_from("<H", self.data, pe + 20)[0]
        self.image_base = struct.unpack_from("<I", self.data, pe + 24 + 28)[0]
        self.sections = []
        for index in range(self.section_count):
            offset = pe + 24 + optional_size + index * 40
            virtual_size, rva, raw_size, raw_offset = struct.unpack_from("<IIII", self.data, offset + 8)
            self.sections.append((rva, max(virtual_size, raw_size), raw_offset))

    def at(self, va: int, size: int) -> bytes:
        rva = va - self.image_base
        for section_rva, span, raw_offset in self.sections:
            if section_rva <= rva and rva + size <= section_rva + span:
                start = raw_offset + rva - section_rva
                return self.data[start:start + size]
        raise ValueError(f"VA 0x{va:08X} is not mapped")

    def call_target(self, site: int) -> int:
        encoded = self.at(site, 5)
        if encoded[0] != 0xE8:
            raise ValueError(f"0x{site:08X} is not a direct call")
        return site + 5 + struct.unpack_from("<i", encoded, 1)[0]


def audit(workspace: Path, exe: Path = EXE) -> dict:
    errors: list[str] = []
    calls = {
        "addItemToInsertItem": (0x0089E7F9, INSERT_ITEM),
        "rewardOnDieToCreatePickup": (0x0074F0D6, CREATE_PICKUP),
        "createPickupToGetRewardItems": (0x0074EC77, 0x0074E860),
        "createPickupToCreateObject": (0x0074ECFA, 0x0074E560),
        "createPickupToInsertItem": (0x0074ED6F, INSERT_ITEM),
    }
    resolved: dict[str, str | None] = {}
    exe_hash = None
    if not exe.exists():
        errors.append(f"installed executable missing: {exe}")
    else:
        exe_hash = hashlib.sha256(exe.read_bytes()).hexdigest().upper()
        if exe_hash != EXE_SHA256:
            errors.append(f"installed executable hash differs: {exe_hash}")
        image = PEImage(exe)
        if image.at(REWARD_ON_DIE, 5).hex() != "51538bd957":
            errors.append("CTCContainerRewardHero::OnDie entry bytes differ")
        if image.at(INSERT_ITEM, 4).hex() != "83ec088b":
            errors.append("CTCContainer::InsertItem entry bytes differ")
        for name, (site, expected) in calls.items():
            try:
                actual = image.call_target(site)
                resolved[name] = f"0x{actual:08X}"
                if actual != expected:
                    errors.append(f"{name} resolves to 0x{actual:08X}, expected 0x{expected:08X}")
            except ValueError as exc:
                resolved[name] = None
                errors.append(str(exc))

    decomp = workspace / "ghidra_out/script_recovery/native_support_decompiles/0x0089E780.c"
    if not decomp.exists():
        errors.append("checked-in AddItemToContainer decompile is missing")
    else:
        text = decomp.read_text(encoding="utf-8-sig")
        for fragment in ("(CScriptThing *)0x3c", "CTCContainer::InsertItem(this_00,lVar3"):
            if fragment not in text:
                errors.append(f"AddItemToContainer decompile is missing: {fragment}")

    constructor = workspace / "ghidra_out/script_recovery/native_support_decompiles/0x0074E6B0.c"
    constructor_defaults = False
    if not constructor.exists():
        errors.append("checked-in CTCContainerRewardHero constructor decompile is missing")
    else:
        text = constructor.read_text(encoding="utf-8-sig")
        constructor_defaults = (
            "*(undefined4 *)(this + 0xc) = 0" in text
            and "*(undefined4 *)(this + 0x10) = 0" in text
            and "*(undefined4 *)(this + 0x14) = 0" in text
            and "this[0x18] = (CTCActivationTrigger)0x1" in text
            and "this[0x19] = (CTCActivationTrigger)0x1" in text
        )
        if not constructor_defaults:
            errors.append("reward component constructor no longer proves enabled defaults at +0x18/+0x19")

    enum_xml = (workspace / "ghidra_out/ego_r_pdb.xml").read_text(encoding="utf-8-sig")
    if '<member name="TCI_SUMMONABLE_CREATURE" value="160" />' not in enum_xml:
        errors.append("interface 0xA0 is no longer identified as TCI_SUMMONABLE_CREATURE")
    barrel = audit_barrel_component(workspace)
    if not barrel["ok"]:
        errors.append("New Oakvale barrel component audit failed")
    if "CTCContainerRewardHero" not in barrel["components"]:
        errors.append("New Oakvale barrel lacks CTCContainerRewardHero")
    if "CTCSummonableCreature" in barrel["components"]:
        errors.append("New Oakvale barrel unexpectedly has the OnDie-suppressing summonable component")

    evidence = workspace / "refs/script_recovery/new_oakvale_intro/runtime_evidence/final-barrel-gold-native-release-20260910.md"
    if not evidence.exists():
        errors.append("native release evidence note is missing")

    return {
        "schema": "new-oakvale-final-barrel-gold-release-audit/0.1",
        "installedExeSha256": exe_hash,
        "addresses": {
            "addItemToContainer": f"0x{ADD_ITEM:08X}",
            "insertItem": f"0x{INSERT_ITEM:08X}",
            "rewardOnDie": f"0x{REWARD_ON_DIE:08X}",
            "createItemPickupContainer": f"0x{CREATE_PICKUP:08X}",
            "releaseItems": f"0x{RELEASE_ITEMS:08X}",
        },
        "resolvedCalls": resolved,
        "rewardEnabledByConstructor": constructor_defaults,
        "barrelHasRewardComponent": "CTCContainerRewardHero" in barrel["components"],
        "barrelHasSummonableCreatureComponent": "CTCSummonableCreature" in barrel["components"],
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
