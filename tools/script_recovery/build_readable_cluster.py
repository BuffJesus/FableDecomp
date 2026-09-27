"""Generate readable Lua for the older named-cluster pipeline.

Keeps native evidence and unresolved diagnostics. Invalid native drafts remain
explicitly invalid and are reported, rather than cosmetically repaired. Output
must be supplied so invoking this tool cannot overwrite a current port by default.
"""
import argparse
import json
from pathlib import Path

from tools.script_recovery.lift_native_lua import lift_cluster, write_package
from tools.script_recovery.build_readable_unit import readable_file
from tools.script_recovery.audit_readability import inspect_source
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker


def build(script, out):
    native = lift_cluster(script)
    write_package(native, out)
    checker = LuaSyntaxChecker()
    report = {'schema': 'readable-cluster/1', 'script': script, 'package': native['package'],
              'qualification': 'Presentation of recovered native drafts, not gameplay certification.', 'files': {}}
    for path in sorted((out/'FSE').rglob('*.lua')):
        rel = path.relative_to(out).as_posix()
        source = path.read_text(encoding='utf-8-sig')
        row = {'before': inspect_source(source)['counts'],
               'nativeSyntax': checker.check({rel: source})}
        if row['nativeSyntax']['ok']:
            result, transformations = readable_file(source)
            row['syntax'] = checker.check({rel: result})
            if not row['syntax']['ok']:
                raise ValueError(f'Readability introduced a syntax failure in {rel}: {row["syntax"]}')
            row['transformations'] = transformations
            path.write_text(result, encoding='utf-8')
        else:
            result = source
            row['syntax'] = row['nativeSyntax']
            row['notStyled'] = 'Native draft needs recovery; retained without hiding its invalid code.'
        row['after'] = inspect_source(result)['counts']
        report['files'][rel] = row
    report['summary'] = {'files': len(report['files']),
        'syntaxFailures': sum(not row['syntax']['ok'] for row in report['files'].values())}
    (out/'READABLE_CLUSTER_REPORT.json').write_text(json.dumps(report, indent=2)+'\n', encoding='utf-8')
    return report


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--script', required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    print(json.dumps(build(args.script, args.out)['summary'], indent=2))
