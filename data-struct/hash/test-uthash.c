#include <stdio.h>
#include <stdlib.h>
#include <uthash.h>

typedef struct HashElem_ {
    int key;
    char info[128];
    UT_hash_handle hh;
} HashElem;

typedef struct HashElem_str_ {
    char key[128];
    char info[256];
    UT_hash_handle hh;
} HashElem_str;

typedef struct HashElem_ptr_ {
    void *key;
    char info[256];
    UT_hash_handle hh;
} HashElem_ptr;

int main(int argc, char**argv) {

    HashElem *hash_tab = NULL;
    HashElem *cur, *tmp; // for iteration
    for (int i = 0; i < 128; i++) {
        HashElem *e = malloc(sizeof(HashElem));
        e->key = i;
        snprintf(e->info, 128, "info for element %d is TBD", i);
        HASH_ADD_INT(hash_tab, key, e);
    }
    printf("\n Total INT keyed hash elements are %d \n", HASH_COUNT(hash_tab));
    HASH_ITER(hh, hash_tab, cur, tmp) {
        printf("Hash element %d info is %s \n", cur->key, cur->info);
    }
    
    int key = 34; tmp = NULL;
    HASH_FIND_INT(hash_tab, &key, tmp);
    printf("\n %s found hash element with key = %d \n ", (tmp ? "":"NOT"),key);

    // hash delete/clean up
    HASH_ITER(hh, hash_tab, cur, tmp) {
        HASH_DEL(hash_tab, cur);
        free(cur);
    }
   
    printf("\n Total hash elements after cleanup: %d \n", HASH_COUNT(hash_tab));
    printf("=======================================================\n");
    HashElem_str *hash_tbl = NULL;
    HashElem_str *c, *t;
    for (int i = 0; i < 32; i++) {
        HashElem_str *e_str = calloc(1, sizeof(HashElem_str));
        snprintf(e_str->key, 128, "key%d", i);
        snprintf(e_str->info, 256, "info for %s is unknown", e_str->key);
        HASH_ADD_STR(hash_tbl, key, e_str);
    }
    printf("\n Total STR keyed hash elements are %d \n", HASH_COUNT(hash_tbl));

    char *key_str = "key20";
    HASH_FIND_STR(hash_tbl, key_str, t);
    printf("\n %s found STR hash element with key = %s \n ", (t ? "":"NOT"), key_str);

    HASH_ITER(hh, hash_tbl, c, t) {
        HASH_DEL(hash_tbl, c);
        free(c);
    }
    printf("\n Total hash elements after cleanup: %d \n", HASH_COUNT(hash_tbl));
    printf("=======================================================\n");
    HashElem_ptr *hash_table = NULL;
    HashElem_ptr *cc, *tt;
    for (int i = 0; i < 56; i++) {
        HashElem_ptr *e_p = malloc(sizeof(HashElem_ptr));
        e_p->key = (void *)(0x2000+i);
        snprintf(e_p->info, 256, "ptr element info for pointer %p unknow", e_p->key);
        HASH_ADD_PTR(hash_table, key, e_p);
    }
    printf("\n Total PTR keyed hash elements are %d \n", HASH_COUNT(hash_table));
    void *key1 = (void *)0x2002;
    HASH_FIND_PTR(hash_table, &key1, tt);
     printf("\n %s found PTR hash element with key = %p \n ", (tt ? "":"NOT"), key1);
   
    HASH_ITER(hh, hash_table, cc, tt) {
        printf("Hash element with pointer %p info is %s \n", cc->key, cc->info);
        HASH_DEL(hash_table, cc);
        free(cc);
    }
    printf("\n Total hash elements after cleanup: %d \n", HASH_COUNT(hash_tbl));    
    return 0;
    
}
