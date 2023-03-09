#include<base64.h>

#include<cutlery_stds.h>

// maximum packet count that we can process
#define MAX_PACKET_COUNT (UINT_MAX >> 2)

// maximum length of the dstring once encoded, or maximum length that we can decode
#define MAX_BASE64_ENCODED_SIZE (MAX_PACKET_COUNT << 2)

// maximum length of the dstring that we can encode
#define MAX_ENCODABLE_SIZE (MAX_PACKET_COUNT * 3)

int is_valid_base64_encoding(const dstring* base64_enc)
{
	unsigned int base64_enc_size = get_char_count_dstring(base64_enc);

	if(base64_enc_size % 4)
		return 0;

	const char* base64_enc_data = get_byte_array_dstring(base64_enc);

	// check if all chars are valid i.e. A-Z, a-z, 0-9, + and / until an = is encountered
	// TODO
}

int can_base64_decode(const dstring* base64_enc)
{
	// if the base64_enc is of length lesser or equal to MAX_BASE64_ENCODED_SIZE,
	// and is_valid_base64_encoding, then it can be decoded
	if(get_char_count_dstring(base64_enc) <= MAX_BASE64_ENCODED_SIZE &&
		is_valid_base64_encoding(base64_enc) )
		return 1;

	return 0;
}

int can_base64_encode(const dstring* dstr)
{
	// we can encode dstr to base64 only if it's length is lesser or equal to MAX_ENCODABLE_SIZE
	return get_char_count_dstring(dstr) <= MAX_ENCODABLE_SIZE;
}

int base64_encode(const dstring* dstr, dstring* base64_enc)
{
	if(!can_base64_encode(dstr))
		return 0;

	make_dstring_empty(base64_enc);

	// perform encoding
	// TODO

	return 1;
}

int base64_decode(const dstring* base64_enc, dstring* dstr)
{
	if(!can_base64_decode(base64_enc))
		return 0;

	make_dstring_empty(dstr);

	// perfrom decoding
	// TODO

	return 1;
}