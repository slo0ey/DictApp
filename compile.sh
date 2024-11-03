g++ -c main.cpp dict.cpp dictloader.cpp
g++ main.o dict.o dictloader.o -o app.out
rm main.o
rm dict.o
rm dictloader.o
