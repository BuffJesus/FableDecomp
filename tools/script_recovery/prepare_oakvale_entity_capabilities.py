"""Compose isolated entity adapters into the bounded common resource owner."""
import hashlib
import json
import re
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.prepare_post_attack_resource_extension import prepare as compose
from tools.script_recovery.live_father_init import ADAPTER as LIVE_FATHER_INIT
from tools.script_recovery.victim_init import ADAPTER as VICTIM_METHODS

OUTPUT=ROOT/'work/oakvale_entity_integration'
SOURCE=Path(__file__).parent
FRAGMENTS=('bully_runoff_methods.inc','bully_talk_predicate.inc','guard_methods.inc',
           'teddy_girl_methods.inc','live_father_methods.inc','affair_man_complete_methods.inc',
           'wife_init_methods.inc','wife_conversation_methods.inc','wife_animation_methods.inc',
           'dead_father_methods.inc')
METHODS=('AddBullyHealthBar',
 'AddConversationText',
 'AddLiveFatherGoodDeedCounter',
 'AddRawConversationPerson',
 'AddVictimRepeatConversationLines',
 'AddWifeWhereHusbandConversation',
 'BullyTalkedWithTeddy',
 'ClearRawInformation',
 'ClearThingHasInformation',
 'DestroyStringMap',
 'DisplayRawGameInfo',
 'FaceRetainedThing',
 'FaceTowardsRetainedThing',
 'FollowThing',
 'FormatBullyIntimidationText',
 'GiveBullyTeddyQuestion',
 'GiveRawHeroGold',
 'GiveTeddyGirlQuestion',
 'GuardAddHeroConversationLine',
 'GuardFaceHero',
 'InitializeAffairManActor',
 'InitializeBullyActor',
 'InitializeDeadFatherActor',
 'InitializeGuardActor',
 'InitializeLiveFatherActor',
 'InitializeTeddyGirlActor',
 'InitializeVictimActor',
 'InitializeWifeActor',
 'IsActorPositionOnScreen',
 'IsDistanceBetweenThingsUnder',
 'IsDistanceOver',
 'IsDistanceUnderThing',
 'IsTalkedToByHero',
 'LiveFatherHeroHasChocolate',
 'NewConversation',
 'NewLiteralText',
 'NewMovie',
 'NewStringMap',
 'PlaceDeadFatherAtMarker',
 'PlayAffairManAnimation',
 'PlayAnimationWithNativeArgument5',
 'PlayDeadFatherPose',
 'PlayWifeArgumentAnimation',
 'ReadBullyProximityRange',
 'ReadBullyRandomModulus',
 'ReadGuardAlertRange',
 'ReadGuardLectureRange',
 'RemoveDeadFatherMarker',
 'RemoveRawThing',
 'RunMacroWithStrings',
 'SetActiveQuestObjective',
 'SetRawCutsceneBehaviour',
 'SetRawScared',
 'SetString',
 'SetThingAsAlly',
 'SetVictimReleasedState',
 'SetWifeDeedReactionsDisabled',
 'StartOwnedMovie',
 'TakeTeddyFromHero',
 'TeddyGirlAddHeroLine',
 'TeddyGirlMoveToThing',
 'TeddyGirlTalkedWithTeddy',
 'TryAcquireThing',
 'VictimFaceHero')

SKIP={'bully_runoff_methods.inc':('NewText','DestroyText','AddConversationPerson','AddConversationLine'),
      'live_father_methods.inc':('DestroyText',)}


def remove_function(source,name):
    # Only reviewed top-level fragment members; mask strings/comments for braces.
    masked=re.sub(r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"',lambda m:' '*len(m[0]),source)
    matches=list(re.finditer(r'(?m)^[ \t]*(?:unsigned|int|void|bool|float)\s+'+re.escape(name)+r'\s*\(',masked))
    if len(matches)!=1:raise ValueError('Entity duplicate member changed: '+name)
    start=matches[0].start();opening=masked.index('{',matches[0].end());depth=1;end=opening+1
    while depth and end<len(masked):
        depth+=(masked[end]=='{')-(masked[end]=='}');end+=1
    if depth:raise ValueError('Entity duplicate member is unterminated: '+name)
    return source[:start]+source[end:]


def adapt_storage(source):
    # New adapters were proved in isolation against the older vector owner.
    # Common owner uses monotonic IDs and erases released entries.
    source=source.replace('return static_cast<unsigned>(m_entries.size());','return e.id;')
    source=source.replace('existing->live && existing->kind==Kind::Movie',
                          'existing.second->live && existing.second->kind==Kind::Movie')
    source=source.replace('void DestroyStringMap(unsigned id) { Destroy(Get(id, Kind::StringMap)); }',
                          'void DestroyStringMap(unsigned id) { Release(id, Kind::StringMap); }')
    return source


def storage_extension(source):
    before='enum class Kind { Resource, ActorMap, Movie, Thing, Text, TheresaGuardVector, TheresaPresented, BarrelWatchSnapshot };'
    if source.count(before)!=1:raise ValueError('Common resource kinds changed')
    source=source.replace(before,before.replace('BarrelWatchSnapshot }','BarrelWatchSnapshot, StringMap }'))
    before='        case Kind::Text: CCharString_Destroy(&e.text); break;'
    if source.count(before)!=1:raise ValueError('Common resource destruction changed')
    source=source.replace(before,before+'\n        case Kind::StringMap: StdMap_String_Destroy_API(e.map); Game_free(e.map); e.map=nullptr; break;')
    before='void DestroyText(unsigned id) { Destroy(Get(id, Kind::Text)); }'
    if source.count(before)!=1:raise ValueError('Common Text release changed')
    return source.replace(before,'void DestroyText(unsigned id) { Release(id, Kind::Text); }')


def prepare():
    bodies={name:(SOURCE/name).read_text() for name in FRAGMENTS}
    bodies['live_father_init.py:ADAPTER']=LIVE_FATHER_INIT
    bodies['victim_init.py:ADAPTER']=VICTIM_METHODS
    hashes={name:hashlib.sha256(body.encode()).hexdigest() for name,body in bodies.items()}
    for name,names in SKIP.items():
        for function in names:bodies[name]=remove_function(bodies[name],function)
    combined=adapt_storage('\n'.join(bodies.values()))
    header=(ROOT/'work/post_attack_world_integration/LuaRetailResources.h').read_text()
    registered=set(re.findall(r'type\["(\w+)"\]',header))
    methods=list(METHODS)
    for method in methods:
        if not re.search(r'\b'+re.escape(method)+r'\s*\(',combined):
            raise ValueError('Missing isolated entity capability: '+method)
    result=compose(base=ROOT/'work/post_attack_world_integration',output=OUTPUT,
        methods=methods,fragments=(),method_source=combined,transform=storage_extension,
        patch_name='oakvale-entity-integration.patch')
    result['entityComposition']={'inputs':hashes,'reusedCommonMembers':SKIP,
        'storageChanges':['Monotonic entry IDs','Map iteration for movie exclusivity',
                          'StringMap kind/destructor','Erase explicitly released Text and StringMap entries'],
        'limits':'Combined compile/runtime tests required; isolated evidence does not prove the composed owner.'}
    (OUTPUT/'proposal.json').write_text(json.dumps(result,indent=2)+'\n')
    return result


if __name__=='__main__':print(json.dumps(prepare(),indent=2))
