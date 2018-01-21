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
#include <string>
#define MSGSIZE 4096

using namespace std;

int main(int argc, char *argv[]){
	int fd, fd2 , i, nwrite, pid, rvalue;
	FILE *processid;
	sigset_t set1, set2;
	char fifo[100] = "myfifo";
	char fifo2[100] = "myfifo2";
	char fifo3[100] = "myfifo3";
	char fifo4[100] = "myfifo4";
	char buffer15[100];
	strcpy(buffer15,"processid.txt");
	if(access(buffer15,F_OK)==-1){/*check if file exists if exists just add a new at the end of the newfile*/
		strcat(buffer15,"new");
		strcat(buffer15,".txt");
		processid=fopen(buffer15,"w+");
	}
	else{
		processid=fopen("processid.txt","w+");
	}

	sigfillset(&set1);
	sigfillset(&set2); 
	sigdelset(&set2, SIGINT);
	sigprocmask(SIG_SETMASK, &set1, NULL);  // disallow everything here!
	sigprocmask(SIG_UNBLOCK, &set2, NULL);  // allow all but INT & TSTP

	if(argc>=5){
	strcpy(fifo,argv[1]);
	strcpy(fifo2,argv[2]);
	strcpy(fifo3,argv[3]);
	strcpy(fifo4,argv[4]);
	}

	rvalue=mkfifo(fifo, 0666);

	rvalue=mkfifo(fifo2, 0666);

	rvalue=mkfifo(fifo3, 0666);

	rvalue=mkfifo(fifo4, 0666);

	pid=fork();

	if(pid==0){
		close(fd);
		close(fd2);
		rvalue=execlp("./boardserver","boardserver.cpp",fifo,fifo2,fifo3,fifo4,NULL);
		exit(2);
	}
	else if(pid==-1){
		cout << "could not create process" << endl;
		exit(1);
	}

	if ( (fd=open(fifo, O_WRONLY)) < 0)
		{ perror("client f1 open error"); exit(1); }

	if ( (fd2=open(fifo2, O_RDONLY | O_NONBLOCK)) < 0)
		{ perror("client f2 open error"); exit(1); }

	fprintf(processid,"Process Id For Server ");
	fprintf(processid,"%d\n",pid);
	fclose(processid);

	while(1){
		char msgbuf[MSGSIZE+1];
		cout << "Commands:" << endl;
		cout << "createchannel-<id>-<name>" <<endl;
		cout << "getmessages-<id>" << endl;
		cout << "exit" << endl;
		cout << "shutdown" << endl;
		cout << endl;
		cin.getline(msgbuf,MSGSIZE+1);
		/*commands format is: command-id-name example: createchannel-1-news*/
		nwrite=write(fd, msgbuf, MSGSIZE+1);
		if(strcmp(msgbuf,"shutdown")==0 || strcmp(msgbuf,"exit")==0){
			if(strcmp(msgbuf,"exit")==0){
				exit(0);
			}
			break;
		}
		
		if(strncmp(msgbuf,"getmessages",11)==0){
				nwrite=1;
				/*read as far there is something to read*/
				char *temp=strtok(msgbuf,"-\n");
				char *temp2=strtok(NULL,"-\n");
				cout << "Messages Channel " << temp << endl;
				while(nwrite>=0){
					char msgbuf2[MSGSIZE+1];
					nwrite=read(fd2, msgbuf2, MSGSIZE+1);
					char *temp3=strtok(msgbuf2,"-\n");
					char *temp4;
					int i=atoi(temp3);
					if(nwrite>0 && i==0){
						temp3=strtok(NULL,"-\n");
						printf("%s\n", temp3);
						fflush(stdout);
					}
					else if(nwrite>0 && i==1){
						temp3=strtok(NULL,"-\n");
						printf("%s\n", temp3);
						fflush(stdout);
						FILE *fp;
						temp4=strtok(NULL,"-\n");
						if(access(temp3,F_OK)==-1){/*check if file exists if exists just add a new at the end of the newfile*/
							char buffer[100];
							strcpy(buffer,temp3);
							strcat(buffer,"new");
							strcat(buffer,".bin");
							fp=fopen(buffer,"wb");
						}
						else{
							fp=fopen(temp3,"wb");
						}
						char buffer20[4096];
						strcpy(buffer20,temp4);
						fwrite(buffer20,1,strlen(buffer20),fp);
						fclose(fp);
					}
				}
			}
		}

	unlink(fifo);
	unlink(fifo2);
	unlink(fifo3);
	unlink(fifo4);
	exit(0);
}
		
