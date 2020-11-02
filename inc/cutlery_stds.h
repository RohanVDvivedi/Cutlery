#ifndef CUTLERY_STDS_H
#define CUTLERY_STDS_H

#define NULL ((void*)0)

/* this is cutlery specific memory move implementation
   this is the function to be used in place of memcpy/memmove std library functions
   features : 	
		* moves size number of bytes from src to dest pointer
		* There is protection againat the memory locations overlap (similar to memmove)
		  * this is solved by reverse iteration for copying memory
		* it copies processor accessible word side if and when possible
		  * this function copies unsigned int instead of unsigned char where ever possible
*/
void memory_move(void* dest, const void* src, unsigned int size);

#endif