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

static void driver(int ch){
    int i;

    switch (ch) {
        case 10:
            if(strcmp(trim_whitespaces(field_buffer(fields[1], 0)),"Message..") != 0){

                // Or the current field buffer won't be sync with what is displayed
                form_driver(form, REQ_NEXT_FIELD);
                form_driver(form, REQ_PREV_FIELD);
                move(LINES-3, 2);

                doCommandAction(trim_whitespaces(field_buffer(fields[1], 0)));
                //wprintw(win_display, strcat(trim_whitespaces(field_buffer(fields[1], 0)),"\n"));
                window_print_magenta(trim_whitespaces(field_buffer(fields[1], 0)));
                wrefresh(win_display);

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
