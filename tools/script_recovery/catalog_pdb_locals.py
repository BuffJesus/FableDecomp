"""Catalog DIA local symbols as cross-build evidence, never retail stack mappings.

Input dumps come from runtime_checks/pdb_locals.cpp. PDB hashes identify each
build; lexical scope IDs preserve repeated names and reused stack locations.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
HEADER = 'depth\ttag\tname\ttype\tbaseType\tsize\tdataKind\tlocation\tregister\toffset\trva'
PREFIX = 'NScript::CQ_NewOakValeIntroScript'


def parse_dump(source):
    lines = source.splitlines()
    if not lines or lines[0] != HEADER:
        raise ValueError('Unexpected DIA dump header')
    functions, current, scopes, ended = [], None, [], False
    for line in lines[1:]:
        if ended:
            raise ValueError('Data after MATCHES footer')
        columns = line.split('\t')
        if columns[0] == 'FUNCTION':
            if len(columns) != 4:
                raise ValueError('Invalid FUNCTION row')
            current = dict(name=columns[1], rva=int(columns[2]), size=int(columns[3]),
                           scopes=[dict(id=0, parent=None)], locals=[])
            functions.append(current)
            scopes = [0]
        elif columns[0] == 'MATCHES':
            if len(columns) != 2 or int(columns[1]) != len(functions):
                raise ValueError('DIA match count differs from function count')
            ended = True
        else:
            if current is None or len(columns) != 11:
                raise ValueError('Invalid symbol row')
            depth, tag = map(int, columns[:2])
            if depth < 0 or depth >= len(scopes):
                raise ValueError('Symbol has no enclosing lexical scope')
            scopes = scopes[:depth + 1]
            if tag == 6:
                scope = dict(id=len(current['scopes']), parent=scopes[-1],
                             rva=int(columns[10]), size=int(columns[5]))
                current['scopes'].append(scope)
                scopes.append(scope['id'])
            elif tag == 7:
                current['locals'].append(dict(
                    name=columns[2], type=columns[3], baseType=int(columns[4]),
                    size=int(columns[5]), dataKind=int(columns[6]),
                    location=int(columns[7]), register=int(columns[8]),
                    offset=int(columns[9]), rva=int(columns[10]), scope=scopes[-1]))
            else:
                raise ValueError('Unsupported DIA symbol tag')
    if not ended:
        raise ValueError('Incomplete DIA dump: no MATCHES footer')
    return functions


def symbol_name(owner, function):
    scope = PREFIX if owner == 'Q_NewOakValeIntro' else PREFIX + '::C' + owner
    return scope + '::' + ('~CQ_NewOakValeIntroScript' if function == 'destructor' else function)


def sha(path):
    digest = hashlib.sha256()
    with path.open('rb') as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b''):
            digest.update(chunk)
    return digest.hexdigest()


def catalog(inputs, inventory):
    builds = []
    for pdb, dump in inputs:
        functions = parse_dump(dump.read_text(encoding='utf-8'))
        builds.append(dict(pdb=pdb.name, pdbSha256=sha(pdb), dumpSha256=sha(dump),
                           functions=functions))
    correspondence = []
    for row in inventory:
        name = symbol_name(row['owner'], row['function'])
        matches = []
        for build in builds:
            found = [f for f in build['functions'] if f['name'] == name]
            if len(found) > 1:
                raise ValueError('Ambiguous function symbol: ' + name)
            if found:
                matches.append(dict(pdb=build['pdb'], symbol=name, rva=found[0]['rva']))
        correspondence.append(dict(owner=row['owner'], function=row['function'],
                                   retailAddress=row['address'], symbols=matches))
    return dict(schema='new-oakvale-pdb-locals/0.1', builds=builds,
                correspondence=correspondence,
                limits=['Names and types are original debug-build evidence.',
                        'Function matching uses qualified names, not address equality.',
                        'RVA, offsets and lexical ranges belong only to the identified PDB.',
                        'No local-to-retail mapping or gameplay parity is asserted.'])


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--input', nargs=2, metavar=('PDB', 'DUMP'), type=Path,
                        action='append', required=True)
    parser.add_argument('--report', type=Path, default=ROOT / 'refs/script_recovery/lifted/NewOakValeIntro/CONVERSION_REPORT.json')
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    result = catalog(args.input, json.loads(args.report.read_text())['functions'])
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({b['pdb']: dict(functions=len(b['functions']),
          locals=sum(len(f['locals']) for f in b['functions'])) for b in result['builds']}))
    print('Matched inventory functions:', sum(bool(r['symbols']) for r in result['correspondence']))


if __name__ == '__main__':
    main()
