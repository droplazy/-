/************************************************************************
    > File Name: addSQL.c
    > Author: ZhangJiaHao
    > Mail: 601321904@qq.com 
    > Created Time: 2019年11月14日 星期四 17时21分09秒
************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sqlite3.h>
#include<string.h>
typedef unsigned int uint;
uint change(char* buf);

int addSQL(char *acc,char *password)
{
	sqlite3 *db = NULL;
	int ret;
	char sql[120]="\0";
	char *errmsg = NULL;
	printf("a a a \n");
	ret = sqlite3_open("info.db",&db);
	if(ret != 0){
	fprintf(stderr,"sqlite3_open : %s\n",sqlite3_errmsg(db));exit(-1);}

	sprintf(sql,"insert into login(acc,pwd) values('%s','%s')",acc,password);
	ret = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
	if(ret != 0){
	fprintf(stderr,"insert: %s\n",sqlite3_errmsg(db));exit(-1);}
	return 0;
}
int del(char* name)
{
	sqlite3 *db = NULL;
	int ret;
	char sql[120]="\0";
	char *errmsg = NULL;
	printf("dd \n");
	ret = sqlite3_open("info.db",&db);
	if(ret != 0){
	fprintf(stderr,"sqlite3_open : %s\n",sqlite3_errmsg(db));exit(-1);}

	sprintf(sql,"delete from ygg where name='%s'",name);
	ret = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
	if(ret != 0){
	printf("ierr deleted!\n");
	fprintf(stderr,"insert: %s\n",sqlite3_errmsg(db));exit(-1);}
	else 
		printf("deleted\n");
	return 0;
}
int modi(char* name,char* dest,char*nnwe)
{
	sqlite3 *db = NULL;
	int ret;
	char sql[120]="\0";
	char *errmsg = NULL;
	printf("dd \n");
	ret = sqlite3_open("info.db",&db);
	if(ret != 0){
	fprintf(stderr,"sqlite3_open : %s\n",sqlite3_errmsg(db));exit(-1);}
	if(!strcmp(dest,"name") && !strcmp(dest,"addr") && !strcmp(dest,"phone") && !strcmp(dest,"job")){
	
	sprintf(sql,"update ygg set '%s'='%s' where name='%s'",dest,nnwe,name);}
	else {
		int tmp =0 ;
		tmp = change(nnwe);
	 	sprintf(sql,"update ygg set '%s'='%d' where name='%s'",dest,tmp,name);}
	ret = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
	if(ret != 0){
		printf("err up\n");
	fprintf(stderr,"insert: %s\n",sqlite3_errmsg(db));exit(-1);}
	return 0;
}
