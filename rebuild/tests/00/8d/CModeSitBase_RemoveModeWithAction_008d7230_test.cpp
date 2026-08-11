#include <cstdio>

struct CCreatureActionBase
{
	virtual void v00(){} virtual void v01(){} virtual void v02(){}
	virtual void v03(){} virtual void v04(){} virtual void v05(){}
	virtual void v06(){} virtual void v07(){} virtual void v08(){}
	virtual void v09(){} virtual void v10(){}
	virtual int  v11(){ return 0; }
};

static int g_removed = -1;

struct CModeSitSub
{
	void RemoveEntry(int a){ g_removed = a; }
};

struct CModeSitBase
{
	char pad00[0x24];
	CModeSitSub sub;
	void RemoveModeWithAction(CCreatureActionBase* action)
	{
		this->sub.RemoveEntry(action->v11());
	}
};

struct MyAction : public CCreatureActionBase
{
	int val;
	MyAction(int v):val(v){}
	virtual int v11(){ return val; }
};

int main()
{
	CModeSitBase m;
	MyAction a(4242);
	m.RemoveModeWithAction(&a);
	if (g_removed == 4242)
		printf("REMOVEMODE_OK\n");
	else
		printf("REMOVEMODE_FAIL %d\n", g_removed);
	return g_removed == 4242 ? 0 : 1;
}