// CModeSitBase::RemoveModeWithAction  0x008d7230
// void __fastcall(CModeSitBase*, CCreatureActionBase*)

struct CCreatureActionBase
{
	virtual void v00();
	virtual void v01();
	virtual void v02();
	virtual void v03();
	virtual void v04();
	virtual void v05();
	virtual void v06();
	virtual void v07();
	virtual void v08();
	virtual void v09();
	virtual void v10();
	virtual int  v11();   // slot 0x2c/4 == 11
};

// subobject living at CModeSitBase+0x24; its method compiles to call 0x668d90
struct CModeSitSub
{
	void RemoveEntry(int a);   // -> call 0x00668d90
};

struct CModeSitBase
{
	char pad00[0x24];
	CModeSitSub sub;          // +0x24
	void RemoveModeWithAction(CCreatureActionBase* action);
};

void CModeSitBase::RemoveModeWithAction(CCreatureActionBase* action)
{
	this->sub.RemoveEntry(action->v11());
}