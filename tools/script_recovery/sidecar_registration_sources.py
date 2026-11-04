"""Static registrations in called helpers from local quoted headers.

This is source-profile inspection, not C++ preprocessing, control-flow analysis,
or proof that any DLL contains or executes a registration.
"""
from pathlib import Path
import re

TOKEN=re.compile(r'R"(?P<delimiter>[^\s()\\]{0,16})\([\s\S]*?\)(?P=delimiter)"|//[^\n]*|/\*[\s\S]*?\*/|"(?:[^"\\]|\\.)*"|\'(?:[^\'\\]|\\.)*\'')
CALL=re.compile(r'\b(Register\w+)\s*\(\s*(?:quest|thing|type|\w+_type)\b[^;{}]*\)\s*;')
FUNCTION=re.compile(r'\b(Register\w+)\s*\([^;{}]*\)\s*\{')
METHOD=re.compile(r'\b(?:\w+_type|quest|thing|type)\s*\[\s*"(\w+)"\s*\]\s*=(?!=)')
INCLUDE=re.compile(r'^\s*#\s*include\s*"([^"\n]+)"',re.M)

def views(text):
    def blank(match):return re.sub(r'[^\n]',' ',match[0])
    comments=TOKEN.sub(lambda match:blank(match) if match[0].startswith(('//','/*')) else match[0],text)
    return comments,TOKEN.sub(blank,text)

def included_registrations(sources):
    """{method: [{path, function}]} for local helpers called by the seed profile.

    Ordinary quoted headers are traversed only within seed source directories.
    Uncalled helpers, ambiguous definitions and external include trees add no
    methods. Cyclic includes/calls are visited once. Existing seed extraction is
    intentionally left to the caller to preserve its historical policy.
    """
    seeds={Path(path).resolve() for path in sources if Path(path).is_file()}
    roots={path.parent for path in seeds};pending=list(seeds);files={}
    while pending:
        path=pending.pop()
        if path in files:continue
        comments,code=views(path.read_text(encoding='utf-8',errors='replace'))
        files[path]=(comments,code)
        for include in INCLUDE.finditer(comments):
            if not code[include.start():include.end()].lstrip().startswith('#'):continue
            target=(path.parent/include[1]).resolve()
            if target.is_file() and any(target.is_relative_to(root) for root in roots):pending.append(target)
    definitions={}
    for path,(comments,code) in files.items():
        if path in seeds:continue
        for match in FUNCTION.finditer(code):
            start=match.end();depth=1;end=start
            while end<len(code) and depth:
                depth+=(code[end]=='{')-(code[end]=='}');end+=1
            if depth:continue
            definitions.setdefault(match[1],[]).append((path,comments[start:end-1],code[start:end-1]))
    pending={name for path in seeds for name in CALL.findall(files[path][1])};visited=set();result={}
    while pending:
        name=pending.pop()
        if name in visited:continue
        visited.add(name);candidates=definitions.get(name,[])
        if len(candidates)!=1:continue
        path,comments,code=candidates[0]
        for method in METHOD.finditer(comments):
            if code[method.start()].isspace():continue
            result.setdefault(method[1],[]).append(dict(path=str(path),function=name))
        pending.update(CALL.findall(code))
    return result
