"""Compose the thanks and careful movies without losing their continuation joins."""
import hashlib
import json
from pathlib import Path

HELPERS='''    local function finishSpeechMovie(movie)
        resources:Pause(false)
        resources:DestroyMovie(movie)
    end
    local function waitForBarrelSpeech()
        while resources:IsPerformingScriptTask(barrel_resource) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then return false end
        end
        return not quest:IsActiveThreadTerminating()
    end
    local function playThanksMovie()
        local movie = resources:StartMovie("")
        resources:Pause(true)
        if controlled_health() > 0.0 then
            resources:Speak(barrel_resource, quest:GetHero(), "TEXT_QST_048_BARRELMAN_THANKS", 0, false, true, false)
            if not waitForBarrelSpeech() then
                finishSpeechMovie(movie)
                return false
            end
        end
        require("NewOakValeIntro.native_quest_helpers").AddGoodDeed(quest, me)
        finishSpeechMovie(movie)
        return true
    end
    local function playCarefulMovie()
        local movie = resources:StartMovie("")
        resources:Pause(true)
        if controlled_health() > 0.0 then
            resources:Speak(barrel_resource, quest:GetHero(), "TEXT_QST_048_BARRELMAN_CAREFUL", 0, false, true, false)
            if not waitForBarrelSpeech() then
                finishSpeechMovie(movie)
                return false
            end
        end
        finishSpeechMovie(movie)
        return true
    end
'''


def recover(source,data):
    w=json.loads(Path(__file__).with_name('native_barrel_speech_movies_witness.json').read_text())
    replacements={
        'thanks':'                                if not playThanksMovie() then goto LAB_00db6afd end\n',
        'careful':'            if not playCarefulMovie() then goto LAB_00db6afd end\n            goto LAB_00db6933\n'}
    for block in w['blocks']:
        raw=data.bytes_at(block['address'],block['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=block['sha256']:
            raise ValueError('Barrel speech movie native instructions changed')
        if source.count(block['oldLua'])!=1:raise ValueError('Barrel speech movie source correspondence changed')
        source=source.replace(block['oldLua'],replacements[block['name']])
    marker='    local bVar3, cVar2,'
    if source.count(marker)!=1:raise ValueError('Barrel speech movie helper scope changed')
    source=source.replace(marker,HELPERS+marker,1)
    return source,dict(w,status='recovered',
        semantics='Thanks awards a good deed after optional positive-health speech; careful returns to the frame loop. Both cancel through unpause/movie destruction before outer cleanup.')
