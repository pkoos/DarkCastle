#include <gtest/gtest.h>
#include "../../src/NumberTypes.h"
#include <limits.h>

TEST(TypeDefinitions, Sbyte) {
    sbyte sbyte_min = SCHAR_MAX +1;
    EXPECT_EQ(sbyte_min, SCHAR_MIN);
    sbyte sbyte_max = SCHAR_MIN - 1;
    EXPECT_EQ(sbyte_max, SCHAR_MAX);
}

TEST(TypeDefinitions, Int16) {
    int16 int16_min = SHRT_MAX + 1;
    EXPECT_EQ(int16_min, SHRT_MIN);
    
    int16 int_16_max = SHRT_MIN -1;
    EXPECT_EQ(int_16_max, SHRT_MAX);
    // EXPECT_EQ(0, 1);
}

TEST(TypeDefinitions, Int32) {
    int32 int32_min = INT_MAX + 1;
    EXPECT_EQ(int32_min, INT_MIN);
    
    int32 int_32_max = INT_MIN -1;
    EXPECT_EQ(int_32_max, INT_MAX);
}

TEST(TypeDefinitions, Int64) {
    int64 int64_min = LLONG_MAX + 1;
    EXPECT_EQ(int64_min, LLONG_MIN);
    
    int64 int_64_max = LLONG_MIN -1;
    EXPECT_EQ(int_64_max, LLONG_MAX);
}

TEST(TypeDefinitions, Ubyte) {
    ubyte ubyte_max = -1;
    EXPECT_EQ(ubyte_max, UCHAR_MAX);
}
TEST(TypeDefinitions, uByte16) {
    uint16 uint16_max = -1;
    EXPECT_EQ(uint16_max, USHRT_MAX);
}
TEST(TypeDefinitions, Ubyte32) {
    uint32 uint32_max = -1;
    EXPECT_EQ(uint32_max, UINT_MAX);
}
TEST(TypeDefinitions, Uint64) {
    uint64 uint64_max = -1;
    EXPECT_EQ(uint64_max, ULLONG_MAX);
}

// typedef signed char		        sbyte;
// typedef signed char		        byte;
// typedef signed short int         int16;
// typedef signed int               int32;
// typedef signed long long         int64;