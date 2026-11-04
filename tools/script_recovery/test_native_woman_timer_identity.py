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
return function(limit,acquireDelay,near,killed,laughs)
 local frame,sets,registered,remaining,lines,animations=0,0,false,0,0,0
 local q,me,hero,r={},{},{},{}
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
 -- Actor/flag operands in these calls remain independently unresolved in production.
 -- This harness records their dialogue effects; it does not certify those operands.
 function q:AddNewConversation(actor,a,b)return 719 end
 function q:AddPersonToConversation(id,actor)assert(id==719 and actor==hero)end
 function q:AddLineToConversation(id,key,actor,target,flag)
  assert(id==719);lines=lines+1
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
CASES=list(itertools.product((1,2,5),(0,3),(False,True),(False,True),(0,1,2)))
@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('limit,acquire,near,killed,laughs',CASES)
def test_timer_lifecycle(generated,style,limit,acquire,near,killed,laughs):
 source=generated[style]
 lua=LuaRuntime();lua.execute(source);lua.execute(H)(limit,acquire,near,killed,laughs)

FRAGMENT_CASES=list(itertools.product((0,1,2),(1,713,0x12345678),(0,7,20,100)))
@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('kind,timer_id,time',FRAGMENT_CASES)
def test_actual_update_fragment(generated,style,kind,timer_id,time):
 source=generated[style]
 alias=re.search(r'(\w+) = quest:RegisterTimer\(\)',source)[1]
 lines=source.splitlines();sites=[i for i,l in enumerate(lines) if 'quest:SetTimer(' in l];assert len(sites)==3
 at=sites[kind];fragment=lines[at].strip();declarations=''
 if kind==2 and 'quest:GetTimer(' not in fragment:
  query=lines[at-1].strip();assert 'quest:GetTimer(' in query
  if not query.startswith('local '):declarations='local '+query.split('=',1)[0].strip()+';'
  fragment=query+'\n'+fragment
 lua=LuaRuntime();run=lua.execute('return function(quest,id) local '+alias+'=id;'+declarations+fragment+' end')
 events=[];q=lua.table()
 q.GetTimer=lambda self,id:(id==timer_id and time) if id==timer_id else (_ for _ in ()).throw(AssertionError('wrong query timer'))
 q.SetTimer=lambda self,id,value:events.append((id,value))
 run(q,timer_id)
 assert events==[(timer_id,20 if kind<2 else time+5)]

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
 elif fault=='counts_absent':row.pop('editCounts')
 elif fault=='counts_type':row['editCounts']='bad'
 elif fault=='counts_length':row['editCounts'].pop()
 elif fault.startswith('counts_'):row['editCounts'][1]={'counts_zero':0,'counts_negative':-1,'counts_bool':True,'counts_wrong':3}[fault]
 with pytest.raises(ValueError,match='reviewed stack alias'):restore(source,fn,row,rd)


def test_original_native_timer_segments():
 from tools.script_recovery.lift_native_lua import RData
 import unicorn as u
 from unicorn import x86_const as x
 rd=RData();cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xecf000,0x2000),(0xcd4000,0x1000),(0x300000,0x10000),(0x500000,0x3000),(0x200000,0x1000),(0x143e000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xecf7f0,rd.bytes_at(0xecf7f0,4885));cpu.mem_write(0xcd4470,rd.bytes_at(0xcd4470,20))
 gsi,vtable,balanced=0x500000,0x501000,0x300800
 callbacks={0x200100:('register',0),0x200110:('get',4),0x200120:('set',8),0x200130:('deregister',4)}
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 put(0x143e8f8,gsi);put(gsi,vtable)
 for slot,target in ((0x15c,0x200100),(0x168,0x200110),(0x164,0x200120),(0x160,0x200130)):put(vtable+slot,target);cpu.mem_write(target,b'\xc3')
 state={}
 def hook(machine,address,size,user):
  if address in callbacks:
   name,purge=callbacks[address];sp=machine.reg_read(x.UC_X86_REG_ESP)
   assert machine.reg_read(x.UC_X86_REG_ECX)==gsi
   if name=='register':machine.reg_write(x.UC_X86_REG_EAX,state['id']);state['events'].append([name,state['id']])
   else:
    timer_id=word(sp+4);assert timer_id==state['id']
    if name=='get':machine.reg_write(x.UC_X86_REG_EAX,state['time']);state['events'].append([name,timer_id,state['time']])
    elif name=='set':state['events'].append([name,timer_id,word(sp+8)])
    else:state['events'].append([name,timer_id])
   machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+4+purge)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 rows=[]
 for timer_id,time,cleanup in itertools.product((0,1,713,0x12345678),(0,7,20),('direct','helper')):
  state=dict(id=timer_id,time=time,events=[])
  segments=[(0xecf92f,0xecf943),(0xecf950,0xecf963),(0xecfbb2,0xecfbc7),(0xed0666,0xed067b),(0xed0965,0xed098f)]
  segments.append((0xed09c9,0xed09dc) if cleanup=='direct' else (0xed0ae8,0xed0af1))
  for start,end in segments:
   cpu.reg_write(x.UC_X86_REG_ESP,balanced);cpu.emu_start(start,end,count=100)
   assert cpu.reg_read(x.UC_X86_REG_ESP)==balanced
   assert word(balanced+0x14)==timer_id
  assert state['events']==[['register',timer_id],['get',timer_id,time],['set',timer_id,20],['set',timer_id,20],['get',timer_id,time],['set',timer_id,time+5],['deregister',timer_id]]
  rows.append(dict(timer_id=timer_id,time=time,cleanup=cleanup,events=state['events']))
