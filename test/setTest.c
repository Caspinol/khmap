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

	int rounds = 100000;
	
	char *five = "Five";
	char **key1 = malloc(rounds * sizeof(char *));

	/* Prepare the keys */
	for(int i = 0; i < rounds; i++){
		asprintf(&key1[i], "key_%d", i);
	}
	
	clock_t start, diff;
	int msec;
	
	start = clock();
	for(int i = 0; i < rounds; i++){
		khmap_insert(hm, key1[i], five);
	
	}
	diff = clock() - start;
	msec = diff * 1000 / CLOCKS_PER_SEC;
	printf("Time taken %d[s] %d[ms] for [%d] inserts\n", msec/1000, msec%1000, rounds);

	start = clock();
	for(int i = 0; i < rounds; i++){

		char *res = khmap_get(hm, key1[i]);
		CuAssertPtrNotNull(tc, res);
		CuAssertStrEquals(tc, five, res);
	}

	diff = clock() - start;
	msec = diff * 1000 / CLOCKS_PER_SEC;
	printf("Time taken %d[s] %d[ms] for [%d] gets\n", msec/1000, msec%1000, rounds);

	start = clock();
	for(int i = 0; i < rounds; i++){

		char *res = khmap_remove(hm, key1[i]);
		CuAssertPtrNotNull(tc, res);
		CuAssertStrEquals(tc, five, res);
	}

	diff = clock() - start;
	msec = diff * 1000 / CLOCKS_PER_SEC;
	printf("Time taken %d[s] %d[ms] for [%d] removes\n", msec/1000, msec%1000, rounds);
	

	start = clock();
	for(int i = 0; i < rounds; i++){

		char *res = khmap_get(hm, key1[i]);
		CuAssertTrue(tc, res == NULL);
	}

	diff = clock() - start;
	msec = diff * 1000 / CLOCKS_PER_SEC;
	printf("Time taken %d[s] %d[ms] for [%d] NULL gets\n", msec/1000, msec%1000, rounds);

	/* Cleanup now */
	for(int i = 0; i < rounds; i++){
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
