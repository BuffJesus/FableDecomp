import pytest

from tools.script_recovery.native_named_resource_receivers import named_resources
from tools.script_recovery.test_lift_native_lua import make


SOURCE = '''{
GSI->GetThingWithScriptName((CScriptThing *)actor, "OtherActor");
resource = RESOURCE_NewResource();
RESOURCE_TryAcquire(resource, (CScriptThing *)actor, 4);
CScriptGameResourceObjectScriptedThingBase::_Speak_CScriptGameResourceObjectScriptedThingBase((CScriptGameResourceObjectScriptedThingBase *)resource, hero, "LINE", 0, 0, 1, 0);
busy = CScriptThing::_IsPerformingScriptTask_CScriptGameResourceObjectScriptedThingBase((CScriptThing *)resource);
}'''


@pytest.mark.parametrize('newline', ['\n', '\r\n'])
def test_named_acquisition_retains_resource_receiver(newline):
    source = SOURCE.replace('\n', newline)
    assert named_resources(source) == {'resource'}
    lifter = make(entity=True)
    lifter.locals.add('hero')
    lifter.kinds['hero'] = 'thing'
    body = '\n'.join(lifter.lift('Main', source, reviewed_resource_receivers=named_resources(source)))
    assert 'resources:Speak(resource, hero, "LINE", 0, false, true, false)' in body
    assert 'resources:IsPerformingScriptTask(resource)' in body
    assert 'me:Speak' not in body
    assert 'me:IsPerformingScriptTask' not in body


@pytest.mark.parametrize('source', [
    SOURCE.replace('resource = RESOURCE_NewResource();', ''),
    SOURCE.replace('resource = RESOURCE_NewResource();', 'resource = RESOURCE_StartMovie("");'),
    SOURCE.replace('RESOURCE_TryAcquire(resource, (CScriptThing *)actor, 4);', ''),
    SOURCE.replace('(CScriptThing *)actor, 4', '(CScriptThing *)(this + 8), 4'),
    SOURCE.replace('resource = RESOURCE_NewResource();', 'resource = RESOURCE_NewResource();\nresource = 7;'),
    SOURCE.replace('GetThingWithScriptName', 'UnknownGetter'),
    SOURCE.replace('_Speak_CScriptGameResourceObjectScriptedThingBase', '_Unknown_CScriptGameResourceObjectScriptedThingBase'),
    SOURCE.replace('hero, "LINE", 0, 0, 1, 0', 'hero, "LINE"'),
    SOURCE.replace('RESOURCE_TryAcquire(resource,', 'RESOURCE_TryAcquire(other,'),
    SOURCE.replace('resource = RESOURCE_NewResource();', '') + '\nresource = RESOURCE_NewResource();',
    SOURCE.replace('GSI->GetThingWithScriptName((CScriptThing *)actor, "OtherActor");', '')
        + '\nGSI->GetThingWithScriptName((CScriptThing *)actor, "OtherActor");',
])
def test_unproven_resource_is_not_selected(source):
    assert named_resources(source) == set()


def test_entity_self_resource_keeps_existing_dispatch():
    source = SOURCE.replace('(CScriptThing *)actor, 4', '(CScriptThing *)(this + 8), 4')
    lifter = make(entity=True)
    body = '\n'.join(lifter.lift('Main', source))
    assert 'me:Speak(' in body
    assert 'resources:Speak(' not in body


def test_named_receiver_requires_reviewed_opt_in():
    lifter = make(entity=True)
    body = '\n'.join(lifter.lift('Main', SOURCE))
    assert 'me:Speak(' in body
    assert 'resources:Speak(' not in body
