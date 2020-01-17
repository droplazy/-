/************************************************************************
  > File Name: gettable.c
  > Author: ZhangJiaHao
  > Mail: 601321904@qq.com 
  > Created Time: 2019年11月14日 星期四 18时22分46秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sqlite3.h>

//int callback(void *arg,int size,char *_valu[],char *c_name);
char *search(char *buf)
{
	sqlite3 *db = NULL;
	int ret;
	char sql[256] = "\0";
	char *errmsg = NULL;
	char **res = NULL;
	int row,ncol;
	static char pwd[30]="";
	ret = sqlite3_open("info.db",&db);
	if(0 != ret){
	fprintf(stderr,"er open sql :%s\n",sqlite3_errmsg(db)); return 0;}

	sprintf(sql,"select * from login");
	ret = sqlite3_get_table(db,sql,&res,&row,&ncol,&errmsg);
	if(0 != ret){
	fprintf(stderr,"select :%s\n",errmsg);return 0;}

	for(int i=0;i<((row+1)*ncol);i++)
	{
		if(!strcmp(buf,*(res+i)))
		{	
			strcpy(pwd,*(res+i+1));}
		//	printf("%s",pwd);break;}
	/*	printf("%-12s\t",*(res+i));
		if((i+1)%ncol == 0)
			printf("\n");*/
	}
	sqlite3_free_table(res);
	sqlite3_close(db);
	return pwd;

} 




