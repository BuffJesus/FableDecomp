from pathlib import Path
import struct
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import Lifter,load_manifest,RData
from tools.script_recovery.native_evidence_lowering import normalise_typed_decompile,strip_receiver_arguments,finish_lua
from tools.script_recovery.build_readable_unit import readable_file

SITES=((0xe67c78,100),(0xe67cd6,50),(0xe67d38,25),(0xe67db2,10),
       (0xe67e10,25),(0xe67e72,10),(0xe67eec,100),(0xe67f4a,50),(0xe67fac,25))

def generated(delta,readable):
 source='''CVar19 = 0;
CVar19 = CVar19 | 1;
CVar19 = *(CCharString_bv *)(this + 4);
xStack_374 = CVar19;
GSI->PauseAllNonScriptedEntities(true);
CVar19 = 0;
xStack_374 = 0;
'''
 for bit in (1,2,4,8,16):
  source+=f'''if ((mask & {bit}) != 0) {{
CVar19 = (CCharString)((int)CVar19 + {delta});
xStack_374 = CVar19;
}}
'''
 source+='''xStack_374 = (CCharString)(float)(int)xStack_374;
GSI->UpdateQuestInfoCounter(1,(float)xStack_374,-1);'''
 source=strip_receiver_arguments(normalise_typed_decompile(source))
 lifter=Lifter(load_manifest(),{},'quest',False,'',RData());lifter.accessor_kinds=True
 body=finish_lua('\n'.join(lifter.lift('Score','{\n'+source+'\n}',parameters={'mask':'number'})))
 result='function Score(quest,mask)\n'+body+'\nend'
 return readable_file(result)[0] if readable else result

@pytest.fixture(scope='module')
def native_score():
 import unicorn as u
 from unicorn import x86_const as x
 rd=RData();vm=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xe67000,0x2000),(0x143e000,0x1000),(0x300000,0x1000),(0x500000,0x1000)):vm.mem_map(a,n)
 vm.mem_write(0xe67000,rd.bytes_at(0xe67000,0x2000));vm.mem_write(0x500000,b'\xdb\xe3')
 assert rd.bytes_at(0xe677c9,8).hex()=='33ed33db895c2440'
 for site,delta in SITES:assert rd.bytes_at(site,7)==bytes.fromhex('83c3')+bytes([delta])+bytes.fromhex('895c2440')
 assert rd.bytes_at(0xe6822b,14).hex()=='db4424408b0d0ce94301d9542440'
 def run(site,mask):
  vm.reg_write(x.UC_X86_REG_ESP,0x300000)
  vm.mem_write(0x300040,b'\xff'*4)
  vm.emu_start(0xe677c9,0xe677d1,count=10)
  for bit in (1,2,4,8,16):
   if mask&bit:vm.emu_start(site,site+7,count=10)
  integer=struct.unpack('<i',vm.mem_read(0x300040,4))[0]
  vm.emu_start(0x500000,0x500002,count=1);vm.reg_write(x.UC_X86_REG_FPCW,0x27f)
  vm.emu_start(0xe6822b,0xe68239,count=10)
  stored=struct.unpack('<f',vm.mem_read(0x300040,4))[0]
  assert integer==stored
  return stored
 return run

@pytest.mark.parametrize('site,delta',SITES)
@pytest.mark.parametrize('readable',(False,True))
def test_five_kicks_keep_zero_or_last_saved_score(native_score,site,delta,readable):
 lua=LuaRuntime();lua.execute(generated(delta,readable))
 run=lua.eval('''function(mask)
 local paused=false;local calls=0;local score
 local q={}
 function q:PauseAllNonScriptedEntities(flag)assert(flag==true);paused=true end
 function q:UpdateQuestInfoCounter(id,value,maximum)
  assert(paused and id==1 and maximum==-1 and type(value)=='number')
  calls=calls+1;score=value
 end
 Score(q,mask);assert(calls==1);return score
 end''')
 for mask in range(32):assert run(mask)==native_score(site,mask)

@pytest.mark.parametrize('object_use',('Touch(&xStack_10);','xStack_10._0_4_ = 4;','Object::Use(xStack_10);'))
def test_numeric_cast_does_not_classify_object_storage(object_use):
 lifter=Lifter(load_manifest(),{},'quest',False,'',RData());lifter.accessor_kinds=True
 source='xStack_10 = 0;\n'+object_use+'\nxStack_10 = value;\nf_stk_10 = (int)xStack_10;'
 body='\n'.join(lifter.lift('Ambiguous','{\n'+source+'\n}',parameters={'value':'number'}))
 assert 'xStack_10 = 0' not in body

@pytest.fixture(scope='module')
def generated_main(tmp_path_factory):
 import json
 from unittest.mock import patch
 from tools.script_recovery.convert_quest_unit import UnitConverter
 root=next(p for p in Path(__file__).resolve().parents if (p/'refs/script_recovery/chicken_kicking').is_dir())
 evidence=root/'refs/script_recovery/chicken_kicking'
 converter=UnitConverter(evidence/'translation_unit_typed.json')
 unit=json.loads((evidence/'units/V_ChickenKicking.json').read_text())
 out=tmp_path_factory.mktemp('score_main')
 with patch.object(converter,'constructor_defaults',return_value={}):converter.convert(unit,out)
 draft=(out/'FSE/V_ChickenKicking/Entities/ChickenMaster.lua').read_text()
 return {'draft':draft,'readable':readable_file(draft)[0]}

@pytest.mark.parametrize('style',('draft','readable'))
def test_actual_main_score_storage_fragments(generated_main,native_score,style):
 import re
 source=generated_main[style]
 pairs=list(re.finditer(r'\b(\w+) = \1 \+ (0x[0-9a-f]+|\d+)\n\s*(\w+) = \1\b',source))
 assert len(pairs)==9 and [int(p[2],0) for p in pairs]==[d for _,d in SITES]
 register,slot=pairs[0][1],pairs[0][3]
 assert all((p[1],p[3])==(register,slot) for p in pairs)
 init=re.search(r'\b'+register+r' = 0\n\s*'+slot+r' = 0\b',source)
 assert init is not None
 lua=LuaRuntime()
 for pair,(site,_) in zip(pairs,SITES):
  # Execute emitted initialization and increment/save statements, skipping
  # selected kicks. This certifies scalar storage, not surrounding gameplay.
  run=lua.execute('return function(mask) local '+register+','+slot+'\n'+init[0]+
    '\nfor bit=0,4 do if (mask & (1 << bit))~=0 then\n'+pair[0]+
    '\nend end\nreturn '+slot+' end')
  for mask in range(32):assert run(mask)==native_score(site,mask)
