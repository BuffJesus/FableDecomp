"""Unnamed inventory threads take their name from the spawn-site CSpawnedFunc literal."""
import json
from pathlib import Path

from tools.script_recovery.quest_unit_evidence import spawn_site_thread_names

ROOT = Path(__file__).resolve().parents[2]
REG, BODY = '0x00001000', '0x00002000'
SPAWN = '''  pCVar9 = operator_new(0x3c);
  if (pCVar9 == (CSpawnedFunc<X> *)0x0) {
    pCVar9 = (CSpawnedFunc<X> *)0x0;
  }
  else {
    uVar1 = *(undefined4 *)((int)this + 0x14);
    CCharString::CCharString
              ((CCharString *)&CStack_1dc,"%s",-1);
    pCVar10 = extraout_EAX;
    CCharString::CCharString((CCharString *)&CStack_1e4,"ParentClass.",-1);
    pCVar10 = CCharString__AppendData(&CStack_170,a,pCVar10);
    CSpawnedFunc<X>::
    CSpawnedFunc<X>(pCVar9,pCVar10,0);
    *(code **)(pCVar9 + 0x34) = %s;
    *(undefined4 *)(pCVar9 + 0x38) = uVar1;
  }
'''


def rows(name=None):
    return [{'registrationFunction': REG, 'store': '0x00001010', 'body': BODY, 'name': name}]


def tu(*spawns):
    return {REG.lower(): {'decompile': ''.join(SPAWN % s for s in spawns)},
            BODY: {'currentName': 'NScript::CQ::Worker', 'decompile': ''}}


def test_wrapped_literal_names_the_body_not_the_parent_prefix():
    row = spawn_site_thread_names(rows(), tu(('Worker', 'FUN_00002000')))[0]
    assert row['name'] == 'Worker'
    assert row['nameEvidence'] == 'spawn-site CSpawnedFunc literal'


def test_body_may_be_named_by_its_symbol():
    assert spawn_site_thread_names(rows(), tu(('Worker', 'NScript::CQ::Worker')))[0]['name'] == 'Worker'


def test_conflicting_spawn_names_and_named_rows_are_left_alone():
    conflict = tu(('Worker', 'FUN_00002000'), ('Other', 'FUN_00002000'))
    assert spawn_site_thread_names(rows(), conflict) == rows()
    assert spawn_site_thread_names(rows('Kept'), tu(('Worker', 'FUN_00002000'))) == rows('Kept')


def test_retail_bordello_and_tour_guide_workers():
    expect = {'bordello': ('0x00E44980', 'WatchForHeroLeavingRegionWithBeer'),
              'tour_guide': ('0x00EE6A40', 'WatchForNoFollowers')}
    for package, (body, name) in expect.items():
        evidence = ROOT / 'refs/script_recovery' / package
        inventory = json.loads((evidence / 'inventory.json').read_text(encoding='utf-8-sig'))
        functions = json.loads((evidence / 'translation_unit_typed.json').read_text(encoding='utf-8-sig'))['functions']
        named = spawn_site_thread_names(inventory['threads'], {f['address'].lower(): f for f in functions})
        assert {r['name'] for r in named if r['body'] == body} == {name}
