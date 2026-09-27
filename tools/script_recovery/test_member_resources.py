"""Script-member resources (`seh_*`) and member string maps (`csargs`) outlive callbacks: they lower to the
sidecar's persistent resources:MemberResource / MemberStringMap entries, never to guessed locals."""
import ast
import json

from tools.script_recovery.quest_unit_evidence import ROOT, class_fields, load_layouts
from tools.script_recovery.native_evidence_lowering import (
    LoweringSpec, fold_actor_maps, lower_member_resources, normalise_typed_decompile,
)
from tools.script_recovery.convert_quest_unit import unwrap_statements

RESOURCE_VTABLE = '&PTR__scalar_deleting_destructor__0127094c'


def unit(quest_resources=None, entity_resources=None, string_maps=None):
    maps = [{'offset': hex(o), 'type': 'map<CCharString,CCharString,std::less<CCharString>>', 'name': n}
            for o, n in (string_maps or {}).items()]
    return {'quest': {'fields': {}, 'unmappedFields': maps,
                      'resourceFields': {hex(k): v for k, v in (quest_resources or {}).items()}},
            'entities': {'Guard': {'fields': {}, 'unmappedFields': [],
                                   'resourceFields': {hex(k): v for k, v in (entity_resources or {}).items()}}}}


def spec(u, entity, labels=None):
    s = LoweringSpec(u, 'Guard' if entity else 'Q', entity=entity, thing_slots={})
    s.call_labels = labels or {}
    return s


def test_pdb_resource_members_reach_the_unit_evidence_at_retail_offsets():
    fields, _, _ = class_fields(load_layouts(), 'CV_BordelloScript', delta=-0x14)
    assert fields['__resources__'] == {'0x74': 'seh_Boss', '0x84': 'seh_Guard', '0x94': 'seh_Madam', '0xa4': 'seh_Whore'}
    assert not any(isinstance(v, list) and v[0].startswith('seh_') for k, v in fields.items() if not k.startswith('__'))


def test_quest_member_operands_are_named_but_dereferences_are_not():
    s = spec(unit({0x74: 'seh_Boss'}, string_maps={0xb4: 'csargs'}), entity=False)
    text = ('  CFourierAnalysis::CFourierAnalysis(pCVar7,(int)(this + 0x74));\n'
            '  CFourierAnalysis::CFourierAnalysis(pCVar7,(int)this + 0x74);\n'
            '  uVar3 = *(undefined4 *)(this + 0x74);\n'
            '  CTCCarryable::OnKill((CTCCarryable *)(this + 0xb4));\n'
            '  x = this_00 + 0x74;\n')
    out = lower_member_resources(text, s)
    assert out.count('CFourierAnalysis(pCVar7,RESOURCE_MemberResource("seh_Boss"));') == 2
    assert '*(undefined4 *)(this + 0x74)' in out
    assert 'OnKill(STRINGMAP_Member("csargs"));' in out
    assert 'this_00 + 0x74' in out


def test_entity_assigns_its_quests_member_only_from_a_constructed_resource():
    labels = {'CScriptGameResourceObjectScriptedThingBase::operator=': 0x8ABD10}
    s = spec(unit({0x84: 'seh_Guard'}), entity=True, labels=labels)
    built = ('  CBaseIntelligentPointer::CBaseIntelligentPointer((CBaseIntelligentPointer *)appuStack_d0);\n'
             f'  appuStack_d0[0] = {RESOURCE_VTABLE};\n')
    assign = ('  CScriptGameResourceObjectScriptedThingBase::operator=\n'
              '            ((CScriptGameResourceObjectScriptedThingBase *)(*(int *)((int)this + 0x14) + 0x84),\n'
              '             {src});\n')
    good = lower_member_resources(unwrap_statements(built + assign.format(src='appuStack_d0')), s)
    assert 'RESOURCE_AssignResource(RESOURCE_MemberResource("seh_Guard"), appuStack_d0);' in good
    # Magicman 0x00E40E80: the operand names a slot the function never constructs as a resource -- no guess
    drifted = lower_member_resources(unwrap_statements(built + assign.format(src='aCStack_370')), s)
    assert 'RESOURCE_AssignResource' not in drifted


def test_cached_parent_pointer_names_members_only_while_it_holds_the_parent():
    labels = {'CFourierAnalysis::CFourierAnalysis': 0x99A3B0}
    s = spec(unit({0x80: 'seh_ChickenMaster'}), entity=True, labels=labels)
    text = ('  CBaseIntelligentPointer::CBaseIntelligentPointer((CBaseIntelligentPointer *)appuStack_394);\n'
            f'  appuStack_394[0] = {RESOURCE_VTABLE};\n'
            '  iVar10 = *(int *)((int)this + 0x14);\n'
            '  CFourierAnalysis::CFourierAnalysis((CFourierAnalysis *)(iVar10 + 0x80),(int)appuStack_394);\n'
            '  piVar5 = piStack_388;\n'
            '  uVar3 = uStack_38c;\n'
            '  piVar2 = *(int **)(iVar10 + 0x8c);\n'
            '  if (piVar2 != piStack_388) {\n'
            '    if (piVar2 != (int *)0x0) {\n'
            '      *piVar2 = *piVar2 + -1;\n'
            '    }\n'
            '    *(undefined4 *)(iVar10 + 0x88) = uVar3;\n'
            '    *(int **)(iVar10 + 0x8c) = piVar5;\n'
            '  }\n'
            '  iVar10 = **(int **)((int)this + 4);\n'
            '  g((void *)(iVar10 + 0x80));\n')
    out = lower_member_resources(text, s)
    assert 'RESOURCE_AssignResource(RESOURCE_MemberResource("seh_ChickenMaster"), appuStack_394);' in out
    assert 'piVar2 = *(int **)' not in out            # the inlined refcount dance is the assignment
    assert 'g((void *)(iVar10 + 0x80));' in out       # the reused local no longer names the parent


def test_bordello_play_cutscene_builds_its_actor_map_from_the_members():
    tu = json.loads((ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json').read_text(encoding='utf-8'))
    fn = next(f for f in tu['functions'] if f['address'] == '0x00E3E720')
    calls = fn['calls'] if isinstance(fn['calls'], list) else ast.literal_eval(fn['calls'])
    labels = {c['currentName']: int(c['target'], 16) for c in calls if c.get('currentName')}
    s = spec(unit({0x74: 'seh_Boss', 0x84: 'seh_Guard', 0x94: 'seh_Madam', 0xa4: 'seh_Whore'},
                  string_maps={0xb4: 'csargs'}), entity=False, labels=labels)
    text = fold_actor_maps(lower_member_resources(normalise_typed_decompile(unwrap_statements(fn['decompile'])), s))
    # the BOSS key is a rdata string (&DAT_012e4224) until the converter resolves it
    for key, member in (('&DAT_012e4224', 'seh_Boss'), ('"GUARD"', 'seh_Guard'), ('"MADAM"', 'seh_Madam'),
                        ('"WHORE"', 'seh_Whore')):
        assert f'ACTORMAP_Set(puStack_1c, {key}, RESOURCE_MemberResource("{member}"));' in text
    assert text.count('RESOURCE_MemberResource(') == 4
    assert 'STRINGMAP_Clear(STRINGMAP_Member("csargs"));' in text
    assert 'RESOURCE_RunMacroWithStrings(' in text and 'STRINGMAP_Member("csargs")' in text
    assert '*piVar2 = *piVar2 + -1;' not in text      # every refcount dance folded into a store
