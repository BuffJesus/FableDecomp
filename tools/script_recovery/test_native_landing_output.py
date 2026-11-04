from pathlib import Path
import struct
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import Lifter,load_manifest,RData
from tools.script_recovery.native_evidence_lowering import finish_lua
from tools.script_recovery.build_readable_unit import readable_file

def source_for(query='bVar1 = GSI->EntityGetShotStrikePos(me,(C3DVector *)xStack_c);',readable=False):
    lifter=Lifter(load_manifest(),{},'quest',True,'',RData());lifter.accessor_kinds=True
    body=finish_lua('\n'.join(lifter.lift('Query','{\n'+query+'\n}',parameters={'me':'thing','xStack_c':'vector'})))
    source='function Query(quest,me,xStack_c)\n'+body+'\nreturn bVar1,xStack_c\nend\n'
    return readable_file(source)[0] if readable else source

@pytest.fixture(scope='module')
def native_query():
    import hashlib
    import unicorn as uc
    from unicorn import x86_const as x
    vm=uc.Uc(uc.UC_ARCH_X86,uc.UC_MODE_32)
    for page in (0x89d000,0x40f000,0x8ac000,0x200000,0x300000,0x400000,0x500000):vm.mem_map(page,4096)
    rd=RData();raw=rd.bytes_at(0x89df70,0x80)
    assert hashlib.sha256(raw).hexdigest()=='fa009865da34fa3d9e2debf11f71906def9ae46c4a642a69386fd743df13b865'
    assert int.from_bytes(rd.bytes_at(0x1260f0c+0x71c,4),'little')==0x89df70
    vm.mem_write(0x89df70,raw)
    vm.mem_write(0x500100,b'\xc3');vm.mem_write(0x40f020,b'\xc2\x04\x00');vm.mem_write(0x8ac7a0,b'\xc2\x08\x00')
    put=lambda a,v:vm.mem_write(a,struct.pack('<I',v))
    get=lambda a:struct.unpack('<I',vm.mem_read(a,4))[0]
    state={}
    def hook(machine,address,size,user):
        if address==0x500100:machine.reg_write(x.UC_X86_REG_EAX,0 if state['mode']=='no_actor_data' else 0x200100)
        elif address==0x40f020:machine.reg_write(x.UC_X86_REG_EAX,0x200400)
        elif address==0x8ac7a0:
            success=state['mode']!='no_message'
            if success:
                esp=machine.reg_read(x.UC_X86_REG_ESP);assert get(esp+4)==get(esp+8)
                put(get(esp+4),0x200900)
            machine.reg_write(x.UC_X86_REG_EAX,int(success))
    vm.hook_add(uc.UC_HOOK_CODE,hook)
    def run(mode,position,previous):
        state['mode']=mode;put(0x200000,0x500000);put(0x50002c,0x500100)
        vm.mem_write(0x200191,bytes([1 if mode=='disabled' else 0]));vm.mem_write(0x200128,bytes([0 if mode=='wrong_type' else 4]))
        put(0x200148,0x200500);put(0x200400,0x42);put(0x200404,0x200800)
        # Native89DFD3 skips the message's eight-byte header before copying xyz.
        vm.mem_write(0x200908,struct.pack('<fff',*position));vm.mem_write(0x200a00,struct.pack('<fff',*previous))
        vm.mem_write(0x300f00,struct.pack('<III',0x400000,0x200000,0x200a00));vm.reg_write(x.UC_X86_REG_ESP,0x300f00)
        vm.emu_start(0x89df70,0x400000,count=1000)
        assert vm.reg_read(x.UC_X86_REG_EIP)==0x400000 and vm.reg_read(x.UC_X86_REG_ESP)==0x300f0c
        return bool(vm.reg_read(x.UC_X86_REG_EAX)&0xff),struct.unpack('<fff',vm.mem_read(0x200a00,12))
    return run

@pytest.mark.parametrize('readable',[False,True])
@pytest.mark.parametrize('mode',['success','no_actor_data','disabled','wrong_type','no_message'])
@pytest.mark.parametrize('position',[(1.25,-2.5,63.5),(0.0,0.0,0.0),(-16384.5,32768.25,0.125)])
def test_vector_and_flag_match_bounded_native_query(native_query,readable,mode,position):
    previous=(-111.0,222.0,-333.0);flag,expected=native_query(mode,position,previous)
    lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(source_for(readable=readable));old=lua.table(x=previous[0],y=previous[1],z=previous[2]);actor=lua.table();quest=lua.table();calls=[]
    def query(self,received):
        assert lua.eval('function(a,b)return a==b end')(received,actor)
        calls.append(True)
        return lua.table(x=position[0],y=position[1],z=position[2]) if flag else None
    quest.EntityGetShotStrikePos=query
    actual,out=lua.globals().Query(quest,actor,old)
    assert actual is flag and tuple(out[k] for k in ('x','y','z'))==expected
    assert calls==[True]
    assert lua.eval('function(a,b)return a==b end')(out,old) is (not flag)

@pytest.mark.parametrize('cast',['(C3DVector *)','(C3DVector_bv *)'])
@pytest.mark.parametrize('address',['xStack_c','&xStack_c'])
def test_known_vector_component_slices_use_xyz(cast,address):
    lifter=Lifter(load_manifest(),{},'quest',True,'',RData());lifter.accessor_kinds=True
    assert lifter.interface_call('bVar1','EntityGetShotStrikePos','me,'+cast+address)
    assert [lifter.expr('xStack_c._'+str(i*4)+'_4_') for i in range(3)]==['xStack_c.x','xStack_c.y','xStack_c.z']
    assert lifter.expr('unproven._4_4_')=='unproven._4_4_'
    assert lifter.expr('xStack_c._12_4_')=='xStack_c._12_4_'
    assert lifter.expr('"literal xStack_c._4_4_"')=='"literal xStack_c._4_4_"'

@pytest.mark.parametrize('readable',[False,True])
@pytest.mark.parametrize('responses',[(False,False),(True,False),(False,True),(True,True)])
def test_repeated_queries_preserve_latest_success(readable,responses):
    query='bVar1 = GSI->EntityGetShotStrikePos(me,(C3DVector *)xStack_c);'
    lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(source_for(query+'\n'+query,readable))
    old=lua.table(x=-1,y=-2,z=-3);quest=lua.table();seen=[]
    def invoke(self,actor):
        index=len(seen);seen.append(index)
        return lua.table(x=index+10,y=index+20,z=index+30) if responses[index] else None
    quest.EntityGetShotStrikePos=invoke
    flag,out=lua.globals().Query(quest,lua.table(),old)
    last=next((i for i in (1,0) if responses[i]),None)
    expected=(-1,-2,-3) if last is None else (last+10,last+20,last+30)
    assert flag is responses[-1] and tuple(out[k] for k in ('x','y','z'))==expected
    assert seen==[0,1]

def test_discarded_flag_still_updates_output_and_generated_name_does_not_collide():
    lifter=Lifter(load_manifest(),{},'quest',True,'',RData());lifter.accessor_kinds=True
    lifter.source_names.add('__native_vector_result_1')
    lifter.interface_call(None,'EntityGetShotStrikePos','me,(C3DVector *)xStack_c')
    text='\n'.join(lifter.out)
    assert '__native_vector_result_2' in text and '__native_vector_result_1' not in text
    assert 'xStack_c = {x =' in text

@pytest.mark.parametrize('operands,target',[
    ('me','bVar1'),('me,(int *)xStack_c','bVar1'),('me,(C3DVector *)(xStack_c + 4)','bVar1'),
    ('unknownActor,(C3DVector *)xStack_c','bVar1'),('me,(C3DVector *)not_a_stack_slot','bVar1'),
    ('me,(C3DVector *)xStack_c','xStack_c'),('me,(C3DVector *)xStack_c,42','bVar1'),
])
def test_ambiguous_output_does_not_invent_destination(operands,target):
    lifter=Lifter(load_manifest(),{},'quest',True,'',RData());lifter.accessor_kinds=True
    lifter.interface_call(target,'EntityGetShotStrikePos',operands)
    assert any('unresolved native output vector' in t for t in lifter.todo)
    assert 'quest:EntityGetShotStrikePos(' not in '\n'.join(lifter.out)

@pytest.mark.parametrize('old_kind',['staged','hidden_result'])
def test_occupied_output_alias_fails_closed(old_kind):
    lifter=Lifter(load_manifest(),{},'quest',True,'',RData());lifter.accessor_kinds=True
    if old_kind=='staged':lifter.push_temp('xStack_c','"old"')
    else:lifter.slot_results['xStack_c']='oldThing'
    lifter.interface_call('bVar1','EntityGetShotStrikePos','me,(C3DVector *)xStack_c')
    assert any('unresolved native output vector' in t for t in lifter.todo)

def test_registered_chicken_query_binds_vector_and_flag_separately(tmp_path):
    import json
    from unittest.mock import patch
    from tools.script_recovery.convert_quest_unit import UnitConverter
    root=next(p for p in Path(__file__).resolve().parents if (p/'refs/script_recovery/chicken_kicking').is_dir())
    evidence=root/'refs/script_recovery/chicken_kicking';unit=json.loads((evidence/'units/V_ChickenKicking.json').read_text(encoding='utf-8'))
    entity=unit['entities']['KickedChicken'];entity['functions']={'Main':entity['functions']['Main']};unit['entities']={'KickedChicken':entity};unit['quest']['functions']={};unit['quest']['helpers']={}
    converter=UnitConverter(evidence/'translation_unit_typed.json')
    with patch.object(converter,'constructor_defaults',return_value={}):converter.convert(unit,tmp_path)
    text=(tmp_path/'FSE/V_ChickenKicking/Entities/KickedChicken.lua').read_text(encoding='utf-8')
    assert text.count('quest:EntityGetShotStrikePos(me)')==1
    assert 'bVar1 = (__native_vector_result_1 ~= nil)' in text
    assert 'xStack_c = {x = __native_vector_result_1.x, y = __native_vector_result_1.y, z = __native_vector_result_1.z}' in text
    assert 'xStack_c.y - pfVar6.y' in text
    assert 'quest:IsXbox' not in text
    assert text.count('markerResources:NormalizeVector3(')==2
