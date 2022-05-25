static void refresh_all(){
    refresh();
    wrefresh(win_screen);
    wrefresh(win_form);
    wrefresh(win_display);
    wrefresh(win_title);
    wrefresh(sub_win_form);
}

