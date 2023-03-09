#include<base64.h>

#include<cutlery_stds.h>

// maximum packet count that we can process
#define MAX_PACKET_COUNT (UINT_MAX >> 2)

// maximum length of the dstring once encoded, or maximum length that we can decode
#define MAX_BASE64_ENCODED_SIZE (MAX_PACKET_COUNT << 2)

// maximum length of the dstring that we can encode
#define MAX_ENCODABLE_SIZE (MAX_PACKET_COUNT * 3)

// MASK_6_BITS is equivalent to 0x3F in unsigned int
#define MASK_6_BITS (('\x1' << 6) - '\x1')

static int is_valid_base64_encoded_char(char c)
{
	// i.e. A-Z, a-z, 0-9, + and /
	if(('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || ('0' <= c && c <= '9') || (c == '+') || (c == '/'))
		return 1;
	return 0;
}

int is_valid_base64_encoding(const dstring* base64_enc)
{
	unsigned int base64_enc_size = get_char_count_dstring(base64_enc);

	if(base64_enc_size % 4)
		return 0;

	const char* base64_enc_data = get_byte_array_dstring(base64_enc);

	// check if all chars are valid i.e. A-Z, a-z, 0-9, + and / until an = is encountered
	unsigned int i = 0;
	while(i < base64_enc_size)
	{
		if(!is_valid_base64_encoded_char(base64_enc_data[i]))
		{
			// '=' implies we are in the last packet
			if(base64_enc_data[i] == '=')
				break;
			else
				return 0;
		}
	}

	// we hit '=', or end of base64_enc
	// there can be 0, 1, or 2 '=' at the end
	if((base64_enc_size - i) > 2)
		return 0;

	// check that all the remaining ones are '=' indeed
	while(i < base64_enc_size)
	{
		if(base64_enc_data[i] != '=')
			return 0;
	}

	return 1;
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

static char encode_6_bits(unsigned char x)
{
	if(0 <= x && x < 26)
		return x + 'A';
	else if(26 <= x && x < 52)
		return x - 26 + 'a';
	else if(52 <= x && x < 62)
		return x - 52 + '0';
	else if(62 == x)
		return '+';
	else if(63 == x)
		return '/';
	else // this case should never occur
		return '\0';
}

int base64_encode(const dstring* dstr, dstring* base64_enc)
{
	if(!can_base64_encode(dstr))
		return 0;

	make_dstring_empty(base64_enc);

	unsigned int dstr_size = get_char_count_dstring(dstr);
	const char* dstr_data = get_byte_array_dstring(dstr);

	// perform encoding
	unsigned int i = 0;
	while(dstr_size - i >= 3)
	{
		unsigned char packet[4] = {
			encode_6_bits((dstr_data[i] >> 2) & MASK_6_BITS),
			encode_6_bits(((dstr_data[i] << 4) | ((dstr_data[i+1] >> 4) & '\xf')) & MASK_6_BITS),
			encode_6_bits(((dstr_data[i+1] << 2) | ((dstr_data[i+2] >> 6) & '\x3')) & MASK_6_BITS),
			encode_6_bits((dstr_data[i+2]) & MASK_6_BITS)
		};
		concatenate_dstring(base64_enc, &get_dstring_pointing_to(((const char*)packet), 4));
		i+=3;
	}

	if(dstr_size - i == 1)
	{
		unsigned char packet[4] = {
			encode_6_bits((dstr_data[i] >> 2) & MASK_6_BITS),
			encode_6_bits((dstr_data[i] << 4) & MASK_6_BITS),
			'=',
			'='
		};
		concatenate_dstring(base64_enc, &get_dstring_pointing_to(((const char*)packet), 4));
		i+=1;
	}
	else if(dstr_size - i == 2)
	{
		unsigned char packet[4] = {
			encode_6_bits((dstr_data[i] >> 2) & MASK_6_BITS),
			encode_6_bits(((dstr_data[i] << 4) | ((dstr_data[i+1] >> 4) & '\xf')) & MASK_6_BITS),
			encode_6_bits((dstr_data[i+1] << 2) & MASK_6_BITS),
			'='
		};
		concatenate_dstring(base64_enc, &get_dstring_pointing_to(((const char*)packet), 4));
		i+=2;
	}

	return 1;
}

static unsigned char decode_to_6_bits(char c)
{
	if('A' <= c && c <= 'Z')
		return c - 'A';
	else if('a' <= c && c <= 'z')
		return c - 'a' + 26;
	else if('0' <= c && c <= '9')
		return c - '0' + 52;
	else if('+' == c)
		return 62;
	else if('/' == c)
		return 63;
	else // this case should never occur
		return -1;
}

int base64_decode(const dstring* base64_enc, dstring* dstr)
{
	if(!can_base64_decode(base64_enc))
		return 0;

	make_dstring_empty(dstr);

	unsigned int base64_enc_size = get_char_count_dstring(base64_enc);
	const char* base64_enc_data = get_byte_array_dstring(base64_enc);

	// perfrom decoding
	unsigned int i = 0;
	while(i < base64_enc_size)
	{
		int equals_count = 0;
		if(base64_enc_size - i == 4)
		{
			if(base64_enc_data[i+2] == '=')
				equals_count = 2;
			else if(base64_enc_data[i+3] == '=')
				equals_count = 1;
		}

		const char* packet_in = base64_enc_data + i;

		switch(equals_count)
		{
			case 0 :
			{
				unsigned char packet[4] = {
					decode_to_6_bits(packet_in[0]),
					decode_to_6_bits(packet_in[1]),
					decode_to_6_bits(packet_in[2]),
					decode_to_6_bits(packet_in[3])
				};

				char packet_merge[3] = {
					(packet[0] << 2) | ((packet[1] >> 6) & '\x3'),
					(packet[1] << 4) | ((packet[2] >> 2) & '\xf'),
					(packet[2] << 6) | ((packet[3]) & '\x3f')
				};

				concatenate_dstring(dstr, &get_dstring_pointing_to(packet_merge, 3));

				break;
			}
			case 1 :
			{
				unsigned char packet[3] = {
					decode_to_6_bits(packet_in[0]),
					decode_to_6_bits(packet_in[1]),
					decode_to_6_bits(packet_in[2])
				};

				char packet_merge[2] = {
					(packet[0] << 2) | ((packet[1] >> 6) & '\x3'),
					(packet[1] << 4) | ((packet[2] >> 2) & '\xf')
				};

				concatenate_dstring(dstr, &get_dstring_pointing_to(packet_merge, 2));

				break;
			}
			case 2 :
			{
				unsigned char packet[2] = {
					decode_to_6_bits(packet_in[0]),
					decode_to_6_bits(packet_in[1])
				};

				char packet_merge[1] = {
					(packet[0] << 2) | ((packet[1] >> 6) & '\x3')
				};

				concatenate_dstring(dstr, &get_dstring_pointing_to(packet_merge, 1));

				break;
			}
		}

		i+=4;
	}

	return 1;
}