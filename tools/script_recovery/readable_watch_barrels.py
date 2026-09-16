"""Replace the verified raw WatchBarrels body with scoped readable helpers."""
import hashlib
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData

RAW_BODY_SHA='b8cd2d1525fdb1be734fe15425452ed1e563de6f1b9cb382afdbfb7b4a164cb4'


def lower(source):
    start=source.index('\nfunction WatchBarrels(')+1;end=source.index('\nfunction WatchForGotGold(',start)+1
    if hashlib.sha256(source[start:end].encode()).hexdigest()!=RAW_BODY_SHA:raise ValueError('WatchBarrels draft changed')
    data=RData()
    if hashlib.sha256(data.bytes_at(0xdbe890,647)).hexdigest()!='3c48ce5c57e30e903c5e6790f7dfd2d4a5cf9b7664f433134d6909840fd06bf2':raise ValueError('WatchBarrels native bytes changed')
    parts=[]
    for name in ('watch_barrels_loop.lua','watch_barrels_body.lua'):
        body=Path(__file__).with_name(name).read_text()
        if body.count('\nreturn ')!=1:raise ValueError('WatchBarrels helper export changed')
        parts.append(body.rsplit('\nreturn ',1)[0])
    glue="""
function WatchBarrels(quest)
    quest:WithRetailResources(function(resources)
        watchBarrelsWithSnapshot(quest, resources, function(deed) AddBadDeed(quest, deed) end)
    end)
end

"""
    result=source[:start]+'\n'.join(parts)+glue+source[end:]
    report=dict(status='disabled scoped WatchBarrels composition',rawBodySha256=RAW_BODY_SHA,
        implementation='watchBarrelsWithSnapshot',remaining=['Snapshot and decision loop compared separately; merged engine execution pending.',
        'Snapshot/reward adapters have compiled checks; merged quest owner, persistence, scheduler, DLL and gameplay pending.'])
    return result,report
