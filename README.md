# Cutlery
A C (standard C99) library that provides configurable (and intrusive (data structures), where necessary) data structures, serving as a backpack for any systems programming project.

It manages pointers to your structures, and not your structures. i.e. if you insert an element to it, then that particular element and not its copy gets inserted. This is unlike c++ STL containers.

Almost all the containers like Singlylist, Linkedlist, Bst, Hashmap (and even Heap) use intrusive nodes for management of the container.

The above 2 statements slides in another feature, references/iterators to the objects inside the containers, are never invalid, unless you explicitly remove it from the container (this is not the case with value_arraylist). Additionally, This library forces you to preallocate all your structs before-hand, or allocate only when you need it.

***(in Cutlery's intrusive data structures like linkedlist, bst, singlylist, head, and hashmap, the pointers to the nodes themselves are iterators, with get_next/prev_\* function flavours in their corresponding data structures in Cutlery)***

**Note**
 * *This library can be used with a baremetal projects (with minor modifications), all you will need is a C compiler.*
 * *It uses embedded nodes (intrusive nodes) to implement hashmap, heap (if you need certain features), bst (binary search tree), linkedlist, singlylist, etc - which is perfect for cache locality and for systems programming.*
 * *It is meant to be used for (but usage is not limited to) very low level systems programming, with minimal environment, without any of stdlib c, posix libs or 3rd party library support.*
 * *It can most certainly be used for high and low level systems software and application software, you may find me using it extensively in most of my C projects on GITHUB.*

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
   * `#include<cutlery_stds.h>`
   * `#include<array.h>`
   * `#include<arraylist.h>`
   * `#include<heap.h>`
   * `#include<linkedlist.h>`
   * `#include<singlylist.h>`
   * `#include<bst.h>`
   * `#include<hashmap.h>`
   * `#include<dstring.h>`
   * `#include<dpipe.h>`
   * `#include<bitmap.h>`
   * `#include<union_find.h>`
   * `#include<bloom_filter.h>`
   * `#include<count_min_sketch.h>`
   * `#include<uc_allocator_interface.h>`
   * `#include<memory_allocator_interface.h>`
   * `#include<dstring_base64.h>`
   * `#include<stream.h>`

## Instructions for uninstalling library

**Uninstall :**
 * `cd Cutlery`
 * `sudo make uninstall`
