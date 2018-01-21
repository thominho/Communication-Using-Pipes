#
# In order to execute this "Makefile" just type "make"
#	A. Delis (ad@di.uoa.gr)
#

OBJS 	= main.o
OBJS2	= boardpost.o
OBJS3	= boardserver.o news.o ListVal.o
SOURCE	= main.c sources/boardserver.cpp sources/boardpost.cpp sources/news.cpp sources/ListVal.cpp
HEADER  = headers/ListVal.hpp headers/news.hpp
OUT  	= boardserver
CC	= g++
FLAGS   = -g -c -pedantic -ansi  -Wall
# -g option enables debugging mode 
# -c flag generates object code for separate files

$(OUT): $(OBJS3)
	$(CC) -g $(OBJS3) -o $@

boardserver.o:	sources/boardserver.cpp
	$(CC) $(FLAGS) sources/boardserver.cpp

news.o:	sources/news.cpp
	$(CC) $(FLAGS) sources/news.cpp

ListVal.o:	sources/ListVal.cpp
	$(CC) $(FLAGS) sources/ListVal.cpp

# clean house
clean:
	rm -f $(OBJS) $(OUT)
	rm -f $(OBJS2) $(OUT2)
	rm -f $(OBJS3) $(OUT3)

others:
	g++ -o boardpost sources/boardpost.cpp
	g++ -o main main.cpp
	chmod 777 boardpost

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
