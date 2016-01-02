#include <gc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define FC_Malloc GC_MALLOC

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
    Node *next;
};

typedef struct Board Board;

struct Board
{
    int width;
    int height;

    int *body;
    int *colors; // init colors

    int *map; // used signes (size is sign_count)

    int sign_count;

    Node *nodes;
};

static Node *NodeNew(int a, int b)
{
    Node *n = FC_Malloc(sizeof(Node));
    n->a = a;
    n->b = b;
    n->next = NULL;
    return n;
}

#define SIGN_MAX 256

static Board *BoardNew(int width, int height)
{
    Board *b = FC_Malloc(sizeof(Board));

    b->width = width;
    b->height = height;

    b->body = FC_Malloc(sizeof(int) * width * height);

    b->colors = FC_Malloc(sizeof(int) * SIGN_MAX);
    bzero(b->colors, sizeof(int) * SIGN_MAX);

    b->map = NULL;
    b->nodes = NULL;

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

static bool BoardHasNode(Board *board, int a, int b)
{
    for (Node *p = board->nodes; p != NULL; p = p->next)
    {
        if ((p->a == a && p->b == b) ||
            (p->a == b && p->b == a))
        {
            return true;
        }
    }
    return false;
}

static void BoardTryAddNode(Board *board, int a, int b)
{
    if (!BoardHasNode(board, a, b)) {
        Node *n = NodeNew(a, b);
        n->next = board->nodes;
        board->nodes = n;
    }
}

static void BoardDetectNodes(Board *b)
{
    for (int y = 0; y < b->height; y++) {
        for (int x = 0; x < b->width; x++) {
            int i = BoardGet(b, x, y);

            if (x < b->width - 1) {
                int j = BoardGet(b, x + 1, y);
                if (i != j) {
                    BoardTryAddNode(b, i, j);
                }
            }

            if (y < b->height - 1) {
                int k = BoardGet(b, x, y + 1);
                if (i != k) {
                    BoardTryAddNode(b, i, k);
                }
            }
        }
    }
}

static bool BoardFinished(Board *b, int *colors)
{
    for (Node *p = b->nodes; p != NULL; p = p->next) {
        int c1 = colors[p->a];
        int c2 = colors[p->b];

        if (c1 == 0 || c2 == 0 || c1 == c2) {
            return false;
        }
    }

    return true;
}

static bool BoardFailed(Board *b, int *colors)
{
    for (Node *p = b->nodes; p != NULL; p = p->next) {
        int c1 = colors[p->a];
        int c2 = colors[p->b];

        if (c1 != 0 && c2 != 0 && c1 == c2) {
            return true;
        }
    }

    return false;
}

static void BoardPrint(Board *b, int *colors)
{
    for (int y = 0; y < b->height; y++) {
        for (int x = 0; x < b->width; x++) {
            int c = colors[BoardGet(b, x, y)];
            PrintDot(c);
        }
        printf("\n");
    }
}

static void BoardPrintDetail(Board *b)
{
    printf("-- nodes --\n");
    for (Node *p = b->nodes; p != NULL; p = p->next) {
        printf("%c - %c\n", p->a, p->b);
    }

    printf("-- map --\n");
    for (int i = 0; i < b->sign_count; i++) {
        printf("%c\n", b->map[i]);
    }

    printf("--\n");
    printf("sign_count: %d\n", b->sign_count);
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

    int *map = FC_Malloc(sizeof(int) * SIGN_MAX);
    bzero(map, sizeof(int) * SIGN_MAX);

    for (int h = 0; h < b->height; h++) {
        for (int w = 0; w < b->width; w++) {
            int s = q[h * (b->width + 1) + w];
            BoardSet(b, w, h, s);
            map[s] = 1;
        }
    }

    b->sign_count = 0;
    for (int i = 0; i < SIGN_MAX; i++) {
        b->sign_count += map[i];
    }

    b->map = FC_Malloc(sizeof(int) * b->sign_count);

    for (int i = 0, j = 0; i < SIGN_MAX; i++) {
        if (map[i] != 0) {
            b->map[j++] = i;
        }
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

    BoardDetectNodes(b);

    return b;
}

static void FillTrivial(Board *b, int *colors)
{
    while (true) {
        bool filled = false;

        for (int i = 0; i < b->sign_count; i++) {
            uint8_t bits = 0;
            int s = b->map[i];

            if (colors[s] != 0) {
                continue;
            }

            for (Node *p = b->nodes; p != NULL; p = p->next) {
                if (p->a == s) {
                    bits |= 1 << (colors[p->b] - 1);
                }
                if (p->b == s) {
                    bits |= 1 << (colors[p->a] - 1);
                }
            }

            switch (bits) {
            case 0b1110:
                colors[s] = 1;
                filled = true;
                break;

            case 0b1101:
                colors[s] = 2;
                filled = true;
                break;

            case 0b1011:
                colors[s] = 3;
                filled = true;
                break;

            case 0b0111:
                colors[s] = 4;
                filled = true;
                break;
            }
        }

        if (!filled) {
            return;
        }
    }
}

typedef struct CandNode CandNode;

struct CandNode
{
    int i;
    CandNode *next;
};

static int **NextCandidates(Board *b, int *colors)
{
    int empty = 0;

    CandNode *head = NULL;

    for (int i = 0; i < b->sign_count; i++) {
        if (colors[b->map[i]] == 0) {
            CandNode *p = FC_Malloc(sizeof(CandNode));
            p->i = b->map[i];
            p->next = head;
            head = p;
            empty++;
        }
    }

    int **cs = FC_Malloc(sizeof(int*) * (empty * 4 + 1));

    int j = 0;
    for (CandNode *p = head; p != NULL; p = p->next) {
        for (int c = 1; c <= 4; c++) {
            cs[j] = FC_Malloc(sizeof(int) * SIGN_MAX);
            memcpy(cs[j], colors, sizeof(int) * SIGN_MAX);
            cs[j][p->i] = c;
            j++;
        }
    }
    cs[j] = NULL;

    return cs;
}

static bool Solve(Board *b, int *colors, int *result)
{
    if (BoardFailed(b, colors)) {
        return false;
    }

    if (BoardFinished(b, colors)) {
        memcpy(result, colors, sizeof(int) * SIGN_MAX);
        return true;
    }

    FillTrivial(b, colors);

    int **cs = NextCandidates(b, colors);

    for (int i = 0; cs[i] != NULL; i++) {
        if (Solve(b, cs[i], result)) {
            return true;
        }
    }

    return false;
}

int main(int argc, char** argv)
{
    GC_INIT();

    Board *b = ReadBoard();
    BoardPrint(b, b->colors);
    BoardPrintDetail(b);

    //*
    int **cs = NextCandidates(b, b->colors);

    for (int i = 0; cs[i] != NULL; i++) {
        printf("------------------%d\n", i);
        BoardPrint(b, cs[i]);
    }
    //*/

    /*
    FillTrivial(b, b->colors);
    BoardPrint(b, b->colors);
    return 0;
    //*/

    int *result = FC_Malloc(sizeof(int) * SIGN_MAX);
    if (Solve(b, b->colors, result)) {
        printf("solved!\n");
        BoardPrint(b, result);
    } else {
        printf("failed\n");
    }

    return 0;
}
