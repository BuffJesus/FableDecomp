import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker

SOURCE = open('refs/script_recovery/lifted/GuildTraining/readable/FSE/GuildTrainingPreMelee/Entities/PreMeleeDummy.lua', encoding='utf-8').read()

class GuildPreMeleeDummyTests(unittest.TestCase):
    def test_modes_hits_and_animation(self):
        lua=LuaRuntime();lua.execute(SOURCE);q=lua.table();me=lua.table();state={'PreMeleeMode':1,'DummyHits':0};events=[];frames=0
        q.EntitySetAsKillable=lambda _,thing,value: events.append(('killable',value))
        q.EntitySetTargetable=lambda _,thing,value: events.append(('targetable',value))
        q.GetStateInt=lambda _,name: state[name]
        q.SetStateInt=lambda _,name,value: state.__setitem__(name,value)
        def frame(_,thing):
            nonlocal frames
            frames+=1
            if frames==1: return True
            state['PreMeleeMode']=2 if frames==2 else 0
            return True
        q.NewScriptFrame=frame;q.IsActiveThreadTerminating=lambda _: False
        me.MsgIsHitByHero=lambda _: (events.append(('hit',)) or True)
        me.MsgIsHitByHeroWithWeapon=lambda _,name: (events.append(('stick',name)) or True)
        q.EntityPlayObjectAnimation=lambda _,thing,name,loop: events.append(('animation',name,loop))
        lua.globals().Init(q,me);lua.globals().Main(q,me)
        self.assertEqual(state['DummyHits'],3)
        self.assertIn(('animation','WOBBLE',False),events);self.assertIn(('animation','GET_HIT_SPIN',False),events)

    def test_lua_syntax(self):
        self.assertTrue(LuaSyntaxChecker().check({'PreMeleeDummy.lua':SOURCE})['ok'])

if __name__ == '__main__': unittest.main()
