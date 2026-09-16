"""Factor identical verified acquisition loops without moving cancellation checks."""
import re

BODY='''resources:PrepareResource(barrel_resource)
cVar2 = resources:TryAcquire(barrel_resource, me, 4)
while not cVar2 do
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if not alive then goto LAB_00db6afd end
    cVar2 = resources:TryAcquire(barrel_resource, me, 4)
end
alive = not quest:IsActiveThreadTerminating()
if not alive then goto LAB_00db6afd end
'''

HELPER='''    local function acquireBarrelControl()
        resources:PrepareResource(barrel_resource)
        while not resources:TryAcquire(barrel_resource, me, 4) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then return false end
        end
        return not quest:IsActiveThreadTerminating()
    end
'''


def recover(source):
    lines=BODY.splitlines()
    pattern=r'(?m)^(?P<indent> *)'+re.escape(lines[0])+'\n'
    pattern+=''.join(r'(?P=indent)'+re.escape(line)+'\n' for line in lines[1:])
    matches=list(re.finditer(pattern,source))
    anchor='    local bVar3, cVar2,'
    if len(matches)!=5 or source.count(anchor)!=1:
        raise ValueError('Barrel acquisition loop correspondence changed')
    source=re.sub(pattern,lambda m:m['indent']+'if not acquireBarrelControl() then goto LAB_00db6afd end\n',source)
    source=source.replace(anchor,HELPER+anchor,1)
    return source,dict(status='structured-acquisition',sites=5,semantics='Prepare once, acquire priority4 before frame, cancellation after each failed-acquire frame and once after success.')
