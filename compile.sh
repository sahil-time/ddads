rm *.o
rm main
gcc -Wall array_util.c -c 
gcc -Wall main.c array_util.o -o main
./main

