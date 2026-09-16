"""Attach the native-verified marker helper to the readable quest package."""
import hashlib
from tools.script_recovery.manage_quest_core_markers import generate

RAW_SHA='b760fddf7d644a88ccedda7e8d982c4747377a1f503827d3bbd129d3ed3b4b3c'


def lower(source):
    start=source.index('\nfunction ManageQuestCoreMarkers(')+1
    end=source.index('\nfunction StartBarrelTimer(',start)+1
    if hashlib.sha256(source[start:end].encode()).hexdigest()!=RAW_SHA:
        raise ValueError('ManageQuestCoreMarkers draft changed')
    helper,evidence=generate()
    return source[:start]+helper+'\n'+source[end:],dict(
        status='structured retained-Thing marker helper',rawSha256=RAW_SHA,evidence=evidence,
        remaining=['Two marker methods require common resource-owner merge.',
                   'Native caller/scheduler and authoritative parent state/save-load remain unverified.',
                   'Pre-return lookup failure construction and arbitrary native exceptions remain outside the proved contract.'])
