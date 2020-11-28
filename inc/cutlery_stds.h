#ifndef CUTLERY_STDS_H
#define CUTLERY_STDS_H

#ifndef NULL
	#define NULL ((void*)0)
#endif

/* this is cutlery specific memory move implementation
   this is the function to be used in place of memcpy/memmove std library functions
   features : 	
		* moves size number of bytes from src to dest pointer
		* There is protection against the memory locations overlap (similar to memmove)
		  * this is solved by reverse iteration for copying memory
		* it copies processor accessible word side if and when possible
		  * this function copies unsigned int instead of unsigned char where ever possible
*/
void memory_move(void* dest, const void* src, unsigned int size);

/*
	this is cutlery specific memory set implementation
	this is the function to be used in place of memset std library function
	features : 	
		* it sets size number of bytes at the dest pointer with the given byte_value
		* it sets processor accessible word side if and when possible
		  * this function copies unsigned int instead of unsigned char where ever possible
*/
void memory_set(void* dest_start, char byte_value, unsigned int size);

#endif