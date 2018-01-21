#ifndef LISTVAL_HPP
#define LISTVAL_HPP
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
using namespace std;


struct node
{
	struct tran{

		int type;//0 for message,1 for file
		char *message;
		char name[100];
		tran *next;
		
		tran(int a,char *buffer,char *bname,tran* n);
		~tran();

	};

		int id;
		char name[100];
		node*  next;
		tran*  edge;
		int size;


		node (int a,char *buffer,node* n);
};


#endif
