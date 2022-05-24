gcc -g -pedantic src/main.c -o client -lform -lncurses -pthread
./client 127.0.0.1 3001