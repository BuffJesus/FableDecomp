from pathlib import Path
import hashlib,json,struct
import pytest
from lupa.lua54 import LuaRuntime
R=Path(__file__).resolve().parents[2]
F=Path(__file__).resolve().parent/'fixtures/typed_boolean_health/main'
@pytest.mark.parametrize('stage',['draft','readable'])
def test_complete_callbacks_retain_finite_traces_and_ordered_nan_decisions(stage):
 source=(R/'refs/script_recovery/lifted/SickChild'/stage/'FSE/V_SickChild/Entities/WomanToAttract.lua').read_text()
 cases=json.loads((F/'cases.json').read_text());goldens=json.loads((F/'goldens.json').read_text())['expected']
 assert len(cases)==len(goldens)==1872
 vm=LuaRuntime(unpack_returned_tuples=True);harness_source=(F/'harness.lua').read_text()
 helper='function r:ResourceThingHealthAbove(handle,minimum)return q:GetHealth(self:ScriptThing(handle))>minimum end\n '
 harness=vm.execute(harness_source.replace('function r:ScriptThing',helper+'function r:ScriptThing'))
 lines={n:True for n,line in enumerate(source.splitlines(),1) if 'quest:GetHealth(' in line or 'resources:ResourceThingHealthAbove(' in line};assert len(lines)==7
 coverage=set();line_table=vm.table_from(lines)
 for n,(case,golden) in enumerate(zip(cases,goldens)):
  value=struct.unpack('<f',struct.pack('<I',case['bits']))[0]
  trace,speeches,queries,hit,paused,timer_live,movie_live=harness(source,vm.table_from(case),value,line_table)
  assert hashlib.sha256(trace.encode()).hexdigest()==golden[stage],(n,case,stage)
  if hit:coverage.update(map(int,hit.split(',')))
 assert coverage==set(lines)
