#ifndef UC_ALLOCATOR_INTERFACE_H
#define UC_ALLOCATOR_INTERFACE_H

#include<cutlery/uc_allocator.h>
#include<cutlery/memory_allocator_interface.h>

int initialize_uc_allocator(struct memory_allocator* uca, uc_allocator_context* ucac_p, void* memory, cy_uint memory_size);

#endif