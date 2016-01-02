#include <stdio.h>
#include <stdlib.h>

#define FC_Malloc malloc

typedef struct Board Board;

enum {
    Blue = 1,
    Yellow,
    Green,
    Red,
};

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

static int BoardGet(Board *b, int x, int y)
{
    return 0; // TODO
}

static void BoardSet(Board *b, int x, int y, int v)
{
    b->body[y * b->width + x] = v;
}

static Board *ReadBoard()
{
    // 19 * 19
    const char *q =
        "AAAAABCCCCCDDEEEEEE\n"
        "AFFFBBBBBBCDDEGGGGE\n"
        "AFAFFFFBCBCDEEGEEEE\n"
        "AAAHHHFBCCCDDGGGGII\n"
        "JJJJJHHHCKLLDDDDDDI\n"
        "JMMMMMMHNKOLLLLLLII\n"
        "JPPPPPMNNKOLLIIIIIQ\n"
        "JPMMMMMNKKORRRRRRRQ\n"
        "JPPSSSNNKKORTTTTTQQ\n"
        "JPSSUOOOOOORTVVVVVQ\n"
        "SSSUUOWWWWWWTXXXXVQ\n"
        "YYYUUZZZZZZZTXXXXVQ\n"
        "YaYYYZbbbbXXXXVVVVQ\n"
        "YaaZZZbcccccdeVQQQQ\n"
        "YYaabbbcdddddefffff\n"
        "gggahhhcdeeeeefiiif\n"
        "gagahccchhejjjfffif\n"
        "gaaahhhhhjejijfffif\n"
        "gggghkkkkjjjiiiiiif\n";

    Board *b = BoardNew(19, 19);

    for (int h = 0; h < b->height; h++) {
        for (int w = 0; w < b->width; w++) {
            BoardSet(b, w, h, q[h * (b->width + 1) + w]);
        }
    }

    return b;
}

int main(int argc, char** argv)
{
    Board *b = ReadBoard();
    return 0;
}

