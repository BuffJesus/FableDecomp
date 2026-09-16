import itertools,unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.teddy_girl_movement import recover
from tools.script_recovery.teddy_girl_presented_phase import SOURCE as COMMON
from tools.script_recovery.teddy_girl_movement_native import execute

class TeddyGirlMovementTests(unittest.TestCase):
    def test_original_departure_branches_inputs_target_and_cancellation(self):
        source,_=recover()
        for spoke,near,onscreen,far,failures,prepare,cancel in itertools.product((False,True),(False,True),((False,),(True,False)),(False,True),(0,2),(False,True),(1,2,3,4,5,999)):
            with self.subTest(spoke=spoke,near=near,onscreen=onscreen,far=far,failures=failures,prepare=prepare,cancel=cancel):
                lua=LuaRuntime();lua.execute(COMMON+source);q=lua.table();r=lua.table();events=[];count={'query':0,'acquire':0,'screen':0}
                def term(_):count['query']+=1;value=count['query']>=cancel;events.append(('term',value));return value
                def acquire(_,control,actor,priority):
                    self.assertEqual((control,actor,priority),(1,7,4));value=count['acquire']>=failures;count['acquire']+=1;events.append(('acquire',value));return value
                def prep(_,control):
                    events.append(('prepare.test',prepare))
                    if prepare:events.append(('prepare.release',))
                def line(_,conversation,actor,key):
                    self.assertEqual((conversation,actor),(37,7));events.extend([('text.new',key),('hero',),('line',key),('text.destroy',key)])
                def target(_,key):events.extend([('text.new',key),('target.new',key),('text.destroy',key)]);return 9
                def move(_,control,t,radius,kind,wait,*flags):self.assertEqual((control,t,radius,kind,wait,flags),(1,9,3.0,1,None,(False,False,True)));events.append(('move',))
                def screen(_,actor):
                    self.assertEqual(actor,7);value=onscreen[min(count['screen'],len(onscreen)-1)];count['screen']+=1;events.extend([('position',),('screen',value)]);return value
                q.GetStateBool=lambda _,key:events.append(('get',key)) or spoke;q.GetHero=lambda _:events.append(('hero',)) or 8
                q.SetMasterGameState=lambda _,key,value:events.append(('master',value));q.IsActiveThreadTerminating=term;q.NewScriptFrame=lambda _:events.append(('frame',))
                r.IsDistanceUnderThing=lambda _,actor,bully,distance:events.append(('near',near)) or near
                r.NewConversation=lambda _,actor,b1,b2:events.append(('conversation',)) or 37;r.AddRawConversationPerson=lambda _,conversation,actor:events.append(('person',));r.TeddyGirlAddHeroLine=line
                r.PrepareResource=prep;r.TryAcquire=acquire;r.NewThingFromScriptName=target;r.TeddyGirlMoveToThing=move;r.IsActorPositionOnScreen=screen
                r.IsDistanceOver=lambda _,actor,hero,distance:events.append(('far',far)) or far
                r.RemoveRawThing=lambda _,actor,b1,b2:events.append(('remove',));r.DestroyThing=lambda _,target:events.append(('target.destroy',))
                result=lua.globals().TeddyGirlDeparture(q,r,7,1,4)
                self.assertEqual((result,events),execute(spoke,near,onscreen,far,failures,prepare,cancel))

if __name__=='__main__':unittest.main()
