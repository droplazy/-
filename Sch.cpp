/************************************************************************
  > File Name: gettable.c
  > Author: ZhangJiaHao
  > Mail: 601321904@qq.com 
  > Created Time: 2019年11月14日 星期四 18时22分46秒
 ************************************************************************/

#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sqlite3.h>
#include<cmath>
#define N 1024
//int callback(void *arg,int size,char *_valu[],char *c_name);

using namespace std;
typedef unsigned int uint;
typedef struct{
	char name[N];
	char addr[N];
	int age;
	int no;
	double salary;
	char  phone[20];
	char job[N];
}INFO;
uint change(char* buf){
	char *p = buf;
	uint a = 0;
	int b = 0;
	int i = 0;
	//printf("debg%s\nss",buf);
	while( *p != '\0' ){
		i++;
		//cout << *p << " " ;
		p++;
	}
	p = buf;
	for(;i>0;i--){
		b = (*p) - 48;
		a += b * pow(10,i-1);
		p++;
	//	printf("a=%d\nb=%d",a,b);
//		usleep(500);
	}
//	cout << a << endl;
	return a;
}
INFO ssch(char *buf)
{
	sqlite3 *db = NULL;
	INFO info;
	int ret;
	char sql[256] = "\0";
	char *errmsg = NULL;
	char **res = NULL;
	int row,ncol;
	ret = sqlite3_open("info.db",&db);
	if(0 != ret){
	fprintf(stderr,"er open sql :%s\n",sqlite3_errmsg(db)); return info;}

	sprintf(sql,"select * from ygg");
	ret = sqlite3_get_table(db,sql,&res,&row,&ncol,&errmsg);
	if(0 != ret){
	fprintf(stderr,"select :%s\n",errmsg);return info;}
	int flg = 0;
	for(int i=0;i<((row+1)*ncol);i++)
	{
		if(!strncasecmp(buf,*(res+i),4))
		{	
			strcpy(info.name,*(res+i));
			strcpy(info.addr,*(res+i+1));
	//		printf("ag:%s\n",*(res+i+2));
			info.age = change(*(res+i+2));
	//		printf("no%s\n",*(res+i+3));
			info.no = change(*(res+i+3));
	//		printf("sa:%s\n",*(res+i+4));
			info.salary = change(*(res+i+4));
	//		printf("ph:%s\n",*(res+i+5));
			strcpy(info.phone,*(res+i+5));
			strcpy(info.job,*(res+i+6));
			break;
		
		}
		flg++;
			}
	if(flg ==((row+1)*ncol))
		cout << " err name\n";
	sqlite3_free_table(res);
	sqlite3_close(db);
	return info;

} 

/*
int main(){
	char p[]="zhan";
	INFO jk;
	jk = ssch(p);
//	char k[]="15123212332";
//	int b = change(k);
//	cout << b << endl;;
	cout <<"name:"<< jk.name << endl;
	cout << "add:"<<jk.addr << endl;
	cout << "ag"<<jk.age << endl;
	cout << "job:"<<jk.job << endl;
	cout << "no"  <<jk.no << endl;
	cout << "ph"<<jk.phone << endl;
	cout << "sa" <<jk.salary << endl;		
}
*/

