#include <stdio.h>
#include <stdint.h>

#include "../include/bitmap.h"
#include "../include/block_store.h"
// include more if you need
#include <string.h>

struct block_store  {
        uint8_t *total_stored;  //Blocks off the right amount of storage needed to store the block_store
        bitmap_t *bitmap;        //Tracks each block to determine if it has been allocated or is still free
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
	bs->bitmap = bitmap_create(BLOCK_STORE_NUM_BLOCKS);
	if (!bs->bitmap) {
		free(bs->total_stored);
		free(bs);
		return NULL;
	}

	//Sets initial block of memory as used to show the start of memory allocated for the whole store.
	for(int i = BITMAP_START_BLOCK; i < BITMAP_START_BLOCK + BITMAP_NUM_BLOCKS; i++)
	{
		bitmap_set(bs->bitmap, i);
	}

	return bs;
}

void block_store_destroy(block_store_t *const bs)
{
	if (!bs) {
		return;
	}
	free(bs->total_stored);
	bitmap_destroy(bs->bitmap);
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
		if(!bitmap_test(bs->bitmap, i))
		{
			bitmap_set(bs->bitmap, i);
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
	if(bitmap_test(bs->bitmap, block_id))
	{
		return false;
	}

	//Sets the block for block_id as in use and increments allocated blocks.
	bitmap_set(bs->bitmap, block_id);
	bs->allocated_blocks++;
	return true;
}

void block_store_release(block_store_t *const bs, const size_t block_id)
{
	// check parameters
	if(!bs || block_id >= BLOCK_STORE_NUM_BLOCKS) return;
	if(block_id >= BITMAP_START_BLOCK && block_id < BITMAP_START_BLOCK + BITMAP_NUM_BLOCKS) return;

	// release the block
	bitmap_reset(bs->bitmap, block_id);
	bs->allocated_blocks--;
}

size_t block_store_get_used_blocks(const block_store_t *const bs)
{
	//Checks to ensure the blockstore isn't null
	if (bs == NULL) {
		return SIZE_MAX;
	}
	//Returns number of used blocks
	return bitmap_total_set(bs->bitmap);
}

size_t block_store_get_free_blocks(const block_store_t *const bs)
{
	//Checks to ensure the blockstore ins't null
	if (bs == NULL) {
		return SIZE_MAX;
	}
	//Returns number of free blocks
	return BLOCK_STORE_NUM_BLOCKS - bitmap_total_set(bs->bitmap);
}

size_t block_store_get_total_blocks()
{
	return BLOCK_STORE_NUM_BLOCKS;
}

size_t block_store_read(const block_store_t *const bs, const size_t block_id, void *buffer)
{
	//Checks to make sure the blockstore and buffer isn't null
    if(bs == NULL || buffer == NULL)
    {
        return 0;
    }
    //Makes sure block_id is with in the bounds of what we have allocated.
    if(block_id >= BLOCK_STORE_NUM_BLOCKS)
    {
        return 0;
    }
    //Makes sure that block has something in it.
    if(!bitmap_test(bs->bitmap, block_id))
    {
        return 0;
    }
    //Copies the bytes from the buffer to the stored total and stores the number of bytes read  to the block size bytes variable.
    memcpy(buffer, bs->total_stored + (block_id * BLOCK_SIZE_BYTES), BLOCK_SIZE_BYTES);
    return BLOCK_SIZE_BYTES;
}

size_t block_store_write(block_store_t *const bs, const size_t block_id, const void *buffer)
{
	//Checks to make sure the blockstore and buffer isn't null
    if(bs == NULL || buffer == NULL)
    {
        return 0;
    }
    //Makes sure block_id is with in the bounds of what we have allocated.
    if(block_id >= BLOCK_STORE_NUM_BLOCKS)
    {
        return 0;
    }
    //Makes sure that block has something in it.
    if(!bitmap_test(bs->bitmap, block_id))
    {
        return 0;
    }
    //Copies the bytes from the total stored to the buffer, and stores the number of bytes writen in block size bytes variable.
    memcpy(bs->total_stored + (block_id * BLOCK_SIZE_BYTES), buffer, BLOCK_SIZE_BYTES);
    return BLOCK_SIZE_BYTES;
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
