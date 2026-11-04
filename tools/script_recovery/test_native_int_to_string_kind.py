from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import Lifter,load_manifest,RData
from tools.script_recovery.native_evidence_lowering import finish_lua,fold_engine_helpers

@pytest.mark.parametrize('operand,expected',[('0','0'),('1','1'),('-1','-1'),('42','42'),('0x7fffffff','2147483647'),('(-9 + 4)','-5'),('(6 * 7)','42')])
@pytest.mark.parametrize('stale',['unrelated','earlier message'])
def test_integer_string_keeps_message_duration_and_boolean(operand,expected,stale):
    lifter=Lifter(load_manifest(),{},'quest',False,'',RData(),execution_entity=False)
    lifter.accessor_kinds=True
    source=fold_engine_helpers('{\n oldMessage = GSI->GetActiveQuestName();\n message = Convert(&slot,'+operand+');\n GSI->AddScreenTitleMessage(message,0.0,true);\n}',{'Convert':0x99f830})
    body=finish_lua('\n'.join(lifter.lift('Main',source)))
    lua=LuaRuntime(unpack_returned_tuples=True)
    calls=[];quest=lua.table();quest.AddScreenTitleMessage=lambda self,*args:calls.append(args)
    quest.GetActiveQuestName=lambda self:stale
    lua.execute('return function(quest)\n'+body+'\nend')(quest)
    assert calls==[(expected,0,True)]

def test_unresolved_integer_keeps_string_slot_without_borrowing_stale_message():
    lifter=Lifter(load_manifest(),{},'quest',False,'',RData(),execution_entity=False)
    lifter.accessor_kinds=True
    lifter.push_temp('oldMessage','"unrelated"')
    lifter.statement('Main','message = ENGINE_IntToString(*(int *)(cursor + 0x10));')
    lifter.interface_call(None,'AddScreenTitleMessage','message,0.0,true')
    assert lifter.kinds['message']=='string'
    assert 'quest:AddScreenTitleMessage(message, 0.0, true)' in '\n'.join(lifter.out)
    # Sanitizing unresolved memory must retain its diagnostic and nil placeholder.
    body=finish_lua('\n'.join(lifter.out))
    assert 'tostring(*' in body

@pytest.mark.parametrize('name',['ENGINE_IntToStringExtra','ENGINE_UnknownNumber'])
def test_unknown_helpers_do_not_acquire_string_provenance(name):
    lifter=Lifter(load_manifest(),{},'quest',False,'',RData())
    lifter.statement('Main','result = '+name+'(42);')
    assert lifter.kinds.get('result')!='string'

def test_native_integer_conversion_identity_is_required():
    source='message = Convert(&slot,42);'
    assert fold_engine_helpers(source,{'Convert':0x99f831})==source

def test_registered_debug_title_keeps_native_operand_order(tmp_path):
    import json
    from unittest.mock import patch
    from tools.script_recovery.convert_quest_unit import UnitConverter
    root=next(p for p in Path(__file__).resolve().parents if (p/'refs/script_recovery/expressions').is_dir())
    evidence=root/'refs/script_recovery/expressions'
    data=json.loads((evidence/'units/Global_DebugCycleThroughSpeech.json').read_text(encoding='utf-8'))
    data['quest']['functions']={'Main':data['quest']['functions']['Main']}
    converter=UnitConverter(evidence/'translation_unit_typed.json')
    with patch.object(converter,'constructor_defaults',return_value={}):converter.convert(data,tmp_path)
    source=(tmp_path/'FSE/Global_DebugCycleThroughSpeech/Global_DebugCycleThroughSpeech.lua').read_text(encoding='utf-8')
    assert 'quest:AddScreenTitleMessage(pCVar4, 0.0, bVar2)' in source
    assert 'pCVar4 = nil --[[unresolved native value]]' in source
    assert '-- TODO(native): pCVar4 = tostring(*(uVar6 + 0x10))' in source
    assert 'Speak: unresolved entity receiver/resource' in source
