from pathlib import Path
import copy,json,struct
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import definition_arena_fixture as a
from tools.script_recovery.definition_vector_fixture import stock_fixture,to_lua,validate
from tools.script_recovery.script_def_offsets import load
from tools.script_recovery.smoke_run_unit import HARNESS
from tools.decode_game_bin_field import _read_value

ROOT=Path(__file__).resolve().parents[2]

@pytest.fixture(scope='module')
def stock():return stock_fixture(include_arena_rounds=True),load()

def execute(body,stock,source=None):
 fixture,metadata=stock;lua=LuaRuntime(unpack_returned_tuples=True)
 runner=lua.execute('return function(SOURCE,NAME,FUNCTIONS,FRAME_BUDGET,INSTRUCTION_LIMIT,PARAM_KINDS,ENTITY_FILE,DEFINITION_FIXTURE) '+HARNESS+' end')
 text=(source or '')+'\nfunction Probe(q)\n'+body+'\nend'
 return runner(text,'arena_fixture.lua',lua.table('Probe'),30,10000,lua.table(),False,to_lua(lua,fixture,metadata))['Probe']

def encode(rows,shape='CArenaRoundDef'):
 out=struct.pack('<I',len(rows))
 for row in rows:
  for name,kind in a.SHAPES[shape]:
   out+=struct.pack('<I',a.field_tag(name));value=row[name]
   if kind=='int':out+=struct.pack('<i',value)
   elif kind=='bool':out+=bytes([int(value)])
   elif kind=='string':out+=value.encode('utf-8')+b'\0'
   else:out+=encode(value,kind)
 return out

def test_stock_counts_raw_roundtrip_and_independent_schema(stock):
 fixture,metadata=stock;group=fixture['arenaRounds'][0];rows=group['rounds'];raw=bytes.fromhex(metadata['0x1044']['value'])
 assert fixture['schema']=='script-definition-vector-fixture/4' and group['offset']==0x1044
 assert len(rows)==8 and sum(len(r['Waves']) for r in rows)==23
 assert sum(len(w['Creatures']) for r in rows for w in r['Waves'])==37
 assert encode(rows)==raw and a.decode_rounds(raw)==rows
 schema=json.loads((ROOT/'ghidra_out/def_schema.json').read_text(encoding='utf-8'))
 for shape in a.SHAPES:
  for field in schema[shape]['fields']:
   if field['type'].startswith('vector<'):field['type']='Vector_'+field['type'][7:-1]
 assert _read_value(raw,0,'Vector_CArenaRoundDef',schema)==len(raw)

def test_all3351_stock_prefix_truncations_rejected(stock):
 raw=bytes.fromhex(stock[1]['0x1044']['value']);assert len(raw)==3351
 for end in range(len(raw)):
  with pytest.raises(ValueError):a.decode_rounds(raw[:end])

@pytest.mark.parametrize('fault',['tag','trailing','count','utf8','terminator','bool'])
def test_serialized_invalid_values_rejected(stock,fault):
 raw=bytes.fromhex(stock[1]['0x1044']['value'])
 if fault=='tag':raw=raw[:4]+bytes([raw[4]^1])+raw[5:]
 if fault=='trailing':raw+=b'X'
 if fault=='count':raw=struct.pack('<I',4097)+raw[4:]
 if fault=='utf8':raw=raw.replace(b'CREATURE_HORNET_01\0',b'\xff\0',1)
 if fault=='terminator':raw=raw[:raw.index(b'CREATURE_HORNET_01')+17]
 if fault=='bool':
  marker=struct.pack('<I',a.field_tag('ShortWave'));pos=raw.index(marker)+4;raw=raw[:pos]+b'\x02'+raw[pos+1:]
 with pytest.raises(ValueError):a.decode_rounds(raw)

@pytest.mark.parametrize('fault',['duplicate','empty_groups','offset','donor','unanchored','wrong_name','wrong_type','unverified','bool_count','float_count','large_count','missing','extra','bool_byte','nonstring','nul','overlap'])
def test_native_shape_and_fields_are_required(stock,fault):
 fixture,metadata=copy.deepcopy(stock);groups=fixture['arenaRounds'];group=groups[0];row=group['rounds'][0];wave=row['Waves'][0];creature=wave['Creatures'][0]
 if fault=='duplicate':groups.append(copy.deepcopy(group))
 if fault=='empty_groups':groups.clear()
 if fault=='offset':group['offset']+=4
 if fault=='donor':group['offset']=0x1104
 if fault=='unanchored':metadata['0x1044'].pop('retailTransferCall')
 if fault=='wrong_name':group['name']='Invented'
 if fault=='wrong_type':metadata['0x1044']['defType']='vector<CConversation>'
 if fault=='unverified':metadata['0x1044']['verified']=False
 if fault=='bool_count':row['NumWaves']=True
 if fault=='float_count':creature['NumCreatures']=1.5
 if fault=='large_count':creature['DeathScore']=2147483648
 if fault=='missing':wave.pop('Creatures')
 if fault=='extra':creature['Invented']=0
 if fault=='bool_byte':wave['ShortWave']=1
 if fault=='nonstring':creature['CreatureType']=1
 if fault=='nul':creature['HUDType']='X\0Y'
 if fault=='overlap':metadata['0x1048']={'name':'Collision','defType':'int32'}
 with pytest.raises(ValueError):validate(fixture,metadata)

def test_all234_flattened_stock_fields_match_sdk_shapes(stock):
 flat=a.flatten(stock[0]['arenaRounds']);assert len(flat)==234
 body='assert(q:GetStateInt("Rounds_Count")==0)\nq:InitialiseArenaRounds()\n'
 for key,value in flat.items():
  method='Bool' if type(value) is bool else 'Int' if type(value) is int else 'String'
  literal='true' if value is True else 'false' if value is False else json.dumps(value)
  body+='assert(q:GetState'+method+'('+json.dumps(key)+')=='+literal+')\n'
 body+='assert(q:ReadGlobalGameData(0x1048)-q:ReadGlobalGameData(0x1044)==8*0x38)\nassert(q:ReadGlobalGameData(0x104c)==q:ReadGlobalGameData(0x1048))\nassert(q:ReadGlobalGameData(0x4ec)==1)'
 result=execute(body,stock);assert result['ok'],result['error']

def test_typed_state_overwrite_snapshot_reset_and_older_keys_retained(stock):
 result=execute('''q:SetStateBool('Rounds_Probe',true)
 assert(q:GetStateBool('Rounds_Probe') and q:GetStateInt('Rounds_Probe')==0 and q:GetStateString('Rounds_Probe')=='')
 q:SetStateInt('Rounds_Probe',-7);assert(q:GetStateInt('Rounds_Probe')==-7 and not q:GetStateBool('Rounds_Probe'))
 q:SetStateString('Rounds_Probe','X');assert(q:GetStateString('Rounds_Probe')=='X' and q:GetStateInt('Rounds_Probe')==0)
 q:SetStateInt('Rounds_Count',99);q:InitialiseArenaRounds()
 assert(q:GetStateInt('Rounds_Count')==8 and q:GetStateString('Rounds_Probe')=='X')
 q:SetStateInt('Unrelated',99);q:SetStateBool('UnrelatedBool',true)
 assert(q:GetStateInt('Unrelated')==0 and q:GetStateBool('UnrelatedBool')==false)
 ''',stock)
 assert result['ok'],result['error']

def test_empty_round_vector_stays_empty(stock):
 fixture,metadata=copy.deepcopy(stock);fixture['arenaRounds'][0]['rounds']=[]
 result=execute("q:InitialiseArenaRounds();assert(q:GetStateInt('Rounds_Count')==0);assert(q:ReadGlobalGameData(0x1044)==q:ReadGlobalGameData(0x1048))",(fixture,metadata))
 assert result['ok'],result['error']

def test_declared_counts_and_signed_scalars_not_replaced_by_lengths(stock):
 fixture,metadata=copy.deepcopy(stock);r=fixture['arenaRounds'][0]['rounds'][0];r['NumWaves']=-1;r['Waves'][0]['NumWaveCreatures']=99;r['Waves'][0]['Creatures'][0]['NumCreatures']=-2147483648
 result=execute("q:InitialiseArenaRounds();assert(q:GetStateInt('Rounds_0_NumWaves')==-1 and q:GetStateInt('Rounds_0_Waves_Count')==4);assert(q:GetStateInt('Rounds_0_Waves_0_NumWaveCreatures')==99 and q:GetStateInt('Rounds_0_Waves_0_Creatures_Count')==1);assert(q:GetStateInt('Rounds_0_Waves_0_Creatures_0_NumCreatures')==-2147483648)",(fixture,metadata))
 assert result['ok'],result['error']

@pytest.mark.parametrize('call',["SetStateInt('Rounds_Probe',2147483648)","SetStateInt('Rounds_Probe',0.5)","SetStateBool('Rounds_Probe',1)","SetStateString('Rounds_Probe',false)"])
def test_state_writes_outside_bounded_fixture_domain_error(stock,call):
 result=execute('q:'+call,stock);assert not result['ok'] and 'Arena fixture:' in result['error']

def test_snapshot_store_is_per_mock_quest(stock):
 first=execute("q:InitialiseArenaRounds();assert(q:GetStateInt('Rounds_Count')==8)",stock)
 second=execute("assert(q:GetStateInt('Rounds_Count')==0)",stock)
 assert first['ok'] and second['ok']

def test_actual_generated_arena_initializer_uses_snapshot(stock):
 source=(ROOT/'refs/script_recovery/lifted/Arena/readable/FSE/Arena/Arena.lua').read_text(encoding='utf-8')
 result=execute("InitialiseVariables(q);assert(q:GetStateInt('Rounds_Count')==8);assert(q:GetStateString('Rounds_0_Waves_0_Creatures_0_CreatureType')=='CREATURE_HORNET_01')",stock,source)
 assert result['ok'],result['error'];assert result['calls']==23

@pytest.mark.parametrize('fault',['schema3','missing_arena','missing_inline','missing_conversations'])
def test_explicit_schema4_required(stock,fault):
 fixture,metadata=copy.deepcopy(stock)
 if fault=='schema3':fixture['schema']='script-definition-vector-fixture/3'
 if fault=='missing_arena':fixture.pop('arenaRounds')
 if fault=='missing_inline':fixture.pop('inlineStrings')
 if fault=='missing_conversations':fixture.pop('conversations')
 with pytest.raises(ValueError):validate(fixture,metadata)

def test_existing_schema3_keeps_ordinary_mock_policy():
 fixture=stock_fixture(include_values=True);metadata=load()
 result=execute("q:InitialiseArenaRounds();q:SetStateInt('Rounds_Count',8);assert(q:GetStateInt('Rounds_Count')==0)",(fixture,metadata))
 assert result['ok'],result['error']

def test_actual_playwave_reports_missing_empty_world_spawn_markers(stock):
 source=(ROOT/'refs/script_recovery/lifted/Arena/readable/FSE/Arena/Arena.lua').read_text(encoding='utf-8')
 result=execute('InitialiseVariables(q);PlayWave(q)',stock,source)
 assert not result['ok'] and 'n%0' in result['error']
