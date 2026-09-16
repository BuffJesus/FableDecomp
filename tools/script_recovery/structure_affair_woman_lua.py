"""Replace verified woman cleanup joins with bounded helpers and a run-off loop."""
import hashlib
import re
import textwrap

from tools.script_recovery.readable_lua import tokens


def structure_woman(source):
    original=source
    markers=['    woman_resource = resources:NewResource()\n',
        '    goto LAB_00db2974\n    ::LAB_00db26f8::\n',
        '    ::LAB_00db282e::\n','    ::LAB_00db28de::\n','    ::LAB_00db296b::\n',
        '    ::LAB_00db2974::\n','    ::LAB_00db2986::\n']
    positions=[]
    for marker in markers:
        matches=list(re.finditer('(?m)^'+re.escape(marker),source))
        if len(matches)!=1:raise ValueError('Woman structural marker changed: '+marker.strip())
        positions.append(matches[0].start())
    if positions!=sorted(positions):raise ValueError('Woman structural phase order changed')
    lookup=re.search(r'(?m)^    \w+ = resources:NewThingFromScriptName\("NOVI_AffairWife"\)\n'
                     r'    \w+ = resources:NewThingFromScriptName\("NOVI_AffairMan"\)\n',source)
    if lookup is None or not positions[0]<lookup.start()<positions[1]:raise ValueError('Woman retained lookup phase changed')
    acquire=source[positions[0]+len(markers[0]):lookup.start()]
    acquire=acquire.replace('goto LAB_00db2986','return')
    interaction=source[lookup.end():positions[1]]
    point_cleanup=source[positions[4]+len(markers[4]):positions[5]]
    partner_cleanup=source[positions[5]+len(markers[5]):positions[6]]
    shared=source[positions[1]+len(markers[1]):positions[2]]
    if shared!='    finish_movie()\n    goto LAB_00db2974\n    ::LAB_00db2732::\n    finish_movie()\n    goto LAB_00db2974\n':
        raise ValueError('Woman shared movie cleanup changed')
    for label in ('LAB_00db26f8','LAB_00db2732'):
        interaction=interaction.replace('goto '+label,'finish_movie(); return')
    interaction=interaction.replace('goto LAB_00db2974','return')
    jump=re.search(r'(?m)^( *)goto LAB_00db282e\n',interaction)
    if jump is None or interaction.count('goto LAB_00db282e')!=1:raise ValueError('Woman run-off entry changed')
    replacement=jump[1]+'runOffAndWaitForCamera()\n'+textwrap.indent(textwrap.dedent(point_cleanup),jump[1])+jump[1]+'return\n'
    interaction=interaction[:jump.start()]+replacement+interaction[jump.end():]
    runoff=source[positions[2]+len(markers[2]):positions[3]]
    guard=re.match(r'    if not (\w+) then goto LAB_00db28de end\n',runoff)
    if guard is None or not runoff.endswith('    goto LAB_00db282e\n'):raise ValueError('Woman run-off back edge changed')
    loop=runoff[guard.end():-len('    goto LAB_00db282e\n')]
    loop=loop.replace('goto LAB_00db296b','return')
    runoff='    while '+guard[1]+' do\n'+textwrap.indent(loop,'    ')+'    end\n'
    camera=source[positions[3]+len(markers[3]):positions[4]].replace('goto LAB_00db296b','return')
    def helper(name,body):
        return '    local function '+name+'()\n'+textwrap.indent(textwrap.dedent(body),'        ')+'    end\n'
    functions=(helper('runOffAndWaitForCamera',runoff+camera)+helper('runInteractions',interaction)+
               helper('acquireAndRun',acquire+lookup[0]+'    runInteractions()\n'+partner_cleanup))
    source=(source[:positions[0]]+functions+markers[0]+'    acquireAndRun()\n'+
            source[positions[6]+len(markers[6]):])
    if any(t.lastgroup=='identifier' and t[0]=='goto' or t[0]=='::' for t in tokens(source)):
        raise ValueError('Woman executable jump remains')
    return source,{'inputSha256':hashlib.sha256(original.encode()).hexdigest(),
        'outputSha256':hashlib.sha256(source.encode()).hexdigest(),
        'helpers':['acquireAndRun','runInteractions','runOffAndWaitForCamera'],
        'cleanup':'run-off marker then husband then wife then controlled resource; movie ends before partner cleanup'}
