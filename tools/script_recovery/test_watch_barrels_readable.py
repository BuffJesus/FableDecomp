import tempfile
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.build_readable_new_oakvale import build


class WatchBarrelsReadableTests(unittest.TestCase):
    def test_emitted_snapshot_rewards_and_cleanup(self):
        with tempfile.TemporaryDirectory() as directory:
            report=build(Path(directory))
            self.assertTrue(report['syntax']['ok'])
            self.assertEqual(report['watchBarrels']['implementation'],'watchBarrelsWithSnapshot')
            text=(Path(directory)/'FSE/NewOakValeIntro/NewOakValeIntro.lua').read_text()
            start=text.index('local function processBarrelBreaks')
            body=text[start:text.index('\nfunction WatchForGotGold(',start)]
            for token in ('TODO','LAB_','goto '):self.assertNotIn(token,body)
            lua=LuaRuntime();lua.execute(body)
            lua.execute('''
                local frames, closed, gold, beetles, deeds = 0,0,0,0,0
                local snapshot = {
                    Refresh=function() return 1 end,
                    Count=function() return 4 end,
                    Close=function() closed=closed+1 end,
                }
                local resources = {
                    NewBarrelWatchSnapshot=function() return snapshot end,
                    RewardRemainingBarrel=function() gold=gold+1 end,
                    SpawnBarrelBeetle=function(_,point)
                        assert(point.x==1 and point.y==2 and point.z==3);beetles=beetles+1
                    end,
                }
                local q = {
                    WithRetailResources=function(_,fn) fn(resources) end,
                    NewScriptFrame=function() frames=frames+1 end,
                    IsActiveThreadTerminating=function() return false end,
                    SetStateBool=function(_,key,value) assert(key=='BarrelBrokenInstantaneous' and value==false) end,
                    GetStateBool=function(_,key)
                        if key=='AttackOver' then return frames==4 end
                        assert(key=='BarrelBrokenInstantaneous');return true
                    end,
                    GetStateFloat=function(_,key) return ({BarrelBrokenPos_x=1,BarrelBrokenPos_y=2,BarrelBrokenPos_z=3})[key] end,
                }
                AddBadDeed=function(quest,deed) assert(quest==q and deed==0);deeds=deeds+1 end
                WatchBarrels(q)
                assert(frames==4 and closed==1 and gold==1 and beetles==2 and deeds==1)
                frames=0;closed=0
                snapshot.Refresh=function() error('refresh failed',0) end
                snapshot.Close=function() closed=closed+1;error('close failed',0) end
                local ok,err=pcall(WatchBarrels,q)
                assert(not ok and err=='refresh failed' and closed==1)
            ''')
