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
        speed =  rand()%101 + 100;
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
    }

    void * track_ride(void * arg){
        sleeping(rand()%3000);
        char rand_char = random_char();
        for (int i = 0; i<3; i++){ // licznik okrążeń
            int x = 0;
            int y = 0;
            int speed = random_speed();
            float new_speed;
            new_speed = 0.6 * (float) speed;
            speed = (int) new_speed;
            while(x<35){
                x++;
                moving( x, y, speed, rand_char);
            }
            new_speed = 1.2 * (float) speed;
            speed = (int) new_speed;
            while(y<15){
                y++;
                moving(x, y, speed, rand_char); 
            }
            new_speed = 0.6 * (float) speed;
            speed = (int) new_speed;
            while(x>0){
                x--;
                moving(x, y, speed, rand_char);
            }
            new_speed = 1.2 * (float) speed;
            speed = (int) new_speed;
            while(y>0 && i<2){
                y--;
                moving(x, y, speed, rand_char);    
            }
        }
        return NULL;
    }

    void draw_track(int width, int height, int start_x, int start_y){ 
    WINDOW * outer_bound = newwin(height, width, start_y, start_x); // okno
    refresh(); // odswiezenie okna
    box(outer_bound, 0, 0);// zrobienie obramowania
    wrefresh(outer_bound);//odswiezenia okna zeby byla ramka
    }

int main()
{
    srand(time(NULL));
    initscr();
    curs_set(0); // usuniecie kursora
    // zewnetrzna krawedz toru
    draw_track(40, 20, 0 ,0);
    //wewnetrzna krawedz toru
    draw_track(32,12,4,4);

    //watki
    pthread_t first, second, third;
    pthread_create(&first, NULL, track_ride,NULL);
    pthread_create(&second, NULL, track_ride,NULL);
    pthread_create(&third, NULL, track_ride, NULL);
    pthread_join(first, NULL);
    pthread_join(third,NULL);
    
    //track_ride();
    getch();
    endwin();
    return 0;
}