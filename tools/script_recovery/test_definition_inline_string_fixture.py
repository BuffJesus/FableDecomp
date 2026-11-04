import copy,json,struct
from pathlib import Path
import pytest
from tools.script_recovery import definition_inline_string_fixture as f
from tools.script_recovery.script_def_offsets import load
from tools.decode_game_bin_field import _read_value

@pytest.fixture(scope='module')
def stock():
 metadata=load();return f.decode_vectors(metadata),metadata

def test_all_stock_inline_vectors_and_byte_roundtrip(stock):
 groups,metadata=stock;assert len(groups)==14 and sum(len(g['items']) for g in groups)==190
 by_name={g['name']:g for g in groups}
 assert len(by_name['BookDefNames']['items'])==40
 assert by_name['BookDefNames']['items'][2]=='OBJECT_BOOK_STORY_13'
 assert by_name['BookBoyMarker']['items'][:3]==['NULL','NULL','MK_BSAS_BOY']
 assert by_name['BookGirlMarker']['items'][2]=='MK_BSAS_GIRL'
 for group in groups:
  raw=bytes.fromhex(metadata[hex(group['offset'])]['value'])
  rebuilt=struct.pack('<I',len(group['items']))+b''.join(s.encode('utf-8')+b'\0' for s in group['items'])
  assert rebuilt==raw
  assert _read_value(raw,0,'Vector_VCCharString__',{})==len(raw)

def test_all_stock_truncations_are_rejected(stock):
 count=0
 for group in stock[0]:
  raw=bytes.fromhex(stock[1][hex(group['offset'])]['value'])
  for end in range(len(raw)):
   with pytest.raises(ValueError):f.decode_items(raw[:end])
   count+=1
 assert count==3271

@pytest.mark.parametrize('raw',[b'',b'\0'*3,b'\xff'*4,struct.pack('<I',4097),struct.pack('<I',1)+b'NO_NUL',struct.pack('<I',1)+b'\xff\0',struct.pack('<I',0)+b'EXTRA',struct.pack('<I',1)+b'X'*16385+b'\0'])
def test_invalid_serialized_shapes_are_rejected(raw):
 with pytest.raises(ValueError):f.decode_items(raw)

def test_empty_and_valid_utf8_are_preserved():
 assert f.decode_items(bytes(4))==[]
 raw=struct.pack('<I',3)+b'\0NULL\0'+chr(0x96ea).encode('utf-8')+b'\0'
 assert f.decode_items(raw)==['','NULL',chr(0x96ea)]

@pytest.mark.parametrize('fault',['duplicate','wrong_name','definition_token_type','unaligned','bool_offset','unanchored','unverified','wrong_item','nul','overlap'])
def test_fixture_requires_native_inline_type_and_shape(stock,fault):
 groups,metadata=copy.deepcopy(stock);g=groups[0];field=metadata[hex(g['offset'])]
 if fault=='duplicate':groups.append(copy.deepcopy(g))
 if fault=='wrong_name':g['name']='Invented'
 if fault=='definition_token_type':field['defType']='Vector_VCDefString__'
 if fault=='unaligned':g['offset']+=1
 if fault=='bool_offset':g['offset']=True
 if fault=='unanchored':field.pop('retailTransferCall')
 if fault=='unverified':field['verified']=False
 if fault=='wrong_item':g['items'][0]=1
 if fault=='nul':g['items'][0]='A\0B'
 if fault=='overlap':metadata[hex(g['offset']+4)]={'name':'Collision','defType':'int32'}
 with pytest.raises(ValueError):f.validate(groups,metadata)


from lupa.lua54 import LuaRuntime
from tools.script_recovery.definition_vector_fixture import stock_fixture,to_lua,validate as validate_fixture
from tools.script_recovery.smoke_run_unit import HARNESS

@pytest.fixture(scope='module')
def full():
 return stock_fixture(include_values=True),load()

def execute(source,full):
 fixture,metadata=full;lua=LuaRuntime(unpack_returned_tuples=True)
 runner=lua.execute('return function(SOURCE,NAME,FUNCTIONS,FRAME_BUDGET,INSTRUCTION_LIMIT,PARAM_KINDS,ENTITY_FILE,DEFINITION_FIXTURE) '+HARNESS+' end')
 return runner(source,'inline_fixture.lua',lua.table('Main'),100,1000,lua.table(),False,to_lua(lua,fixture,metadata))['Main']

def test_full_fixture_is_explicit_and_preserves_existing_modes(full):
 fixture,metadata=full
 assert fixture['schema']=='script-definition-vector-fixture/3'
 assert len(fixture['vectors'])==12 and len(fixture['conversations'])==1 and len(fixture['inlineStrings'])==14
 assert stock_fixture()['schema']=='script-definition-vector-fixture/1'
 assert stock_fixture(include_conversations=True)['schema']=='script-definition-vector-fixture/2'
 validate_fixture(fixture,metadata)

@pytest.mark.parametrize('offset',[0x3c0,0x3cc,0x4b0,0x4bc,0x4d4,0x5ac,0x5b8,0x5c4,0x92c,0x938,0x944,0x950,0x9ac,0x106c])
def test_all_190_stock_strings_and_vector_words(full,offset):
 group=next(g for g in full[0]['inlineStrings'] if g['offset']==offset);items=group['items']
 source='function Main(q)\n'
 source+=f'assert(q:ReadGlobalGameData({offset+4})-q:ReadGlobalGameData({offset})=={len(items)*4})\n'
 source+=f'assert(q:ReadGlobalGameData({offset+8})==q:ReadGlobalGameData({offset+4}))\n'
 for i,value in enumerate(items):source+=f'assert(q:ReadGlobalGameDataStringAt({offset},{i})=='+json.dumps(value)+')\n'
 source+=f'assert(q:ReadGlobalGameDataStringAt({offset},-1)=="")\nassert(q:ReadGlobalGameDataStringAt({offset},{len(items)})=="")\n'
 source+='assert(q:ReadGlobalGameData(0x4ec)==1)\nend'
 result=execute(source,full);assert result['ok'],result['error']

@pytest.mark.parametrize('offset,index',[('-1','0'),('0x4d4','-1'),('0x4d4','40'),('0x4d4','41'),('0x4d8','-1')])
def test_ordinary_invalid_inline_indices_follow_sdk_empty_result(full,offset,index):
 result=execute('function Main(q) assert(q:ReadGlobalGameDataStringAt('+offset+','+index+')=="") end',full)
 assert result['ok'],result['error']

@pytest.mark.parametrize('offset,index',[('0x4d4','nil'),('0x4d4','"1"'),('0x4d4','0.5'),('0x4d4','0/0'),('0x4d4','math.huge'),('0x4d4','2147483648'),('nil','0'),('0.5','0')])
def test_fixture_rejects_arguments_outside_integer_signature(full,offset,index):
 result=execute('function Main(q) q:ReadGlobalGameDataStringAt('+offset+','+index+') end',full)
 assert not result['ok'] and 'signed 32-bit integers' in result['error']

@pytest.mark.parametrize('source',["q:ReadGlobalGameDataStringAt(0x74c,0)","q:ReadGlobalGameDataStringAt(0x4d8,0)","q:ReadGlobalGameDataDefStringAt(0x4d4,0)"])
def test_conversion_tokens_and_inline_values_are_not_interchangeable(full,source):
 result=execute('function Main(q) '+source+' end',full)
 assert not result['ok'] and 'unknown' in result['error']

def test_inline_empty_vector_retains_native_empty_result(full):
 fixture,metadata=copy.deepcopy(full);fixture['inlineStrings'][0]['items']=[]
 result=execute("function Main(q) assert(q:ReadGlobalGameData(0x3c0)==q:ReadGlobalGameData(0x3c4));assert(q:ReadGlobalGameDataStringAt(0x3c0,0)=='') end",(fixture,metadata))
 assert result['ok'],result['error']

@pytest.mark.parametrize('book,lines,marker',[(2,7,'MK_BSAS'),(12,9,'MK_BSAF'),(17,6,'MK_BSAF'),(20,10,'MK_BSAL'),(24,7,'MK_BSAF')])
def test_actual_reactions_use_stock_classroom_markers(full,book,lines,marker):
 r=Path(__file__).resolve().parents[2];source=(r/'refs/script_recovery/lifted/BookCollecting/readable/FSE/V_BookCollecting/V_BookCollecting.lua').read_text(encoding='utf-8')
 source+=f'''
 function Main(q)
 Init(q)
 local seen={{}};local get=q.GetThingWithScriptName
 q.GetThingWithScriptName=function(self,name) seen[name]=true;return get(self,name) end
 BookReaction(q,{book})
 assert(seen.{marker}_BOY and seen.{marker}_GIRL)
 end
 '''
 result=execute(source,full);assert result['ok'],result['error'];assert result['frames']==lines

@pytest.mark.parametrize('fault',['old_schema','missing_inline','missing_conversations'])
def test_value_fixture_schema_is_explicit(full,fault):
 fixture,metadata=copy.deepcopy(full)
 if fault=='old_schema':fixture['schema']='script-definition-vector-fixture/2'
 if fault=='missing_inline':fixture.pop('inlineStrings')
 if fault=='missing_conversations':fixture.pop('conversations')
 with pytest.raises(ValueError):validate_fixture(fixture,metadata)
