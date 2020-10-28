rm *.o
rm main
gcc -Wall array_util.c -c -pthread
gcc -Wall main.c array_util.o -o main
./main

