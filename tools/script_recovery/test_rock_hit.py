import math
import struct
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.rock_hit import recover
from tools.script_recovery.rock_hit_native import execute
from tools.script_recovery.lift_native_lua import RData


def run(hits=(False,True),abilities=(False,),stop=100,empty=False,health=1.0,bar_id=0x12345678,error=False):
    body,_=recover();lua=LuaRuntime();lua.execute('function Main(quest,troll)\n'+body+'end')
    q,actor,troll=lua.table(),lua.table(),lua.table();events=[];state={'queries':0,'hits':0,'abilities':0,'id':77,'open':False}
    def combined(_,filter):
        assert state['open'] and filter=='SCRIPT_NAME_HERO';events.append(('string.new',filter))
        try:
            if not empty:
                value=hits[min(state['hits'],len(hits)-1)];state['hits']+=1;events.append(('hits',value))
                if value:return True
            events.append(('string.new',filter))
            try:
                if empty:return False
                value=abilities[min(state['abilities'],len(abilities)-1)];state['abilities']+=1;events.append(('abilities',value));return value
            finally:events.append(('string.destroy',filter))
        finally:events.append(('string.destroy',filter))
    actor.MsgIsHitOrAggressiveAbilityFrom=combined
    actor.GetHealth=lambda _:(events.append(('health',)),struct.unpack('<f',struct.pack('<f',health))[0])[1]
    def bar(_,name,color,texture,scale):
        assert state['open'] and name=='RockTrollHealthBarID'
        events.append(('string.new',texture))
        try:
            raw=bytes(color[k] for k in ('b','g','r','a'))
            events.append(('bar',raw.hex(),texture,float(scale)))
            if error:raise RuntimeError('bar error')
            state['id']=bar_id;events.append(('state',bar_id))
        finally:events.append(('string.destroy',texture))
    actor.AddHealthBarToState=bar
    q.NewScriptFrame=lambda _:events.append(('frame',))
    def term(_):state['queries']+=1;result=state['queries']>=stop;events.append(('term',result));return result
    q.IsActiveThreadTerminating=term;q.DisplayQuestInfo=lambda _,flag:events.append(('display',flag))
    def scope(_,source,callback):
        assert lua.eval('rawequal')(source,troll);state['open']=True
        try:callback(actor)
        finally:state['open']=False;events.append(('argument.destroy',))
    q.WithCopiedThreadThing=scope
    try:lua.globals().Main(q,troll)
    except RuntimeError:
        if not error:raise
    assert not state['open'];return events,state['id']


class HitTests(unittest.TestCase):
    def test_native_short_circuit_empty_and_cancellation_traces(self):
        for hits,abilities in [((True,),(True,)),((False,),(True,)),((False,True),(False,)),((False,),(False,))]:
            for stop in (1,2,3,5):
                for empty in (False,True):
                    with self.subTest(hits=hits,abilities=abilities,stop=stop,empty=empty):
                        self.assertEqual(run(hits,abilities,stop,empty),execute(hits,abilities,stop,empty))

    def test_original_x87_ordered_threshold_nan_infinity_and_id_width(self):
        threshold=struct.unpack('<f',bytes.fromhex('17b7d138'))[0]
        below=struct.unpack('<f',bytes.fromhex('16b7d138'))[0]
        above=struct.unpack('<f',bytes.fromhex('18b7d138'))[0]
        for health in (-math.inf,-1,0,below,threshold,above,1,math.inf,math.nan):
            for id in (0x12345678,-2147483648):
                with self.subTest(health=health,id=id):
                    actual=run(hits=(True,),health=health,bar_id=id)
                    self.assertEqual(actual,execute(hits=(True,),health=health,bar_id=id))
                    self.assertEqual(actual[1],id if health>threshold else 77)

    def test_healthbar_state_is_written_inside_texture_scope(self):
        events,id=run(hits=(True,));self.assertEqual(id,0x12345678)
        self.assertEqual(events[-6:],[('string.new','HUD_QUEST_ICON_ROCK_TROLL'),
            ('bar','0000ffff','HUD_QUEST_ICON_ROCK_TROLL',1.0),('state',id),
            ('string.destroy','HUD_QUEST_ICON_ROCK_TROLL'),('display',True),('argument.destroy',)])
        events,id=run(hits=(True,),error=True)
        self.assertEqual(id,77);self.assertEqual(events[-2:],[('string.destroy','HUD_QUEST_ICON_ROCK_TROLL'),('argument.destroy',)])

    def test_native_colour_threshold_filter_and_shortcircuit_mutation(self):
        class Changed(RData):
            def bytes_at(self,address,size):
                raw=super().bytes_at(address,size)
                if address<=self.changed<address+size:
                    raw=bytearray(raw);raw[self.changed-address]^=1;return bytes(raw)
                return raw
        for address in (0xec5177,0xec519b,0x129ba3c,0xec5269,0xec528e,0xec52ad,0xec52bf,0xec5314):
            data=Changed();data.changed=address
            with self.subTest(address=address),self.assertRaises(ValueError):recover(data)


if __name__=='__main__':unittest.main()
