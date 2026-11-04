from pathlib import Path
import pytest
from tools.script_recovery.annotate_interface_slots import annotate,_is_gsi_alias_at

CALL='(**(code **)(*piVar8 + 0x18))();'
SLOTS={0x18:'IsXbox',0x120:'GetThingWithScriptName'}

@pytest.mark.parametrize('prefix',['','piVar8 = *(int **)(this + 4);\n'])
@pytest.mark.parametrize('assign',[
    'if ((begin != end) && (piVar8 = begin + 3, piVar8 != end)) {\n',
    'if ((piVar8 = begin + 3) != end) {\n',
    'while ((piVar8 = nextEntry()) != end) {\n',
    'Use(value, piVar8 = begin + 3);\n',
    'if (ready && (\n piVar8\n = begin + 3, ready)) {\n',
])
def test_inline_write_cannot_be_assumed_to_be_game_interface(prefix,assign):
    source=prefix+assign+CALL
    assert annotate(source,SLOTS)==source

@pytest.mark.parametrize('noise',[
    'Print("(piVar8 = entries)");\n',
    'Print("escaped\\\"(piVar8 = entries)");\n',
    '// (piVar8 = entries)\n',
    '/* (piVar8 = entries) */\n',
    'if (piVar8 == expected) {\n',
    'if ((other = piVar8)) {\n',
])
def test_quotes_comments_and_comparisons_are_not_receiver_writes(noise):
    source='piVar8 = *(int **)(this + 4);\n'+noise+CALL
    assert annotate(source,SLOTS).endswith('GSI->IsXbox();')

def test_later_proven_interface_load_restores_provenance():
    source='if ((piVar8 = entries, ready)) {}\npiVar8 = *(int **)(this + 4);\n'+CALL
    assert annotate(source,SLOTS).endswith('GSI->IsXbox();')

def test_copying_an_inline_defined_iterator_does_not_make_it_an_interface():
    source='if ((pIter = entries, ready)) {}\npiVar8 = pIter;\n'+CALL
    assert annotate(source,SLOTS)==source

def test_registered_chicken_iterator_call_stays_explicit():
    import json
    root=next(p for p in Path(__file__).resolve().parents if (p/'refs/script_recovery/chicken_kicking').is_dir())
    data=json.loads((root/'refs/script_recovery/chicken_kicking/translation_unit_typed.json').read_text(encoding='utf-8'))
    fn=next(f for f in data['functions'] if f['address'].lower()=='0x00e64210')
    source=fn['decompile'].replace('\r','')
    result=annotate(source,SLOTS)
    assert 'pfVar7 = (float *)GSI->IsXbox' not in result
    assert '(**(code **)(*piVar8 + 0x18))()' in result

def test_native_call_receiver_is_current_array_entry():
    from tools.script_recovery.lift_native_lua import RData
    rd=RData()
    assert rd.bytes_at(0xe649d5,3)==bytes.fromhex('8d680c')  # lea ebp,[eax+12]
    assert rd.bytes_at(0xe64a31,8)==bytes.fromhex('8b45008bcdff5018') # vtable[18](ecx=ebp)
    assert rd.bytes_at(0xe64b95,3)==bytes.fromhex('83c50c') # next12-byte Thing
    assert rd.bytes_at(0x1238c8c+0x18,4)!=rd.bytes_at(0x1260f0c+0x18,4)

def test_registered_converter_retains_unresolved_native_operation(tmp_path):
    import json
    from unittest.mock import patch
    from tools.script_recovery.convert_quest_unit import UnitConverter
    root=next(p for p in Path(__file__).resolve().parents if (p/'refs/script_recovery/chicken_kicking').is_dir())
    evidence=root/'refs/script_recovery/chicken_kicking'
    unit=json.loads((evidence/'units/V_ChickenKicking.json').read_text(encoding='utf-8'))
    entity=unit['entities']['KickedChicken'];entity['functions']={'Main':entity['functions']['Main']}
    unit['entities']={'KickedChicken':entity};unit['quest']['functions']={};unit['quest']['helpers']={}
    converter=UnitConverter(evidence/'translation_unit_typed.json')
    with patch.object(converter,'constructor_defaults',return_value={}):converter.convert(unit,tmp_path)
    text=(tmp_path/'FSE/V_ChickenKicking/Entities/KickedChicken.lua').read_text(encoding='utf-8')
    assert 'quest:IsXbox' not in text
    assert '-- TODO(native): pfVar7 = (**(*piVar8 + 0x18))()' in text
    assert 'pfVar7 = nil --[[unresolved native value]]' in text
