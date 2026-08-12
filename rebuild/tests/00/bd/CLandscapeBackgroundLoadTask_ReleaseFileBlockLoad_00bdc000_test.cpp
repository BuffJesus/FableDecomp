#include <cstdio>

struct CFileBlockManager
{
	int lastHandle;
	int callCount;
	void Release(int handle);
};

struct CLandscapeBackgroundLoadTask
{
	char pad00[0x20];
	CFileBlockManager* manager;   // +0x20
	char pad24[0x30 - 0x24];
	int  fileHandle;              // +0x30
	char pad34[0x5e - 0x34];
	char loaded;                  // +0x5e

	void ReleaseFileBlockLoad();
};

void CFileBlockManager::Release(int handle)
{
	this->lastHandle = handle;
	this->callCount++;
}

void CLandscapeBackgroundLoadTask::ReleaseFileBlockLoad()
{
	if (this->loaded)
	{
		this->manager->Release(this->fileHandle);
		this->loaded = 0;
	}
}

int main()
{
	CFileBlockManager mgr;
	mgr.lastHandle = -999;
	mgr.callCount = 0;

	CLandscapeBackgroundLoadTask task;
	task.manager = &mgr;
	task.fileHandle = 0x1234;
	task.loaded = 1;

	// loaded => release, clear flag
	task.ReleaseFileBlockLoad();
	bool ok = (mgr.callCount == 1) && (mgr.lastHandle == 0x1234) && (task.loaded == 0);

	// second call: not loaded => no-op
	task.ReleaseFileBlockLoad();
	ok = ok && (mgr.callCount == 1);

	// not-loaded from the start
	CLandscapeBackgroundLoadTask task2;
	task2.manager = &mgr;
	task2.fileHandle = 0x5;
	task2.loaded = 0;
	task2.ReleaseFileBlockLoad();
	ok = ok && (mgr.callCount == 1);

	if (ok)
		printf("RELEASE_FBL_OK\n");
	else
		printf("RELEASE_FBL_FAIL count=%d handle=%d loaded=%d\n", mgr.callCount, mgr.lastHandle, task.loaded);
	return ok ? 0 : 1;
}