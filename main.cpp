#include <ncurses.h>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <pthread.h>
#include <random>
#include <time.h>
#include <iostream>

const float max_speed = 40;
int positions[10];
int speed_table[10];
int t_created_number = 0;


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
        char rand_char = random_char();
        int t_number = t_created_number;
        int position = 0;
        float speed = random_speed();
        for (int i = 0; i<3; i++){ // licznik okrążeń
            int x = 0;
            int y = 0;
            float new_speed;
            new_speed = 0.6 * (float) speed;
            if(new_speed <= max_speed){
                speed = (int) max_speed;
            }
            else{
                speed = (int) new_speed;
            }
            speed_table[t_number] = speed;
            while(x<35){
                x++;
                position++;
                positions[t_number] = position;
                moving( x, y, speed, rand_char);
            }
            new_speed = 1.2 * (float) speed; 
            speed = (int) new_speed;
            speed_table[t_number] = speed;
            while(y<15){
                y++;
                position++;
                positions[t_number] = position;
                moving(x, y, speed, rand_char); 
            }
            new_speed = 0.6 * (float) speed;
            if(new_speed <= max_speed){
                speed = (int) max_speed;
            }
            else{
                speed = (int) new_speed; 
            }
            speed_table[t_number] = speed;
            while(x>0){
                x--;
                position++;
                positions[t_number] = position;
                moving(x, y, speed, rand_char);
            }
            new_speed = 1.2 * (float) speed;
            speed = (int) new_speed;
            speed_table[t_number] = speed;
            while(y>0 && i<2){
                y--;
                position ++;
                positions[t_number] = position;
                moving(x, y, speed, rand_char);    
            }
        }
        position = 100;
        return NULL;
    }

    void draw_track(int width, int height, int start_x, int start_y){ 
    WINDOW * outer_bound = newwin(height, width, start_y, start_x); // okno
    refresh(); // odswiezenie okna
    box(outer_bound, 0, 0);// zrobienie obramowania
    wrefresh(outer_bound);//odswiezenia okna zeby byla ramka
    }

    void *create_threads(void *ptr){
        pthread_t my_thread[10];
        for (int i=0;i<10;i++){
            sleeping(rand()%500 + 1400);
            pthread_create(&my_thread[i],NULL,track_ride, NULL);
        }
        return ptr;
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
    pthread_t thr;
    pthread_create(&thr, NULL,*create_threads,NULL);
    getch();
    endwin();
    return 0;
}