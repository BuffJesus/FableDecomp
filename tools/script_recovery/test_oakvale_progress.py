import itertools
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_oakvale_progress import execute


def run(kind,initial=0,delay=1,cancel=99,alias=0,mutate=False,source=None):
    lua=LuaRuntime();lua.execute(source or Path(__file__).with_name('oakvale_progress_bodies.lua').read_text())
    q,r=lua.table(),lua.table();events=[];queries=0;gold_calls=0
    def event(*args):events.append(args)
    def gold(_):
        nonlocal gold_calls
        value=initial if gold_calls<delay else 3;gold_calls+=1;value=value&0xffffffff;value=value if value<2**31 else value-2**32
        event('gold',value);return value
    def term(_):
        nonlocal queries
        queries+=1;value=queries>=cancel;event('term',value);return value
    def wrapped(name,key):event('key.new',key);event(name,key);event('key.close',key)
    def objective(_,hasGold):
        key='TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_'+('03' if hasGold else '06')
        for value in ('','',key):event('key.new',value)
        event('active');event('objective',key,'','',alias,mutate)
        for value in ('active',key,'',''):event('key.close',value)
    q.GetHeroGold=gold;q.IsActiveThreadTerminating=term;q.NewScriptFrame=lambda _:event('frame')
    q.WithRetailResources=lambda _,body:body(r);r.SetOakvaleProgressObjective=objective
    q.ActivateQuest=lambda _,key:wrapped('activate',key);q.DeactivateQuest=lambda _,key,zero:wrapped('deactivate',key)
    q.TransitionToTheme=lambda _,key,zero:wrapped('theme',key);q.SetTimeOfDay=lambda _,value:event('time',value)
    (lua.globals().WatchForGotGold if kind=='gold' else lua.globals().AttackStuff)(q)
    return events


class OakvaleProgressTests(unittest.TestCase):
    def test_complete_gold_watcher_signed_gold_cancellation_and_objective(self):
        for initial,delay,cancel in itertools.product((-2147483648,-1,0,2,3,2147483647),(0,1,3),(1,2,3,5,99)):
            args=dict(initial=initial,delay=delay,cancel=cancel)
            self.assertEqual(run('gold',**args),execute('gold',**args),args)

    def test_objective_alias_and_captured_table(self):
        for kind,alias,mutate in itertools.product(('gold','attack'),range(3),(False,True)):
            args=dict(initial=3,alias=alias,mutate=mutate)
            self.assertEqual(run(kind,**args),execute(kind,**args),(kind,args))


if __name__=='__main__':unittest.main()
