// CAIStateGroup_HoverAttackUnlessTargetIsFleeing::IsRunnableLeaf @ 0x0091e480
// retail:
//   call 0x91e270      ; helper returning bool in al
//   neg al ; sbb eax,eax ; inc eax   ; eax = !al
//   ret
// => return !helper(this);

struct CAIStateGroup_HoverAttackUnlessTargetIsFleeing
{
    // out-of-line helper (defined in another translation unit) so the
    // compiler emits a plain 'call' with a relocation.
    bool IsTargetFleeing();
    bool IsRunnableLeaf();
};

bool CAIStateGroup_HoverAttackUnlessTargetIsFleeing::IsRunnableLeaf()
{
    return !IsTargetFleeing();
}