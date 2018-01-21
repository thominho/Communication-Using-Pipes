#ifndef NEWS_HPP
#define NEWS_HPP
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <signal.h>
#include "ListVal.hpp"
using namespace std;

class news{
private:


	node *_start;
	int size;


public:

	news();
	~news();
	void destroy();
	void insertchannel(int ch,char *buffer);
	void insertmessage(char *buffer,int ch);
	void insertfile(char *buffer,int ch,char* name);
	void get_message(int fd,int ch);
	void get_channel(int fd);
};



#endif
