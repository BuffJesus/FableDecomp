class CCharString;
class CAIStateGroupBase {
public:
  virtual const CCharString* GetDebugText() const;
};
const CCharString* __fastcall CAIStateGroupBase_GetDebugText(const CAIStateGroupBase* self)
{
  (void)self;
  return 0;
}