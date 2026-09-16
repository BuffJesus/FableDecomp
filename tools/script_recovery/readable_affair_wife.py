"""Split wife helper storage only when no value crosses helper boundaries."""
import re
import textwrap
from collections import deque

from tools.script_recovery.lua_local_versions import flow_graph
from tools.script_recovery.readable_lua import GENERATED,readable_source,rename_identifiers,tokens,wrap_local_declarations


def assigned_before_reads(body,name):
    code=''.join(re.sub(r'[^\n]',' ',t[0]) if t.lastgroup in ('comment','longcomment','string','longstring') else t[0] for t in tokens(body))
    lines=code.splitlines();graph=flow_graph(lines)
    if graph is None:return False
    assignment=re.compile(r'^\s*'+re.escape(name)+r'\s*=(?!=)')
    queue=deque([(0,False)]);seen=set()
    while queue:
        index,defined=queue.popleft()
        if (index,defined) in seen:continue
        seen.add((index,defined));line=lines[index]
        rhs=assignment.sub('',line,count=1)
        if not defined and rename_identifiers(rhs,{name:'__read__'})!=rhs:return False
        defined=defined or bool(assignment.match(line))
        queue.extend((target,defined) for target in graph[index])
    return True


def readable_wife_source(source):
    declaration=re.search(r'(?m)^    local CVar29, [^\n]+\n',source)
    if declaration is None:raise ValueError('Wife generated local declaration changed')
    names=declaration[0].strip().removeprefix('local ').split(', ')
    without_declaration=source.replace(declaration[0],'',1)
    unused=[n for n in names if rename_identifiers(without_declaration,{n:'__probe__'})==without_declaration]
    helper_names=('waitUntilNearHusband','processHeroInteraction','runBody')
    starts=[source.index('    local function '+name+'()\n') for name in helper_names]
    ends=starts[1:]+[source.index('    runBody()\n',starts[-1])]
    blocks={name:source[start:end] for name,start,end in zip(helper_names,starts,ends)}
    outside=source[:starts[0]]+source[ends[-1]:]
    outside=outside.replace(declaration[0],'',1)
    # The synchronous argument callback captures counter/conversation values.
    # Exclude every value mentioned in it; its internals cannot then affect
    # definite-assignment analysis of any eligible value.
    callback=re.search(r'(?m)^(?P<i> *)local continueArgument = resources:WithArgumentKey[^\n]+\n[\s\S]*?^(?P=i)end\)\n',blocks['runBody'])
    if callback is None:raise ValueError('Wife argument callback changed')
    marker=callback['i']+'local continueArgument = true\n'
    analysis=dict(blocks)
    analysis['runBody']=analysis['runBody'].replace(callback[0],marker,1)
    uses={n:[h for h,b in blocks.items() if rename_identifiers(b,{n:'__probe__'})!=b] for n in names}
    independent=[n for n in names if GENERATED.fullmatch(n) and uses[n]
        and rename_identifiers(outside,{n:'__probe__'})==outside
        and rename_identifiers(callback[0],{n:'__probe__'})==callback[0]
        and all(assigned_before_reads(textwrap.dedent(analysis[h]),n) for h in uses[n])]
    localized={};helper_maps={}
    for name in helper_names:
        local_names=[n for n in independent if name in uses[n]]
        if not local_names:continue
        body=textwrap.dedent(analysis[name]);first=body.index('\n')+1
        staged=body[:first]+'    local '+', '.join(local_names)+'\n'+body[first:]
        output,mapping=readable_source(staged,inline_literals=True)
        if name=='runBody':
            # No callback-captured generated value was localized or renamed.
            dedented_marker=textwrap.dedent(analysis[name]).splitlines(keepends=True)
            marker_line=next(line for line in dedented_marker if 'local continueArgument = true' in line)
            if output.count(marker_line)!=1:raise ValueError('Wife callback restoration changed')
            margin=len(blocks[name].splitlines()[0])-len(blocks[name].splitlines()[0].lstrip())
            # textwrap.dedent uses the minimum indentation (four for this draft).
            restored='\n'.join(line[margin:] if line.startswith(' '*margin) else line for line in callback[0].split('\n'))
            output=output.replace(marker_line,restored,1)
        source=source.replace(blocks[name],textwrap.indent(output,'    '),1)
        localized[name]=local_names;helper_maps[name]=mapping
    remaining=[n for n in names if n not in independent and n not in unused]
    source=source.replace(declaration[0],'    local '+', '.join(remaining)+'\n' if remaining else '',1)
    source,mapping=readable_source(source,inline_literals=True)
    source,wraps=wrap_local_declarations(source)
    return source,{'functions':mapping,'localizedHelpers':localized,'helperMaps':helper_maps,
                   'removedUnusedDeclarations':unused,'declarationWraps':wraps}
