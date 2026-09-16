"""Replace the verified husband's cleanup jumps with lexical helper returns.

Run after local-name recovery so introducing closures does not obscure reaching
definitions. Native lifetime verification remains the candidate generator's job.
"""
import hashlib
import re
import textwrap
from tools.script_recovery.readable_lua import tokens


def structure_cleanup(source):
    original = source
    def once(text):
        if source.count(text) != 1:
            raise ValueError('husband cleanup structure changed: ' + text.strip())
        return source.index(text)

    actor_start = once('            manConversationId = 0\n')
    actor_end = once('            ::releaseAffairActors::\n')
    if actor_start >= actor_end:
        raise ValueError('husband actor scope order changed')
    body = source[actor_start:actor_end]
    actor_returns = body.count('goto releaseAffairActors')
    if not actor_returns or 'goto releaseManControl' in body:
        raise ValueError('husband cleanup exit ownership changed')
    body = body.replace('goto releaseAffairActors', 'return')
    body = ''.join('    ' + line if line.strip() else line for line in body.splitlines(True))
    source = (source[:actor_start] + '            local function runInteractions()\n' + body +
              '            end\n            runInteractions()\n' +
              source[actor_end + len('            ::releaseAffairActors::\n'):])
    source = source.replace('        resources:DestroyThing(affairWife)\n',
                            '            resources:DestroyThing(affairWife)\n')
    source = source.replace('        resources:DestroyThing(affairWoman)\n',
                            '            resources:DestroyThing(affairWoman)\n')

    constructor = '        man_resource = resources:NewResource()\n'
    start = once(constructor) + len(constructor)
    end = once('        ::releaseManControl::\n')
    body = source[start:end]
    control_returns = body.count('goto releaseManControl')
    if control_returns != 1:
        raise ValueError('husband initial control exit changed')
    body = body.replace('goto releaseManControl', 'return')
    body = ''.join('    ' + line if line.strip() else line for line in body.splitlines(True))
    source = (source[:start] + '        local function acquireAndRunInteractions()\n' + body +
              '        end\n        acquireAndRunInteractions()\n' +
              source[end + len('        ::releaseManControl::\n'):])
    if re.search(r'\b(?:goto|::)\s*release(?:AffairActors|ManControl)\b', source):
        raise ValueError('husband cleanup jump remains')
    loop = re.search(r'(?m)^( *)::checkInteractions::\n', source)
    back = re.search(r'(?m)^ *goto checkInteractions\n', source)
    if (loop is None or back is None or loop.end() >= back.start()
            or source.count('goto checkInteractions') != 1):
        raise ValueError('husband interaction loop changed')
    body = source[loop.end():back.start()]
    body = ''.join('    ' + line if line.strip() else line for line in body.splitlines(True))
    source = (source[:loop.start()] + loop[1] + 'while true do\n' + body +
              loop[1] + 'end\n' + source[back.end():])
    loop = re.search(r'(?m)^( *)while true do\n', source)
    join = re.search(r'(?m)^ *::finishInteraction::\n *::waitForNextInteractionFrame::\n', source)
    if loop is None or join is None or loop.end() >= join.start():
        raise ValueError('husband interaction frame join changed')
    body = source[loop.end():join.start()]
    # Existing bare returns cancel the outer interaction scope. The extracted
    # step reports cancellation as false; successful paths request the next frame.
    body = ''.join('return false' if t.lastgroup == 'identifier' and t[0] == 'return'
                   else t[0] for t in tokens(body))
    body = body.replace('goto waitForNextInteractionFrame', 'return true')
    body = body.replace('goto finishInteraction', 'return true')
    indent = loop[1]
    source = (source[:loop.start()] + indent + 'local function processInteraction()\n' + body +
              indent + '    return true\n' + indent + 'end\n' +
              indent + 'while true do\n' + indent + '    if not processInteraction() then return end\n' +
              source[join.end():])
    face = re.search(r'(?m)^( *)::checkAffairWomanAfterConversation::\n', source)
    movie = re.search(r'(?m)^( *)::finishConversationMovie::\n', source)
    if face is None or movie is None or face.end() >= movie.start():
        raise ValueError('husband conversation joins changed')
    tail = (movie[1] + 'resources:Pause(false)\n' +
            movie[1] + 'resources:DestroyMovie(man_movie)\n' + movie[1] + 'return true\n')
    if source[movie.end():movie.end()+len(tail)] != tail:
        raise ValueError('husband conversation movie cleanup changed')
    face_body = source[face.end():movie.start()]
    if face_body.count('goto finishConversationMovie') != 1:
        raise ValueError('husband alternate facing join changed')
    face_body = face_body.replace('goto finishConversationMovie', 'return finishConversationMovie()')
    source = (source[:face.start()] + face[1] + 'return facePartnerAndFinishConversation()\n' +
              source[movie.end()+len(tail):])
    source = source.replace('goto checkAffairWomanAfterConversation',
                            'return facePartnerAndFinishConversation()')
    helpers = (indent + 'local function finishConversationMovie()\n' +
               textwrap.indent(textwrap.dedent(tail), indent + '    ') + indent + 'end\n' +
               indent + 'local function facePartnerAndFinishConversation()\n' +
               textwrap.indent(textwrap.dedent(face_body), indent + '    ') +
               indent + '    return finishConversationMovie()\n' + indent + 'end\n')
    source = source.replace(indent + 'local function processInteraction()\n',
                            helpers + indent + 'local function processInteraction()\n', 1)
    if any(t.lastgroup == 'identifier' and t[0] == 'goto' or t[0] == '::' for t in tokens(source)):
        raise ValueError('husband executable jump remains')
    return source, {
        'inputSha256': hashlib.sha256(original.encode()).hexdigest(),
        'outputSha256': hashlib.sha256(source.encode()).hexdigest(),
        'actorCleanupReturns': actor_returns, 'controlCleanupReturns': control_returns,
        'basis': 'Verified candidate nested resource/actor lifetime regions; cleanup calls remain in original order.',
        'interactionLoop': 'processInteraction returns false to cancel; true advances exactly one native frame',
        'conversationJoins': 'facePartnerAndFinishConversation and finishConversationMovie preserve shared tails',
        'limits': 'No executable jumps remain; operand readability and runtime/gameplay parity remain separate work.',
    }
