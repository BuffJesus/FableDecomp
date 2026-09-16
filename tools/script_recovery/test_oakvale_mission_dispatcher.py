import itertools
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_oakvale_mission_dispatcher import execute


def run(region_delay=0,attack_initial=False,attack_after=2,cancel=99,post_cancel=False,source=None):
    lua=LuaRuntime();lua.execute(source or Path(__file__).with_name('oakvale_mission_body.lua').read_text())
    q,r=lua.table(),lua.table();events=[];frames=0;queries=0;regions=0
    def event(*args):events.append(args)
    def frame(_):
        nonlocal frames
        frames+=1;event('frame')
    def region(_,key):
        nonlocal regions
        assert key=='StartOakVale';value=regions>=region_delay;regions+=1;event('region',value);return value
    def term(_):
        nonlocal queries
        queries+=1;value=queries>=cancel;event('term',value);return value
    def attack(_,key):
        assert key=='AttackOver';value=attack_initial or frames>=attack_after;event('attack.over',value);return value
    q.IsRegionLoaded=region;q.NewScriptFrame=frame;q.IsActiveThreadTerminating=term;q.GetStateBool=attack
    q.WithRetailResources=lambda _,body:body(r)
    q.CreateThread=lambda _,name:event('thread',name);q.CacheMusicSet=lambda _,value:event('cache',value)
    q.ActivateQuest=lambda _,name:event('activate',name)
    q.FadeScreenOutUntilNextCallToFadeScreenIn=lambda _,value,zero:event('fade',value)
    q.SetTimeAsStopped=lambda _,value:event('stop',value);q.SetTimeOfDay=lambda _,value:event('time',value)
    q.SetHeroSleepingAsEnabled=lambda _,value:event('sleeping',value);q.DisplayMoneyBag=lambda _,value:event('money',value)
    r.TurnOakvaleHeroIntoChild=lambda _:event('child');r.PrepareOakvaleHouseAndStartScreen=lambda _:event('house')
    r.SetOakvaleHeroKillable=lambda _,value:event('killable.on' if value else 'killable.off')
    r.FinishOakvaleActiveQuest=lambda _,value:event('deactivate' if value else 'complete')
    lua.globals().AttackStuff=lambda _:event('attack');lua.globals().PostAttackStuff=lambda _:event('post',post_cancel)
    lua.globals().DoMission(q)
    return events


class OakvaleMissionDispatcherTests(unittest.TestCase):
    def test_original_control_flow_and_cancellation_boundaries(self):
        for delay,initial,after,cancel,post in itertools.product((0,2),(False,True),(1,4),(1,2,3,4,5,6,8,99),(False,True)):
            args=dict(region_delay=delay,attack_initial=initial,attack_after=after,cancel=cancel,post_cancel=post)
            self.assertEqual(run(**args),execute(**args),args)


if __name__=='__main__':unittest.main()
