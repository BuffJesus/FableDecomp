#!/usr/bin/env python3
"""Focused synthetic tests for the save-browser metadata join."""

import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))

from tools import save_edit, save_metadata
from tools.save_install import Profile


def make_header(region="TXT_REGION_GREATWOOD", world_frame=740):
    out = bytearray(b"HEADER\0\0\0\0\0")

    def tag(name):
        out.extend(struct.pack("<I", save_edit.getcrc0(name)))

    def ascii_value(value):
        out.extend(value.encode("latin1") + b"\0")

    def wide_value(value):
        out.extend(value.encode("utf-16le") + b"\0\0")

    tag("WorldName"); ascii_value("FinalAlbion")
    tag("WorldFrame"); out.extend(struct.pack("<I", world_frame))
    for name, value in (
        ("TeleportingEnabled", 0), ("SavingEnabled", 1),
        ("ExperienceSpendingEnabled", 1), ("CreatureGenerationEnabled", 1),
    ):
        tag(name); out.append(value)
    tag("CreatureGenerationDisabledGroups"); out.extend(struct.pack("<I", 0))
    for name, value in (
        ("HeroSleepingEnabled", 0), ("MapTableShowQuestCardsOnUsed", 1),
        ("MiniMapEnabled", 1), ("MiniMapActiveBeforeDisabled", 1),
        ("GuildMasterMessagesEnabled", 1),
        ("SummonerDeathExplosionAffectsHero", 0),
    ):
        tag(name); out.append(value)
    tag("MostRecentSaveType"); out.extend(struct.pack("<I", 3))
    tag("MostRecentSaveTypeBeforeManualSave"); out.extend(struct.pack("<I", 2))
    tag("MostRecentManualSaveName"); wide_value("Manual - Save1")
    tag("SaveGameMarkerPos"); out.extend(struct.pack("<3f", 1.0, 2.0, 3.0))
    tag("SaveGameMarkerAngleXY"); out.extend(struct.pack("<f", 4.0))
    tag("GuildSealRecallPos"); out.extend(struct.pack("<3f", 0.0, 0.0, 0.0))
    tag("GuildSealRecallAngleXY"); out.extend(struct.pack("<f", 0.0))
    tag("CurrentRegionName"); ascii_value(region)
    tag("CurrentRegionMinimapGraphicName"); ascii_value("MINIMAP_GREATWOOD")
    tag("TotalTimePlayed"); out.extend(struct.pack("<f", 12.5))

    struct.pack_into("<I", out, 7, len(out) - 11)
    return bytes(out)


def make_save(header):
    return save_edit.build_save(len(header), 0, header)


def make_profile(path, slots):
    text = "".join(
        "SaveGameNames%d  \"%s\";\r\n" % (i, slots.get(i, ""))
        for i in range(1, 51)
    ).encode("latin1")
    total_data_len = 0x10 + len(text)
    raw = (b"FablePro" + struct.pack("<II", 0, total_data_len) +
           text + b"\0\0\0\0")
    profile = Profile(raw)
    path.write_bytes(profile.build())


def test_decode_header():
    values = save_metadata.decode_header(make_header())
    assert values["WorldName"] == "FinalAlbion"
    assert values["WorldFrame"] == 740
    assert values["CurrentRegionName"] == "TXT_REGION_GREATWOOD"
    assert values["MostRecentManualSaveName"] == "Manual - Save1"
    assert values["TotalTimePlayed"] == 12.5


def test_enumerates_registry_and_marks_missing(tmp_path):
    make_profile(tmp_path / "Profile.bin", {1: "Manual - Save1", 2: "Manual - Save2"})
    (tmp_path / "Manual - Save1").write_bytes(make_save(make_header()))
    (tmp_path / "AutoSave").write_bytes(make_save(make_header("TXT_REGION_OAKVALE", 195)))
    (tmp_path / "Unregistered.sav").write_bytes(make_save(make_header()))

    result = save_metadata.enumerate_profile(tmp_path)
    assert result.profile_crc_valid
    assert [row.filename for row in result.rows] == [
        "AutoSave", "Manual - Save1", "Manual - Save2"
    ]
    assert result.rows[0].action == 0x11
    assert result.rows[1].metadata["CurrentRegionName"] == "TXT_REGION_GREATWOOD"
    assert result.rows[2].exists is False
    assert result.rows[2].action == 0xDC


def test_corrupt_save_stays_visible_but_not_loadable(tmp_path):
    make_profile(tmp_path / "Profile.bin", {1: "Manual - Save1"})
    (tmp_path / "Manual - Save1").write_bytes(b"not a save")
    row = save_metadata.enumerate_profile(tmp_path).rows[0]
    assert row.exists and not row.valid and row.action == 0xDC
    assert "ValueError" in row.error


if __name__ == "__main__":
    test_decode_header()
