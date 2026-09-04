#ifndef MATH_H
#define MATH_H

#include "types.h"

static inline uint abs(const int v) { return v < 0 ? -v : v; }
static inline uint64 abs64(const int64 v) { return v < 0 ? -v : v; }

static inline int min(const int a, const int b) { return a < b ? a : b; }
static inline int64 min64(const int64 a, const int64 b) { return a < b ? a : b; }

static inline int max(const int a, const int b) { return a > b ? a : b; }
static inline int64 max64(const int64 a, const int64 b) { return a > b ? a : b; }

static inline int clamp(const int v, const int _min, const int _max) { return v < _min ? _min : v > _max ? _max : v; }
static inline int64 clamp64(const int64 v, const int64 _min, const int64 _max) { return v < _min ? _min : v > _max ? _max : v; }

static inline int8 sign(const int v) { return v < 0 ? -1 : v > 0 ? 1 : 0; }
static inline int8 sign64(const int64 v) { return v < 0 ? -1 : v > 0 ? 1 : 0; }

#endif
