"""Victim hit consequences and first-time retained Hero/map/movie macro phase."""
import json
from tools.script_recovery.victim_subdued import recover as proof
from tools.script_recovery.lift_native_lua import ROOT,RData
SOURCE='''function VictimAcquire(quest, resources, control, target)
    resources:PrepareResource(control)
    while not resources:TryAcquire(control, target(), 4) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return false end
    end
    return not quest:IsActiveThreadTerminating()
end

function VictimFirstHitMovie(quest, me, resources, control, bully)
    local hero = resources:NewResource()
    local actors, movie
    local pauseAttempted, cameraAttempted = false, false
    local ok, complete = xpcall(function()
        if not VictimAcquire(quest, resources, hero, function() return quest:GetHero() end) then return false end
        actors = resources:NewActorMap()
        resources:SetActor(actors, "HERO", hero)
        resources:SetActor(actors, "BRAT", control)
        movie = resources:NewMovie()
        resources:StartOwnedMovie(movie, "")
        pauseAttempted = true
        quest:PauseAllNonScriptedEntities(true)
        cameraAttempted = true
        quest:FixMovieSequenceCamera(true)
        resources:RunMacro("CS_OAKVALEINTRO_BRATHIT", actors, false, true)
        quest:FixMovieSequenceCamera(false)
        cameraAttempted = false
        resources:ClearRawInformation(me)
        resources:FaceTowardsRetainedThing(me, bully, false)
        return true
    end, function(err) return err end)
    local cleanupError
    local function close(callback)
        local closed, err = pcall(callback)
        if not closed and cleanupError == nil then cleanupError = err end
    end
    if cameraAttempted then close(function() quest:FixMovieSequenceCamera(false) end) end
    if pauseAttempted then close(function() quest:PauseAllNonScriptedEntities(false) end) end
    if movie then close(function() resources:DestroyMovie(movie) end) end
    if actors then close(function() resources:DestroyActorMap(actors) end) end
    close(function() resources:ReleaseResource(hero) end)
    if not ok then error(complete, 0) end
    if cleanupError ~= nil then error(cleanupError, 0) end
    return complete
end

function VictimBeginHit(quest, me, resources, control, bully, addBadDeed)
    if not resources:IsHitByHeroExceptAbility(me, 14) then return "continue" end
    if quest:IsActiveThreadTerminating() then return "cancel" end
    resources:SetThingAsAlly(me, quest:GetHero())
    resources:SetThingAsAlly(quest:GetHero(), me)
    addBadDeed(2)
    quest:SetStateBool("HeroAttackedVictim", true)
    if quest:GetStateBool("GivenHeroTeddy") then return "repeat" end
    if quest:IsActiveThreadTerminating() then return "cancel" end
    quest:SetStateBool("GivenHeroTeddy", true)
    if not VictimAcquire(quest, resources, control, function() return me end) then return "cancel" end
    if not VictimFirstHitMovie(quest, me, resources, control, bully) then return "cancel" end
    return "continue"
end
'''
LITERALS={0x1255174:'HERO',0x12d99e8:'BRAT',0x12d9bf0:'CS_OAKVALEINTRO_BRATHIT',0x125d1c8:'SCRIPT_NAME_HERO'}
def recover(data=None):
    data=data or RData();w=proof(data)[1]
    for address,value in LITERALS.items():
        if data.string_at(address)!=value:raise ValueError('Victim hit/map/macro literal changed')
    return SOURCE,{'mainSha256':w['mainSha256'],'start':0xdbd60f,'continue':0xdbdc58,'repeat':0xdbd9d0,'cancel':0xdbde18,
        'locals':{'selfResource':16,'retainedBully':32,'mask':48,'heroResource':176,'actorMap':192,'movie':204},
        'macro':{'flags':None,'inputs':None,'setup':False,'skippable':True},
        'limits':['Repeat result stops before DBD9D0 cancellation query; repeat-conversation body remains separate.',
                  'Caller retains Bully Thing and self resource; first-time helper destroys only its own movie/map/Hero resource.',
                  'Engine API internals are checked boundaries; failed acquisition may populate output.',
                  'Merged resource owner, condition/state persistence and scheduler teardown remain pending.']}
def generate():
    source,w=recover();out=ROOT/'work/victim_converter';out.mkdir(parents=True,exist_ok=True);(out/'HIT_PHASE.lua').write_text('-- Disabled Victim hit prefix and first-time macro.\n'+source);(out/'HIT_EVIDENCE.json').write_text(json.dumps(w,indent=2)+'\n');return source,w
