#include "../headers/news.hpp"

/*---------------------------------------------------------------------------------------------------------------------*/

news::news(){
	size=0;
	_start=NULL;
}

/*---------------------------------------------------------------------------------------------------------------------*/

news::~news(){
	while(size!=0){
		destroy();
	}
}

/*---------------------------------------------------------------------------------------------------------------------*/

void news::destroy(){
	node *t=_start;
	if(size==0){
		return;
	}
	_start=_start->next;
	while(t->size!=0){
		node::tran *t2=t->edge;
		t->edge=t->edge->next;
		delete t2;
		t->size--;
	}
	delete t;
	size--;
}

/*------------------------------------------------------------------------------------------------------------------------*/
void news::insertchannel(int ch,char *buffer){
	node *t=_start;
	while(t!=NULL){
		if(t->id==ch){
			cout << "Channel Exists" << endl;
			return;
		}
		t=t->next;
	}
	_start=new node(ch,buffer,_start);
	cout << "Server channel added" << endl;
	size++;

}

/*---------------------------------------------------------------------------------------------------------------------------*/

void news::insertmessage(char *buffer,int ch){
	node *t=_start;
	while(t!=NULL){
		if(t->id==ch){
			break;
		}
		t=t->next;
	}
	if(t==NULL){
		cout << "Channel Doesnt Exist" << endl;
		return;
	}
	t->edge=new node::tran(0,buffer,NULL,t->edge);
	cout << "Server messages added" << endl;
	t->size++;
	
}

/*---------------------------------------------------------------------------------------------------------------------------*/

void news::insertfile(char *buffer,int ch,char *name){
	node *t=_start;
	while(t!=NULL){
		if(t->id==ch){
			break;
		}
		t=t->next;
	}
	if(t==NULL){
		cout << "Channel Doesnt Exist" << endl;
		return;
	}
	t->edge=new node::tran(1,buffer,name,t->edge);
	cout << "Server file added" << endl;
	t->size++;
}

/*---------------------------------------------------------------------------------------------------------------------------*/	

void news::get_message(int fd,int ch){
	node *t=_start;
	if(size==0){
		write(fd,"No-Channels",strlen("No-Channels"));
		return;
	}
	while(t!=NULL){
		if(t->id==ch){
			break;
		}
		t=t->next;
	}
	if(t==NULL){
		write(fd,"No-Channel",strlen("No-Channel"));
		return;
	}
	node::tran *t2=t->edge;
	if(t->size==0){
		write(fd,"No-Messages",strlen("No-Messages"));
		return;
	}
	while(t2!=NULL){
		if(t2->type==1){
			char buffer[4096];
			char buffer3[10]; 
			sprintf(buffer3,"%d",t2->type);
			strcpy(buffer,buffer3);
			strcat(buffer,"-");
			strcat(buffer,t2->name);
			strcat(buffer,"-");
			strcat(buffer,t2->message);
			strcat(buffer,"-");
			//cout << buffer << endl;
			write(fd,buffer,strlen(buffer));
		}
		else{
			char buffer[4096];
			char buffer3[10]; 
			sprintf(buffer3,"%d",t2->type);
			strcpy(buffer,buffer3);
			strcat(buffer,"-");
			strcat(buffer,t2->message);
			strcat(buffer,"-");
			//cout << buffer << endl;
			write(fd,buffer,strlen(buffer));
		}
		t2=t2->next;
	}
}


/*---------------------------------------------------------------------------------------------------------------------------*/


void news::get_channel(int fd){
	node *t=_start;
	if(size==0){
		write(fd,"No-Channels",strlen("No-Channels"));
		return;
	}
	while(t!=NULL){
		char buffer[1000];
		char buffer2[1000];
		sprintf(buffer2,"%d",t->id);
		strcpy(buffer,buffer2);
		strcat(buffer," ");
		strcat(buffer,t->name);
		strcat(buffer,"\n");
		write(fd,buffer,strlen(buffer));
		t=t->next;
	}
}

/*----------------------------------------------------------------------------------------------------------------------------*/