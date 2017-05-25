#ifndef MEMMANAGE_H_
#define MEMMANAGE_H_

void* memory_allocate(size_t byte_count);
void memory_free(void *pointer);
void memory_print();
void memory_init();
#endif
