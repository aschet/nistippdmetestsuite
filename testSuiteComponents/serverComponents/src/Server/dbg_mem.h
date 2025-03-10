#ifndef DBG_MEM_H
#define DBG_MEM_H

#include <stdlib.h>

#ifdef __cplusplus

#ifdef DEBUG_MEMORY

void * operator new(size_t);
void operator delete(void *);

#endif

extern "C" {
#endif

void * debug_malloc(const char *file, int line, size_t );
void * debug_calloc(const char *file, int line, size_t, size_t);
void * debug_realloc(const char *file, int line, void *, size_t );
void debug_free(void *);

void disable_debug_memory();
void enable_debug_memory();
void clear_dbg_mem_list();
void print_dbg_mem_list();

extern int log_debug_mem_list;
extern int print_debug_mem_calls;
extern int next_log_debug_id;


#ifdef __cplusplus
};
#endif

#ifdef DEBUG_MEMORY

#define DEBUG_MALLOC(X) debug_malloc(__FILE__, __LINE__, X)
#define DEBUG_CALLOC(X,Y)  debug_calloc(__FILE__, __LINE__, X, Y)
#define DEBUG_REALLOC(X,Y) debug_realloc(__FILE__, __LINE__, X, Y)
#define DEBUG_FREE debug_free

#else


#define DEBUG_MALLOC malloc
#define DEBUG_CALLOC calloc
#define DEBUG_REALLOC realloc
#define DEBUG_FREE free

#endif

#endif

