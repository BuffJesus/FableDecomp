from pathlib import Path
import hashlib,json,re
import pytest
from lupa.lua54 import LuaRuntime
W=Path(__file__).resolve().parent
ROOT=next(p for p in W.parents if (p/'tools/script_recovery/script_units.py').is_file())
FIXTURES=W/'fixtures/trader2_resource_health/main'
SOURCE_ROOT=ROOT/'refs/script_recovery/lifted/SickChild'
@pytest.mark.parametrize('stage',['draft','readable'])
def test_complete_main_traces_match_accepted_stage(stage):
 cases=json.loads((FIXTURES/'cases.json').read_text());goldens=json.loads((FIXTURES/'goldens.json').read_text());harness=(FIXTURES/'harness.lua').read_text()
 assert goldens['schema']==1 and goldens['cases']==len(cases)==3164
 source=(SOURCE_ROOT/stage/'FSE/V_SickChild/Entities/TalkingTrader2.lua').read_text()
 expected={i+1 for i,line in enumerate(source.splitlines()) if 'resources:ResourceThingHealthAbove(' in line};assert len(expected)==3
 assert source.count('resources:IsHitByHeroExceptAbilityAndHealthAbove(')==1
 observed=set();vm=LuaRuntime(unpack_returned_tuples=True);vm.globals().HEALTH_OBSERVER=lambda line,health:observed.add((line,health>0));vm.execute(source);run=vm.execute(harness)
 for index,case in enumerate(cases):
  trace=run(vm.table_from(case));actual=hashlib.sha256(json.dumps(trace,separators=(',',':')).encode()).hexdigest()
  assert actual==goldens['traceSha256'][stage][index],(stage,index,case,trace)
 assert observed=={(line,positive) for line in expected for positive in [False,True]}
