/************************************************************************
    > File Name: addSQL.c
    > Author: ZhangJiaHao
    > Mail: 601321904@qq.com 
    > Created Time: 2019年11月14日 星期四 17时21分09秒
************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<iostream>
#include<stdlib.h>
#include<sqlite3.h>
#define N 1024
typedef struct{
	char name[N];
	char addr[N];
	int age;
	int no;
	double salary;
	char phone[20];
	char job[N];
}INFO;
int addinfo(INFO inf)
{
	sqlite3 *db = NULL;
	int ret;
	char sql[1200]="\0";
	char *errmsg = NULL;
	printf("a a a \n");
	ret = sqlite3_open("info.db",&db);
	if(ret != 0){
	fprintf(stderr,"sqlite3_open : %s\n",sqlite3_errmsg(db));exit(-1);}

	sprintf(sql,"insert into ygg(name,addr,age,no,salary,phone,job) values('%s','%s','%d','%d','%e','%s','%s')"
			,inf.name,inf.addr,inf.age,inf.no,inf.salary,inf.phone,inf.job);

	ret = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
	if(ret != 0){
	fprintf(stderr,"insert: %s\n",sqlite3_errmsg(db));
	printf("err ins");
	return 1;
	}
	else 
		std::cout << "ins success\n";

	return 0;
}
