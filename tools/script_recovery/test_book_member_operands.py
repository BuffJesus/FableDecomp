"""Typed master string copies and conservative actor resource-pointer lowering."""
import json
from pathlib import Path
import pytest

from tools.script_recovery.native_evidence_lowering import (
    LoweringSpec, lower, fold_member_resource_actor_addresses)


def spec():
    root=Path(__file__).resolve().parents[2]
    unit=json.loads((root/'refs/script_recovery/book_collecting/units/V_BookCollecting.json').read_text())
    s=LoweringSpec(unit,'BS_Teacher',entity=True,thing_slots={})
    s.call_labels={}
    return s


@pytest.mark.parametrize('offset,expected',[('0x54',True),('0x4',False),('0xffff',False)])
def test_master_copy_requires_known_string_field(offset,expected):
    source='{\nCCharString::CCharString((CCharString *)&native_arg_param_1,(CCharString *)(*(int *)(this + 0x18) + '+offset+'));\n}'
    out=lower(source,spec())[0]
    assert ('native_arg_param_1 = GSI->GetMasterGameState("TeddySolution");' in out)==expected
    if not expected: assert 'CCharString::CCharString' in out


def test_parent_field_cannot_be_mistaken_for_master_string():
    source='{\nCCharString::CCharString((CCharString *)&dst,(CCharString *)(*(int *)(this + 0x14) + 0x54));\n}'
    assert 'TeddySolution' not in lower(source,spec())[0]


@pytest.mark.parametrize('middle,folded',[
    ('other = 1;\n',True),
    ('CCharString::CCharString(&key,"Teacher",-1);\n',True),
    ('if (flag) {\n',False),
    ('}\n',False),
    ('LAB_1234:\n',False),
    ('goto LAB_1234;\n',False),
    ('pOther = unknown;\n',False),
    ('overwrite(&pOther);\n',False),
    ('pOther++;\n',False),
    ('call(pOther = unknown);\n',False),
])
def test_actor_alias_requires_uninterrupted_member_value(middle,folded):
    source='pOther = RESOURCE_MemberResource("seh_me", (CScriptThing *)(this + 8));\n'+middle+'ACTORMAP_Set(actors, "Teacher", &pOther);\n'
    out=fold_member_resource_actor_addresses(source)
    assert ('ACTORMAP_Set(actors, "Teacher", pOther);' in out)==folded
    if not folded: assert out==source


def test_actor_alias_without_prior_definition_remains_unresolved():
    source='ACTORMAP_Set(actors, "Teacher", &pOther);\npOther = RESOURCE_MemberResource("seh_me");\n'
    assert fold_member_resource_actor_addresses(source)==source
