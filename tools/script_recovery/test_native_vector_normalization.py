import copy,hashlib,json,math,struct,random
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import Lifter,load_manifest,RData
from tools.script_recovery.native_evidence_lowering import finish_lua
from tools.script_recovery.build_readable_unit import readable_file
from native_vector_normalization import recover

SOURCE="""{
f_stk_20 = (float)xStack_c._4_4_ - pfVar6.y;
fStack_24 = (float)xStack_c._0_4_ - pfVar6.x;
f_stk_1c = 0.0;
C3DVector::GetScaled((C3DVector *)&fStack_24);
}"""
FUNCTION={'calls':[{'site':'0x00E648AD','target':'0x00A14440','ecxStack':-36,'currentName':'C3DVector::GetScaled'}]}

@pytest.fixture(scope='module')
def native():
    import unicorn as uc
    from unicorn import x86_const as x
    rd=RData();vm=uc.Uc(uc.UC_ARCH_X86,uc.UC_MODE_32)
    for p in (0xe64000,0xa14000,0x122d000,0x200000,0x300000):vm.mem_map(p,4096)
    vm.mem_write(0xe6488c,rd.bytes_at(0xe6488c,0x26));vm.mem_write(0xa14440,rd.bytes_at(0xa14440,0x3d));vm.mem_write(0x122ded8,rd.bytes_at(0x122ded8,4))
    def run(a,b):
        vm.mem_write(0x30086c,struct.pack('<fff',*a));vm.mem_write(0x200000,struct.pack('<fff',*b))
        vm.reg_write(x.UC_X86_REG_ESP,0x300800);vm.reg_write(x.UC_X86_REG_EAX,0x200000);vm.reg_write(x.UC_X86_REG_FPCW,0x27f)
        vm.emu_start(0xe6488c,0xe648b2,count=80)
        assert vm.reg_read(x.UC_X86_REG_EIP)==0xe648b2 and vm.reg_read(x.UC_X86_REG_ESP)==0x300800
        return struct.unpack('<fff',vm.mem_read(0x300854,12))
    return run

@pytest.fixture(scope='module',params=[False,True])
def emitted(request):
    lift=Lifter(load_manifest(),{},'quest',False,'',RData());lift.accessor_kinds=True
    body=finish_lua('\n'.join(lift.lift('Normalize',SOURCE,native_function=FUNCTION,parameters={'xStack_c':'vector','pfVar6':'vector'})))
    source='function Normalize(quest,xStack_c,pfVar6)\n'+body+'\nreturn fStack_24,f_stk_20,f_stk_1c\nend'
    assert 'inverseLength' in source and 'ENGINE_NormalizeFloat3' not in source
    if request.param:source=readable_file(source)[0]
    lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(source)
    return lua

rng=random.Random(29)
CASES=[((3,4,0),(0,0,0)),((0,0,0),(0,0,0)),((-0.,0.,0.),(0.,0.,0.)),((2**-149,2**-149,0),(0,0,0)),((3e38,0,0),(0,0,0)),((float('inf'),1,0),(0,0,0)),((float('nan'),1,0),(0,0,0))]
for _ in range(128):CASES.append((tuple(struct.unpack('<f',struct.pack('<f',rng.uniform(-1e6,1e6)))[0] for _ in range(3)),tuple(struct.unpack('<f',struct.pack('<f',rng.uniform(-1e6,1e6)))[0] for _ in range(3))))
@pytest.mark.parametrize('a,b',CASES)
def test_emitted_displacement_and_normalize_native(native,emitted,a,b):
    a=struct.unpack('<fff',struct.pack('<fff',*a));b=struct.unpack('<fff',struct.pack('<fff',*b))
    actual=emitted.globals().Normalize(emitted.table(),emitted.table(**dict(zip('xyz',a))),emitted.table(**dict(zip('xyz',b))))
    expect=native(a,b)
    assert all((math.isnan(x) and math.isnan(y)) or struct.pack('<f',x)==struct.pack('<f',y) for x,y in zip(actual,expect))

@pytest.mark.parametrize('change',['target','receiver','site','count','x_component','y_component','different_vector','missing_z','nonzero_z','wrong_x_slot'])
def test_unproven_calls_rejected(change):
    fn=copy.deepcopy(FUNCTION);source=SOURCE
    if change=='target':fn['calls'][0]['target']='0x00A14441'
    elif change=='receiver':fn['calls'][0]['ecxStack']=-40
    elif change=='site':fn['calls'][0]['site']='0x00E648AE'
    elif change=='count':fn['calls']=[]
    elif change=='x_component':source=source.replace('._0_4_','._8_4_')
    elif change=='y_component':source=source.replace('pfVar6.y','pfVar6.z')
    elif change=='different_vector':source=source.replace('pfVar6.x','other.x')
    elif change=='missing_z':source=source.replace('f_stk_1c = 0.0;','')
    elif change=='nonzero_z':source=source.replace('f_stk_1c = 0.0;','f_stk_1c = 1.0;')
    elif change=='wrong_x_slot':source=source.replace('fStack_24','fStack_28')
    result,evidence=recover(fn,source,RData());assert result==source and not evidence

@pytest.mark.parametrize('where',[0xa14440,0xe6488c,0xe648ad,0x122ded8])
def test_changed_native_bytes_rejected(where):
    class Changed(RData):
        def bytes_at(self,a,n):
            data=super().bytes_at(a,n)
            return bytes([data[0]^1])+data[1:] if a==where else data
    assert recover(FUNCTION,SOURCE,Changed())==(SOURCE,[])

@pytest.mark.parametrize('kind',['thing','vector','string','bool'])
def test_unproven_scalar_kind_keeps_diagnostic(kind):
    lift=Lifter(load_manifest(),{},'quest',False,'',RData());lift.accessor_kinds=True
    lift.normalization_tokens={'ENGINE_NormalizeFloat3_0':['fStack_24','f_stk_20','f_stk_1c']}
    lift.kinds.update(fStack_24=kind,f_stk_20='number',f_stk_1c='number')
    lift.statement('Normalize','ENGINE_NormalizeFloat3_0(fStack_24, f_stk_20, f_stk_1c);')
    assert any('TODO(native)' in s for s in lift.out) and not any('inverseLength' in s for s in lift.out)

@pytest.mark.parametrize('slot',['slot_alias','slot_results'])
def test_occupied_output_does_not_normalize(slot):
    lift=Lifter(load_manifest(),{},'quest',False,'',RData());lift.accessor_kinds=True
    lift.normalization_tokens={'ENGINE_NormalizeFloat3_0':['fStack_24','f_stk_20','f_stk_1c']}
    lift.kinds.update(fStack_24='number',f_stk_20='number',f_stk_1c='number')
    getattr(lift,slot)['fStack_24']='occupied'
    lift.statement('Normalize','ENGINE_NormalizeFloat3_0(fStack_24, f_stk_20, f_stk_1c);')
    assert any('TODO(native)' in s for s in lift.out) and not any('inverseLength' in s for s in lift.out)

def test_unproved_pseudo_stays_diagnostic():
    lift=Lifter(load_manifest(),{},'quest',False,'',RData());lift.accessor_kinds=True
    lift.statement('Normalize','ENGINE_NormalizeFloat3_0(fStack_24, f_stk_20, f_stk_1c);')
    assert any('TODO(native)' in s for s in lift.out)

def test_registered_first_call_only(tmp_path):
    from unittest.mock import patch
    from tools.script_recovery.convert_quest_unit import UnitConverter
    root=next(p for p in Path(__file__).resolve().parents if (p/'refs/script_recovery/chicken_kicking').is_dir())
    path=root/'refs/script_recovery/chicken_kicking';unit=json.loads((path/'units/V_ChickenKicking.json').read_text())
    ent=unit['entities']['KickedChicken'];ent['functions']={'Main':ent['functions']['Main']};unit['entities']={'KickedChicken':ent};unit['quest']['functions']={};unit['quest']['helpers']={}
    converter=UnitConverter(path/'translation_unit_typed.json')
    with patch.object(converter,'constructor_defaults',return_value={}):converter.convert(unit,tmp_path)
    text=(tmp_path/'FSE/V_ChickenKicking/Entities/KickedChicken.lua').read_text()
    assert text.count('local inverseLength =')==1
    assert text.count('markerResources:NormalizeVector3(')==2
    assert '-- TODO(native): C3DVector::GetScaled' not in text
    assert 'ENGINE_NormalizeFloat3' not in text
    assert text.count('markerPositions:AppendScriptName(')==4
    assert 'Vector_PushBack_ScriptThing' not in text
    LuaRuntime().compile(text);LuaRuntime().compile(readable_file(text)[0])
