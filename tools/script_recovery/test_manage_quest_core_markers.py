"""Compare complete native instructions with emitted Lua, including counted cleanup."""
import itertools,unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.manage_quest_core_markers import SOURCE,prove,generate
from tools.script_recovery.manage_quest_core_markers_native import execute
from tools.script_recovery.lift_native_lua import RData

def run(cancel=99,delays=(0,0,0,0,0),tutorial=True,counts=(1,1,1),populated=(True,True,True),gold=2,source=SOURCE):
    lua=LuaRuntime();q,r=lua.table(),lua.table();events=[];live=[];polls=[0]*5;queries=0
    names=('father','trader','theresa');keys=('NOVI_LiveFather','NOVI_BookTrader','NOVI_Theresa')
    def event(*a):events.append(a)
    def poll(i):v=polls[i]>=delays[i];polls[i]+=1;return v
    def term(_):
        nonlocal queries
        queries+=1;v=queries>=cancel;event('term',v);return v
    def lookup(_,key):
        i=keys.index(key);event('key.new',key);event('lookup',names[i],populated[i]);event('key.destroy',key);live.append(i);return i+1
    def destroy(_,id):
        i=id-1;assert i in live;live.remove(i)
        if counts[i]==1:event('object.destroy',names[i]);event('info.free',names[i])
        event('destroy',names[i])
    def add(_,id):
        assert id-1 in live
        event('key.new','HUD_ORB_QUEST_CORE');event('add',names[id-1]);event('key.destroy','HUD_ORB_QUEST_CORE')
    def state(_,key):
        i={'GivenSweets':3,'GivenTheresaChocs':4}[key];v=poll(i);event(('sweets','chocs')[i-3],v);return v
    def getgold(_):v=3 if poll(0) else gold;event('gold',v);return v
    def controlled(_):v=poll(1);event('controlled',v);return v
    def clicked(_):v=poll(2);event('clicked',v);return v
    def display(_,id):assert id==19;event('tutorial',tutorial);return tutorial
    q.WithRetailResources=lambda _,body:body(r);q.NewScriptFrame=lambda _:event('frame');q.IsActiveThreadTerminating=term
    q.GetHeroGold=getgold;q.IsHeroControlledByPlayer=controlled;q.MsgIsTutorialClickedPast=clicked;q.DisplayTutorial=display;q.GetStateBool=state
    r.NewThingFromScriptName=lookup;r.DestroyThing=destroy;r.AddCoreQuestMarker=add;r.RemoveCoreQuestMarker=lambda _,id:event('remove',names[id-1])
    lua.execute(source);lua.globals().ManageQuestCoreMarkers(q);assert not live;return events

class ManageQuestCoreMarkersTests(unittest.TestCase):
    def test_full_original_function_wait_and_cancellation_paths(self):
        for cancel,delays,tutorial in itertools.product(range(1,21),itertools.product((0,2),repeat=5),(False,True)):
            args=dict(cancel=cancel,delays=delays,tutorial=tutorial)
            self.assertEqual(run(**args),execute(**args),args)

    def test_real_destructors_refcounts_empty_outputs_and_signed_gold(self):
        for counts,populated,cancel in itertools.product(itertools.product((0,1,2),repeat=3),itertools.product((False,True),repeat=3),(1,99)):
            args=dict(counts=counts,populated=populated,cancel=cancel,delays=(2,0,0,0,0))
            self.assertEqual(run(**args),execute(**args),args)
        for gold in (-2147483648,-1,0,2,3,2147483647):
            args=dict(gold=gold,delays=(2,0,0,0,0));self.assertEqual(run(**args),execute(**args),args)

    def test_native_changes_rejected_and_cancel_does_not_remove_current_marker(self):
        data=RData()
        class Mutated:
            def bytes_at(self,address,size):
                result=data.bytes_at(address,size)
                return bytes([result[0]^1])+result[1:] if address==0xdbe4e0 else result
            def string_at(self,address):return data.string_at(address)
        with self.assertRaisesRegex(ValueError,'native bytes changed'):prove(Mutated())
        trace=run(cancel=1,delays=(2,0,0,0,0))
        self.assertEqual([x for x in trace if x[0] in ('add','remove')],[('add','father')])
        self.assertEqual([x[1] for x in trace if x[0]=='destroy'],['theresa','trader','father'])
        changed=SOURCE.replace('return not quest:IsActiveThreadTerminating()','return true')
        self.assertNotEqual(run(source=changed),execute())

if __name__=='__main__':unittest.main()
