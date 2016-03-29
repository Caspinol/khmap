#include "setTest.h"

khmap *hm = NULL;


void TestHashCreate(CuTest *tc){

	hm = khmap_create(NULL, NULL, NULL);
	CuAssertPtrNotNull(tc, hm);	
}

void TestHashLibBasic(CuTest *tc){

	int five = 5;

	char* key = strdup("Hello");

	int ret = khmap_insert(hm, key, &five);

	CuAssertIntEquals(tc, 0, ret);
	
	ret = khmap_insert(hm, key, &five);
	CuAssertIntEquals(tc, 0, ret);
	
	ret = khmap_insert(hm, key, &five);
	CuAssertIntEquals(tc, 0, ret);
        
	int *result = khmap_get(hm, key);
	CuAssertIntEquals(tc, five, *result);

	result = khmap_remove(hm, key);
	CuAssertIntEquals(tc, five, *result);
	
	result = khmap_remove(hm, key);
	CuAssertIntEquals(tc, five, *result);

	result = khmap_remove(hm, key);
	CuAssertIntEquals(tc, five, *result);

	result = khmap_remove(hm, key);
	CuAssertTrue(tc, result == NULL);

	free(key);
}

void TestHashLibResize(CuTest *tc){

	int five = 5;
	char **key1 = malloc(15000 * sizeof(char *));

	for(int i = 0; i < 15000; i++){
		asprintf(&key1[i], "key_%d", i);

		khmap_insert(hm, key1[i], &five);
	}

	for(int i = 0; i < 15000; i++){

		int *res = khmap_get(hm, key1[i]);
		CuAssertPtrNotNull(tc, res);
		CuAssertIntEquals(tc, five, *res);

		res = khmap_remove(hm, key1[i]);
		CuAssertPtrNotNull(tc, res);
		CuAssertIntEquals(tc, five, *res);

		res = khmap_get(hm, key1[i]);
		CuAssertTrue(tc, res == NULL);
	}

	for(int i = 0; i < 15000; i++){
		free(key1[i]);
	}

	free(key1);
}

void TestHashLibCleanup(CuTest *tc){

	khmap_destroy(&hm);

	CuAssertTrue(tc, hm == NULL);
}

CuSuite* TestHashLib(void){
	
	CuSuite* suite = CuSuiteNew();
	
	SUITE_ADD_TEST(suite, TestHashCreate);
	SUITE_ADD_TEST(suite, TestHashLibBasic);
       	SUITE_ADD_TEST(suite, TestHashLibResize);
	SUITE_ADD_TEST(suite, TestHashLibCleanup);

	return suite;
}
