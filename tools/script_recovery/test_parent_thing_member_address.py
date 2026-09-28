"""A quest thing member passed by address from an entity (V_TourGuide TourGuideGuide Main, 0x00EE57B0)."""
import json
from pathlib import Path

from tools.script_recovery.native_evidence_lowering import LoweringSpec, lower

ROOT = Path(__file__).resolve().parents[2]


def spec():
    unit = json.loads((ROOT / 'refs/script_recovery/tour_guide/units/V_TourGuide.json').read_text())
    result = LoweringSpec(unit, 'TourGuideGuide', entity=True, thing_slots={})
    result.call_labels = {}
    return result


def lowered(body):
    return lower('{\n' + body + '\n}', spec())[0]


def test_by_value_cast_of_the_member_is_the_thing():
    out = lowered('  bVar4 = IsDistanceBetweenThingsUnder(pThing,(CScriptThing_bv *)(*(int *)(this + 0x14) + 0x168),2.0);')
    assert 'QUESTTHING_Get("NextTourWaypoint")' in out and '0x168' not in out


def test_parent_loaded_as_the_script_class_is_respelled_onto_a_known_member():
    out = lowered('  Helper(*(CV_TourGuideScript **)(this + 0x14),(CScriptThing *)(*(CV_TourGuideScript **)(this + 0x14) + 0x168),x);')
    assert 'QUESTTHING_Get("NextTourWaypoint")' in out
    # an offset that is no parent member never becomes a thing
    out = lowered('  Helper((CScriptThing *)(*(CV_TourGuideScript **)(this + 0x14) + 0x3),x);')
    assert 'QUESTTHING_Get' not in out


def test_a_dereferenced_member_word_is_not_the_thing():
    out = lowered('  iVar1 = *(int *)(*(int *)(this + 0x14) + 0x168);')
    assert 'QUESTTHING_Get("NextTourWaypoint")' not in out.replace('__thing_valid', '')


def test_array_base_folded_into_the_index_reads_the_element_member():
    # Main 0x00EE57B0: GetThingWithScriptName(WaypointInfo[WaypointCounter].locMarker), WaypointInfo at +0x48, stride 0xc
    out = lowered('  x = F((CCharString *)(*(int *)(this + 0x14) + (*(int *)(*(int *)(this + 0x14) + 0x164) + 6) * 0xc));')
    assert 'QUESTSTATE_GetString(__key("WaypointInfo_" .. *(int *)(*(int *)(this + 0x14) + 0x164) .. "_locMarker"))' in out
    out = lowered('  CCharString::CCharString(&xStack_68,(CCharString *)(*(int *)(this + 0x14) + 0x4c + *(int *)(*(int *)(this + 0x14) + 0x164) * 0xc));')
    assert '_stk_68 = QUESTSTATE_GetString(__key("WaypointInfo_" .. *(int *)(*(int *)(this + 0x14) + 0x164) .. "_locTextOverheard"));' in out
