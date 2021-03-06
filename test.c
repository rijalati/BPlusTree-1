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
    internal_t *root;
    my_key_t newKey;
    struct tree_t tree;
    char fileName[] = "hello.db";
    int i, j, n;
    off_t offset;
    scanf("%d", &n);
    fp = fopen(fileName, "w");
    fclose(fp);
    // test InitTree
    InitTree(&tree, fileName, intType);
    meta = (meta_t *)ReadBlock(fileName, META_OFFSET, sizeof(meta_t));
    printf("meta data:\n");
    printf("meta.order: %ld\nmeta.valueSize: %ld\nmeta.keySize: %ld\nmeta.internalNum: %ld\nmeta.leafNum: %ld\nmeta.height: %ld\nmeta.slot: %ld\nmeta.rootOffset: %ld\nmeta.leafOffset: %ld\n", meta->order, meta->valueSize, meta->keySize, meta->internalNum, meta->leafNum, meta->height, meta->slot, meta->rootOffset, meta->leafOffset);
    printf("=================================================\n");
    // test insert
    //for (i = n; i >= 0; i--)
    for (i = 1; i < 2 * n; i = i + 2)
    {
    #ifndef DEBUG
        printf("i: %d\n", i);
    #endif
        newKey.key = i;
        Insert(&tree, newKey, i);
    }

    //for (i = 0; i < 2 * n; i = i + 2)
    for (i = 2 * n - 2; i >= 0; i = i - 2)
    {
    #ifndef DEBUG
        printf("i: %d\n", i);
    #endif
        newKey.key = i;
        Insert(&tree, newKey, i);
    }

    /*
    for (i = 0; i < n; i++)
    {
        newKey.key = i;
        Insert(&tree, newKey, i);
    }
    */
    // test search
    newKey.key = n / 2;
    printf("value of n / 2: %ld\n", Search(&tree, newKey));
    newKey.key = 0x12345678;
    printf("value of 0x12345678: %ld\n", Search(&tree, newKey));
    newKey.key = 9;
    printf("value of 9: %ld\n", Search(&tree, newKey));
    newKey.key = 11;
    printf("value of 11: %ld\n", Search(&tree, newKey));
    newKey.key = 12;
    printf("value of 12: %ld\n", Search(&tree, newKey));
    newKey.key = 13;
    printf("value of 13: %ld\n", Search(&tree, newKey));
    //newKey.key = 6666;
    //printf("Insert faile? %d\n", Insert(&tree, newKey, 0x12345678));
    //newKey.key = 4112;
    //printf("Insert faile? %d\n", Insert(&tree, newKey, 0x4112));
    printf("=================================================\n");
    meta = (meta_t *)ReadBlock(fileName, META_OFFSET, sizeof(meta_t));
    leaf = (leaf_t *)ReadBlock(fileName, meta->leafOffset, sizeof(leaf_t));
    printf("leaf data:\n");
    printf("leaf->parent: %ld\nleaf->next: %ld\nleaf->prev: %ld\nleaf->n: %ld\nleaf->children[0].value: %ld\nleaf->children[0].key.key: %d\n", leaf->parent, leaf->next, leaf->prev, leaf->n, leaf->children[0].value, leaf->children[0].key.key);
    printf("=================================================\n");
    offset = meta->leafOffset;
    for (j = 0; j < (int)meta->leafNum; j++)
    {
        printf("offset: %ld\n", offset);
        for (i = 0; i < (int)leaf->n; i++)
        {
            printf("%ld ", leaf->children[i].value);
        }
        printf("\n\n");
        offset = leaf->next;
#ifdef NOBUFFER
        free(leaf);
#endif
        leaf = (leaf_t *)ReadBlock(fileName, offset, sizeof(leaf_t));
    }
    printf("\n\n");
    // test Remove
    //for (i = 0; i < 2 * n; i++)
    for (i = n; i >= 0; i--)
    {
        newKey.key = i;
        printf("Remove %d return: %d\n", i, Remove(&tree, newKey));
    }
    for (i = n + 1; i < 2 * n; i++)
    {
        newKey.key = i;
        printf("Remove %d return: %d\n", i, Remove(&tree, newKey));
    }
    printf("=================================================\n");
    meta = (meta_t *)ReadBlock(fileName, META_OFFSET, sizeof(meta_t));
    leaf = (leaf_t *)ReadBlock(fileName, meta->leafOffset, sizeof(leaf_t));
    printf("leaf data:\n");
    printf("leaf->parent: %ld\nleaf->next: %ld\nleaf->prev: %ld\nleaf->n: %ld\nleaf->children[0].value: %ld\nleaf->children[0].key.key: %d\n", leaf->parent, leaf->next, leaf->prev, leaf->n, leaf->children[0].value, leaf->children[0].key.key);
    printf("=================================================\n");
    offset = meta->leafOffset;
    for (j = 0; j < (int)meta->leafNum; j++)
    {
        printf("offset: %ld\n", offset);
        for (i = 0; i < (int)leaf->n; i++)
        {
            printf("%ld ", leaf->children[i].value);
        }
        printf("\n\n");
        offset = leaf->next;
#ifdef NOBUFFER
        free(leaf);
#endif
        leaf = (leaf_t *)ReadBlock(fileName, offset, sizeof(leaf_t));
    }
    printf("\n\n");
    printf("=================================================\n");
    // test ReadBlock
    printf("meta data:\n");
    printf("meta.order: %ld\nmeta.valueSize: %ld\nmeta.keySize: %ld\nmeta.internalNum: %ld\nmeta.leafNum: %ld\nmeta.height: %ld\nmeta.slot: %ld\nmeta.rootOffset: %ld\nmeta.leafOffset: %ld\n", meta->order, meta->valueSize, meta->keySize, meta->internalNum, meta->leafNum, meta->height, meta->slot, meta->rootOffset, meta->leafOffset);
    // test root
    root = (internal_t *)ReadBlock(fileName, meta->rootOffset, sizeof(internal_t));
    printf("=================================================\n");
    printf("root data: \n");
    printf("root->n: %ld\n", root->n);
#ifdef NOBUFFER
    free(leaf);
    free(root);
#endif
    return 0;
}
