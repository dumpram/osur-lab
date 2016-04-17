/*!  Dynamic memory allocator - first fit */

#define _BF_SIMPLE_C_
#include <lib/bf_simple.h>

#include <api/stdio.h>
#include <arch/processor.h>
#include <kernel/kprint.h>

#ifndef ASSERT
#include ASSERT_H
#endif

/*!
 * Initialize dynamic memory manager
 * \param mem_segm Memory pool start address
 * \param size Memory pool size
 * \return memory pool descriptor
*/
void *bfs_init ( void *mem_segm, size_t size )
{
	size_t start, end;
	bfs_hdr_t *chunk, *border;
	bfs_mpool_t *mpool;

	ASSERT ( mem_segm && size > sizeof (bfs_hdr_t) * 2 );

	/* align all on 'size_t' (if already not aligned) */
	start = (size_t) mem_segm;
	end = start + size;
	ALIGN_FW ( start );
	mpool = (void *) start;		/* place mm descriptor here */
	start += sizeof (bfs_mpool_t);
	ALIGN ( end );

	mpool->first = NULL;

	if ( end - start < 2 * HEADER_SIZE )
		return NULL;

	border = (bfs_hdr_t *) start;
	border->size = sizeof (size_t);
	MARK_USED ( border );

	chunk = GET_AFTER ( border );
	chunk->size = end - start - 2 * sizeof(size_t);
	MARK_FREE ( chunk );
	CLONE_SIZE_TO_TAIL ( chunk );

	border = GET_AFTER ( chunk );
	border->size = sizeof (size_t);
	MARK_USED ( border );

	bfs_insert_chunk ( mpool, chunk ); /* first and only free chunk */

	return mpool;
}

/*!
 * Get free chunk with required size (or slightly bigger)
 * \param mpool Memory pool to be used (if NULL default pool is used)
 * \param size Requested chunk size
 * \return Block address, NULL if can't find adequate free chunk
 */
void *bfs_alloc ( bfs_mpool_t *mpool, size_t size )
{
	bfs_hdr_t *iter, *chunk, *min_chunk;

    size_t min_size = -1;

	ASSERT ( mpool );

	size += sizeof (size_t) * 2; /* add header and tail size */
	if ( size < HEADER_SIZE )
		size = HEADER_SIZE;

	/* align request size to higher 'size_t' boundary */
	ALIGN_FW ( size );

	iter = mpool->first;

    // modify second condition and find chunk with minimum size
    // thats just one way of doing it
	while ( iter != NULL ) {
        if ( iter->size >= size && min_size == -1 ) {
            min_size = iter->size;
            min_chunk = iter;
        }
        if ( iter->size >= size && iter->size < min_size) {
            min_size = iter->size;
            min_chunk = iter;
        }
        iter = iter->next;
    }
    iter = min_chunk;

	if ( iter == NULL )
		return NULL; /* no adequate free chunk found */

	if ( iter->size >= size + HEADER_SIZE )
	{
		/* split chunk */
		/* first part remains in free list, just update size */
		iter->size -= size;
		CLONE_SIZE_TO_TAIL ( iter );

		chunk = GET_AFTER ( iter );
		chunk->size = size;
	}
	else { /* give whole chunk */
		chunk = iter;

		/* remove it from free list */
		bfs_remove_chunk ( mpool, chunk );
	}

	MARK_USED ( chunk );
	CLONE_SIZE_TO_TAIL ( chunk );

	return ( (void *) chunk ) + sizeof (size_t);
}

/*!
 * Free memory chunk
 * \param mpool Memory pool to be used (if NULL default pool is used)
 * \param chunk Chunk location (starting address)
 * \return 0 if successful, -1 otherwise
 */
int bfs_free ( bfs_mpool_t *mpool, void *chunk_to_be_freed )
{
	bfs_hdr_t *chunk, *before, *after;

	ASSERT ( mpool && chunk_to_be_freed );

	chunk = chunk_to_be_freed - sizeof (size_t);
	ASSERT ( CHECK_USED ( chunk ) );

	MARK_FREE ( chunk ); /* mark it as free */

	/* join with left? */
	before = ( (void *) chunk ) - sizeof(size_t);
	if ( CHECK_FREE ( before ) )
	{
		before = GET_HDR ( before );
		bfs_remove_chunk ( mpool, before );
		before->size += chunk->size; /* join */
		chunk = before;
	}

	/* join with right? */
	after = GET_AFTER ( chunk );
	if ( CHECK_FREE ( after ) )
	{
		bfs_remove_chunk ( mpool, after );
		chunk->size += after->size; /* join */
	}

	/* insert chunk in free list */
	bfs_insert_chunk ( mpool, chunk );

	/* set chunk tail */
	CLONE_SIZE_TO_TAIL ( chunk );

	return 0;
}

/*!
 * Routine that removes an chunk from 'free' list (free_list)
 * \param mpool Memory pool to be used
 * \param chunk Chunk header
 */
static void bfs_remove_chunk ( bfs_mpool_t *mpool, bfs_hdr_t *chunk )
{
	if ( chunk == mpool->first ) /* first in list? */
		mpool->first = chunk->next;
	else
		chunk->prev->next = chunk->next;

	if ( chunk->next != NULL )
		chunk->next->prev = chunk->prev;
}

/*!
 * Routine that insert chunk into 'free' list (free_list)
 * \param mpool Memory pool to be used
 * \param chunk Chunk header
 */
static void bfs_insert_chunk ( bfs_mpool_t *mpool, bfs_hdr_t *chunk )
{
	chunk->next = mpool->first;
	chunk->prev = NULL;

	if ( mpool->first )
		mpool->first->prev = chunk;

	mpool->first = chunk;
}
