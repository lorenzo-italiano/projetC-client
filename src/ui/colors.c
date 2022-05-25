void window_print_white(char* text){
    wattron(win_display, COLOR_PAIR(1));
    wprintw(win_display, text);
    wattroff(win_display, COLOR_PAIR(1));
    refresh_all();
}

void window_print_red(char* text){
    wattron(win_display, COLOR_PAIR(2));
    wprintw(win_display, text);
    wattroff(win_display, COLOR_PAIR(2));
    refresh_all();
}

void window_print_green(char* text){
    wattron(win_display, COLOR_PAIR(3));
    wprintw(win_display, text);
    wattroff(win_display, COLOR_PAIR(3));
    refresh_all();
}

void window_print_yellow(char* text){
    wattron(win_display, COLOR_PAIR(4));
    wprintw(win_display, text);
    wattroff(win_display, COLOR_PAIR(4));
    refresh_all();
}

void window_print_blue(char* text){
    wattron(win_display, COLOR_PAIR(5));
    wprintw(win_display, text);
    wattroff(win_display, COLOR_PAIR(5));
    refresh_all();
}

void window_print_magenta(char* text){
    wattron(win_display, COLOR_PAIR(6));
    wprintw(win_display, text);
    wattroff(win_display, COLOR_PAIR(6));
    refresh_all();
}

void window_print_cyan(char* text){
    wattron(win_display, COLOR_PAIR(7));
    wprintw(win_display, text);
    wattroff(win_display, COLOR_PAIR(7));
    refresh_all();
}
