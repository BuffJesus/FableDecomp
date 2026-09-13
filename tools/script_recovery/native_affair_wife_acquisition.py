"""Recover Wife's acquisition operands; native handle lifetime remains unresolved."""
import hashlib
import json
import re
from pathlib import Path
from tools.script_recovery.native_post_attack_resources import map_affair_wife_resources

CALL = re.compile(r'GSI->StartScriptingEntity\([^;]*\);')


def recover_affair_wife_acquisition(function, source, rdata):
    w = json.loads(Path(__file__).with_name('native_affair_wife_acquisition_witness.json').read_text())
    if str(function.get('address', '')).lower() != w['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    if hashlib.sha256(source.encode()).hexdigest() != w['annotatedSha256']:
        return reject('acquisition source changed')
    resources = map_affair_wife_resources(function, rdata)
    if not resources or resources[0]['status'] != 'mapped':
        return reject('resource mapping unavailable')
    calls = [e for e in resources[0]['events'] if e['name'] == 'acquire']
    if len(calls) != 10 or CALL.findall(source) != w['oldCalls']:
        return reject('acquisition correspondence changed')
    for event, priority in zip(calls, w['priorities']):
        if event['setup']['stack_arguments'] != [['register', 'edi'], ['stack', 16], ['constant', priority]]:
            return reject('acquisition actor/resource/priority changed')
    priorities = iter(w['priorities'])
    result = CALL.sub(lambda _: 'GSI->StartScriptingEntity(me,&local_native_wife_control,' + str(next(priorities)) + ');', source)
    return result, [dict(w, status='recovered', resourceEvidence=resources[0], ownershipLowering='unresolved')]
