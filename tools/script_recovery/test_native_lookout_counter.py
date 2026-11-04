import copy,re
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.test_native_lookout_first_reply import evidence
from tools.script_recovery.native_reviewed_stack_aliases import restore

def make_probe(source):
    init=re.search(r'^ *battleCryReplyCount = 0',source,re.M)[0]
    begin=source.index('battleCryReplyCount = (battleCryReplyCount + 1)')
    line=source.index('"TEXT_QST_015_BEGGAR_THIRD_BATTLE_CRY_REPLY"',begin)
    end=source.index('\n',line)
    closing=re.match(r'(?:\n[ \t]*end){1,2}',source[end:]);assert closing
    body=source[begin:end+closing.end()]
    lua=LuaRuntime(unpack_returned_tuples=True)
    return lua.execute("""local battleCryReplyCount
local quest={};local me,hero={},{};local reply
function quest:GetHero() return hero end
function quest:AddNewConversation(actor,a,b) assert(actor==me and a==false and b==false);return 57 end
function quest:AddPersonToConversation(id,actor) assert(id==57 and actor==hero) end
function quest:AddLineToConversation(id,line,speaker,listener,flag)
 assert(id==57 and speaker==me and listener==hero and flag==false);reply=line
end
"""+init+"""
return function(seed)
if seed~=nil then battleCryReplyCount=seed end
reply=nil
"""+body+"\nreturn battleCryReplyCount,reply end")


@pytest.mark.parametrize('seed',[0,1,2,0xfffffffe,0xffffffff])
def test_generated_native_counter_selects_responses_and_wraps(evidence,seed):
    probe=make_probe(evidence[3]);count=seed
    for step in range(4):
        count=(count+1)&0xffffffff;suffix={1:'FIRST',2:'SECOND'}.get(count,'THIRD')
        assert probe(seed if step==0 and seed else None)==(count,'TEXT_QST_015_BEGGAR_'+suffix+'_BATTLE_CRY_REPLY')

@pytest.mark.parametrize('kind',['initialize','increment','store','compare','alias'])
def test_unproven_counter_evidence_rejected(evidence,kind):
    c,fn,row,_=evidence;row=copy.deepcopy(row);source='\n'.join(old for old,_ in row['edits'])
    if kind=='alias':row['additionalAliases'].append(row['alias'])
    else:row['nativeInstructions'][{'initialize':'0x00E58E7B','increment':'0x00E5B055','store':'0x00E5B057','compare':'0x00E5B07D'}[kind]]='00'
    with pytest.raises(ValueError,match='reviewed stack alias'):restore(source,fn,row,c.rdata)
