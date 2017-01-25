#include <stdio.h>
#include <stdlib.h>       
#include <unistd.h>  	   //fork
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>  //read write
#include <netdb.h> 	 //hostent structure

char *text=NULL,*pattern =NULL;
void print(char* msg){
	printf("%s",msg);
	printf("Re-enter input:\n");
}
void processinput(char *input){
	/*processing the input*/
	const char delimiter[2] = ";";
	if(input[0]==';') {							//if no text present
		text =NULL;
		pattern = strtok(input,delimiter);
		pattern[strlen(pattern)-1]='\0';
	}
	else {
		text = strtok(input,delimiter);
		while(1){
			pattern = strtok(NULL,delimiter);
			break;
		}
		pattern[strlen(pattern)-1]='\0';
	}
}
int countsemicolon(char *input){
	int count =0;
	char pat[2]=";";
	char *inp =input;
	while((inp= strstr(inp,pat))!=NULL){
		count++;
		inp++;
	}
	return count;
}
int inputformat(char *input){
	/*check for presence of semicolon*/
	if(!strchr(input,';')){
		print("\nplease provide a pattern along with the text\n (semicolon missing)\n");
		return 0;
	}
	/*semicolons should not exceed 1*/
	else if(countsemicolon(input)>1){
		print("\nplease provide a single \';\'\n");
		return 0;
	}
	/*text or pattern shouldn't be NULL*/
	else{
		processinput(input);
		if(text==NULL || pattern== NULL || strlen(pattern)==0){
			print("please provide both text and pattern\n");
			return 0;
		}
		else if(strlen(text)>30 || strlen(pattern)>5){
			print("please provide both (text and pattern) of length atmost (30,5) characters\n");
			return 0;
		}
		else if(strchr(pattern,' ')){
			print("pattern can't have spaces\n");
			return 0;
		}
		else{
			int i;
			for(i=0;i<strlen(text);i++){
				if(text[i]<'a' || text[i]>'z'){
					if(text[i]<'A' || text[i]>'Z'){
						if(text[i] != ' '){
							print("text cannot have special characters\n");
							return 0; }}}}
			for(i=0;i<strlen(pattern);i++){
				if(pattern[i]<'a' || pattern[i]>'z'){
					if(pattern[i]<'A' || pattern[i]>'Z'){
							print("pattern cannot have special characters\n");
							return 0; }}}
			}
	}
	return 1;
}

int main(){
	int portno = 5000;
	
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1) {printf("error:socket formation\n"); return 0;}
	/*server information*/
	struct hostent *server;	
	server = gethostbyname("127.0.0.1");
	struct sockaddr_in servaddr;
	bzero((char *) &servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&servaddr.sin_addr.s_addr, server->h_length);
	servaddr.sin_port = htons(portno);
	
	if(connect(sockfd,(struct sockaddr *) &servaddr,sizeof(servaddr))== -1) {printf("error:could not connect to server\n"); return 0;}

	char buffer[256];
	char *input = NULL;
	size_t size;
	
	/*input string*/
	printf("enter the text and pattern:-\n");
	getline(&input,&size,stdin);
	sprintf(buffer, "%s", input);
	
	while(1){
		if(inputformat(input)==1) break;
		getline(&input,&size,stdin);
		sprintf(buffer, "%s", input);
	}

	int n;
	n = write(sockfd,buffer,sizeof(buffer));
	bzero(buffer,256);
	n = read(sockfd,buffer,255);
	printf("count of occurrence of \"%s\" in the \"%s\"= %s \n\n",pattern,text, buffer);
	
	close(sockfd);
	return 0;
}
