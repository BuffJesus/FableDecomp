import copy,itertools,json,re
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.native_reviewed_stack_aliases import restore

@pytest.fixture(scope='module')
def evidence(tmp_path_factory):
    c=UnitConverter(ROOT/'refs/script_recovery/beggar_and_child/translation_unit_typed.json')
    unit=json.loads((ROOT/'refs/script_recovery/beggar_and_child/units/V_BeggarAndChild.json').read_text())
    fn=c.native('0x00e58d40');row=unit['reviewedStackScalarAliases']['0x00e58d40']
    out=tmp_path_factory.mktemp('lookout_first_reply');c.convert(unit,out)
    source=(out/'FSE/V_BeggarAndChild/Entities/LookoutPointBeggar.lua').read_text()
    return c,fn,row,source


def make_probe(source):
    init=re.search(r'^ *initialReplySelected = false',source,re.M)[0]
    match=re.search(r'^ *if .*initialReplySelected.* then',source,re.M)
    guard=source.rfind('quest:IsActiveThreadTerminating()',0,match.start());begin=source.rfind('\n',0,guard)+1
    stop=source.index('quest:GetStateInt("BelchedAtBeggar")',match.end());stop=source.rfind('\n',0,stop)+1
    body=source[begin:stop]
    lua=LuaRuntime(unpack_returned_tuples=True)
    return lua.execute('local initialReplySelected,alive,bVar3,bVar5\n'+init+"""
local inputCancel,taught,released
local quest={IsActiveThreadTerminating=function() return inputCancel end,GetStateBool=function(_,key) assert(key=='TaughtBattleCry');return taught end}
return function(cancel,lesson)
inputCancel=cancel;taught=lesson;released=false
local function attempt()
"""+body+"""
do return true end
end
do return false end
::LAB_00e5b19b::
released=true
return false
end
local picked=attempt()
return initialReplySelected,not not picked,released end""")

@pytest.mark.parametrize('cancels',list(itertools.product((False,True),repeat=3)))
@pytest.mark.parametrize('lessons',list(itertools.product((False,True),repeat=3)))
def test_generated_initial_reply_flag_tracks_native_selection(evidence,cancels,lessons):
    probe=make_probe(evidence[3]);selected=False
    for cancel,lesson in zip(cancels,lessons):
        picked=not cancel and not lesson and not selected;selected=selected or picked
        assert probe(cancel,lesson)==(selected,picked,cancel)

@pytest.mark.parametrize('kind',['native','source','printed','collision'])
def test_changed_evidence_does_not_restore_flag(evidence,kind):
    c,fn,row,_=evidence;row=copy.deepcopy(row);text='\n'.join(old for old,_ in row['edits'])
    if kind=='native':row['nativeInstructions']['0x00E58E76']='00'
    if kind=='source':row['sourceSha256']='0'*64
    if kind=='printed':text=text.replace(row['edits'][0][0],'unknown;')
    if kind=='collision':text+='\ninitialReplySelected=7;'
    with pytest.raises(ValueError,match='reviewed stack alias'):restore(text,fn,row,c.rdata)
