#include <stdio.h>
#include <stdint.h>

#include "../include/bitmap.h"
#include "../include/block_store.h"
// include more if you need

struct block_store  {
        uint8_t *total_stored;  //Blocks off the right amount of storage needed to store the block_store
        uint8_t *bitmap;        //Tracks each block to determine if it has been allocated or is still free
        size_t allocated_blocks; //Gives the count of blocks that have been allocated
};

// You might find this handy. I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)

block_store_t *block_store_create()
{
	block_store_t *bs = malloc(sizeof(block_store_t));
	if (!bs) {
		return NULL;
	}
	bs->allocated_blocks = 0;

	// allocate block storage
	bs->total_stored = calloc(BLOCK_STORE_NUM_BLOCKS, BLOCK_SIZE_BYTES);
	if (!bs->total_stored) {
		free(bs);
		return NULL;
	}

	// allocate bitmap
	bs->bitmap = calloc(BLOCK_STORE_NUM_BLOCKS, BLOCK_SIZE_BYTES);
	if (!bs->bitmap) {
		free(bs->total_stored);
		free(bs);
		return NULL;
	}
	return bs;
}

void block_store_destroy(block_store_t *const bs)
{
	if (!bs) {
		return;
	}
	free(bs->total_stored);
	free(bs->bitmap);
	free(bs);
}

size_t block_store_allocate(block_store_t *const bs)
{
	UNUSED(bs);
	return 0;
}

bool block_store_request(block_store_t *const bs, const size_t block_id)
{
	UNUSED(bs);
	UNUSED(block_id);
	return false;
}

void block_store_release(block_store_t *const bs, const size_t block_id)
{
	UNUSED(bs);
	UNUSED(block_id);
}

size_t block_store_get_used_blocks(const block_store_t *const bs)
{
	UNUSED(bs);
	return 0;
}

size_t block_store_get_free_blocks(const block_store_t *const bs)
{
	UNUSED(bs);
	return 0;
}

size_t block_store_get_total_blocks()
{
	return 0;
}

size_t block_store_read(const block_store_t *const bs, const size_t block_id, void *buffer)
{
	UNUSED(bs);
	UNUSED(block_id);
	UNUSED(buffer);
	return 0;
}

size_t block_store_write(block_store_t *const bs, const size_t block_id, const void *buffer)
{
	UNUSED(bs);
	UNUSED(block_id);
	UNUSED(buffer);
	return 0;
}

block_store_t *block_store_deserialize(const char *const filename)
{
	UNUSED(filename);
	return NULL;
}

size_t block_store_serialize(const block_store_t *const bs, const char *const filename)
{
	UNUSED(bs);
	UNUSED(filename);
	return 0;
}
