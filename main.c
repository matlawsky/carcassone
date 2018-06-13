#define  RND_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>
#include <ctype.h>
#include <time.h> // for time
#include "rnd.h"


#define cellSize 6
#define tileNumber 14
#define ilosc 7




// [] [] [] [] [] [] [] []

//ListHeader ---> &[val] *alloca*
//  prev //--- [val] ---> next || prev <--- [val] ---> next || prev <--- [val] ---> next || prev <--- [val]  ---> next || prev <--- [val]  ---// next 
// --// NULL ptr
// insert                 (ListHeader, val)
// delete by name / by id (ListHeader, id) (ListHeader, name)

// Definicja struktur
	
extern int errno;
 // example https://github.com/wutproggroup/carcassonne
struct tile{
    int id;
    char* name;
    int isTemple;   //if possible boolean
    int crossRoads; //if possible boolean
    int bonus;      //if possible boolean
    int top;        //for all 4 directions what's on the tile 0-nothing, 1-road, 2-city
    int right;
    int bottom;
    int left;
}; //poprawić strukturę tak żeby można było rozszerzać tablicę

typedef struct tile tile;

struct tile_placed
{
    int id;
    int rotation;
};

typedef struct tile_placed tile_placed;
// problem z rozszerzaniem - rozszerza się zawsze w dół... ;/

// defiinicja funkcji tablica 2d

void free2Darray(tile_placed** p, int N);
void print(tile_placed** p, int N);
void fill_with_zeros(tile_placed** p, int N);
void fill_with_nothing(tile* tl, int N);
void fill_available(tile* avtl, tile* tl, int rozmiar);

void fill_from_file(tile_placed** p, char* filename, int N, int powiekszone);

//void getNoReturn(tile_placed*** table, int N);
int **get_2d(int N);

// definicja funkcji tablica 1d
int *get_1d(int N);

//ai


// graphics
void welcomeScreen (tile* tl, int N);
void draw_grid(tile_placed** map, int N);
void available_tiles(tile* tl, int H, int N);

// text file handling
void errors(char* m, char* t);
int countlines(char *filename);
void read_tiles(tile* tl, char* filename, int N);

// text file output
void print_to_file(tile_placed** p, char* filename, int N);

//intraction with user
void check_for_action(tile_placed**p, tile* tl, int N);
void menu();


int main(int argc, char *argv[])
{
    // pobierane argumenty przy wywo³ywaniu programu
    // char *folder = argv[1];
    char *tilesy = argv[1];
    char *mapa = argv[2];

    //char* mapa = "output.list";
    //char* tilesy = "tiles.list";
    //char* output = "output.list";

    // structs entities
    tile *tl;
    tile *avtl;
    tile_placed **p;
    tile_placed **pn;

    errors(mapa, tilesy);
    if(countlines(tilesy) < tileNumber-1)
    {
        printf("Some tiles description are missing!");
    }


    int rozmiar = countlines(mapa);
    int new_rozmiar = 0;

    p = get_2d(rozmiar);

    fill_with_zeros(p , rozmiar);

    fill_from_file(p, mapa, rozmiar, check_the_edge(p, rozmiar));

    //tl = get_1d(tileNumber);
    if(check_the_edge(p, rozmiar)!=0)
    {

        pn = get_2d(rozmiar+2);
        fill_with_zeros(pn , rozmiar+2);
        fill_from_file(pn, mapa, rozmiar+2, check_the_edge(p, rozmiar));

    }
    else
    {
        pn = p;
    }

    // liczy sie wlasciwa alokacja pamieci i przepisanie wartosci ze


    tl = get_1d(tileNumber);
    avtl = get_1d(ilosc);

    //fill_with_nothing(tl, tileNumber);


    read_tiles(tl,tilesy,(countlines(tilesy)+1));
    fill_available(tl, avtl, ilosc);

    welcomeScreen (tl, tileNumber);

    //print(p, rozmiar);

    draw_grid(pn, rozmiar+2);
    //draw_available(avtl, ilosc);
    check_for_action(pn, tl, rozmiar+2);
    draw_grid(pn, rozmiar+2);
    
    print_to_file( pn, mapa, rozmiar+2);

    free2Darray(pn , rozmiar+2);
    free2Darray(p , rozmiar);
    free(tl);

    return 0;
}

int check_the_edge(tile_placed** p, int N)
{
    int f = 0;
    for(int i = 0 ; i < N ; i++)
    {
        for(int j = 0 ; j < N ; j++)
        {

            if(p[0][j].id!=0)
            {
                //printf(map,",");
                f = 1;
            }
            else if(p[i][0].id!=0)
            {
                //printf(map,",");
                f = 1;
            }
            else if(p[N-1][j].id!=0)
            {
                //printf(map,",");
                f = 1;
            }
            else if(p[i][N-1].id!=0)
            {
                //printf(map,",");
                f = 1;
            }
            }

        }
        return f;
}

void read_tiles(tile* tl, char* filename, int N)
{
    FILE *tilies;
    char *mode = "r";
    int i = 0;
    char* buffer;
    char* sentence;
    tilies = fopen(filename, mode);
    buffer = malloc(300 * sizeof(char));
    sentence = malloc(300 * sizeof(char) + 8 * sizeof(int) +9 * sizeof(char));
    while( fgets( sentence, 256, tilies) != NULL )
    {
                //fgets( sentence, 256, tiles);
                //fscanf(tiles, "%[^\n]", sentence);
                sscanf(sentence,"%d,%[^,],%d,%d,%d,%d,%d,%d,%d;",
                       &tl[i].id,
                       buffer,
                       &tl[i].isTemple,
                       &tl[i].crossRoads,
                       &tl[i].bonus,
                       &tl[i].top,
                       &tl[i].right,
                       &tl[i].bottom,
                       &tl[i].left);
                tl[i].name = buffer;
                printf("%d,%s,%d,%d,%d,%d,%d,%d,%d;\n", tl[i].id, tl[i].name, tl[i].isTemple, tl[i].crossRoads, tl[i].bonus,  tl[i].top, tl[i].right, tl[i].bottom, tl[i].left);
                memset(buffer, 0, sizeof(buffer));
                i++;
                if(i == N)
                {
                    break;
                }
        }
    free(buffer);
    free(sentence);
    fclose(tilies);
}

void print_to_file(tile_placed** p, char* filename, int N)
{
    FILE *map;
    char *mode = "w";
    int i, j;
    map = fopen(filename, mode);

    for(i = 0 ; i < N ; i++)
    {
        for(j = 0 ; j < N ; j++)
        {
            if(p[j][i].id != 0)
                   {
                      fprintf(map,"%d_%d",p[j][i].id,p[j][i].rotation);
                      if(j+1!=N)
                      {
                            fprintf(map,",");
                      }
                   }
            else
            {
                fprintf(map,"");
                 if(j+1!=N)
                      {
                            fprintf(map,",");
                      }
            }
  //          else if(p[i][j].id == 0)
 //                   fprintf(map,"%c", "");


        }
        if(j+1!=N)
        {
            fprintf(map,"\n");
        }

    }
    fclose(map);
}

void draw_grid(tile_placed** map, int grid_size)
{


    int total = grid_size * cellSize;
    printf("%*c",2,' ');

    // Drukowanie na ekran pierwszego wiersza
    for(int i = 0; i<=total; i++)
    {
                if((grid_size) >= 0 && (grid_size) < 10 )
                {
                    if((i%(cellSize/2) == 0) && (i%2 != 0) )
                    {
                        printf("%d", i/cellSize);
                    }
                    else
                    {
                        printf("%c",' ');
                    }
                }
                else if((grid_size) >= 10 && (grid_size) < 100 )
                {

                    if((i/cellSize) >= 10)
                    {
                        if(((i+1)%(cellSize/2) == 0) && ((i+1)%2 != 0) )
                        {
                            printf("%d", i/cellSize);
                            i++;
                        }
                        else
                        {
                            printf("%c",' ');
                        }
                    }
                    else
                    {
                        if(((i)%(cellSize/2) == 0) && ((i)%2 != 0) )
                        {
                            printf("%d", i/cellSize);
                            //i++;
                        }
                        else
                        {
                            printf("%c",' ');
                        }
                    }
                }
    }

    printf("\n");

    // Drukowanie pozostalych wierszy
    for(int i = 0; i<=total; i++)
    {
        for(int j = 0; j<=total+1; j++)
        {
            // j to kolumna
            // i to wiersz
            if(j == 0)
            {
                if((i/cellSize) >= 0 && (i/cellSize) < 10 )
                {
                    if(i%(cellSize/2) == 0 && ((i/(cellSize/2))%2 != 0))
                    {
                        printf("%d ", i/cellSize);
                    }
                    else
                    {
                        printf("%c ",' ');
                    }
                }
                else if((i/cellSize) >= 10 && (i/cellSize) < 100 )
                {
                    if(i%(cellSize/2) == 0 && ((i/(cellSize/2))%2 != 0))
                    {
                        printf("%d", i/cellSize);
                    }
                    else
                    {
                        printf("%*c",2,' ');
                    }
                }
            }
            else
            {
                if(((j-1)%(cellSize/2) == 0) && ((j-1)%2 != 0) && ((i)%(cellSize/2) == 0) && ((i)%2 != 0))
                    printf("%d", map[j/cellSize][i/cellSize]);

                else if(i % cellSize == 0 || j % cellSize == 1)
                    printf("%c",'*');
                else
                {
                    if(((j+1)%(cellSize/2) == 0) && ((j+1)%2 != 0) && ((i)%(cellSize/2) == 0) && ((i)%2 != 0) && (map[j/cellSize][i/cellSize].id>=10))
                    {
                        printf("");
                    }
                    else
                    {
                        printf("%c",' ');
                    }
                }
            }
        }

        printf("\n");
    }
}

// Ta funkcja na potem
void welcomeScreen (tile* tl, int N)
{
    printf ("XXXXX   XXXX    XXXX     XXXXX   XXXX   XXXXX  XXXXX  XXXXXXX  XX   XX  XX   XX  XXXXXX\n");
	printf ("XX     XX  XX   XX  XX   XX     XX  XX XX     XX      XX   XX  XXX  XX  XXX  XX  XX\n");
	printf ("XX     XX  XX   XXXX     XX     XX  XX  XXXX   XXXX   XX   XX  XX X XX  XX X XX  XXXXXX\n");
	printf ("XX     XXXXXX   XX  XX   XX     XXXXXX     XX     XX  XX   XX  XX  XXX  XX  XXX  XX\n");
	printf ("XXXXX  XX  XX   XX   XX  XXXXX  XX  XX XXXXX  XXXXX   XXXXXXX  XX   XX  XX   XX  XXXXXX\n");
	printf ("\n\n");
	printf ("RULES OF THE GAME:\n");
	printf ("First you get few tiles available for you to use.\n");
	printf ("Your task is to choose which one you want to place,\n");
	printf ("where you want to place it and choose whether you want to rotate them by angle equal to:\n");
	printf ("(0) 0   degrees\n");
	printf ("(1) 90  degrees\n");
	printf ("(2) 180 degrees\n");
	printf ("(3) 270 degrees\n");
	printf ("There are 14 types of tiles to be placed on the map.\n");
	printf ("Game ends when you and your enemy will run out of tiles,\n");
	printf ("then points will be counted and player who gets more points wins.\n");
	printf ("List of the tiles. (below is the information what does the number below imply)\n");
	printf ("id   name   isTemple   crossRoads   bonus   top   right   bottom   left\n");
#if defined LINUX
	getchar();
#elif defined WINDOWS
	getch();
#elif defined MACOSX
	char z;
	scanf("%c",z);
#endif
	for(int i =0; i<N ; i++)
    {
            printf("%d,%s,%d,%d,%d,%d,%d,%d,%d;\n", tl[i].id, tl[i].name, tl[i].isTemple, tl[i].crossRoads, tl[i].bonus,  tl[i].top, tl[i].right, tl[i].bottom, tl[i].left);
    }

}

// zwracaja pointer
int **get_2d(int N)
{
    /* Check if allocation succeeded. (check for NULL pointer) */
    tile_placed **table;
    table = malloc(N*sizeof(tile_placed *));
    for(int i = 0 ; i < N ; i++)
        table[i] = malloc( N*sizeof(tile_placed) );
    return table;
}

int *get_1d(int N)
{
    /* Check if allocation succeeded. (check for NULL pointer) */
    char *tl = malloc(N*N*sizeof(tile *));
    if(!tl)
        return NULL;
    return tl;
}

// wypelnij mape zerami

void fill_with_nothing(tile* tl, int N)
{
    int i;
    for(i = 0 ; i < N ; i++)
    {
            printf("Start");
            tl[i].id = 0;
            tl[i].name = " ";
            tl[i].isTemple = 0;
            tl[i].crossRoads = 0;
            tl[i].bonus = 0;
            tl[i].top = 0;
            tl[i].right = 0;
            tl[i].bottom = 0;
            tl[i].left = 0;

    }
    printf("Koniec");
}

void fill_with_zeros(tile_placed** p, int N)
{
    int i, j;
    for(i = 0 ; i < N ; i++)
    {
        for(j = 0; j < N; j++)
        {
            p[i][j].id = 0;
            p[i][j].rotation = 0;
        }
    }
}

void fill_from_file(tile_placed** p, char* filename, int N, int powiekszone) //powiekszone, bo tu muszze zmienić x i y przy obrocie na numer komórki w nowej powiekszonej tablicy
{
    FILE *map;
    char *mode = "r";
    char c;
    int count;
    char *array;
    array = malloc((N + N) * (N + N) * 3 * sizeof(char));
    map = fopen(filename, mode);
    int i = 0;
    int j = 0;
    int x, ograniczenie;
    int z=0, y=0, f=0;
    do {
         c = getc(map);
         array[i] = c;
         i++;
    }
    while(c != EOF);
    printf("\n");

    if(powiekszone==0)
    {
        i = 0;
        j =0;
        ograniczenie = strlen(array);
    }
    else
    {
        i = 1;
        j =1;
        ograniczenie = strlen(array) -1;
    }
    for(x=0 ; x < strlen(array); x++)
    {
        if((array[x] == '1') || (array[x] == '2') ||(array[x] == '3') ||(array[x] == '4')  || (array[x] == '5') || (array[x] == '6') ||(array[x] == '7')  || (array[x] == '8') || (array[x] == '9'))
        {

            if(array[x+1] == '_')
            {
                z = array[x] - '0';
                y = array[x+2] - '0';
                p[i][j].id = z;
                p[i][j].rotation = y;

                i++;
            }

            else if((array[x+1] == '0') ||(array[x+1] == '1') || (array[x+1] == '2') ||(array[x+1] == '3') ||(array[x+1] == '4')  || (array[x+1] == '5') || (array[x+1] == '6') ||(array[x+1] == '7')  || (array[x+1] == '8') || (array[x+1] == '9'))
            {
                z = array[x] - '0';
                f = array[x+1] - '0';
                y = array[x+3] - '0';

                p[i][j].id = (z * 10 + f);
                p[i][j].rotation = (y);
                i++;

            }
        }
        else if((array[x] == ',')&&(array[x+1] == ','))
        {
                p[i][j].id = 0;
                p[i][j].rotation = 0;
                i++;
        }
        else if(array[x] == '\n')
        {
            j++;
            i = powiekszone;
            count--;
            if(count == 0)
            {
                break;
            }
        }
    }
    printf("\n");
    free(array);
    fclose(map);
}

void print(tile_placed** p, int N)
{
    int i, j;
    for(i = 0 ; i < N ; i++)
        for(j = 0 ; j < N ; j++)
            printf("map[%d][%d] => %d => %d\n", i, j, p[i][j].id, p[i][j].rotation);
}

void free2Darray(tile_placed** p, int N)
{
    int i;
    for(i = 0 ; i < N ; i++)
        free(p[i]);
    free(p);
}

int countlines(char *filename)
{
  // count the number of lines in the file called filename
  char *mode = "r";
  FILE *fp = fopen(filename,mode);
  int ch=0;
  int lines=0;

  if (fp == NULL)
  {
      return 0;
  }

  while(!feof(fp))
{
  ch = fgetc(fp);
  if(ch == '\n')
  {
    lines++;
  }
}
  fclose(fp);
  return lines;
}

void errors(char* m, char* t)
{
    // Przerobiæ to ¿eby dzia³a³o na pojedynczy plik i sprawdza³o czy pojawi³ siê bl¹d
   FILE *tiles;
   FILE *map;
   char *mode = "r";
   int errnum_t, errnum_m;
   tiles = fopen(t, mode);
   map = fopen(m, mode);

   if (tiles == NULL)
   {
       errnum_t = errno;
       fprintf(stderr, "Error opening file: %s\n", strerror( errnum_t ));
       //return 1;
   }

   if (map == NULL)
   {
       errnum_m = errno;
       fprintf(stderr, "Error opening file: %s\n", strerror( errnum_m ));
       //return 2;
   }
   fclose(tiles);
   fclose(map);
}


void check_for_action(tile_placed**p, tile* tl, int N)
{

    int x , y, id, rotation;
    int g = 0;
    printf("Your turn. Choose number of tile you want to place:\n");


    while(g == 0)
    {
        scanf("%d", &id);
        if(((id == 1 )||(id == 2 )||(id == 3 )||(id == 4 )||(id == 5 )
            ||(id == 6 )||(id == 7 )||(id == 8 )||(id == 9 )||(id == 10 ) //&& avtl[for(int i = 0;i<ilosc;i++)]
            ||(id == 11 )||(id == 12 )||(id == 13 )||(id == 14 )) ) //tu dodać warunek czy zawiera się w available tiles
        {
            printf("Now choose where you want to place it by entering number of field using y axis:\n");
            while(g == 0)
            {
                scanf("%d", &y);
                if(y >= 0 && y < N)
                {
                    printf("Now choose where you want to place it by entering number of field using x axis:\n");
                    while(g == 0)
                    {
                       scanf("%d", &x);
                       if(x >= 0 && x < N)
                       {
                           printf("Now choose what angle you want to rotate the tile:\n");
                           while(g == 0)
                           {
                               scanf("%d", &rotation);
                               if(rotation>=0 && rotation<=4)
                               {

                                   p[x][y].id = id;
                                   p[x][y].rotation = rotation;
                                   g =1;
                               }
                           }
                       }
                       else
                       {
                           printf("There is no such x index.\n");
                       }
                    }


                }
                else
                {
                    printf("There is no such y index.\n");
                }

            }
        }
        else
        {
            printf("There is no such tile. Choose number of tile you want to place:\n");
        }
    }

}

void fill_available(tile* avtl, tile* tl, int rozmiar)
{
    rnd_pcg_t pcg;
	rnd_pcg_seed( &pcg, 0u ); // initialize generator
    time_t seconds;
	time( &seconds );
	rnd_pcg_seed( &pcg, (RND_U32) seconds ); 
    for( int i = 0; i < rozmiar; i++ ) 
	{
        RND_U32 n = rnd_pcg_next( &pcg );
		int r = rnd_pcg_range( &pcg, 1, 6 );
		printf( "%d, ", r );
	}
}

//void menu()
//{
//    char choice;
//    begin:
//    printf("MENU\n");
//    printf("1. START PVP GAME");
//    printf("2. START PVAI GAME");
//    printf("3. CONTROLS");
//    printf("4. EXIT");
//    error1:
//    choice=getchar();
//    switch(choice)
//    {
//	case '1' : pvp();
//		   return;
//	case '2' : pvai();
//		   goto begin;
//    case '3' : controls();
//	       goto begin;
//	case '4' : closegraph();
//		   exit(0);
//	default  : goto error1;
//    }
//}
//niedokonczona zmiana funkcji ani kopiowanie ich

// That function would only work on c11
//int dlugosc_linii(char* string)
//{
//   int n;
//   mbstate_t t;
//   char *scopy = string;
//   memset (&t, '\0', sizeof (t));
//   n = mbsrtowcs (NULL, &scopy, strlen (scopy), &t);
//
//   return n;
//}


// We don't return the pointer
//void getNoReturn(int*** table, int N)
//{
//    /* Check if allocation succeeded. (check for NULL pointer) */
//    int i;
//    *table = malloc(N*sizeof(int *));
//    for(i = 0 ; i < N ; i++)
//        (*table)[i] = malloc( N*sizeof(int) );
//}

//int liczba_linii(FILE* ifp)
//{
//   int rozmiar;
//   char *przecinki;
//
//   fscanf(ifp,"%s", przecinki);
//
//   char *ps = przecinki;
//   while(*ps != '\0')
//   {
//       if(*ps == ',')
//       {
//           rozmiar = rozmiar + 1;
//       }
//       ps++;
//   }
//   return rozmiar;
//}
