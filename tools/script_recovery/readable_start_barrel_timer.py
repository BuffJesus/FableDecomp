"""Attach the whole-native-function timer recovery to the disabled package."""
import hashlib

from tools.script_recovery.start_barrel_timer import generate

RAW_SHA = 'd166ae7d93bc9d37aa9b16d4f84738227d8e8ddb4de101c08446e5640e0e96ec'


def lower(source):
    start = source.index('\nfunction StartBarrelTimer(') + 1
    end = source.index('\nfunction WatchBarrels(', start) + 1
    if hashlib.sha256(source[start:end].encode()).hexdigest() != RAW_SHA:
        raise ValueError('StartBarrelTimer draft changed')
    helper, evidence = generate()
    return source[:start] + helper + '\n' + source[end:], dict(
        status='structured timer helper with retained guard Thing',
        rawSha256=RAW_SHA, evidence=evidence,
        remaining=['Six timer capabilities require common runtime integration.',
                   'Engine scheduling, timer state restore and live gameplay remain unverified.'])
