from pathlib import Path
import hashlib,struct
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import Lifter,load_manifest,RData
from tools.script_recovery.native_evidence_lowering import finish_lua
from tools.script_recovery.build_readable_unit import readable_file

def lifted(lines,readable=False):
    lifter=Lifter(load_manifest(),{},'quest',False,'',RData());lifter.accessor_kinds=True
    body=finish_lua('\n'.join(lifter.lift('Displacement','{\n'+lines+'\n}',parameters={'xStack_c':'vector','pfVar6':'vector','previous':'number','take':'bool'})))
    # Observe staged literal operands through the same expression lowering used
    # by real consumers; an externally appended raw name bypasses staging.
    result='function Displacement(quest,xStack_c,pfVar6,previous,take)\n'+body+'\nreturn fStack_24,f_stk_20,'+lifter.expr('f_stk_1c')+'\nend'
    return readable_file(result)[0] if readable else result

@pytest.fixture(scope='module')
def native_delta():
    import unicorn as uc
    from unicorn import x86_const as x
    rd=RData();raw=rd.bytes_at(0xe6488c,0x21)
    assert raw.hex()=='d94424708d4c2454d86004d944246cd820c744245c00000000d95c2454d95c2458'
    vm=uc.Uc(uc.UC_ARCH_X86,uc.UC_MODE_32)
    for page in (0xe64000,0x200000,0x300000):vm.mem_map(page,4096)
    vm.mem_write(0xe6488c,raw)
    def run(landing,arena):
        vm.mem_write(0x30006c,struct.pack('<fff',*landing));vm.mem_write(0x200000,struct.pack('<fff',*arena))
        vm.mem_write(0x300054,struct.pack('<fff',91,92,93))
        vm.reg_write(x.UC_X86_REG_ESP,0x300000);vm.reg_write(x.UC_X86_REG_EAX,0x200000)
        vm.emu_start(0xe6488c,0xe648ad,count=20)
        assert vm.reg_read(x.UC_X86_REG_EIP)==0xe648ad and vm.reg_read(x.UC_X86_REG_ECX)==0x300054
        return struct.unpack('<fff',vm.mem_read(0x300054,12))
    return run

@pytest.mark.parametrize('readable',[False,True])
@pytest.mark.parametrize('landing,arena',[
    ((1,2,3),(0,0,0)),((0,0,0),(0,0,0)),((-5,12,99),(2,-3,42)),
    ((1.25,-2.5,63.5),(-0.5,1.25,99)),((16384,32768,1),(-16384,-32768,2)),
    ((-0.125,0.25,-8),(0.25,-0.125,8)),((8,4,2),(8,4,100)),
    ((-8,-4,-2),(-8,-4,-100)),((1024.5,2048.25,0),(1024.25,2048,1)),
    ((1,0,0),(-2**-24,0,0)),((16777216,0,0),(-1,0,0)),
    ((0,1,0),(0,-2**-24,0)),((0,16777216,0),(0,-1,0)),
])
def test_planar_displacement_matches_native_stores(native_delta,readable,landing,arena):
    lua=LuaRuntime(unpack_returned_tuples=True)
    lua.execute(lifted('f_stk_20 = (float)xStack_c._4_4_ - pfVar6.y;\nfStack_24 = (float)xStack_c._0_4_ - pfVar6.x;\nf_stk_1c = 0.0;',readable))
    actual=lua.globals().Displacement(lua.table(),lua.table(**dict(zip('xyz',landing))),lua.table(**dict(zip('xyz',arena))),0,True)
    assert actual==native_delta(landing,arena)

@pytest.mark.parametrize('left','xyz')
@pytest.mark.parametrize('right','xyz')
def test_axes_preserved(left,right):
    lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(lifted(f'fStack_24 = xStack_c.{left} - pfVar6.{right};'))
    a=dict(x=1,y=2,z=4);b=dict(x=8,y=16,z=32)
    assert lua.globals().Displacement(lua.table(),lua.table(**a),lua.table(**b),0,True)[0]==a[left]-b[right]

@pytest.mark.parametrize('readable',[False,True])
@pytest.mark.parametrize('take',[False,True])
def test_conditional_overwrite_preserves_previous_scalar(readable,take):
    lua=LuaRuntime(unpack_returned_tuples=True)
    lua.execute(lifted('fStack_24 = previous;\nif (take) {\nfStack_24 = xStack_c.x - pfVar6.x;\n}',readable))
    value=lua.globals().Displacement(lua.table(),lua.table(x=7),lua.table(x=2),41,take)[0]
    assert value==(5 if take else 41)

@pytest.mark.parametrize('expr',[
 'unknown.x - pfVar6.x','xStack_c.x - unknown.x','xStack_c.w - pfVar6.x',
 'xStack_c.x + pfVar6.x','xStack_c.x - pfVar6.x + 1','GetPos().x - pfVar6.x',
 'xStack_c._12_4_ - pfVar6.x','"xStack_c.x - pfVar6.x"',
 '(int)xStack_c.x - pfVar6.x','xStack_c.x - (uint)pfVar6.x',
 '(char)xStack_c.x - pfVar6.x','(float *)xStack_c.x - pfVar6.x',
])
def test_unknown_or_unsupported_expression_stays_unresolved(expr):
    # A literal string is valid existing operand staging, but must never become
    # a numeric vector-component store.
    source=lifted('fStack_24 = '+expr+';')
    import re
    assert not any(re.match(r'\s*(?:local )?fStack_24 =',line) for line in source.splitlines())

@pytest.mark.parametrize('occupied',['thing','vector','alias','result'])
def test_occupied_storage_does_not_become_float(occupied):
    lifter=Lifter(load_manifest(),{},'quest',False,'',RData());lifter.accessor_kinds=True
    lifter.kinds.update(xStack_c='vector',pfVar6='vector')
    if occupied=='alias':lifter.slot_alias['fStack_24']='apStack_8'
    elif occupied=='result':lifter.slot_results['fStack_24']='oldThing'
    else:lifter.kinds['fStack_24']=occupied
    lifter.statement('Displacement','fStack_24 = xStack_c.x - pfVar6.x;')
    assert not any(l.strip().startswith('fStack_24 =') or l.strip().startswith('local fStack_24 =') for l in lifter.out)

def test_registered_chicken_scalar_store_recovered(tmp_path):
    import json
    from unittest.mock import patch
    from tools.script_recovery.convert_quest_unit import UnitConverter
    root=next(p for p in Path(__file__).resolve().parents if (p/'refs/script_recovery/chicken_kicking').is_dir())
    evidence=root/'refs/script_recovery/chicken_kicking';unit=json.loads((evidence/'units/V_ChickenKicking.json').read_text(encoding='utf-8'))
    entity=unit['entities']['KickedChicken'];entity['functions']={'Main':entity['functions']['Main']};unit['entities']={'KickedChicken':entity};unit['quest']['functions']={};unit['quest']['helpers']={}
    converter=UnitConverter(evidence/'translation_unit_typed.json')
    with patch.object(converter,'constructor_defaults',return_value={}):converter.convert(unit,tmp_path)
    source=(tmp_path/'FSE/V_ChickenKicking/Entities/KickedChicken.lua').read_text(encoding='utf-8')
    assert 'fStack_24 = (string.unpack("<f", string.pack("<f", xStack_c.x - pfVar6.x)))' in source
    assert 'f_stk_20 = (string.unpack("<f", string.pack("<f", xStack_c.y - pfVar6.y)))' in source
    assert source.count('markerResources:NormalizeVector3(')==2
