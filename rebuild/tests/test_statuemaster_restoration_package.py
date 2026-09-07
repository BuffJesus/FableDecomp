from pathlib import Path
import json
import re

from lupa import LuaRuntime


ROOT = Path(__file__).resolve().parents[2]
PACKAGE = ROOT / "mods" / "StatueMasterRestoration"


def test_restoration_package_has_namespaced_fail_closed_vertical_slice():
    master = (PACKAGE / "StatueMasterRestoration.lua").read_text(encoding="utf-8")
    hub = (PACKAGE / "Entities" / "LookoutPointStatue.lua").read_text(encoding="utf-8")
    seal = (PACKAGE / "Entities" / "SealStatue.lua").read_text(encoding="utf-8")
    probe = (PACKAGE / "AugmentationProbe.lua").read_text(encoding="utf-8")

    assert 'PREFIX = "FSE_SM_RECON_"' in master
    assert 'PREFIX = "FSE_SM_RECON_"' in hub
    assert "Probe.InspectHit(me)" in seal
    assert "EntitySetTargetable(statue, true)" in seal
    assert "EntitySetMaxHealth(statue, MAX_HEALTH, true)" in seal
    assert "WorldEvent.TryAward" in seal
    assert "if bridgeError then" in seal
    assert "MsgGetHitByWeaponAugmentations" in probe


def test_recovered_steel_and_diamond_aliases_are_explicit():
    probe = (PACKAGE / "AugmentationProbe.lua").read_text(encoding="utf-8")
    assert 'STEEL = "SHARPENING"' in probe
    assert 'DIAMOND = "PIERCING"' in probe
    assert "SHARPENING = 0x001" in probe
    assert "SILVER = 0x002" in probe
    assert "FLAME = 0x004" in probe
    assert "LIGHTNING = 0x008" in probe
    assert "PIERCING = 0x010" in probe


def test_runtime_probe_is_structured_and_debounced():
    diagnostic = (PACKAGE / "Entities" / "ForgeHitTarget.lua").read_text(encoding="utf-8")
    seal = (PACKAGE / "Entities" / "SealStatue.lua").read_text(encoding="utf-8")
    state = (PACKAGE / "WorldEventState.lua").read_text(encoding="utf-8")
    matrix = (PACKAGE / "RUNTIME_PROBE.md").read_text(encoding="utf-8")

    assert 'PROBE_PREFIX = "SM_HIT_PROBE"' in diagnostic
    assert '"repeat_same_window"' in diagnostic
    assert '"post_yield"' in diagnostic
    assert "WorldEvent.TryAward" in seal
    assert "reportedBridgeError" in seal
    assert 'PREFIX .. "LOOKOUT_SEAL"' in state
    assert 'sealKey("PIERCING")' in state
    assert "if awarded then" in seal
    assert "augmented bow" in matrix
    assert "non-hero attacker" in matrix
    assert "immediately swap weapons" in matrix
    mock = (PACKAGE / "tests" / "augmentation_probe_mock_test.lua").read_text(encoding="utf-8")
    assert "single Flame" in mock
    assert "multi Piercing" in mock
    assert "unknown value fails closed" in mock


def test_world_event_state_machine_executes_all_five_seals():
    lua = LuaRuntime(unpack_returned_tuples=True)
    test_dir = PACKAGE / "tests"
    old_cwd = Path.cwd()
    try:
        import os

        os.chdir(test_dir)
        script = (test_dir / "world_event_state_test.lua").read_text(encoding="utf-8")
        lua.execute(script)
        lua.execute((test_dir / "reward_plan_test.lua").read_text(encoding="utf-8"))
    finally:
        os.chdir(old_cwd)


def test_runtime_lua_files_load_with_the_installed_package_path():
    lua = LuaRuntime(unpack_returned_tuples=True)
    package_root = PACKAGE.parent.as_posix()
    lua.execute(f'package.path = "{package_root}/?.lua;" .. package.path')
    lua.execute((PACKAGE / "StatueMasterRestoration.lua").read_text(encoding="utf-8"))
    lua.execute((PACKAGE / "Entities" / "LookoutPointStatue.lua").read_text(encoding="utf-8"))


def test_placement_manifest_matches_registry_and_wrappers():
    placements = json.loads((PACKAGE / "placements.json").read_text(encoding="utf-8"))
    registry = (PACKAGE / "quests.lua").read_text(encoding="utf-8")
    expected = {"FLAME", "SHARPENING", "SILVER", "PIERCING", "LIGHTNING"}
    actual = {entry["category"] for entry in placements["placements"]}
    assert actual == expected
    assert set(placements["bearings"]) == {"Greatwood", "Bowerstone", "Guild"}
    for entry in placements["placements"]:
        script_name = entry["target"]["scriptName"]
        assert re.search(rf'name\s*=\s*"{re.escape(script_name)}"', registry)
        wrapper = PACKAGE / "Entities" / (entry["category"].title() + "Seal.lua")
        if entry["category"] == "SHARPENING":
            wrapper = PACKAGE / "Entities" / "SharpeningSeal.lua"
        text = wrapper.read_text(encoding="utf-8")
        assert f'category = "{entry["category"]}"' in text
        assert f'scriptName = "{script_name}"' in text


def test_authored_lightning_candidate_has_guarded_staging_contract():
    placements = json.loads((PACKAGE / "placements.json").read_text(encoding="utf-8"))
    authored = [p for p in placements["placements"] if p["status"] == "candidate-authored"]
    assert len(authored) == 1
    lightning = authored[0]
    assert lightning["category"] == "LIGHTNING"
    assert lightning["map"] == "Darkwood_11"
    assert lightning["target"]["uid"] == "18446741874686400000"
    assert lightning["target"]["position"] == [76.0, 42.0, 32.5]

    stage = (PACKAGE / "Stage-AuthoredCandidates.ps1").read_text(encoding="utf-8")
    verify = (PACKAGE / "Test-StagedAuthoredCandidates.ps1").read_text(encoding="utf-8")
    patch = (PACKAGE / "patches" / "Darkwood_11.lightning-seal.patch").read_text(
        encoding="utf-8"
    )
    assert "Test-PlacementManifest.ps1" in stage
    assert "tng place" in stage
    assert "candidate-authored" in stage
    assert "changes bytes outside the authored block" in verify
    assert "UID 18446741874686400000;" in patch
    assert "PositionX 76.0;" in patch
    assert "PositionY 42.0;" in patch
    assert "PositionZ 32.5;" in patch


def test_statue_master_hammer_source_asset_is_internally_bound():
    asset = PACKAGE / "assets" / "source" / "statue_master_hammer"
    model = asset / "source" / "model" / "Martillo_low.obj"
    material = asset / "source" / "model" / "Martillo_low.mtl"
    intake = (asset / "ASSET_INTAKE.md").read_text(encoding="utf-8")
    obj = model.read_text(encoding="utf-8-sig")
    mtl = material.read_text(encoding="utf-8")

    assert "mtllib Martillo_low.mtl" in obj
    used = set(re.findall(r"^usemtl\s+(.+)$", obj, flags=re.MULTILINE))
    declared = set(re.findall(r"^newmtl\s+(.+)$", mtl, flags=re.MULTILINE))
    assert used == declared == {"Cabeza_M1", "Mango_M1", "Pinchos_M1"}
    for relative in re.findall(r"^map_Kd\s+(.+)$", mtl, flags=re.MULTILINE):
        assert (material.parent / relative).resolve().is_file()
    assert "Creator: `angeldavidvillanueva`" in intake
    assert "Creative Commons Attribution" in intake
    assert "CC BY 4.0" in intake
    assert "d5b61f4716a04de886d3422f1ac69ace" in intake
    assert (asset / "LICENSE_SCREENSHOT.png").is_file()
    credits = (asset / "CREDITS.md").read_text(encoding="utf-8")
    assert "angeldavidvillanueva" in credits
    assert "https://creativecommons.org/licenses/by/4.0/" in credits
    assert "d5b61f4716a04de886d3422f1ac69ace" in credits
    assert "57ADD7E0D478F0DFCA97DA46B7FCC5E7BA25A33ED888A2C0F04FCE77A586E885" in intake

    prepared = PACKAGE / "assets" / "prepared" / "statue_master_hammer"
    report = json.loads((prepared / "conversion_report.json").read_text(encoding="utf-8"))
    assert report["status"] == "source-scale-not-game-ready"
    assert report["vertices"] == 4844
    assert report["triangles"] == 9616
    assert set(report["materials"]) == {"Cabeza_M1", "Mango_M1", "Pinchos_M1"}
    assert report["scaleNormalized"] is False
    assert report["gripAlignedToRetailDonor"] is False
    assert (prepared / "StatueMasterHammer.source-scale.glb").stat().st_size > 1_000_000

    alignment = json.loads((prepared / "alignment_report.json").read_text(encoding="utf-8"))
    assert alignment["donor"] == "MESH_HAMMER_GREAT_LEGENDARY_01"
    assert alignment["transform"]["rotateXDegrees"] == 180.0
    assert alignment["transform"]["uniformScale"] == 10.0
    assert alignment["gripValidatedInGame"] is False
    assert (prepared / "StatueMasterHammer.retail-axis-candidate.glb").stat().st_size > 1_000_000

    reward = json.loads((PACKAGE / "reward_definition_plan.json").read_text(encoding="utf-8"))
    assert reward["reward"]["objectDefinition"] == "OBJECT_STATUEMASTER_HAMMER"
    assert reward["donor"]["graphicModelId"] == 7943
    assert reward["customMesh"]["graphicModelId"] == 8114
    assert reward["customMesh"]["textureIds"] == {
        "Cabeza_M1": 6294, "Mango_M1": 6295, "Pinchos_M1": 6296
    }
    assert reward["customMesh"]["stagedBankValidation"]["meshAuditFailures"] == 0
    assert reward["customMesh"]["stagedBankValidation"]["preExistingEntriesChanged"] == 0
    assert reward["augmentationComponent"]["combinedTypeMask"] == 31
    assert [entry["gameBinIndex"] for entry in reward["augmentationComponent"]["initialAugmentations"]] == [2880, 2877, 2879, 2878, 2882]
    assert reward["safety"]["runtimeRewardEnabled"] is False
    assert reward["stagedDefinition"]["objectIndex"] == 14761
    assert reward["stagedDefinition"]["augmentationSubDefinitionIndex"] == 14762
    assert reward["stagedDefinition"]["inventorySubDefinitionIndex"] == 14763
    assert reward["stagedDefinition"]["preExistingEntriesChanged"] == 0
    assert reward["integratedStage"]["verified"] is True
    assert (PACKAGE / "Build-HammerRewardStage.ps1").is_file()
    assert (PACKAGE / "tools" / "verify_hammer_stage.py").is_file()
