#pragma once

#include <cstddef>
#include <cstdint>

typedef std::uint8_t u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;
typedef std::uint64_t u64;

typedef std::int8_t s8;
typedef std::int16_t s16;
typedef std::int32_t s32;
typedef std::int64_t s64;

#define ARRAY_COUNT(arr) (size_t)(sizeof(arr) / sizeof((arr)[0]))

#define IMGUI_SUPPORT 0