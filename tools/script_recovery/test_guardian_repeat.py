"""Repeat-interaction Lua reference versus native replay events and ownership."""
import hashlib,json
from pathlib import Path
import unittest
from lupa import LuaRuntime
ROOT=Path(__file__).resolve().parents[2]
PORT=ROOT/'refs/script_recovery/guardian_trophy_dealer_info'
HARNESS='''function(acquisitions,health,busy,stop_query)
    local h={events={},maze={owned=true},queries=0,attempt=0,hi=0,bi=0}
    local function emit(...) h.events[#h.events+1]={...} end
    function h:terminating()
        self.queries=self.queries+1;local stop=stop_query>0 and self.queries>=stop_query
        emit('query',stop);return stop
    end
    function h:ally(...) emit('ally',...) end
    function h:prepare(r)
        assert(r==self.maze);emit('prepare',r.owned)
        if r.owned then r.owned=false;emit('reset') end
    end
    function h:acquire(r,name,priority)
        assert(r==self.maze and name=='maze' and priority==4)
        self.attempt=self.attempt+1;local v=acquisitions[self.attempt];assert(v~=nil)
        r.owned=v;emit('acquire',v,priority);return v
    end
    function h:release(r) assert(r==self.maze);r.owned=false;emit('release') end
    function h:frame() emit('frame') end
    function h:start_movie(name)
        assert(not self.movie);self.movie={};emit('movie_start',name);return self.movie
    end
    function h:pause(v) emit('pause',v) end
    function h:end_movie(m) assert(m==self.movie);self.movie=nil;emit('movie_end') end
    function h:face_hero(v) emit('face_hero',v) end
    function h:resource_thing(r)
        assert(r==self.maze and not self.thing);self.thing={};emit('resource_thing');return self.thing
    end
    function h:health(t)
        assert(t==self.thing);self.hi=self.hi+1;local v=health[self.hi];assert(v~=nil)
        emit('health',v);return v
    end
    function h:release_thing(t) assert(t==self.thing);self.thing=nil;emit('release_thing') end
    function h:speak(r,...) assert(r==self.maze and r.owned);emit('speak',...) end
    function h:busy(r)
        assert(r==self.maze);self.bi=self.bi+1;local v=busy[self.bi];assert(v~=nil)
        emit('busy',v);return v
    end
    return h
end'''


class GuardianRepeatTests(unittest.TestCase):
    def test_native_cases(self):
        evidence=json.loads((PORT/'runtime_evidence/native-repeat-cases-20260912.json').read_text())
        replay=ROOT/'work/runtime-20260912-video-comparison/prove_guardian_repeat.py'
        self.assertEqual(hashlib.sha256(replay.read_bytes()).hexdigest(),evidence['replay_sha256'])
        self.assertEqual(len(evidence['cases']),22)
        for case in evidence['cases']:
            with self.subTest(case=case['name']):
                lua=LuaRuntime(unpack_returned_tuples=True)
                q=lua.execute((PORT/'reference/quest.lua').read_text())
                h=lua.eval(HARNESS)(*[lua.table_from(case[k]) for k in ('acquisitions','health','busy')],case['stop_query'])
                continued=q.repeat_interaction(h,case['branch'],h.maze)
                self.assertEqual([list(e.values()) for e in h.events.values()],case['events'])
                self.assertEqual(continued,case['continued']);self.assertEqual(h.maze.owned,case['maze_owned'])
                self.assertIsNone(h.movie);self.assertIsNone(h.thing)


if __name__=='__main__':unittest.main()
