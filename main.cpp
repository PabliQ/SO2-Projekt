#include <ncurses.h>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <pthread.h>
#include <random>
#include <time.h>
#include <iostream>


    int random_speed(){
        int speed;
        speed =  rand()%91 + 30;
        return speed;
    }

    char  random_char(){
        char rand_char = "abcdefghijklmnoqprstuvwxyz"[rand()%26];
        char * c;
        c = & rand_char;
        return rand_char;
    }

    void sleeping(int time){
        std::this_thread::sleep_for(std::chrono::milliseconds(time));
    }

    void moving(int x, int y, int speed, char  znak)
    {
        move(2+y, 2+x);
        printw("%c", znak);
        refresh();
        sleeping(speed);                
        move(2+y, 2+x);
        printw("%c", ' ');
        //sleeping(speed);
    }

    void track_ride(){
        char rand_char = random_char();
        for (int i = 0; i<3; i++){ // licznik okrążeń
            int x = 0;
            int y = 0;
            while(x<35){
                x++;
                moving( x, y, random_speed(), rand_char);
            }
            while(y<15){
                y++;
                moving(x, y, random_speed(), rand_char); 
            }
            while(x>0){
                x--;
                moving(x, y, random_speed(), rand_char);
            }
            while(y>0 && i<2){
                y--;
                moving(x, y, random_speed(), rand_char);    
            }
        }
    }


int main()
{
    srand(time(NULL));
    initscr();
    curs_set(0);
    int height, width, start_y, start_x;
    // zewnetrzna krawedz toru
    start_x = start_y = 0;
    height = 20; // wysokosc w znakach
    width = 40; // szerokosc w znakach 
    WINDOW * outer_bound = newwin(height, width, start_y, start_x); // okno
    refresh(); // odswiezenie okna
    box(outer_bound, 0, 0);// zrobienie obramowania
    wrefresh(outer_bound);//odswiezenia okna zeby byla ramka
    //wewnetrzna krawedz toru
    height = 12;
    width = 32;
    start_x = start_y = 4;
    WINDOW * inner_bound = newwin(height, width, start_y, start_x);
    refresh();
    box(inner_bound, 0, 0);
    wrefresh(inner_bound);
    track_ride();
    getch();
    endwin();
    return 0;
}