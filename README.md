# Cutlery
A C (standard C99) library that provides configurable (and intrusive (data structures), where possible) data structures with stable pointers (pointer stability), serving as a backpack for any systems programming project.

It manages pointers to your structures, and not your structures. i.e. if you insert an element to it, then that particular element (stable pointer) and not its copy gets inserted. This is unlike C++ STL containers.

Almost all the containers like Singlylist, Linkedlist, Bst, Hashmap, Pheap, (and even Heap) use intrusive nodes for management of the container.

The above 2 statements slides in another feature, references/iterators to the objects inside the containers, are never invalid, unless you explicitly remove it from the container (this is not the case with value_arraylist). Additionally, This library forces you to preallocate all your structs before-hand, or allocate only when you need it.

***(in Cutlery's intrusive data structures like linkedlist, bst, singlylist, and hashmap, the pointers to your data themselves are iterators, with get_next/prev_\* function flavours for their corresponding data structures in Cutlery)***

**Note**
 * *This library can be used with a baremetal projects (with minor modifications), all you will need is a C compiler.*
 * *It uses embedded nodes (intrusive nodes) to implement hashmap, pheap (pointer based skew or leftist binary tree heap), heap (if you need certain features), bst (binary search tree with or without order statistics), linkedlist, singlylist, etc - which is perfect for cache locality, stable pointers and for systems programming.*
 * *It is meant to be used for (but usage is not limited to) very low level systems programming, with minimal environment, without any of stdlib C, posix libs or 3rd party library support.*
 * *It can most certainly be used for high and low level systems software and application software, you may find me using it extensively in most of my C projects on GITHUB.*

**Exclusions**
 * radix tree, skip list, will never be part of part of Cutlery, as these data structures need additional nodes to be allocated for insertions, and if these allocations fail, we can do nothing but panic and exit as the datastructure will be left in an inconsistent state due to partial insert. Additionally they can be implemented with primitives from Cutlery, hence the decission.

## Setup instructions
**Install dependencies :**
 * This project does not have any dependencies.

**Download source code :**
 * `git clone https://github.com/RohanVDvivedi/Cutlery.git`

**Build from source :**
 * `cd Cutlery`
 * `make clean all`

**Install from the build :**
 * `sudo make install`
 * ***Once you have installed from source, you may discard the build by*** `make clean`

## Using The library
 * add `-lcutlery` linker flag, while compiling your application
 * do not forget to include appropriate public api headers as and when needed. this includes
   * `#include<cutlery/cutlery_stds.h>`
   * `#include<cutlery/array.h>`
   * `#include<cutlery/arraylist.h>`
   * `#include<cutlery/heap.h>`
   * `#include<cutlery/pheap.h>`
   * `#include<cutlery/linkedlist.h>`
   * `#include<cutlery/singlylist.h>`
   * `#include<cutlery/bst.h>`
   * `#include<cutlery/hashmap.h>`
   * `#include<cutlery/cachemap.h>`
   * `#include<cutlery/dstring.h>`
   * `#include<cutlery/dpipe.h>`
   * `#include<cutlery/bitmap.h>`
   * `#include<cutlery/union_find.h>`
   * `#include<cutlery/bloom_filter.h>`
   * `#include<cutlery/count_min_sketch.h>`
   * `#include<cutlery/uc_allocator_interface.h>`
   * `#include<cutlery/memory_allocator_interface.h>`
   * `#include<cutlery/dstring_base64.h>`
   * `#include<cutlery/dstring_utf8.h>`
   * `#include<cutlery/stream.h>`
   * `#include<cutlery/deferred_callbacks.h>`

## Instructions for uninstalling library

**Uninstall :**
 * `cd Cutlery`
 * `sudo make uninstall`
