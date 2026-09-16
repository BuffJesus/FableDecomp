import itertools,struct,unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.start_barrel_timer import SOURCE,prove
from tools.script_recovery.start_barrel_timer_native import execute
from tools.script_recovery.lift_native_lua import RData

def run(cancel=99,start=(-1,0,1),spoken=2,near=(True,False),timer_ids=(-1,0,3),bar_ids=(-1,0,17),values=(-2147483648,2147483647),created=-3,count=1,populated=True,source=SOURCE):
    lua=LuaRuntime();q,r=lua.table(),lua.table();events=[];s=dict(query=0,timers=0,start=0,spoken=0,distance=0,bars=0,updates=0,live=False)
    def event(*a):events.append(a)
    def term(_):s['query']+=1;v=s['query']>=cancel;event('term',v);return v
    def get(_,key):
        if key=='WatchTimer':return timer_ids[s['timers']%len(timer_ids)]
        assert key=='GUIBarrelCounter';i=s['bars'];s['bars']+=1;v=bar_ids[i%len(bar_ids)];event('bar.read',v);return v
    def setint(_,key,value):assert key=='GUIBarrelCounter';event('bar.store',value)
    def timer(id,update=False):
        i=s['timers'];s['timers']+=1;assert id==timer_ids[i%len(timer_ids)]
        if update:v=values[s['updates']%len(values)];s['updates']+=1
        else:v=start[min(s['start'],len(start)-1)];s['start']+=1
        event('timer',i%2,id,v);return v
    def add(_,publish):event('key.new','');event('key.new','HUD_CLOCK_ICON');event('add',created);publish(created);event('key.destroy','HUD_CLOCK_ICON');event('key.destroy','')
    def lookup(_,key):assert key=='M_WHouse_GuardPoint';event('key.new',key);event('lookup',populated);event('key.destroy',key);s['live']=True;return 1
    def close(_,id):
        assert id==1 and s['live'];s['live']=False
        if count==1:event('object.destroy');event('info.free')
        event('destroy')
    def nearfn(_,id):assert id==1 and s['live'];i=s['distance'];s['distance']+=1;v=near[i%len(near)];event('hero');event('near',v);return v
    def spokenfn(_,key):assert key=='BarrelManSpokenToHeroOnReturn';v=s['spoken']>=spoken;s['spoken']+=1;event('spoken',v);return v
    def update(_,gettimer,getbar):v=timer(gettimer(),True);id=getbar();event('update',id,struct.unpack('<f',struct.pack('<f',v))[0],-1.,-1.)
    q.WithRetailResources=lambda _,body:body(r);q.NewScriptFrame=lambda _:event('frame');q.IsActiveThreadTerminating=term;q.GetStateInt=get;q.SetStateInt=setint;q.GetStateBool=spokenfn
    r.ReadBarrelWatchTimer=lambda _,id:timer(id);r.AddBarrelTimerBar=add;r.NewThingFromScriptName=lookup;r.DestroyThing=close;r.IsHeroNearBarrelGuard=nearfn
    r.ColourBarrelTimer=lambda _,id,v:event('colour',id,0xff00ff00 if v else 0xffff0000);r.UpdateBarrelTimer=update;r.RemoveBarrelTimer=lambda _,id:event('remove',id)
    lua.execute(source);lua.globals().StartBarrelTimer(q);assert not s['live'];return events

class StartBarrelTimerTests(unittest.TestCase):
    def test_empty_literal_must_be_a_native_zero_byte(self):
        d = RData()
        empty_address = next(int(address, 16) for address, key in prove(d)['strings'].items() if key == '')
        class Changed:
            def bytes_at(self, address, size):
                return b'X' if address == empty_address and size == 1 else d.bytes_at(address, size)
            def string_at(self, address):
                return None if address == empty_address else d.string_at(address)
        with self.assertRaisesRegex(ValueError, 'StartBarrelTimer string changed'):
            prove(Changed())

    def test_whole_native_wait_loop_float_conversion_and_cancellation(self):
        for cancel,start,spoken,near in itertools.product(range(1,11),((1,),(-1,0,1),(-2147483648,1)),(0,1,3),((True,),(False,),(True,False))):
            args=dict(cancel=cancel,start=start,spoken=spoken,near=near);self.assertEqual(run(**args),execute(**args),args)
    def test_empty_retained_things_signed_ids_refcounts_and_timer_rounding(self):
        for count,populated,created,values,cancel in itertools.product((0,1,2),(False,True),(-2147483648,-1,0,2147483647),((0,-1),(16777217,-16777217)),(1,4,99)):
            args=dict(count=count,populated=populated,created=created,values=values,cancel=cancel);self.assertEqual(run(**args),execute(**args),args)
    def test_reject_changes_and_no_invented_hud_cleanup(self):
        d=RData()
        class Changed:
            def bytes_at(self,a,n):
                v=d.bytes_at(a,n);return v[:-1]+bytes([v[-1]^1]) if a==0xdb4f70 else v
        with self.assertRaisesRegex(ValueError,'native bytes changed'):prove(Changed())
        trace=run(cancel=4);self.assertTrue(any(e[0]=='add' for e in trace));self.assertFalse(any(e[0]=='remove' for e in trace))
        self.assertNotEqual(run(source=SOURCE.replace('<= 0','< 0')),execute())
if __name__=='__main__':unittest.main()
