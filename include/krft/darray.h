#include <stdlib.h>

#ifndef DARRAY_T
#define DARRAY_T void *
#endif

#define COMBINE(a, b) a##b
#define COMBINE_MACRO(a, b) COMBINE(a, b)

#ifndef DARRAY_PREFIX
#define DARRAY_PREFIX
#endif

#define _DARRAY_PREFIX(name) COMBINE_MACRO(DARRAY_PREFIX, name)

typedef struct _DARRAY_PREFIX(_darray) {
	DARRAY_T *array;
	size_t size;
	size_t capacity;
} _DARRAY_PREFIX(_darray);

static inline void _DARRAY_PREFIX(_darray_extend)(_DARRAY_PREFIX(_darray) *
						  darray)
{
	if (darray->capacity == 0) {
		darray->capacity = 1;
		darray->array = malloc(sizeof(DARRAY_T) * darray->capacity);
	} else {
		darray->capacity *= 2;
		darray->array = realloc(darray->array,
					sizeof(DARRAY_T) * darray->capacity);
	}
}

static inline void
_DARRAY_PREFIX(_darray_push)(_DARRAY_PREFIX(_darray) * darray, DARRAY_T element)
{
	if (darray->size == darray->capacity)
		_DARRAY_PREFIX(_darray_extend)(darray);
	darray->array[darray->size++] = element;
}

#undef DARRAY_T
#undef COMBINE
#undef COMBINE_MACRO
#undef DARRAY_PREFIX
