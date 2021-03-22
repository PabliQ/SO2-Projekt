#include <ncurses.h>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <pthread.h>
#include <random>
#include <time.h>


    int random_speed(){
        int speed;
        speed =  rand()%91 + 30;
        return speed;
    }

    char * random_char(){
        int rand_number;
        char * rand_char;
        rand_number = rand()%26;
        char c = 'a' + rand_number;
        rand_char = & c;
        return rand_char;
    }
    void sleeping(int time){
        std::this_thread::sleep_for(std::chrono::milliseconds(time));
    }

    void track_ride(int speed, WINDOW * track, char  * znak){
        for (int i = 0; i<3; i++){ // licznik okrążeń
            int x = 0;
            int y = 0;
            while(x<35){
                x++;
                mvwprintw(track, y, x, znak);
                wrefresh(track);
                sleeping(speed);
                mvwprintw(track, y, x, " ");
                wrefresh(track);
                sleeping(speed);  
            }
            refresh();
            while(y<15){
                y++;
                mvwprintw(track, y, x, znak);
                wrefresh(track);
                sleeping(speed);
                mvwprintw(track, y, x, " ");
                wrefresh(track);     
            }
            refresh();
            while(x>0){
                x--;
                mvwprintw(track, y, x, znak);
                wrefresh(track);
                sleeping(speed);
                mvwprintw(track, y, x, " ");
                wrefresh(track);  
            }
            refresh();
            while(y>0){
                y--;
                mvwprintw(track, y, x, znak);
                wrefresh(track);
                sleeping(speed);
                mvwprintw(track, y, x, " ");
                wrefresh(track);     
            }
            refresh();
        }
    }


int main()
{
    srand(time(NULL));
    initscr();
    int height, width, start_y, start_x;
    // zwnetrzna krawedz toru
    start_x = start_y = 0;
    height = 20; // wysokosc w znakach
    width = 40; // szerokosc w znakach 
    WINDOW * outer_bound = newwin(height, width, start_y, start_x); // okno
    refresh(); // odswiezenie okna
    box(outer_bound, 0, 0);// zrobienie obramowania
    wrefresh(outer_bound);//odswiezenia okna zeby byla ramka

    //trasa do przejazdu
    height = 16;
    width = 36;
    start_x = start_y = 2;
    WINDOW * track = newwin(height, width, start_y, start_x);
    refresh();
    box(track, 0, 0);
    wrefresh(track);

    //wewnetrzna krawedz toru
    /*height = 12;
    width = 32;
    start_x = start_y = 4;
    WINDOW * inner_bound = newwin(height, width, start_y, start_x);
    refresh();
    wrefresh(inner_bound);*/

    track_ride(random_speed(), track, random_char());    

    getch();
    endwin();
    return 0;
}