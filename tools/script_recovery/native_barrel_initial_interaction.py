"""Structure phase zero with a timer that outlives movement and departure."""
import hashlib
import json
from pathlib import Path

HELPER='''    local function playInitialInteraction(movie)
        if quest:IsActiveThreadTerminating() then
            finishSpeechMovie(movie)
            return false
        end
        local continued = resources:WithTimer(function(timer)
            while resources:IsBarrelManFarFromHero(me) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return false end
                resources:MoveBarrelManToHero(barrel_resource)
                timer:Set(2)
                while resources:IsPerformingScriptTask(barrel_resource) do
                    if timer:Get() <= 0 then break end
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then return false end
                end
                if quest:IsActiveThreadTerminating() then return false end
            end
            if quest:IsActiveThreadTerminating() then return false end
            if controlled_health() > 0.0 then
                resources:Speak(barrel_resource, quest:GetHero(), "TEXT_QST_048_BARRELMAN_FAVOUR", 0, false, true, false)
                if not waitForBarrelSpeech() then return false end
            end
            quest:FadeScreenOut(1.0, 1.0)
            quest:Pause(2.0)
            local guardPoint = quest:GetThingWithScriptName("M_WHouse_GuardPoint")
            quest:EntityTeleportToThing(quest:GetHero(), guardPoint, false)
            local hiddenPoint = quest:GetThingWithScriptName("M_BarrelManHiddenPos")
            quest:EntityTeleportToThing(me, hiddenPoint, false)
            __native_entity_state:SetStateInt("MyPhase", 2)
            quest:ClearThingHasInformation(me)
            quest:FadeScreenIn()
            resources:SetBarrelWatchTimer(quest:GetStateInt("WatchTimer"))
            quest:SetStateBool("BarrelManLeftHeroInCharge", true)
            return true
        end)
        if continued then resources:PrepareResource(barrel_resource) end
        finishSpeechMovie(movie)
        return continued
    end
'''


def recover(source,data):
    w=json.loads(Path(__file__).with_name('native_barrel_initial_interaction_witness.json').read_text())
    raw=data.bytes_at(w['address'],w['size'])
    if raw is None or hashlib.sha256(raw).hexdigest()!=w['sha256']:
        raise ValueError('Barrel initial interaction native instructions changed')
    if source.count(w['oldLua'])!=1:raise ValueError('Barrel initial interaction source correspondence changed')
    source=source.replace(w['oldLua'],'''            local continued = playInitialInteraction(barrel_interaction_movie)
            barrel_interaction_movie = nil
            if not continued then goto LAB_00db6afd end
            goto LAB_00db6933
''')
    source=source.replace('    local bVar3, cVar2,',HELPER+'    local bVar3, cVar2,',1)
    return source,dict(w,status='structured-with-owned-timer',
        requires=['WithTimer','IsBarrelManFarFromHero','MoveBarrelManToHero','SetBarrelWatchTimer'],
        runtimeValidation='work/barrel_movement_runtime_checks/result.json',
        runtimeProposal='work/barrel_resource_integration/proposal.json',
        remaining='Borrowed hero adapters are staged; departure marker and hero wrapper ownership still need lowering, full DLL/gameplay remain pending.')
