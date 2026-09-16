"""Analyze generated locals around the woman's verified, inert movie helper."""
import re
from tools.script_recovery.readable_lua import readable_source

HELPER='''    local function finish_movie()
        resources:Pause(false)
        resources:DestroyMovie(woman_movie); woman_movie = nil
    end
'''
MARKER='    -- reviewed woman movie helper restored after local analysis\n'


def readable_woman_source(source):
    if source.count(HELPER)!=1 or MARKER in source:
        raise ValueError('Woman movie helper capture review changed')
    placeholder=MARKER+'\n'*(HELPER.count('\n')-1)
    analysis=source.replace(HELPER,placeholder)
    # The CFG analyzer accepts one statement per line. These two native movie
    # cancellation exits perform cleanup before jumping to partner destruction.
    # Expand their syntax so reaching definitions can distinguish reused locals.
    expansion=[]
    def expand(match):
        expansion.append({'original':match[0], 'inputLine':analysis.count('\n',0,match.start())+1})
        indent=match[1]
        return indent+'if not alive then\n'+indent+'    finish_movie()\n'+indent+'    goto LAB_00db2974\n'+indent+'end\n'
    analysis,count=re.subn(r'(?m)^( *)if not alive then finish_movie\(\); goto LAB_00db2974 end\n',expand,analysis)
    if count!=2:raise ValueError('Woman inline movie cancellation review changed')
    output,mapping=readable_source(analysis,inline_literals=True)
    for function in mapping:
        if function['function']=='__resource_main':function['expandedMovieCancellation']=expansion
    if output.count(placeholder)!=1:raise ValueError('Woman helper analysis anchor changed')
    return output.replace(placeholder,HELPER),mapping
