#include <stdio.h>

// Function-under-test: __fastcall setter, this=ecx, one stack arg -> [ecx+0x64].
__declspec(naked) void AddExclusiveObserver()
{
	__asm {
		mov eax, dword ptr [esp+4]
		mov dword ptr [ecx+0x64], eax
		ret 4
	}
}

struct CObservable {
	char pad[0x64];
	void* m_exclusiveObserver; // +0x64
};

int main()
{
	CObservable obj;
	obj.m_exclusiveObserver = (void*)0;
	int sentinel = 0;
	void* arg = (void*)&sentinel;
	CObservable* pThis = &obj;

	__asm {
		mov ecx, pThis          ; this -> ecx
		push arg                ; stack arg
		call AddExclusiveObserver ; callee pops (ret 4)
	}

	if (obj.m_exclusiveObserver == (void*)&sentinel) {
		printf("OK_0x0047d008\n");
		return 0;
	}
	printf("FAIL got=%p\n", obj.m_exclusiveObserver);
	return 1;
}