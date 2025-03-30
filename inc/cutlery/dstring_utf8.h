#ifndef DSTRING_UTF8_H
#define DSTRING_UTF8_H

#include<cutlery/cutlery_stds.h>

// non-negative range of long must be greater than or equal to 21 bits of information
fail_build_on(((sizeof(long) * CHAR_BIT) - 1) < 21)

// error values returned by the utf8 decoding function
#define UTF8_ENCODING_ERROR -1
#define UTF8_TOO_FEW_BYTES -2

// number lesser than (1L << 21)
int is_valid_utf8_code_point(long code_point);

// returns 0 if it is an invalid code point
cy_uint whole_bytes_required_for_encoding_utf8_code_point(long code_point);

// return value is either a valid 21 bit code point if positive,
// else i will be one of the error codes above
long utf8_decode_code_point(const char* data, cy_uint data_size, cy_uint* bytes_consumed);

// returns 1 for success, or returns one of the above error values
int utf8_encode_code_point(char* data, cy_uint data_size, long code_point, cy_uint* bytes_consumed);

// both the above functions will return the bytes_consumed, and it will be set to the number of bytes consumed from the data buffer
// this value will be set to 0, in case of an error

#endif