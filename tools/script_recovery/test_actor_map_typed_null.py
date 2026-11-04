import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_evidence_lowering import fold_actor_maps, finish_lua
from tools.script_recovery.lift_native_lua import Lifter, load_manifest
from tools.script_recovery.test_lift_native_lua import NoRData

SOURCE='''{
xStack_38._0_4_ = malloc(0x24);
*(undefined1 *)xStack_38._0_4_ = 0;
*(undefined **)(xStack_38._0_4_ + 4) = (undefined *)0x0;
*(undefined4 *)(xStack_38._0_4_ + 8) = xStack_38._0_4_;
*(undefined4 *)(xStack_38._0_4_ + 0xc) = xStack_38._0_4_;
RESOURCE_RunMacro("SCENE", xStack_38, false, true);
StdMap_Destroy_API(xStack_38);
}'''


@pytest.mark.parametrize('member',['._0_4_',''])
@pytest.mark.parametrize('zero',['0','0x0','(undefined *)0x0'])
@pytest.mark.parametrize('store',['undefined **','undefined4 *'])
def test_equivalent_null_parent_store_retains_constructor_and_consumers(member,zero,store):
    source=SOURCE.replace('._0_4_',member).replace('(undefined *)0x0',zero).replace('(undefined **)',f'({store})')
    result=fold_actor_maps(source)
    assert result=='''{
xStack_38 = ACTORMAP_New();
RESOURCE_RunMacro("SCENE", xStack_38, false, true);
ACTORMAP_Destroy(xStack_38);
}'''


def test_actual_lifter_uses_and_destroys_the_constructed_map():
    lifter=Lifter(load_manifest(),{},'quest',False,'Pkg',NoRData());lifter.accessor_kinds=True
    body=finish_lua('\n'.join(lifter.lift('Main',fold_actor_maps(SOURCE))))
    assert not lifter.todo,lifter.todo
    lua=LuaRuntime();actor_map=lua.table();seen=[];equal=lua.eval('function(a,b) return a==b end')
    def new(_):seen.append('new');return actor_map
    def run(_,name,value,setup,skip):
        assert name=='SCENE' and equal(value,actor_map) and setup is False and skip is True
        seen.append('run')
    def destroy(_,value):assert equal(value,actor_map);seen.append('destroy')
    resources=lua.table_from(dict(NewActorMap=new,RunMacro=run,DestroyActorMap=destroy))
    lua.execute('return function(resources)\n'+body+'\nend')(resources)
    assert seen==['new','run','destroy']


@pytest.mark.parametrize('old,new', [
    ('malloc(0x24)','malloc(0x28)'),
    ('(undefined *)0x0','(undefined *)0x1'),
    ('(undefined **)(xStack_38._0_4_ + 4)','(undefined **)(other + 4)'),
    ('+ 4) =','+ 5) ='),
    ('+ 8) = xStack_38._0_4_','+ 8) = other'),
    ('+ 0xc) = xStack_38._0_4_','+ 0xc) = other'),
    ('*(undefined1 *)xStack_38._0_4_ = 0','*(undefined1 *)xStack_38._0_4_ = 1'),
    ('*(undefined **)', '*(undefined8 *)'),
])
def test_incomplete_or_different_tree_shape_is_not_constructed(old,new):
    assert 'ACTORMAP_New' not in fold_actor_maps(SOURCE.replace(old,new))
