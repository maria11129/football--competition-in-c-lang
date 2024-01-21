#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h> 

typedef struct  {
    char name[27];
    int goals;   //Goal Difference
    int points;
    int win; 
    int draw; 
    int lose;
    char game; //number of matches played
} info;

void Entrance (int width);  
void Exit (int width);
void comments (info team[], int n, int b, int width); 

void PremierLeague (char* names, info team[], int n);
void SerieA (char* names, info team[], int n);
void Bundesliga (char* names, info team[], int n);
void LaLiga (char* names, info team[], int n);
void Multiple (char* names, info team[], int n);

void SortNames(info team[], int n); //will sort the teams by name
void DrawTable (info team[], int n);
void RoundRobin(info team[], int n, int RoundNumber, int q, int c); //organizes matches
void scores (info team[], int n, char flag[], int c);
void Sorting (info team[], int n, int RoundNumber);


void printGradually(const char *text, int delay) {
    while (*text != '\0') {
        putchar(*text++);
        fflush(stdout);
        struct timespec request; request.tv_sec = delay / 1000; request.tv_nsec = (delay % 1000) * 1000000; nanosleep(&request, NULL);
    }
}



int TerminalWidth() {
    int width;
    printf("\033[999C\033[6n");
    scanf("\033[%dR", &width);
    return width;
}



int main () { 
    
  int width = TerminalWidth();
    Entrance (width);


    int  i, j, q, c, d, end;


int n = 0; // initialize n to 0

do {
    printf("\n\n\nEnter the number of teams (10-20): ");
    if (scanf("%d", &n) != 1 || n < 10 || n > 20) {
        printf("Invalid input. Please enter a number between 10 and 20.\n");
        // Clear the input buffer
        while (getchar() != '\n');
    }
} while (n < 10 || n > 20);












    if (n % 2 == 0)  {
            q = 1;
            end = 2*(n-1); //last round
    }
    else {
        q = 0;
        end = 2*n; //last round
    } 
    

    info team[n];

    do {
        printf ("\n\nIf you want to build your league enter 1.\n%+7cIf you want to choose one enter 0: ", ' ');
        scanf ("%i", &c);
    } while (c!=1 && c!=0);
    
        
    if (c==0) {
        char* names = malloc (20 * sizeof(char) * 27); //27 size of teams's names, 20 number of teams
        
        if (names == NULL) {
            printf ("Something get wrong.");
            return 1;
        }

        do {
            printf ("\n\nFor PREMIER LEAGUE tap 1.\n%+4cFor BUNDESLIGA tap 2.\n%+7cFor SERIE A tap 3. \n%+7cFor LA LIGA tap 4.\nFor a multiple one tap 5: ", ' ', ' ', ' ');
            scanf ("%i", &d);
        } while (d<1 || d>5);
        
        switch (d) {
            case 1: PremierLeague(names, team, n); break;
            
            case 2: Bundesliga(names, team, n); break;
            
            case 3: SerieA(names, team, n); break;
            
            case 4: LaLiga(names, team, n); break;
            
            case 5: Multiple(names, team, n); break;
             
            default: break;
        }
        
        free (names);
    }    
        
    else {
        i = 0;
        while (i<n) {
            printf("Enter the name of team %i: ", i + 1);
            scanf(" %[^\n]s", team[i].name);

            i++;
        }
    }

    SortNames (team, n);


//Filling the array:
    i = 0;
    while (i<n) {
        team[i].points = 0;
        team[i].goals = 0;
        team[i].win = 0;
        team[i].draw = 0;
        team[i].lose = 0;
        team[i].game = 0;

        i++;
    }

    char flag [4];
    printf("\n\nAnd the last question:\nDo you want the score to be added randomly? Enter \033[1;35m'yes'\033[0m or \033[1;35m'no'\033[0m: ");
    scanf(" %[^\n]s", flag);
    
    i = 0;
    while (i<strlen(flag)) {
        flag[i] = tolower(flag[i]); 
        i++;
    }


//End of informations, Begin of the tourtment

    int RoundNumber = 1;
    while (RoundNumber <= end+1) {

        printf ("\n\n");
        sleep (2);
        DrawTable (team, n); 
        
        if (RoundNumber == (end/2)+1) {
            printf("\n\n%*s", (width-16)/2, ""); 
            printGradually ("\033[1;33mEnd of First Leg\033[0m", 100);
        }
        
        else if (RoundNumber == end+1) {
            printf("\n\n%*s", (width-17)/2, ""); 
            printGradually ("\033[1;33mEnd of Second Leg\033[0m", 100);
        }

        do {
            printf ("\n\n\033[1;96mPress enter to continue...\033[0m");
            getchar();
         } while (getchar() != '\n');

        system ("cls || clear"); 
        
        if (RoundNumber != end+1) { 
            
            if (RoundNumber==1) {
                printf("%*s\033[1;33mFirst Leg\033[0m\n", (width-9)/2, "");
                printf("%*s\033[1;33m---------\033[0m\n\n", (width-9)/2, "");
                c = 0;
            } 
            
            else if (RoundNumber==(end/2)+1) {
                printf("%*s\033[1;33mSecond Leg\033[0m\n", (width-10)/2, "");
                printf("%*s\033[1;33m----------\033[0m\n\n", (width-10)/2, "");
                c = 1;
            }      


            printf("%*s\033[9;96mROUND %d\033[0m\n\n", (width - 7) / 2, "", RoundNumber);

            RoundRobin(team, n, RoundNumber, q, c);
            scores (team, n, flag, c);
            Sorting (team, n, RoundNumber);

        }

        RoundNumber++;
    }

//End of tourtment:
    
    printf ("%*sAnd the WINNER IS...\n", (width-17)/2, ""); 
    
    i = 0;
    while (i < strlen(team[0].name)) {
        team[0].name[i] = toupper (team[0].name[i]);
        i++;
    }
        
    printf ("%*s", (width - strlen(team[0].name))/2, "");
    printGradually (team[0].name , 100);
    printf ("\n");

    
//End of code, jut some comments to make it funny    
    comments (team, n, d, width);
    
    do {
            printf ("\n\n\033[1;96mPress enter to continue...\033[0m");
            getchar();
         } while (getchar() != '\n');
    
    system ("cls || clear");     
         
    Exit (width); 
}







void Entrance (int width) { 

    
    printf("%*s", width-29, "");
    printGradually ("\033[1;04mHouari Boumediene University.\033[0m", 80); 
    sleep (0.95);
    
    
    printf ("\n%*s", width-26, "");
    printGradually ( "\033[1;04mFaculty of CS - Section A.\033[0m" , 80);
    sleep (0.95);
    
    printf ("\n%*s ", width-24, "");
    printGradually ( "\033[1;04mDr. Haouari Ahmed Taha.\033[0m", 80); 
    sleep (1.5);
    
    
    printf("\n\n\n%*s", (width-34)/2, "");
    printGradually ("\033[3;96mWelcome to the football tournament,\033[0m", 80);
   
    printf("\n%*s", (width-46)/2, "");
    printGradually ("\033[3;96mHope you have an enjoyable and exciting time! \033[0m", 80);

    sleep (2); 
    
    return;
}    
    


void PremierLeague(char* names, info team[], int n) {

    strcpy(names + 0 * 27, "Arsenal");
    strcpy(names + 1 * 27, "Liverpool");
    strcpy(names + 2 * 27, "Manchester City");
    strcpy(names + 3 * 27, "Brighton");
    strcpy(names + 4 * 27, "Manchester United");
    strcpy(names + 5 * 27, "Chelsea");
    strcpy(names + 6 * 27, "Newcastle United");
    strcpy(names + 7 * 27, "West Ham United");
    strcpy(names + 8 * 27, "Tottenham Hotspur");
    strcpy(names + 9 * 27, "Nottingham Forest");
    strcpy(names + 10 * 27, "Aston Villa");
    strcpy(names + 11 * 27, "Brentford");
    strcpy(names + 12 * 27, "Burnley");
    strcpy(names + 13 * 27, "Crystal Palace");
    strcpy(names + 14 * 27, "Fulham");
    strcpy(names + 15 * 27, "Bournemouth");
    strcpy(names + 16 * 27, "Luton Town");
    strcpy(names + 17 * 27, "Sheffield United");
    strcpy(names + 18 * 27, "Everton");
    strcpy(names + 19 * 27, "Wolverhampton");

    int i = 0;
    while (i < n) {
        strcpy(team[i].name, names + i * 27);
        i++;
    }
    return;
}



void SerieA(char* names, info team[], int n) {

    strcpy(names + 0 * 27, "Atalanta");
    strcpy(names + 1 * 27, "Bologna");
    strcpy(names + 2 * 27, "Fiorentina");
    strcpy(names + 3 * 27, "Inter");
    strcpy(names + 4 * 27, "Juventus");
    strcpy(names + 5 * 27, "Lazio");
    strcpy(names + 6 * 27, "Milan");
    strcpy(names + 7 * 27, "Napoli");
    strcpy(names + 8 * 27, "Roma");
    strcpy(names + 9 * 27, "Salernitana");
    strcpy(names + 10 * 27, "Lecce");
    strcpy(names + 11 * 27, "Sassuolo");
    strcpy(names + 12 * 27, "Monza");
    strcpy(names + 13 * 27, "Torino");
    strcpy(names + 14 * 27, "Udinese");
    strcpy(names + 15 * 27, "Empoli");
    strcpy(names + 16 * 27, "Cagliari");
    strcpy(names + 17 * 27, "Frosinone");
    strcpy(names + 18 * 27, "Genoa");
    strcpy(names + 19 * 27, "Hellas Verona");

    int i = 0;
    while (i < n) {
        strcpy(team[i].name, names + i * 27);
        i++;
    }

    return;
}


void Bundesliga(char* names, info team[], int n) {

    strcpy(names + 0 * 27, "Arminia Bielefeld");
    strcpy(names + 1 * 27, "Augsburg");
    strcpy(names + 2 * 27, "Bayer Leverkusen");
    strcpy(names + 3 * 27, "Bayern Munich");
    strcpy(names + 4 * 27, "Bochum");
    strcpy(names + 5 * 27, "Borussia Dortmund");
    strcpy(names + 6 * 27, "Borussia Monchengladbach");
    strcpy(names + 7 * 27, "Eintracht Frankfurt");
    strcpy(names + 8 * 27, "RB Leipzig");
    strcpy(names + 9 * 27, "Union Berlin");
    strcpy(names + 10 * 27, "FC Cologne");
    strcpy(names + 11 * 27, "SC Freiburg");
    strcpy(names + 12 * 27, "Hertha BSC");
    strcpy(names + 13 * 27, "Mainz");
    strcpy(names + 14 * 27, "Stuttgart");
    strcpy(names + 15 * 27, "VfL Wolfsburg");
    strcpy(names + 16 * 27, "Werder Bremen");
    strcpy(names + 17 * 27, "Greuther Furth");
    strcpy(names + 18 * 27, "Hoffenheim");
    strcpy(names + 19 * 27, "VfB Stuttgart");

    int i = 0;
    while (i < n) {
        strcpy(team[i].name, names + i * 27);
        i++;
    }
    return;
}


void LaLiga(char* names, info team[], int n) {

    strcpy(names + 0 * 27, "Alaves");
    strcpy(names + 1 * 27, "Athletic Bilbao");
    strcpy(names + 2 * 27, "Atletico Madrid");
    strcpy(names + 3 * 27, "Barcelona");
    strcpy(names + 4 * 27, "Osasuna");
    strcpy(names + 5 * 27, "Real Betis");
    strcpy(names + 6 * 27, "Real Madrid");
    strcpy(names + 7 * 27, "Real Sociedad");
    strcpy(names + 8 * 27, "Sevilla");
    strcpy(names + 9 * 27, "Valencia");
    strcpy(names + 10 * 27, "Cadiz");
    strcpy(names + 11 * 27, "Celta Vigo");
    strcpy(names + 12 * 27, "Las Palmas");
    strcpy(names + 13 * 27, "Almeria");
    strcpy(names + 14 * 27, "Getafe");
    strcpy(names + 15 * 27, "Granada");
    strcpy(names + 16 * 27, "Villarreal");
    strcpy(names + 17 * 27, "Girona");
    strcpy(names + 18 * 27, "Mallorca");
    strcpy(names + 19 * 27, "Rayo Vallecano");

    int i = 0;
    while (i < n) {
        strcpy(team[i].name, names + i * 27);
        i++;
    }
    return;
} 


void Multiple (char* names, info team[], int n) {

    strcpy(names + 0 * 27, "Real Madrid");
    strcpy(names + 1 * 27, "Barcelona");
    strcpy(names + 2 * 27, "Atletico Madrid");
    strcpy(names + 3 * 27, "Bayern Munich");
    strcpy(names + 4 * 27, "Borussia Dortmund");
    strcpy(names + 5 * 27, "Inter");
    strcpy(names + 6 * 27, "Juventus");
    strcpy(names + 7 * 27, "Manchester City");
    strcpy(names + 8 * 27, "Manchester United");
    strcpy(names + 9 * 27, "Milan");
    strcpy(names + 10 * 27, "Liverpool");
    strcpy(names + 11 * 27, "Arsenal");
    strcpy(names + 12 * 27, "Boca Juniors");
    strcpy(names + 13 * 27, "Santos");
    strcpy(names + 14 * 27, "USM Alger");
    strcpy(names + 15 * 27, "River Plate");
    strcpy(names + 16 * 27, "Sao Paulo");
    strcpy(names + 17 * 27, "Al Nassr");
    strcpy(names + 18 * 27, "Al Ahli");
    strcpy(names + 19 * 27, "Paris Saint Germain");


    int i = 0;
    while (i < n) {
        strcpy(team[i].name, names + i * 27);
        i++;
    }
    return;
} 



void SortNames(info team[], int n) {
    info swap;
    int i, j;
    
        do {
            j = 0;
            i = 0;
            while (i < n - 1) {
                if (strcmp(team[i].name, team[i + 1].name) > 0) {
                    swap = team[i];
                    team[i] = team[i + 1];
                    team[i + 1] = swap;
                
                    j = 1;
                }
                i++;
            }
            n--;
        } while (j==1);    
    
    return;
}



void DrawTable (info team[], int n) {

    int i, j;
    printf ("%-3c | Team %-25c | MP | Pts | GD | \033[1;32mW \033[0m | D  | \033[1;31mL \033[0m |\n", ' ', ' ');

    j = 0;
    while (j < 2) {
    
        printf ("____|________________________________|____|_____|____|____|____|____|");

        i = 0;
        if (j == 0) {
            while (i < 4) { 
                printf("\n\033[1;32m%-3d\033[0m | \033[1;32m%-30s\033[0m | \033[1;32m%-2d\033[0m | \033[1;32m%-3d\033[0m | \033[1;32m%-2d\033[0m | \033[1;32m%d\033[0m  | \033[1;32m%d\033[0m  | \033[1;32m%d\033[0m  |", i+1, team[i].name, team[i].game, team[i].points, team[i].goals, team[i].win, team[i].draw, team[i].lose);
                i++;
            }
            
            while (i < n - 3) {
                printf ("\n%-3d | %-30s | %-2d | %-3d | %-2d | %d  | %d  | %d  |", i+1, team[i].name, team[i].game, team[i].points, team[i].goals, team[i].win, team[i].draw, team[i].lose);
                i++;
            }
            
            while (i < n) {
                printf("\n\033[1;31m%-3d\033[0m | \033[1;31m%-30s\033[0m | \033[1;31m%-2d\033[0m | \033[1;31m%-3d\033[0m | \033[1;31m%-2d\033[0m | \033[1;31m%d\033[0m  | \033[1;31m%d\033[0m  | \033[1;31m%d\033[0m  |", i+1, team[i].name, team[i].game, team[i].points, team[i].goals, team[i].win, team[i].draw, team[i].lose);
                i++;
            }
        }
        printf ("\n");
        j++;
    }
    return;
}



void RoundRobin(info team[], int n, int RoundNumber, int q , int c) {

    SortNames (team, n);

    int i, w;
    info swap;

    w = 0;
    while (w < RoundNumber) {
        swap = team[n - 1];
        
        i = n - 1;
        while (i > q) {
            team[i] = team[i - 1];
            i--;
        }

        team[q] = swap;

        w++;
    }

    sleep (1);
    printf("\n%+23c\033[4;35mToday's Matches :\033[0m", ' ' );
    
    i = 0;
    while (i < n / 2) {
        sleep(1); 
        
        if (c == 0) {
            printf("\n%-30s VS %+30s \n", team[i].name, team[n - 1 - i].name);
        }
        
        else {
            printf("\n%-30s VS %+30s \n", team[n - 1 - i].name, team[i].name);
        } 
        //cause we have the first and second leg
        
        team[i].game++;
        team[n - 1 - i].game++;
        i++;
    } 

    return;
}



void scores (info team[], int n, char flag[], int c) {

    int i, j, a, b;
    srand (time(NULL));
    
    sleep (2);

    printf ("\n\n%+29c \033[4;35mScores:\033[0m\n", ' ');

    i = 0;
    while (i < n/2) {

        if (strcasecmp(flag, "yes") == 0)
        {
            //random score
            a = rand() % 6;
            b = rand() % 6;
        }
        else
        {
            // Manual score
            
            if (c == 0) {
                printf("Enter the score for %s vs %s: ", team[i].name, team[n - 1 - i].name);
                scanf("%d %d", &a, &b);
            }
            
            else {
                printf("Enter the score for %s vs %s: ", team[n - 1 - i].name, team[i].name);
                scanf("%d %d", &b, &a);
            }
        }
        
        
        sleep (1);

        team[i].goals += (a-b);
        team[n - 1 - i].goals += (b-a);

        // Update points
            if ( a > b)
            {
                team[i].points += 3; 
                team[i].win ++;
                team[n - 1 -i].lose ++; 
                
                if (c == 0) {
                    printf("\n\n\033[1;32m%-27s\033[0m | \033[1;32m%i\033[0m  -  \033[1;31m%i\033[0m | \033[1;31m%+27s\033[0m ", team[i].name, a, b, team[n - 1 - i].name);
                } 
                
                else {
                    printf("\n\n\033[1;31m%+27s\033[0m | \033[1;31m%i\033[0m  -  \033[1;32m%i\033[0m | \033[1;32m%-27s\033[0m ", team[n - 1 - i].name, b, a, team[i].name);
                }
            }
            
            else if (a < b)
            {
                team[n - 1 - i].points += 3;
                team[n - 1 -i].win ++;
                team[i].lose ++;
                
                if (c == 0) {
                    printf("\n\n\033[1;31m%-27s\033[0m | \033[1;31m%i\033[0m  -  \033[1;32m%i\033[0m | \033[1;32m%+27s\033[0m", team[i].name, a, b, team[n - 1 - i].name);
                }
                
                else {
                    printf("\n\n\n\n\033[1;32m%-27s\033[0m | \033[1;32m%i\033[0m  -  \033[1;31m%i\033[0m | \033[1;31m%+27s\033[0m", team[n - 1 - i].name, b, a, team[i].name);
                }
            }
            
            else
            {
                team[i].points += 1;
                team[n - 1 - i].points += 1;
                team[i].draw ++;
                team[n - 1 -i].draw ++;
                
                if (c == 0) {
                    printf("\n\n%-27s | %i  -  %i | %+27s ", team[i].name, a, b, team[n - 1 - i].name);
                }
                
                else {
                    printf("\n\n%-27s | %i  -  %i | %+27s ", team[n - 1 - i].name, b, a, team[i].name);
                }
            }
            
            sleep (1);

            i++;
    }

    return;
}



void Sorting (info team[], int n, int RoundNumber) {

    int i, j, k;
    info swap;

    do {
        i = 0;
        j = 0;
        while ( j < n-1 )
        {
            if (team[j].points < team[j+1].points || (team[j].points == team[j+1].points && team[j].goals < team[j+1].goals) )
            {
                swap = team[j];
                team[j] = team[j + 1];
                team[j + 1] = swap;

                i = 1;
            }
            j++;
        }
        n--;
        k++;

    } while (i==1);

    n += k;

    return;

} 



void comments (info team[], int n, int b, int width) {

    int i, a; 
    
    if (strcmp(team[0].name, "JUVENTUS") == 0 ) {
    
        printf ("%*sForza Juve!\n\n", (width - 11)/2, "");
        
        i = 1;
        do {
            if (b==3 || strcmp(team[i].name, "Inter") == 0) {
                printf ("•Unidentified sources tweet: Inter communicates with the prosecutor, and the reason is entirely unknown.\n");
                a = 6;
                sleep (1);
            }
            i++;
        } while (i < n && a!=6);
        
        printf ("•Fabrizio Romano tweets: JUVENTUS TO THE SERIE C!\n");
    }     
    
    else if (strcmp(team[0].name, "BAYERN MUNICH") == 0 ) {
    
        printf ("%*s\033[1;31mMia San Mia!\033[0m\n\n", (width - 12)/2, "");
        
        if (b==2) { 
            printf ("•Farmers' League.\n");
        } 
    }
    
    else if (strcmp(team[0].name, "MANCHESTER UNITED") == 0 ) {
        
        printf ("%*s\033[1;31mGLORY GLORY MAN UNITED!\033[0m", (width - 23)/2, "");
        printf ("\n\n•How long have we waited!\n");
        printf ("•Sir Alex is this you?\n");
    }    
    
    else if (strcmp(team[0].name, "MILAN") == 0 ) {
    
        printf ("%*s \033[1;31mFORZA MILAN!\033[0m \n\n ", (width - 12)/2, "");
        printf ("•Renewing Pioli's contract until 2050.\n");
    }    
    
    else if (strcmp(team[0].name, "INTER") == 0 ) {

        printf ("%*s\033[1;94mforza inter\033[0m\n\n", (width - 11)/2, "");
        printf ("•Lizards, FORZA MILAN!\n");
    }    
    
    else if (strcmp(team[0].name, "MANCHESTER CITY") == 0) {
    
        printf ("%*s\033[1;96mCome on CITY!\033[0m\n\n", (width - 13)/2, "");
        printf ("•League of Money, Guardiola spends two billion. \n");
    }    
    
    else if (strcmp(team[0].name, "BARCELONA") == 0 ) {
        
        printf ("%*s\033[1;94mVisca\033[0m \033[1;31mBarça!\033[0m\n\n", (width - 12)/2, "");
        printf ("•Barcelona to the Champions League, and Abssi intensifies prayers to avoid playing against Bayern.\n");
    }    
    
    else if (strcmp(team[0].name, "REAL BETIS") == 0 ) {
        
        printf ("•The winner of the prestigious Galactic Cup, Betis, gracefully steps down, showcasing their abilities against the youngsters.\n");
    }    
    
    else if (strcmp(team[0].name, "REAL MADRID") == 0 ) {
        
        printf ("%*s\033[1;01mHALA MADRID!\033[0m\n\n", (width - 12)/2, "");
        printf ("•Hamdoun starts the praise.\n");
    }    
    
    else if (strcmp(team[0].name, "BORUSSIA DORTMUND") == 0 )  {
    
        printf ("%*s\033[1;33mECHTE LIEBE\033[0m\n\n", (width - 11)/2, "");
        printf ("•Reus can die in pease\n"); 
    }   
    
    else if (strcmp(team[0].name, "TOTTENHAM HOTSPUR") == 0 ) {
        
        printf ("%*s\033[1;01mCome On Spurs!\033[0m\n\n", (width - 14)/2, "");
        printf ("•Tottenham secures a champion! miracle! \n");
    }   
    
    else if (strcmp(team[0].name, "SEVILLA") == 0 ) {
        
        printf ("•Is that the europa league?\n");
    }   
    
    else if (strcmp(team[0].name, "PARIS SAN GERMAN") == 0) {
        
        printf ("%*s\033[1;94mIci c'est paris\033[0m", (width - 15)/2, "");
        printf ("•0 champions league.\n");
    }   
    
    else if (strcmp(team[0].name, "ARSENAL") == 0 ) {
     
        printf ("%*s\033[1;31mCome On You Gunners!\033[0m\n\n", (width - 19)/2, "");
        printf ("•Finally!\n");
    }
    
    else if ( strcmp(team[0].name, "LIVERPOOL") == 0 ) {
    
        printf ("%*s\033[1;31mYou'll Never Walk Alone!\033[1;33m\n\n", (width - 24)/2, "");
    }
    
    else if ( strcmp(team[0].name, "BOKA JUNIORS") == 0  ) {
        
        printf ("%*sVAMOS BOKA!\n\n", (width - 11)/2, "");
    }
    
    else if ( strcmp(team[0].name, "SANTOS") == 0 ) {    
    
        printf ("%*s\033[1;01mVai Pra Cima, SANTOS!\033[0m\n\n", (width - 21)/2, "");
    }
    
    else if ( strcmp(team[0].name, "RIVER PLATE") == 0 ) { 
    
        printf ("%*sVamos Vamos, RIVER PLATE!\n\n", (width - 25)/2, "");
    }    
    
    else if ( strcmp(team[0].name, "SAO PAULO") == 0 ) {
    
        printf ("%*sVamos, SAO PAULO!\n\n", (width - 17)/2, "");
    } 
    
    else if ( strcmp(team[0].name, "Atletico Madrid") == 0 ) {
    
        printf ("%*s\033[1;94mAupa\033[0m \033[1;31mAtleti!\033[1;33m\n\n", (width - 12)/2, "");
    }
    
    else if (b > 1 && b < 5) {
        
        printf ("•Now it's confirmed that it's a fabricated league. \n");
    }    
    
    
    
    i = 1;
    while (i < n) {
    
        if (strcmp(team[i].name, "Bayern Munich") == 0) {
            printf ("•Bayern Munich tweets: Barcelona will pay the price for this defeat.\n");
        }
        
        if (strcmp(team[i].name, "Real Madrid") == 0) {
            printf ("•Hamdoun comments: Anyway, the crucial thing is the Champions League \n");
        }
        
        if (strcmp(team[i].name, "Barcelona") == 0) {
            printf ("•Abssi on social media: Messi has 8 Ballons d'Or.\n");
        } 
        
        if (strcmp(team[i].name, "Milan") == 0) {
            printf ("•Reliable sources confirm: The administration decides to give Pioli a final chance (for the 1234th time) \n");
        } 
        
        if (strcmp(team[i].name, "Arsenal") == 0) {
            printf ("•As usual, Arsenal without a league title.\n");
        }
        
        if (strcmp(team[i].name, "Tottenham Hotspur") == 0) {
            printf ("•As usual, Tottenham without a championship\n");
        } 
        
        else if (strcmp(team[i].name, "PARIS SAN GERMAN") == 0) {

        printf ("•Al-Khalifi reaps the results of depriving people of enjoyment. \n ");
    }   
    
    
        i++;
    }    
    
    return;
}    



void Exit (int width) {

    printf ("%*s", (width - 32)/2, "");
    printGradually ("\033[3;96mSo, The tournament concludes here.\033[0m" , 80);
    sleep (0.95); 
    
    printf ("\n%*s", (width - 87)/2, ""); 
    printGradually ("\033[3;96mWe hope you've had an enjoyable time (and that the teams you support emerged victorious).\033[0m" , 80);
    sleep (0.95);
    
    printf("\n%*s", (width - 16)/2, "");
    printGradually ("\033[3;96mUntil next time!\033[0m", 80); 
    
    return;
}    