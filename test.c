#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MiniSQL.h"
#include "BPlusTree.h"
int main()
{
    FILE *fp;
    meta_t *meta;
    leaf_t *leaf;
    //internal_t *internal;
    my_key_t newKey;
    struct tree_t tree;
    char fileName[1024];
    int i;
    scanf("%s", fileName);
    fp = fopen(fileName, "w");
    fclose(fp);
    // test InitTree
    InitTree(&tree, fileName, intType);
    meta = (meta_t *)ReadBlock(fileName, META_OFFSET, sizeof(meta_t));
    printf("meta data:\n");
    printf("meta.order: %ld\nmeta.valueSize: %ld\nmeta.keySize: %ld\nmeta.internalNum: %ld\nmeta.leafNum: %ld\nmeta.height: %ld\nmeta.slot: %ld\nmeta.rootOffset: %ld\nmeta.leafOffset: %ld\n", meta->order, meta->valueSize, meta->keySize, meta->internalNum, meta->leafNum, meta->height, meta->slot, meta->rootOffset, meta->leafOffset);
    // test insert
    for (i = 0; i < 2000; i = i + 2)
    {
    #ifndef DEBUG
        printf("i: %d\n", i);
    #endif
        newKey.key = i;
        Insert(&tree, newKey, i);
    }

    newKey.key = 666;
    printf("Insert faile? %d\n", Insert(&tree, newKey, 666));

    leaf = (leaf_t *)ReadBlock(fileName, meta->leafOffset, sizeof(leaf_t));
    printf("leaf data:\n");
    printf("leaf->parent: %ld\nleaf->next: %ld\nleaf->prev: %ld\nleaf->n: %ld\nleaf->children[0].value: %ld\nleaf->children[0].key.key: %d\n", leaf->parent, leaf->next, leaf->prev, leaf->n, leaf->children[0].value, leaf->children[0].key.key);
    // test ReadBlock
    meta = (meta_t *)ReadBlock(fileName, META_OFFSET, sizeof(meta_t));
    printf("meta data:\n");
    printf("meta.order: %ld\nmeta.valueSize: %ld\nmeta.keySize: %ld\nmeta.internalNum: %ld\nmeta.leafNum: %ld\nmeta.height: %ld\nmeta.slot: %ld\nmeta.rootOffset: %ld\nmeta.leafOffset: %ld\n", meta->order, meta->valueSize, meta->keySize, meta->internalNum, meta->leafNum, meta->height, meta->slot, meta->rootOffset, meta->leafOffset);
    return 0;
}