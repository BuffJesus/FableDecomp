import itertools
import struct
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_oakvale_deed_helpers import execute


def run(kind,good=0,bad=0,done=False,gold=0,sweets=False,cancel=99,delay=0,deed=2,amount=0.125,source=None):
    lua=LuaRuntime();bodies=lua.execute(source or Path(__file__).with_name('oakvale_deed_bodies.lua').read_text());q,r=lua.table(),lua.table()
    events=[];state=dict(GoodDeedsPerformed=good,BadDeedsPerformed=bad,GUIGoodDeedCounter=-9,GivenSweets=sweets)
    state['WhichBadDeedsPerformed_'+str(deed)]=done;queries=0;clicks=0
    def event(*args):events.append(args)
    def term(_):
        nonlocal queries
        queries+=1;value=queries>=cancel;event('term',value);return value
    def clicked(_):
        nonlocal clicks
        value=clicks>=delay;clicks+=1;event('clicked',value);return value
    def wrapped(name,key):event('key.new',key);event(name,key);event('key.close',key)
    def morality(_,positive):
        event('counts',state['GoodDeedsPerformed'],state['BadDeedsPerformed'])
        value=struct.unpack('<f',struct.pack('<f',amount))[0];event('morality',struct.pack('<f',value if positive else -value).hex())
    def objective(_):
        for key in ('','','TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_02'):event('key.new',key)
        event('active');event('objective')
        for key in ('active','TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_02','',''):event('key.close',key)
    def record(_,key,value):assert key=='WhichBadDeedsPerformed_'+str(deed) and value;state[key]=value;event('bad.record',deed)
    q.WithRetailResources=lambda _,body:body(r);q.GetStateInt=lambda _,key:state[key];q.SetStateInt=lambda _,key,value:state.update({key:value})
    q.GetStateBool=lambda _,key:state[key];q.SetStateBool=record;q.IsActiveThreadTerminating=term;q.MsgIsGameInfoClickedPast=clicked
    q.NewScriptFrame=lambda *_:event('frame');q.AddLogbookTutorialEntry=lambda _,key:wrapped('tutorial',key)
    q.GetHeroGold=lambda _:(event('gold',gold),gold)[1];q.UpdateQuestInfoCounter=lambda _,id,value,maximum:event('counter',value)
    r.ApplyOakvaleDeedMorality=morality;r.DisplayRawGameInfo=lambda _,key:wrapped('info',key);r.SetOakvaleDeedObjective=objective
    if kind=='good':(bodies.good or bodies.AddGoodDeed)(q,None)
    else:(bodies.bad or bodies.AddBadDeed)(q,None,deed)
    return events


class OakvaleDeedHelperTests(unittest.TestCase):
    def test_whole_native_helper_messages_cancellation_and_deed_bookkeeping(self):
        for kind,counts,done,gold,sweets,cancel,delay in itertools.product(('good','bad'),((0,0),(1,0),(0,1),(-1,-1)),(False,True),(2,3),(False,True),(1,2,3,5,99),(0,2)):
            options=dict(good=counts[0],bad=counts[1],done=done,gold=gold,sweets=sweets,cancel=cancel,delay=delay)
            self.assertEqual(run(kind,**options),execute(kind,**options),(kind,options))

    def test_signed_counter_wrap_and_live_morality_values(self):
        for kind,value,amount in itertools.product(('good','bad'),(-2147483648,2147483647),(0.0,-0.0,0.001,0.125,-2.5)):
            options=dict(good=value,bad=value,amount=amount)
            self.assertEqual(run(kind,**options),execute(kind,**options),(kind,options))


if __name__=='__main__':unittest.main()
