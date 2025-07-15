#ifndef _VECTOR_H
#define _VECTOR_H

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

#endif

