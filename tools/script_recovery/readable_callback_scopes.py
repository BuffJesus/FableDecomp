"""Readable processing of a sole, returned resource callback.

Only an emitted wrapper with one ordinary receiver parameter and one ordinary
callback parameter is supported. Its callback remains at the original call site;
the temporary named-function form exists only during readability analysis.
"""
import re
from tools.script_recovery.readable_lua import tokens
from tools.script_recovery.lua_binding_guard import ambiguous_bindings

SPLIT=re.compile(r'(?m)^(?=function |local function )')
WRAPPER=re.compile(r'\Afunction (?P<name>\w+)\((?P<receiver>\w+)\)\n'
 r'    return (?P=receiver):WithRetailResources\(function\((?P<parameter>\w+)\)\n'
 r'(?P<body>[\s\S]*)\n    end\)\nend(?P<tail>\n*)\Z')
GENERATED=re.compile(r'(?:[A-Za-z]{1,3}Var\d+(?:_\d+)*|r\d+|native_arg_\w+|[A-Za-z]*Stack_\w+)\Z')

def rewrite(source,convert,**options):
 output=[];reports={}
 # Function-shaped text in literals/comments is not an executable boundary.
 # Keep byte positions (including newlines) so original chunks remain exact.
 code=''.join(re.sub(r'[^\r\n]', ' ', t[0])
              if t.lastgroup in ('string','longstring','comment','longcomment')
              else t[0] for t in tokens(source))
 boundaries=[0]+[m.start() for m in SPLIT.finditer(code)]+[len(source)]
 for start,end in zip(boundaries,boundaries[1:]):
  chunk=source[start:end]
  match=WRAPPER.fullmatch(chunk)
  if not match:
   output.append(chunk);continue
  name,receiver,parameter,body=match['name'],match['receiver'],match['parameter'],match['body']
  if receiver==parameter or GENERATED.fullmatch(receiver) or GENERATED.fullmatch(parameter):
   output.append(chunk);continue
  stream=tokens(body)
  if any(t.lastgroup in ('string','longstring','longcomment')and '\n' in t[0] for t in stream):
   output.append(chunk);continue
  code=''.join(' '*len(t[0])if t.lastgroup in ('string','longstring','comment','longcomment')else t[0]for t in stream)
  # Nested anonymous callbacks and lexical-environment changes are outside
  # this bounded wrapper shape. Named cleanup closures remain under the
  # existing pipeline's preservation/flow guards.
  if re.search(r'\bfunction\s*\(|\b_ENV\b',code):
   output.append(chunk);continue
  header=f'function {name}({receiver}, {parameter})\n'
  virtual=header+body+'\nend\n'
  if ambiguous_bindings(virtual):
   output.append(chunk);continue
  # A returned callback can execute under either bridge lifetime. Keep the
  # native caller's explicit post-frame termination query in both contexts.
  callback_options = dict(options, frame_returns_alive=False)
  styled,report=convert(virtual,**callback_options)
  offset=styled.find(header)
  prefix=styled[:offset] if offset>=0 else ''
  # Existing enum naming may introduce immutable numeric aliases before the
  # analyzed function. Accept only that emitted form, never arbitrary calls,
  # mutable declarations or executable statements outside the callback.
  if any(line.strip() and not re.fullmatch(r'local [A-Z][A-Z_0-9]* = (?:-?\d+(?:\.\d+)?|0x[0-9a-fA-F]+)(?:  --[^\n]*)?',line)
         for line in prefix.splitlines()):
   output.append(chunk);continue
  if offset<0 or styled.count(header)!=1 or not styled.endswith('end\n'):
   output.append(chunk);continue
  inner=styled[offset+len(header):-len('end\n')]
  inner=''.join('    '+line if line.strip()else line for line in inner.splitlines(True))
  output.append(prefix+f'function {name}({receiver})\n    return {receiver}:WithRetailResources(function({parameter})\n'+inner+'    end)\nend'+match['tail'])
  reports[name]=report
 return ''.join(output),reports
