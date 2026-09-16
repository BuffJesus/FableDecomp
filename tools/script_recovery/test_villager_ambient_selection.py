"""Native deed branches, dynamic vector size and post-yield vector reload."""
import itertools
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_villager_conversation_key import verify
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_EDI,UC_X86_REG_EAX,UC_X86_REG_ESI,UC_X86_REG_EBX

VECTORS={0x9c:('good',True),0xcc:('good',False),0xa8:('bad',True),0xd8:('bad',False),
         0xb4:('both',True),0xe4:('both',False),0xc0:('none',True),0xf0:('none',False)}


def native(data,bad,good,sex,cancel,changed):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for address,size in ((0xDAE000,0x1000),(0x99E000,0x2000),(0xF35000,0x1000),(0x100000,0x10000),(0x200000,0x6000)):
        uc.mem_map(address,size)
    uc.mem_write(0xDAE6BC,data.bytes_at(0xDAE6BC,0xDAE98E-0xDAE6BC))
    def put(address,value):uc.mem_write(address,(value&0xFFFFFFFF).to_bytes(4,'little'))
    def get(address):return int.from_bytes(uc.mem_read(address,4),'little')
    stack,actor,thread,game,table,parent=0x108000,0x200000,0x200100,0x200200,0x201000,0x203000
    put(thread+4,game);put(thread+0x14,parent);put(parent+0x54,good);put(parent+0x58,bad);put(game,table)
    sexcall,hero,line=0x202000,0x202010,0x202020
    put(table+0x804,sexcall);put(table+0x118,hero);put(table+0x5b8,line)
    counts={};starts={};selected=[]
    for i,(offset,pair) in enumerate(VECTORS.items()):
        start=0x204000+i*64;starts[offset]=start;counts[offset]=i+2
        put(parent+offset,start);put(parent+offset+4,start+4*(i+2))
    for reg,value in ((UC_X86_REG_ESP,stack),(UC_X86_REG_EDI,actor),(UC_X86_REG_ESI,thread),(UC_X86_REG_EBX,73)):uc.reg_write(reg,value)
    events=[];terms=[];finished=[]
    count_reads={0xDAE707:0x9c,0xDAE744:0xcc,0xDAE7B6:0xa8,0xDAE7F3:0xd8,
                 0xDAE865:0xb4,0xDAE8A2:0xe4,0xDAE901:0xc0,0xDAE93B:0xf0}
    def hook(machine,address,size,user):
        if address in (0xDAEA49,0xDAE98E):finished.append(address==0xDAE98E);machine.emu_stop();return
        if address==0xDAE6BF:events.append('bad')
        if address in (0xDAE6CA,0xDAE779,0xDAE828):events.append('good')
        if address in count_reads:
            offset=count_reads[address];selected.append(offset);category,male=VECTORS[offset]
            events.append(f'count:{category}:{int(male)}:{counts[offset]}')
        if address not in (0xF35B30,sexcall,hero,line,0xDAEBF0,0x99EFB0):return
        esp=machine.reg_read(UC_X86_REG_ESP);receiver=machine.reg_read(UC_X86_REG_ECX)
        args=lambda n:[get(esp+4+i*4) for i in range(n)]
        pop=0;result=0xBADBAD00
        if address==0xF35B30:
            assert receiver==thread;terms.append(1);events.append('term');result=int(len(terms)==cancel)
        elif address==sexcall:assert receiver==game and args(1)==[actor];pop=4;result=sex;events.append('sex')
        elif address==0xDAEBF0:
            offset=selected[-1];assert receiver==thread and args(1)==[counts[offset]]
            events.append('index');pop=4;result=1
            if changed:put(parent+offset,starts[offset]+32)
        elif address==0x99EFB0:
            offset=selected[-1];assert receiver==stack+20 and args(1)==[starts[offset]+(32 if changed else 0)+4]
            category,male=VECTORS[offset];events.append(f'assign:{category}:{int(male)}:1:{int(changed)}');pop=4
        elif address==hero:assert receiver==game;result=0x205000;events.append('hero')
        elif address==line:assert receiver==game and args(5)==[73,stack+20,0,actor,0x205000];pop=20;events.append('line')
        machine.reg_write(UC_X86_REG_EAX,result);machine.reg_write(UC_X86_REG_ESP,esp+4+pop);machine.reg_write(UC_X86_REG_EIP,get(esp))
    uc.hook_add(UC_HOOK_CODE,hook)
    try:uc.emu_start(0xDAE6BC,0xDAEFFF,count=150)
    except Exception as error:raise AssertionError((hex(uc.reg_read(UC_X86_REG_EIP)),events)) from error
    assert len(finished)==1 and uc.reg_read(UC_X86_REG_ESP)==stack
    return finished[0],events


def structured(bad,good,sex,cancel,changed):
    lua=LuaRuntime(unpack_returned_tuples=True)
    lua.globals().body=lua.execute(Path(__file__).with_name('villager_ambient_selection.lua').read_text())
    lua.globals().config=lua.table_from(dict(bad=bad,good=good,sex=sex,cancel=cancel,changed=changed))
    lua.globals().sizes=lua.table_from({category+str(int(male)):i+2 for i,(category,male) in enumerate(VECTORS.values())})
    result,events=lua.execute('''
        local e,terms,changed={},0,false
        local function emit(v) e[#e+1]=v end
        local quest={}
        function quest:GetStateInt(key) local name=key=='BadDeedsPerformed' and 'bad' or 'good';emit(name);return config[name] end
        function quest:IsActiveThreadTerminating() terms=terms+1;emit('term');return terms==config.cancel end
        function quest:EntityGetSex(me) assert(me==9);emit('sex');return config.sex end
        local lists={}
        function lists:Count(category,male)
            local suffix=male and '1' or '0';local n=sizes[category..suffix]
            emit('count:'..category..':'..suffix..':'..n);return n
        end
        local resources={AddVillagerAmbientText=function(_,conversation,key,me)
            assert(conversation==73 and key==7 and me==9);emit('hero');emit('line') end}
        function resources:AssignVillagerSpeechText(key,speechLists,category,male,index)
            assert(speechLists==lists and key==7 and index==1);emit('assign:'..category..':'..(male and '1' or '0')..':1:'..(changed and '1' or '0'))
        end
        local function index(q,me,count) assert(q==quest and me==9 and count>=2);emit('index');changed=config.changed;return 1 end
        return body(quest,9,resources,lists,7,73,index),e
    ''')
    return result,list(events.values())


class VillagerAmbientSelectionTests(unittest.TestCase):
    def test_native_selection_size_and_vector_reload(self):
        data=RData();verify(data)
        for case in itertools.product((-2,0,1,7),(-2,0,1,7),(0,1,2),(0,1,2),(False,True)):
            with self.subTest(case=case):self.assertEqual(structured(*case),native(data,*case))
