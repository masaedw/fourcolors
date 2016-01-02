#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define FC_Malloc malloc

enum {
    Blue = 1,
    Yellow,
    Green,
    Red,
};

typedef struct Node Node;

struct Node
{
    int a;
    int b;
};

typedef struct Board Board;

struct Board
{
    int width;
    int height;

    int *body;
    int *colors;

    int *map; // used chars

    int sign_count;
};

#define SIGN_MAX 256

static Board *BoardNew(int width, int height)
{
    Board *b = FC_Malloc(sizeof(Board));

    b->width = width;
    b->height = height;

    b->body = FC_Malloc(sizeof(int) * width * height);
    b->colors = FC_Malloc(sizeof(int) * SIGN_MAX);
    bzero(b->colors, sizeof(int) * SIGN_MAX);

    b->map = FC_Malloc(sizeof(int) * SIGN_MAX);
    bzero(b->map, sizeof(int) * SIGN_MAX);

    return b;
}

static int BoardGet(Board *b, int x, int y)
{
    return b->body[y * b->width + x];
}

static void BoardSet(Board *b, int x, int y, int v)
{
    b->body[y * b->width + x] = v;
}

static void PrintDot(int color)
{
    const char *c = "47";

    switch (color) {
        case Blue:
            c = "44"; // blue
            break;
        case Yellow:
            c = "43"; // yellow
            break;
        case Green:
            c = "42"; // green
            break;
        case Red:
            c = "41";
            break;
    }

    printf("\033[%sm  \033[49m", c);
}

static void BoardPrint(Board *b)
{
    for (int y = 0; y < b->height; y++) {
        for (int x = 0; x < b->width; x++) {
            int c = b->colors[BoardGet(b, x, y)];
            PrintDot(c);
        }
        printf("\n");
    }
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
            int s = q[h * (b->width + 1) + w];
            BoardSet(b, w, h, s);
            b->map[s] = 1;
        }
    }

    b->sign_count = 0;
    for (int i = 0; i < SIGN_MAX; i++) {
        b->sign_count += b->map[i];
    }

    b->colors['F'] = Green;
    b->colors['E'] = Red;
    b->colors['P'] = Blue;
    b->colors['N'] = Blue;
    b->colors['K'] = Green;
    b->colors['R'] = Red;
    b->colors['Z'] = Green;
    b->colors['g'] = Red;
    b->colors['k'] = Green;
    b->colors['i'] = Yellow;

    return b;
}

int main(int argc, char** argv)
{
    Board *b = ReadBoard();
    BoardPrint(b);
    printf("%d\n", b->sign_count);
    return 0;
}

