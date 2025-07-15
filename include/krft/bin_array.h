#ifndef __BIN_ARRAY_H__
#define __BIN_ARRAY_H__

#include <inttypes.h>
#include <netinet/in.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>

#define FLOAT_SCALE 10000

struct binarr {
    int8_t *buf;
    size_t size;
    size_t index;
};

static inline struct binarr *binarr_new(struct binarr *arr, size_t capacity) {
    int8_t *buf = malloc(capacity);
    if (buf == NULL) {
        perror("malloc");
    }
    arr->buf = buf;
    arr->size = 0;
    arr->index = 0;
    return arr;
}

static inline void binarr_destroy(struct binarr arr) {
    free(arr.buf);
}

static inline void binarr_write_i8(struct binarr *arr, int8_t data) {
    memcpy(arr->buf + arr->index, &data, sizeof(data));
    arr->index += sizeof(data);
}
static inline void binarr_append_i8(struct binarr *arr, int8_t data) {
    binarr_write_i8(arr, data);
    arr->size = arr->index;
}

static inline void binarr_write_i16(struct binarr *arr, int16_t data) {
    memcpy(arr->buf + arr->index, &data, sizeof(data));
    arr->index += sizeof(data);
}

static inline void binarr_append_i16(struct binarr *arr, int16_t data) {
    binarr_write_i16(arr, data);
    arr->size = arr->index;
}

static inline void binarr_append_i16_n(struct binarr *arr, int16_t data) {
    data = htons(data);
    binarr_append_i16(arr, data);
}

static inline void binarr_write_i32(struct binarr *arr, int32_t data) {
    memcpy(arr->buf + arr->index, &data, sizeof(data));
    arr->index += sizeof(data);
}

static inline void binarr_write_i32_n(struct binarr *arr, int32_t data) {
    data = htonl(data);
    binarr_write_i32(arr, data);
}

static inline void binarr_append_i32(struct binarr *arr, int32_t data) {
    binarr_write_i32(arr, data);
    arr->size = arr->index;
}


static inline void binarr_append_i32_n(struct binarr *arr, int32_t data) {
    data = htonl(data);
    binarr_append_i32(arr, data);
}

static inline void binarr_write_u32(struct binarr *arr, uint32_t data) {
    memcpy(arr->buf + arr->index, &data, sizeof(data));
    arr->index += sizeof(data);
}

static inline void binarr_write_u32_n(struct binarr *arr, uint32_t data) {
    data = htonl(data);
    binarr_write_u32(arr, data);
}

static inline void binarr_append_u32(struct binarr *arr, uint32_t data) {
    binarr_write_u32(arr, data);
    arr->size = arr->index;
}


static inline void binarr_append_u32_n(struct binarr *arr, uint32_t data) {
    data = htonl(data);
    binarr_append_u32(arr, data);
}

static inline int8_t binarr_read_i8(struct binarr *arr) {
    return arr->buf[arr->index++];
}

static inline int16_t binarr_read_i16(struct binarr *arr) {
    int16_t res = 0;
    memcpy(&res, arr->buf + arr->index, sizeof(res));
    arr->index += sizeof(res);
    return res;
}

static inline int16_t binarr_read_i16_n(struct binarr *arr) {
    return ntohs(binarr_read_i16(arr));
}

static inline int32_t binarr_read_i32(struct binarr *arr) {
    int32_t res = 0;
    memcpy(&res, arr->buf + arr->index, sizeof(res));
    arr->index += sizeof(res);
    return res;
}

static inline int32_t binarr_read_i32_n(struct binarr *arr) {
    return ntohl(binarr_read_i32(arr));
}

static inline uint32_t binarr_read_u32(struct binarr *arr) {
    uint32_t res = 0;
    memcpy(&res, arr->buf + arr->index, sizeof(res));
    arr->index += sizeof(res);
    return res;
}

static inline uint32_t binarr_read_u32_n(struct binarr *arr) {
    return ntohl(binarr_read_u32(arr));
}

static inline void binarr_write_float(struct binarr *arr, float data) {
    int32_t idata = data * FLOAT_SCALE;
    binarr_write_i32(arr, idata);
}

static inline void binarr_write_float_n(struct binarr *arr, float data) {
    int32_t idata = data * FLOAT_SCALE;
    binarr_write_i32_n(arr, idata);
}

static inline void binarr_append_float(struct binarr *arr, float data) {
    int32_t idata = data * FLOAT_SCALE;
    binarr_append_i32(arr, idata);
}

static inline void binarr_append_float_n(struct binarr *arr, float data) {
    int32_t idata = data * FLOAT_SCALE;
    binarr_append_i32_n(arr, idata);
}

static inline float binarr_read_float(struct binarr *arr) {
    float res = 0;
    int32_t ires = binarr_read_i32(arr);
    res = ires / (float)FLOAT_SCALE;
    return res;
}

static inline float binarr_read_float_n(struct binarr *arr) {
    float res = 0;
    int32_t ires = binarr_read_i32_n(arr);
    res = ires / (float)FLOAT_SCALE;
    return res;
}

#endif

