static void refresh_all(){
    refresh();
    wrefresh(win_screen);
    wrefresh(win_form);
    wrefresh(win_display);
    wrefresh(win_title);
    wrefresh(sub_win_form);
}

void set_title(char* title){
    wclear(win_title);
    box(win_title,0,0);
    mvwprintw(win_title, 1, (COLS/2)-(strlen(title)/2),title);
    refresh_all();
}

void discoAux(int colorPairTitle, int colorPairOther, unsigned int interval){
    wbkgd(win_title,COLOR_PAIR(colorPairTitle));
    wbkgd(win_screen, COLOR_PAIR(colorPairOther));
    wbkgd(win_form, COLOR_PAIR(colorPairOther));
    refresh_all();
    usleep(interval);
}

void raimbowOnce(unsigned int ms){
    discoAux(8, 2, ms);
    discoAux(9, 4, ms);
    discoAux(10, 3, ms);
    discoAux(11, 7, ms);
    discoAux(12, 5, ms);
    discoAux(13, 6, ms);
    discoAux(1, 1, ms);
}


void *disco(){
    //wclear(win_title);
    //set_title("Salon 1");
    unsigned int ms = 500000;
    while(isDiscoMode){
            raimbowOnce(ms);
    }
    pthread_exit(NULL);
}
