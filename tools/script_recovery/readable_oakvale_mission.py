"""Replace native scratch bookkeeping with the checked mission dispatcher."""
import hashlib
from pathlib import Path
from tools.script_recovery.native_oakvale_mission_scopes import prove

RAW_SHA='4a695b74994525d2f3746cf14a89a20a266ce89b986102b3a631c91f824c3eed'


def lower(source):
    start=source.index('function DoMission(');end=source.index('function AttackStuff(',start)
    if hashlib.sha256(source[start:end].encode()).hexdigest()!=RAW_SHA:raise ValueError('DoMission draft changed')
    evidence=prove();body=Path(__file__).with_name('oakvale_mission_body.lua').read_text()
    return source[:start]+body+'\n'+source[end:],dict(status='structured native mission dispatcher',
        rawSha256=RAW_SHA,native=evidence,
        improvements=['Scoped child transformation output','House retained through start screen and music',
                      'Fresh active-name output for each completion action','Exact termination query placement'],
        limits='Native dispatcher comparisons use successful thread registration boundaries; native allocation failure, engine scheduler and persistence remain pending.')
