"""First-encounter reference versus instrumented native execution evidence."""
import hashlib,json
from pathlib import Path
import unittest
from lupa import LuaRuntime
ROOT=Path(__file__).resolve().parents[2]
PORT=ROOT/'refs/script_recovery/guardian_trophy_dealer_info'

HARNESS='''function(acquisitions,stop_query,cancel_macro)
    local h={events={},resources={maze={name='maze',owned=true}},piece=false,
             queries=0,attempt=0,macros=0,cancelled=false}
    local function emit(...) h.events[#h.events+1]={...} end
    function h:terminating()
        self.queries=self.queries+1
        self.cancelled=self.cancelled or (stop_query>0 and self.queries>=stop_query)
        emit('query',self.cancelled);return self.cancelled
    end
    function h:new_resource(name)
        assert(not self.resources[name]);local r={name=name,owned=false}
        self.resources[name]=r;return r
    end
    function h:prepare(r)
        emit('prepare',r.name,r.owned)
        if r.owned then r.owned=false;emit('reset',r.name) end
    end
    function h:acquire(r,actor,priority)
        assert(actor==r.name and priority==4)
        self.attempt=self.attempt+1;local success=acquisitions[self.attempt]
        assert(success~=nil);r.owned=success;emit('acquire',r.name,success,priority);return success
    end
    function h:release(r) r.owned=false;emit('release',r.name) end
    function h:frame() emit('frame') end
    function h:ally(a,b) emit('ally',a,b) end
    function h:new_actor_map() assert(not self.map);self.map={};emit('map_start');return self.map end
    function h:map_actor(m,key,r)
        assert(m==self.map and r.owned and key==string.upper(r.name));m[key]=r
        emit('map_actor',key,r.name)
    end
    function h:start_movie(name) assert(not self.movie);self.movie={};emit('movie_start',name);return self.movie end
    function h:pause(v) emit('pause',v) end
    function h:fix_camera(v) emit('fix_camera',v) end
    function h:macro(name,m,skip)
        assert(self.movie and m==self.map and m.MAZE and m.HERO)
        self.macros=self.macros+1;emit('macro',name,skip)
        if self.macros==cancel_macro then self.cancelled=true end
    end
    function h:end_movie(m) assert(m==self.movie);self.movie=nil;emit('movie_end') end
    function h:end_actor_map(m) assert(m==self.map);self.map=nil;emit('map_end') end
    function h:quest_card(...) emit('quest_card',...) end
    function h:lookup(name) emit('lookup',name);return name end
    function h:teleporter(t,v) assert(t=='WitchwoodTeleporter');emit('teleporter',v) end
    function h:release_lookup(t) assert(t=='WitchwoodTeleporter');emit('release_lookup') end
    function h:clear_information() emit('clear_information') end
    function h:remove_marker() emit('remove_marker') end
    function h:facing(...) emit('facing',...) end
    function h:logbook(...) emit('logbook',...) end
    function h:active() emit('active');return 'QS_GuardianTrophyDealerInfo' end
    function h:complete(...) emit('complete',...) end
    function h:set_piece_over(v) self.piece=v;emit('piece_over',v) end
    return h
end'''


class GuardianEncounterTests(unittest.TestCase):
    def test_native_cases(self):
        evidence=json.loads((PORT/'runtime_evidence/native-encounter-cases-20260912.json').read_text())
        replay=ROOT/'work/runtime-20260912-video-comparison/prove_guardian_encounter.py'
        self.assertEqual(hashlib.sha256(replay.read_bytes()).hexdigest(),evidence['replay_sha256'])
        self.assertEqual(len(evidence['cases']),27)
        for case in evidence['cases']:
            with self.subTest(case=case['name']):
                lua=LuaRuntime(unpack_returned_tuples=True)
                q=lua.execute((PORT/'reference/quest.lua').read_text())
                h=lua.eval(HARNESS)(lua.table_from(case['acquisitions']),case['stop_query'],case['cancel_macro'])
                continued=q.encounter(h,case['branch'],h.resources.maze,1.25)
                self.assertEqual([list(e.values()) for e in h.events.values()],case['events'])
                self.assertEqual(h.piece,case['piece_over'])
                self.assertEqual(continued,case['continued'])
                self.assertEqual(sorted(k for k,r in h.resources.items() if r.owned),case['remaining_resources'])
                self.assertIsNone(h.movie);self.assertIsNone(h.map)


if __name__=='__main__':unittest.main()
