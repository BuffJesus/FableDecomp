"""Measure the unreviewed Guild lift against the native ownership inventory.

This is a diagnostic draft under work/, not an installable quest package.
"""
import json

from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker
from tools.script_recovery.guild_training_inventory import ROOT, EVIDENCE, recover
from tools.script_recovery.lift_native_lua import lift_cluster, lift_entity, write_package


def build():
    inventory = recover()
    output = ROOT / 'work/guild_training_baseline'
    output.mkdir(parents=True, exist_ok=True)
    entities = output / 'entities'
    entities.mkdir(exist_ok=True)
    quest_reports, entity_reports, sources = [], [], {}
    for quest in inventory['quests']:
        report = lift_cluster(quest['script'])
        write_package(report, output / report['package'])
        quest_reports.append({'script': quest['script'],
                              'missingBindings': [e['name'] for e in quest['entities'] if e['name'] not in report['entities']],
                              'diagnostics': sum(len(f['todo']) for f in report['functions'].values()),
                              'state': report['state'], 'threads': report['threads']})
        for entity in quest['entities']:
            if entity['missingExports']:
                raise ValueError(f'Finish native export before lifting {entity["name"]}')
            key = quest['script'] + '.' + entity['name']
            lifted = lift_entity(EVIDENCE / 'translation_unit.json', key,
                                 entity['functions']['Init'], entity['functions']['Main'])
            sources[key] = lifted.pop('lua')
            entity_reports.append(lifted)
            (entities / (key + '.lua')).write_text(sources[key], encoding='utf-8')
    result = {'status': 'unreviewed diagnostic baseline; syntax does not establish behavior',
              'quests': quest_reports, 'entities': entity_reports,
              'syntax': LuaSyntaxChecker().check(sources),
              'entityDiagnostics': sum(len(f['todo']) for r in entity_reports for f in r['functions'].values())}
    (output / 'report.json').write_text(json.dumps(result, indent=2)+'\n', encoding='utf-8')
    print(json.dumps({'entityDiagnostics': result['entityDiagnostics'], 'syntax': result['syntax']}, indent=2))
    return result


if __name__ == '__main__':
    build()
