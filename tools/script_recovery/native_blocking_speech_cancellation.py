"""Use the recovered native wait's own terminating exit after blocking speech."""
import re

def recover(source, manifest):
    spec=manifest.get('Speak',{})
    if spec.get('blocking') is not True or spec.get('scope')!='Entity' or spec.get('returnType')!='bool':
        return source,[]
    lines=source.splitlines(True);edits=[];evidence=[]
    for i,line in enumerate(lines):
        speech=re.fullmatch(r'( *)(?:(?:local )?(\w+) = )?(\w+):Speak\(([^\n]*)\)\n?',line)
        if not speech:continue
        indent,target,actor,args=speech.groups();j=i+1
        if j>=len(lines):continue
        query=re.fullmatch(re.escape(indent)+r'(?:local )?(\w+) = '+re.escape(actor)+r':IsPerformingScriptTask\(\)\n?',lines[j])
        if not query:continue
        busy=query[1];j+=1
        if j<len(lines):
            alias=re.fullmatch(re.escape(indent)+r'(?:local )?(\w+) = '+re.escape(busy)+r'\n?',lines[j])
            if alias:busy=alias[1];j+=1
        if j>=len(lines) or lines[j].rstrip('\r\n')!=indent+'while '+busy+' do':continue
        body=indent+'    ';k=j+1
        if k>=len(lines) or not re.fullmatch(re.escape(body)+r'(?:(?:local )?\w+ = )?quest:NewScriptFrame\((?:me)?\)\n?',lines[k]):continue
        k+=1
        if k>=len(lines):continue
        alive=re.fullmatch(re.escape(body)+r'(\w+) = not quest:IsActiveThreadTerminating\(\)\n?',lines[k])
        if not alive:continue
        k+=1;condition='not '+alive[1]
        if k<len(lines):
            inverse=re.fullmatch(re.escape(body)+r'(\w+) = not '+re.escape(alive[1])+r'\n?',lines[k])
            if inverse:condition=inverse[1];k+=1
        if k>=len(lines):continue
        inline=re.fullmatch(re.escape(body)+'if '+re.escape(condition)+r' then (.+) end\n?',lines[k])
        exit_lines=[]
        if inline:exit_lines=[indent+'    '+inline[1]+'\n']
        elif lines[k].rstrip('\r\n')==body+'if '+condition+' then':
            end=next((n for n in range(k+1,len(lines)) if lines[n].rstrip('\r\n')==body+'end'),None)
            if end is None:continue
            exit_lines=lines[k+1:end]
            if not exit_lines or any(not l.startswith(body+'    ') or l.lstrip().startswith(('local ','if ','while ','for ','repeat','::')) for l in exit_lines):continue
            exit_lines=[l[4:] for l in exit_lines]
        if not exit_lines:continue
        # Retain only an already recovered exiting cleanup. A guard whose body
        # continues the task is not a terminating exit we can reuse.
        if not re.search(r'\b(?:return|goto [A-Za-z_]\w*)\b',exit_lines[-1]):continue
        if any('TODO' in l or 'missing' in l for l in exit_lines):continue
        if target:
            prefix=[line,indent+'if not '+target+' then\n']
        else:
            prefix=[indent+'if not '+actor+':Speak('+args+') then\n']
        edits.append((i,prefix+exit_lines+[indent+'end\n']))
        evidence.append({'line':i+1,'actor':actor,'query':query[1],'loop':busy,'exit':''.join(exit_lines).strip(),'reason':'blocking Speak uses native wait termination cleanup before duplicate task poll'})
    for i,replacement in reversed(edits):lines[i:i+1]=replacement
    return ''.join(lines),evidence
