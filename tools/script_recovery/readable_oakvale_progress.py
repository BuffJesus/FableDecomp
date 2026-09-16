"""Emit native-checked attack transition and gold objective watcher."""
import hashlib
from pathlib import Path
from tools.script_recovery.native_oakvale_progress import prove

REGIONS=(('AttackStuff','PostAttackStuff','c13781da960c451a1613c1cfeb85d09ac39a5f30562bac6bb423e011c2b20cbe'),
         ('WatchForGotGold','AddGoodDeed','9f6580dc7ac0df989c670f16ab5a5fd3c825703fc3cdda737c6a32d1178d9c21'))


def lower(source):
    evidence=prove();bodies=Path(__file__).with_name('oakvale_progress_bodies.lua').read_text()
    split=bodies.index('function WatchForGotGold(');replacements=(bodies[:split],bodies[split:]+'\n')
    for (name,next_name,expected),body in zip(REGIONS,replacements):
        start=source.index('\nfunction '+name+'(')+1;end=source.index('\nfunction '+next_name+'(',start)+1
        if hashlib.sha256(source[start:end].encode()).hexdigest()!=expected:raise ValueError('Oakvale progress draft changed: '+name)
        source=source[:start]+body+source[end:]
    return source,dict(status='structured native attack and gold watcher',native=evidence,
        correction='Both objective region arguments are empty strings; gold objective text is not a region.',
        limits='Engine/CString operations and scheduling remain boundary doubles.')
