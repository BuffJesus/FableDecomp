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



FUNNY={'EXPRESSION_COSSACK', 'EXPRESSION_SATURDAY_NIGHT_FEVER', 'EXPRESSION_TAP', 'EXPRESSION_AIR_GUITAR', 'EXPRESSION_COCK_A_DOODLE_DO', 'EXPRESSION_FLAMENCO', 'EXPRESSION_BALLET', 'EXPRESSION_GIGGLE', 'EXPRESSION_PELVIC_THRUST', 'EXPRESSION_CROTCH_GRAB', 'EXPRESSION_KISS_MY_ASS'}
CASES=[('EXPRESSION_PELVIC_THRUST', False, False), ('EXPRESSION_PELVIC_THRUST', True, False), ('EXPRESSION_COCK_A_DOODLE_DO', False, False), ('EXPRESSION_COCK_A_DOODLE_DO', True, False), ('EXPRESSION_CROTCH_GRAB', False, False), ('EXPRESSION_CROTCH_GRAB', True, False), ('EXPRESSION_KISS_MY_ASS', False, False), ('EXPRESSION_KISS_MY_ASS', True, False), ('EXPRESSION_FLAMENCO', False, False), ('EXPRESSION_FLAMENCO', True, False), ('EXPRESSION_COSSACK', False, False), ('EXPRESSION_COSSACK', True, False), ('EXPRESSION_AIR_GUITAR', False, False), ('EXPRESSION_AIR_GUITAR', True, False), ('EXPRESSION_BALLET', False, False), ('EXPRESSION_BALLET', True, False), ('EXPRESSION_SATURDAY_NIGHT_FEVER', False, False), ('EXPRESSION_SATURDAY_NIGHT_FEVER', True, False), ('EXPRESSION_TAP', False, False), ('EXPRESSION_TAP', True, False), ('EXPRESSION_GIGGLE', False, False), ('EXPRESSION_GIGGLE', True, False), ('EXPRESSION_SHIT', False, False), ('EXPRESSION_SHIT', True, False), ('EXPRESSION_FLIRT', False, False), ('EXPRESSION_FLIRT', True, False), ('EXPRESSION_HEROIC_STANCE', False, False), ('EXPRESSION_HEROIC_STANCE', True, False), ('EXPRESSION_VICTORY_PUMP', False, False), ('EXPRESSION_VICTORY_PUMP', True, False), ('', False, False), ('', True, False), ('UNKNOWN', False, False), ('UNKNOWN', True, False), ('EXPRESSION_GIGGLE_X', False, False), ('EXPRESSION_GIGGLE_X', True, False), ('expression_giggle', False, False), ('expression_giggle', True, False), ('EXPRESSION_GIGGL', False, False), ('EXPRESSION_GIGGL', True, False), ('EXPRESSION_TAP ', False, False), ('EXPRESSION_TAP ', True, False), ('EXPRESSION_FLAMENCO_X', False, False), ('EXPRESSION_FLAMENCO_X', True, False), ('EXPRESSION_PELVIC_THRUS', False, False), ('EXPRESSION_PELVIC_THRUS', True, False), ('EXPRESSION_PELVIC_THRUSTX', False, False), ('EXPRESSION_PELVIC_THRUSTX', True, False), ('EXPRESSION_COCK_A_DOODLE_D', False, False), ('EXPRESSION_COCK_A_DOODLE_D', True, False), ('EXPRESSION_COCK_A_DOODLE_DOX', False, False), ('EXPRESSION_COCK_A_DOODLE_DOX', True, False), ('EXPRESSION_CROTCH_GRA', False, False), ('EXPRESSION_CROTCH_GRA', True, False), ('EXPRESSION_CROTCH_GRABX', False, False), ('EXPRESSION_CROTCH_GRABX', True, False), ('EXPRESSION_KISS_MY_AS', False, False), ('EXPRESSION_KISS_MY_AS', True, False), ('EXPRESSION_KISS_MY_ASSX', False, False), ('EXPRESSION_KISS_MY_ASSX', True, False), ('EXPRESSION_FLAMENC', False, False), ('EXPRESSION_FLAMENC', True, False), ('EXPRESSION_FLAMENCOX', False, False), ('EXPRESSION_FLAMENCOX', True, False), ('EXPRESSION_COSSAC', False, False), ('EXPRESSION_COSSAC', True, False), ('EXPRESSION_COSSACKX', False, False), ('EXPRESSION_COSSACKX', True, False), ('EXPRESSION_AIR_GUITA', False, False), ('EXPRESSION_AIR_GUITA', True, False), ('EXPRESSION_AIR_GUITARX', False, False), ('EXPRESSION_AIR_GUITARX', True, False), ('EXPRESSION_BALLE', False, False), ('EXPRESSION_BALLE', True, False), ('EXPRESSION_BALLETX', False, False), ('EXPRESSION_BALLETX', True, False), ('EXPRESSION_SATURDAY_NIGHT_FEVE', False, False), ('EXPRESSION_SATURDAY_NIGHT_FEVE', True, False), ('EXPRESSION_SATURDAY_NIGHT_FEVERX', False, False), ('EXPRESSION_SATURDAY_NIGHT_FEVERX', True, False), ('EXPRESSION_TA', False, False), ('EXPRESSION_TA', True, False), ('EXPRESSION_TAPX', False, False), ('EXPRESSION_TAPX', True, False), ('EXPRESSION_GIGGL', False, False), ('EXPRESSION_GIGGL', True, False), ('EXPRESSION_GIGGLEX', False, False), ('EXPRESSION_GIGGLEX', True, False), (None, False, False), (None, False, True), (None, True, False), (None, True, True)]
H=r'''
return function(expression,given,talked,expected)
 local q,me,hero,r={},{},{},{};local frame=0;local state;local initialized=false
 Init(q,me)
 for n=1,80 do
  local name,value=debug.getupvalue(Main,n);if not name then break end
  if type(value)=='table' and value.SetStateInt then state=value;value:SetStateBool('GivenObject',given);initialized=true end
  if name=='givenObject' or name=='givenObject_' then debug.setupvalue(Main,n,given);initialized=true end
 end
 assert(initialized,'GivenObject state setup not reached')
 local function count()
  if state then return state:GetStateInt('TimesMadeLaugh')end
  for n=1,80 do local name,value=debug.getupvalue(Main,n);if not name then break end;if name=='timesMadeLaugh_' then return value end end
  error('missing native counter')
 end
 function q:GetHero()return hero end
 function q:RetailResources()return r end
 function q:NewScriptFrame(actor)assert(actor==me);frame=frame+1;if frame>1 then error('__dispatch_skipped')end;return true end
 function q:IsActiveThreadTerminating()if count()==1 then error('__dispatch_funny')end;return false end
 function q:GetStateBool(name)assert(name=='LaughingWomanKilled');return false end
 function q:SetIsPushableByHero(actor,flag)assert(actor==me and not flag)end
 function q:SetThingHasInformation(actor,a,b,c)assert(actor==me and not a and not b and not c)end
 function q:RemoveThing(actor,a,b)assert(given and actor==me and not a and b)end
 function q:RegisterTimer()return 713 end
 function q:GetTimer(id)assert(id==713);return 1 end
 function q:DeregisterTimer(id)assert(id==713 and given);error('__dispatch_skipped')end
 function r:MemberResource(name,actor)assert(name=='seh_me' and actor==me);return 811 end
 function r:PrepareResource(id)assert(id==811)end
 function r:StartMovie(key)error(expression~=nil and '__dispatch_other' or '__dispatch_talked')end
 function me:AcquireControl(kind)assert(kind==4);return true end
 function me:MsgExpressionPerformedTo()return expression end
 function me:IsTalkedToByHero()return talked end
 debug.sethook(function()error('instruction budget')end,'',100000)
 local ok,err=pcall(Main,q,me);debug.sethook()
 assert(not ok and string.find(err,'__dispatch_'..expected,1,true),tostring(err)..' expected '..expected)
 assert(count()==(expected=='funny' and 1 or 0),'dispatch counter changed')
 return expected
end
'''
@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('expression,given,talked',CASES)
def test_dispatch(generated,style,expression,given,talked):
 source=generated[style]
 expected='skipped' if given or (expression is None and not talked) else 'talked' if expression is None else 'funny' if expression in FUNNY else 'other'
 lua=LuaRuntime();lua.execute(source);lua.execute(H)(expression,given,talked,expected)

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
 'message_reload','output_missing','output_duplicate','compare_missing','compare_duplicate','self_store','self_reload','pointer_alias_missing','speaker_duplicate','speaker_missing',
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
 elif fault=='message_reload':row['nativeInstructions']['0xecfbc7']='90'
 elif fault=='output_missing':source=source.replace('(&xStack_cc);','();')
 elif fault=='output_duplicate':source+='\n'+next(old for old,new in row['edits'] if '0x74' in old)
 elif fault=='compare_missing':source=source.replace('CBasicString<char>::Compare(pvVar11,"EXPRESSION_FLIRT")','MISSING()')
 elif fault=='compare_duplicate':source+='\nCBasicString<char>::Compare(pvVar11,"EXPRESSION_FLIRT");'
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




def test_native_string_dispatch():
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 import unicorn as u
 from unicorn import x86_const as x
 rd=RData();assert rd.bytes_at(0x411570,40).hex()=='538a0184c07505803a00740c8a1a3ac37c0a7f0d4142ebe933c05bc383c8ff5bc3b8010000005bc3';cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xecf000,0x2000),(0x411000,0x1000),(0x300000,0x10000),(0x500000,0x10000)):cpu.mem_map(a,n)
 cpu.mem_write(0xecf7f0,rd.bytes_at(0xecf7f0,4885));cpu.mem_write(0x411570,rd.bytes_at(0x411570,40))
 for name,va,n in rd.names:
  if name=='.rdata':
   addr=rd.base+va;cpu.mem_map(addr,(n+4095)&~4095);cpu.mem_write(addr,rd.bytes_at(addr,n))
 sp,rep,buffer=0x300800,0x500100,0x500200
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v))
 FUNNY=['EXPRESSION_'+s for s in ('PELVIC_THRUST','COCK_A_DOODLE_DO','CROTCH_GRAB','KISS_MY_ASS','FLAMENCO','COSSACK','AIR_GUITAR','BALLET','SATURDAY_NIGHT_FEVER','TAP','GIGGLE')]
 OTHER=['EXPRESSION_'+s for s in ('SHIT','FLIRT','HEROIC_STANCE','VICTORY_PUMP')]
 names=FUNNY+OTHER+['','UNKNOWN','EXPRESSION_GIGGLE_X','expression_giggle','EXPRESSION_GIGGL','EXPRESSION_TAP ','EXPRESSION_FLAMENCO_X']
 for s in FUNNY:names.extend([s[:-1],s+'X'])
 state={}
 def hook(machine,a,size,user):
  if a in state['stops']:state['hit']=a;machine.emu_stop()
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 def run(start,stops,name,null):
  state.clear();state.update(stops=stops,hit=None)
  cpu.mem_write(buffer,name.encode()+b'\0');put(rep,buffer);put(sp+0x18,0 if null else rep)
  cpu.reg_write(x.UC_X86_REG_ESP,sp);cpu.reg_write(x.UC_X86_REG_EFLAGS,2)
  cpu.emu_start(start,0xed0b05,count=5000)
  assert state['hit'] is not None and cpu.reg_read(x.UC_X86_REG_ESP)==sp
  return state['hit']
 rows=[]
 for name in names:
  funny=run(0xecfbf9,{0xecfd04,0xecff84},name,False)
  assert funny==(0xecfd04 if name in FUNNY else 0xecff84),(name,hex(funny))
  rude=run(0xecffa3,{0xed002a,0xed005f,0xed0091,0xed00c3,0xed00e0},name,False)
  assert rude==dict(zip(OTHER,[0xed002a,0xed005f,0xed0091,0xed00c3])).get(name,0xed00e0)
  destinations=[0xed0210,0xed0245,0xed027a,0xed02ac,0xed02e2,0xed04c2,0xed02ff]
  animation=run(0xecfd2a,set(destinations),name,False)
  expected=(destinations[FUNNY.index(name)] if name in FUNNY[:5] else 0xed04c2 if name==FUNNY[-1] else 0xed02e2 if name in FUNNY[5:-1] else 0xed02ff)
  assert animation==expected,(name,hex(animation),hex(expected))
  rows.append(dict(name=name,funny=funny,rude=rude,animation=animation))
 assert run(0xecfbf9,{0xecfd04,0xecff84},'',True)==0xecff84
 assert run(0xecffa3,{0xed002a,0xed005f,0xed0091,0xed00c3,0xed00e0},'',True)==0xed00e0
 assert run(0xecfd2a,set(destinations),'',True)==0xed02ff


def test_native_message_layouts():
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 import unicorn as u
 from unicorn import x86_const as x
 rd=RData();targets={slot:int.from_bytes(rd.bytes_at(0x01238c8c+slot,4),'little') for slot in (0x74,0x6c)}
 assert targets=={0x74:0x4aabf0,0x6c:0x4aabd0} and all(callee_stack_words(v)==1 for v in targets.values())
 assert rd.string_at(0x125d1c8)=='SCRIPT_NAME_HERO'
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xecf000,0x2000),(0x300000,0x10000),(0x500000,0x50000),(0x200000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xecf7f0,rd.bytes_at(0xecf7f0,4885))
 entry,table,rep=0x300800,0x500000,0x501000
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 for slot,addr in ((0x74,0x200100),(0x6c,0x200110)):put(table+slot,addr);cpu.mem_write(addr,b'\xc3')
 state={}
 def hook(machine,a,size,user):
  if a in (0x200100,0x200110):
   sp=machine.reg_read(x.UC_X86_REG_ESP);assert machine.reg_read(x.UC_X86_REG_ECX)==state['owner']+8
   out=word(sp+4);expected=state['balanced']+(0x18 if a==0x200100 else 0x48);assert out==expected
   if a==0x200100:put(out,state['value']);value=state['expression']
   else:assert word(out)==rep;value=state['talked']
   state['events'].append(dict(site=hex(a),receiver=state['owner']+8,string_address=out,result=value))
   machine.reg_write(x.UC_X86_REG_EAX,int(value));machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+8)
 cpu.hook_add(u.UC_HOOK_CODE,hook);rows=[]
 for owner,expression,talked,value in itertools.product((0x510000,0x520000,0x530000),(False,True),(False,True),(0,rep)):
  state=dict(owner=owner,expression=expression,talked=talked,value=value,events=[])
  put(owner+8,table);cpu.reg_write(x.UC_X86_REG_ESP,entry);cpu.reg_write(x.UC_X86_REG_ECX,owner);cpu.emu_start(0xecf7f0,0xecf818,count=100)
  balanced=entry-228;state['balanced']=balanced;assert word(entry-192)==owner+8
  cpu.reg_write(x.UC_X86_REG_ESP,balanced);cpu.emu_start(0xecfbc7,0xecfbd7,count=100)
  assert cpu.reg_read(x.UC_X86_REG_ESP)==balanced and word(balanced+0x18)==value and cpu.reg_read(x.UC_X86_REG_EAX)==expression
  put(balanced+0x48,rep);cpu.reg_write(x.UC_X86_REG_ESI,0);cpu.emu_start(0xed06b1,0xed06c4,count=100)
  assert cpu.reg_read(x.UC_X86_REG_ESP)==balanced and cpu.reg_read(x.UC_X86_REG_EAX)==talked
  rows.append({k:v for k,v in state.items() if k!='balanced'})

FRAGMENT_CASES=[('EXPRESSION_PELVIC_THRUST', 15532256, 15532560), ('EXPRESSION_COCK_A_DOODLE_DO', 15532256, 15532613), ('EXPRESSION_CROTCH_GRAB', 15532256, 15532666), ('EXPRESSION_KISS_MY_ASS', 15532256, 15532716), ('EXPRESSION_FLAMENCO', 15532256, 15532770), ('EXPRESSION_COSSACK', 15532256, 15532770), ('EXPRESSION_AIR_GUITAR', 15532256, 15532770), ('EXPRESSION_BALLET', 15532256, 15532770), ('EXPRESSION_SATURDAY_NIGHT_FEVER', 15532256, 15532770), ('EXPRESSION_TAP', 15532256, 15532770), ('EXPRESSION_GIGGLE', 15532256, 15533250), ('EXPRESSION_SHIT', 15532074, 15532799), ('EXPRESSION_FLIRT', 15532127, 15532799), ('EXPRESSION_HEROIC_STANCE', 15532177, 15532799), ('EXPRESSION_VICTORY_PUMP', 15532227, 15532799), ('', 15532256, 15532799), ('UNKNOWN', 15532256, 15532799), ('EXPRESSION_GIGGLE_X', 15532256, 15532799), ('expression_giggle', 15532256, 15532799), ('EXPRESSION_GIGGL', 15532256, 15532799), ('EXPRESSION_TAP ', 15532256, 15532799), ('EXPRESSION_FLAMENCO_X', 15532256, 15532799), ('EXPRESSION_PELVIC_THRUS', 15532256, 15532799), ('EXPRESSION_PELVIC_THRUSTX', 15532256, 15532799), ('EXPRESSION_COCK_A_DOODLE_D', 15532256, 15532799), ('EXPRESSION_COCK_A_DOODLE_DOX', 15532256, 15532799), ('EXPRESSION_CROTCH_GRA', 15532256, 15532799), ('EXPRESSION_CROTCH_GRABX', 15532256, 15532799), ('EXPRESSION_KISS_MY_AS', 15532256, 15532799), ('EXPRESSION_KISS_MY_ASSX', 15532256, 15532799), ('EXPRESSION_FLAMENC', 15532256, 15532799), ('EXPRESSION_FLAMENCOX', 15532256, 15532799), ('EXPRESSION_COSSAC', 15532256, 15532799), ('EXPRESSION_COSSACKX', 15532256, 15532799), ('EXPRESSION_AIR_GUITA', 15532256, 15532799), ('EXPRESSION_AIR_GUITARX', 15532256, 15532799), ('EXPRESSION_BALLE', 15532256, 15532799), ('EXPRESSION_BALLETX', 15532256, 15532799), ('EXPRESSION_SATURDAY_NIGHT_FEVE', 15532256, 15532799), ('EXPRESSION_SATURDAY_NIGHT_FEVERX', 15532256, 15532799), ('EXPRESSION_TA', 15532256, 15532799), ('EXPRESSION_TAPX', 15532256, 15532799), ('EXPRESSION_GIGGL', 15532256, 15532799), ('EXPRESSION_GIGGLEX', 15532256, 15532799), (None, 15532256, 15532799)]
def fragment(source,kind):
 if kind=='rude':
  end=source.index('::LAB_00ed00e0::');start=source.index('::FLOW_past_lab_00ed002a::')
  stops={0xed002a,0xed005f,0xed0091,0xed00c3};default=0xed00e0
 else:
  start=source.index('::FLOW_past_lab_00ed0210::');end=source.index('::LAB_00ed02e2::',start)
  stops={0xed0210,0xed0245,0xed027a,0xed02ac,0xed02e2,0xed04c2,0xed02ff};default=0xed02e2
 assert start>=0
 body=source[start:end]
 body=re.sub(r'goto LAB_([0-9a-f]+)',lambda m:'return '+str(int(m[1],16)) if int(m[1],16) in stops else m[0],body)
 assert 'goto ' not in body,body
 names=set(re.findall(r'^\s*(\w+) = ',body,re.M))|{'pCVar19','scratchValue'};names.discard('local_womanExpressionName')
 return 'return function(local_womanExpressionName) local '+','.join(sorted(names))+'\n'+body+'\nreturn '+str(default)+' end'
@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('kind',('rude','animation'))
@pytest.mark.parametrize('name,rude,animation',FRAGMENT_CASES)
def test_native_destination(generated,style,kind,name,rude,animation):
 source=generated[style]
 lua=LuaRuntime();actual=lua.execute(fragment(source,kind))(name)
 assert actual==(rude if kind=='rude' else animation)
