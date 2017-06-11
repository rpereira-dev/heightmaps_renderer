#include "hmap.h"

t_hmap * hmap_new(unsigned long int const capacity,
        t_hash_function hashf, t_cmp_function keycmpf,
        t_function keyfreef, t_function datafreef) {

    // set the hmap capacity to the closest power of two
    unsigned long int c = 1;
    while (c < capacity) {
        c = c << 1;
    }

    unsigned long int size = sizeof(t_list) * c;
    void * values = malloc(size);
    if (values == NULL) {
        return (NULL);
    }
    memset(values, 0, size);

    t_hmap * hmap = (t_hmap *)malloc(sizeof(t_hmap));
    if (hmap == NULL) {
        free(values);
        return (NULL);
    }

    hmap->values = values;
    hmap->capacity = capacity;
    hmap->size = 0;
    hmap->hashf = hashf;
    hmap->keycmpf = keycmpf;
    hmap->datafreef = datafreef;
    hmap->keyfreef = keyfreef;

    return (hmap);
}

void hmap_delete(t_hmap * hmap) {
    unsigned long int i = 0;
    while (i < hmap->capacity) {
        t_list * lst = hmap->values + i;
        //if the list has been initialized
        if (lst->head) {
            LIST_ITER_START(lst, t_hmap_node *, node) {
                if (hmap->datafreef) {
                    hmap->datafreef(node->data);
                }

                if (hmap->keyfreef) {
                    hmap->keyfreef(node->key);
                }				
            }
            LIST_ITER_END(lst, t_hmap_node *, node)
			list_delete(lst);
        }
        ++i;
    }
}

void const * hmap_insert(t_hmap * hmap, void const * data, void const * key)
{
    unsigned long int hash = hmap->hashf(key); //get the hash for this key
    unsigned long int addr = hash & (hmap->capacity - 1); //get the array list from the hash

    t_hmap_node node = {hash, data, key}; //set the node buffer

    t_list * lst = hmap->values + addr; //get the list from it address
    //if the list hasnt already been initialized
    if (lst->head == NULL) {
        list_init(lst); //initialize it				
    }
    list_add(lst, &node, sizeof(t_hmap_node)); //add the node to the list

    hmap->size++;
    return (data); //return the data
}

void * hmap_get(t_hmap * hmap, void const * key) {
    unsigned long int hash = hmap->hashf(key); //get the hash for this key
    unsigned long int addr = hash & (hmap->capacity - 1); //get the lst list from the hash

    t_list * lst = hmap->values + addr; //list of collision for this key hash

    if (lst->size == 0) {
        return (NULL);
    }

    //so compare the exact key to find the wanted data
    LIST_ITER_START(lst, t_hmap_node *, node) {
        if (hmap->keycmpf(key, node->key) == 0) {
            return ((void *)node->data);
        }
    }
    LIST_ITER_END(lst, t_hmap_node *, node)
	return (NULL);
}

int hmap_remove_data(t_hmap * hmap, void const * data) {
    unsigned long int i = 0;
    while (i < hmap->capacity) {
        t_list * lst = hmap->values + i;
        LIST_ITER_START(lst, t_hmap_node *, node) {
            if (node->data == data) {
                //__node is the current LIST_ITER_START node of the linked list
                list_remove_node(lst, __node);
                hmap->size--;

                if (hmap->datafreef) {
                    hmap->datafreef(node->key);
                }

                if (hmap->keyfreef) {
                    hmap->keyfreef(node->key);
                }

                return (1);
            }
        }
        LIST_ITER_END(array, t_hmap_node *, node)
		++i;
    }
    return (0);
}

int hmap_remove_key(t_hmap * hmap, void const * key) {
    unsigned long int hash = hmap->hashf(key); //get the hash for this key
    unsigned long int addr = hash & (hmap->capacity - 1); //get the array list from the hash

    t_list * lst = hmap->values + addr; //lst of collision for this key hash

    if (lst->size == 0) {
        return (0);
    }

    //so compare the exact key to find the wanted data
    LIST_ITER_START(lst, t_hmap_node *, node) {
        if (hmap->keycmpf(key, node->key) == 0) {
            //__node is the current LIST_ITER_START node of the linked list
			list_remove_node(lst, __node);
			hmap->size--;

            if (hmap->datafreef) {
                hmap->datafreef(node->key);
            }

            if (hmap->keyfreef) {
                hmap->keyfreef(node->key);
            }

            return (1);
        }
    }
    LIST_ITER_END(array, t_hmap_node *, node)
	return (0);
}

unsigned long int strhash(char const * str) {
    if (str == NULL) {
        return (0);
    }

    unsigned long int hash = 5381;
    int c;
    while ((c = *str) != '\0') {
        hash = ((hash << 5) + hash) + c;
        str++;
    }
    return (hash);
}

unsigned long int inthash(int const value) {
    return (value);
}
