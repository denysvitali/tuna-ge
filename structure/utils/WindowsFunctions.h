#pragma once

// Windows related shit:
/*
 *  There is nothing remotely unsafe about fopen(), but a few people at MS seem to have lost their collective marbles
 *  over functions that take null-terminated strings as parameters. – anon
 *
 */
#ifndef _WINDOWS
#define fopen_s(pointer, path, mode) *pointer = fopen(path, mode)
#define strcpy_s(dst, size, src) strncpy(dst, src, size)
#define strncpy_s(dst, size, src, size2) strncpy(dst, src, size)
#endif