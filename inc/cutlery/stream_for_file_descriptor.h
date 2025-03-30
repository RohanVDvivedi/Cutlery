#ifndef STREAM_FOR_FILE_DESCRIPTOR_H
#define STREAM_FOR_FILE_DESCRIPTOR_H

#include<cutlery/stream.h>

/*
	This stream is designed to be used with unix/linux like file descriptors
	Can not be used with bare-metal systems.
	No attempt has been made to make this piece of code any more Cutlery compliant.
*/

int initialize_stream_for_fd(stream* strm, int fd);

#endif