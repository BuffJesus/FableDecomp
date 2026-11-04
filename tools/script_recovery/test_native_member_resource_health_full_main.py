from pathlib import Path
import hashlib,json,struct
import pytest
from lupa.lua54 import LuaRuntime
R=Path(__file__).resolve().parents[2]
F=Path(__file__).resolve().parent/'fixtures/member_resource_health/main'
@pytest.mark.parametrize('stage',['draft','readable'])
def test_complete_member_callbacks_retain_accepted_stage_traces(stage):
 source=(R/'refs/script_recovery/lifted/SickChild'/stage/'FSE/V_SickChild/Entities/WomanToAttract.lua').read_text()
 cases=json.loads((F/'cases.json').read_text());goldens=json.loads((F/'goldens.json').read_text())['expected']
 assert len(cases)==len(goldens)==3744
 vm=LuaRuntime(unpack_returned_tuples=True);harness=vm.execute((F/'harness.lua').read_text())
 lines={n:True for n,line in enumerate(source.splitlines(),1) if 'resources:ResourceThingHealthAbove(' in line};assert len(lines)==7
 coverage=set();line_table=vm.table_from(lines)
 for n,(case,golden) in enumerate(zip(cases,goldens)):
  value=struct.unpack('<f',struct.pack('<I',case['bits']))[0]
  trace,speeches,queries,hit,paused,timer_live,movie_live=harness(source,vm.table_from(case),value,line_table)
  assert hashlib.sha256(trace.encode()).hexdigest()==golden[stage],(n,case,stage)
  if hit:coverage.update(map(int,hit.split(',')))
 assert coverage==set(lines)
