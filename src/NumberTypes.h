#ifndef NUMBER_TYPES_H_
#define NUMBER_TYPES_H_

typedef signed char sbyte;
typedef unsigned char ubyte;
typedef signed char byte;

typedef signed short int int16;
typedef unsigned short int uint16;

typedef signed int int32;
typedef unsigned int uint32;

// Can't use these unfortunatly because long longs just don't work right
// for some reason.
typedef signed long long int64;
typedef unsigned long long uint64;

#endif