"""Camera name/thing target overloads must retain their positional operands."""
import copy
import subprocess
import sys
import tempfile
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import Lifter, load_manifest


@pytest.mark.parametrize('camera,kind,setup', [
    ('"CAM_SC_GIRL"', None, ''),
    ('"UNRELATED_CAMERA"', None, ''),
    ('native_arg_camera', 'string', ''),
    ('native_arg_camera', 'thing', ''),
    ('&camera_slot', None, 'CCharString::CCharString((CCharString *)&camera_slot,"CAM_SC_GIRL",-1);\n'),
])
def test_camera_overload_executes_with_all_five_operands(camera, kind, setup):
    manifest = load_manifest()
    original = copy.deepcopy(manifest)
    lifter = Lifter(manifest, {}, 'quest', True, '', None)
    lifter.accessor_kinds = True
    body = '\n'.join(lifter.lift('Camera', '{\n' + setup +
        f'GSI->CameraUseCameraPoint({camera},me,-1.0,0,-1);\n}}',
        parameters={'native_arg_camera': kind} if kind else {}))
    lua = LuaRuntime()
    events = []
    actor, point = lua.table(), lua.table()
    value = point if kind == 'thing' else 'DYNAMIC_CAMERA'
    quest = lua.table_from({'CameraUseCameraPoint': lambda _, *args: events.append(args)})
    lua.execute('return function(quest,me,native_arg_camera)\n' + body + '\nend')(quest, actor, value)
    assert len(events) == 1
    actual, target, duration, flag1, flag2 = events[0]
    equal = lua.eval('function(a,b) return a==b end')
    expected = value if kind else ('UNRELATED_CAMERA' if 'UNRELATED' in camera else 'CAM_SC_GIRL')
    assert equal(actual, expected) and equal(target, actor)
    assert (duration, flag1, flag2) == (-1, 0, -1)
    assert manifest == original


def test_ambiguous_camera_operand_keeps_missing_diagnostic():
    lifter = Lifter(load_manifest(), {}, 'quest', True, '', None)
    lifter.accessor_kinds = True
    body = '\n'.join(lifter.lift('Camera', '{\nGSI->CameraUseCameraPoint(unknown_camera,me,-1.0,0,-1);\n}'))
    assert 'nil --[[missing]]' in body


@pytest.fixture(scope='module')
def sister_sources():
    from tools.script_recovery.build_readable_unit import build
    root = Path(__file__).resolve().parents[2]
    with tempfile.TemporaryDirectory(prefix='sister_camera_', dir=root / 'work') as temp:
        out = Path(temp)
        subprocess.run([sys.executable, '-m', 'tools.script_recovery.convert_quest_unit',
                        '--unit', 'sick_child', '--out', str(out / 'draft')],
                       cwd=root, check=True, capture_output=True)
        build('sick_child', draft=out / 'draft', out=out / 'readable')
        yield [(out / stage / 'FSE/V_SickChild/Entities/SickChildsSister.lua').read_text()
               for stage in ('draft', 'readable')]


@pytest.mark.parametrize('mode', ['normal', 'host_abort', 'caller_abort', 'issue_failure'])
def test_generated_sister_talk_camera_and_movie_cleanup(sister_sources, mode):
    for source in sister_sources:
        lua = LuaRuntime()
        lua.execute('require=function() return {} end')
        lua.execute(source)
        lua.globals().mode = mode
        result = lua.execute('''
            local me,hero,resource,movie={},{},{},{}
            local q,r,events={},{},{}
            local speaking,terminated=false,false
            local callerSpeechFrames,hostSpeechFrames=0,0
            local stop={}
            local function event(s) events[#events+1]=s end
            function q:GetHero() return hero end
            function q:RetailResources() return r end
            function q:NewScriptFrame(v)
                assert(v==me)
                if speaking and mode=='caller_abort' then
                    callerSpeechFrames=callerSpeechFrames+1;terminated=true
                end
                return not terminated
            end
            function q:IsActiveThreadTerminating() return terminated end
            function q:GetStateBool(k) assert(k=='MotherIntroDone'); return false end
            function q:RegisterTimer() return 42 end
            function q:DeregisterTimer(v) assert(v==42);event('timer-release') end
            function q:FixMovieSequenceCamera(v) event('fix:'..tostring(v)) end
            function q:CameraUseCameraPoint(name,target,duration,a,b)
                assert(name=='CAM_SC_GIRL' and target==me and duration==-1 and a==0 and b==-1)
                event('camera')
            end
            function q:PauseAllNonScriptedEntities(v) event('pause:'..tostring(v)) end
            function q:GetHealth(v) assert(v==me);return 1 end
            function q:GetThingWithScriptName(k) assert(k=='SickChildsMother');error(stop) end
            function r:NewResource() return resource end
            function r:PrepareResource(v) assert(v==resource) end
            function r:TryAcquire(v,actor,priority) assert(v==resource and actor==me and priority==4);return true end
            function r:ReleaseResource(v) assert(v==resource);event('resource-release') end
            function r:ScriptThing(v) assert(v==resource);return me end
            function r:StartMovie(k) assert(k=='');event('movie-start');return movie end
            function r:DestroyMovie(v) assert(v==movie);event('movie-destroy') end
            function me:FollowThing(v,d,b) assert(v==hero and d==1 and b) end
            function me:StopFollowingThing(v) assert(v==hero) end
            function me:IsTalkedToByHero() return true end
            function me:Speak(v,key,selection,a,b,c)
                assert(v==hero and key=='TEXT_QST_B10_SISTER_TOLD_TO_FOLLOW_20')
                assert(selection==0 and a==false and b==true and c==false)
                speaking=true;event('speech')
                -- The registered host method returns bool after its blocking wait.
                if mode=='host_abort' then hostSpeechFrames=1;terminated=true;return false end
                if mode=='issue_failure' then return false end
                return true
            end
            function me:IsPerformingScriptTask() return mode=='caller_abort' and not terminated end
            local ok,err=pcall(Main,q,me)
            assert((mode~='normal' and ok) or (mode=='normal' and err==stop),tostring(err))
            assert(callerSpeechFrames==(mode=='caller_abort' and 1 or 0))
            assert(hostSpeechFrames==(mode=='host_abort' and 1 or 0))
            return table.concat(events,',')
        ''')
        expected = 'fix:true,camera,movie-start,pause:true,speech,'
        expected += ('pause:false,movie-destroy,timer-release,resource-release' if mode!='normal'
                     else 'fix:false,pause:false,movie-destroy')
        assert result == expected
