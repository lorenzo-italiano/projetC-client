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
