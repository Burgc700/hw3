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
	bs->allocated_blocks = BITMAP_NUM_BLOCKS;

	// allocate block storage
	bs->total_stored = calloc(BLOCK_STORE_NUM_BLOCKS, sizeof(uint8_t));
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

	//Sets initial block of memory as used to show the start of memory allocated for the whole store.
	for(int i = BITMAP_START_BLOCK; i < BITMAP_START_BLOCK + BITMAP_NUM_BLOCKS; i++)
	{
		bs->bitmap[i] = 1;
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
	//Checks null
	if(bs == NULL) {
		return SIZE_MAX;
	}

	//Loops through every block for the allocated store
	for(size_t i = 0; i < BLOCK_STORE_NUM_BLOCKS; i++)
	{
		//If the bitmap at that location is not used gets set to 1 and allocated blocks is incremented
		if(bs->bitmap[i] == 0)
		{
			bs->bitmap[i] = 1;
			bs->allocated_blocks++;
			return i;
		}
	}
	return SIZE_MAX;
}

bool block_store_request(block_store_t *const bs, const size_t block_id)
{
	//checks invalid parameters
	if(!bs || block_id >= BLOCK_STORE_NUM_BLOCKS) {
		return false;
	}

	//Makes sure the request is in the valid range of memory allocated
	if(block_id >= BITMAP_START_BLOCK && block_id < BITMAP_START_BLOCK + BITMAP_NUM_BLOCKS) {
		return false;
	}

	//Makes sure the block isn't already in use.
	if(bs->bitmap[block_id] == 1)
	{
		return false;
	}

	//Sets the block for block_id as in use and increments allocated blocks.
	bs->bitmap[block_id] = 1;
	bs->allocated_blocks++;
	return true;
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
