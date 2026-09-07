# ForgeFSE seed-script compatibility report

- Scripts with trace templates: **25**
- API call sites: **825** across **111** scope/name pairs
- Missing API names: **0**

## Missing capabilities

| Priority | API | Occurrences | Consumers |
|---:|---|---:|---|

## Interpretation

A missing capability means the reconstructed scripts call a name that is absent from the current
ForgeFSE/tutorial API manifest. It is a conversion blocker, not proof that the proposed signature or
behavior is correct. Each capability still needs native address, ABI, wrapper, and runtime validation.

Trace templates are ordered lexical call skeletons. Arguments and branch outcomes remain unresolved
until the native decompiler or a runtime fixture supplies them.
