#include <ncurses.h>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <pthread.h>
#include <random>
#include <time.h>
#include <iostream>

/*Proszę dodać do ruchu odcinek trasy,
 na którym może przebywać na raz tylko jeden pojazd.
 Jeżeli następny dojeżdża do zajętego odcinka,
 to się zatrzymuje i czeka na zwolnienie odcinka.
 Jeżeli czeka parę pojazdów, to wjeżdża ten, z największym przyśpieszeniem.*/

const float max_speed = 60;
int positions[10];
int speed_table[10];
int acceleration[10];
int t_created_number = 0;
bool is_route_busy = false;
bool is_waiting_for_route[10];
int best_waiting_thread;
int cars_out=0;
pthread_mutex_t route_mutex;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


    void close_track(){
        is_route_busy = true;
    }

    void open_track(){
        is_route_busy = false;
    }

    int random_speed(){
        int speed;
        speed = rand()%300 + 120;
        return speed;
    }

    int random_acceleration(){
        int acceleration;
        acceleration =  rand()%10 + 5;
        return acceleration;
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
    

    bool is_fastest(int t_number){
        int fastest_thread = t_number;
        for(int i=0;i<10;i++){
            if(is_waiting_for_route[i] && i!=fastest_thread){
                if(acceleration[fastest_thread] < acceleration[i]){
                    return false;
                }
            }
        }
        return true;
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

    bool check_move(int thread_number){
        for (int i=0;i<10;i++){
            if(i!=thread_number){
                if((positions[thread_number]+1)==positions[i]){
                    if(speed_table[thread_number]<speed_table[i]){
                        speed_table[thread_number]=speed_table[i];
                        if(acceleration[thread_number]>acceleration[i]){
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }


    void accelerate(int t_number){
        if(speed_table[t_number]-acceleration[t_number]>=max_speed){
            speed_table[t_number] = speed_table[t_number]-acceleration[t_number];
        }
    }

    int how_many_wait(){
        int counter = 0;
        for (int i=0;i<10;i++){
            if(is_waiting_for_route[i]==true){
                counter++;
            }
        }
        return counter;
    }

    void * track_ride(void * arg){
        char rand_char = random_char();
        int t_number = t_created_number;
        speed_table[t_number]=random_speed();
        acceleration[t_number] = random_acceleration();
        is_waiting_for_route[t_number] = false;
        for (int i = 0; i<3; i++){ // licznik okrążeń
            int x = 0;
            int y = 0;
            positions[t_number]=0;
            while(x<35){
                if(check_move(t_number)){
                    x++;
                    positions[t_number]++;
                    moving( x, y, speed_table[t_number], rand_char);
                    accelerate(t_number);
                }
            }
            while(y<15){
                if(check_move(t_number)){
                    y++;
                    positions[t_number]++;
                    moving(x, y, speed_table[t_number], rand_char);
                    accelerate(t_number);
                }
            }
            while(i<3 && x>0){
                if(check_move(t_number)){
                    x--;
                    positions[t_number]++;
                    moving(x, y, speed_table[t_number], rand_char);
                    accelerate(t_number);  
                }
            }
            pthread_mutex_lock(&route_mutex);
            is_waiting_for_route[t_number] = true;
            while(!is_fastest(t_number)){
                pthread_cond_wait(&cond, &route_mutex);
            }
            while(y>0 && i<2){
                    y--;
                    positions[t_number]++;
                    moving(x, y, speed_table[t_number], rand_char);
                    accelerate(t_number);
            }
            is_waiting_for_route[t_number]=false;
            pthread_mutex_unlock(&route_mutex);
            pthread_cond_signal(&cond);
        }
        positions[t_number]=1000; //pojazd poza torem
        cars_out++;
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
        //pthread_t controller;
            //pthread_create(&controller,NULL, select_fastest, NULL);
        for (int i=0;i<10;i++){
            sleeping(rand()%1000 + 600);
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