#ifndef STREAM_TO_READ_UNTIL_DSTRING_H
#define STREAM_TO_READ_UNTIL_DSTRING_H

#include<cutlery/stream.h>
#include<cutlery/dstring.h>
#include<cutlery/dpipe.h>

// this stream produces everything from the underlying_strm until a given dstring is encountered
// after which it remains in closed state
// the given read_until_dstr is consumed from the underlying stream, but it does not get produced

/*
	This stream relies on malloc/realloc/calloc and free for its internal structures, so it is not fit for some baremetal embedded platform
	It does not just point to your provided input patamaters but instead makes a deep-copy of the read_until_dstr, read_until_dstr_spml
	And also heap allocates its internal dpipe structure (that preserves the characters read up-until now, but those have not been produced yet)

	All-In-All, if you don't have malloc/free on your system, then either avoid using this stream OR write your own version of initialize_stream_for_reading_until_dstring
	such that
	  * it either takes in cutlery specific memory_allocator
	  * OR points to input parameters provided by you (and then you need to release these resources when this stream is closed)
	  * OR design it such that it takes your input parameters and takes full ownership to destroy them upon exit
	** all of the above three options would make this stream api too complicated and hence are not provided
*/

typedef struct read_until_dstring_stream_context read_until_dstring_stream_context;
struct read_until_dstring_stream_context
{
	stream* underlying_strm;

	cy_uint matched_length;

	dpipe cached_bytes;

	dstring read_until_dstr;

	cy_uint* read_until_dstr_spml;
};

// 0 implies an error
int initialize_stream_for_reading_until_dstring(stream* strm, stream* underlying_strm, const dstring* read_until_dstr);

// same as above function but with precalculated suffix prefic match lengths
// this should be produced as per (and by) Cutlery library api
int initialize_stream_for_reading_until_dstring2(stream* strm, stream* underlying_strm, const dstring* read_until_dstr, const cy_uint* read_until_dstr_spml);

#define UNDERLYING_STREAM_FINISHED_BEFORE_READ_UNTIL_DSTRING -108

#endif