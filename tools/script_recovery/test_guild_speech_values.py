"""GuildMaster speech: execute retail control flow and compare recovered Lua."""
import json
import os
from pathlib import Path
import re
import struct
import subprocess
import sys
import tempfile

import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_string_values import fold_constructor_copy, fold_rep_equality

ROOT = Path(__file__).resolve().parents[2]
PREFIX = 'TEXT_QST_081_'


class NativeSpeech:
    """Execute retail instructions; mock string primitives, random and quest API."""
    def __init__(self):
        import unicorn as uc
        from unicorn import x86_const as x
        self.x = x
        self.vm = uc.Uc(uc.UC_ARCH_X86, uc.UC_MODE_32)
        self.image = RData()
        self.primitives = {0x99ebf0:8, 0x99ec30:4, 0x99e4b0:0, 0x99eae0:0,
                           0x99efe0:4, 0x99efb0:4, 0x411570:0, 0xbfeb16:0,
                           0xcb7940:0, 0x600100:4, 0x600110:4, 0x600120:0}
        for base,size in [(0xe91000,0x2000),(0x200000,0x4000),(0x300000,0x4000),
                          (0x400000,0x1000),(0x500000,0x10000),(0x600000,0x1000)]:
            self.vm.mem_map(base,size)
        for page in {a & ~4095 for a in self.primitives} - {0x600000}:
            self.vm.mem_map(page,4096)
        self.vm.mem_write(0xe91000,self.image.bytes_at(0xe91000,0x2000))
        for address,purge in self.primitives.items():
            self.vm.mem_write(address,b'\xc2'+struct.pack('<H',purge))
        self.vm.hook_add(uc.UC_HOOK_CODE,self.hook)

    def word(self, address):
        return struct.unpack('<I',self.vm.mem_read(address,4))[0]

    def put(self,address,value):
        self.vm.mem_write(address,struct.pack('<I',value & 0xffffffff))

    def rep(self,text):
        if text is None: return 0
        address=self.next_rep; self.next_rep += 0x200
        data=text.encode()+b'\0'
        self.vm.mem_write(address+16,data)
        self.put(address,address+16); self.put(address+4,len(data)-1)
        self.strings[address]=text; self.buffers[address+16]=text
        return address

    def hook(self,vm,address,size,data):
        if address not in self.primitives: return
        x=self.x; dest=vm.reg_read(x.UC_X86_REG_ECX)
        esp=vm.reg_read(x.UC_X86_REG_ESP); arg=self.word(esp+4)
        result=dest
        if address in (0x99ebf0,0x99efe0):
            self.put(dest,self.rep(self.image.string_at(arg)))
        elif address in (0x99ec30,0x99efb0):
            self.put(dest,self.word(arg))
            if dest == 0x200050: self.events.append(('cache',self.strings.get(self.word(dest),'')))
        elif address == 0x99e4b0: self.put(dest,0)
        elif address == 0x99eae0: self.destroyed += 1
        elif address == 0x411570:
            left=self.buffers[dest]; right=self.buffers[vm.reg_read(x.UC_X86_REG_EDX)]
            result=(left>right)-(left<right)
        elif address == 0xbfeb16:
            self.events.append(('random',)); result=self.config.get('random',1)
        elif address == 0xcb7940:
            self.events.append(('terminating',)); result=self.config.get('terminating',False)
        elif address in (0x600100,0x600110):
            key='completed' if address == 0x600100 else 'active'
            self.events.append((key,self.strings[self.word(arg)])); result=self.config.get(key,False)
        elif address == 0x600120:
            self.events.append(('title',)); result=self.config.get('title',19)
        vm.reg_write(x.UC_X86_REG_EAX,int(result)&0xffffffff)

    def run(self,config):
        self.config=config; self.next_rep=0x500000;self.strings={};self.buffers={};self.events=[];self.destroyed=0
        self.vm.mem_write(0x200000,b'\0'*0x4000)
        self.put(0x200044,0x201000);self.put(0x201004,config['phase'])
        self.put(0x201070,config.get('souls',0));self.vm.mem_write(0x201060,bytes([config.get('seen',False)]))
        self.put(0x200040,0x202000);self.put(0x202000,0x203000)
        for offset,target in [(0x478,0x600100),(0x470,0x600110),(0x334,0x600120)]: self.put(0x203000+offset,target)
        self.put(0x200050,self.rep(config.get('cache')))
        for index in range(1,5): self.vm.mem_write(0x200048+index,bytes([config.get('flags',{}).get(index,False)]))
        self.vm.mem_write(0x303000,struct.pack('<II',0x400000,0x200100))
        self.vm.reg_write(self.x.UC_X86_REG_ESP,0x303000)
        self.vm.reg_write(self.x.UC_X86_REG_ECX,0x200000)
        self.vm.emu_start(0xe91f20,0x400000,count=10000)
        assert self.vm.reg_read(self.x.UC_X86_REG_EIP)==0x400000
        assert self.vm.reg_read(self.x.UC_X86_REG_ESP)==0x303008
        assert self.vm.reg_read(self.x.UC_X86_REG_EAX)==0x200100
        return {'result':self.strings.get(self.word(0x200100),''),
                'cache':self.strings.get(self.word(0x200050)),
                'flags':{i:bool(self.vm.mem_read(0x200048+i,1)[0]) for i in range(1,5)},
                'events':self.events}


@pytest.fixture(scope='module')
def native():
    # Keep Unicorn's handled Windows memory probes outside pytest's fault handler.
    # The worker executes the same retail bytes and fails on a nonzero exit.
    process=subprocess.Popen([sys.executable,'-m','tools.script_recovery.test_guild_speech_values','--native-worker'],cwd=ROOT,
                             stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,
                             text=True,encoding='utf-8',bufsize=1)
    class Proxy:
        def run(self,config):
            process.stdin.write(json.dumps(config)+'\n');process.stdin.flush()
            line=process.stdout.readline()
            assert line, process.stderr.read()
            result=json.loads(line)
            result['events']=[tuple(e) for e in result['events']]
            result['flags']={int(k):v for k,v in result['flags'].items()}
            return result
    try:
        yield Proxy()
    finally:
        process.stdin.close()
        code=process.wait(timeout=30)
        assert code==0,process.stderr.read()


@pytest.fixture(scope='module')
def generated():
    explicit=os.environ.get('GUILD_SPEECH_TEST_ARTIFACT')
    if explicit:
        yield Path(explicit).resolve(); return
    with tempfile.TemporaryDirectory(prefix='guild_speech_',dir=ROOT/'work') as directory:
        p=Path(directory)
        subprocess.run([sys.executable,'-m','tools.script_recovery.convert_quest_unit','--unit','guild_master_village','--out',str(p/'draft')],cwd=ROOT,check=True,capture_output=True)
        code='from pathlib import Path; import sys; from tools.script_recovery.build_readable_unit import build; p=Path(sys.argv[1]); build("guild_master_village",draft=p/"draft",out=p/"readable")'
        subprocess.run([sys.executable,'-c',code,str(p)],cwd=ROOT,check=True,capture_output=True)
        yield p


def lua_run(generated,stage,filename,config):
    lua=LuaRuntime();events=[];state={'cache':config.get('cache'),'flags':{i:config.get('flags',{}).get(i,False) for i in range(1,5)}}
    def random(*args): events.append(('random',));return config.get('random',1)
    lua.globals().math.random=random
    def get_master(_self,key):
        return {'PostSavePosition':config['phase'],'CurrentHeroSoulsPosition':config.get('souls',0),'SeenAbbeyMotherAtGuild':config.get('seen',False)}[key]
    def get_flag(_self,key): return state['flags'][int(key[-1])]
    def set_flag(_self,key,value): state['flags'][int(key[-1])]=value
    def set_cache(_self,key,value): state['cache']=value;events.append(('cache',value))
    def query(key,arg=None):
        events.append((key,) if arg is None else (key,arg));return config.get(key,19 if key=='title' else False)
    quest=lua.table_from({'GetMasterGameState':get_master,'GetStateString':lambda *_:state['cache'],
        'SetStateString':set_cache,'GetStateBool':get_flag,'SetStateBool':set_flag,
        'IsQuestCompleted':lambda _self,key:query('completed',key),'IsQuestActive':lambda _self,key:query('active',key),
        'IsActiveThreadTerminating':lambda *_:query('terminating'),'GetHeroTitle':lambda *_:query('title')})
    module=lua.execute((generated/stage/'FSE/V_GuildMaster'/filename).read_text()) or lua.globals()
    name='helper_E91F20' if stage=='draft' else 'GetGuildMasterSpeech'
    result=module[name](quest,lua.table())
    return dict(result=result,cache=state['cache'],flags=state['flags'],events=events)


CASES = ([{'phase':p} for p in [0,100,150,200,300,400,450,500,550,600,700,733,766,800,850,856,865,870,875,900,1000,1050,1100,1200,1216,1233,1250,1300,1450,1500,1550,1600,1700,2100,2300,2400,2600,9999]]
 + [{'phase':2500,'souls':s,'terminating':t} for s in [-1,*range(10)] for t in (False,True)]
 + [{'phase':p,'completed':v,'active':v,'seen':v,'terminating':t} for p in (600,800,1300) for v in (False,True) for t in (False,True)]
 + [{'phase':100,'cache':cache,'random':r,'flags':flags,'title':title} for cache in (None,'',PREFIX+'TRAINING','XEXT_QST_081_TRAINING') for r in (0,1,4,32767) for flags,title in [({},19),({2:True},19),({2:True,3:True},19),({2:True,3:True,4:True},19),({2:True,3:True,4:True},20),({1:True,2:True,3:True,4:True},19)]])


@pytest.mark.parametrize('config',CASES)
@pytest.mark.parametrize('stage', ['draft','readable'])
@pytest.mark.parametrize('filename',['native_quest_helpers.lua','V_GuildMaster.lua'])
def test_speech_matches_retail(native,generated,stage,filename,config):
    assert lua_run(generated,stage,filename,config)==native.run(config)


CTOR = """  CCharString *other;
  other = CCharString::CCharString((CCharString *)&local_4,"",-1);
  CCharString::CCharString((CCharString *)&local_c,other);
"""
EQUALITY = 'if ((a == b) || ((((a != (CCharString)0x0 && (b != (CCharString)0x0)) && (*(int *)((int)a + 4) == *(int *)((int)b + 4))) && (temp = CBasicString<char>::Compare(*(void **)a,*(void **)b), temp == 0)))) {'


def test_constructor_return_copy_uses_the_constructed_slot():
    out=fold_constructor_copy(CTOR)
    assert 'other = ' not in out
    assert 'CCharString::CCharString((CCharString *)&local_c,&local_4);' in out


@pytest.mark.parametrize('extra',['Escape(other);','other = unknown;','Escape(&other);'])
def test_constructor_pointer_escape_is_not_guessed(extra):
    text=CTOR+extra
    assert fold_constructor_copy(text)==text


def test_rep_equality_requires_the_complete_short_circuit_shape():
    assert fold_rep_equality(EQUALITY)=='if (ENGINE_StrEq(a, b)) {'
    for old,new in [('temp == 0','temp != 0'),('b + 4','b + 8'),('*(void **)b','*(void **)c'),('(CCharString)0x0','0x1')]:
        text=EQUALITY.replace(old,new)
        assert fold_rep_equality(text)==text


@pytest.mark.parametrize('tail',['\nUse(temp);\n}', '\nif (flag) {\n temp = 1;\n}\nUse(temp);\n}', '\ntemp = temp + 1;\n}', '\nif (flag) goto LAB;\ntemp = 1;\nLAB:\nUse(temp);\n}'])
def test_compare_result_must_be_dead_on_every_later_path(tail):
    text=EQUALITY+tail
    assert fold_rep_equality(text)==text


def test_comparison_temporary_can_be_reused_in_a_nested_block():
    text=EQUALITY+'\nif (flag) {\n temp = GetTitle();\n Use(temp);\n}\n}'
    assert 'ENGINE_StrEq(a, b)' in fold_rep_equality(text)


def test_master_pointer_self_reuse_reads_before_overwriting():
    from tools.script_recovery.native_evidence_lowering import LoweringSpec, lower
    unit=json.loads((ROOT/'refs/script_recovery/guild_master_village/units/V_GuildMaster.json').read_text())
    spec=LoweringSpec(unit,'V_GuildMaster',entity=False,thing_slots={})
    text='int progress;\nprogress = *(int *)(this + 0x44);\nprogress = *(int *)(progress + 0x70);\nUse(progress);'
    out,_=lower(text,spec)
    assert 'progress = GSI->GetMasterGameState("CurrentHeroSoulsPosition");' in out
    assert 'Use(progress);' in out
    text=text.replace('0x70','0x999')
    out,_=lower(text,spec)
    assert 'GetMasterGameState' not in out
    assert '0x999' in out


@pytest.mark.parametrize('stage',['draft','readable'])
@pytest.mark.parametrize('filename',['native_quest_helpers.lua','V_GuildMaster.lua'])
def test_repeated_speech_advances_information_flags_without_repeating_them(native,generated,stage,filename):
    config={'phase':100,'random':0,'title':19}
    results=[]
    for _ in range(11):
        expected=native.run(config)
        actual=lua_run(generated,stage,filename,config)
        assert actual==expected
        results.append(actual['result'])
        config.update(cache=actual['cache'],flags=actual['flags'])
    assert results == [PREFIX+suffix for suffix in ['TRAINING','INFO_TELEPORTERS','TRAINING','INFO_WEAPONS',
        'TRAINING','INFO_LOG_BOOK','TRAINING','INFO_HERO_TITLE','TRAINING','TRAINING','TRAINING']]


def test_reviewed_abi_matches_retail_bytes():
    from tools.script_recovery.infer_helper_prototypes import REVIEWED
    image=RData()
    assert image.bytes_at(0x99ec27,6)==bytes.fromhex('8bc65ec20800')
    assert image.bytes_at(0xbfeb16,6)==bytes.fromhex('ff2524014401')
    for address in (0xe922e2,0xe924e9):assert image.bytes_at(address,3)==bytes.fromhex('c20400')
    assert REVIEWED[0x99ebf0]['ret']=='CCharString *'
    assert REVIEWED[0xbfeb16]['params']==[]
    assert REVIEWED[0xbfeb16]['cc']=='__cdecl'
    assert [p['type'] for p in REVIEWED[0xe91f20]['params']]==['CCharString *']



def test_raw_rep_read_needs_a_recovered_value_consumer():
    from tools.script_recovery.native_string_values import fold_member_rep_read
    member=r'\(CCharString \*\)\(this \+ 0x50\)'
    read='  rep = *(CCharString *)(this + 0x50);\n'
    for tail in ['UsePointer(rep);','if (rep == 0) {','if (ENGINE_StrEq(rep, other)) {\n UsePointer(rep);\n}']:
        text=read+tail
        assert fold_member_rep_read(text,member,'GetValue()')==text
    text=read+'if (ENGINE_StrEq(rep, other)) {\n DoSomething();\n}'
    assert 'rep = GetValue();' in fold_member_rep_read(text,member,'GetValue()')



@pytest.mark.parametrize('offset,field,key',[(0x5c,'RequiredBeard',0x468),(0x5c,'RequiredBeard',0x460),(0x60,'RequiredTash',0x478),(0x60,'RequiredTash',0x470)])
def test_string_state_store_copies_the_recovered_local(offset,field,key):
    from tools.script_recovery.native_evidence_lowering import LoweringSpec,lower,finish_lua
    from tools.script_recovery.lift_native_lua import Lifter,load_manifest
    unit=json.loads((ROOT/'refs/script_recovery/beardy_baldy/units/V_BeardyBaldy.json').read_text())
    spec=LoweringSpec(unit,'BB_BeardyBaldyMan',entity=True,thing_slots={})
    tu=json.loads((ROOT/'refs/script_recovery/beardy_baldy/translation_unit_typed.json').read_text())
    raw=next(f['decompile'] for f in tu['functions'] if int(f['address'],16)==0xe50fd0).replace('\r','')
    reads=re.finditer(r'CDefString::operator_class_CCharString\s*\(\(CDefString \*\)\(DAT_0143e90c \+ '
                      +hex(key)+r'\),\(int\)&(\w+)\);',raw)
    store=None
    for read in reads:
        store=re.match(r'\s*(CCharString::operator=\s*\(\(CCharString \*\)\(\*\(int \*\)\(\(int\)this \+ 0x14\) \+ '
                       +hex(offset)+r'\),&'+re.escape(read[1])+r'\)\s*;)',raw[read.end():])
        if store: break
    assert store
    source=store[1].replace('(int)this','this').replace(read[1],'chosen')
    out,_=lower(source,spec)
    assert f'QUESTSTATE_SetString("{field}", chosen);' in out
    lifter=Lifter(load_manifest(),{},'entity',False,'',RData())
    lifter.locals.add('chosen');lifter.kinds['chosen']='string'
    lifter.statement('Main',out.strip())
    assert not lifter.todo
    lua=LuaRuntime();stores=[]
    quest=lua.table_from({'SetStateString':lambda _self,name,value:stores.append((name,value))})
    text='hair_'+hex(key)
    lua.globals().quest=quest;lua.globals().chosen=text
    lua.execute(finish_lua('\n'.join(lifter.out)))
    assert stores==[(field,text)]


if __name__ == '__main__' and '--native-worker' in sys.argv:
    worker=NativeSpeech()
    for line in sys.stdin:
        config=json.loads(line)
        config['flags']={int(k):v for k,v in config.get('flags',{}).items()}
        print(json.dumps(worker.run(config)),flush=True)
