import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_evidence_lowering import fold_local_thing_vectors, finish_lua
from tools.script_recovery.lift_native_lua import Lifter, load_manifest
from tools.script_recovery.test_lift_native_lua import NoRData

SOURCE = '''{
int iStack_50;
int byteOffset;
CCharString xStack_6c;
CCharString xStack_20;
byteOffset = 0;
CCharString::CCharString(&xStack_20,"DefensiveGuardBandit",-1);
GSI->GetAllThingsWithScriptName(&xStack_20,&iStack_50);
signFix = iStack_50 - iStack_50 >> 0x1f;
index = 0;
if ((iStack_50 - iStack_50) / 0xc + signFix != signFix) {
 do {
  stop = GSI->IsActiveThreadTerminating();
  if (stop) {
   return;
  }
  xStack_6c = (CCharString)(iStack_50 + byteOffset);
  hero = GSI->GetHero();
  GSI->GiveThingBestEnemyTarget((CScriptThing *)xStack_6c,hero);
  index = index + 1;
  byteOffset = byteOffset + 0xc;
 } while (index < (uint)((iStack_50 - iStack_50) / 0xc));
}
}'''


def test_integer_begin_scalar_element_is_a_thing_and_count_is_numeric():
    text=fold_local_thing_vectors(SOURCE)
    assert 'xStack_6c = LOCALLIST_At(iStack_50, (byteOffset) / 0xc);' in text
    assert 'if (LOCALLIST_Count(iStack_50) != 0)' in text
    assert 'signFix' not in text and 'iStack_50 - iStack_50' not in text


@pytest.mark.parametrize('count', [0,1,2,5])
@pytest.mark.parametrize('limit', [0,1,3,9])
def test_actual_lifter_targets_each_list_actor_once_until_cancelled(count,limit):
    lifter=Lifter(load_manifest(),{},'quest',False,'Pkg',NoRData(),live_termination=True)
    lifter.accessor_kinds=True
    body=finish_lua('\n'.join(lifter.lift('Main',fold_local_thing_vectors(SOURCE))))
    assert not lifter.todo, lifter.todo
    lua=LuaRuntime();actors=[lua.table() for _ in range(count)];hero=lua.table();seen=[]
    equal=lua.eval('function(a,b) return a==b end')
    calls=0
    def stopped(_):
        nonlocal calls
        calls+=1
        return calls>limit
    def target(_,actor,enemy):
        assert equal(actor,actors[len(seen)]) and equal(enemy,hero)
        seen.append(len(seen))
    quest=lua.table_from(dict(GetAllThingsWithScriptName=lambda _,name:lua.table_from(actors),
                             IsActiveThreadTerminating=stopped,GetHero=lambda _:hero,GiveThingBestEnemyTarget=target))
    lua.execute('return function(quest)\n'+body+'\nend')(quest)
    assert len(seen)==min(count,limit)


@pytest.mark.parametrize('old,new', [
    ('int iStack_50;', 'undefined4 *iStack_50;'),
    ('int iStack_50;', ''),
    ('(CScriptThing *)xStack_6c','xStack_6c'),
    ('byteOffset + 0xc','byteOffset + 4'),
    ('hero = GSI->GetHero();','xStack_6c = other;\n  hero = GSI->GetHero();'),
    ('GetAllThingsWithScriptName','UnknownCall'),
])
def test_unproven_storage_or_units_stay_explicit(old,new):
    text=fold_local_thing_vectors(SOURCE.replace(old,new))
    assert 'xStack_6c = (CCharString)(iStack_50 + byteOffset);' in text


def test_non_vector_uncast_sign_arithmetic_is_unchanged():
    text='sign = value - value >> 0x1f;\nif ((value - value) / 0xc + sign != sign) { f(); }\n'
    assert fold_local_thing_vectors(text)==text
