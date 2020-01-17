/*************************************************************************
  > File Name: cli.cpp
  > Author: Master lu
  > Mail: luj@hqyj.com 
  > Created Time: 2019年11月08日 星期五 14时01分07秒
 ************************************************************************/



#define WELTEXT "/****************************************/\n			1.登录\n			2.注册\n			3.退出\n/**************************************/\n\0"
#define MATEXT "/*********************************\n		1.插入信息\n		2.删除条目\n		3.修改信息\n		4.返回\n/******************************"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#define N 1024
using namespace std;
void login(int connfd);
void regist(int connfd);
void usermode(int connfd);
int manage(int connfd);
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
int flg =0;
int main()
{
	int connfd;
	struct sockaddr_in seraddr;
	int ret;
//	char buf[1024] = "\0";
	//创建套接字
	connfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == connfd){
		perror("socket");
		exit(-1);
	}
	//发起连接
	bzero(&seraddr, sizeof(seraddr));
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(8888);
	seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ret = connect(connfd, (struct sockaddr *)&seraddr, sizeof(struct sockaddr_in));
	if(-1 == ret){
		perror("connect");
		exit(-1);
	}
	//	send(connfd,"niaho",6,0);
	sysmsg msg;
	//cin.getline(msg.TYPE,100);
	while(1){

		if(flg == 0){
			cout << WELTEXT;	
	 		scanf("%d",&msg.no);
			cin.get();
			switch(msg.no){
			case 1:strcpy(msg.TYPE,"login");break;
			case 2:strcpy(msg.TYPE,"regist");break;
			case 3:strcpy(msg.TYPE,"quit");break;
			case 4:strcpy(msg.TYPE,"manage");break;
			default:
				   strcpy(msg.TYPE,"quit");break;
			}
			send(connfd,&msg,sizeof(msg),0);
			if(!strncasecmp("quit",msg.TYPE , 4))break;
					flg++;
		}

		else{	
	//		cout <<"debug\n";
			bzero(&msg,sizeof(msg));
			recv(connfd,&msg,sizeof(msg),0);
	//		cout << "recv:="<<msg.TYPE << endl;;
			switch(msg.no){
			case 1:login(connfd);
				   break;
			case 2:regist(connfd);
				   break;
			case 3:usermode(connfd);
				   break;
			case 4:manage(connfd);
				   break;}//else1 
		}
	}

	close(connfd);
	return 0;
}
void login(int connfd){
	USER user;
	sysmsg msg;
//	char buf[1024];
	cout <<" put account\n";
	cin.getline(user.name,100);
	cout << "put password\n";
	cin.getline(user.passwd,100);
	send(connfd,&user,sizeof(user),0);//fa

	recv(connfd,&msg,sizeof(msg),0);
	if(msg.no == 1){//错误的帐号密码
		cout << "wrong acc or passwd \n";
	bzero(&msg,sizeof(msg));
	msg.no = 1;
	strcpy(msg.TYPE,"reput\n");
	send(connfd,&msg,sizeof(msg),0);//这条是发给服务区头的 下面也一样
	}
	else if(msg.no == 4){
	msg.no = 4;
	strcpy(msg.TYPE,"man use");
	send(connfd,&msg,sizeof(msg),0);//这条是发给服务区头的 下面也一样
	}
	else{
	msg.no = 3;
	strcpy(msg.TYPE,"cli use ");
	send(connfd,&msg,sizeof(msg),0);//这条是发给服务区头的 下面也一样
	}
	
}
void regist(int connfd){
	USER user;
	sysmsg msg;
	cout << "reg acc\n";
	cin.getline(user.name,100);
	cout << "reg pwd\n";
	cin.getline(user.passwd,100);
	send(connfd,&user,sizeof(user),0);//服务器那边等一个界面指令下面再读一些
	recv(connfd,&msg,sizeof(msg),0);
	cout << msg.TYPE << endl;
	bzero(&msg,sizeof(msg));
	msg.no = 1;//让服务器给一个登录界面
	strcpy(msg.TYPE,"I want login");
	send(connfd,&msg,sizeof(msg),0);//理论上万事 这个信号是给服务器的头里
}

void usermode(int connfd)
{	sysmsg msg;
	//recv(connfd,&msg,sizeof(msg),0);
	INFO inf;int key;
	cout << "name of demand\n";
	cin.getline(msg.TYPE,20);
	send(connfd,&msg,sizeof(msg),0);// 发送要查询的名字
	bzero(&inf,sizeof(inf));
	recv(connfd,&inf,sizeof(inf),0);//收到内容并显示
	if(inf.age == 0){
		cout <<"no user find \n";
	}
	else {
	cout << "name :"<< inf.name <<endl;
	cout << "addr :"<< inf.addr <<endl;
	cout << "no   :"<< inf.no <<endl;
	cout << "age  :"<< inf.age <<endl;
	cout << "job  :"<< inf.job <<endl;
	cout << "phone:"<< inf.phone <<endl;
	cout << "salar:"<< inf.salary <<endl;}
	bzero(&msg,sizeof(msg));
	cout << "/************************************/\n";
	cout << "		1.继续\n		2.注销\n		3.退出\n";
	cout << "/************************************/\n";
	scanf("%d",&key);cin.get();
	if(key == 1){
	msg.no = 3;
	strcpy(msg.TYPE,"cont\n");
	send(connfd,&msg,sizeof(msg),0);}
	else if(key == 2){
		msg.no = 5;	
		strcpy(msg.TYPE,"qt");
		send(connfd,&msg,sizeof(msg),0);flg--;
	}
	else if(key == 3){
		msg.no = 6;
		strcpy(msg.TYPE,"qt");
		send(connfd,&msg,sizeof(msg),0);
		exit(0);
	}
}
int manage(int connfd){
	sysmsg msg;
	INFO inf;
	cout << MATEXT <<endl; 
	scanf("%d",&msg.no);
	send(connfd,&msg,sizeof(msg),0);
//	getchar();
/*	switch(msg.no){
		case 1:
			strcpy(msg.TYPE,"ins\n");
			send(connfd,&msg,sizeof(msg),0);break;
		case 2:
			strcpy(msg.TYPE,"del\n");
			send(connfd,&msg,sizeof(msg),0);break;
		case 3:
			strcpy(msg.TYPE,"modi\n");
			send(connfd,&msg,sizeof(msg),0);break;
	}*/
	
	
	cout <<msg.TYPE <<endl;
	if(msg.no == 1){
		strcpy(msg.TYPE,"ins");
		cout << "*****put***** \nno|name|age|addr|phone|sal|job\n**********\n";
		scanf("%d",&inf.no);
		getchar();
		cout << "name\n";
		cin.getline(inf.name,N);	
		cout << "age\n";
		scanf("%d",&inf.age);	
		getchar();
		cout << "addr\n";
		cin.getline(inf.addr,N);	
		cout << "ph\n";
		cin.getline(inf.phone,N);	
		cout << "sal\n";
		scanf("%le",&inf.salary);	
		getchar();
		cout << "job\n";
		cin.getline(inf.job,N);	
		cout << "name :"<< inf.name <<endl;
		cout << "addr :"<< inf.addr <<endl;
		cout << "no   :"<< inf.no <<endl;
		cout << "age  :"<< inf.age <<endl;
		cout << "job  :"<< inf.job <<endl;
		cout << "phone:"<< inf.phone <<endl;
		cout << "salar:"<< inf.salary <<endl;
		send(connfd,&inf,sizeof(inf),0);
	}
	else if(msg.no == 2){
		fflush(stdin);
		bzero(&msg,sizeof(msg));
		cout << "which name del\n";
		cin.get();
		cin.getline(msg.TYPE,N);
		send(connfd,&msg,sizeof(msg),0);
	}
	else if(msg.no == 3){
		strcpy(msg.TYPE,"modi\n");
		cout << " put modi clu and newlis\n";
		bzero(&inf,sizeof(inf));
		cin.get();
		cout << " name\n";
		cin.getline(inf.job,100);
		cout << "culom\n";
		cin.getline(inf.name,100);
		cout << "new\n";
		cin.getline(inf.addr,100);
		send(connfd,&inf,sizeof(inf),0);
	}
	else if(msg.no == 4){
		bzero(&msg,sizeof(msg));
		msg.no = 5;
		strcpy(msg.TYPE,"quit");
		send(connfd,&msg,sizeof(msg),0);
		flg--;
		return 0;
	}
	bzero(&msg,sizeof(msg));
	msg.no = 4;
	strcpy(msg.TYPE,"re man\n");
	send(connfd,&msg,sizeof(msg),0);

	return 1;
}
