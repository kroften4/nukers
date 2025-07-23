#include <stdlib.h>

#ifndef DARRAY_T
#define DARRAY_T void *
#endif

#define _COMBINE(a, b) a##b
#define _COMBINE_MACRO(a, b) _COMBINE(a, b)

#ifndef DARRAY_PREFIX
#define DARRAY_PREFIX
#endif

#define _DARRAY_PREFIX(name) _COMBINE_MACRO(DARRAY_PREFIX, name)

typedef struct _DARRAY_PREFIX(_darray) {
	DARRAY_T *array;
	size_t size;
	size_t capacity;
} _DARRAY_PREFIX(_darray);

static inline void _DARRAY_PREFIX(_darray_extend)(_DARRAY_PREFIX(_darray) *darray)
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
_DARRAY_PREFIX(_darray_push)(_DARRAY_PREFIX(_darray) *darray, DARRAY_T element)
{
	if (darray->size == darray->capacity)
		_DARRAY_PREFIX(_darray_extend)(darray);
	darray->array[darray->size++] = element;
}

static inline void _DARRAY_PREFIX(_darray_remove_swapback)(_DARRAY_PREFIX(_darray) *darray, size_t index) {
	darray->size -= 1;
	darray->array[index] = darray->array[darray->size];
}

#undef DARRAY_T
#undef _COMBINE
#undef _COMBINE_MACRO
#undef _DARRAY_PREFIX
#undef DARRAY_PREFIX
