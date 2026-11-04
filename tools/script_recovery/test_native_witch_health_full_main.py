from pathlib import Path
import hashlib,json,struct
import pytest
from lupa.lua54 import LuaRuntime
R=Path(__file__).resolve().parents[2]
F=Path(__file__).resolve().parent/'fixtures/witch_health/main'
@pytest.mark.parametrize('stage',['draft','readable'])
def test_complete_parent_member_callbacks_retain_all_accepted_stage_observations(stage):
 source=(R/'refs/script_recovery/lifted/SickChild'/stage/'FSE/V_SickChild/Entities/Witch.lua').read_text();helper=(F/'helper.lua').read_text();cases=json.loads((F/'cases.json').read_text());goldens=json.loads((F/'goldens.json').read_text())['expected'];assert len(cases)==len(goldens)==56320
 vm=LuaRuntime(unpack_returned_tuples=True);run=vm.execute((F/'harness.lua').read_text());lines={n:True for n,line in enumerate(source.splitlines(),1) if 'resources:ResourceThingHealthAbove(' in line};assert len(lines)==6;line_table=vm.table_from(lines);coverage=set()
 for n,(case,golden) in enumerate(zip(cases,goldens)):
  value=struct.unpack('<f',struct.pack('<I',case['bits']))[0];result=run(source,helper,vm.table_from(case),value,line_table);observed=result[:3]+result[4:];assert hashlib.sha256(json.dumps(observed,separators=(',',':')).encode()).hexdigest()==golden[stage],(n,case,stage)
  if result[3]:coverage.update(map(int,result[3].split(',')))
 assert coverage==set(lines)
