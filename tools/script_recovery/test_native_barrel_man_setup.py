import struct
import unittest
from types import SimpleNamespace
from lupa.lua54 import LuaRuntime
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_barrel_man_setup import verify,recover,LOWERED


class BarrelManSetupTests(unittest.TestCase):
    def test_native_setup_arguments_and_retained_copies(self):
        data=RData();witness=verify(data)
        for retained in (False,True):
            for position in ((1.0,2.0,3.0),(-10.5,0.0,45.25)):
                uc=Uc(UC_ARCH_X86,UC_MODE_32)
                for address,size in ((0xDB5000,0x1000),(0x99E000,0x1000),(0x100000,0x10000),(0x200000,0x140000)):
                    uc.mem_map(address,size)
                def put(a,v):uc.mem_write(a,int(v).to_bytes(4,'little'))
                def get(a):return int.from_bytes(uc.mem_read(a,4),'little')
                def floats(a):return tuple(struct.unpack('<fff',uc.mem_read(a,12)))
                actor=0x200008;game=0x300000;info=0x330000 if retained else 0
                put(0x200004,game);put(actor,0x320000);put(actor+4,0x123456);put(actor+8,info)
                if info:put(info,7)
                put(game,0x310000)
                slots={0x934:'brain',0xBEC:'center',0xBF8:'minimum',0xC04:'maximum',0xC40:'stateGroup'}
                targets={0x321000+slot:name for slot,name in slots.items()}
                for slot in slots:put(0x310000+slot,0x321000+slot)
                put(0x32001C,0x322000)
                uc.reg_write(UC_X86_REG_ESI,0x200000);uc.reg_write(UC_X86_REG_EDI,actor)
                uc.reg_write(UC_X86_REG_EBP,0);uc.reg_write(UC_X86_REG_ESP,0x108000)
                live=[];events=[];copies=[]
                def hook(machine,address,size,user):
                    stack=machine.reg_read(UC_X86_REG_ESP);receiver=machine.reg_read(UC_X86_REG_ECX)
                    value=0
                    if address==0x99EBF0:
                        self.assertEqual((get(stack+4),get(stack+8)),(0x12D921C,0xFFFFFFFF))
                        live.append(receiver);pop=8
                    elif address==0x99EAE0:
                        self.assertEqual(live,[receiver]);live.clear();pop=0
                    elif address==0x322000:
                        self.assertEqual(receiver,actor);destination=get(stack+4)
                        machine.mem_write(destination,struct.pack('<fff',*position));value=destination;pop=4
                        events.append(('home',))
                    elif address in targets:
                        name=targets[address];self.assertEqual(receiver,game)
                        if name=='brain':
                            self.assertEqual(get(stack+4),actor);self.assertEqual(live,[get(stack+8)])
                            events.append(('brain','BRAIN_PASSIVE_OVERRIDE'));pop=8
                        else:
                            self.assertFalse(live)
                            self.assertEqual((get(stack+4),get(stack+8),get(stack+12)),(0x1238C8C,0x123456,info))
                            if info:self.assertEqual(get(info),8);put(info,7)
                            copies.append(name)
                            if name=='center':events.append((name,*floats(stack+16)));pop=24
                            elif name=='stateGroup':events.append((name,get(stack+16)));pop=16
                            else:events.append((name,struct.unpack('<f',uc.mem_read(stack+16,4))[0]));pop=16
                    else:return
                    destination=get(stack);machine.reg_write(UC_X86_REG_ESP,stack+4+pop)
                    machine.reg_write(UC_X86_REG_EAX,value);machine.reg_write(UC_X86_REG_EIP,destination)
                uc.hook_add(UC_HOOK_CODE,hook)
                uc.mem_write(witness['address'],data.bytes_at(witness['address'],witness['size']))
                end=witness['address']+witness['size'];uc.emu_start(witness['address'],end,count=200)
                self.assertEqual(uc.reg_read(UC_X86_REG_EIP),end);self.assertEqual(uc.reg_read(UC_X86_REG_ESP),0x108000)
                self.assertEqual(copies,['center','minimum','maximum','stateGroup']);self.assertFalse(live)
                lua=LuaRuntime();lua.globals().position=lua.table_from(dict(zip(('x','y','z'),position)))
                actual=[];lua.globals().record=lambda *row:actual.append(row)
                lua.execute('''
                    me={};quest={}
                    function me:GetHomePos() record('home');return position end
                    function quest:SetCreatureBrain(actor,name) assert(actor==me);record('brain',name) end
                    function quest:SetWanderCentrePoint(actor,p) assert(actor==me);record('center',p.x,p.y,p.z) end
                    function quest:SetWanderMinDistance(actor,d) assert(actor==me);record('minimum',d) end
                    function quest:SetWanderMaxDistance(actor,d) assert(actor==me);record('maximum',d) end
                    function quest:SetScriptingStateGroup(actor,g) assert(actor==me);record('stateGroup',g) end
                ''')
                lua.execute(LOWERED)
                self.assertEqual(events,actual)

    def test_changed_maximum_and_missing_draft_reject(self):
        data=RData();witness=verify(data)
        def changed(address,size):
            raw=data.bytes_at(address,size)
            if address==witness['address']:
                raw=bytearray(raw);raw[0xDB5436-address]^=0x80;return bytes(raw)
            return raw
        with self.assertRaises(ValueError):verify(SimpleNamespace(bytes_at=changed,string_at=data.string_at))
        with self.assertRaises(ValueError):recover(witness['oldLua'].replace('SetWanderMaxDistance','changed'),data)
        output,_=recover(witness['oldLua'],data)
        self.assertEqual(output,LOWERED)
