#ifndef DSTRING_BASE64_H
#define DSTRING_BASE64_H

#include<cutlery/dstring.h>

int is_valid_base64_encoding(const dstring* base64_enc);

int can_base64_decode(const dstring* base64_enc);

int can_base64_encode(const dstring* dstr);

int base64_encode(const dstring* dstr, dstring* base64_enc);

int base64_decode(const dstring* base64_enc, dstring* dstr);

#endif