#pragma once

typedef __UINT8_TYPE__ uint8_t;
typedef __UINT16_TYPE__ uint16_t;
typedef __UINT32_TYPE__ uint32_t;
typedef __UINT64_TYPE__ uint64_t;

typedef __INT8_TYPE__ int8_t;
typedef __INT16_TYPE__ int16_t;
typedef __INT32_TYPE__ int32_t;
typedef __INT64_TYPE__ int64_t;

// Acrhitecture specific to x86 protected mode

#ifdef __x86_64__
typedef uint64_t size_t;
typedef uint64_t uword_t;
#else
typedef uint32_t size_t;
typedef uint32_t uword_t;
#endif