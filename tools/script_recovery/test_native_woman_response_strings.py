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



H=r'''return function(route,health,speak,tasks,limit,mode,expectedKey)
 local q,me,hero,r={},{},{},{};local frame,active,starts,ends,sequences=0,nil,0,0,0
 local expressionSent,talkSent=false,false;local polls=0;local registered=false;local speechCalls=0
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
 function me:Speak(actor,key,group,a,b,c)assert(actor==hero and key==expectedKey,'wrong selected response text');assert(group==0 and a==false and b==true and c==false,'changed response speech flags');speechCalls=speechCalls+1;return speak end
 function me:IsPerformingScriptTask()polls=polls+1;return polls<=tasks end
 debug.sethook(function()error('instruction budget')end,'',200000)
 Main(q,me);debug.sethook()
 assert(active==nil and not registered and starts==ends,'movie/timer remains live')
 assert(starts==(limit>1 and 1 or 0),'movie route not exercised')
 if mode~='handle' then assert(sequences==0,'extra native sequence')end
 assert(speechCalls==((limit>1 and health) and 1 or 0),'response speech branch not exercised')
 return starts,ends,sequences,speechCalls
end
'''
NATIVE=[{'expression': 'EXPRESSION_PELVIC_THRUST', 'funny': True, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_VULGAR_ONCE', 'events': [{'site': '0x99ebf0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_VULGAR_ONCE'}]}, {'expression': 'EXPRESSION_COCK_A_DOODLE_DO', 'funny': True, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_COCK_ONCE', 'events': [{'site': '0x99ebf0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_COCK_ONCE'}]}, {'expression': 'EXPRESSION_CROTCH_GRAB', 'funny': True, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_CROTCH_ONCE', 'events': [{'site': '0x99ebf0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_CROTCH_ONCE'}]}, {'expression': 'EXPRESSION_KISS_MY_ASS', 'funny': True, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_KISSARSE_ONCE', 'events': [{'site': '0x99ebf0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_KISSARSE_ONCE'}]}, {'expression': 'EXPRESSION_FLAMENCO', 'funny': True, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE', 'events': [{'site': '0x99ebf0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE'}]}, {'expression': 'EXPRESSION_COSSACK', 'funny': True, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE', 'events': [{'site': '0x99ebf0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE'}]}, {'expression': 'EXPRESSION_AIR_GUITAR', 'funny': True, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE', 'events': [{'site': '0x99ebf0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE'}]}, {'expression': 'EXPRESSION_BALLET', 'funny': True, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE', 'events': [{'site': '0x99ebf0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE'}]}, {'expression': 'EXPRESSION_SATURDAY_NIGHT_FEVER', 'funny': True, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE', 'events': [{'site': '0x99ebf0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE'}]}, {'expression': 'EXPRESSION_TAP', 'funny': True, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE', 'events': [{'site': '0x99ebf0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE'}]}, {'expression': 'EXPRESSION_GIGGLE', 'funny': True, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_GIGGLE_ONCE', 'events': [{'site': '0x99ebf0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_GIGGLE_ONCE'}]}, {'expression': 'EXPRESSION_SHIT', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_RUDE', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_RUDE'}]}, {'expression': 'EXPRESSION_FLIRT', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_FLIRT', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_FLIRT'}]}, {'expression': 'EXPRESSION_HEROIC_STANCE', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_HEROPOSE', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_HEROPOSE'}]}, {'expression': 'EXPRESSION_VICTORY_PUMP', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_ARMPUMP', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_ARMPUMP'}]}, {'expression': '', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'UNKNOWN', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_GIGGLE_X', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'expression_giggle', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_GIGGL', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_TAP ', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_FLAMENCO_X', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_PELVIC_THRUS', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_PELVIC_THRUSTX', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_COCK_A_DOODLE_D', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_COCK_A_DOODLE_DOX', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_CROTCH_GRA', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_CROTCH_GRABX', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_KISS_MY_AS', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_KISS_MY_ASSX', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_FLAMENC', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_FLAMENCOX', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_COSSAC', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_COSSACKX', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_AIR_GUITA', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_AIR_GUITARX', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_BALLE', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_BALLETX', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_SATURDAY_NIGHT_FEVE', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_SATURDAY_NIGHT_FEVERX', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_TA', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_TAPX', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_GIGGL', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_GIGGLEX', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}]
CASES=[(r['expression'],r['text'],health,speak,tasks,limit) for r in NATIVE for health,speak,tasks,limit in itertools.product((False,True),(False,True),(1,3),(1,2,5))]

@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('expression,key,health,speak,tasks,limit',CASES)
def test_response(generated,style,expression,key,health,speak,tasks,limit):
 source=generated[style]
 lua=LuaRuntime();lua.execute(source);lua.execute(H)(expression,health,speak,tasks,limit,'all',key)
@pytest.fixture(scope='module')
def review_input(tmp_path_factory):
 from tools.script_recovery import native_reviewed_stack_aliases as adapter
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json')
 unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text())
 row=unit['reviewedStackScalarAliases']['0x00ecf7f0']
 unit['entities']={'WomanToAttract':unit['entities']['WomanToAttract']};unit['quest']['functions']={};unit['quest']['helpers']={}
 original=adapter.restore;captured={};materialize=adapter.materialize_string_values
 def capture_values(source,review):
  if review and review.get('stringValueAliases'):captured['lowered']=source
  return materialize(source,review)
 def capture(source,fn,row,rd):
  if str(fn['address']).lower()=='0x00ecf7f0':captured.update(source=source,fn=fn,rd=rd)
  return original(source,fn,row,rd)
 with patch.object(c,'constructor_defaults',return_value={}),patch.object(adapter,'restore',capture),patch.object(adapter,'materialize_string_values',capture_values):c.convert(unit,tmp_path_factory.mktemp('woman_timer_review'))
 return captured,row

@pytest.mark.parametrize('index',range(6))
@pytest.mark.parametrize('fault',('missing','duplicate','native','instruction'))
def test_response_review_guards(review_input,index,fault):
 import copy
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 captured,row=review_input;row=copy.deepcopy(row);fn=copy.deepcopy(captured['fn']);text=captured['source'];rd=captured['rd']
 needle=('(CCharString *)&xStack_c4,"TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY",-1', 'CCharString::operator=((CCharString *)&xStack_c4,pcVar18)', 'CCharString::operator_char_const_((CCharString *)&xStack_c4)', '(CCharString *)&xStack_c8,"TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY",-1', 'CCharString::operator=((CCharString *)&xStack_c8,pcVar18)', 'CCharString::operator_char_const_((CCharString *)&xStack_c8)')[index];assert sum(old==needle for old,new in row['edits'])==1;assert text.count(needle)==1
 if fault=='missing':text=text.replace(needle,'MISSING_RESPONSE_OPERAND')
 elif fault=='duplicate':text+='\n'+needle
 elif fault=='native':row['nativeSha256']='0'*64
 else:
  site=(0xecff9e,0xed00db,0xed0184,0xecfd25,0xed02fa,0xed0610)[index]
  key=next(k for k in row['nativeInstructions'] if int(k,16)==site);row['nativeInstructions'][key]='90'
 with pytest.raises(ValueError,match='reviewed stack'):restore(text,fn,row,rd)

@pytest.mark.parametrize('fault',('list_type','list_none','list_bool','entry_type','alias','duplicate','ctor_absent','assign_absent','ctor_zero','assign_negative','ctor_bool','assign_type','ctor_count','assign_count'))
def test_explicit_string_value_guards(review_input,fault):
 import copy
 from tools.script_recovery.native_reviewed_stack_aliases import materialize_string_values as restore
 captured,row=review_input;row=copy.deepcopy(row);value=row['stringValueAliases'][0]
 if fault=='list_type':row['stringValueAliases']='not a list'
 elif fault=='list_none':row['stringValueAliases']=None
 elif fault=='list_bool':row['stringValueAliases']=False
 elif fault=='entry_type':row['stringValueAliases']=[None]
 elif fault=='alias':value['alias']='unreviewedString'
 elif fault=='duplicate':row['stringValueAliases'].append(dict(value))
 elif fault=='ctor_absent':value.pop('constructorCount')
 elif fault=='assign_absent':value.pop('assignmentCount')
 elif fault=='ctor_zero':value['constructorCount']=0
 elif fault=='assign_negative':value['assignmentCount']=-1
 elif fault=='ctor_bool':value['constructorCount']=True
 elif fault=='assign_type':value['assignmentCount']='1'
 elif fault=='ctor_count':value['constructorCount']=2
 elif fault=='assign_count':value['assignmentCount']=2
 with pytest.raises(ValueError,match='reviewed stack alias'):restore(captured['lowered'],row)

def test_missing_string_value_opt_in_preserves_constructor(review_input):
 import copy
 from tools.script_recovery.native_reviewed_stack_aliases import materialize_string_values as restore
 captured,row=review_input;row=copy.deepcopy(row);row.pop('stringValueAliases')
 out=restore(captured['lowered'],row)
 assert out==captured['lowered']
 assert re.search(r'CCharString::operator=\([^;]*response_stk_c4[^;]*pcVar18\)',out)

def test_native_response_selection():
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 import unicorn as u
 from unicorn import x86_const as x
 rd=RData();assert callee_stack_words(0x99ebf0)==2 and callee_stack_words(0x99efe0)==1
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xecf000,0x2000),(0x411000,0x1000),(0x99e000,0x1000),(0xf35000,0x1000),(0x300000,0x10000),(0x500000,0x4000)):cpu.mem_map(a,n)
 cpu.mem_write(0xecf7f0,rd.bytes_at(0xecf7f0,4885));cpu.mem_write(0x411570,rd.bytes_at(0x411570,40))
 for name,va,n in rd.names:
  if name=='.rdata':addr=rd.base+va;cpu.mem_map(addr,(n+4095)&~4095);cpu.mem_write(addr,rd.bytes_at(addr,n))
 for a in (0x99ebf0,0x99efe0,0xf35b30):cpu.mem_write(a,b'\xc3')
 sp,owner,rep,buf=0x300800,0x500000,0x501000,0x502000
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 state={}
 def hook(machine,a,size,user):
  if a in (0xed00e0,0xed02ff):state['stop']=a;machine.emu_stop()
  elif a in (0x99ebf0,0x99efe0,0xf35b30):
   stack=machine.reg_read(x.UC_X86_REG_ESP);receiver=machine.reg_read(x.UC_X86_REG_ECX)
   if a==0xf35b30:assert receiver==owner;purge=0;value=0
   else:
    address=word(stack+4);text=rd.string_at(address);assert text and text.startswith('TEXT_QST_B10_LAUGHING_WOMAN_')
    slot=receiver-sp;assert slot in (0x20,0x1c)
    state['response'][slot]=text;state['events'].append(dict(site=hex(a),slot=slot,text=text));purge=8 if a==0x99ebf0 else 4;value=receiver
    if a==0x99ebf0:assert word(stack+8)==0xffffffff
   machine.reg_write(x.UC_X86_REG_EAX,value);machine.reg_write(x.UC_X86_REG_EIP,word(stack));machine.reg_write(x.UC_X86_REG_ESP,stack+4+purge)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 source={'cases':[{'name': 'EXPRESSION_PELVIC_THRUST', 'funny': 15531268}, {'name': 'EXPRESSION_COCK_A_DOODLE_DO', 'funny': 15531268}, {'name': 'EXPRESSION_CROTCH_GRAB', 'funny': 15531268}, {'name': 'EXPRESSION_KISS_MY_ASS', 'funny': 15531268}, {'name': 'EXPRESSION_FLAMENCO', 'funny': 15531268}, {'name': 'EXPRESSION_COSSACK', 'funny': 15531268}, {'name': 'EXPRESSION_AIR_GUITAR', 'funny': 15531268}, {'name': 'EXPRESSION_BALLET', 'funny': 15531268}, {'name': 'EXPRESSION_SATURDAY_NIGHT_FEVER', 'funny': 15531268}, {'name': 'EXPRESSION_TAP', 'funny': 15531268}, {'name': 'EXPRESSION_GIGGLE', 'funny': 15531268}, {'name': 'EXPRESSION_SHIT', 'funny': 0}, {'name': 'EXPRESSION_FLIRT', 'funny': 0}, {'name': 'EXPRESSION_HEROIC_STANCE', 'funny': 0}, {'name': 'EXPRESSION_VICTORY_PUMP', 'funny': 0}, {'name': '', 'funny': 0}, {'name': 'UNKNOWN', 'funny': 0}, {'name': 'EXPRESSION_GIGGLE_X', 'funny': 0}, {'name': 'expression_giggle', 'funny': 0}, {'name': 'EXPRESSION_GIGGL', 'funny': 0}, {'name': 'EXPRESSION_TAP ', 'funny': 0}, {'name': 'EXPRESSION_FLAMENCO_X', 'funny': 0}, {'name': 'EXPRESSION_PELVIC_THRUS', 'funny': 0}, {'name': 'EXPRESSION_PELVIC_THRUSTX', 'funny': 0}, {'name': 'EXPRESSION_COCK_A_DOODLE_D', 'funny': 0}, {'name': 'EXPRESSION_COCK_A_DOODLE_DOX', 'funny': 0}, {'name': 'EXPRESSION_CROTCH_GRA', 'funny': 0}, {'name': 'EXPRESSION_CROTCH_GRABX', 'funny': 0}, {'name': 'EXPRESSION_KISS_MY_AS', 'funny': 0}, {'name': 'EXPRESSION_KISS_MY_ASSX', 'funny': 0}, {'name': 'EXPRESSION_FLAMENC', 'funny': 0}, {'name': 'EXPRESSION_FLAMENCOX', 'funny': 0}, {'name': 'EXPRESSION_COSSAC', 'funny': 0}, {'name': 'EXPRESSION_COSSACKX', 'funny': 0}, {'name': 'EXPRESSION_AIR_GUITA', 'funny': 0}, {'name': 'EXPRESSION_AIR_GUITARX', 'funny': 0}, {'name': 'EXPRESSION_BALLE', 'funny': 0}, {'name': 'EXPRESSION_BALLETX', 'funny': 0}, {'name': 'EXPRESSION_SATURDAY_NIGHT_FEVE', 'funny': 0}, {'name': 'EXPRESSION_SATURDAY_NIGHT_FEVERX', 'funny': 0}, {'name': 'EXPRESSION_TA', 'funny': 0}, {'name': 'EXPRESSION_TAPX', 'funny': 0}, {'name': 'EXPRESSION_GIGGL', 'funny': 0}, {'name': 'EXPRESSION_GIGGLEX', 'funny': 0}]};rows=[]
 for case in source['cases']:
  name=case['name'];state=dict(stop=None,response={},events=[])
  cpu.mem_write(buf,name.encode()+b'\0');put(rep,buf);put(sp+0x18,rep);put(owner+0x20,0)
  cpu.reg_write(x.UC_X86_REG_EBP,owner);cpu.reg_write(x.UC_X86_REG_ESP,sp);cpu.reg_write(x.UC_X86_REG_EFLAGS,2)
  cpu.emu_start(0xecfbf9,0xed0b05,count=10000)
  funny=case['funny']==0xecfd04;assert state['stop']==(0xed02ff if funny else 0xed00e0)
  assert word(owner+0x20)==int(funny) and cpu.reg_read(x.UC_X86_REG_ESP)==sp
  key=state['response'][0x1c if funny else 0x20];rows.append(dict(expression=name,funny=funny,text=key,events=state['events']))
 
 assert rows==[{'expression': 'EXPRESSION_PELVIC_THRUST', 'funny': True, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_VULGAR_ONCE', 'events': [{'site': '0x99ebf0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_VULGAR_ONCE'}]}, {'expression': 'EXPRESSION_COCK_A_DOODLE_DO', 'funny': True, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_COCK_ONCE', 'events': [{'site': '0x99ebf0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_COCK_ONCE'}]}, {'expression': 'EXPRESSION_CROTCH_GRAB', 'funny': True, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_CROTCH_ONCE', 'events': [{'site': '0x99ebf0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_CROTCH_ONCE'}]}, {'expression': 'EXPRESSION_KISS_MY_ASS', 'funny': True, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_KISSARSE_ONCE', 'events': [{'site': '0x99ebf0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_KISSARSE_ONCE'}]}, {'expression': 'EXPRESSION_FLAMENCO', 'funny': True, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE', 'events': [{'site': '0x99ebf0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE'}]}, {'expression': 'EXPRESSION_COSSACK', 'funny': True, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE', 'events': [{'site': '0x99ebf0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE'}]}, {'expression': 'EXPRESSION_AIR_GUITAR', 'funny': True, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE', 'events': [{'site': '0x99ebf0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE'}]}, {'expression': 'EXPRESSION_BALLET', 'funny': True, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE', 'events': [{'site': '0x99ebf0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE'}]}, {'expression': 'EXPRESSION_SATURDAY_NIGHT_FEVER', 'funny': True, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE', 'events': [{'site': '0x99ebf0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE'}]}, {'expression': 'EXPRESSION_TAP', 'funny': True, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE', 'events': [{'site': '0x99ebf0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE'}]}, {'expression': 'EXPRESSION_GIGGLE', 'funny': True, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_GIGGLE_ONCE', 'events': [{'site': '0x99ebf0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 28, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_GIGGLE_ONCE'}]}, {'expression': 'EXPRESSION_SHIT', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_RUDE', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_RUDE'}]}, {'expression': 'EXPRESSION_FLIRT', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_FLIRT', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_FLIRT'}]}, {'expression': 'EXPRESSION_HEROIC_STANCE', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_HEROPOSE', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_HEROPOSE'}]}, {'expression': 'EXPRESSION_VICTORY_PUMP', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_ARMPUMP', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}, {'site': '0x99efe0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_ARMPUMP'}]}, {'expression': '', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'UNKNOWN', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_GIGGLE_X', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'expression_giggle', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_GIGGL', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_TAP ', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_FLAMENCO_X', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_PELVIC_THRUS', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_PELVIC_THRUSTX', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_COCK_A_DOODLE_D', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_COCK_A_DOODLE_DOX', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_CROTCH_GRA', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_CROTCH_GRABX', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_KISS_MY_AS', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_KISS_MY_ASSX', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_FLAMENC', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_FLAMENCOX', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_COSSAC', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_COSSACKX', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_AIR_GUITA', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_AIR_GUITARX', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_BALLE', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_BALLETX', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_SATURDAY_NIGHT_FEVE', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_SATURDAY_NIGHT_FEVERX', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_TA', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_TAPX', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_GIGGL', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}, {'expression': 'EXPRESSION_GIGGLEX', 'funny': False, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY', 'events': [{'site': '0x99ebf0', 'slot': 32, 'text': 'TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY'}]}]

