"""A parent worker captures its runtime word before the caller changes it."""
import json
from pathlib import Path

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.lift_native_lua import Lifter, RData, RE_THREAD_WORD, load_manifest
from tools.script_recovery.native_evidence_lowering import finish_lua
from tools.script_recovery.guild_training_inventory import recover

ROOT = Path(__file__).resolve().parents[2]
# BookCollecting teacher 0x00E56458..0x00E56520 after generic operand lowering.
SPAWN = '''  this_00 = ::operator_new(0x40);
  if (this_00 == (CSpawnedFunc<NScript::CExpression_FollowScript> *)0x0) {
    this_00 = (CSpawnedFunc<NScript::CExpression_FollowScript> *)0x0;
  }
  else {
    xStack_50 = *(CCharString *)(this + 0x14);
    CCharString::CCharString(xStack_40,"BookReaction",-1);
    pCVar5 = extraout_EAX;
    CCharString::CCharString(xStack_44,"ParentClass.",-1);
    pCVar5 = ENGINE_Concat(a, pCVar5);
    CSpawnedFunc<NScript::CExpression_FollowScript>::CSpawnedFunc<NScript::CExpression_FollowScript> (this_00,pCVar5,0);
    *(undefined ***)this_00 = &PTR__vector_deleting_destructor__012e6228;
    *(code **)(this_00 + 0x34) = BookReaction;
    *(CCharString *)(this_00 + 0x38) = xStack_50;
    *(CCharString *)(this_00 + 0x3c) = value;
    uVar10 = 7;
  }
  CCharString::CCharString(&xStack_54,"",-1);
  CGuiVarTransferStruct::Add(*(CGuiVarTransferStruct **)(this + 0x14),this_00,sectionName);
  std::_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&> (&xStack_54);
  if ((uVar10 & 4) != 0) {
    uVar10 = uVar10 & 0xfffffffb;
    std::_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&> (&xStack_48);
  }
  if ((uVar10 & 2) != 0) {
    uVar10 = uVar10 & 0xfffffffd;
    std::_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&> (xStack_44);
  }
  if ((uVar10 & 1) != 0) {
    std::_Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&> (xStack_40);
  }
'''

@pytest.mark.parametrize('index', [0, 1, 17, -1])
def test_runtime_word_is_captured_once_before_reassignment(index):
    lifter = Lifter(load_manifest(), {}, 'quest', True, '', RData())
    source = '{\nvalue = native_arg_index;\n' + SPAWN + '\nvalue = 999;\nreturn;\n}'
    body = finish_lua('\n'.join(lifter.lift('Main', source, parameters={'native_arg_index': 'number'})))
    lua = LuaRuntime(unpack_returned_tuples=True)
    calls = []
    quest = lua.table_from({'CreateThread': lambda _, name, opts: calls.append((name, opts['args'][1]))})
    lua.execute('return function(quest, native_arg_index)\n' + body + '\nend')(quest, index)
    assert calls == [('BookReaction', index)]
    assert 'TODO' not in body

@pytest.mark.parametrize('old,new', [
    ('operator_new(0x40)', 'operator_new(0x44)'),
    ('this + 0x14', 'this + 0x18'),
    ('+ 0x38) = xStack_50', '+ 0x38) = otherParent'),
    ('+ 0x3c) = value', '+ 0x40) = value'),
    ('ParentClass.', 'OtherClass.'),
    ('0x14),this_00,sectionName', '0x14),otherObject,sectionName'),
    ('uVar10 = 7;', 'QuestSpecificEffect(); uVar10 = 7;'),
])
def test_unproven_layout_owner_or_extra_effect_is_left_unresolved(old, new):
    assert old in SPAWN
    assert RE_THREAD_WORD.search(SPAWN)
    assert RE_THREAD_WORD.search(SPAWN.replace(old, new)) is None

def test_retail_inventory_and_export_agree_on_worker_name():
    evidence = ROOT / 'refs/script_recovery/book_collecting'
    inventory = recover(evidence=evidence, unit_name='book_collecting')
    row = next(t for t in inventory['threads'] if t['body'] == '0x00E566F0')
    assert row['name'] == 'BookReaction'
    assert row['store'] == '0x00E564B8'
    unit = json.loads((evidence / 'units/V_BookCollecting.json').read_text())
    assert unit['quest']['functions'][row['name']]['address'].lower() == row['body'].lower()
