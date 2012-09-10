#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>
#include<string.h>
#include <sys/wait.h>
/********************************** Defined a structure **************************************************/
int j=0,h=0,hh=0,h3=0,bc=0,bb=0,don=0,fk=0,fp=0;
typedef struct vish{
	char process[100];
	char nam[100];
	char forbc[100];
	int pno;
	int state;
}vish;
vish vv[1000];
int fl=0;
/*********************************** Get Prompt ************************************************************/
void prompt()
{
	char *name,*path;
	name=getenv("USER");
	path=getenv("HOSTNAME");
	int len;
	char cwd[1000];
	char *home;
	char *gg;
	gg=getenv("PWD");
	char *chr=getcwd(cwd,1024*(sizeof(char)));
	home=getenv("HOME");
	len=strlen(home);
	chr[len-1]='~';
	chr=chr+len-1;
	printf("<%s@%s:%s>",name,path,chr);	
}
/************************************ To handle SIGCHLD signal ************************************************/
void child_handler(int signum){
	int pid;
	pid = waitpid(WAIT_ANY, NULL, WNOHANG);
	//	if(pid != -1) //&& (fk==1 || don==1))
	if(pid>0)
	{
		printf("\n%s %d exited normally\n",vv[bb++].forbc,pid);
		prompt();
	}
	fflush(stdout);
	signal(SIGCHLD, child_handler);
	return;
}
/************************************* To handle other signals *************************************************/
void sig_handler(int signum){
	if(signum == 2 || signum == 20){
		//	printf("\nEnter \"quit\" to terminate the program\n");
	}
	signal(SIGINT, sig_handler);
	signal(SIGTSTP, sig_handler);
	return;
}

int main(int argc,char **argv)
{
	pid_t pid;
	int knw=0;
	char new[100];
	char *k[100];
	char str[100];
	while(1)
	{

		signal(SIGINT,  sig_handler);
		signal(SIGTSTP, sig_handler);
		signal(SIGCHLD,SIG_IGN);
		signal(SIGCHLD, child_handler);
		int flag=0;
		char * pch;
		int i=0,status,r;
		char c;
		if(knw==0)
		{
			prompt();
			strcpy(str,"\0");
			scanf("%[^\n]",str);
			scanf("%c",&c);
/****************************** when we just type enter *******************************************************/
			if(strcmp(str,"\0")==0)
			{
				continue;
			}
			strcpy(new,str);
			pch = strtok (str," \t");
			while (pch != NULL)
			{
				flag++;
				k[i++]=pch;   
				pch = strtok (NULL, " \t");
			}
			k[i]=(char *)NULL;
			if(k[0][1]!='!')
				strcpy(vv[fp].nam,new);
		}
		int ret;
/**********************************  For command "pid all" ****************************************************/
		if(strcmp(k[0],"pid")==0 && flag>1 && strcmp(k[1],"all")==0)
		{
			int jug;
			printf("List of all processes spawned from this shell:\n");
			for(jug=0;jug<h;jug++)		
				printf("command name:%s process id : %d\n",vv[jug].process,vv[jug].pno);
			knw=0;
		}
/*********************************** For command "pid current" *************************************************/
		else if(strcmp(k[0],"pid")==0 && flag>1 && strcmp(k[1],"current")==0)
		{
			int mug;
			printf("List of currently executing processes spawned from this shell:\n");
			for(mug=0;mug<h;mug++)		
			{
				if(vv[mug].state==1)
				{
					printf("command name:%s process id : %d\n",vv[mug].process,vv[mug].pno);
				}
			}
			knw=0;
		}
/************************************  For comand "quit"  *********************************************************/
		else if(strcmp(k[0],"quit")==0)
		{
			knw=0;
			break;break;break;break;break;break;break;break;break;
			exit(1);
		}
/************************************ For command "hist" ************************************************************/
		else if(strcmp(k[0],"hist")==0)
		{
			for(r=0;r<fp;r++)
			{	
				printf("%d. %s\n",r+1,vv[r].nam);
			}
			knw=0;
		}
/************************************ For command "histn" ************************************************************/
		else if(k[0][0]=='h' && k[0][1]=='i' && k[0][2]=='s' && k[0][3]=='t')
		{
			int x=k[0][4]-'0';
			for(r=0;r<x;r++)
				printf("%d. %s\n",r+1,vv[r].nam);
			knw=0;
		}
/************************************ For command "!histn" ************************************************************/
		else if(k[0][0]=='!')
		{
			int y=k[0][5]-'0';
			y=y-1;
			printf("%s\n",vv[y].nam);
			strcpy(k[0],vv[y].nam);
			strcpy(vv[fp].nam,k[0]);
			knw=1;
		}
/************************************** For command "pid" **************************************************************/
		else if(strcmp(k[0],"pid")==0)
		{
			knw=0;
			printf("commond name:%s process id:%d\n",argv[0],(int)getpid());
		}
/*************************************** For command "cd" **************************************************************/
		else if(strcmp(k[0],"cd")==0)
		{
			knw=0;
			if(chdir(k[1])==-1)
				printf("Error:No such file or directory\n");
		}
/**************************************** For Background processes ****************************************************/
		else if(flag>1 && k[1][0]=='&')
		{
			knw=0;
			pid=fork();
			if(pid==0)
			{
				ret=execvp(k[0],k);
				if(ret==-1)
					printf("This command does not exists!!\n");
			}
			else{
				strcpy(vv[bc++].forbc,k[0]);
				vv[h3++].state=1;
				strcpy(vv[hh++].process,k[0]);
				vv[h++].pno=pid;
				printf("command %s pid %d\n",k[0],pid);
			}

		}
/******************************************* For Foreground processes **********************************************/
		else		
		{
			knw=0;
			pid=fork();
			if(pid==0)
			{
				ret=execvp(k[0],k);
				if(ret==-1)
					printf("This command does not exists!!\n");
			}
			else
			{
				vv[h3++].state=0;
				strcpy(vv[hh++].process,k[0]);
				vv[h++].pno=pid;
				wait(NULL);
			}
		}
		fp++;
	}
	return 0;
}
