"""Named Thing references must agree with native hidden outputs and fastcall slots."""
from copy import deepcopy
import hashlib,itertools,json,re
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_thing_results import MOVEMENT,DISTANCE,restore
R=Path(__file__).resolve().parents[2]
@pytest.fixture(scope='module')
def evidence():
 c=UnitConverter(R/'refs/script_recovery/bordello/translation_unit_typed.json');fn=c.native('0x00e44ea0')
 row=json.loads((R/'refs/script_recovery/bordello/units/V_Bordello.json').read_text())['reviewedThingResults']['0x00e44ea0']
 source=(R/'tools/script_recovery/fixtures/thing_results/client_input.lua').read_text()
 return source,fn,row,c.rdata
def test_exact_scoped_recovery_and_native_identities(evidence):
 source,fn,row,rd=evidence;out,proof=restore(source,fn,row,rd,entity=True);edits=[]
 for m,local in zip(MOVEMENT.finditer(source),['r1','r2','r1']):edits.append((m.start(),m.end(),m[0].replace(m['resource']+' ~= nil',local+' ~= nil',1).replace('resources:ScriptThing('+m['resource']+')',local)))
 for m,args in zip(DISTANCE.finditer(source),['r1, me, 5.0','r2, me, 5.0','me, r1, 5.0']):edits.append((m.start(),m.end(),'quest:IsDistanceBetweenThingsUnder('+args+')'))
 expected=source
 for a,b,text in sorted(edits,reverse=True):expected=expected[:a]+text+expected[b:]
 assert out==expected and 'if cVar5 == 0 then' in out
 assert [p['frameOffset'] for p in proof[0]['lookups']]==[40,52]
 assert [p['local'] for p in proof[0]['movements']]==['r1','r2','r1']
 assert [p['changed'] for p in proof[0]['distances']]==[True,False,True]
 for name in ['TryAcquire','ReleaseResource','PrepareResource','DeregisterTimer','DestroyMovie']:assert source.count(':'+name+'(')==out.count(':'+name+'(')
 assert restore(source,fn,None,rd)==(source,[])
@pytest.mark.parametrize('fault',['nativeSha256','sourceSha256','luaBodySha256','actorLocal','actorOffset','scriptBind','lookupTarget','lookupBytes','lookupName','lookupLocal','lookupSetup','nameSetup','distanceTarget','distanceBytes','distanceBefore','distanceSetup','moveTarget','moveBytes','movementData','movementArguments','witness','lookupOrder','distanceOrder','movementOrder'])
def test_changed_review_fails_closed(evidence,fault):
 source,fn,row,rd=evidence;row=deepcopy(row);fn=deepcopy(fn)
 if fault.endswith('Sha256'):row[fault]='0'*64
 elif fault=='actorLocal':row[fault]='other'
 elif fault=='actorOffset':row[fault]=12
 elif fault=='scriptBind':row[fault]='0xe44f7b'
 elif fault=='lookupTarget':row['lookup']['target']='0x8a7d61'
 elif fault=='lookupBytes':row['lookup']['bytes']='00'*256
 elif fault=='lookupName':row['bindings'][0]['name']='Missing'
 elif fault=='lookupLocal':row['bindings'][0]['local']='r2'
 elif fault=='lookupSetup':row['bindings'][0]['setup'].pop()
 elif fault=='nameSetup':row['bindings'][0]['nameSetup'].pop()
 elif fault=='distanceTarget':row[fault]='0xcbe300'
 elif fault=='distanceBytes':row[fault]=''
 elif fault=='distanceBefore':row['distances'][0]['luaBefore']='changed'
 elif fault=='distanceSetup':row['distances'][0]['setup'].pop()
 elif fault=='moveTarget':row[fault]='0x7e72f1'
 elif fault=='moveBytes':row[fault]='00'*15
 elif fault=='movementData':row['movements'][0]['dataLoad']='0xe4659f'
 elif fault=='movementArguments':row['movements'][0]['arguments'].pop()
 elif fault=='witness':row['nativeInstructions'].pop('0xe45724')
 elif fault.endswith('Order'):
  sites={'lookupOrder':['0xe44f8f','0xe44fbd'],'distanceOrder':['0xe4575b','0xe4671b'],'movementOrder':['0xe45748','0xe4670a']}[fault]
  a,b=[next(n for n,s in enumerate(fn['callOrder']) if int(s,16)==int(site,16)) for site in sites];fn['callOrder'][a],fn['callOrder'][b]=fn['callOrder'][b],fn['callOrder'][a]
 with pytest.raises(ValueError,match='reviewed Thing results'):restore(source,fn,row,rd,entity=True)
def test_entity_context_required(evidence):
 source,fn,row,rd=evidence
 with pytest.raises(ValueError,match='actor context'):restore(source,fn,row,rd)
def test_rebound_result_cannot_be_used_as_marker(evidence):
 source,fn,row,rd=evidence;row=deepcopy(row);source=source.replace('    r1 = quest:GetThingWithScriptName("M_ClientStand")','    r1 = quest:GetThingWithScriptName("M_ClientStand")\n    r1 = me',1);row['luaBodySha256']=hashlib.sha256(source.encode()).hexdigest()
 with pytest.raises(ValueError,match='rebound or missing'):restore(source,fn,row,rd,entity=True)
def test_extra_movement_block_is_not_silently_rewritten(evidence):
 source,fn,row,rd=evidence;row=deepcopy(row);source+='\n'+next(MOVEMENT.finditer(source))[0];row['luaBodySha256']=hashlib.sha256(source.encode()).hexdigest()
 with pytest.raises(ValueError,match='movement source inventory'):restore(source,fn,row,rd,entity=True)
@pytest.mark.parametrize('site,encoded,message',[(0xe44f8a,'8d442438','overlapping result'),(0xe44f77,'8d4c2448','name or slot'),(0xe45757,'8d4c243c','unresolved Thing'),(0xe450a6,'8d4d0c','unclassified native'),(0xe45750,'6800000000','radius'),(0xe45724,'8b4c241c','does not belong'),(0xe45735,'ff521c','GetPos dispatch'),(0xe4573e,'6800000040','argument values'),(0xe45744,'8d4c242c','control receiver'),(0xe4572c,'b8e4e84301','zero vector')])
def test_decoded_native_operands_checked_after_rehash(evidence,site,encoded,message):
 source,fn,row,rd=evidence;row=deepcopy(row);start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16);raw=bytearray(rd.bytes_at(start,end-start));replacement=bytes.fromhex(encoded)
 raw[site-start:site-start+len(replacement)]=replacement;row['nativeSha256']=hashlib.sha256(raw).hexdigest();row['nativeInstructions'][hex(site)]=encoded
 class Changed:
  def bytes_at(self,a,n):return bytes(raw) if (a,n)==(start,end-start) else rd.bytes_at(a,n)
  def string_at(self,a):return rd.string_at(a)
 with pytest.raises(ValueError,match=message):restore(source,fn,row,Changed(),entity=True)
def test_actual_lua_distance_calls_receive_named_things(evidence):
 source,fn,row,rd=evidence;out,_=restore(source,fn,row,rd,entity=True);calls=[m[0] for m in DISTANCE.finditer(out)];lua=LuaRuntime();count=0
 prefix='''local me={id="actor",x=0,alive=actorAlive};local r1={id="stand",x=x,alive=standAlive};local r2={id="spawn",x=1,alive=spawnAlive};local trace;local quest={IsDistanceBetweenThingsUnder=function(_,a,b,r)assert(type(a)=="table" and type(b)=="table");trace=a.id..":"..b.id;if not a.alive or not b.alive then return false end;return (a.x-b.x)^2<r*r end};'''
 functions=[lua.eval('function(actorAlive,standAlive,spawnAlive,x)'+prefix+'local result='+call+';return result,trace end') for call in calls]
 for aa,sa,pa,x in itertools.product([False,True],[False,True],[False,True],[0,3,5,6]):
  expected=[aa and sa and x<5,aa and pa,aa and sa and x<5]
  for function,value,pair in zip(functions,expected,['stand:actor','spawn:actor','actor:stand']):actual,trace=function(aa,sa,pa,x);assert bool(actual)==value and trace==pair;count+=1
 assert count==96
def test_actual_lua_movement_fragments_use_marker_positions(evidence):
 source,fn,row,rd=evidence;out,_=restore(source,fn,row,rd,entity=True)
 pattern=re.compile(r'(?m)^(?P<ind>[ \t]*)if not \((?P<thing>r[12]) ~= nil and not (?P=thing):IsNull\(\)\) then\n(?P=ind)    (?P<position>\w+) = \{x = 0, y = 0, z = 0\}\n(?P=ind)else\n(?P=ind)    (?P=position) = (?P=thing):GetPos\(\)\n(?P=ind)end\n(?P=ind)me:MoveToPosition\((?P=position), 1.0, 0, false, true\)')
 blocks=list(pattern.finditer(out));assert [m['thing'] for m in blocks]==['r1','r2','r1'];lua=LuaRuntime();count=0
 prefix='''local gets=0;local moved;local function marker(pos)return {IsNull=function()return not live end,GetPos=function()gets=gets+1;return pos end}end;local r1=marker({x=x,y=0,z=0});local r2=marker({x=-x,y=2,z=1});local me={MoveToPosition=function(_,p,s,m,a,b)assert(s==1 and m==0 and a==false and b==true);if control then moved=p end end};local puVar8;'''
 functions=[lua.eval('function(live,control,x)'+prefix+m[0]+';return gets,moved end') for m in blocks]
 for live,control,x in itertools.product([False,True],[False,True],[0,3,5,6]):
  for n,function in enumerate(functions):
   gets,moved=function(live,control,x);assert gets==int(live)
   if control:assert (moved['x'],moved['y'],moved['z'])==(((-x,2,1) if n==1 else (x,0,0)) if live else (0,0,0))
   else:assert moved is None
   count+=1
 assert count==48
