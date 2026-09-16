"""Pin Init/GivenTeddy lifecycle and eliminate filtering in Init's actor calls."""
import hashlib,json
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData

def prove(data=None):
    data=data or RData();w=json.loads(Path(__file__).with_name('bully_lifecycle_witness.json').read_text())
    for row in w['ranges']:
        if hashlib.sha256(data.bytes_at(row['address'],row['size'])).hexdigest()!=row['sha256']:
            raise ValueError('Bully native lifecycle bytes changed')
    if data.string_at(0x12d8958)!='OBJECT_TEDDY_BEAR_UNGIVEABLE':raise ValueError('Bully Teddy removal literal changed')
    return w

def lower(source,data=None):
    proof=prove(data)
    old='''    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetThingHasInformation(me, false, false, false)
    local hero = quest:GetHero()
    quest:EntitySetThingAsAllyOfThing(me, hero)
    quest:SetIsPushableByHero(me, false)'''
    if source.count(old)!=1:raise ValueError('Bully Init API sequence correspondence changed')
    source=source.replace(old,'''    quest:WithRetailResources(function(resources)
        resources:InitializeBullyActor(me)
    end)''')
    return source,dict(proof,initFields={'DoneIntro':[0x24,False],'HitsTaken':[0x20,0],'InitialHealth':[0x1c,4],
        'SpokenOnFirstProximity':[0x26,False],'parent.SpokeAboutFindingTeddy':[0x70,False],
        'SaidPieceAboutAttackingVictim':[0x25,False],'IntimidateSpeechLoop':[0x28,10]},
        givenTeddy={'gold':1,'takeObjectScope':[0xdbcd1c,0xdbcd35],
            'parentFlags':{'SpokeAboutFindingTeddy':0x70,'TeddyRuined':0x91},'addBadDeed':[0xdbcd50,3]},
        limits='By-value pushability argument is owned/consumed by native callee; state/save-load and helper integration remain host gates.')
