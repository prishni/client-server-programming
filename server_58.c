#include <stdio.h>
#include <stdlib.h>       //fork
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>  //read, write, sockaddr
#include <netdb.h> 	 //To store server info in clinet's process, hostent struct
char *text =NULL, *pattern =NULL;

void processinput(char *input){
	/*processing the input*/
	const char delimiter[2] = ";";
	text = strtok(input,delimiter);

	while(text!= NULL){
		pattern = strtok(NULL,delimiter);
		break;
	}
	pattern[strlen(pattern)-1]='\0';			//Remove end of line character
}

/*counts occurrences of pattern in the text*/
int printoccurrence(){
	int count =0;
	while((text= strstr(text,pattern))!=NULL){
		count++;
		text++;
	}
	return count;
}
int main(){
	/*port at which your server will be working (can choose anything between 1024 and 65535)*/
	int port_no = 5000;
	
	int sock_fd = socket(AF_INET,SOCK_STREAM,0);
	if(sock_fd ==-1){printf("error:socket formation\n"); return 0;}
	/*server information*/
	struct sockaddr_in servaddr;
	bzero((char *)&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr= INADDR_ANY;
	servaddr.sin_port = htons(port_no);
	
	if(bind(sock_fd,(struct sockaddr*)&servaddr,sizeof(servaddr)) <0);
	
	listen(sock_fd,10);
	
	/*client information*/
	struct sockaddr_in clientaddr;
	socklen_t c_len = sizeof(clientaddr);
	int no_of_server=0;
	for(;;)
	{
		int newsock_fd = accept(sock_fd,(sockaddr *)&clientaddr , &c_len );
		if(newsock_fd ==-1){printf("error:accept\n"); return 0;}
		pid_t pid =fork();
		if(pid == -1){
			close(newsock_fd);
			continue;
		}
		if(pid > 0){							//Parent
			no_of_server++;
			close(newsock_fd);
			continue;
		}
		if(pid == 0){							//Child
			no_of_server++;
			char buffer[256];
			int n = read(newsock_fd,buffer,255);
			processinput(buffer);
			int occ =printoccurrence();
			sprintf(buffer, "%d", occ);
			n = write(newsock_fd,buffer,sizeof(buffer));
			close(newsock_fd);
			break;
		}
		
		
	}
	close(sock_fd);
	return 0;
}
