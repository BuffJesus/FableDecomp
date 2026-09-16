"""Contain new Lua/adapter errors while preserving checked native normal cleanup."""
import hashlib
import json
from pathlib import Path


def recover(source):
    witness=json.loads(Path(__file__).with_name('scythe_cleanup_witness.json').read_text())
    if hashlib.sha256(source.encode()).hexdigest()!=witness['sourceSha256']:
        raise ValueError('Scythe cleanup source correspondence changed')
    source=source.replace('        local function controlled_body()',
        '        local timer_live, movie_live, pause_live = false, false, false\n        local function controlled_body()')
    for call, track, count in [
        ('TurningTimer = quest:RegisterTimer()','timer_live = true',1),
        ('quest:DeregisterTimer(TurningTimer)','timer_live = false',6),
        ('quest:StartMovieSequence()','movie_live = true',2),
        ('quest:EndMovieSequence()','movie_live = false',4),
        ('quest:PauseAllNonScriptedEntities(true)','pause_live = true',2),
        ('quest:PauseAllNonScriptedEntities(false)','pause_live = false',4)]:
        if source.count(call)!=count: raise ValueError('Scythe cleanup call correspondence changed')
        source=source.replace(call,call+'; '+track)
    old='''        local ok, failure = pcall(controlled_body)
        me:ReleaseControl()
        if not ok then error(failure, 0) end'''
    new='''        local ok, failure = pcall(controlled_body)
        if not ok then
            -- Extra host-error containment; normal native exits already close these.
            if pause_live then pcall(function() quest:PauseAllNonScriptedEntities(false) end) end
            if movie_live then pcall(function() quest:EndMovieSequence() end) end
            if timer_live then pcall(function() quest:DeregisterTimer(TurningTimer) end) end
        end
        local release_ok, release_error = pcall(function() me:ReleaseControl() end)
        if not ok then error(failure, 0) end
        if not release_ok then error(release_error, 0) end'''
    if source.count(old)!=1: raise ValueError('Scythe cleanup callback correspondence changed')
    return source.replace(old,new),dict(witness,status='host error containment applied')
