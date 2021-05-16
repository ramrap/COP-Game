build: 
	g++ -o game -std=c++11 main.cpp objects.cpp client_udp.cpp server_udp.cpp network.cpp physic.cpp list.cpp font.cpp menu.cpp  `sdl2-config --cflags --libs` -lSDL2_ttf -lpthread -I. -Wall 

