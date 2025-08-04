#ifndef _ENTITY_MANAGER_H
#define _ENTITY_MANAGER_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
#include "krft/log.h"
#endif

#define MAX_COMPONENTS 1024
#define INVALID_INDEX (size_t)-1

typedef size_t entity_id_t;
typedef size_t component_id_t;

struct component_pool {
	void *dense;
	size_t data_size;
	size_t *sparse;
	size_t *dense_to_sparse;
	size_t dense_count;
	size_t dense_capacity;
};

struct entity_manager {
	struct component_pool component_pools[MAX_COMPONENTS];
	size_t component_pool_count;
	size_t sparse_count;
	size_t sparse_capacity;
};

static inline void __extend_dense(struct component_pool *pool)
{
	if (pool->dense_capacity == 0) {
		pool->dense_capacity = 1;
		pool->dense = malloc(pool->data_size * pool->dense_capacity);
		pool->dense_to_sparse =
			malloc(sizeof(size_t) * pool->dense_capacity);
		return;
	}
	pool->dense_capacity *= 2;
	pool->dense =
		realloc(pool->dense, pool->data_size * pool->dense_capacity);
	pool->dense_to_sparse = realloc(pool->dense_to_sparse,
					sizeof(size_t) * pool->dense_capacity);
}

static inline void __extend_sparse(struct component_pool *pool,
				   size_t sparse_capacity)
{
	if (sparse_capacity == 0)
		pool->sparse = malloc(sizeof(size_t));
	else
		pool->sparse = realloc(pool->sparse,
				       sizeof(size_t) * sparse_capacity * 2);
}

static inline void *get_component_from_pool(struct component_pool *pool,
					    entity_id_t entity_id)
{
	size_t dense_idx = pool->sparse[entity_id];
	void *component = (char *)pool->dense + pool->data_size * dense_idx;
	return component;
}

static inline void *get_component(struct entity_manager *manager,
				  entity_id_t entity_id,
				  component_id_t component_id)
{
	struct component_pool *pool = &manager->component_pools[component_id];
	return get_component_from_pool(pool, entity_id);
}

static inline void add_component_to_pool(struct component_pool *pool,
					 entity_id_t entity_id, void *component)
{
#ifdef DEBUG
	if (pool->sparse[entity_id] != INVALID_INDEX)
		ERRORF("component already exists on %zu", entity_id);
	if (pool->dense_count > pool->dense_capacity)
		ERROR("skipped extending dense array");
#endif
	if (pool->dense_count == pool->dense_capacity)
		__extend_dense(pool);
	pool->sparse[entity_id] = pool->dense_count;
	pool->dense_to_sparse[pool->dense_count] = entity_id;
	pool->dense_count++;
	size_t dense_idx = pool->sparse[entity_id];
	memcpy((char *)pool->dense + pool->data_size * dense_idx, component,
	       pool->data_size);
}

static inline void add_component(struct entity_manager *manager,
				 entity_id_t entity_id,
				 component_id_t component_id, void *component)
{
	struct component_pool *pool = &manager->component_pools[component_id];
	add_component_to_pool(pool, entity_id, component);
}

static inline void __extend_all_sparse(struct entity_manager *manager)
{
	for (size_t pool_id = 0; pool_id < MAX_COMPONENTS; pool_id++) {
		struct component_pool *pool =
			&manager->component_pools[pool_id];
		__extend_sparse(pool, manager->sparse_capacity);
	}
	if (manager->sparse_capacity == 0)
		manager->sparse_capacity = 1;
	else
		manager->sparse_capacity *= 2;
}

static inline entity_id_t add_entity(struct entity_manager *manager)
{
	if (manager->sparse_count == manager->sparse_capacity)
		__extend_all_sparse(manager);
	for (size_t pool_id = 0; pool_id < MAX_COMPONENTS; pool_id++) {
		struct component_pool *pool =
			&manager->component_pools[pool_id];
		pool->sparse[manager->sparse_count] = INVALID_INDEX;
	}
	return manager->sparse_count++;
}

static inline void remove_component_from_pool(struct component_pool *pool,
					      entity_id_t entity_id)
{
	if (pool->sparse[entity_id] == INVALID_INDEX)
		return;
	size_t dense_removed_idx = pool->sparse[entity_id];
	size_t dense_last_idx = pool->dense_count - 1;
	void *removed =
		(char *)pool->dense + pool->data_size * dense_removed_idx;
	void *last = (char *)pool->dense + pool->data_size * dense_last_idx;
	memcpy(removed, last, pool->data_size);
	pool->dense_to_sparse[dense_removed_idx] =
		pool->dense_to_sparse[dense_last_idx];
	pool->dense_count--;

	pool->sparse[pool->dense_to_sparse[dense_removed_idx]] =
		dense_removed_idx;
	pool->sparse[entity_id] = INVALID_INDEX;
}

static inline void remove_component(struct entity_manager *manager,
				    entity_id_t entity_id,
				    component_id_t component_id)
{
	struct component_pool *pool = &manager->component_pools[component_id];
	remove_component_from_pool(pool, entity_id);
}

static inline bool has_component_in_pool(struct component_pool *pool,
					 entity_id_t entity_id)
{
	return pool->sparse[entity_id] != INVALID_INDEX;
}

static inline bool has_component(struct entity_manager *manager,
				 entity_id_t entity_id,
				 component_id_t component_id)
{
	struct component_pool *pool = &manager->component_pools[component_id];
	return has_component_in_pool(pool, entity_id);
}

static inline void remove_entity(struct entity_manager *manager,
				 entity_id_t entity_id)
{
	for (size_t pool_id = 0; pool_id < MAX_COMPONENTS; pool_id++)
		remove_component(manager, entity_id, pool_id);
}

static inline void init_component_pool(struct component_pool *pool,
				       size_t component_size)
{
	memset(pool, 0, sizeof(struct component_pool));
	pool->data_size = component_size;
}

static inline component_id_t register_component(struct entity_manager *manager,
						size_t component_size)
{
	struct component_pool pool = { 0 };
	init_component_pool(&pool, component_size);
	manager->component_pools[manager->component_pool_count] = pool;
	return manager->component_pool_count++;
}

#endif
