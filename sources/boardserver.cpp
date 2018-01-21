#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include "../headers/news.hpp"
#include <iostream>

#define MSGSIZE 4096

using namespace std;

int main(int argc, char *argv[]){
	int fd, fd2, fd3,fd4, nwrite;
	sigset_t set1, set2;
	fd_set rfds;
	char fifo[100],fifo2[100],fifo3[100],fifo4[100];
	news l;
	strcpy(fifo,argv[1]);
	strcpy(fifo2,argv[2]);
	strcpy(fifo3,argv[3]);
	strcpy(fifo4,argv[4]);
	

	/*we delete cntl+c signal*/
	sigfillset(&set1);
	sigfillset(&set2); 
	sigdelset(&set2, SIGINT);
	sigprocmask(SIG_SETMASK, &set1, NULL);  
	sigprocmask(SIG_UNBLOCK, &set2, NULL);
	close(0);

	if ( (fd=open(fifo, O_RDONLY | O_NONBLOCK)) < 0){
		perror("fifo f1 open problem"); exit(3);	
		}
		//cout << "here" << endl;
	if ( (fd2=open(fifo2, O_WRONLY | O_APPEND)) < 0){
		perror("fifo f2 open problem"); exit(3);	
		}
		//cout << "here2" << endl;
	//i think is correct that it blocks here as there is no reason for a server who have nothing to send back to client
	if ( (fd3=open(fifo3, O_RDONLY | O_NONBLOCK)) < 0){
		perror("fifo f3 open problem"); exit(3);	
		}
		//cout << "here3" << endl;
	if ( (fd4=open(fifo4, O_WRONLY | O_APPEND)) < 0){
		perror("fifo f4 open problem"); exit(3);	
		}
		//cout << "here4" << endl;
	while(1){

/*initializing select*/
		FD_ZERO(&rfds);
		FD_SET(fd,&rfds);
		FD_SET(fd3,&rfds);
		char msgbuf[MSGSIZE+1];
		select(1024,&rfds,NULL,NULL,NULL);
		//cout << "here5" << endl;
/*HERE IS FOR THE COMMUNICATION BOARDCLIENT-BOARDSERVER*/

		if(FD_ISSET(fd,&rfds)>0){
			nwrite=read(fd, msgbuf, 1000);
			if(strncmp(msgbuf,"createchannel",13)==0 && nwrite>0){

				char *temp=strtok(msgbuf,"- \n");
				temp=strtok(NULL,"- \n");
				int i=atoi(temp);
				temp=strtok(NULL,"- \n");
				l.insertchannel(i,temp);

			}
			else if(strncmp(msgbuf,"getmessages",11)==0 && nwrite>0){

				char *temp=strtok(msgbuf,"- \n");
				temp=strtok(NULL,"- \n");
				int i=atoi(temp);
				l.get_message(fd2,i);

			}
			else if(strncmp(msgbuf,"shutdown",8)==0 && nwrite>0){
				break;
			}
		}	

/*HERE IS FOR THE COMMUNICATION BOARDSERVER-BOARDPOST*/
		//cout << "here6" << endl;
		//cout << FD_ISSET(fd3,&rfds) << endl;
		if(FD_ISSET(fd3,&rfds)>0){
			//cout << "here3" << endl;
			nwrite=read(fd3, msgbuf, MSGSIZE+1);
			cout << msgbuf << endl;
			if(strncmp(msgbuf,"list",4)==0 && nwrite>0){
				l.get_channel(fd4);
			}
			else if(strncmp(msgbuf,"write",5)==0 && nwrite>0){

				char *temp=strtok(msgbuf,"- \n");
				temp=strtok(NULL,"- \n");
				int i=atoi(temp);
				cout << i << endl;
				temp=strtok(NULL,"- \n");
				l.insertmessage(temp,i);

			}
			else if(strncmp(msgbuf,"send",4)==0 && nwrite>0){
				char buffer25[4096];
				strcpy(buffer25,msgbuf);
				char *temp=strtok(buffer25,"- \n");
				char *temp2,*temp3;
				temp=strtok(NULL,"- \n");
				temp3=strtok(NULL,"- \n");
				int i=atoi(temp);
				temp=strtok(buffer25,"-\n");
				temp2=strtok(NULL,"-\n");
				l.insertfile(temp2,i,temp3);

			}
		}

/*----------------------------------------------------------*/


	}



}

