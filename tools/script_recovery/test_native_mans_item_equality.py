import itertools,json,sys
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').exists())
sys.path.insert(0,str(ROOT))
from tools.script_recovery import native_string_values as m
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.build_readable_unit import readable_file
@pytest.fixture(scope='module')
def sources(tmp_path_factory):
 unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text())
 unit['entities']={'MansLover':unit['entities']['MansLover']};unit['quest']['functions']={};unit['quest']['helpers']={}
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json');out=tmp_path_factory.mktemp('mans_item')
 with patch.object(c,'constructor_defaults',return_value={}):c.convert(unit,out)
 draft=(out/'FSE/V_SickChild/Entities/MansLover.lua').read_text()
 return {'draft':draft,'readable':readable_file(draft)[0]}
H="return function(expected,item,message,talked)\n local hit,queries=false,0\n local me={}\n local resources={}\n function resources:NewResource()return 21 end\n function resources:PrepareResource(r)assert(r==21)end\n function resources:TryAcquire(r,actor,kind)assert(r==21 and actor==me and kind==4);return true end\n function resources:ReleaseResource(r)assert(r==21)end\n function resources:StartMovie(key)assert(key=='');hit=true;error('PROBE_MOVIE',0)end\n local q={}\n function q:RetailResources()return resources end\n function q:GetHero()return 717 end\n function q:NewScriptFrame(actor)assert(actor==811);return true end\n function q:IsActiveThreadTerminating()return queries>0 end\n function q:GetStateInt(key)assert(key=='MansLoverState');return 1 end\n function q:ReadGlobalGameDataString(offset)assert(offset==0x73c);return expected end\n function me:MsgIsPresentedWithItem()queries=queries+1;if message then g_PresentedItemName=item end;return message end\n function me:IsTalkedToByHero()return talked end\n -- Termination must remain false until the comparison has decided whether\n -- to enter its conversation; leave at the next frame for the idle path.\n function q:NewScriptFrame(actor) return true end\n function q:IsActiveThreadTerminating()return false end\n function me:IsTalkedToByHero()\n  if talked then return true end\n  if not message or expected~=item then error('PROBE_IDLE',0)end\n  return false\n end\n local ok,err=pcall(Main,q,me)\n assert(not ok and (err=='PROBE_MOVIE' or err=='PROBE_IDLE'),tostring(err))\n assert(hit==(talked or (message and expected==item)),'wrong item-trigger branch')\n return hit,queries\nend"
CASES=[('ITEM_A', 'ITEM_A', False, False), ('ITEM_A', 'ITEM_A', False, True), ('ITEM_A', 'ITEM_A', True, False), ('ITEM_A', 'ITEM_A', True, True), ('ITEM_A', 'ITEM_B', False, False), ('ITEM_A', 'ITEM_B', False, True), ('ITEM_A', 'ITEM_B', True, False), ('ITEM_A', 'ITEM_B', True, True), ('ITEM_A', '', False, False), ('ITEM_A', '', False, True), ('ITEM_A', '', True, False), ('ITEM_A', '', True, True), ('ITEM_A', 'OBJECT_BEER_TANKARD', False, False), ('ITEM_A', 'OBJECT_BEER_TANKARD', False, True), ('ITEM_A', 'OBJECT_BEER_TANKARD', True, False), ('ITEM_A', 'OBJECT_BEER_TANKARD', True, True), ('ITEM_B', 'ITEM_A', False, False), ('ITEM_B', 'ITEM_A', False, True), ('ITEM_B', 'ITEM_A', True, False), ('ITEM_B', 'ITEM_A', True, True), ('ITEM_B', 'ITEM_B', False, False), ('ITEM_B', 'ITEM_B', False, True), ('ITEM_B', 'ITEM_B', True, False), ('ITEM_B', 'ITEM_B', True, True), ('ITEM_B', '', False, False), ('ITEM_B', '', False, True), ('ITEM_B', '', True, False), ('ITEM_B', '', True, True), ('ITEM_B', 'OBJECT_BEER_TANKARD', False, False), ('ITEM_B', 'OBJECT_BEER_TANKARD', False, True), ('ITEM_B', 'OBJECT_BEER_TANKARD', True, False), ('ITEM_B', 'OBJECT_BEER_TANKARD', True, True), ('', 'ITEM_A', False, False), ('', 'ITEM_A', False, True), ('', 'ITEM_A', True, False), ('', 'ITEM_A', True, True), ('', 'ITEM_B', False, False), ('', 'ITEM_B', False, True), ('', 'ITEM_B', True, False), ('', 'ITEM_B', True, True), ('', '', False, False), ('', '', False, True), ('', '', True, False), ('', '', True, True), ('', 'OBJECT_BEER_TANKARD', False, False), ('', 'OBJECT_BEER_TANKARD', False, True), ('', 'OBJECT_BEER_TANKARD', True, False), ('', 'OBJECT_BEER_TANKARD', True, True), ('OBJECT_BEER_TANKARD', 'ITEM_A', False, False), ('OBJECT_BEER_TANKARD', 'ITEM_A', False, True), ('OBJECT_BEER_TANKARD', 'ITEM_A', True, False), ('OBJECT_BEER_TANKARD', 'ITEM_A', True, True), ('OBJECT_BEER_TANKARD', 'ITEM_B', False, False), ('OBJECT_BEER_TANKARD', 'ITEM_B', False, True), ('OBJECT_BEER_TANKARD', 'ITEM_B', True, False), ('OBJECT_BEER_TANKARD', 'ITEM_B', True, True), ('OBJECT_BEER_TANKARD', '', False, False), ('OBJECT_BEER_TANKARD', '', False, True), ('OBJECT_BEER_TANKARD', '', True, False), ('OBJECT_BEER_TANKARD', '', True, True), ('OBJECT_BEER_TANKARD', 'OBJECT_BEER_TANKARD', False, False), ('OBJECT_BEER_TANKARD', 'OBJECT_BEER_TANKARD', False, True), ('OBJECT_BEER_TANKARD', 'OBJECT_BEER_TANKARD', True, False), ('OBJECT_BEER_TANKARD', 'OBJECT_BEER_TANKARD', True, True)]

@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('expected,item,message,talked',CASES)
def test_actual_item_gate(sources,style,expected,item,message,talked):
 lua=LuaRuntime();lua.execute(sources[style]);lua.execute(H)(expected,item,message,talked)
TEXT="      if ((cVar3 != '\\0') && ((xStack_128 == xStack_12c || ((((xStack_128 != (undefined4 *)0x0 && (xStack_12c != (undefined4 *)0x0)) && (xStack_128[1] == xStack_12c[1])) && (compare_result = CBasicString<char>::Compare((void *)*xStack_128,(void *)*xStack_12c), compare_result == 0)))))) {"
def test_complete_guarded_array_equality():
 assert m.fold_rep_equality(TEXT)=="      if ((cVar3 != '\\0') && ENGINE_StrEq(xStack_128, xStack_12c)) {"
@pytest.mark.parametrize('old,new',[
 ('xStack_128[1]','xStack_128[2]'),('xStack_12c[1]','other[1]'),('compare_result == 0','compare_result != 0'),
 ('(void *)*xStack_12c','(void *)*other'),('xStack_128 == xStack_12c','xStack_128 != xStack_12c'),
 ('xStack_128 != (undefined4 *)0x0','xStack_128 == (undefined4 *)0x0'),
 ('xStack_12c != (undefined4 *)0x0','xStack_12c != (undefined4 *)0x1'),
 ("cVar3 != '\\0'","cVar3 == '\\0'"),(' && ',' || '),('xStack_12c[1]','xStack_12c[0]')])
def test_incomplete_or_different_comparison_stays_explicit(old,new):
 text=TEXT.replace(old,new);assert text!=TEXT and m.fold_rep_equality(text)==text
@pytest.mark.parametrize('tail',['\nUse(compare_result);','\nEscape(&compare_result);','\ncompare_result = compare_result + 1;', '\nif (flag) {\n compare_result = 1;\n}\nUse(compare_result);'])
def test_observable_compare_result_rejects_fold(tail):
 text=TEXT+tail;assert m.fold_rep_equality(text)==text
def test_default_cast_spelling_unchanged():
 text='if ((a == b) || ((((a != (CCharString)0x0 && (b != (CCharString)0x0)) && (*(int *)((int)a + 4) == *(int *)((int)b + 4))) && (temp = CBasicString<char>::Compare(*(void **)a,*(void **)b), temp == 0)))) {'
 assert m.fold_rep_equality(text)=='if (ENGINE_StrEq(a, b)) {'

def test_original_native_item_equality():
 import struct
 import unicorn as u
 from unicorn import x86_const as x
 from tools.script_recovery.lift_native_lua import RData
 rd=RData();cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xec9000,0x2000),(0x411000,0x2000),(0xf35000,0x1000),(0x300000,0x10000),(0x500000,0x5000)):cpu.mem_map(a,n)
 cpu.mem_write(0xec9e98,rd.bytes_at(0xec9e98,0x41));cpu.mem_write(0x411570,rd.bytes_at(0x411570,0x40));cpu.mem_write(0xf35b30,b'\x31\xc0\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def hook(machine,a,size,user):
  if a==0xec9ed9:machine.emu_stop()
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 rows=[]
 for left,right,identity,sp in itertools.product((None,'','A','B','LONG_ITEM'),(None,'','A','B','LONG_ITEM'),(False,True),(0x300800,0x301800)):
  if identity and left!=right:continue
  a=0 if left is None else 0x500000;b=a if identity else (0 if right is None else 0x501000)
  for rep,value,buffer in ((a,left,0x502000),(b,right,0x503000)):
   if rep:
    put(rep,buffer);put(rep+4,len(value));cpu.mem_write(buffer,value.encode()+b'\0')
  put(sp+0x14,a);put(sp+0x10,b);cpu.mem_write(sp+0x2f,b'\0')
  cpu.reg_write(x.UC_X86_REG_ESP,sp);cpu.reg_write(x.UC_X86_REG_EDI,0);cpu.reg_write(x.UC_X86_REG_ESI,0x504000)
  cpu.emu_start(0xec9e98,0xec9ed9,count=1000)
  result=cpu.mem_read(sp+0x2f,1)==b'\1';assert result==(left==right),(left,right,identity,result)
  assert cpu.reg_read(x.UC_X86_REG_ESP)==sp
  rows.append(dict(left=left,right=right,identity=identity,stack=sp,result=result))
 assert len(rows)==60
