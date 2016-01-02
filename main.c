#include <stdio.h>
#include <stdlib.h>

#define FC_Malloc malloc

typedef struct Board Board;

struct Board
{
    int width;
    int height;

    int *body;
};

static Board *BoardNew(int width, int height)
{
    Board *b = FC_Malloc(sizeof(Board));

    b->width = width;
    b->height = height;

    b->body = FC_Malloc(sizeof(int) * width * height);

    return b;
}

static Board *ReadBoard(const char* board)
{
    /*
      19 * 19
      AAAAABCCCCCDDEEEEEE
      AFFFBBBBBBCDDEGGGGE
      AFAFFFFBCBCDEEGEEEE
      AAAHHHFBCCCDDGGGGII
      JJJJJHHHCKLLDDDDDDI
      JMMMMMMHNKOLLLLLLII
      JPPPPPMNNKOLLIIIIIQ
      JPMMMMMNKKORRRRRRRQ
      JPPSSSNNKKORTTTTTQQ
      JPSSUOOOOOORTVVVVVQ
      SSSUUOWWWWWWTXXXXVQ
      YYYUUZZZZZZZTXXXXVQ
      YaYYYZbbbbXXXXVVVVQ
      YaaZZZbcccccdeVQQQQ
      YYaabbbcdddddefffff
      gggahhhcdeeeeefiiif
      gagahccchhejjjfffif
      gaaahhhhhjejijfffif
      gggghkkkkjjjiiiiiif
     */

    return NULL;
}

int main(int argc, char** argv)
{
    return 0;
}

