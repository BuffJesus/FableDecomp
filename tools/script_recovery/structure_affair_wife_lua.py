"""Replace wife phase/cleanup jumps with local helpers and explicit returns."""
import hashlib
import textwrap

from tools.script_recovery.readable_lua import tokens


def structure_wife(source):
    original=source
    def unique(marker):
        if source.count(marker)!=1:raise ValueError('Wife phase marker changed: '+marker.strip())
        return source.index(marker)
    end_label='    ::LAB_00db3e16::\n'
    unique(end_label)
    source=source.replace('goto LAB_00db3e16','return').replace(end_label,'')
    approach_start=unique('    until false\n    while true do\n')+len('    until false\n')
    approach_end=unique('    ::LAB_00db3593::\n')
    approach=source[approach_start:approach_end]
    # Every return in this phase is cancellation; normal loop exit means arrival.
    approach=approach.replace('return end','return false end')+'    return true\n'
    source=source[:approach_start]+source[approach_end+len('    ::LAB_00db3593::\n'):]
    jump='            if bVar4 then goto LAB_00db3593 end\n            break\n'
    unique(jump)
    source=source.replace(jump,'''            if not bVar4 then
                if not waitUntilNearHusband() then return end
            end
            break
''')
    interaction_start=unique('        native_arg_wife_hit = resources:IsHitByHeroExceptAbility(me, 14)\n')
    interaction_end=unique('        ::LAB_00db32aa::\n')
    interaction=source[interaction_start:interaction_end]
    # Phase exit false terminates runBody; the old shared join continues timers.
    import re
    interaction=re.sub(r'\breturn\b','return false',interaction)
    interaction=interaction.replace('goto LAB_00db32aa','return true')
    interaction+='        return true\n'
    source=source[:interaction_start]+'        if not processHeroInteraction() then return end\n'+source[interaction_end+len('        ::LAB_00db32aa::\n'):]
    anchor='    local function runBody()\n'
    unique(anchor)
    def helper(name,body):
        return '    local function '+name+'()\n'+textwrap.indent(textwrap.dedent(body),'        ')+'    end\n'
    source=source.replace(anchor,helper('waitUntilNearHusband',approach)+helper('processHeroInteraction',interaction)+anchor)
    if any(t[0]=='::' or t.lastgroup=='identifier' and t[0]=='goto' for t in tokens(source)):
        raise ValueError('Wife executable jumps remain')
    return source,{'inputSha256':hashlib.sha256(original.encode()).hexdigest(),
                   'outputSha256':hashlib.sha256(source.encode()).hexdigest(),
                   'helpers':['waitUntilNearHusband','processHeroInteraction','runBody']}
