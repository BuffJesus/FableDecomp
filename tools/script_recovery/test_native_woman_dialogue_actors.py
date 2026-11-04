import itertools,json,re,struct,sys
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file());sys.path.insert(0,str(ROOT))
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.build_readable_unit import readable_file
@pytest.fixture(scope='module')
def generated(tmp_path_factory):
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json')
 unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text())
 unit['entities']={'WomanToAttract':unit['entities']['WomanToAttract']};unit['quest']['functions']={};unit['quest']['helpers']={}
 out=tmp_path_factory.mktemp('woman_timer')
 with patch.object(c,'constructor_defaults',return_value={}):c.convert(unit,out)
 source=(out/'FSE/V_SickChild/Entities/WomanToAttract.lua').read_text()
 return {'draft':source,'readable':readable_file(source)[0]}


H=r'''
return function(limit,acquireDelay,near,killed,laughs,mode)
 local frame,sets,registered,remaining,lines,animations=0,0,false,0,0,0
 local q,me,hero,r={},{},{},{}
 Init(q,me)
 for n=1,40 do
  local name,value=debug.getupvalue(Main,n);if not name then break end
  if type(value)=='table' and value.SetStateInt then value:SetStateInt('TimesMadeLaugh',laughs)end
  if name=='timesMadeLaugh_' then debug.setupvalue(Main,n,laughs)end
 end
 function me:MsgExpressionPerformedTo()return nil end
 function me:IsTalkedToByHero()return false end
 local function term()return frame>=limit end
 function q:GetHero()return hero end
 function q:RetailResources()return r end
 function q:NewScriptFrame(actor)assert(actor==me);frame=frame+1;return not term()end
 function q:IsActiveThreadTerminating()return term()end
 function q:GetStateBool(name)assert(name=='LaughingWomanKilled');return killed end
 function q:RemoveThing(actor,a,b)assert(actor==me and not a and b)end
 function q:SetIsPushableByHero(actor,value)assert(actor==me and value==false)end
 function q:SetThingHasInformation(actor,a,b,c)assert(actor==me and not a and not b and not c)end
 function q:RegisterTimer()assert(not registered);registered=true;return 713 end
 function q:GetTimer(id)assert(registered and id==713);return remaining end
 function q:SetTimer(id,value)assert(registered and id==713 and value==20,'lost native timer ID/duration');sets=sets+1;remaining=value end
 function q:DeregisterTimer(id)assert(registered and id==713,'wrong/duplicate timer cleanup');registered=false end
 function q:IsDistanceBetweenThingsUnder(a,b,d)assert(a==me and b==hero and d==15);return near end
 function q:AddNewConversation(actor,a,b)
  if mode~='line' then assert(actor==me and a==false and b==false,'wrong conversation actor/flags')end
  return 719
 end
 function q:AddPersonToConversation(id,actor)assert(id==719 and actor==hero)end
 function q:AddLineToConversation(id,key,actor,target,flag)
  assert(id==719);if mode~='conversation' then assert(actor==me and target==hero and flag==false,'wrong line speaker/listener/flag')end;lines=lines+1
  local expected=({'TEXT_QST_B10_LAUGHING_WOMAN_NOT_LAUGHED','TEXT_QST_B10_LAUGHING_WOMAN_LAUGHED_A_BIT','TEXT_QST_B10_LAUGHING_WOMAN_NEARLY_HYSTERICAL'})[laughs+1]
  if lines==1 then assert(key==expected)else assert(key==(laughs==1 and 'TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_ONCE' or 'TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_TWICE'))end
 end
 function r:MemberResource(name,actor)assert(name=='seh_me' and actor==me);return 811 end
 function r:PrepareResource(id)assert(id==811)end
 function me:AcquireControl(kind)assert(kind==4);return frame>=acquireDelay end
 function me:PlayAnimation(name,a,b,c,d,e,f,g)
  local expected=({'STANDARD_TALK_SADNESS','ST_OPINION_RIDICULE_SNIGGER','ST_OPINION_RIDICULE_POINT_AND_LAUGH'})[laughs+1]
  assert(name==expected);animations=animations+1
 end
 debug.sethook(function()error('instruction budget')end,'',100000)
 Main(q,me);debug.sethook()
 local expected=(near and limit>math.max(1,acquireDelay)) and 1 or 0
 assert(not registered and sets==expected,'timer lifecycle/coverage mismatch')
 assert(animations==expected and lines==expected*(laughs==0 and 1 or 2),'dialogue count/order changed')
 return sets,lines,animations
end
'''
CASES=list(itertools.product((1,2,5),(0,3),(False,True),(False,True),(0,1,2),('conversation','line','all')))
@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('limit,acquire,near,killed,laughs,mode',CASES)
def test_dialogue_lifecycle(generated,style,limit,acquire,near,killed,laughs,mode):
 source=generated[style]
 lua=LuaRuntime();lua.execute(source);lua.execute(H)(limit,acquire,near,killed,laughs,mode)

@pytest.fixture(scope='module')
def review_input(tmp_path_factory):
 from tools.script_recovery import native_reviewed_stack_aliases as adapter
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json')
 unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text())
 row=unit['reviewedStackScalarAliases']['0x00ecf7f0']
 unit['entities']={'WomanToAttract':unit['entities']['WomanToAttract']};unit['quest']['functions']={};unit['quest']['helpers']={}
 original=adapter.restore;captured={}
 def capture(source,fn,row,rd):
  if str(fn['address']).lower()=='0x00ecf7f0':captured.update(source=source,fn=fn,rd=rd)
  return original(source,fn,row,rd)
 with patch.object(c,'constructor_defaults',return_value={}),patch.object(adapter,'restore',capture):c.convert(unit,tmp_path_factory.mktemp('woman_timer_review'))
 return captured,row
@pytest.mark.parametrize('fault',('no_review','native','source','instruction','alias','missing','duplicate','edit_duplicate',
 'self_store','self_reload','pointer_alias_missing','speaker_duplicate','speaker_missing',
 'counts_absent','counts_type','counts_length','counts_zero','counts_negative','counts_bool','counts_wrong'))
def test_review_guards(review_input,fault):
 import copy
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 captured,row=review_input;row=copy.deepcopy(row);fn=copy.deepcopy(captured['fn']);source=captured['source'];rd=captured['rd']
 if fault=='no_review':assert restore(source,fn,None,rd)==source;return
 if fault=='native':row['nativeSha256']='0'*64
 elif fault=='source':fn['decompile']+='\n'
 elif fault=='instruction':row['nativeInstructions'][next(iter(row['nativeInstructions']))]='90'
 elif fault=='alias':row['alias']='existing';source+='\nexisting;'
 elif fault=='missing':source=source.replace(row['edits'][0][0],'')
 elif fault=='duplicate':source+='\n'+row['edits'][1][0]
 elif fault=='edit_duplicate':row['edits'].append(copy.deepcopy(row['edits'][0]))
 elif fault=='self_store':row['nativeInstructions']['0xecf814']='90'
 elif fault=='self_reload':row['nativeInstructions']['0xecf99e']='90'
 elif fault=='pointer_alias_missing':row.pop('additionalAliases')
 elif fault=='speaker_duplicate':source+='\n'+row['edits'][-1][0]
 elif fault=='speaker_missing':source=source.replace(row['edits'][-1][0],'')
 elif fault=='counts_absent':row.pop('editCounts')
 elif fault=='counts_type':row['editCounts']='bad'
 elif fault=='counts_length':row['editCounts'].pop()
 elif fault.startswith('counts_'):row['editCounts'][1]={'counts_zero':0,'counts_negative':-1,'counts_bool':True,'counts_wrong':3}[fault]
 with pytest.raises(ValueError,match='reviewed stack alias'):restore(source,fn,row,rd)


def test_original_native_dialogue_layouts():
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 import unicorn as u
 from unicorn import x86_const as x
 rd=RData();targets={slot:int.from_bytes(rd.bytes_at(0x01260f0c+slot,4),'little') for slot in (0x118,0x5b0,0x5b8)}
 assert {slot:callee_stack_words(a) for slot,a in targets.items()}=={0x118:0,0x5b0:3,0x5b8:5}
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xecf000,0x2000),(0x300000,0x10000),(0x500000,0x60000),(0x200000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xecf7f0,rd.bytes_at(0xecf7f0,4885))
 gsi,vtable,entry=0x500000,0x501000,0x300800
 callbacks={0x200100:('hero',0),0x200110:('conversation',12),0x200120:('line',20)}
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 put(gsi,vtable)
 for slot,a in ((0x118,0x200100),(0x5b0,0x200110),(0x5b8,0x200120)):put(vtable+slot,a);cpu.mem_write(a,b'\xc3')
 state={}
 def hook(machine,a,size,user):
  if a in callbacks:
   name,purge=callbacks[a];sp=machine.reg_read(x.UC_X86_REG_ESP);assert machine.reg_read(x.UC_X86_REG_ECX)==gsi
   if name=='hero':machine.reg_write(x.UC_X86_REG_EAX,state['hero'])
   elif name=='conversation':
    values=[word(sp+k) for k in (4,8,12)];assert values==[state['owner']+8,0,0]
    machine.reg_write(x.UC_X86_REG_EAX,state['conversation']);state['events'].append(dict(kind=name,values=values))
   else:
    values=[word(sp+k) for k in (4,8,12,16,20)]
    assert values==[state['conversation'],state['key'],0,state['owner']+8,state['hero']]
    state['events'].append(dict(kind=name,values=values))
   machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+4+purge)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 rows=[]
 for owner,hero,conversation in itertools.product((0x510000,0x520000,0x530000),(0x540000,0x550000),(0,1,719)):
  state=dict(owner=owner,hero=hero,conversation=conversation,events=[]);put(owner+4,gsi)
  cpu.reg_write(x.UC_X86_REG_ESP,entry);cpu.reg_write(x.UC_X86_REG_ECX,owner)
  cpu.emu_start(0xecf7f0,0xecf818,count=100)
  assert cpu.reg_read(x.UC_X86_REG_ESP)==entry-232 and word(entry-192)==owner+8
  balanced=entry-228
  # Constructor/condition/frame/resource/distance calls are outside these segments.
  cpu.reg_write(x.UC_X86_REG_ESP,balanced);cpu.emu_start(0xecf99b,0xecf9af,count=100)
  assert cpu.reg_read(x.UC_X86_REG_ESP)==balanced
  for begin,end,key_offset in ((0xecf9f1,0xecfa13,0x4c),(0xecfa66,0xecfa88,0x50),
                                (0xecfaa1,0xecfac3,0x58),(0xecfb15,0xecfb37,0x5c),
                                (0xecfb4d,0xecfb6f,0x30)):
   cpu.reg_write(x.UC_X86_REG_ESP,balanced);cpu.reg_write(x.UC_X86_REG_ESI,conversation)
   state['key']=balanced+key_offset
   cpu.emu_start(begin,end,count=100)
   assert cpu.reg_read(x.UC_X86_REG_ESP)==balanced and word(entry-192)==owner+8
  assert len(state['events'])==6
  rows.append(dict(owner=owner,hero=hero,conversation=conversation,events=state['events']))
