#ifndef STREAM_FOR_DSTRING_H
#define STREAM_FOR_DSTRING_H

#include<stream.h>
#include<dstring.h>

// every write concatenates data back to the provided str dstring
// every read reads data from the prefix of the str dstring and then discard_chars_from_front_dstring is done
int initialize_dstring_stream(stream* strm, dstring* str);

#endif