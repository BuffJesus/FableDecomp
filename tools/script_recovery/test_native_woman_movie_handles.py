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
return function(route,health,speak,tasks,limit,mode)
 local q,me,hero,r={},{},{},{};local frame,active,starts,ends,sequences=0,nil,0,0,0
 local expressionSent,talkSent=false,false;local polls=0;local registered=false
 Init(q,me)
 local function term()return frame>=limit end
 function q:GetHero()return hero end
 function q:RetailResources()return r end
 function q:NewScriptFrame(actor)assert(actor==me);frame=frame+1;return not term()end
 function q:IsActiveThreadTerminating()return term()end
 function q:GetStateBool(name)assert(name=='LaughingWomanKilled');return false end
 function q:SetIsPushableByHero(actor,flag)assert(actor==me and not flag)end
 function q:SetThingHasInformation(actor,a,b,c)assert(actor==me and not a and not b and not c)end
 function q:RegisterTimer()assert(not registered);registered=true;return 713 end
 function q:GetTimer(id)assert(registered and id==713);return 1 end
 function q:SetTimer(id,value)assert(registered and id==713 and (value==20 or value==6))end
 function q:DeregisterTimer(id)assert(registered and id==713);registered=false end
 function q:IsDistanceBetweenThingsUnder(a,b,d)assert(a==me and b==hero and d==15);return false end
 function q:StartMovieSequence()sequences=sequences+1;if mode~='handle' then error('duplicate sequence start')end end
 function q:PauseAllNonScriptedEntities(flag)assert(type(flag)=='boolean')end
 function q:GetHealth(actor)assert(actor==me);return health and 10 or 0 end
 function r:MemberResource(name,actor)assert(name=='seh_me' and actor==me);return 811 end
 function r:PrepareResource(id)assert(id==811)end
 function r:ScriptThing(id)assert(id==811);return me end
 function r:StartMovie(name)assert(name=='');assert(active==nil,'movie already live');starts=starts+1;active=1000+starts;return active end
 function r:DestroyMovie(id)
  if mode~='sequence' then assert(id==active and active~=nil,'wrong movie cleanup handle')end
  active=nil;ends=ends+1
 end
 function me:AcquireControl(kind)assert(kind==4);return true end
 function me:MsgExpressionPerformedTo()
  if expressionSent or route=='talk' then return nil end;expressionSent=true;return route
 end
 function me:IsTalkedToByHero()if route~='talk' or talkSent then return false end;talkSent=true;return true end
 -- Existing speech operand/termination mapping is deliberately outside this resource identity probe.
 function me:Speak(...)return speak end
 function me:IsPerformingScriptTask()polls=polls+1;return polls<=tasks end
 debug.sethook(function()error('instruction budget')end,'',200000)
 Main(q,me);debug.sethook()
 assert(active==nil and not registered and starts==ends,'movie/timer remains live')
 assert(starts==(limit>1 and 1 or 0),'movie route not exercised')
 if mode~='handle' then assert(sequences==0,'extra native sequence')end
 return starts,ends,sequences
end
'''
CASES=list(itertools.product(('EXPRESSION_SHIT','EXPRESSION_PELVIC_THRUST','EXPRESSION_COSSACK','EXPRESSION_GIGGLE','talk'),(False,True),(False,True),(1,3),(1,2,5),('handle','sequence','all')))
@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('route,health,speak,tasks,limit,mode',CASES)
def test_movie_lifecycle(generated,style,route,health,speak,tasks,limit,mode):
 source=generated[style]
 lua=LuaRuntime();lua.execute(source);lua.execute(H)(route,health,speak,tasks,limit,mode)

@pytest.fixture(scope='module')
def review_input(tmp_path_factory):
 from tools.script_recovery import native_reviewed_movie_members as adapter
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json')
 unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text())
 unit['entities']={'WomanToAttract':unit['entities']['WomanToAttract']};unit['quest']['functions']={};unit['quest']['helpers']={}
 row=unit['reviewedMovieMembers']['0x00ecf7f0'];original=adapter.restore;captured={}
 def capture(source,fn,review,rd):
  if str(fn['address']).lower()=='0x00ecf7f0':captured.update(source=source,fn=fn,rd=rd)
  return original(source,fn,review,rd)
 with patch.object(c,'constructor_defaults',return_value={}),patch.object(adapter,'restore',capture):c.convert(unit,tmp_path_factory.mktemp('woman_movie_review'))
 return captured,row
@pytest.mark.parametrize('index',range(3))
@pytest.mark.parametrize('fault',('native','source','instruction','slot','frame','constructor','receiver','alias_collision','constructor_missing','constructor_duplicate','start_missing','start_duplicate','source_args','start_site','depth','native_pushed','input_lea','output_lea','push','table','count','additional_duplicate','additional_missing'))
def test_stale_movie_review(review_input,index,fault):
 import copy
 from tools.script_recovery import native_reviewed_movie_members as adapter
 from tools.script_recovery.test_native_inline_self_argument import ChangedBytes
 captured,row=review_input;row=copy.deepcopy(row);fn=copy.deepcopy(captured['fn']);text=captured['source'];rd=captured['rd'];alias=row['stackReceiverAliases'][index];start=alias['startOperands']
 if fault=='native':row['nativeSha256']='0'*64
 elif fault=='source':fn['decompile']+='\n'
 elif fault=='instruction':row['nativeInstructions'][alias['receiverLeas'][0]]='90'
 elif fault=='slot':alias['entrySlot']+=4
 elif fault=='frame':alias['frameBytes']+=4
 elif fault=='constructor':alias['constructorTarget']='0x006e7b80'
 elif fault=='receiver':alias['constructorReceiver']='missing'
 elif fault=='alias_collision':text+='\n'+alias['new']+';'
 elif fault in ('constructor_missing','constructor_duplicate'):
  receiver=alias['constructorReceiver'];needle='CCarriedReadableDef::CCarriedReadableDef('+('' if index<2 else '(CCarriedReadableDef *)')+receiver+');'
  assert text.count(needle)==1
  text=text.replace(needle,'') if fault=='constructor_missing' else text+'\n'+needle
 elif fault in ('start_missing','start_duplicate'):
  from tools.script_recovery.convert_quest_unit import _text_order_sites
  entry=next(e for e in _text_order_sites(text,fn) if e[3]['site']==start['site']);a,b=entry[:2]
  text=text[:a]+'MISSING'+text[b:] if fault=='start_missing' else text+'\n'+text[text.rfind('(**(code **)',0,a):b+2]
 elif fault=='source_args':start['sourceArgs']=['wrong']
 elif fault=='start_site':start['site']='0x00ED0730'
 elif fault=='depth':next(c for c in fn['indirectCalls'] if c['site']==start['site'])['depth']+=4
 elif fault=='native_pushed':next(c for c in fn['indirectCalls'] if c['site']==start['site'])['pushedStack'][0]+=4
 elif fault in ('input_lea','output_lea'):row['nativeInstructions'][start['inputLea' if fault=='input_lea' else 'outputLea']]='90'
 elif fault=='push':
  at=int(start['outputLea'],16);key=next(k for k in row['nativeInstructions'] if int(k,16)==at+next(i.size for i in __import__('capstone').Cs(__import__('capstone').CS_ARCH_X86,__import__('capstone').CS_MODE_32).disasm(rd.bytes_at(at,16),at)));row['nativeInstructions'][key]='90'
 elif fault=='table':rd=ChangedBytes(rd,{0x01260f0c+0x5c8:b'\0'*4})
 elif fault=='count':alias['count']+=1
 elif fault=='additional_duplicate':alias['additionalReceiverSpellings']=[dict(name=alias['old'],count=1)]
 elif fault=='additional_missing':alias['additionalReceiverSpellings']=[dict(name='MISSING_MOVIE',count=1)]
 with pytest.raises(ValueError,match='reviewed movie members'):adapter.restore(text,fn,row,rd)
def test_no_review_preserves_source(review_input):
 from tools.script_recovery.native_reviewed_movie_members import restore
 captured,_=review_input;assert restore(captured['source'],captured['fn'],None,captured['rd'])==captured['source']


def test_original_native_movie_lifetimes():
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 import unicorn as u
 from unicorn import x86_const as x
 rd=RData();target=int.from_bytes(rd.bytes_at(0x01260f0c+0x5c8,4),'little');assert target==0x89b110 and callee_stack_words(target)==2 and callee_stack_words(0x99ebf0)==2
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xecf000,0x2000),(0x6e7000,0x1000),(0x99a000,0x5000),(0x300000,0x10000),(0x500000,0x4000),(0x200000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xecf7f0,rd.bytes_at(0xecf7f0,4885))
 helpers={a:rd.bytes_at(a,n) for a,n in ((0x6e7b60,26),(0x6e7b80,66),(0x99a380,16),(0x99a430,64))}
 assert {hex(a):raw.hex() for a,raw in helpers.items()}=={'0x6e7b60': '568bf1e818282b0033c0c706f40e260189460889460c8bc65ec3', '0x6e7b80': '568bf18b460c85c0741cff088b460c83380075128b4808ff50048b460c50e8196e510083c404c7460800000000c7460c00000000c7068c0026018bce5ee96e282b00', '0x99a380': '8bc1c700d8a72901c7400400000000c3', '0x99a430': '568bf18b460485c0c706d8a729017420c700000000008b46048b480485c9750950e86645260083c404c7460400000000c706101723015ec3cccccccccccccccc'}
 for a,raw in helpers.items():cpu.mem_write(a,raw)
 cpu.mem_write(0x99ebf0,b'\xc3');cpu.mem_write(0x200100,b'\xc3')
 sp,gsi,table=0x300800,0x500000,0x501000
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 put(gsi,table);put(table+0x5c8,0x200100);state={}
 def hook(machine,a,size,user):
  if a==0x99ebf0:
   stack=machine.reg_read(x.UC_X86_REG_ESP);receiver=machine.reg_read(x.UC_X86_REG_ECX)
   assert receiver==sp+state['string'] and [word(stack+4),word(stack+8)]==[0x122d70e,0xffffffff]
   put(receiver,0);machine.reg_write(x.UC_X86_REG_EAX,receiver);machine.reg_write(x.UC_X86_REG_EIP,word(stack));machine.reg_write(x.UC_X86_REG_ESP,stack+12)
  elif a==0x200100:
   stack=machine.reg_read(x.UC_X86_REG_ESP);assert machine.reg_read(x.UC_X86_REG_ECX)==gsi
   assert [word(stack+4),word(stack+8)]==[sp+state['string'],sp+state['movie']]
   obj=word(stack+8);assert [word(obj+i) for i in (0,4,8,12)]==[0x01260ef4,0,0,0]
   # A cookie in the implementation word, null shared-ref word: no engine allocation claim.
   put(obj+8,state['cookie']);state['starts']+=1
   machine.reg_write(x.UC_X86_REG_EIP,word(stack));machine.reg_write(x.UC_X86_REG_ESP,stack+12)
  elif a==0x6e7b80:
   assert machine.reg_read(x.UC_X86_REG_ECX)==sp+state['movie'];state['ends']+=1
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 groups=[(0xed00e0,0xed010e,0x60,0x3c,[(0xed01e4,0xed01ed),(0xed09fc,0xed0a05),(0xed0a1f,0xed0a28)]),(0xed02ff,0xed032d,0x70,0x40,[(0xed0687,0xed0690),(0xed0a62,0xed0a6b)]),(0xed070f,0xed0743,0x80,0x34,[(0xed099b,0xed09a7),(0xed0a85,0xed0a91),(0xed0a9f,0xed0aab)])]
 rows=[]
 for owner,cookie in itertools.product((0x502000,0x503000),(0,1,713,0x12345678)):
  put(owner+4,gsi)
  for begin,end,movie,string,cleanups in groups:
   for cleanup_begin,cleanup_end in cleanups:
    state=dict(movie=movie,string=string,cookie=cookie,starts=0,ends=0)
    cpu.mem_write(sp,b'\xa5'*0xb0);cpu.reg_write(x.UC_X86_REG_EBP,owner);cpu.reg_write(x.UC_X86_REG_ESP,sp)
    cpu.emu_start(begin,end,count=300)
    assert cpu.reg_read(x.UC_X86_REG_ESP)==sp and word(sp+movie+8)==cookie
    cpu.emu_start(cleanup_begin,cleanup_end,count=300)
    assert cpu.reg_read(x.UC_X86_REG_ESP)==sp and [word(sp+movie+i) for i in (4,8,12)]==[0,0,0]
    assert state['starts']==state['ends']==1
    rows.append(dict(owner=owner,cookie=cookie,start=begin,cleanup=cleanup_begin,movie_entry_slot=movie-228,string_entry_slot=string-228))
