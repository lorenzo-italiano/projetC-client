
static void interface_init(){
    initscr();
    noecho();
    cbreak(); // voir ce que ça fait
    keypad(stdscr, TRUE);
    refresh();

    if(has_colors() == 0)
    {   endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_BLUE, COLOR_BLACK);
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(7, COLOR_CYAN, COLOR_BLACK);
    bkgd(COLOR_PAIR(1));

    /* height, width offsetHeight, offsetWidth */
    win_screen = newwin(LINES-8,COLS-2,3,1);
    assert(win_screen != NULL);
    box(win_screen, 0 , 0);

    win_title = newwin(3,COLS-2,0,1);
    assert(win_title != NULL);
    box(win_title, 0 , 0);

    win_display = subwin(win_screen, LINES-12,COLS-6,5,3);
    scrollok(win_display, TRUE);

    win_form = newwin(5,COLS-2,LINES-5,1);
    assert(win_form != NULL);
    box(win_form, 0, 0);

    sub_win_form = subwin(win_form, 1,COLS-6,LINES-3,3);
    assert(sub_win_form != NULL);
    box(sub_win_form, 0, 0);

    refresh_all();
}

static char* trim_whitespaces(char *str){
    char *end;

    // trim leading space
    while(isspace(*str)){
        str++;
    }

    if(*str == 0){ // all spaces?
        return str;
    }

    // trim trailing space
    end = str + strnlen(str, 128) - 1;

    while(end > str && isspace(*end)){
        end--;
    }

    // write new null terminator
    *(end+1) = '\0';

    return str;
}

//static char* supprWeirdChars(char* str){
//    int i = 0;
//    while(i!=)
//}

static void driver(int ch){
    int i;

    switch (ch) {
        case 10:
            if(strcmp(trim_whitespaces(field_buffer(fields[1], 0)),"Message..") != 0){

                // Or the current field buffer won't be sync with what is displayed
                form_driver(form, REQ_NEXT_FIELD);
                form_driver(form, REQ_PREV_FIELD);
                move(LINES-3, 2);

                doCommandAction(strcat(trim_whitespaces(field_buffer(fields[1], 0)),"\n"));
                //wprintw(win_display, strcat(trim_whitespaces(field_buffer(fields[1], 0)),"\n"));
                //window_print_magenta(strcat(trim_whitespaces(field_buffer(fields[1], 0)),"\n"));
                //wrefresh(win_display);

                set_field_buffer(fields[1], 0, "Message..");

                refresh();
                pos_form_cursor(form);
            }

            break;

        case KEY_DOWN:
            form_driver(form, REQ_NEXT_FIELD);
            form_driver(form, REQ_END_LINE);
            break;

        case KEY_UP:
            form_driver(form, REQ_PREV_FIELD);
            form_driver(form, REQ_END_LINE);
            break;

        case KEY_LEFT:
            form_driver(form, REQ_PREV_CHAR);
            break;

        case KEY_RIGHT:
            form_driver(form, REQ_NEXT_CHAR);
            break;

            // Delete the char before cursor
        case KEY_BACKSPACE:
        case 127:
            form_driver(form, REQ_DEL_PREV);
            break;

            // Delete the char under the cursor
        case KEY_DC:
            form_driver(form, REQ_DEL_CHAR);
            break;

        default:
            if(strcmp(trim_whitespaces(field_buffer(fields[1], 0)),"Message..") == 0){
                set_field_buffer(fields[1], 0, "");
            }
            form_driver(form, ch);
            break;
    }

    /*if(strlen(trim_whitespaces(field_buffer(fields[1], 0))) == 0){
        set_field_buffer(fields[1], 0, "Message..");
    }*/

    wrefresh(win_form);
}

static void form_initialisation(){
    fields[0] = new_field(1, 10, 0, 0, 0, 0);
    fields[1] = new_field(1, 40, 0, 15, 0, 0);
    fields[2] = NULL;

    assert(fields[0] != NULL && fields[1] != NULL);

    set_field_buffer(fields[0], 0, "Message: ");
    set_field_buffer(fields[1], 0, "Message..");

    set_field_opts(fields[0], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(fields[1], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);

    set_field_back(fields[1], A_UNDERLINE);

    form = new_form(fields);
    assert(form != NULL);
    set_form_win(form, sub_win_form);
    set_form_sub(form, sub_win_form);
    post_form(form);
}

static void free_interface(){
    unpost_form(form);
    free_form(form);
    free_field(fields[0]);
    free_field(fields[1]);
    free_field(fields[2]);

    delwin(win_screen);
    delwin(win_form);
    delwin(win_display);
    delwin(sub_win_form);
}

void resize_window(){
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    COLS = w.ws_col;
    LINES = w.ws_row;

    wclear(stdscr);
    wresize(stdscr, LINES, COLS);
    mvwin(stdscr, 0, 0);

    // win screen

    wclear(win_screen);

    wrefresh(win_screen);

    wresize(win_screen, LINES-8,COLS-2);

    wrefresh(win_screen);

    mvwin(win_screen, 3, 1);

    wclear(win_screen);

    wrefresh(win_screen);

    // win title

    wclear(win_title);

    wrefresh(win_title);

    wresize(win_title, 3,COLS-2);

    wrefresh(win_title);

    mvwin(win_title, 0, 1);

    wclear(win_title);

    wrefresh(win_title);

    // win display

    wclear(win_display);

    wrefresh(win_display);

    wresize(win_display, LINES-12,COLS-6);

    wrefresh(win_display);

    mvwin(win_display, 5,3);

    wclear(win_display);

    wrefresh(win_display);

    // win form

    wclear(win_form);

    wrefresh(win_form);

    wresize(win_form, 5,COLS-2);

    wrefresh(win_form);

    mvwin(win_form, LINES-5, 1);

    wclear(win_form);

    wrefresh(win_form);

    // sub win form

    wclear(sub_win_form);

    wrefresh(sub_win_form);

    wresize(sub_win_form, 1,COLS-6);

    wrefresh(sub_win_form);

    mvwin(sub_win_form, 3, LINES-3);

    wclear(sub_win_form);

    wrefresh(sub_win_form);

    refresh_all();

    // boxes

    clear();
    wclear(stdscr);

    box(win_form, 0, 0);

    box(win_screen, 0 , 0);

    box(win_title, 0 , 0);

    box(sub_win_form, 0, 0);

    refresh_all();
}
