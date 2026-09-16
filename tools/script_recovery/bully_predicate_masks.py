"""Replace native CString lifetime masks by scoped atomic predicate consumers."""
import re
from tools.script_recovery.bully_initial_phases import recover
from tools.script_recovery.lift_native_lua import RData

def recover_masks(source):
    data=RData();recover(data)
    if data.string_at(0x125d1c8)!='SCRIPT_NAME_HERO' or data.string_at(0x12d8958)!='OBJECT_TEDDY_BEAR_UNGIVEABLE':
        raise ValueError('Bully predicate names changed')
    # Source may already have the independently verified loop fixes. Anchor the
    # entire predicate region; do not rename unresolved masks elsewhere.
    talk=re.search(r'(?m)^                uVar16 = uStack_120 \| 1\n[\s\S]*?^                if not native_arg_bully_talked_with_teddy then',source)
    hit=re.search(r'(?m)^        uVar16 = uStack_120 \| 4\n[\s\S]*?^        if native_arg_bully_hit then',source)
    if talk is None or hit is None:raise ValueError('Bully predicate mask anchors changed')
    from tools.script_recovery.bully_initial_candidate import DRAFT,correspondence
    correspondence()  # Guard the baseline draft independently of other passes.
    draft=DRAFT.read_text()
    # Pin these exact source blocks independently of the separate flow pass.
    for match in (talk,hit):
        if draft.count(match[0])!=1:raise ValueError('Bully predicate mask source changed')
    source=source.replace(talk[0],'                native_arg_bully_talked_with_teddy = resources:BullyTalkedWithTeddy(me)\n                if not native_arg_bully_talked_with_teddy then')
    source=source.replace(hit[0],'        native_arg_bully_hit = resources:IsHitByHeroExceptAbility(me, 14)\n        if native_arg_bully_hit then')
    return source,{'talkRegion':[0xdbb613,0xdbb6b9],'hitRegion':[0xdbc29e,0xdbc385],
        'maskInitialStore':0xdbb32e,'maskStackOffset':56,
        'policy':'Mask bits1/2 and4/8/16 only track local CString construction; predicate short circuit and reverse destruction retained atomically.',
        'requiredCapabilities':['BullyTalkedWithTeddy(boundThing)','IsHitByHeroExceptAbility(boundThing,14)'],
        'limits':'Other mask-looking native storage and actor/map outputs are not inferred.'}
