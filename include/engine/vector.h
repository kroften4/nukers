#ifndef _VECTOR_H
#define _VECTOR_H

#define VEC_ZERO (struct vector){0, 0}
#define VEC_UP (struct vector){0, 1}
#define VEC_DOWN (struct vector){0, -1}
#define VEC_LEFT (struct vector){-1, 0}
#define VEC_RIGHT (struct vector){1, 0}

struct vector {
    float x;
    float y;
};

struct vector vector_add(struct vector first, struct vector second);

struct vector vector_subtract(struct vector first, struct vector second);

struct vector vector_multiply(struct vector vec, float scalar);

struct vector reflect_orthogonal(struct vector vec, struct vector normal_orthogonal);

struct vector vector_normalize(struct vector vec);

struct vector vector_random_angle(float min, float max, float step);

float vector_dot(struct vector vec1, struct vector vec2);

#endif

