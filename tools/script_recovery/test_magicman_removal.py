"""Fresh Magicman Main removes its own actor when the player owns the bordello."""
from pathlib import Path
import re,subprocess,sys,tempfile
import pytest
from lupa.lua54 import LuaError, LuaRuntime


@pytest.fixture(scope='module')
def sources():
    from tools.script_recovery.build_readable_unit import build
    root=Path(__file__).resolve().parents[2]
    with tempfile.TemporaryDirectory(prefix='magicman_remove_',dir=root/'work') as temp:
        out=Path(temp)
        subprocess.run([sys.executable,'-m','tools.script_recovery.convert_quest_unit','--unit','bordello','--out',str(out/'draft')],cwd=root,check=True,capture_output=True)
        build('bordello',draft=out/'draft',out=out/'readable')
        yield [(out/stage/'FSE/V_Bordello/Entities/Magicman.lua').read_text() for stage in ['draft','readable']]


def execute(source,terminate):
    lua=LuaRuntime();lua.execute('require=function() return {} end');lua.execute(source)
    lua.globals().terminate=terminate
    return lua.execute('''
        local q,r,me,hero,res,member={},{},{},{},{},{}
        local owned,removed,released=false,0,0
        local registered,deregistered=0,{}
        function q:GetHero() return hero end
        function q:RetailResources() return r end
        function q:NewScriptFrame(v) assert(v==me);return true end
        function q:IsActiveThreadTerminating() return terminate and owned end
        function q:GetStateBool(k) assert(k=='PlayerOwned');owned=true;return true end
        function q:RegisterTimer() registered=registered+1;return registered*11 end
        function q:SetTimer(id,value) assert(id==22 and value==40) end
        function q:DeregisterTimer(id) deregistered[#deregistered+1]=id end
        function q:RemoveThing(v,a,b) assert(v==me and a==false and b==true);removed=removed+1 end
        function r:NewResource() return res end
        function r:PrepareResource(v) assert(v==res) end
        function r:TryAcquire(v,actor,priority) assert(v==res and actor==me and priority==4);return true end
        function r:MemberResource(name) assert(name=='seh_Boss');return member end
        function r:AssignResource(a,b) assert(a==member and b==res) end
        function r:ReleaseResource(v) assert(v==res);released=released+1 end
        Main(q,me)
        assert(released==1)
        assert(registered==2 and #deregistered==2)
        assert(deregistered[1]==22 and deregistered[2]==11)
        return removed
    ''')


@pytest.mark.parametrize('terminate',[False,True])
def test_player_owned_removal_and_termination_guard(sources,terminate):
    for source in sources:assert execute(source,terminate)==int(not terminate)


def test_movie_facing_uses_hero_not_outgoing_argument(sources):
    # Exercise each freshly generated opening call setup in both representations.
    # Distinct sentinels make a stale p0 / actor / movie reference observable.
    for source in sources:
        scenes = re.findall(r'quest:FixMovieSequenceCamera\(true\)\s*'
                            r'(.*?)quest:EntitySetFacingAngleTowardsThing\(([^\n]+)\)',
                            source, re.S)
        assert len(scenes) == 6
        for setup, args in scenes:
            lua = LuaRuntime()
            lua.execute('''
                quest,me,hero,p0 = {},{},{},{}
                function quest:GetHero() return hero end
                function quest:EntitySetFacingAngleTowardsThing(actor,target,flag)
                    assert(actor==me and target==hero and flag==false)
                end
            ''')
            lua.execute(setup + 'quest:EntitySetFacingAngleTowardsThing(' + args + ')')


def test_beer_award_uses_native_count_and_flag(sources):
    for source, expected_count in zip(sources, (2, 5)):
        calls = re.findall(r'quest:GiveHeroObject\("OBJECT_BEER_TANKARD",[^\n]+\)', source)
        assert len(calls) == expected_count
        for call in calls:
            lua = LuaRuntime()
            lua.execute('''quest={}; function quest:GiveHeroObject(name,count,flag)
                assert(name=="OBJECT_BEER_TANKARD" and count==-1 and flag==true)
            end''')
            lua.execute(call)


def test_first_health_query_uses_acquired_resource(sources):
    """Drive readable Main through the first movie to its health query."""
    lua = LuaRuntime()
    lua.execute('require=function() return {IsHeroWearingBeard=function() return true end} end')
    lua.execute(sources[1])
    with pytest.raises(Exception, match='HEALTH_PROVEN'):
        lua.execute('''
            local quest,me,hero,resources,held,member,movie,thing={},{},{},{},{},{},{},{}
            function quest:GetHero() return hero end
            function quest:RetailResources() return resources end
            function quest:NewScriptFrame() return true end
            function quest:IsActiveThreadTerminating() return false end
            function resources:NewResource() return held end
            function resources:PrepareResource(v) assert(v==held) end
            function resources:TryAcquire(v,actor,priority)
                assert(v==held and actor==me and priority==4); return true end
            function resources:MemberResource(name) assert(name=='seh_Boss'); return member end
            function resources:AssignResource(a,b) assert(a==member and b==held) end
            function quest:RegisterTimer() return 11 end
            function quest:SetTimer() end
            function quest:GetTimer() return 1 end
            function quest:GetStateBool(name)
                if name=='PlayerOwned' or name=='HeroTricking' or name=='BeerSetToDPad' then return false end
                error('unexpected state '..name)
            end
            function quest:GetHeroTargetedThing() return nil end
            function me:IsDistanceFromPositionOver() return false end
            function me:GetHomePos() return {} end
            function me:IsTalkedToByHero() return true end
            function me:ClearCommands() end
            function resources:StartMovie() return movie end
            function quest:PauseAllNonScriptedEntities() end
            function quest:FixMovieSequenceCamera() end
            function quest:EntitySetFacingAngleTowardsThing() end
            function quest:Pause() end
            function quest:CameraUseCameraPoint() end
            function resources:ScriptThing(value)
                assert(value==held,'health receiver is not the acquired resource'); return thing end
            function quest:GetHealth(value)
                assert(value==thing,'GetHealth has the wrong thing'); error('HEALTH_PROVEN') end
            Main(quest,me)
        ''')


def test_chatter_health_query_uses_resource_and_native_counter(sources):
    """Drive five chatter turns after the intro without a cleanup flag hook."""
    source = sources[1]
    lua = LuaRuntime()
    lua.execute('require=function() return {IsHeroWearingBeard=function() return false end} end')
    lua.execute(source)
    with pytest.raises(LuaError, match='CHATTER_PROVEN_5'):
        lua.execute('''
            local q,me,hero,r,held,member,movie,thing={},{},{},{},{},{},{},{}
            local frames,health,chatter=0,0,0
            local function fallback() return false end
            setmetatable(q,{__index=function() return fallback end})
            setmetatable(me,{__index=function() return fallback end})
            setmetatable(r,{__index=function() return fallback end})
            function q:GetHero() return hero end
            function q:RetailResources() return r end
            function q:NewScriptFrame() frames=frames+1; assert(frames<500,'TOO_MANY_FRAMES');return true end
            function q:IsActiveThreadTerminating() return false end
            function q:GetStateBool() return false end
            function q:RegisterTimer() return 11 end
            function q:SetTimer() end
            function q:GetTimer() return 1 end
            function q:GetHeroTargetedThing() return nil end
            function q:GetHealth(v) assert(v==thing); health=health+1; return 1 end
            function r:NewResource() return held end
            function r:PrepareResource(v) assert(v==held) end
            function r:TryAcquire(v,actor,priority)
                assert(v==held and actor==me and priority==4); return true end
            function r:MemberResource() return member end
            function r:AssignResource() end
            function r:StartMovie() return movie end
            function r:ScriptThing(v) assert(v==held,'WRONG_RESOURCE'); return thing end
            function me:IsDistanceFromPositionOver() return false end
            function me:IsTalkedToByHero() return true end
            function me:ClearCommands() end
            function me:Speak(target,tag)
                assert(target==hero)
                if string.find(tag,'TEXT_QST_B13_MAGICMAN_CHATTER_0',1,true) then
                    chatter=chatter+1
                    local expected='TEXT_QST_B13_MAGICMAN_CHATTER_0'..tostring((chatter-1)%4+1)
                    assert(tag==expected,'WRONG_CHATTER_INDEX')
                    assert(health>=2,'MISSING_HEALTH_CHECK')
                    if chatter==5 then error('CHATTER_PROVEN_5') end
                end
                return true
            end
            function me:IsPerformingScriptTask() return false end
            Main(q,me)
            error('RETURNED_WITHOUT_CHATTER')
        ''')


def test_attacked_movie_cleanup_uses_started_handle(sources):
    """Hit Magicman with a beard and zero health, then check movie ownership."""
    for source in sources[1:]:
        lua = LuaRuntime()
        lua.execute('require=function() return {IsHeroWearingBeard=function() return true end} end')
        lua.execute(source)
        with pytest.raises(LuaError, match='MOVIE_CLEANUP_PROVEN'):
            lua.execute('''
                local q,me,hero,r,held,member,movie={},{},{},{},{},{},{}
                local frames,hits,starts=0,0,0
                local function fallback() return false end
                setmetatable(q,{__index=function() return fallback end})
                setmetatable(me,{__index=function() return fallback end})
                setmetatable(r,{__index=function() return fallback end})
                function q:GetHero() return hero end
                function q:RetailResources() return r end
                function q:NewScriptFrame()
                    frames=frames+1;assert(frames<100,'TOO_MANY_FRAMES');return true end
                function q:IsActiveThreadTerminating() return false end
                function q:GetStateBool() return false end
                function q:RegisterTimer() return 11 end
                function q:SetTimer() end
                function q:GetTimer() return 1 end
                function q:GetHeroTargetedThing() return nil end
                function q:GetHealth() return 0 end
                function r:NewResource() return held end
                function r:PrepareResource(v) assert(v==held) end
                function r:TryAcquire(v,actor,priority)
                    assert(v==held and actor==me and priority==4);return true end
                function r:MemberResource() return member end
                function r:AssignResource() end
                function r:ScriptThing(v) assert(v==held);return held end
                function r:StartMovie() starts=starts+1;assert(starts==1);return movie end
                function r:DestroyMovie(v)
                    assert(v==movie,'WRONG_MOVIE_CLEANUP_'..tostring(v))
                    error('MOVIE_CLEANUP_PROVEN')
                end
                function me:IsDistanceFromPositionOver() return false end
                function me:MsgIsHitByHero() hits=hits+1;return hits==1 end
                function me:ClearCommands() end
                Main(q,me)
                error('RETURNED_WITHOUT_MOVIE_CLEANUP')
            ''')


@pytest.mark.parametrize('health', [0, 1])
def test_tricking_talk_movie_cleanup_uses_started_handle(sources, health):
    """Normal and speech-failure exits must destroy the talk movie."""
    lua = LuaRuntime()
    lua.execute('''require=function() return {
        IsHeroWearingBeard=function() return true end,
        helper_E44A40=function() return true end
    } end''')
    lua.execute(sources[1])
    lua.globals().health = health
    with pytest.raises(LuaError, match='TRICKING_MOVIE_PROVEN'):
        lua.execute('''
            local q,me,hero,r,held,member,movie={},{},{},{},{},{},{}
            local frames,talks,starts=0,0,0
            local function fallback() return false end
            setmetatable(q,{__index=function() return fallback end})
            setmetatable(me,{__index=function() return fallback end})
            setmetatable(r,{__index=function() return fallback end})
            function q:GetHero() return hero end
            function q:RetailResources() return r end
            function q:NewScriptFrame()
                frames=frames+1;assert(frames<100,'TOO_MANY_FRAMES');return true end
            function q:IsActiveThreadTerminating() return false end
            function q:GetStateBool(name) return name=='HeroTricking' end
            function q:RegisterTimer() return 11 end
            function q:SetTimer() end
            function q:GetTimer() return 1 end
            function q:GetHeroTargetedThing() return nil end
            function q:GetHealth() return health end
            function r:NewResource() return held end
            function r:PrepareResource(v) assert(v==held) end
            function r:TryAcquire(v,actor,priority)
                assert(v==held and actor==me and priority==4);return true end
            function r:MemberResource() return member end
            function r:AssignResource() end
            function r:ScriptThing(v) assert(v==held);return held end
            function r:StartMovie() starts=starts+1;assert(starts==1);return movie end
            function r:DestroyMovie(v)
                assert(v==movie,'WRONG_TRICKING_MOVIE')
                error('TRICKING_MOVIE_PROVEN')
            end
            function me:IsDistanceFromPositionOver() return false end
            function me:IsTalkedToByHero() talks=talks+1;return talks==1 end
            function me:ClearCommands() end
            function me:Speak() return false end
            Main(q,me)
            error('RETURNED_WITHOUT_TRICKING_MOVIE')
        ''')


@pytest.mark.parametrize('tricking', [False, True])
@pytest.mark.parametrize('item_case', ['beer', 'other', 'stale', 'false'])
def test_presented_item_name_uses_sidecar_output(sources, tricking, item_case):
    """Both native poll sites accept beer and reject absent or stale names."""
    lua = LuaRuntime()
    lua.execute('require=function() return {} end')
    lua.execute(sources[1])
    lua.globals().tricking = tricking
    lua.globals().item_case = item_case
    expected = 'BEER_MOVIE_STARTED' if item_case == 'beer' else 'SKIPPED_NO_MOVIE'
    with pytest.raises(LuaError, match=expected):
        lua.execute('''
            g_PresentedItemName='OBJECT_BEER_TANKARD'
            local q,me,hero,r,held,member={},{},{},{},{},{}
            local frames,polls=0,0
            local function fallback() return false end
            setmetatable(q,{__index=function() return fallback end})
            setmetatable(me,{__index=function() return fallback end})
            setmetatable(r,{__index=function() return fallback end})
            function q:GetHero() return hero end
            function q:RetailResources() return r end
            function q:NewScriptFrame()
                frames=frames+1;assert(frames<100,'TOO_MANY_FRAMES');return true end
            function q:IsActiveThreadTerminating() return false end
            function q:GetStateBool(name) return name=='HeroTricking' and tricking end
            function q:RegisterTimer() return 11 end
            function q:SetTimer() end
            function q:GetTimer() return 1 end
            function q:GetHeroTargetedThing() return nil end
            function q:GetHealth() return 0 end
            function r:NewResource() return held end
            function r:PrepareResource(v) assert(v==held) end
            function r:TryAcquire(v,actor,priority)
                assert(v==held and actor==me and priority==4);return true end
            function r:MemberResource() return member end
            function r:AssignResource() end
            function r:ScriptThing(v) assert(v==held);return held end
            function r:StartMovie()
                assert(item_case=='beer','WRONG_ITEM_STARTED_MOVIE')
                error('BEER_MOVIE_STARTED')
            end
            function me:IsDistanceFromPositionOver() return false end
            function me:MsgIsPresentedWithItem()
                polls=polls+1
                if polls==2 then error('SKIPPED_NO_MOVIE') end
                if item_case=='false' then return false end
                if item_case=='beer' then g_PresentedItemName='OBJECT_BEER_TANKARD' end
                if item_case=='other' then g_PresentedItemName='OBJECT_OTHER' end
                return true
            end
            Main(q,me)
            error('RETURNED_WITHOUT_ITEM_RESULT')
        ''')


@pytest.mark.parametrize('beard', [False, True])
def test_attacked_movie_early_exit_releases_started_movie(sources, beard):
    """Both beard branches release the attacked movie on termination."""
    lua = LuaRuntime()
    lua.execute('require=function() return {IsHeroWearingBeard=function() return beard end} end')
    lua.globals().beard = beard
    lua.execute(sources[1])
    with pytest.raises(LuaError, match='ATTACKED_MOVIE_PROVEN'):
        lua.execute('''
            local q,me,hero,r,held,member,movie={},{},{},{},{},{},{}
            local frames,started=0,false
            local function fallback() return false end
            setmetatable(q,{__index=function() return fallback end})
            setmetatable(me,{__index=function() return fallback end})
            setmetatable(r,{__index=function() return fallback end})
            function q:GetHero() return hero end
            function q:RetailResources() return r end
            function q:NewScriptFrame()
                frames=frames+1;assert(frames<100,'TOO_MANY_FRAMES');return true end
            function q:IsActiveThreadTerminating() return started end
            function q:GetStateBool(name) return name=='HeroTricking' end
            function q:RegisterTimer() return 11 end
            function q:SetTimer() end
            function q:GetTimer() return 1 end
            function q:GetHeroTargetedThing() return nil end
            function r:NewResource() return held end
            function r:PrepareResource(v) assert(v==held) end
            function r:TryAcquire(v,actor,priority)
                assert(v==held and actor==me and priority==4);return true end
            function r:MemberResource() return member end
            function r:AssignResource() end
            function r:StartMovie() assert(not started);started=true;return movie end
            function r:DestroyMovie(v)
                assert(v==movie,'WRONG_ATTACKED_MOVIE_'..tostring(v))
                error('ATTACKED_MOVIE_PROVEN')
            end
            function me:IsDistanceFromPositionOver() return false end
            function me:IsTalkedToByHero() return false end
            function me:MsgIsHitByHero() return true end
            Main(q,me)
            error('RETURNED_WITHOUT_ATTACKED_MOVIE')
        ''')


@pytest.mark.parametrize('early_exit', [False, True])
def test_final_beer_movie_releases_started_movie(sources, early_exit):
    """The overdrunk beer branch releases its movie on both exits."""
    lua = LuaRuntime()
    lua.execute('require=function() return {} end')
    lua.execute(sources[1])
    lua.globals().early_exit = early_exit
    with pytest.raises(LuaError, match='FINAL_BEER_MOVIE_PROVEN'):
        lua.execute('''
            local q,me,hero,r,held,member,movie={},{},{},{},{},{},{}
            local frames,polls,starts,afterMovie=0,0,0,false
            local function fallback() return false end
            setmetatable(q,{__index=function() return fallback end})
            setmetatable(me,{__index=function() return fallback end})
            setmetatable(r,{__index=function() return fallback end})
            function q:GetHero() return hero end
            function q:RetailResources() return r end
            function q:NewScriptFrame()
                frames=frames+1;assert(frames<100,'TOO_MANY_FRAMES')
                if starts>0 then afterMovie=true end
                return true end
            function q:IsActiveThreadTerminating() return early_exit and afterMovie end
            function q:GetStateBool(name) return name=='HeroTricking' end
            function q:GetStateInt(name) assert(name=='BeersDrunk');return 6 end
            function q:RegisterTimer() return 11 end
            function q:SetTimer() end
            function q:GetTimer() return 1 end
            function q:GetHeroTargetedThing() return nil end
            function q:GetHealth() return early_exit and 1 or 0 end
            function r:NewResource() return held end
            function r:PrepareResource(v) assert(v==held) end
            function r:TryAcquire(v,actor,priority)
                assert(v==held and actor==me and priority==4);return true end
            function r:MemberResource() return member end
            function r:AssignResource() end
            function r:ScriptThing(v) assert(v==held);return held end
            function r:StartMovie() starts=starts+1;assert(starts==1);return movie end
            function r:DestroyMovie(v)
                assert(v==movie,'WRONG_FINAL_BEER_MOVIE_'..tostring(v))
                error('FINAL_BEER_MOVIE_PROVEN')
            end
            function me:IsDistanceFromPositionOver() return false end
            function me:IsTalkedToByHero() return false end
            function me:IsPerformingScriptTask() return early_exit end
            function me:MsgIsPresentedWithItem()
                polls=polls+1
                if polls==2 then g_PresentedItemName='OBJECT_BEER_TANKARD';return true end
                return false
            end
            Main(q,me)
            error('RETURNED_WITHOUT_FINAL_BEER_MOVIE')
        ''')


@pytest.mark.parametrize('terminate', [False, True])
def test_first_beer_movie_overdrunk_task_finishes_or_releases(sources, terminate):
    """Speech completion awards beer; termination releases the movie."""
    lua = LuaRuntime()
    lua.execute('require=function() return {} end')
    lua.execute(sources[1])
    lua.globals().terminate = terminate
    with pytest.raises(LuaError, match='FIRST_BEER_TASK_PROVEN'):
        lua.execute('''
            local q,me,hero,r,held,member,movie={},{},{},{},{},{},{}
            local frames,starts,tasks,afterMovie,awarded=0,0,0,false,false
            local function fallback() return false end
            setmetatable(q,{__index=function() return fallback end})
            setmetatable(me,{__index=function() return fallback end})
            setmetatable(r,{__index=function() return fallback end})
            function q:GetHero() return hero end
            function q:RetailResources() return r end
            function q:NewScriptFrame()
                frames=frames+1;assert(frames<100,'TOO_MANY_FRAMES')
                if starts>0 then afterMovie=true end
                return true end
            function q:IsActiveThreadTerminating() return terminate and afterMovie end
            function q:GetStateBool() return false end
            function q:GetStateInt(name) assert(name=='BeersDrunk');return 6 end
            function q:RegisterTimer() return 11 end
            function q:SetTimer() end
            function q:GetTimer() return 1 end
            function q:GetHeroTargetedThing() return nil end
            function q:GetHealth() return 1 end
            function q:GiveHeroObject(name,count,flag)
                assert(not terminate and name=='OBJECT_BEER_TANKARD' and count==-1 and flag==true)
                awarded=true
            end
            function r:NewResource() return held end
            function r:PrepareResource(v) assert(v==held) end
            function r:TryAcquire(v,actor,priority)
                assert(v==held and actor==me and priority==4);return true end
            function r:MemberResource() return member end
            function r:AssignResource() end
            function r:ScriptThing(v) assert(v==held);return held end
            function r:StartMovie() starts=starts+1;assert(starts==1);return movie end
            function r:DestroyMovie(v)
                assert(v==movie,'WRONG_FIRST_BEER_MOVIE_'..tostring(v))
                assert(terminate or awarded,'MISSING_BEER_AWARD')
                error('FIRST_BEER_TASK_PROVEN')
            end
            function me:IsDistanceFromPositionOver() return false end
            function me:IsTalkedToByHero() return false end
            function me:MsgIsPresentedWithItem()
                g_PresentedItemName='OBJECT_BEER_TANKARD'
                return true
            end
            function me:IsPerformingScriptTask()
                if starts==0 then return false end
                tasks=tasks+1
                return tasks==1
            end
            Main(q,me)
            error('RETURNED_WITHOUT_FIRST_BEER_TASK')
        ''')


@pytest.mark.parametrize('tricking', [False, True])
def test_talking_out_loud_timer_uses_numeric_counter(sources, tricking):
    """Both routes add successive dialogue lines with the native actor order."""
    lua = LuaRuntime()
    lua.execute('require=function() return {} end')
    lua.execute(sources[1])
    lua.globals().tricking = tricking
    with pytest.raises(LuaError, match='ALOUD_PROVEN'):
        lua.execute('''
            local q,me,hero,r,held,member={},{},{},{},{},{}
            local frames,registered,seen,lines=0,0,{},0
            local function fallback() return false end
            setmetatable(q,{__index=function() return fallback end})
            setmetatable(me,{__index=function() return fallback end})
            setmetatable(r,{__index=function() return fallback end})
            function q:GetHero() return hero end
            function q:RetailResources() return r end
            function q:NewScriptFrame() frames=frames+1;assert(frames<100,'TOO_MANY_FRAMES');return true end
            function q:IsActiveThreadTerminating() return false end
            function q:GetStateBool(name) return name=='HeroTricking' and tricking end
            function q:RegisterTimer() registered=registered+1;return registered*11 end
            function q:SetTimer() end
            function q:GetTimer(id) return id==11 and 1 or 0 end
            function q:GetHeroTargetedThing() return nil end
            function q:IsDistanceBetweenThingsUnder() return true end
            function q:TextEntryExists(tag)
                seen[#seen+1]=tag
                local expected='TEXT_QST_B13_MAGICMAN_TALKING_OUT_LOUD_0'..tostring(#seen)
                assert(tag==expected,'WRONG_DIALOGUE_INDEX')
                return true
            end
            function q:AddNewConversation(actor,a,b)
                assert(actor==me and a==false and b==false)
                return 77
            end
            function q:AddPersonToConversation(id,actor)
                assert(id==77 and actor==hero)
            end
            function q:AddLineToConversation(id,key,speaker,listener,showSubtitle)
                lines=lines+1
                assert(id==77 and key==seen[lines] and speaker==me
                    and listener==hero and showSubtitle==false,'WRONG_CONVERSATION_LINE')
                if lines==2 then error('ALOUD_PROVEN') end
            end
            function r:NewResource() return held end
            function r:PrepareResource(v) assert(v==held) end
            function r:TryAcquire(v,actor,priority)
                assert(v==held and actor==me and priority==4);return true end
            function r:MemberResource() return member end
            function r:AssignResource() end
            function me:IsDistanceFromPositionOver() return false end
            function me:IsTalkedToByHero() return false end
            Main(q,me)
            error('RETURNED_WITHOUT_ALOUD')
        ''')
