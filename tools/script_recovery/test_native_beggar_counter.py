import copy,re
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.test_native_beggar_friend_reply import evidence
from tools.script_recovery.native_reviewed_stack_aliases import restore


def counter_probe(source):
    init=re.search(r'^ *belchCount = 0; initialReplySelected = false',source,re.M)[0]
    begin=source.index('belchCount = belchCount + 1')
    end=re.search(r'^ *until (?:true|not \(false\))',source[begin:],re.M).end()+begin
    body=source[begin:end]
    lua=LuaRuntime(unpack_returned_tuples=True)
    return lua.execute("""local belchCount,initialReplySelected
local quest={};local me,hero={},{};local events
function quest:GetHero() return hero end
function quest:AddNewConversation(actor,a,b) assert(actor==me and a==false and b==false);return 57 end
function quest:AddPersonToConversation(id,actor) assert(id==57 and actor==hero) end
function quest:AddLineToConversation(id,line,speaker,listener,flag)
 assert(id==57 and speaker==me and listener==hero and flag==false);events[#events+1]=line
end
function quest:SetStateBool(key,value) assert(key=='BelchedAtBully' and value==true);events[#events+1]=key end
"""+init+"""
return function()
events={}
"""+body+"""
do return belchCount,table.concat(events,','),false end
::LAB_00e5cd53::
return belchCount,table.concat(events,','),true
end""")


def test_generated_four_belches_follow_native_switch(evidence):
    _,_,_,source=evidence;probe=counter_probe(source)
    for count,text in enumerate(('TEXT_QST_015_BULLY_FIRST_BELCH','TEXT_QST_015_BULLY_SECOND_BELCH','TEXT_QST_015_BULLY_THIRD_BELCH','BelchedAtBully'),1):
        assert probe()==(count,text,count==4)


@pytest.mark.parametrize('kind',['duplicate','collision','increment','initialize','switch'])
def test_multi_scalar_evidence_rejects_ambiguity(evidence,kind):
    converter,fn,row,_=evidence;row=copy.deepcopy(row)
    source='\n'.join(old for old,_ in row['edits'])
    if kind=='duplicate':row['additionalAliases'].append(row['alias'])
    elif kind=='collision':source+='\nbelchCount=3;'
    else:
        address={'increment':'0x00E5CC16','initialize':'0x00E5B4F0','switch':'0x00E5CC42'}[kind]
        row['nativeInstructions'][address]='00'
    with pytest.raises(ValueError,match='reviewed stack alias'):restore(source,fn,row,converter.rdata)
