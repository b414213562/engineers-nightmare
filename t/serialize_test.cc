#include <stdio.h>
#include <assert.h>

#include "../src/serializer.h"

/* return an initialised block
 * returns 0 on error
 */
block *
testing_block1(void)
{
    block * b = 0;

    b = (block*) calloc(1, sizeof(block));
    assert(b);

    b->type = block_empty;

    b->surfs[surface_xp] = surface_none;
    b->surfs[surface_xm] = surface_none;
    b->surfs[surface_yp] = surface_none;
    b->surfs[surface_ym] = surface_none;
    b->surfs[surface_zp] = surface_none;
    b->surfs[surface_zm] = surface_none;

    b->surf_space[surface_xp] = surface_none;
    b->surf_space[surface_xm] = surface_none;
    b->surf_space[surface_yp] = surface_none;
    b->surf_space[surface_ym] = surface_none;
    b->surf_space[surface_zp] = surface_none;
    b->surf_space[surface_zm] = surface_none;

    return b;
}

/* return an initialised block
 * returns 0 on error
 */
block *
testing_block2(void)
{
    block * b = 0;

    b = (block*) calloc(1, sizeof(block));
    assert(b);

    b->type = block_support;

    b->surfs[surface_xp] = surface_wall;
    b->surfs[surface_xm] = surface_none;
    b->surfs[surface_yp] = surface_none;
    b->surfs[surface_ym] = surface_wall;
    b->surfs[surface_zp] = surface_grate;
    b->surfs[surface_zm] = surface_text;

    b->surf_space[surface_xp] = 0;
    b->surf_space[surface_xm] = 3;
    b->surf_space[surface_yp] = 4;
    b->surf_space[surface_ym] = 1;
    b->surf_space[surface_zp] = 127;
    b->surf_space[surface_zm] = 3;

    return b;
}

/* returns 1 if equal
 * returns 0 if not
 */
unsigned int
block_equal(block *b1, block*b2)
{
    return 0 == memcmp(b1, b2, sizeof(block));
}

void
test_block_serialization(void)
{
    uint8_t *buffer = 0;
    unsigned int index = 0;

    /* b1 and b2 are to serialize */
    block *b1 = testing_block1();
    block *b2 = testing_block2();
    /* b3 and b4 are deserialized */
    block *b3 = 0;
    block *b4 = 0;

    buffer = (uint8_t*) calloc(2 * SERIALIZE_BLOCK_SIZE, sizeof(uint8_t));
    assert(buffer);

    assert(b1);
    assert(b2);


    assert( 0 == serialize_block(b1, buffer, &index) );
    assert( 0 == serialize_block(b2, buffer, &index) );
    assert( (2 * SERIALIZE_BLOCK_SIZE) == index );


    index = 0;
    b3 = deserialize_block(buffer, &index);
    assert(b3);

    b4 = deserialize_block(buffer, &index);
    assert(b4);

    assert( (2 * SERIALIZE_BLOCK_SIZE) == index );

    assert( block_equal(b1, b3) );
    assert( block_equal(b2, b4) );


    free(b1);
    free(b2);
    free(b3);
    free(b4);
}


chunk *
testing_chunk(void)
{
    chunk *c = 0;
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int k = 0;
    block *b = 0;

    c = (chunk*) calloc(1, sizeof(chunk));

    /* make something mildly interesting */
    for( k=0; k < CHUNK_SIZE; ++k ){
        for( j=0; j < CHUNK_SIZE; ++j ){
            for( i=0; i < CHUNK_SIZE; ++i ){
                b = c->get_block(i, j, k);
                if( ! b ){
                    errx(1, "deserialize_chunk: call to get_block failed");
                }

                if( i == j ){
                    b->type = block_support;

                    b->surfs[surface_xp] = surface_wall;
                    b->surfs[surface_xm] = surface_none;
                    b->surfs[surface_yp] = surface_none;
                    b->surfs[surface_ym] = surface_wall;
                    b->surfs[surface_zp] = surface_grate;
                    b->surfs[surface_zm] = surface_text;
                } else {
                    b->type = block_empty;
                }
            }
        }
    }

    return c;
}

/* returns 1 if equal
 * 0 if not
 */
unsigned int
chunk_equal(chunk *c1, chunk *c2){
    return 0 == memcmp(c1, c2, sizeof(chunk));
}

void
test_chunk_serialization(void)
{
    chunk *c1 = 0;
    chunk *c2 = 0;

    uint8_t *buffer = 0;
    unsigned int index = 0;

    buffer = (uint8_t*) calloc(SERIALIZE_CHUNK_SIZE, sizeof(uint8_t));
    assert(buffer);

    c1 = testing_chunk();
    assert(c1);

    assert( 0 == serialize_chunk(c1, buffer, &index) );
    assert( index == SERIALIZE_CHUNK_SIZE );

    index = 0;
    c2 = deserialize_chunk(buffer, &index);
    assert(c2);
    assert( index == SERIALIZE_CHUNK_SIZE );

    assert( chunk_equal(c1, c2) );

    free(c1);
    free(c2);
}


/* some light manual testing of serialization
 */
int
main(void)
{
    test_block_serialization();
    test_chunk_serialization();
}

