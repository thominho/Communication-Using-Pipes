#include "../headers/ListVal.hpp"



node::tran::tran(int a,char *buffer,char *bname,tran* n){
	type=a;
	if(type==1){
		strcpy(name,bname);
		cout << "here" << endl;
	}
	int b=strlen(buffer)+10;
	message=(char *) malloc(sizeof(char)*b);
	strcpy(message,buffer);
	cout << "buffer:" << buffer << endl;
	cout << "message:" << message << endl;
	next=n;
}

node::tran::~tran(){
	free(message);
}

node::node (int a,char *buffer,node* n){
		id=a;
		strcpy(name,buffer);
		next=n;
		edge=NULL;
		size=0;
}