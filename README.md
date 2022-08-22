# Cutlery
A C (standard C99) library that provides configurable data structures, serving as a backpack for any systems programming project.

**Note**
 * *This library is currently pivoting to a barebone implementation, i.e. to compile using any c (C99) compiler, even with -nostdlibs.*
 * *It uses embedded nodes to implement bst (binary search tree), linkedlist.* and singlylist.
 * *It is meant to be used for (but usage is not limited to) very low level systems programming, with minimal environment without any of stdlib c, posix libs or 3rd party library support.*
 * *It can most certainly be used for high level systems software and application software, you may find me using it extensively in most of my C/C++ projects on GITHUB.*

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
   * `#include<stack.h>`
   * `#include<queue.h>`
   * `#include<heap.h>`
   * `#include<linkedlist.h>`
   * `#include<singlylist.h>`
   * `#include<bst.h>`
   * `#include<hashmap.h>`
   * `#include<dstring.h>`
   * `#include<bitmap.h>`
   * `#include<union_find.h>`
   * `#include<bloom_filter.h>`
   * `#include<memory_allocator_interface.h>`

## Instructions for uninstalling library

**Uninstall :**
 * `cd Cutlery`
 * `sudo make uninstall`
