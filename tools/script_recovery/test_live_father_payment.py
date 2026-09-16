import itertools,unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.live_father_payment import SOURCE,recover,generate
from tools.script_recovery.live_father_payment_native import execute,signed
from tools.script_recovery.lift_native_lua import RData

def lua_case(good=1,bad=0,paid=0,gold=3,chocolate=False,health=1.0,busy=0,cancel=999,mutation=None,active_quest='NewOakValeIntro',gold_error=False):
    lua=LuaRuntime();lua.execute(SOURCE);q=lua.table();r=lua.table();state=lua.table();events=[];values={'GoodDeedsPerformed':signed(good),'BadDeedsPerformed':signed(bad),'PenniesGiven':signed(paid)};count={'query':0,'busy':0}
    def get(_,key):events.append(('get',key,values[key]));return values[key]
    def put(_,key,value):values[key]=value;events.append(('set',key,value))
    def term(_):
        count['query']+=1
        if mutation and count['query']==mutation[0]:
            _,g,b,p=mutation;values.update(GoodDeedsPerformed=signed(g),BadDeedsPerformed=signed(b),PenniesGiven=signed(p));events.append(('mutate',signed(g),signed(b),signed(p)))
        value=count['query']>=cancel;events.append(('term',value));return value
    def speak(_,control,hero,key,*flags):assert (control,hero,flags)==(1,8,(0,False,True,False));events.append(('speak',key));count['busy']=busy
    def task(_,control):assert control==1;value=count['busy']>0;count['busy']-=1;events.append(('busy',value));return value
    def possession(_):events.extend([('text.new','OBJECT_CHOCOLATE_BOX_UNGIVEABLE'),('hero',),('chocolate',chocolate),('text.destroy','OBJECT_CHOCOLATE_BOX_UNGIVEABLE')]);return chocolate
    def objective(_,key):
        assert key=='TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_01';events.extend([('text.new',''),('text.new',''),('text.new',key),('quest.new',active_quest),('objective',active_quest,key,'',''),('text.destroy',active_quest),('text.destroy',key),('text.destroy',''),('text.destroy','')])
    q.GetStateInt=get;state.GetStateInt=get;state.SetStateInt=put;q.IsActiveThreadTerminating=term;q.GetHero=lambda _:events.append(('hero',)) or 8;q.NewScriptFrame=lambda _:events.append(('frame',));q.GetHeroGold=lambda _:events.append(('gold',signed(gold))) or signed(gold)
    r.GiveRawHeroGold=lambda _,amount:events.append(('giveGold',amount,values['PenniesGiven']))
    if gold_error:r.GiveRawHeroGold=lua.eval('function(f) return function(...) f(...); error("GOLD ERROR",0) end end')(r.GiveRawHeroGold)
    r.LiveFatherHeroHasChocolate=possession;r.SetActiveQuestObjective=objective;r.ClearRawInformation=lambda _,actor:events.append(('clear',))
    r.NewThingFromResource=lambda _,control:events.append(('thing.new',)) or 9;r.ThingHealth=lambda _,actor:events.append(('health',)) or health;r.DestroyThing=lambda _,actor:events.append(('thing.destroy',));r.Speak=speak;r.IsPerformingScriptTask=task
    try:result=lua.globals().LiveFatherPaymentDialogue(q,7,r,1,state)
    except Exception as error:result=str(error)
    return result,events,values['PenniesGiven']

class LiveFatherPaymentTests(unittest.TestCase):
    def test_original_full_dialogue_state_amount_and_gold_boundaries(self):
        recover();states=((0,0,0),(0,1,0),(1,0,0),(3,2,1),(1,0,1),(1,-1,1),(-1,1,-1),(0,-1,0),(2147483647,0,-2147483648),(0,1,-2147483648),(-2147483648,1,2147483647),(7,-1,3));cases=0
        for counts,gold,chocolate,health,busy,cancel in itertools.product(states,(-1,3,4),(False,True),(0.0,1.0,float('nan')),(0,2),(1,2,3,4,6,999)):
            with self.subTest(counts=counts,gold=gold,chocolate=chocolate,health=health,busy=busy,cancel=cancel):
                args=(*counts,gold,chocolate,health,busy,cancel)
                self.assertEqual(execute(*args),lua_case(*args));cases+=1
        self.assertEqual(cases,2592)

    def test_reload_after_query_and_owned_dynamic_active_quest_name(self):
        for mutation,active in itertools.product(((1,1,0,7),(1,2147483647,-1,-2147483648),(2,0,1,99)),('','OtherActiveQuest')):
            with self.subTest(mutation=mutation,active=active):
                args=dict(good=5,bad=0,paid=2,gold=3,chocolate=False,health=0.0,mutation=mutation,active_quest=active)
                self.assertEqual(execute(**args),lua_case(**args))
        result,events,paid=lua_case(good=3,paid=1,gold_error=True)
        self.assertIn('GOLD ERROR',result);self.assertEqual(paid,3)
        self.assertEqual(events[-2:],[('set','PenniesGiven',3),('giveGold',2,3)])

    def test_changed_subtraction_store_signed_branch_and_health_operand_rejected(self):
        original=RData()
        class Changed:
            def __init__(self,at):self.at=at
            def bytes_at(self,at,size):
                raw=bytearray(original.bytes_at(at,size))
                if at<=self.at<at+size:raw[self.at-at]^=1
                return bytes(raw)
            def string_at(self,at):return original.string_at(at)
        for address in (0xdb8d3c,0xdb8d5a,0xdb8d5e,0xdb8d66,0xdb8d97,0xdb8ffa,0xdb91d8,0xdb921b,0x1260f0c+0x1f8,0x1260f0c+0xa3c,0x1260f0c+0x4a0):
            with self.subTest(address=address),self.assertRaises(ValueError):recover(Changed(address))
        _,report=generate();self.assertEqual(len(report['healthSites']),9)

if __name__=='__main__':unittest.main()
