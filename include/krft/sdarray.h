#include <stdlib.h>

#ifndef SDARRAY_T
#define SDARRAY_T void *
#endif

#define _COMBINE(a, b) a##b
#define _COMBINE_MACRO(a, b) _COMBINE(a, b)

#ifndef SDARRAY_PREFIX
#define SDARRAY_PREFIX
#endif

#define _SDARRAY_PREFIX(name) _COMBINE_MACRO(SDARRAY_PREFIX, name)

typedef struct _SDARRAY_PREFIX(_sdarray) {
	SDARRAY_T *dense;
	size_t *sparse;
	size_t size_dense;
	size_t capacity_dense;
	size_t size_sparse;
	size_t capacity_sparse;
} _SDARRAY_PREFIX(_sdarray);

static inline
void _SDARRAY_PREFIX(_sdarray_extend_dense)(_SDARRAY_PREFIX(_sdarray) *sdarray)
{
	if (sdarray->capacity_dense == 0) {
		sdarray->capacity_dense = 1;
		sdarray->dense =
			malloc(sizeof(SDARRAY_T) * sdarray->capacity_dense);
		return;
	}
	sdarray->capacity_dense *= 2;
	sdarray->dense = realloc(sdarray->dense,
				 sizeof(SDARRAY_T) * sdarray->capacity_dense);
}

static inline void
_SDARRAY_PREFIX(_sdarray_extend_sparse)(_SDARRAY_PREFIX(_sdarray) *sdarray)
{
	if (sdarray->capacity_sparse == 0) {
		sdarray->capacity_sparse = 1;
		sdarray->sparse =
			malloc(sizeof(size_t) * sdarray->capacity_sparse);
		return;
	}
	sdarray->capacity_sparse *= 2;
	sdarray->sparse = realloc(sdarray->sparse,
				  sizeof(size_t) * sdarray->capacity_sparse);
}

static inline SDARRAY_T *
_SDARRAY_PREFIX(_sdarray_get)(_SDARRAY_PREFIX(_sdarray) *sdarray, size_t index)
{
	return &(sdarray->dense[sdarray->sparse[index]]);
}

static inline void
_SDARRAY_PREFIX(_sdarray_set)(_SDARRAY_PREFIX(_sdarray) *sdarray, size_t index,
			      SDARRAY_T item)
{
	sdarray->dense[sdarray->sparse[index]] = item;
}

static inline
void _SDARRAY_PREFIX(_sdarray_push)(_SDARRAY_PREFIX(_sdarray) *sdarray,
				    SDARRAY_T element)
{
	if (sdarray->size_dense == sdarray->capacity_dense)
		_SDARRAY_PREFIX(_sdarray_extend_dense)(sdarray);
	sdarray->dense[sdarray->size_dense] = element;
	if (sdarray->size_sparse == sdarray->capacity_sparse)
		_SDARRAY_PREFIX(_sdarray_extend_sparse)(sdarray);
	sdarray->sparse[sdarray->size_sparse++] = sdarray->size_dense;
	sdarray->size_dense++;
}

static inline void
_SDARRAY_PREFIX(_sdarray_remove)(_SDARRAY_PREFIX(_sdarray) *sdarray,
				 size_t index)
{
	SDARRAY_T *last =
		_SDARRAY_PREFIX(_sdarray_get)(sdarray, sdarray->size_dense - 1);
	_SDARRAY_PREFIX(_sdarray_set)(sdarray, index, *last);
	sdarray->sparse[index] = (size_t)-1;
}

#undef SDARRAY_T
#undef _COMBINE
#undef _COMBINE_MACRO
#undef _SDARRAY_PREFIX
#undef SDARRAY_PREFIX
