"""Turn verified BookTrader joins into bounded phases and ordinary loop exits."""
import hashlib
import re
import textwrap

from tools.script_recovery.readable_lua import tokens


def phase_body(source):
    """An exit from the caller's loop becomes false; nested loop breaks stay local."""
    source = re.sub(r'(?m)^ *release_book\(\)\n', '', source)
    source = ''.join('return false' if t.lastgroup=='identifier' and t[0]=='return' else t[0]
                     for t in tokens(source))
    source = source.replace('goto LAB_00db4f5a','return false')
    result, stack = [], []
    for raw in source.splitlines(True):
        code=''.join(t[0] for t in tokens(raw) if t.lastgroup not in ('comment','longcomment')).strip()
        in_loop=any(kind in ('while','repeat') for kind in stack)
        if code=='break' or re.fullmatch(r'if .+ then break end',code):
            if not in_loop:
                raw=''.join('return false' if t.lastgroup=='identifier' and t[0]=='break' else t[0]
                            for t in tokens(raw))
        elif code.startswith('while ') and code.endswith(' do'):
            stack.append('while')
        elif code=='repeat':
            stack.append('repeat')
        elif code=='do':
            stack.append('do')
        elif code.startswith('if ') and code.endswith(' then'):
            stack.append('if')
        elif code=='end' or code.startswith('until '):
            if not stack or (code.startswith('until ') != (stack[-1]=='repeat')):
                raise ValueError('BookTrader phase block boundary changed')
            stack.pop()
        elif code.startswith(('local function ','function ','for ')):
            raise ValueError('BookTrader phase contains unsupported scope')
        result.append(raw)
    if stack:
        raise ValueError('BookTrader phase has unmatched blocks')
    return ''.join(result)


def structure_book(source):
    original=source
    markers=['        while true do\n', '            ::LAB_00db4234::\n',
             '            ::LAB_00db4ce6::\n', '            ::LAB_00db4e64::\n',
             '        ::LAB_00db4f5a::\n']
    positions=[]
    for marker in markers:
        matches=list(re.finditer('(?m)^'+re.escape(marker),source))
        if len(matches)!=1:
            raise ValueError('BookTrader phase marker changed: '+marker.strip())
        positions.append(matches[0].start())
    if positions!=sorted(positions):
        raise ValueError('BookTrader phase order changed')
    setup=phase_body(source[positions[0]+len(markers[0]):positions[1]])
    interaction=phase_body(source[positions[1]+len(markers[1]):positions[2]])
    intermittent=phase_body(source[positions[2]+len(markers[2]):positions[3]])
    frame=source[positions[3]+len(markers[3]):positions[4]]
    if not frame.endswith('        end\n'):
        raise ValueError('BookTrader frame loop boundary changed')
    frame=frame[:-len('        end\n')]
    if setup.count('goto LAB_00db4234')!=1:
        raise ValueError('BookTrader home-to-interaction join changed')
    setup=setup.replace('goto LAB_00db4234','return true')
    trade=re.search(r'(?m)^( *)::LAB_00db4c85::\n',interaction)
    if trade is None:
        raise ValueError('BookTrader trade cleanup join changed')
    tail=(trade[1]+'quest:FaceThingByScriptName(me, "NOVI_Theresa", false)\n'+
          trade[1]+'resources:Pause(false)\n'+
          trade[1]+'resources:DestroyMovie(book_movie); book_movie = nil\n'+
          trade[1]+'goto LAB_00db4ce6\n')
    if interaction[trade.end():trade.end()+len(tail)]!=tail:
        raise ValueError('BookTrader trade cleanup order changed')
    interaction=(interaction[:trade.start()]+trade[1]+'return finishTrade()\n'+
                 interaction[trade.end()+len(tail):])
    interaction=interaction.replace('goto LAB_00db4c85','return finishTrade()')
    interaction=interaction.replace('goto LAB_00db4ce6','return true')
    intermittent=intermittent.replace('goto LAB_00db4e64','return true')
    tail=tail.replace('goto LAB_00db4ce6','return true')
    def helper(name,body,fallthrough=True):
        return ('        local function '+name+'()\n'+textwrap.indent(textwrap.dedent(body),'            ')+
                ('            return true\n' if fallthrough else '')+'        end\n')
    helpers=(helper('prepareAndReturnHome',setup)+helper('finishTrade',tail,False)+
             helper('processInteraction',interaction)+helper('tryIntermittentLine',intermittent))
    replacement=(helpers+'        while true do\n'+
        '            if not prepareAndReturnHome() then break end\n'+
        '            if not processInteraction() then break end\n'+
        '            if not tryIntermittentLine() then break end\n'+frame+'        end\n')
    source=source[:positions[0]]+replacement+source[positions[4]+len(markers[4]):]
    if any(t.lastgroup=='identifier' and t[0]=='goto' or t[0]=='::' for t in tokens(source)):
        raise ValueError('BookTrader executable jump remains')
    return source, {'inputSha256':hashlib.sha256(original.encode()).hexdigest(),
                    'outputSha256':hashlib.sha256(source.encode()).hexdigest(),
                    'phases':['prepareAndReturnHome','processInteraction','tryIntermittentLine'],
                    'cleanup':'false exits outer loop to release_book; nested breaks retain their targets',
                    'nativeJoins':[m.strip() for m in markers[1:]]}
