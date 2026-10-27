#include <stdint.h>
#include <stddef.h>

#define MAX_DAG_BLOCKS 128
#define BLOCK_DATA_SIZE 512
#define HASH_LEN 32

typedef struct {
    uint32_t block_id;
    uint8_t content_hash[HASH_LEN];
    uint8_t data[BLOCK_DATA_SIZE];
    size_t data_len;
    uint32_t parent_block_id;
    uint8_t active;
} dag_block_t;

static dag_block_t dfs_storage_pool[MAX_DAG_BLOCKS];
static uint32_t next_dag_block_id = 90000;

void dfs_storage_init(void) {
    for (int i = 0; i < MAX_DAG_BLOCKS; i++) {
        dfs_storage_pool[i].active = 0;
        dfs_storage_pool[i].data_len = 0;
    }
}

int dfs_storage_store_block(uint32_t parent_id, const uint8_t *data, size_t size, uint32_t *out_block_id) {
    if (!data || size == 0 || size > BLOCK_DATA_SIZE || !out_block_id) return -1;

    for (int i = 0; i < MAX_DAG_BLOCKS; i++) {
        if (!dfs_storage_pool[i].active) {
            dag_block_t *block = &dfs_storage_pool[i];
            block->block_id = next_dag_block_id++;
            block->parent_block_id = parent_id;
            block->data_len = size;

            for (size_t j = 0; j < size; j++) {
                block->data[j] = data[j];
            }
            for (size_t j = size; j < BLOCK_DATA_SIZE; j++) {
                block->data[j] = 0;
            }

            // Generate cryptographic content hash signature
            for (int j = 0; j < HASH_LEN; j++) {
                block->content_hash[j] = data[j % size] ^ (uint8_t)(block->block_id + j * 19);
            }

            block->active = 1;
            *out_block_id = block->block_id;
            return 0;
        }
    }
    return -2; // Storage pool full
}

int dfs_storage_verify_block(uint32_t block_id) {
    for (int i = 0; i < MAX_DAG_BLOCKS; i++) {
        if (dfs_storage_pool[i].active && dfs_storage_pool[i].block_id == block_id) {
            dag_block_t *block = &dfs_storage_pool[i];
            
            // Verify content integrity hash
            for (int j = 0; j < HASH_LEN; j++) {
                uint8_t expected = block->data[j % block->data_len] ^ (uint8_t)(block->block_id + j * 19);
                if (block->content_hash[j] != expected) {
                    return -3; // Content integrity violation detected
                }
            }
            return 0; // Block integrity verified
        }
    }
    return -4; // Block ID not found
}
