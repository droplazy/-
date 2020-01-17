/*************************************************************************
  > File Name: tcp_server.c
  > Author: Master lu
  > Mail: luj@hqyj.com 
  > Created Time: 2019年11月08日 星期五 11时00分36秒
 ************************************************************************/
#include <stdio.h>
#include <iostream>
#define N 1024
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
using namespace std;
typedef struct{
	int no;
	char TYPE[40];
}sysmsg;
typedef struct{
	int type;
	char name[N];
	char passwd[N];
	int no;
}USER;
typedef struct{
	char name[N];
	char addr[N];
	int age;
	int no;
	double salary;
	char phone[20];
	char job[N];
}INFO;
int del(char*);
int modi(char*name,char*dest,char*neww);
int addinfo(INFO inf);
void slogin(int connfd);
void sregist(int connfd);
int addSQL(char* acc,char *password);
char* search(char * buf);
void sighandler(int signum);
void discover(int connfd);
INFO ssch(char* name);
int Smanag(int connfd);
void UserQuit(int connfd);

int main()
{
	int listenfd, connfd;
	struct sockaddr_in seraddr, cliaddr;
	int ret;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	char buf[1024] = "\0";
	pid_t pid;

//	signal(SIGCHLD, sighandler);
	signal(SIGCHLD, SIG_IGN);
	//创建套接字
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == listenfd){
		perror("socket");
		exit(-1);
	}
	//绑定IP+Port 
	bzero(&seraddr, sizeof(seraddr));
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(8888);
	seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ret = bind(listenfd, (struct sockaddr *)&seraddr, sizeof(struct sockaddr_in));
	if(-1 == ret){
		perror("bind");
		exit(-1);
	}
	//监听连接
	ret = listen(listenfd, 10);
	if(-1 == ret){
		perror("listen");
		exit(-1);
	}
	while(1){
		//接受连接
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &addrlen);
		if(-1 == connfd){
			perror("accept");
			exit(-1);
		}
		printf("Accept client[%s:%d] success\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

		pid = fork();
		if(-1 == pid){
			perror("fork");
			exit(-1);
		}
		else if (pid > 0){
			close(connfd);
			continue;
		}
		else if (0 == pid){
			close(listenfd);

			while(1){
				//接受数
			sysmsg msg;
			ret = recv(connfd,&msg, sizeof(msg), 0);
				if(-1 == ret){
					perror("recv");
					exit(-1);
				}
			//	printf("recv : %s\n", buf);
				switch(msg.no){
				case 1:slogin(connfd);break;			
				case 2:sregist(connfd);break;			
				case 3:discover(connfd);break;			
				case 4:Smanag(connfd);break;
				case 6:UserQuit(connfd);
					   strcpy(msg.TYPE,"quit");
					   break;
				case 5:cout << "user quited\n";
					   strcpy(msg.TYPE,"continue");break;
					}		
				if(!strncasecmp("quit",msg.TYPE, 4))
					break;
				else if(!strncasecmp("continue",msg.TYPE, 8))
					continue;
				bzero(buf, sizeof(buf));
			}

			close(connfd); //断开连接
			exit(0);
		}

	}
	close(listenfd); //关闭服务器
	return 0;
}

void sighandler(int signum){
	if(SIGCHLD == signum)
		while(waitpid(-1, NULL, WNOHANG) > 0);
}
void slogin( int connfd){
	sysmsg msg;
	msg.no = 1;
	strcpy(msg.TYPE,"put");
	send(connfd,&msg,sizeof(msg),0);//发送msg信息给cli
	USER zhh;
	recv(connfd,&zhh,sizeof(zhh),0);//收到帐号密码
	cout << zhh.name << endl;
	cout << zhh.passwd << endl;
	//////////验证帐号密码//////
	int ret=0,ret1=1,ret2=0;
	ret = strncasecmp(zhh.passwd,search(zhh.name),sizeof(zhh.passwd));
//	printf("%d\n l", ret);
	ret1 = strcasecmp(zhh.name,"123");
	ret2 = strcasecmp(zhh.passwd,"321");
	if(ret1 == 0 && ret2 == 0){
		cout << "manager login!\n";	
		msg.no = 4;
		strcpy(msg.TYPE,"manager");
		send(connfd,&msg,sizeof(msg),0);
	}

	else if( 0 == ret ){
		cout << "lOGIN!\n";
	msg.no = 3;
	strcpy(msg.TYPE,"use\n");
	send(connfd,&msg,sizeof(msg),0);}
	else {
		msg.no = 1;
		strcpy(msg.TYPE,"wrong\n");
		cout <<"right is =" << search(zhh.name) <<endl;
		send(connfd,&msg,sizeof(msg),0);
	} 

//	send(connfd,"hahaha",7,0);
}
void sregist(int connfd){
	sysmsg msg;
	msg.no = 2;
	send(connfd,&msg,sizeof(msg),0);//发送msg信息给cli
	USER zhh;
	recv(connfd,&zhh,sizeof(zhh),0);//收到帐号密码
	cout << zhh.name << endl;
	cout << zhh.passwd << endl;
///////////调用数据库///////	
	addSQL(zhh.name,zhh.passwd);

/////////////////
	bzero(&msg,sizeof(msg));
	msg.no = 1;
	strcpy(msg.TYPE,"login");
	send(connfd,&msg,sizeof(msg),0);
}
void discover(int connfd){
	char tp[120];
	sysmsg msg;
	INFO pe;
	msg.no = 3;
	send(connfd,&msg,sizeof(msg),0);//对面收到后进入选择
	bzero(&msg,sizeof(msg));	

	recv(connfd,&msg,sizeof(msg),0);//收到要查询的名字;
	cout << " client name is " << msg.TYPE <<endl;
	strcpy(tp,msg.TYPE);
	pe = ssch(tp);//pe.name);
	cout << "name :"<< pe.name <<endl;
	cout << "addr :"<< pe.addr <<endl;
	cout << "no   :"<< pe.no <<endl;
	cout << "age  :"<< pe.age <<endl;
	cout << "job  :"<< pe.job <<endl;
	cout << "phone:"<< pe.phone <<endl;
	cout << "salar:"<< pe.salary <<endl;


	send(connfd,&pe,sizeof(pe),0);


}
int Smanag(int connfd){
	 cout << "ent mange mode \n";
	 sysmsg msg;
	 int ret;
	 INFO inf;
	 msg.no = 4;
	 strcpy(msg.TYPE,"man\n");
	 send(connfd,&msg,sizeof(msg),0);
	 bzero(&msg,sizeof(msg));
	  recv(connfd,&msg,sizeof(msg),0);
	  cout << msg.TYPE <<endl;
	  
	if(msg.no == 1){
		cout << "recv inf\n";
		recv(connfd,&inf,sizeof(inf),0);
		cout << "name :"<< inf.name <<endl;
		cout << "addr :"<< inf.addr <<endl;
		cout << "no   :"<< inf.no <<endl;
		cout << "age  :"<< inf.age <<endl;
		cout << "job  :"<< inf.job <<endl;
		cout << "phone:"<< inf.phone <<endl;
		cout << "salar:"<< inf.salary <<endl;

		ret =addinfo(inf);
		if(ret == 1)
			printf ("not insed\n");
		bzero(&msg,sizeof(msg));
		msg.no = 4;
		strcpy(msg.TYPE,"man\n");
		send(connfd,&msg,sizeof(msg),0);
	}
	else if(msg.no == 2){
		recv(connfd,&msg,sizeof(msg),0);
		cout << "cli del: " <<msg.TYPE<<endl; 
		del(msg.TYPE);
		bzero(&msg,sizeof(msg));
		msg.no = 4;strcpy(msg.TYPE,"man\n");
		send(connfd,&msg,sizeof(msg),0);
	}
	else if(msg.no == 3){
		recv(connfd,&inf,sizeof(inf),0);
		cout << inf.name << inf.addr <<inf.job<<endl;
		modi(inf.name,inf.job,inf.addr);
		bzero(&msg,sizeof(msg));
		msg.no = 4;
		strcpy(msg.TYPE,"man\n");
		send(connfd,&msg,sizeof(msg),0);
	}
	else if(msg.no == 4){
		cout << "user BACK\n";
		/*
		recv(connfd,&msg,sizeof(msg),0);
		cout << msg.TYPE << endl;
		msg.no = 4;
		strcpy(msg.TYPE,"quited");
		send(connfd,&msg,sizeof(msg),0);*/
		return 0;
	}
	return 1;
}
void UserQuit(int connfd){
		sysmsg msg;
		msg.no = 3;
		strcpy(msg.TYPE,"quit");
		send(connfd,&msg,sizeof(msg),0);
}
