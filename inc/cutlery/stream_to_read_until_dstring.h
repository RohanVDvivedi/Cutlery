#ifndef STREAM_TO_READ_UNTIL_DSTRING_H
#define STREAM_TO_READ_UNTIL_DSTRING_H

#include<cutlery/stream.h>
#include<cutlery/dstring.h>
#include<cutlery/dpipe.h>

// this stream produces everything from the underlying_strm until a given dstring is encountered
// after which it remains in closed state
// the given read_until_dstr is consumed from the underlying stream, but it does not get produced

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