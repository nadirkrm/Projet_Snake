#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "snakeAPI.h"
#include "clientAPI.h"
#include "main.h"
#define PROFONDEUR 8

void initArena(Arena *arena, int sizeX, int sizeY, int *walls, int numWalls, int tour) {
    arena->sizeX = sizeX;
    arena->sizeY = sizeY;
    arena->numWalls = numWalls ; // Chaque mur est défini par 4 coordonnées
    arena->walls = (Wall *)malloc(arena->numWalls * sizeof(Wall));
    for (int i = 0; i < numWalls; i++) {
        arena->walls[i].start.x = walls[i * 4];
        arena->walls[i].start.y = walls[i * 4 + 1];
        arena->walls[i].end.x = walls[i * 4 + 2];
        arena->walls[i].end.y = walls[i * 4 + 3];
    }

    allocateArena(arena,sizeX,sizeY);
    
    for (int i = 0; i < (sizeX); i++) {
        for (int j = 0; j < (sizeY); j++) {
            if (j == 0) {
                arena->cells[i][j].mur_haut = 1;
            }
            if (j == sizeY - 1) {
                arena->cells[i][j].mur_bas = 1;
            }
            if (i == 0) {
                arena->cells[i][j].mur_gauche = 1;
            } 
            if (i == sizeX - 1) {
                arena->cells[i][j].mur_droite = 1;
            } 
        }
    }

    printArenax(arena);

    for (int i = 0; i < numWalls; i++) {
        int x1 = walls[i*4];
        int y1 = walls[i*4+1];
        int x2 = walls[i*4+2];
        int y2 = walls[i*4+3];
        if (x1 == x2) {
            if (y1 < y2) {
                arena->cells[x1][y1].mur_bas = 1;
                arena->cells[x2][y2].mur_haut = 1;
            } 
            
            else {
                arena->cells[x1][y1].mur_haut = 1;
                arena->cells[x2][y2].mur_bas = 1;
            }
        } 
        
        else{
            if (x1 < x2) {
                arena->cells[x1][y1].mur_droite = 1;
                arena->cells[x2][y2].mur_gauche = 1;
            } 
            
            else {
                arena->cells[x1][y1].mur_gauche = 1;
                arena->cells[x2][y2].mur_droite = 1;
            }
        }
    }

    printArenax(arena);
    // Initialiser la position de notre serpent dans l'attribut tete
    // arena->mySnake.tete = NULL;
    // arena->opponentSnake.tete = NULL;
    if (arena->mySnake.tete == NULL){
        printf("On va initialiser tete, l'instant tete->Position = NULL\n");
    }
    initList(arena, tour);
    printf("Après initlist, on a initialisé tete->Position = (%u,%u)\n",arena->mySnake.tete->Pos.x,arena->mySnake.tete->Pos.y);


    // Initialiser la longueur de notre serpent
    arena->mySnake.length = 1;
    arena->opponentSnake.length = 1;

    // Initialiser les couleurs de l'arène
    couleur(arena);


    
}

void allocateArena(Arena* arena, int sizeX, int sizeY) {

    
    // Allouer la mémoire pour les pointeurs de ligne
    arena->cells = (Cell**)malloc(sizeX * sizeof(Cell*));
    if (arena->cells == NULL) {
        perror("Échec de l'allocation de mémoire pour les lignes");
        exit(EXIT_FAILURE);
    }

    // Allouer la mémoire pour chaque ligne
    for (int i = 0; i < sizeX; i++) {
        arena->cells[i] = (Cell*)malloc(sizeY * sizeof(Cell));
        if (arena->cells[i] == NULL) {
            perror("Échec de l'allocation de mémoire pour les colonnes");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i<sizeX;i++){
        // Initialiser chaque case de la ligne à 0
        for (int j = 0; j < sizeY; j++) {
            
            arena->cells[i][j].mur_haut = 0;
            arena->cells[i][j].mur_bas = 0;
            arena->cells[i][j].mur_gauche = 0;
            arena->cells[i][j].mur_droite = 0;
        }
    }

}
        
void couleur(Arena* arena){
    int count = 0;
    
    // On parcours les cells pour savoir lesquels ont des murs et on leur affecte une couleur en fonction du nombre de mur
    for (int i = 0; i < arena->sizeX; i++) {
        for (int j = 0; j < arena->sizeY; j++) {
            if (arena->cells[i][j].mur_haut == 1) {
                count++;
            }
            if (arena->cells[i][j].mur_bas == 1) {
                count++;
            }
            if (arena->cells[i][j].mur_gauche == 1) {
                count++;
            }
            if (arena->cells[i][j].mur_droite == 1) {
                count++;
            }

            if (count == 0){
                strcpy(arena->cells[i][j].couleur,"Vert");
            }
            if (count == 1){
                strcpy(arena->cells[i][j].couleur,"Jaune");
            }
            if (count == 2){
                strcpy(arena->cells[i][j].couleur,"Orange");
            }
            if (count == 3){
                strcpy(arena->cells[i][j].couleur,"Rouge");
            }
            if (count == 4){
                strcpy(arena->cells[i][j].couleur,"Noir");
            }
        }
                
    }

}

void majCouleur(Arena *arena){

    // On met les couleurs de base de l'arène
    couleur(arena);

    // On augmente l'intensité de la couleur s'il y a un serpent sur la case 
    ListNode *current = arena->mySnake.tete;
    while (current != NULL) {
        int posx = current->Pos.x;
        int posy = current->Pos.y;
        strcpy(arena->cells[posx][posy].couleur,"Noir");
        current = current->suivant;
    }
    current = arena->opponentSnake.tete;
    while (current != NULL) {
        int posx = current->Pos.x;
        int posy = current->Pos.y;
        strcpy(arena->cells[posx][posy].couleur,"Noir");
        current = current->suivant;
    }


}

void initList(Arena *arena, int tour) {
    if (arena == NULL) {
        printf("Error: invalid arena pointer\n");
        return;
    }

    ListNode *MyNode = (ListNode *)malloc(sizeof(ListNode));
    if (MyNode == NULL) {
        printf("Error: malloc failed for mySnake\n");
        
        return;
    }

    ListNode *OpponentNode = (ListNode *)malloc(sizeof(ListNode));
    if (OpponentNode == NULL) {
        printf("Error: malloc failed for opponentSnake\n");
        free(MyNode);  
        return;
    }

    printf("Tout est alloué normalement.\n");

    if (tour == 0) {
        MyNode->Pos.x = 2;
        MyNode->Pos.y = arena->sizeY / 2;
        MyNode->suivant = NULL;
        arena->mySnake.tete = MyNode;
        arena->mySnake.direction = WEST;

        OpponentNode->Pos.x = arena->sizeX - 3;
        OpponentNode->Pos.y = arena->sizeY / 2;
        OpponentNode->suivant = NULL;
        arena->opponentSnake.tete = OpponentNode;
        arena->opponentSnake.direction = EAST;
    } else if (tour == 1) {
        MyNode->Pos.x = arena->sizeX - 3;
        MyNode->Pos.y = arena->sizeY / 2;
        MyNode->suivant = NULL;
        arena->mySnake.tete = MyNode;
        arena->mySnake.direction = EAST;

        OpponentNode->Pos.x = 2;
        OpponentNode->Pos.y = arena->sizeY / 2;
        OpponentNode->suivant = NULL;
        arena->opponentSnake.tete = OpponentNode;
        arena->opponentSnake.direction = WEST;
    }

    printf("On a initialiser tete->Position = (%u,%u)\n",arena->mySnake.tete->Pos.x,arena->mySnake.tete->Pos.y);

}


int max (int a, int b, int c, int d){
    int max = a;
    if (b > max){
        max = b;
    }
    if (c > max){
        max = c;
    }
    if (d > max){
        max = d;
    }
    return max;
}

int min(int a, int b){
    if (a < b) return a;
    return b;
}

void addPos(Arena* arena, Snake* snake, t_move move) {
    if (snake->tete == NULL) {
        printf("Erreur : la tête du serpent est nulle\n");
        return;
    }

    Position tete = snake->tete->Pos;

    switch (move) {
        case NORTH:
            tete.y -= 1;
            break;
        case SOUTH:
            tete.y += 1;
            break;
        case EAST:
            tete.x += 1;
            break;
        case WEST:
            tete.x -= 1;
            break;
    }

    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    newNode->Pos = tete;
    newNode->suivant = snake->tete;
    snake->tete = newNode;

    ListNode* current = snake->tete;
    for (int i = 0; i < snake->length-1; i++) {
        if (current == NULL) break;
        current = current->suivant;
    }

    while (current != NULL && current->suivant != NULL) {
        ListNode* temp = current->suivant;
        current->suivant = temp->suivant;
        free(temp);
    }
}


bool isWithinArena(Arena *arena, int x, int y){
    if (x < 0 || x >= arena->sizeX-1 || y < 0 || y >= arena->sizeY-1) return 1;
    return 0;
}

int isMySnake(Arena *arena, int x, int y) {
    ListNode *list_pos = arena->mySnake.tete;
    for (int i=0; i<arena->mySnake.length;i++){
        
        // Au cas on on cherche une position qui n'existe pas
        if (list_pos == NULL) break;
        if (list_pos->Pos.x == x && list_pos->Pos.y == y) {
            printf("On retombe sur la position (%d,%d) d'indice %d\n",x,y,i);
            return 1; // Le serpent mord sa queue
        }
        list_pos = list_pos->suivant;

    }
    // On check les positions du serpent opposé
    ListNode *list_posopps = arena->opponentSnake.tete;
    for (int i=0; i<arena->opponentSnake.length;i++){
        
        // Au cas on on cherche une position qui n'existe pas
        if (list_posopps == NULL) break;
        if (list_posopps->Pos.x == x && list_posopps->Pos.y == y) {
            printf("On tombe sur la position (%d,%d) d'indice %d dans les positions de l'adversaire\n",x,y,i);
            return 1; // Le serpent mord sa queue
        }
        list_posopps = list_posopps->suivant;

    }

    return 0; // Le serpent ne mord pas sa queue
}

int isOppositeMove(Arena *arena, t_move move){

    if (move == NORTH && arena->mySnake.direction == SOUTH) return 1;
    if (move == SOUTH && arena->mySnake.direction == NORTH) return 1;
    if (move == EAST && arena->mySnake.direction == WEST) return 1;
    if (move == WEST && arena->mySnake.direction == EAST) return 1;
    return 0;

}

void printArenax(Arena* arena) {
    int size_supposed = arena->sizeX*arena->sizeY;
    printf("Arène de taille %d\n",size_supposed);

    int vrai_size = sizeof(arena->cells) / sizeof(Cell);
    printf("Taille alloué =  %d\n",vrai_size);
    printf(".");
    for (int j = 0; j < arena->sizeX; j++) {
        printf("--.");
    }
    printf("\n");
    for (int i = 0; i < arena->sizeY; i++) {
        for (int j = 0; j < arena->sizeX; j++) {
            if (arena->cells[j][i].mur_gauche) {
                printf("|");
            } else {
                printf(" ");
            }
            printf("  ");
        }
        if (arena->cells[arena->sizeX - 1][i].mur_droite) {
            printf("|");
        } else {
            printf(" ");
        }
        printf("\n");
        printf(".");
        for (int j = 0; j < arena->sizeX; j++) {
            if (arena->cells[j][i].mur_bas) {
                printf("--.");
            } else {
                printf("  .");
            }
        }
        printf("\n");
    }
}

int isValidMove(Arena *arena, t_move move) {
    Position next_pos;
    next_pos.x = arena->mySnake.tete->Pos.x;
    next_pos.y = arena->mySnake.tete->Pos.y;
    printmove(move);

    // Mettre à jour la position en fonction du mouvement
    switch (move) {
        case NORTH:
            printf("Move à Valider :  ");
            printmove(NORTH);
            next_pos.y -= 1; // Vers le haut
             // Vérifier si la nouvelle position est en dehors de l'arène
            if (next_pos.x < 0 || next_pos.x >= arena->sizeX || next_pos.y < 0 || next_pos.y >= arena->sizeY) {
                printf("Position hors map: (%d,%d).\n",next_pos.x,next_pos.y);
                return 0;
            }
            else if (arena->cells[next_pos.x][next_pos.y].mur_bas == 1 || isMySnake(arena, next_pos.x, next_pos.y)) {
                printf("Mur ou serpent.\n");
                return 0;
            }
            else {
                printf("Le move à été validé\n");
                break;
            }
        case SOUTH:
            printf("Move à Valider :  ");
            printmove(SOUTH);
            next_pos.y += 1; // Vers le bas
            if (next_pos.x < 0 || next_pos.x >= arena->sizeX || next_pos.y < 0 || next_pos.y >= arena->sizeY) {
                printf("Position hors map: (%d,%d).\n",next_pos.x,next_pos.y);
                return 0;
            }
            else if (arena->cells[next_pos.x][next_pos.y].mur_haut == 1 || isMySnake(arena, next_pos.x, next_pos.y)) {
                printf("Mur ou serpent.\n");
                return 0;
            }
            break;
        case EAST:
            printf("Move à Valider :  ");
            printmove(EAST);
            next_pos.x += 1; // Vers la droite
            if (next_pos.x < 0 || next_pos.x >= arena->sizeX || next_pos.y < 0 || next_pos.y >= arena->sizeY ){
                printf("Position hors map: (%d,%d).\n",next_pos.x,next_pos.y);
                return 0;
            }
            else if (arena->cells[next_pos.x][next_pos.y].mur_gauche == 1 || isMySnake(arena, next_pos.x, next_pos.y)) {
                printf("Mur ou serpent.\n");
                return 0;
            }
            break;
        case WEST:
            printf("Move à Valider :  ");
            printmove(WEST);
            next_pos.x -= 1; // Vers la gauche
            if (next_pos.x < 0 || next_pos.x >= arena->sizeX || next_pos.y < 0 || next_pos.y >= arena->sizeY){
                printf("Position hors map: (%d,%d).\n",next_pos.x,next_pos.y);
                return 0;
            }
            else if (arena->cells[next_pos.x][next_pos.y].mur_droite == 1 || isMySnake(arena, next_pos.x, next_pos.y)) {
                printf("Mur ou serpent.\n");
                return 0;
            }
            break;
    }
   

   

    // La nouvelle position est valide
    return 1;
}


int isPositionExplored(Position pos, Position *futurs_potentiel, int size) {
    for (int i = 0; i < size; i++) {
        if (futurs_potentiel[i].x == pos.x && futurs_potentiel[i].y == pos.y) {
            printf("La position (%d,%d) a déjà été exploré\n",pos.x,pos.y);
            return 1; // La position est explorée
        }
    }
    return 0; // La position n'est pas explorée
}

t_move HakiObservation(Arena* arena) {
    int coups_east = 0;
    int coups_west = 0;
    int coups_north = 0;
    int coups_south = 0;
    int i = 0;

    if (isValidMove(arena, EAST) && !isMySnake(arena, arena->mySnake.tete->Pos.x + 1, arena->mySnake.tete->Pos.y)) {
        printf("Admettons que le premier move soit:\t");
        printmove(EAST);
        Position pos = {arena->mySnake.tete->Pos.x + 1, arena->mySnake.tete->Pos.y};
        Position* futurs_potentiel = malloc(200 * sizeof(Position));
        futurs_potentiel[i].x = pos.x;
        futurs_potentiel[i].y = pos.y;
        coups_east = getMoveCount(pos.x, pos.y, arena, PROFONDEUR, futurs_potentiel, i, 1);
        printf("Force de %d à l'est \n", coups_east);
        free(futurs_potentiel);
    }

    if (isValidMove(arena, NORTH) && !isMySnake(arena, arena->mySnake.tete->Pos.x, arena->mySnake.tete->Pos.y - 1)) {
        printf("Admettons que le premier move soit:\t");
        printmove(NORTH);
        Position pos = {arena->mySnake.tete->Pos.x, arena->mySnake.tete->Pos.y - 1};
        Position* futurs_potentiel = malloc(200 * sizeof(Position));
        futurs_potentiel[i].x = pos.x;
        futurs_potentiel[i].y = pos.y;
        coups_north = getMoveCount(pos.x, pos.y, arena, PROFONDEUR, futurs_potentiel, i, 1);
        printf("Force de %d au nord \n", coups_north);
        free(futurs_potentiel);
    }

    if (isValidMove(arena, WEST) && !isMySnake(arena, arena->mySnake.tete->Pos.x - 1, arena->mySnake.tete->Pos.y)) {
        printf("Admettons que le premier move soit:\t");
        printmove(WEST);
        Position pos = {arena->mySnake.tete->Pos.x - 1, arena->mySnake.tete->Pos.y};
        Position* futurs_potentiel = malloc(200 * sizeof(Position));
        futurs_potentiel[i].x = pos.x;
        futurs_potentiel[i].y = pos.y;
        coups_west = getMoveCount(pos.x, pos.y, arena, PROFONDEUR, futurs_potentiel, i, 1);
        printf("Force de %d à l'ouest \n", coups_west);
        free(futurs_potentiel);
    }

    if (isValidMove(arena, SOUTH) && !isMySnake(arena, arena->mySnake.tete->Pos.x, arena->mySnake.tete->Pos.y + 1)) {
        printf("Admettons que le premier move soit:\t");
        printmove(SOUTH);
        Position pos = {arena->mySnake.tete->Pos.x, arena->mySnake.tete->Pos.y + 1};
        Position* futurs_potentiel = malloc(200 * sizeof(Position));
        futurs_potentiel[i].x = pos.x;
        futurs_potentiel[i].y = pos.y;
        coups_south = getMoveCount(pos.x, pos.y, arena, PROFONDEUR, futurs_potentiel, i, 1);
        printf("Force de %d au sud \n", coups_south);
        free(futurs_potentiel);
    }

    int max_coups = max(coups_east, coups_west, coups_north, coups_south);
    t_move current_direction = arena->mySnake.direction;

    while (max_coups > 0) {
        printf("MaxCoups = %d\n",max_coups);
        switch (current_direction) {
            case EAST:
                if (max_coups == coups_east && isValidMove(arena, EAST)) return EAST;
                if (max_coups == coups_south && isValidMove(arena, SOUTH)) return SOUTH;
                if (max_coups == coups_north && isValidMove(arena, NORTH)) return NORTH;
                break;
            case WEST:
                if (max_coups == coups_west && isValidMove(arena, WEST)) return WEST;
                if (max_coups == coups_north && isValidMove(arena, NORTH)) return NORTH;
                if (max_coups == coups_south && isValidMove(arena, SOUTH)) return SOUTH;
                break;
            case NORTH:
                if (max_coups == coups_north && isValidMove(arena, NORTH)) return NORTH;
                if (max_coups == coups_east && isValidMove(arena, EAST)) return EAST;
                if (max_coups == coups_west && isValidMove(arena, WEST)) return WEST;
                break;
            case SOUTH:
                if (max_coups == coups_south && isValidMove(arena, SOUTH)) return SOUTH;
                if (max_coups == coups_east && isValidMove(arena, EAST)) return EAST;
                if (max_coups == coups_west && isValidMove(arena, WEST)) return WEST;
                break;
        }

        max_coups--;
    }

    // Si aucun mouvement sûr n'est trouvé, retourne SOUTH par défaut.
    return SOUTH;
}


int getMoveCount(int x_case, int y_case, Arena* arena, int N, Position* futurs_potentiel, int i, int c) {
    int x = x_case;
    int y = y_case;
    int coups = 0;
    c++;

    if (N == 0) {
        return coups;
    }

    printf("On explore la position (%d, %d) à profondeur de %d\n", x, y, N);

    // Ajouter la position actuelle à futurs_potentiel
    futurs_potentiel[i].x = x;
    futurs_potentiel[i].y = y;
    i++;

    // Vérifier les mouvements possibles et les explorer
    Position new_pos;

    if (isValidMove(arena, WEST)) {
        new_pos.x = x - 1;
        new_pos.y = y;
        if (!isPositionExplored(new_pos, futurs_potentiel, i) && !isMySnake(arena, new_pos.x, new_pos.y)) {
            printf("Moving WEST to (%d, %d)\n", new_pos.x, new_pos.y);
            coups += 1 + getMoveCount(new_pos.x, new_pos.y, arena, N - 1, futurs_potentiel, i, c);
        }
    }
    if (isValidMove(arena, NORTH)) {
        new_pos.x = x;
        new_pos.y = y - 1;
        if (!isPositionExplored(new_pos, futurs_potentiel, i) && !isMySnake(arena, new_pos.x, new_pos.y)) {
            printf("Moving NORTH to (%d, %d)\n", new_pos.x, new_pos.y);
            coups += 1 + getMoveCount(new_pos.x, new_pos.y, arena, N - 1, futurs_potentiel, i, c);
        }
    }
    if (isValidMove(arena, EAST)) {
        new_pos.x = x + 1;
        new_pos.y = y;
        if (!isPositionExplored(new_pos, futurs_potentiel, i) && !isMySnake(arena, new_pos.x, new_pos.y)) {
            printf("Moving EAST to (%d, %d)\n", new_pos.x, new_pos.y);
            coups += 1 + getMoveCount(new_pos.x, new_pos.y, arena, N - 1, futurs_potentiel, i, c);
        }
    }
    if (isValidMove(arena, SOUTH)) {
        new_pos.x = x;
        new_pos.y = y + 1;
        if (!isPositionExplored(new_pos, futurs_potentiel, i) && !isMySnake(arena, new_pos.x, new_pos.y)) {
            printf("Moving SOUTH to (%d, %d)\n", new_pos.x, new_pos.y);
            coups += 1 + getMoveCount(new_pos.x, new_pos.y, arena, N - 1, futurs_potentiel, i, c);
        }
    }

    return coups;
}


int printmur(Arena *arena){
    for (int i = 0; i < arena->sizeY; i++){
        for (int j = 0; j < arena->sizeX; j++){
            printf("À la coordonnée : (%d ; %d), on a :\n",i,j);
            printf("Mur en haut : %d\n",arena->cells[j][i].mur_haut);
            printf("Mur en bas : %d\n",arena->cells[j][i].mur_bas);
            printf("Mur à gauche : %d\n",arena->cells[j][i].mur_gauche);
            printf("Mur à droite : %d\n",arena->cells[j][i].mur_droite);
        }
    }
    return 0;
}

void playGame(Arena *arena, int tour){
    // Attend que l'on ouvre localhost
    sleep(2);

    // Affiche les dimensions de l'arene
    printf("Arena: %d x %d\n", arena->sizeX, arena->sizeY);

    // Extract the snakes positions
    printf("MySnake position: (%d;%d)\n", arena->mySnake.tete->Pos.x,arena->mySnake.tete->Pos.y);
    printf("MySnake length: %d\n", arena->mySnake.length);
    printf("OpponentSnake position: (%d,%d)\n", arena->opponentSnake.tete->Pos.x,arena->opponentSnake.tete->Pos.y);
    printf("OpponentSnake length: %d\n", arena->opponentSnake.length);
    printf("Tour: %d\n",tour);
    int manche = 0;


    // Play the game
    if (tour == 0){ // C'est à nous de commencer
        while(1){
            
            printArena();
            printf("Position actuelle : (%d,%d)\n",arena->mySnake.tete->Pos.x, arena->mySnake.tete->Pos.y);

            // Logique de prise de décision lvl 3
            t_move move = HakiObservation(arena);

            // Mettre à jour la position du serpent en fonction du mouvement choisi
            printmove(move);
            addPos(arena, &arena->mySnake, move);

            //updateList(arena);
            t_return_code ret = sendMove(move);
            // Incrémenter la taille des serpents toutes les 10 manches
            if (manche % 10 == 0 ){ 
                arena->mySnake.length++;
            }
            if (ret == WINNING_MOVE) {
                printf("Vous avez gagné !\n");
                break;
            } 
            else if (ret == LOSING_MOVE) {
                // On print les murs
                //printmur(arena);
                printf("Votre serpent a atteint une longueur de %d \n",arena->mySnake.length);
                printpos(arena);
                printf("Vous avez perdu.\n");
                break;
            }
            t_move opponentMove;
            ret = getMove(&opponentMove);
            addPos(arena, &arena->opponentSnake, opponentMove);
            if (ret == WINNING_MOVE) {
                printf("Votre adversaire a gagné.\n");
                printpos(arena);
                //(arena);
                break;
            } else if (ret == LOSING_MOVE) {
                printf("Votre adversaire a perdu.\n");
                printf("VOUS AVEZ DONC GAGNÉ !!!");
                break;
            }
            
            arena->mySnake.direction = move;
            arena->opponentSnake.direction = opponentMove;


            // Incrémenter la taille des serpents toutes les 10 manches
            if (manche % 10 == 0 ){ 
                arena->opponentSnake.length++;
            }
            manche++;

        }

    }
    else if (tour == 1){ // C'est à l'adversaire de commencer
        while(1){
            printArena();

            //sleep(5);

            // Prise en compte du mouvement de l'adversaire
            t_move opponentMove;
            t_return_code ret = getMove(&opponentMove);
            // Incrémenter la taille des serpents toutes les 10 manches
            if (manche % 10 == 0 ){ 
                arena->opponentSnake.length++;
            }
            if (ret == WINNING_MOVE) {
                printf("Votre adversaire a gagné.\n");
                printf("Votre serpent a atteint une longueur de %d\n",arena->mySnake.length);
                printpos(arena);
                break;
            } else if (ret == LOSING_MOVE) {
                printf("Votre adversaire a perdu\n");
                printf("VOUS AVEZ GAGNÉ !!");
                break;
            }
            addPos(arena, &arena->opponentSnake, opponentMove);

            // Print toutes les cases de mon serpent 
            printf("Position actuelle : (%d,%d)\n",arena->mySnake.tete->Pos.x, arena->mySnake.tete->Pos.y);

            // Logique de prise de décision lvl 3      
            t_move move = HakiObservation(arena);

            // Mettre à jour la position du serpent en fonction du mouvement choisi
            printf("%d", move);

            addPos(arena, &arena->mySnake, move);
            ret = sendMove(move);
            // Incrémenter la taille des serpents toutes les 10 manches
            if (manche % 10 == 0 ){ 
                arena->mySnake.length++;
            }
            if (ret == WINNING_MOVE) {
                printf("Vous avez gagné !\n");
                break;
            } 
            else if (ret == LOSING_MOVE) {
                printf("Votre serpent a atteint une longueur de %d\n",arena->mySnake.length);
                printpos(arena);
                printf("%u\n", move);
                printf("Vous avez perdu.\n");
                break;
            }
            
            arena->mySnake.direction = move;
            arena->opponentSnake.direction = opponentMove;

            // Incrémenter la taille des serpents toutes les 10 manches
            if (manche % 10 == 0 && manche != 0){ // Correction pour correspondre aux changements spécifiés
                arena->opponentSnake.length++;
            }
            manche++;

            
        }

    }
}

// On print les position de chaque serpent
int printpos(Arena *arena){
    ListNode *current = arena->mySnake.tete;
    while (current != NULL) {
        printf("(%d,%d)\t",current->Pos.x,current->Pos.y);
        current = current->suivant;
    }
    printf("\n");
    current = arena->opponentSnake.tete;
    while (current != NULL) {
        printf("(%d,%d)\t",current->Pos.x,current->Pos.y);
        current = current->suivant;
    }
    printf("\n");
    return 0;

}

void printmove(t_move move){
    if (move == NORTH){
        printf("NORTH\n");
    }
    else if (move == SOUTH){
        printf("SOUTH\n");
    }
    else if (move == EAST){
        printf("EAST\n");
    }
    else if (move == WEST){
        printf("WEST\n");
    }

}
        
int main(int argc, char **argv) {

    // Connexion au serveur et initialisation de la game
    connectToServer("localhost", 3456, "Bob");
    printf("Connexion Réalisé !\n");
        char gameName[50];
        int sizeX, sizeY, nbWalls;
        int tour;
        int walls[MAX_NB_WALLS * 4];
        
        waitForSnakeGame( "TRAINING SUPER_PLAYER ",gameName, &sizeX, &sizeY, &nbWalls);
        tour = getSnakeArena(walls);

        // Initialisation de la structure arena
        Arena arena;
        
        // Initialiser l'arène
        initArena(&arena, sizeX, sizeY, walls, nbWalls,tour);
        printf("Tour : %d\n",tour);
        

        //printArenax(&arena);

        // Commencer l'interraction des bots
        playGame(&arena,tour);

    // Fermer la connexion
    closeConnection();

    return 0;
}