#include <assert.h>
#include <stdio.h>
#include "darray_test.h"

#define DARRAY_T int
#define DARRAY_PREFIX int
#include "krft/darray.h"

struct test {
	int a;
	bool b;
};

#define DARRAY_T struct test
#define DARRAY_PREFIX test
#include "krft/darray.h"

void darray_test(void)
{
	int_darray array = {0};
	int_darray_push(&array, 1);
	int_darray_push(&array, 2);
	int_darray_push(&array, 3);
	int_darray_push(&array, 4);
	int_darray_push(&array, 5);
	int_darray_push(&array, 6);
	assert(array.array[0] == 1);
	assert(array.array[1] == 2);
	assert(array.array[2] == 3);
	assert(array.array[3] == 4);
	assert(array.array[4] == 5);
	assert(array.array[5] == 6);
	assert(array.size == 6);
	assert(array.capacity == 8);
	puts("tests for int_darray passed");

	test_darray test_array = {0};
	test_darray_push(&test_array, (struct test){1, 0});
	test_darray_push(&test_array, (struct test){2, 1});
	test_darray_push(&test_array, (struct test){3, 0});
	test_darray_push(&test_array, (struct test){4, 1});
	test_darray_push(&test_array, (struct test){5, 0});
	test_darray_push(&test_array, (struct test){6, 1});
	assert(test_array.array[0].a == 1);
	assert(test_array.array[1].a == 2);
	assert(test_array.array[2].a == 3);
	assert(test_array.array[3].a == 4);
	assert(test_array.array[4].a == 5);
	assert(test_array.array[5].a == 6);
	assert(test_array.array[0].b == 0);
	assert(test_array.array[1].b == 1);
	assert(test_array.array[2].b == 0);
	assert(test_array.array[3].b == 1);
	assert(test_array.array[4].b == 0);
	assert(test_array.array[5].b == 1);
	assert(test_array.size == 6);
	assert(test_array.capacity == 8);
	puts("tests for struct darray passed");
}
