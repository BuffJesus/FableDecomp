import copy,hashlib,json
from pathlib import Path
import pefile,pytest
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from tools.script_recovery.native_random import recover_raw_retail_random,RAND_THUNK,RAW_MODULUS,RAW_THUNK_BYTES
from tools.script_recovery.lift_native_lua import RETAIL_EXE,ROOT

@pytest.fixture
def native():
    fn={'calls':[dict(currentName='MSVCR71.dll::rand',site='0x1000',target=hex(RAND_THUNK))]}
    data={RAND_THUNK:RAW_THUNK_BYTES,0x1000:b'\xe8'+(RAND_THUNK-0x1005).to_bytes(4,'little',signed=True)}
    return fn,data

@pytest.mark.parametrize('args',['','(int)p0,(int)p1,p2,(int)unaff_EDI','(uint)uVar1','0'])
def test_pure_stale_operands_are_removed_without_moving_the_draw(native,args):
    fn,data=native;source=f'if (ready) {{ value = rand({args}); use(value & 3); }}'
    result,evidence=recover_raw_retail_random(source,fn,lambda address,size:data.get(address))
    assert result=='if (ready) { value = GSI->RetailRandModulo(32768); use(value & 3); }'
    assert evidence[0]['draws']==1

@pytest.mark.parametrize('args',['foo()','++value','*pointer','pointer[0]','value=4','/* retained */','"literal"'])
def test_argument_effects_or_unknown_memory_are_not_discarded(native,args):
    fn,data=native;source=f'value = rand({args});'
    assert recover_raw_retail_random(source,fn,lambda address,size:data.get(address))==(source,[])

@pytest.mark.parametrize('kind',['opcode','target','thunk','duplicate','missing','extra_source'])
def test_raw_draw_requires_complete_native_call_inventory(native,kind):
    fn,data=native;source='value = rand();'
    if kind=='opcode':data[0x1000]=b'\x90'*5
    if kind=='target':fn['calls'][0]['target']='0x1234'
    if kind=='thunk':data[RAND_THUNK]=b'\x90'*6
    if kind=='duplicate':fn['calls']*=2;source+='other = rand();'
    if kind=='missing':fn['calls']=[]
    if kind=='extra_source':source+='other = rand();'
    assert recover_raw_retail_random(source,fn,lambda address,size:data.get(address))==(source,[])


def test_retail_crt_range_makes_the_raw_modulus_exact():
    witness=json.loads(Path(__file__).with_name('native_raw_random_witness.json').read_text())
    dll=RETAIL_EXE.parent/witness['dll']
    assert hashlib.sha256(dll.read_bytes()).hexdigest()==witness['sha256']
    pe=pefile.PE(str(dll));symbol=next(e for e in pe.DIRECTORY_ENTRY_EXPORT.symbols if e.name==b'rand')
    code=[]
    for ins in Cs(CS_ARCH_X86,CS_MODE_32).disasm(pe.get_data(symbol.address,128),pe.OPTIONAL_HEADER.ImageBase+symbol.address):
        code.append((ins.mnemonic,ins.op_str))
        if ins.mnemonic=='ret':break
    assert code[-3:]==[('shr','eax, 0x10'),('and','eax, 0x7fff'),('ret','')]
    assert all(value%RAW_MODULUS==value for value in range(0x8000))


def test_real_beggar_call_sites_and_stale_arguments_are_recovered():
    from tools.script_recovery.convert_quest_unit import UnitConverter
    c=UnitConverter(ROOT/'refs/script_recovery/beggar_and_child/translation_unit_typed.json')
    for address,count in [('0x00e58d40',4),('0x00e5b3b0',1)]:
        fn=c.native(address);source=fn['decompile']
        result,evidence=recover_raw_retail_random(source,fn,c.rdata.bytes_at)
        assert result.count('GSI->RetailRandModulo(32768)')==count
        assert evidence[0]['draws']==count
