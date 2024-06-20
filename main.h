#ifndef MAIN_H
#define MAIN_H


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


#define MAX_NB_WALLS 100
#define ARENA_SIZE 50

// Définition de la structure Position
typedef struct {
    int x, y;
} Position;

typedef struct {
    bool mur_haut;
    bool mur_bas;
    bool mur_gauche;
    bool mur_droite;
    char couleur[10];
} Cell;

// Structure pour représenter un mur
typedef struct {
    Position start;
    Position end;
} Wall;

// Définition de la structure ListNode
typedef struct ListNode {
    Position Pos;                // La valeur du nœud
    struct ListNode* suivant;   // Pointeur vers le prochain nœud
} ListNode;

// Définition de la structure Snake
typedef struct {
    ListNode *tete;
    int length;
    t_move direction;

} Snake;

// Définition de la structure Arena
typedef struct {
    int sizeX, sizeY;
    Wall *walls;
    int numWalls;
    Snake mySnake;
    Snake opponentSnake;
    Cell **cells;
    int tour;
    int *wallss;
    int manche;
} Arena;

// Définition de la structure MoveOption
typedef struct {
    t_move move;
    int restrictions;
} MoveOption;

typedef struct arbre{
    Arena *arena;
    struct arbre *fils[4];
    int coups;
} arbre;

void printTreeNode(arbre *node);

void printTree(arbre *root);

arbre *initTreeRoot(Arena *arena);

arbre *initTreeNode(Arena *arena, int x, int y, int N);

void freeTree(arbre *root);

// Déclarations des fonctions
void initArena(Arena *arena, int sizeX, int sizeY, int *walls, int numWalls, int tour);
int isWall(Arena *arena, int x, int y,Position ma_position);
int isCollision(Arena *arena, int x, int y, t_move move,Position ma_position);
void playGame(Arena *arena, int tour);
void updateSnakePosition(Snake *snake, t_move move);
bool isWithinArena(Arena *arena, int x, int y);
int algo_base(Arena *arena);
int minlist(int *liste);
int maxlist(int *liste);
void initList(Arena *arena, int tour);
void addPos(Arena *arena, Snake *snake, t_move move);
void printmove(t_move move);
int isMySnake(Arena *arena, int x, int y);
void printList(Arena *arena);
int compareMoveOptions(const void *a, const void *b);
void printmove(t_move move);
void freeNode(Arena *arena);
void printWalls(Arena *arena);
int FutureSight(Arena *arena);
int printmur(Arena *arena);
void allocateArena(Arena* arena, int sizeX, int sizeY);
void printArenax(Arena* arena);
t_move infinite_sight(Arena *arena, int N);
void couleur(Arena* arena);
void majCouleur(Arena *arena);
int FutureSight2(Arena *arena);
void printcouleur(Arena *arena);
Arena *makeMove(Arena *arena, t_move move);
t_move FutureSight3(Arena *arena, int depth);
Arena* copyArena(const Arena* arena) ;
int getMaxDepth(arbre *node);
void moveSnake(Arena *arena, t_move move) ;
t_move findBestMove(Arena *arena, int depth, Arena **states);
int removePos(Arena *arena,Snake *snake, t_move move);
int getMoveCount(int x_case, int y_case, Arena* arena, int N, Position* futurs_potentiel, int i, int c) ;
int isSnake(Snake snake,int x,int y);
int printpos(Arena *arena);
t_move validateMoveWithAlgoBase(Arena *arena, t_move initial_move, int levels);
#endif /* MAIN_H */