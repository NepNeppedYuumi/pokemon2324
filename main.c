#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <io.h>
#define F_OK 0
#define access _access

#define POKEMON_NAME_LENGTH 13
#define MOVE_NAME_LENGTH 17
#define AMOUNT_OF_POKEMON 150
#define AMOUNT_OF_MOVES 167
#define TEAM_SIZE 4
#define IMAGE_PATH "pokepack/001.txt"
#define IMAGE_PATH_SIZE (sizeof(IMAGE_PATH) / sizeof(IMAGE_PATH[0]))


//#region FOREACH_TYPE
#define FOREACH_TYPE(TYPE) \
        TYPE(Normal)   \
        TYPE(Fighting)  \
        TYPE(Flying)   \
        TYPE(Poison)  \
        TYPE(Ground)  \
        TYPE(Rock)  \
        TYPE(Bug)  \
        TYPE(Ghost)  \
        TYPE(Steel)  \
        TYPE(Fire)  \
        TYPE(Water)  \
        TYPE(Grass)  \
        TYPE(Electric)  \
        TYPE(Psychic)  \
        TYPE(Ice)  \
        TYPE(Dragon)  \
        TYPE(Dark)  \
        TYPE(Fairy)  \

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,
//#endregionregion
typedef enum {
    None = 0,
    FOREACH_TYPE(GENERATE_ENUM)
} TYPE_ENUM;

static char *TYPE_STRING[] = {
    [0] = "",
    FOREACH_TYPE(GENERATE_STRING)
};
#define TYPE_AMOUNT sizeof(TYPE_STRING) / sizeof(TYPE_STRING[0])
// #define str(x) #x
// #define xstr(x) str(x)


typedef struct {
    unsigned char id;
    char name[POKEMON_NAME_LENGTH];
    unsigned char type1;
    unsigned char type2;
    unsigned char hp;
    unsigned char attack;
    unsigned char defense;
    unsigned char speed;
    bool hasImage;
} BasePokemon;


typedef struct {
    BasePokemon *base;
    int currentHp;
    unsigned char moves[4];
} Pokemon;


typedef struct {
    char name[MOVE_NAME_LENGTH];
    unsigned char type;
} Move;


unsigned int fastAtou(const char *str);
unsigned char typeEnumValueFromString(char *type);
static int tolower_ctype(char* str);
int randint(int n);
void imagePrinter(BasePokemon *pokemon);
int pokemonImagePath(char *image_path, char *idStr);
int fillPokemonArray(BasePokemon pokemonArray[], char fileName[]);
int fillMovesArray(Move movesArray[], char fileName[]);
int pokedex(BasePokemon pokemonArray[]);
int selectStarter(BasePokemon pokemonArray[]);
void assignPokemon(BasePokemon pokemonArray[], Move moves[], Pokemon *pPokemon, int id);
void showTeam(Pokemon playerTeam[]);
int randomOpponent(Pokemon playerTeam[]);
int chooseTeamPokemon(BasePokemon pokemonArray[], Pokemon team[]);
void setCurrentHpMax(Pokemon *pPokemon);
int fight(Move moves[],Pokemon *pPlayer, Pokemon *pOpponent);




int main () {
    srand((unsigned int)time(NULL));

    printf(""); // It doesn't run if this is not here???
    
    BasePokemon pokemonArray[AMOUNT_OF_POKEMON];
    Move movesArray[AMOUNT_OF_MOVES];

    int paFillStatus = fillPokemonArray(pokemonArray, "pokemon_150.txt");
    int mFillStatus = fillMovesArray(movesArray, "combat_moves.csv");

    Pokemon playerTeam[TEAM_SIZE];
    playerTeam[0].base = NULL;

    // test code comes from here

    //

    char input = '0';
    while (input != '5'){
        printf(
            "\n[1] Pokedex raadplegen\n"
            "[2] Nieuw Pokemon team starten\n"
            "[3] Nieuwe Pokemon vangen\n"
            "[4] Team tonen\n"
            "[5] Stoppen\n\n"
            "Wat wil je doen?\n"
            "> "
        );
        scanf(" %c", &input);
        printf("\n");

        switch (input)
        {
            case '1':
                pokedex(pokemonArray);
                break;
            case '2':
                for (int i = 0; i < TEAM_SIZE; i++){
                    playerTeam[i].base = NULL;
                }
                int starter = selectStarter(pokemonArray);
                assignPokemon(pokemonArray, movesArray, &playerTeam[0], starter);
                break;
            case '3':
                if (playerTeam[0].base == NULL){
                    printf("Stel eerst een team samen voordat je deze keuze kunt maken.\n");
                    continue;
                }

                printf("Je gaat op weg om nieuwe pokemon te vangen.\n");

                Pokemon opponent;

                int opponentId = randomOpponent(playerTeam);
                assignPokemon(pokemonArray, movesArray, &opponent, opponentId);
                
                imagePrinter(opponent.base);

                printf("\nJe komt een wilde %s tegen! Welke pokemon wil je tegen de wilde %s gebruiken?\n\n", 
                        opponent.base->name, opponent.base->name);
                
                showTeam(playerTeam);
                int chosenPokemon = 0;


                printf("Het gevecht tussen %s en %s begint!\n", playerTeam[chosenPokemon].base->name, opponent.base->name);

                int result = fight(movesArray, &playerTeam[chosenPokemon], &opponent);
                setCurrentHpMax(&playerTeam[chosenPokemon]);

                if (result == 0){
                    int i;
                    for (i = 0; i < TEAM_SIZE; i++){
                        if (playerTeam[i].base == NULL){
                            break;
                        }
                    }
                    playerTeam[i].base = opponent.base;
                    setCurrentHpMax(&playerTeam[i]);
                    for (int j; j < 4; j++){
                        playerTeam[i].moves[j] = opponent.moves[j];
                    }
                    printf("%s is toegevoegd aan je team!\n", opponent.base->name);
                    for (i = 0; i < TEAM_SIZE; i++){
                        if (playerTeam[i].base == NULL){
                            break;
                        }
                    }
                    if (i == TEAM_SIZE){
                        printf("Gefeliciteerd! Je hebt een team van %d pokemon!\nTot de volgende keer\n", TEAM_SIZE);
                        return 0;
                    }
                } else {
                    printf("Helaas je hebt niet gewonnen van de wilde pokemon!\n");
                }
                break;
            case '4':
                if (playerTeam[0].base == NULL){
                    printf("Stel eerst een team samen voordat je deze keuze kunt maken.\n");
                    continue;
                }
                showTeam(playerTeam);
                break;
            case '5': 
                printf("Goodbye world\n");
                return 0;
                break;
            default:
                printf("vul een van de correcte opties in.\n");
                break;
        }
    };
    return 0;
}


unsigned int fastAtou(const char *str)
{
    unsigned int val = 0;
    while (*str) {
        val = val * 10 + (*str - '0');
        str++;
    }
    return val;
}   


unsigned char typeEnumValueFromString(char *type){
    for (unsigned char i = 0; i < TYPE_AMOUNT; i++){
        if (strcmp(TYPE_STRING[i], type) == 0){
            return i;
        }
    }
    return 0; 
}


int randint(int n) {
  if ((n - 1) == RAND_MAX) {
    return rand();
  } else {
    int end = RAND_MAX / n; // truncate skew
    end *= n;

    // ... and ignore results from rand() that fall above that limit.
    // (Worst case the loop condition should succeed 50% of the time,
    // so we can expect to bail out of this loop pretty quickly.)
    int r;
    while ((r = rand()) >= end);

    return r % n;
  }
}


static int tolower_ctype(char* str) {
  char *p = str;
  for ( ; *p; ++p) *p = tolower(*p);
  return 0;
}


int pokemonImagePath(char *imagePath, char *idStr){
    strcpy(imagePath, IMAGE_PATH);
    int offset = IMAGE_PATH_SIZE - 8;
    for (int i = 0; i < + 3; i++){
        imagePath[i + offset] = idStr[i];
    }
    return 0;
}


int fillPokemonArray(BasePokemon pokemonArray[], char fileName[]){
    FILE *file;

    file = fopen(fileName, "r");

    if (file == NULL){
        printf("Error opening the Pokémon file.\n");
        return 1;
    }

    fscanf(file, "%*[^\n]\n");

    char tempData[8][13];

    char image_path[IMAGE_PATH_SIZE];
    bool hasImage;

    char *line;
    int row = 0;

    int column, i, k, id;

    while (fgets(line, 100, file)){
        column = 0;
        k = 0;
        for (i = 0; line[i] != '\n'; i++){
            if (line[i] == ','){
                tempData[column][k] = '\0';
                column++;
                k = 0;
            } else{
                tempData[column][k] = line[i];
                k++;
            }
        }

        pokemonImagePath(image_path, tempData[0]);
        hasImage = access(image_path, F_OK) == 0;

        BasePokemon tempPokemon = {
            fastAtou(tempData[0]),
            "",
            typeEnumValueFromString(tempData[2]),
            typeEnumValueFromString(tempData[3]),
            fastAtou(tempData[4]),
            fastAtou(tempData[5]),
            fastAtou(tempData[6]),
            fastAtou(tempData[7]),
            hasImage
        };
        strcpy(tempPokemon.name, tempData[1]);

        //exists because in this arbritary assignment not image files are present.


        id = tempPokemon.id - 1;
        pokemonArray[id] = tempPokemon;
        row++;
    }

    // BasePokemon p1 = pokemonArray[130];
    // printf("%03d %s %s %d\n", p1.id, p1.name, TYPE_STRING[p1.type1], p1.speed);

    return 0;
}


int fillMovesArray(Move movesArray[], char fileName[]){
    FILE *file;

    file = fopen(fileName, "r");

    char tempData[2][MOVE_NAME_LENGTH];

    char line[100];
    int row = 0;

    int column, i, k, id;

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)){
        column = 0;
        k = 0;
        for (i = 0; line[i] != '\n'; i++){
            if (line[i] == ';'){
                tempData[column][k] = '\0';
                column++;
                k = 0;
            } else{
                tempData[column][k] = line[i];
                k++;
            }
        }
        tempData[column][k] = '\0';

        Move tempMove = {
            "",
            typeEnumValueFromString(tempData[1])
        };
        strcpy(tempMove.name, tempData[0]);

        movesArray[row] = tempMove;

        row++;
    }
    return 0;
}


void imagePrinter(BasePokemon *pokemon){
    char fileName[IMAGE_PATH_SIZE];
    char idStr[4];

    sprintf(idStr, "%03d", pokemon->id);
    pokemonImagePath(fileName, idStr);

    FILE *file;

    file = fopen(fileName, "r");

    int c;
    if (file) {
        while ((c = getc(file)) != EOF){
            putchar(c);
        }
        fclose(file);
    }
    // printf("\n");
    printf("\n[#:%03d][Name:%s][Type 1:%s][Type 2:%s][HP:%d][Attack:%d][Defense:%d][Speed:%d]\n",
    pokemon->id, 
    pokemon->name, 
    TYPE_STRING[pokemon->type1], 
    TYPE_STRING[pokemon->type2],
    pokemon->hp, pokemon->attack, pokemon->defense, pokemon->speed);
}


int inputPokemon(BasePokemon pokemonArray[]){
    int i;
    char input[POKEMON_NAME_LENGTH];

    int numericInput;
    int tempId; 

    char numStr[4];

    scanf(" %s", input);
    input[13] = '\0';

    numericInput = 1;
    for (i = 0; input[i] != '\0'; i++){
        if (!isdigit(input[i])){
            numericInput = 0;
            return -1;
        }
    }
    if (numericInput){
        if (i > 3){
            return -1;
        }
        strncpy(numStr, input, 4);
        tempId = fastAtou(numStr);
        if (tempId >= AMOUNT_OF_POKEMON){
            return -1;
        }
        if (!pokemonArray[tempId - 1].hasImage){
            return -1;
        }
        return tempId - 1;
    } else {
        for (i = 0; i < AMOUNT_OF_POKEMON; i++){
            if (stricmp(input, pokemonArray[i].name) == 0){
                if (pokemonArray[i].hasImage){
                    return i + AMOUNT_OF_POKEMON; // this is done to be able to see if the input was a number or name
                    // the return value will always be bigger than the amount of pokemon if the input is a name
                } else{
                    return -2;
                }
            }
        }
    }
}


int pokedex(BasePokemon pokemonArray[]){
     for (int i = 0; i < AMOUNT_OF_POKEMON; i++){
        if (pokemonArray[i].hasImage){
            printf("%03d %s\n", pokemonArray[i].id, pokemonArray[i].name);
        }
    }
    int id;
    while (1){
        printf("\nWelke pokemon wil je bekijken?\n> ");
        id = (inputPokemon(pokemonArray) % AMOUNT_OF_POKEMON) + 1;

        if (id == -1){
            printf("Deze naam komt niet voor in dit pokedex.\n");
        } else if (id == -2){
            printf("Dit nummer komt niet voor in dit pokedex.\n");
        }
        break;
    }

    BasePokemon p = pokemonArray[id - 1];
    BasePokemon *pp = &p;

    imagePrinter(pp);
}


int chooseTeamPokemon(BasePokemon pokemonArray[], Pokemon team[]){
    int id, actualId;
    int i;
    while (1){
        printf("\nKies je teampokemon uit met een nummer of naam:\n> ");
        id = inputPokemon(pokemonArray);
        
        for (i = 0; i < TEAM_SIZE; i++){
            if (team[i].base->id == ((id % AMOUNT_OF_POKEMON) + 1)){
                break;
            }
        }

        if (id == -1 || (i == TEAM_SIZE && id <= AMOUNT_OF_POKEMON)){
            printf("Dit nummer komt niet voor in dit team.\n");
        } else if (id == -2 || i == TEAM_SIZE){
            printf("Dit naam komt niet voor in dit team.\n");
        } else {
            return i;
        }
    }

}


void setCurrentHpMax(Pokemon *pPokemon){
    pPokemon->currentHp = pPokemon->base->hp;
}


void assignPokemon(BasePokemon pokemonArray[], Move moves[], Pokemon *pPokemon, int id){
    pPokemon->base = &pokemonArray[id - 1];

    setCurrentHpMax(pPokemon);

    int i = 0;
    int j = 0;
    int r;
    int t1 = pPokemon->base->type1, t2 = pPokemon->base->type2;
    while (i != 4) {
        r = randint(AMOUNT_OF_MOVES);
        if (moves[r].type == t1 || moves[r].type == t2) {
            for (j = 0; j < i; j++){
                if (pPokemon->moves[j] == r){
                    j = i + 1;
                    break;
                }
            }
            if (j == i) {
                pPokemon->moves[i] = r;
                i++;
            }
        }
    }
}


int selectStarter(BasePokemon pokemonArray[]){
    char starters[] = {1, 4, 7};
    
    char starterChoice;
    int chosenId;
    char confirmation;
    while (1) {
        printf("Welke start-pokemon wil je gaan gebruiken? Je kunt een keuze maken uit:\n");

        for (int i = 0; i < 3; i++){
            int id = starters[i] - 1;
            printf("[%d] %s\n", i + 1, pokemonArray[id].name);
        }
        printf("\nGeef je keuze op: ");
        scanf(" %c", &starterChoice);

        if (starterChoice < '1' || starterChoice > '3'){
            continue;
        }
        chosenId = starters[(starterChoice - '1')];
        printf("Je hebt gekozen voor %s!\n", pokemonArray[chosenId - 1].name);
        imagePrinter(&pokemonArray[chosenId - 1]);
        while (1){
            printf("\nBen je zeker van je keuze?(j/n) ");
            scanf(" %c", &confirmation);
            if (confirmation == 'j' || confirmation == 'J'){
                return chosenId;
            } else if (confirmation == 'n' || confirmation == 'N'){
                break;
            }
        }
    }
}


void showTeam(Pokemon playerTeam[]){
    int stBuf = 12;
    int nameBuf = 12;
    int endBuf = 5;
    printf("%-*s%-*s%-*s%-*s%-*s%-*s%-*s%-*s\n", 
            stBuf, "#", 
            nameBuf, "Name", 
            stBuf, "Type 1", 
            stBuf, "Type 2", 
            stBuf, "HP", 
            stBuf, "Attack", 
            stBuf, "Defense", 
            endBuf, "Speed"
            );
    for (int i = 0; i < (stBuf * 6 + nameBuf + endBuf); i++){
        putchar('-');
    }
    putchar('\n');
    int id;

    for (int i = 0; i < TEAM_SIZE; i++){
        if (playerTeam[i].base == NULL){
            continue;
        }

        printf("%-0*.3d%-*s%-*s%-*s%-*d%-*d%-*d%-*d\n", 
            stBuf, playerTeam[i].base->id, 
            nameBuf, playerTeam[i].base->name, 
            stBuf, TYPE_STRING[playerTeam[i].base->type1], 
            stBuf, TYPE_STRING[playerTeam[i].base->type2], 
            stBuf, playerTeam[i].base->hp, 
            stBuf, playerTeam[i].base->attack, 
            stBuf, playerTeam[i].base->defense, 
            endBuf, playerTeam[i].base->speed
            );
    }
}


int randomOpponent(Pokemon playerTeam[]){
    int i;
    int r;
    while (1){
        r = randint(AMOUNT_OF_POKEMON - 1) + 1;
        for (i = 0; i < TEAM_SIZE; i++){
            if (playerTeam[i].base != NULL && r == playerTeam[i].base->id){
                i = TEAM_SIZE + 1;
            }
        }
        if (i == TEAM_SIZE){
            return r;
        }
    }
}


int fight(Move moves[],Pokemon *pPlayer, Pokemon *pOpponent){

    Pokemon *pokemon[] = {pPlayer, pOpponent};
    bool current;

    if (pPlayer->base->speed == pOpponent->base->speed){
        current = randint(2);
    } else {
        current = pPlayer->base->speed > pOpponent->base->speed;
    }

    int j = 0;
    int damage;
    int moveId;
    putchar('\n');

    while (1) {
        moveId = pokemon[current]->moves[randint(4)];
        // fuck the not the same move used.
        damage = (int) (
            (( (double) pokemon[current]->base->attack 
                / (double) pokemon[!current]->base->defense) 
            / 6.0)
            + ((double) (2 + randint(19)))
            );
        if (damage < 1) {
            damage = 1;
        }
        pokemon[!current]->currentHp = fmax(0, pokemon[!current]->currentHp - damage);
        printf("%s valt aan met %s!\nDit doet %d schade!\n%s heeft nu %d hp over.\n", 
                pokemon[current]->base->name,
                moves[moveId].name,
                damage,
                pokemon[!current]->base->name,
                pokemon[!current]->currentHp
                );
        if (pokemon[!current]->currentHp == 0){
            printf("De winaar is %s!\n",
                pokemon[current]->base->name
            );
            return current;
        }
        putchar('\n');
        current = !current;
        j++;
    }

    return 0;
}