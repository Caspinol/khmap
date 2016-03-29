#include "setTest.h"

CuSuite * TestHashLib();

void run_all_tests(void){
	
	CuString *output = CuStringNew();
	CuSuite *suite = CuSuiteNew();

	CuSuiteAddSuite(suite, TestHashLib());
        
	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);

	CuStringDelete(output);
	CuSuiteDelete(suite);
}


int main(void){
	run_all_tests();
}
