# Sneaky Sneaky

A 2D Game Based on SDL C++



## Build
```
$ make
```

here make file corresponds to 

```
$ g++ -o game -std=c++11 main.cpp objects.cpp client_udp.cpp server_udp.cpp network.cpp physic.cpp list.cpp font.cpp menu.cpp audio.cpp  `sdl2-config --cflags --libs` -lSDL2_ttf  -lpthread -lSDL_mixer -lsmpeg -I. -Wall 
```

here we compiled all .cpp files 

All flags required are also present in MakeFile


* Tested on MacOS
* Sorry Linux/Windows



## Library Used

*SDL2* ,
*SDL2_ttf* , 
*SDL2_mixer*

All Libraries can be install using brew in mac



## Resources

* pthread tutorial

* SDL Library Documentations
-SMLPEG (Used for audio mixing but later removed)

-Github (Repos present all over github to learn about application part)
-Youtube Tutorials
-Medium Articles
-Game Idea - (Capture The Flag)
-Last but Not Least (Stack Overflow)



