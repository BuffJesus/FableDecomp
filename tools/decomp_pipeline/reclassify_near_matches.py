"""Recompile existing DIFFER rows and rerun candidate-required behavior fixtures.

Writes an independent audit, leaving old exact-byte reports intact. A numeric
similarity alone never promotes a row. Run from the repository root.
"""
import argparse
import csv
import hashlib
import json
import re
from pathlib import Path

import verify_and_land as V
from compare_candidate_objects import symbol_leaf


def read_rows(path):
    with path.open(encoding='utf-8-sig', newline='') as stream:
        return list(csv.DictReader(stream, delimiter='\t'))


def audit_candidates(prior_rows, catalog):
    """Also audit newly landed near matches absent from the old object report."""
    selected = [r for r in prior_rows if r['status'] == 'DIFFER']
    known = {r['address'].lower(): r for r in prior_rows}
    selected_addresses = {r['address'].lower() for r in selected}
    for address, entry in catalog.items():
        previous = known.get(address, {'address': address, 'status': 'CATALOG_NEAR_MATCH'})
        if (entry.get('Grade') == 'near_match' and address not in selected_addresses
                and previous['status'] not in ('MATCH', 'RELOCATION_MATCH')):
            selected.append(previous)
    return selected


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', type=Path, default=V.ROOT/'rebuild/compile-gate/near-match.json')
    parser.add_argument('--work', type=Path, default=V.ROOT/'work/near-match-audit')
    args = parser.parse_args()
    gate = V.ROOT/'rebuild/compile-gate'
    old = read_rows(gate/'retail-parity.tsv')
    oracles = {r['address'].lower(): r for r in read_rows(V.ROOT/'rebuild/oracles/auto-re-candidates.tsv')}
    catalog = {}
    for block in re.findall(r'\[pscustomobject\]@\{([^}]+)\}', (V.ROOT/'rebuild/build_candidates.ps1').read_text(encoding='utf-8')):
        fields = dict(re.findall(r"(\w+)\s*=\s*'([^']*)'", block))
        if 'Address' in fields:
            catalog[fields['Address'].lower()] = fields
    rows = []
    for prior in audit_candidates(old, catalog):
        address = prior['address'].lower()
        result = {'address': address, 'previous_status': prior['status'], 'status': 'UNVERIFIED'}
        rows.append(result)
        entry, oracle = catalog.get(address), oracles.get(address)
        if not entry or not oracle:
            result['reason'] = 'catalog/oracle missing'
            continue
        source = V.ROOT/'rebuild/src/compiled'/entry['Source']
        test = V.ROOT/'rebuild/tests'/entry['TestSource']
        if not source.exists() or not test.exists():
            result['reason'] = 'source/test missing'
            continue
        src, tst = source.read_text(encoding='utf-8'), test.read_text(encoding='utf-8')
        result.update(source=source.relative_to(V.ROOT).as_posix(), test=test.relative_to(V.ROOT).as_posix(), source_sha256=hashlib.sha256(src.encode('utf-8')).hexdigest(),
                      test_sha256=hashlib.sha256(tst.encode('utf-8')).hexdigest())
        if not V.purity.is_genuine(src) or V.purity.unsanctioned_header_asm(src):
            result['status'] = 'NOT_GENUINE'
            continue
        work = args.work/address
        work.mkdir(parents=True, exist_ok=True)
        original_compiler, original_bin = V.CL_EXE, V.CC_BIN
        try:
            if entry.get('Compiler') == 'qfe4035':
                V.use_qfe()
            flags = entry.get('CompilerFlags', '').split() or V.BASE_FLAGS
            status, _, _ = V.parity_of(src, address, symbol_leaf(oracle['name']),
                                       bytes.fromhex(oracle['bytes']), work, V.env(), base_flags=flags)
            result['parity'] = status
            score_path = work/f'{address}.similarity.json'
            if score_path.exists() and not status.startswith(('MATCH', 'RELOCATION_MATCH')):
                result['similarity'] = json.loads(score_path.read_text(encoding='utf-8'))
            behavior = V.behaviour_of(tst, address, entry['PassPattern'], work, V.env(),
                                      work/f'{address}.obj', require_candidate=True)
            result['behavior'] = behavior
            result['status'] = status if status.startswith(V.ACCEPTED_PARITY) and behavior == 'PASS' else 'DIFFER'
        finally:
            V.CL_EXE, V.CC_BIN = original_compiler, original_bin
        print(address, result['status'], result.get('behavior', ''), flush=True)
    report = {'schema_version': 1, 'thresholds': {'instruction_delta': 0.10, 'opcode_similarity': 0.90},
              'reviewed': len(rows), 'near_match': sum(r['status'] == 'NEAR_MATCH' for r in rows), 'rows': rows}
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(report, indent=2)+'\n', encoding='utf-8')
    print(f"Reviewed {len(rows)} prior DIFFER/new near-match rows; {report['near_match']} verified NEAR_MATCH")


if __name__ == '__main__':
    main()
