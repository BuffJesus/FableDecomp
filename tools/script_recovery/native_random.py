"""Fold a dead rand temporary into the host's retail CRT modulo operation."""
import re
import hashlib
import json
from pathlib import Path

RAND_THUNK = 0x00BFEB16
PROTECTED = re.compile(r'/\*[\s\S]*?\*/|//[^\n]*|"(?:[^"\\]|\\.)*"|\'(?:[^\'\\]|\\.)*\'')
PAIR = re.compile(r'\b(?P<temp>iVar\d+)\s*=\s*(?:rand|ENGINE_Rand)\(\s*\)\s*;\s*'
                  r'(?P<dest>[iu]Var\d+)\s*=\s*(?P=temp)\s*%\s*'
                  r'(?P<modulus>[A-Za-z_]\w*|[1-9]\d*)\s*;')


def fold_book_trader_random(source, native_function, read_bytes):
    if str(native_function.get('address', '')).lower() != '0x00db3fa0':
        return source, []
    return fold_retail_random(source, native_function, read_bytes,
                              witness_file='native_book_trader_random_witness.json')


def fold_affair_wife_random(source, native_function, read_bytes):
    if str(native_function.get('address', '')).lower() != '0x00db2b10':
        return source, []
    return fold_retail_random(source, native_function, read_bytes,
                              witness_file='native_affair_wife_random_witness.json')


def fold_retail_random(source, native_function, read_bytes, *, witness_file=None):
    """Require exact call identity and no surviving use of the raw random value.

    The host calls 0x00BFEB16 and applies C++ integer remainder. This rule is for
    positive counts, as supplied by retail vector sizes; it does not use Lua RNG.
    """
    code = PROTECTED.sub(lambda m: ' ' * len(m[0]), source)
    calls = [c for c in native_function.get('calls', [])
             if c.get('currentName', '').split('::')[-1] == 'rand']
    if not calls or len(re.findall(r'\b(?:rand|ENGINE_Rand)\s*\(', code)) != len(calls):
        return source, []
    for call in calls:
        try:
            site, target = int(call['site'], 16), int(call['target'], 16)
            raw = read_bytes(site, 5)
            if target != RAND_THUNK or not raw or len(raw) != 5 or raw[0] != 0xE8:
                return source, []
            if (site + 5 + int.from_bytes(raw[1:], 'little', signed=True)) & 0xffffffff != target:
                return source, []
        except (KeyError, TypeError, ValueError):
            return source, []
    edits, evidence = [], []
    witness_file = witness_file or ('native_affair_man_random_witness.json'
                    if str(native_function.get('address', '')).lower() == '0x00db09e0'
                    else 'native_random_condition_witness.json')
    witness = json.loads(Path(__file__).with_name(witness_file).read_text())
    if str(native_function.get('address', '')).lower() == witness['address'].lower():
        for value, key in ((native_function.get('decompile', '').encode(), 'sourceSha256'),
                           (source.encode(), 'annotatedSha256'),
                           (read_bytes(int(witness['address'], 16), witness['size']), 'bytesSha256')):
            if value is None or hashlib.sha256(value).hexdigest() != witness[key]:
                return source, [{'status': 'rejected', 'reason': key + ' changed', 'kind': 'conditional modulo'}]
        replacements = witness.get('edits', [{
            'old': witness.get('old', '(iVar8 = rand(), iVar8 % 100 == 0)'),
            'new': witness.get('new', '(iVar8 = GSI->RetailRandModulo(100), iVar8 == 0)'),
            'modulus': 100}])
        if any(source.count(edit['old']) != 1 for edit in replacements):
            return source, [{'status': 'rejected', 'reason': 'conditional modulo shape changed'}]
        # Villager: DAE635 calls rand; DAE63A..DAE642 performs signed remainder/test.
        # On the nonzero edge DAE98E..DAE998 reaches frame/termination calls; on
        # the zero edge DAE654 overwrites EAX with GetHero. Neither uses the random
        # value afterward. Replacing that dead value with its remainder is safe.
        # AffairMan's separate witness reviews DB1821..DB1841 and DB1C71:
        # both successors discard the raw value before any further script use.
        for edit in replacements:
            source = source.replace(edit['old'], edit['new'], 1)
        return source, [
            {'status': 'recovered', 'kind': 'conditional modulo', 'callee': '0x00BFEB16',
             'modulus': edit['modulus'], 'nativeSha256': witness['bytesSha256']}
            for edit in replacements]
    for match in PAIR.finditer(code):
        temp = match['temp']
        rest = code[:match.start()] + code[match.end():]
        rest = re.sub(r'\bint\s+' + re.escape(temp) + r'\s*;', '', rest)
        # An in-place remainder overwrites the raw draw before any later use.
        # A distinct result requires the raw temporary to be dead everywhere.
        # The divisor must not itself refer to the freshly drawn value.
        if match['modulus'] == temp or (temp != match['dest'] and
                re.search(r'\b' + re.escape(temp) + r'\b', rest)):
            continue
        # Do not swallow any intervening source comment.
        if source[match.start():match.end()] != match[0]:
            continue
        edits.append((match.start(), match.end(),
                      f"{match['dest']} = GSI->RetailRandModulo({match['modulus']});"))
        evidence.append({'temporary': temp, 'result': match['dest'], 'modulus': match['modulus'],
                         'callee': '0x00BFEB16'})
    for start, end, replacement in reversed(edits):
        source = source[:start] + replacement + source[end:]
    return source, evidence


RAW_MODULUS=32768
RAW_THUNK_BYTES=bytes.fromhex('ff2524014401')
RAW_CALL=re.compile(r'(?<![\w:.>])(?:rand|ENGINE_Rand)\s*\((?P<args>(?:[^()]|\([^()]*\))*)\)')
RAW_PURE=re.compile(r'(?:(?:\((?:int|uint|undefined4|long|ulong)\)\s*)*)(?:[A-Za-z_]\w*|-?(?:0x[0-9a-fA-F]+|[0-9]+))')

def recover_raw_retail_random(source,fn,read_bytes):
    """Preserve raw draws when no modulo-pair or reviewed conditional fold applies."""
    calls=[c for c in fn.get('calls',[]) if c.get('currentName','').split('::')[-1]=='rand']
    if not calls or len({c.get('site') for c in calls}) != len(calls):return source,[]
    # Retail imports this MSVCR71 rand thunk. Its exported implementation ends
    # SHR EAX,16; AND EAX,7fff; RET (native_raw_random_witness.json).
    # Modulo32768 therefore retains the whole draw and its native stream advance.
    if read_bytes(RAND_THUNK,6)!=RAW_THUNK_BYTES:return source,[]
    code=PROTECTED.sub(lambda m:' '*len(m[0]),source)
    matches=list(RAW_CALL.finditer(code))
    if len(matches)!=len(calls):return source,[]
    for call in calls:
        try:
            site,target=int(call['site'],16),int(call['target'],16);raw=read_bytes(site,5)
            if target!=RAND_THUNK or not raw or len(raw)!=5 or raw[0]!=0xe8:return source,[]
            if (site+5+int.from_bytes(raw[1:],'little',signed=True))&0xffffffff!=target:return source,[]
        except (KeyError,TypeError,ValueError):return source,[]
    for match in matches:
        if source[match.start():match.end()]!=match[0]:return source,[]
        # Typed exports attach stale registers to this zero-argument import.
        # Never remove an argument expression with calls, memory reads or writes.
        args=match['args'].strip()
        if args and not all(RAW_PURE.fullmatch(arg.strip()) for arg in args.split(',')):return source,[]
    for match in reversed(matches):source=source[:match.start()]+'GSI->RetailRandModulo(32768)'+source[match.end():]
    return source,[dict(kind='raw CRT draw',modulus=RAW_MODULUS,draws=len(matches),callee=hex(RAND_THUNK),callSites=[c['site'] for c in calls])]


def fold_retail_random_with_raw_fallback(source, fn, read_bytes):
    result, evidence = fold_retail_random(source, fn, read_bytes)
    if result == source:
        if evidence:
            return result, evidence
        return recover_raw_retail_random(source, fn, read_bytes)
    # Reviewed special-case transforms keep their own acceptance gates. Only
    # the generic one-draw pairs participate in this incremental completion.
    keys = {'temporary', 'result', 'modulus', 'callee'}
    if not evidence or not all(set(row) == keys for row in evidence):
        return result, evidence
    _, proof = recover_raw_retail_random(source, fn, read_bytes)
    if not proof:
        return result, evidence
    code = PROTECTED.sub(lambda match: ' '*len(match[0]), result)
    remaining = list(RAW_CALL.finditer(code))
    if not remaining or len(remaining) + len(evidence) != proof[0]['draws']:
        return result, evidence
    native_api = re.compile(r'\bGSI->RetailRandModulo\s*\(')
    original = PROTECTED.sub(lambda match: ' '*len(match[0]), source)
    if len(native_api.findall(code)) - len(native_api.findall(original)) != len(evidence):
        return result, evidence
    for match in reversed(remaining):
        result = result[:match.start()] + 'GSI->RetailRandModulo(32768)' + result[match.end():]
    return result, evidence + [dict(kind='remaining raw CRT draws', modulus=32768,
                                   draws=len(remaining), pairedDraws=len(evidence),
                                   verifiedFunctionCallSites=proof[0]['callSites'])]
