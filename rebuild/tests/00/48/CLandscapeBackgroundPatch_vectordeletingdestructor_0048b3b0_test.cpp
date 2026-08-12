#include <stdio.h>

// ---- unit under test (mirror of s.cpp) ----
static int g_dtor_calls = 0;
static int g_delete_calls = 0;
static void* g_last_deleted = 0;

void operator_delete_impl(void* p) { g_delete_calls++; g_last_deleted = p; }

struct CLandscapeBackgroundPatch
{
	int marker;
	void ScalarDtor();
	void* vector_deleting_destructor(unsigned int flags);
};

void CLandscapeBackgroundPatch::ScalarDtor() { g_dtor_calls++; }

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
	this->ScalarDtor();
	if (flags & 1)
		operator_delete_impl(this);
	return this;
}

int main()
{
	CLandscapeBackgroundPatch obj;
	obj.marker = 0x1234;

	// flags bit0 clear -> dtor runs, no delete, returns this
	g_dtor_calls = g_delete_calls = 0; g_last_deleted = 0;
	void* r1 = obj.vector_deleting_destructor(0);
	if (r1 != &obj)            { printf("FAIL r1\n"); return 1; }
	if (g_dtor_calls != 1)     { printf("FAIL dtor1\n"); return 1; }
	if (g_delete_calls != 0)   { printf("FAIL del1\n"); return 1; }

	// flags bit0 set -> dtor runs, delete(this), returns this
	g_dtor_calls = g_delete_calls = 0; g_last_deleted = 0;
	void* r2 = obj.vector_deleting_destructor(1);
	if (r2 != &obj)            { printf("FAIL r2\n"); return 1; }
	if (g_dtor_calls != 1)     { printf("FAIL dtor2\n"); return 1; }
	if (g_delete_calls != 1)   { printf("FAIL del2\n"); return 1; }
	if (g_last_deleted != &obj){ printf("FAIL delptr\n"); return 1; }

	// even flag (bit0 clear) -> no delete
	g_dtor_calls = g_delete_calls = 0;
	obj.vector_deleting_destructor(2);
	if (g_delete_calls != 0)   { printf("FAIL del3\n"); return 1; }

	printf("VDD_OK\n");
	return 0;
}