"""Replace reviewed post-attack dispatcher after movie-scope lowering."""
import hashlib
from pathlib import Path
from tools.script_recovery.native_post_attack_world import prove

RAW_SHA='d05fa7120ac241ce9fcdff4cdca2edaeb21e1cdccee64973b365b335dd75c1b3'


def lower(source):
    start=source.index('\nfunction PostAttackStuff(')+1
    end=source.index('\nfunction ManageQuestCoreMarkers(',start)+1
    if hashlib.sha256(source[start:end].encode()).hexdigest()!=RAW_SHA:
        raise ValueError('PostAttackStuff dispatcher draft changed')
    evidence=prove()
    body=Path(__file__).with_name('post_attack_body.lua').read_text()
    if body.count('\nreturn runPostAttack')!=1:raise ValueError('PostAttackStuff export changed')
    body=body.rsplit('\nreturn runPostAttack',1)[0]
    return source[:start]+body+'\n\n'+source[end:],dict(
        status='structured dispatcher with atomic lookup scopes',rawSha256=RAW_SHA,
        native=evidence,implementation='runPostAttack',
        remaining=['Full linked runtime, scheduler/state restoration and live gameplay remain unverified.',
                   'Dispatcher phase comparisons and atomic/movie scope comparisons are separate gates.',
                   'Pre-return lookup construction failures remain outside the native ownership contract.'])
