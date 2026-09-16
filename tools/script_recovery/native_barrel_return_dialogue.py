"""Structure the nonzero-phase replies inside Barrel Man's interaction movie."""
import hashlib
import json
from pathlib import Path

HELPER='''    local function playReturnInteraction(movie, phase)
        local function cancel()
            finishSpeechMovie(movie)
            return false
        end
        local line
        if phase < 4 then
            if quest:IsActiveThreadTerminating() then return cancel() end
            line = "TEXT_QST_048_BARRELMAN_NOT_LARKING"
        elseif phase == 5 then
            if quest:IsActiveThreadTerminating() then return cancel() end
            if not __native_entity_state:GetStateBool("HeroLetMeDown") then
                if quest:IsActiveThreadTerminating() then return cancel() end
                line = "TEXT_QST_048_BARRELMAN_NO_TIME"
            else
                if quest:IsActiveThreadTerminating() then return cancel() end
                local broken = quest:GetStateBool("BarrelBrokenPersistent")
                if quest:IsActiveThreadTerminating() then return cancel() end
                if broken then
                    line = "TEXT_QST_048_BARRELMAN_LETDOWN_BROKEN"
                else
                    line = "TEXT_QST_048_BARRELMAN_LETDOWN_NOT_BROKE"
                end
            end
        end
        if line and controlled_health() > 0.0 then
            resources:Speak(barrel_resource, quest:GetHero(), line, 0, false, true, false)
            if not waitForBarrelSpeech() then return cancel() end
        end
        resources:PrepareResource(barrel_resource)
        finishSpeechMovie(movie)
        return true
    end
'''


def recover(source,data):
    w=json.loads(Path(__file__).with_name('native_barrel_return_dialogue_witness.json').read_text())
    raw=data.bytes_at(w['address'],w['size'])
    if raw is None or hashlib.sha256(raw).hexdigest()!=w['sha256']:
        raise ValueError('Barrel return dialogue native instructions changed')
    if source.count(w['oldLua'])!=1:raise ValueError('Barrel return dialogue source correspondence changed')
    source=source.replace(w['oldLua'],'''            if iVar11 ~= 0 then
                local continued = playReturnInteraction(barrel_interaction_movie, iVar11)
                barrel_interaction_movie = nil
                if not continued then goto LAB_00db6afd end
                goto LAB_00db6933
            end
''')
    if source.count('quest:StartMovieSequence()')!=1:
        raise ValueError('Barrel interaction movie constructor correspondence changed')
    source=source.replace('quest:StartMovieSequence()', 'barrel_interaction_movie = resources:StartMovie("")')
    source=source.replace('quest:PauseAllNonScriptedEntities(', 'resources:Pause(')
    source=source.replace('    local barrel_resource\n','    local barrel_resource, barrel_interaction_movie\n',1)
    source=source.replace('    local bVar3, cVar2,',HELPER+'    local bVar3, cVar2,',1)
    end='    ::LAB_00db6af8::\n'
    if source.count(end)!=1:raise ValueError('Barrel interaction movie cleanup join changed')
    source=source.replace(end,end+'    resources:DestroyMovie(barrel_interaction_movie); barrel_interaction_movie = nil\n',1)
    return source,dict(w,status='recovered-nonzero-phase',remaining='Phase-zero timer/movement/teleport control flow still needs completion.')
