#include <cstdio>
#include <cstring>

class CCharString;

struct SubList {
	void *received;
	void Remove(CCharString *name);
};

struct CQuestManager {
	char pad0[0x58];
	SubList list;          // +0x58
	char pad1[0x8f - 0x58 - 4];
	unsigned char dirty;   // +0x8f

	void RemoveAvailableQuestCardType(CCharString *name);
};

// test-local definition of the external callee
static void *g_lastArg = 0;
void SubList::Remove(CCharString *name)
{
	this->received = (void*)name;
	g_lastArg = (void*)name;
}

void CQuestManager::RemoveAvailableQuestCardType(CCharString *name)
{
	this->list.Remove(name);
	this->dirty = 1;
}

int main()
{
	CQuestManager qm;
	memset(&qm, 0, sizeof(qm));
	CCharString *fake = (CCharString*)0xDEADBEEF;
	qm.RemoveAvailableQuestCardType(fake);

	bool ok = (g_lastArg == (void*)0xDEADBEEF) && (qm.dirty == 1);
	// verify the +0x58 subobject and +0x8f offset are as expected
	unsigned char *base = (unsigned char*)&qm;
	bool off_ok = ((unsigned char*)&qm.list == base + 0x58) &&
	              ((unsigned char*)&qm.dirty == base + 0x8f);
	if (ok && off_ok) { printf("QCARD_REMOVE_OK\n"); return 0; }
	printf("QCARD_REMOVE_FAIL ok=%d off=%d dirty=%d\n", ok, off_ok, qm.dirty);
	return 1;
}