#include "array_list.h"

t_array_list * array_list_new(unsigned long int nb, unsigned int elem_size) {
    t_array_list * array = (t_array_list *)malloc(sizeof(t_array_list));
    if (array == NULL) {
        return (NULL);
    }
    
    array->data = calloc(nb, elem_size);
    array->capacity = nb;
    array->elem_size = elem_size;
    array->size = 0;
    array->default_capacity = nb;
    return (array);
}

static void array_list_resize(t_array_list * array, unsigned size) {
    array->data = realloc(array->data, size * array->elem_size);
    array->capacity = size;
    if (array->size > size) {
        array->size = size;
    }
}

static void array_list_expand(t_array_list * array) {
    unsigned long int size = array->capacity * 2;
    array_list_resize(array, size);
}

int array_list_add(t_array_list * array, void * data) {
    if (array->size == array->capacity) {
        array_list_expand(array);
    }
    memcpy(array->data + array->size * array->elem_size, data, array->elem_size);
    array->size++;
    return (array->size);
}

void array_list_add_all(t_array_list * array, void * buffer, unsigned long int nb) {
    unsigned int array_idx = array->size * array->elem_size;
    while (nb) {
        unsigned int copy_nb = array->capacity - array->size;
        if (copy_nb > nb) {
            copy_nb = nb;
        }
        if (copy_nb == 0) {
            array_list_expand(array);
            continue ;
        }
        unsigned int copy_size = copy_nb * array->elem_size;
        memcpy(array->data + array_idx, buffer, copy_size);
        nb -= copy_nb;
        array->size += copy_nb;
        buffer += copy_size;
        array_idx += copy_size;
    }
}

void * array_list_get(t_array_list * array, unsigned int idx) {
    return (array->data + idx * array->elem_size);
}

void array_list_remove(t_array_list * array, unsigned int idx) {
    if (array->size == 0 || idx >= array->size) {
        return ;
    }
    
	unsigned int begin = idx * array->elem_size;
    unsigned int end = (array->size - 1) * array->elem_size;    
    memmove(array->data + begin, array->data + begin + array->elem_size, end - begin);
	
    array->size--;
}

void array_list_clear(t_array_list * array) {
    array->size = 0;
    array_list_resize(array, array->default_capacity);
}

void array_list_delete(t_array_list * array) {
    free(array->data);
}

void array_list_sort(t_array_list * array, t_cmp_function cmpf) {
    qsort(array->data, array->size, array->elem_size, cmpf);
}

void * array_list_raw(t_array_list * array) {
    return (array->data);
}
