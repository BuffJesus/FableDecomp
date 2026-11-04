"""Recover DragonBossFight's two summoner positions and creature key."""
import hashlib

TODO_LINE = '            -- TODO(native): pCVar4 = (**(*(quest:GetStateListRef("SummonerSpawnPoints") + quest:GetStateInt("NextSummonerSpawnPoint") * 0xc) + 0x18))()\n'
POS_LINE = '            pCVar4 = nil --[[unresolved native value]]\n'
GET_POS = '            pCVar4 = quest:GetStateListAt("SummonerSpawnPoints", quest:GetStateInt("NextSummonerSpawnPoint")):GetPos()\n'
EFFECT_OLD = '            r1 = quest:CreateEffectAtPos("SUMMON_CREATURE_SUMMON_EVIL", nil --[[missing]], 0.0, bVar2)\n'
EFFECT_NEW = '            r1 = quest:CreateEffectAtPos("SUMMON_CREATURE_SUMMON_EVIL", pCVar4, 0.0, bVar2)\n'
CREATURE_OLD = '            r2 = quest:CreateCreature(pCVar4, nil --[[missing]], "DBSummoner", bVar2)\n'
CREATURE_NEW = '            r2 = quest:CreateCreature(string, pCVar4, "DBSummoner", bVar2)\n'


def restore(source, todo, calls, fn, row, rdata, fields):
    if row is None:
        return source, todo, calls
    def require(ok, reason):
        if not ok:
            raise ValueError('Dragon SpawnSummoners: ' + reason)
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'], 'source changed')
    begin, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    require(hashlib.sha256(rdata.bytes_at(begin, end - begin)).hexdigest() == row['nativeSha256'], 'native body changed')
    require(row['spawnList'] == 'SummonerSpawnPoints' and row['indexField'] == 'NextSummonerSpawnPoint' and
            fields.get('0x58') is None and fields.get('0x64') == ['NextSummonerSpawnPoint', 'Int'],
            'field mapping changed')
    indirect = {c['site'].lower(): c for c in fn['indirectCalls']}
    require(all(indirect.get(site, {}).get('slot') == '0x18' for site in row['indirectSites']), 'position calls changed')
    require(indirect.get(row['effectSite'], {}).get('slot') == '0x190' and
            indirect.get(row['creatureSite'], {}).get('slot') == '0x16c', 'spawn calls changed')
    require(fn['decompile'].count('*(int *)(this + 0x58) + *(int *)(this + 100) * 0xc') == 2,
            'summoner list indexing changed')
    require('CCharString::CCharString((CCharString *)&local_30,string,-1);' in fn['decompile'] and
            '"CREATURE_SUMMONER_01"' in fn['decompile'] and
            '"CREATURE_SUMMONER_02"' in fn['decompile'], 'creature choice changed')
    require(source.count(TODO_LINE + POS_LINE) == 2 and source.count(EFFECT_OLD) == 1 and
            source.count(CREATURE_OLD) == 1, 'lifted spawn shape changed')
    require(todo.count('CreateEffectAtPos: missing position (3 args for 5 params)') == 1 and
            todo.count('CreateCreature: missing position (3 args for 4 params)') == 1 and
            sum(x.startswith('unlifted: pCVar4 = (**(*(QUESTLIST_BeginValue("SummonerSpawnPoints")') for x in todo) == 2,
            'diagnostics changed')
    first = TODO_LINE + POS_LINE + EFFECT_OLD
    second = TODO_LINE + POS_LINE + CREATURE_OLD
    source = source.replace(first, GET_POS + EFFECT_NEW, 1).replace(second, GET_POS + CREATURE_NEW, 1)
    todo = [x for x in todo if x not in ('CreateEffectAtPos: missing position (3 args for 5 params)',
                                           'CreateCreature: missing position (3 args for 4 params)') and
            not x.startswith('unlifted: pCVar4 = (**(*(QUESTLIST_BeginValue("SummonerSpawnPoints")')]
    calls = [*calls, 'GetStateListAt', 'GetPos']
    return source, todo, calls
