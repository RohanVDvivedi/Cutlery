# project name
PROJECT_NAME:=cutlery

# this is the place where we download in your system
DOWNLOAD_DIR:=/usr/local

# we may download all the public headers

# list of public api headers (only these headers will be installed)
PUBLIC_HEADERS:=cutlery_stds.h cutlery_math.h array.h arraylist.h index_accessed_search_sort.h index_accessed_interface.h heap.h pheap.h heap_info.h linkedlist.h singlylist.h bst.h hashmap.h cachemap.h comparator_interface.h hasher_interface.h notifier_interface.h dstring.h dstring_substring.h dstring_diff.h dstring_char.h dstring_base64.h dstring_index_accessed_interface.h dstring_utf8.h dpipe.h stream.h stream_for_dstring.h stream_for_file_descriptor.h stream_to_read_until_dstring.h stream_util.h bitmap.h union_find.h bloom_filter.h count_min_sketch.h memory_allocator_interface.h multi_dim_array_util.h probabilistic_data_structure_util.h n-ary_tree_as_array_util.h circular_buffer_array_util.h value_arraylist.h uc_allocator.h uc_allocator_interface.h deferred_callbacks.h
# the library, which we will create
LIBRARY:=lib${PROJECT_NAME}.a
# the binary, which will use the created library
BINARY:=${PROJECT_NAME}

# list of all the directories, in the project
INC_DIR:=./inc
OBJ_DIR:=./obj
LIB_DIR:=./lib
SRC_DIR:=./src
BIN_DIR:=./bin

# compiler
CC:=gcc
# compiler flags
CFLAGS:=-Wall -Werror -O3 -I${INC_DIR}
# linker flags, this will used to compile the binary
LFLAGS:=-L${LIB_DIR} -l${PROJECT_NAME}
# Archiver
AR:=ar rcs

# utility
RM:=rm -f
MK:=mkdir -p
CP:=cp

# sources and objects must be evaluated every time you use them
# figure out all the sources in the project
SOURCES=$(shell find ${SRC_DIR} -name '*.c')
# and the required objects to be built, as intermediary
OBJECTS=$(patsubst ${SRC_DIR}/%.c, ${OBJ_DIR}/%.o, ${SOURCES})

# rule to make the directory for storing object files, that we create
${OBJ_DIR} :
	${MK} $@

# generic rule to build any object file
${OBJ_DIR}/%.o : ${SRC_DIR}/%.c | ${OBJ_DIR}
	${CC} ${CFLAGS} -c $< -o $@

# rule to make the directory for storing libraries, that we create
${LIB_DIR} :
	${MK} $@

# generic rule to make a library
${LIB_DIR}/${LIBRARY} : ${OBJECTS} | ${LIB_DIR}
	${AR} $@ ${OBJECTS}

# rule to make the directory for storing binaries, that we create
${BIN_DIR} :
	${MK} $@

# generic rule to make a binary using the library that we just created
${BIN_DIR}/${BINARY} : ./main.c ${LIB_DIR}/${LIBRARY} | ${BIN_DIR}
	${CC} ${CFLAGS} $< ${LFLAGS} -o $@

# to build the binary along with the library, if your project has a binary aswell
#all : ${BIN_DIR}/${BINARY}
# else if your project is only a library use this
all : ${LIB_DIR}/${LIBRARY}

# clean all the build, in this directory
clean :
	${RM} -r ${BIN_DIR} ${LIB_DIR} ${OBJ_DIR}

# -----------------------------------------------------
# INSTALLING and UNINSTALLING system wide
# -----------------------------------------------------

PUBLIC_HEADERS_TO_INSTALL=$(patsubst %.h, ${INC_DIR}/${PROJECT_NAME}/%.h, ${PUBLIC_HEADERS})

# install the library, from this directory to user environment path
# you must uninstall current installation before making a new installation
install : uninstall all
	${MK} ${DOWNLOAD_DIR}/include/${PROJECT_NAME}
	${CP} ${PUBLIC_HEADERS_TO_INSTALL} ${DOWNLOAD_DIR}/include/${PROJECT_NAME}
	${MK} ${DOWNLOAD_DIR}/lib
	${CP} ${LIB_DIR}/${LIBRARY} ${DOWNLOAD_DIR}/lib
	#${MK} ${DOWNLOAD_DIR}/bin
	#${CP} ${BIN_DIR}/${BINARY} ${DOWNLOAD_DIR}/bin

# removes what was installed
uninstall : 
	${RM} -r ${DOWNLOAD_DIR}/include/${PROJECT_NAME}
	${RM} ${DOWNLOAD_DIR}/lib/${LIBRARY}
	#${RM} ${DOWNLOAD_DIR}/bin/${BINARY}