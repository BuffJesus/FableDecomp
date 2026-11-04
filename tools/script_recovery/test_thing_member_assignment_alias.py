"""A native pointer to a thing member remains a member after assignment."""
import json
from pathlib import Path

from tools.script_recovery.native_evidence_lowering import LoweringSpec, lower

ROOT = Path(__file__).resolve().parents[2]


def lowered(body):
    unit = json.loads((ROOT / 'refs/script_recovery/arena/units/Q_Arena.json').read_text())
    spec = LoweringSpec(unit, 'Q_Arena', entity=False, thing_slots={})
    spec.call_labels = {}
    return lower('{\n' + body + '\n}', spec)[0]


def test_member_alias_assignment_stores_then_refreshes_the_alias():
    out = lowered('  this_00 = (CScriptThing *)(this + 0x108);\n'
                  '  CScriptThing::operator=(this_00,(int)pCVar7);\n'
                  '  Use(this_00);\n')
    assert 'QUESTTHING_Set("Whisper", pCVar7);\n  this_00 = QUESTTHING_Get("Whisper");' in out
    assert 'operator=' not in out


def test_reassigned_alias_no_longer_stores_to_the_old_member():
    out = lowered('  this_00 = (CScriptThing *)(this + 0x108);\n'
                  '  this_00 = other;\n'
                  '  CScriptThing::operator=(this_00,(int)pCVar7);\n')
    assert 'QUESTTHING_Set' not in out


def test_unknown_offset_remains_unresolved():
    out = lowered('  this_00 = (CScriptThing *)(this + 0x109);\n'
                  '  CScriptThing::operator=(this_00,(int)pCVar7);\n')
    assert 'QUESTTHING_Set' not in out
