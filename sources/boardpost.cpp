#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <signal.h>

#define MSGSIZE 4096

using namespace std;


int main(int argc, char *argv[]){
	int fd, fd2 , i, nwrite, pid, rvalue;
	fd_set rfds,wfds;
	sigset_t set1, set2;
	char fifo[100] = "myfifo3";
	char fifo2[100] = "myfifo4";


	if(argc>=3){
	strcpy(fifo,argv[1]);
	strcpy(fifo2,argv[2]);
	}

	sigfillset(&set1);
	sigfillset(&set2); 
	sigdelset(&set2, SIGINT);
	sigprocmask(SIG_SETMASK, &set1, NULL);  // disallow everything here!
	sigprocmask(SIG_UNBLOCK, &set2, NULL);  // allow all but INT & TSTP

	if ( (fd=open(fifo, O_WRONLY)) < 0)
		{ perror("client f1 open error"); exit(1); }

	if ( (fd2=open(fifo2, O_RDONLY | O_NONBLOCK)) < 0)
		{ perror("client f2 open error"); exit(1); }


	while(1){
		char msgbuf[MSGSIZE+1];
		cout << "Commands:" << endl;
		cout << "list" <<endl;
		cout << "send-<id>-<filename>" << endl;
		cout << "write-<id>-<message>" << endl;
		cout << "shutdown" << endl;
		cout << endl;
		cin.getline(msgbuf,MSGSIZE+1);
		/*commands format is: command-id-message example: write-1-news*/
		if(strcmp(msgbuf,"shutdown")==0){
			exit(0);
		}
		if(strncmp(msgbuf,"list",4)==0){
				nwrite=write(fd, msgbuf, MSGSIZE+1);
				nwrite=1;
				/*read as far there is something to read*/
				cout << "Channels " << endl;
				while(nwrite>=0){
					char msgbuf2[MSGSIZE+1];
					nwrite=read(fd2, msgbuf2, MSGSIZE+1);
					//cout << nwrite << endl;
					if(nwrite>0){
					printf("%s", msgbuf2);
					}
					fflush(stdout);
				}
			}
		else if(strncmp(msgbuf,"write",5)==0){
			nwrite=write(fd, msgbuf, MSGSIZE+1);
		}
		else if(strncmp(msgbuf,"send",4)==0){
			char msgbuf15[4096];
			strcpy(msgbuf15,msgbuf);
			char *temp=strtok(msgbuf,"- \n");
			temp=strtok(NULL,"- \n");
			temp=strtok(NULL,"- \n");
			FILE *fp;
			fp=fopen(temp,"rb");
			char msgbuf3[MSGSIZE+1];
			if(fp!=NULL){
				fread(msgbuf3,1,MSGSIZE+1,fp);
				strcat(msgbuf15,"-");
				strcat(msgbuf15,msgbuf3);
				nwrite=write(fd, msgbuf15, MSGSIZE+1);
				fclose(fp);
			}
			else
			{
				cout << "No-Such-File" << endl;
			}
		}
	}
	exit(0);
}


