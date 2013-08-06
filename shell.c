/* Vishrut Mehta
   201102128*/

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>
#include<string.h>
#include <sys/wait.h>
#include<fcntl.h>
#include<limits.h>
/********************************** Defined a structure **************************************************/
int j=0,h=0,hh=0,h3=0,bc=0,bb=0,don=0,fk=0,fp=0;
typedef struct vish{
	char process[100];
	char nam[100];
	char forbc[100];
	int pno;
	int state;
}vish;
typedef struct node{
	char *pcommand[100];
}multiplecmd;
multiplecmd pcarray[1000];
vish vv[1000];
int superf=0;
pid_t pid;
//int pfds[100];
int fl=0,pcmd_cnt=0;
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
		//	if(superf==0)
		//	{
		printf("\n%s %d exited normally\n",vv[bb++].forbc,pid);
		//	}
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
int parse(char str[],char *kok[]){
	//to diff
	int c_cnt=0,pc=0,index=-1,i=0,pcommand_cnt=0,lt=0,gt=0;
	char *new1=strtok(str," ");
	while(new1!=NULL)
	{
		if(strcmp("|",new1)==0 || strcmp("<",new1)==0 || strcmp(">",new1)==0)
		{
			if(strcmp("|",new1)==0)
			{pc++;}
			pcommand_cnt=0;
			for(i=index+1;i<c_cnt;i++)
				pcarray[pcmd_cnt].pcommand[pcommand_cnt++]=kok[i];
			pcarray[pcmd_cnt++].pcommand[pcommand_cnt++]=(char *)NULL;
			index=c_cnt;
		}
		kok[c_cnt++]=new1;
		new1=strtok(NULL," ");
	}
	if(pcmd_cnt>0){
		pcommand_cnt=0;
		for(i=index+1;i<c_cnt;i++)
			pcarray[pcmd_cnt].pcommand[pcommand_cnt++]=kok[i];
		pcarray[pcmd_cnt++].pcommand[pcommand_cnt++]=(char *)NULL;
	}
	kok[c_cnt]=(char *)NULL;
	return pc;
}
int ltcnt(char str2[]){
	//to diff
	int lt=0;
	char *new2=strtok(str2," ");
	while(new2!=NULL)
	{
		if(strcmp("<",new2)==0)
		{
			lt++;
		}
		new2=strtok(NULL," ");
	}
	return lt;
}

int gtcnt(char str3[]){
	//to diff
	int gt=0;
	char *new3=strtok(str3," ");
	while(new3!=NULL)
	{
		if(strcmp(">",new3)==0)
		{
			gt++;
		}
		new3=strtok(NULL," ");
	}
	return gt;
}
int main(int argc,char **argv)
{
	int mo,no=0,lo=0;
	int knw=0;
	char new[100];
	char *kok[100];
	char *k[100];
	int debug=0;
	char str[100],str1[100],str2[100],str3[100],str4[100];
	while(1)
	{

		signal(SIGINT,  sig_handler);
		signal(SIGTSTP, sig_handler);
		signal(SIGCHLD,SIG_IGN);
		//		if(superf==0)
		signal(SIGCHLD, child_handler);
		int flag=0;
		char * pch;
		int i=0,status,r,ii=0;
		int gl=0;
		char c;
		if(knw==0)
		{
			superf=0;
			prompt();
			strcpy(str,"\0");
			scanf("%[^\n]",str);
			scanf("%c",&c);
			debug++;
			strcpy(str2,str);
			//		printf("%d\nsexxxxxx\n",debug);
			strcpy(str1,str);
			strcpy(str3,str);
			pcmd_cnt=0;
			int pfds[100][2];
			int tp=0,lt=0,gt=0;
			tp=parse(str,kok);
			lt=ltcnt(str2);
			gt=gtcnt(str3);
			if(lt>0 || gt>0 || tp>0)
				gl=1;
			printf("--%d--%d--%d",tp,lt,gt);
			printf("the value of lt is :%d\n",lt);
			/****************************** when we just type enter *******************************************************/
			if(strcmp(str,"\0")==0)
			{
				continue;
			}
			else if(tp>0 && lt==0 && gt==0)//else if
			{
				printf("pipe ma");
				int ij=0;
				for(i=0;i<pcmd_cnt;i++){
					for(ij=0;pcarray[i].pcommand[ij]!=NULL;ij++)
					{
						printf("%s ",pcarray[i].pcommand[ij]);
					}
					printf("\n");
				}
				int in,out;
				gl=1;i=0;
				superf=1;
				for(i=0;i<tp;i++){
					pipe((pfds+i)[1]);
					pipe((pfds+i)[0]);
				}
				//	printf("the pcmd_cnt is :%d",pcmd_cnt);
				i=0;
				int jjj;
				int myfl=0;
				while(i<=tp)//tp
				{
					pid=fork();
					if(pid==0)//child process
					{
						if(i==0)
						{
							dup2(pfds[i][1],1);
						}
						else if(i>0 && i<tp)//tp
						{
							dup2(pfds[i-1][0],0);
							dup2(pfds[i][1],1);
						}
						else if(i==tp)
						{
							dup2(pfds[i-1][0],0);
						}
						for(jjj=0;jjj<=i;jjj++)
						{	
							close(pfds[jjj][0]);
							close(pfds[jjj][1]);
						}
						printf("the value for pcmd_cnt:%d \n",pcmd_cnt);
						//	printf("c is %s\n",pcarray[i].pcommand[0]);
						execvp(pcarray[i].pcommand[0],pcarray[i].pcommand);
					}
					else{
						for(jjj=0;jjj<i;jjj++)
						{
							close(pfds[jjj][0]);
							close(pfds[jjj][1]);
						}

					}
					i++;
				}
				for(i=0;i<=tp;i++)
					wait(NULL);
				//strcpy(str,"");*/
			}
			else if(tp>0)//else if
			{
				printf("the value for pcmd_cnt:%d \n",pcmd_cnt);
				printf("both pipe ma");
				int ij=0;
				for(i=0;i<pcmd_cnt;i++){
					for(ij=0;pcarray[i].pcommand[ij]!=NULL;ij++)
					{
						printf("%s ",pcarray[i].pcommand[ij]);
					}
					printf("\n");
				}
				int in,out;
				gl=1;i=0;
				superf=1;
				for(i=0;i<tp;i++){
					pipe((pfds+i)[1]);
					pipe((pfds+i)[0]);
				}
				//	printf("the pcmd_cnt is :%d",pcmd_cnt);
				if(gt>0)//out
				{
					out = open(pcarray[pcmd_cnt-1].pcommand[0], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
					//	dup2(out,1);
				}
				if(lt>0)//in
				{
					in = open(pcarray[1].pcommand[0], O_RDONLY);
					//	dup2(in,0);
				}
				i=0;int dusraa=0;
				int jjj;
				int myfl=0;
				while(i<pcmd_cnt)//tp
				{
					if((i==1 && lt>0) || (i==pcmd_cnt-1 && gt>0)){
						dusraa++;
					//	printf("iff%d\n",dusraa);
					}	
					else{
						pid=fork();
						if(pid==0)//child process
						{
							if(lt>0 && i==0)//
							{
								printf("1st->%d\n",i);
								dup2(pfds[i][1],1);
								close(pfds[i][1]);
								close(pfds[i][0]);
								dup2(in,0);
								close(in);
					//			myfl=1;
							}
							else if(i==0)
							{
								printf("2nd->%d\n",i);

								dup2(pfds[i][1],1);
								close(pfds[i][1]);
							}
							else if(i>0 && i<pcmd_cnt-2)//tp
							{
								printf("3rd->%d\n",i);
							//	if(lt>0)
							//	{
							///		dup2(in,0);
							//		close(in,0);
							//		dup2(pfds[i][1],1);
							//		close(pfds[i][1]);
							//	}
							//	else{
								if(lt>0){
									dup2(in,0);
									dup2(pfds[i][1],1);
									close(in);
									close(pfds[i][1]);
								}
							//	}
							}
						//	else if(i==pcmd_cnt-1){//tp
						//		dup2(pfds[i-1][0],0);
						//		close();
						//	}
							if(gt>0 && i==pcmd_cnt-2)//
							{
							//	printf("%d\n",dusraa);
								printf("4th-->>>%dghuss\n",i);
								//if(lt>0){
								dup2(pfds[i-2][0],0);
							//	dup2(pfds[i-2][0],1);
								close(pfds[i-2][0]);
								close(pfds[i-2][1]);
								dup2(out,1);
								close(out);
							//	}
							/*	else{
							//	myfl=1;
									dup2(pfds[i-1][0],0);
									dup2(pfds[i][1],1);
									close(pfds[i][1]);
									close(pfds[i-1][0]);
									close(pfds[i-1][1]);
								}*/
							}
							execvp(pcarray[i].pcommand[0],pcarray[i].pcommand);
						}
						else{
							if(in>0)
								close(in);
							if(out>0)
								close(out);
						//	if(lt==0){
						//		if(gt>0 && i==pcmd_cnt-2){
						//		close(pfds[i-1][0]);
						//		close(pfds[i-1][1]);
						//		}
						//	close(pfds[i][0]);
						//	close(pfds[i][1]);
						//	}else
							 if(i==pcmd_cnt-2 && gt>0)
							{
								close(pfds[0][0]);
								close(pfds[0][1]);
							}

						}
					}
					i++;
				}
				for(i=0;i<=tp;i++)
					wait(NULL);
			}
			else if(lt>0 && tp==0 && gt==0)
			{
				//	gl=1;
				i=0;int ij;
				printf("in lt\n");
				for(i=0;i<pcmd_cnt;i++){
					for(ij=0;pcarray[i].pcommand[ij]!=NULL;ij++)
					{
						printf("%s ",pcarray[i].pcommand[ij]);
					}
					printf("\n");
				}
				int in;
				in = open(pcarray[1].pcommand[0], O_RDONLY);
				pid=fork();
				if(pid==0)//child
				{
					dup2(in,0);
					close(in);
					printf("$$$%s$$$$%s$$$",pcarray[0].pcommand[0],pcarray[0].pcommand[1]);
					execvp(pcarray[0].pcommand[0],pcarray[0].pcommand);
				}//parent
				else{
					printf("**%s***",pcarray[1].pcommand[0]);
					//dup2(in,1);
					wait(NULL);
					close(in);
				}
			}
			else if(gt>0 && tp==0 && lt==0)
			{
				//	gl=1;
				i=0;int ij;
				printf("in gt\n");
				for(i=0;i<pcmd_cnt;i++){
					for(ij=0;pcarray[i].pcommand[ij]!=NULL;ij++)
					{
						printf("%s ",pcarray[i].pcommand[ij]);
					}
					printf("\n");
				}
				int out;
				out = open(pcarray[1].pcommand[0], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
				pid=fork();
				if(pid==0)//child
				{
					dup2(out,1);
					close(out);
					printf("$$$%s$$$$%s$$$",pcarray[0].pcommand[0],pcarray[0].pcommand[1]);
					execvp(pcarray[0].pcommand[0],pcarray[0].pcommand);
				}//parent
				else{
					printf("**%s***",pcarray[1].pcommand[0]);
					//dup2(in,1);
					wait(NULL);
					close(out);
				}
			}
			else if(gt>0 && lt>0 && tp==0)
			{
				i=0;int ij;
				printf("in gt nd lt\n");
				for(i=0;i<pcmd_cnt;i++){
					for(ij=0;pcarray[i].pcommand[ij]!=NULL;ij++)
					{
						printf("%s ",pcarray[i].pcommand[ij]);
					}
					printf("\n");
				}
				int out,in;
				out = open(pcarray[2].pcommand[0], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
				in = open(pcarray[1].pcommand[0], O_RDONLY);
				pid=fork();
				if(pid==0)//child
				{
					dup2(in,0);
					dup2(out,1);
					close(in);
					close(out);
					printf("$$$%s$$$$%s$$$\n",pcarray[0].pcommand[0],pcarray[0].pcommand[1]);
					execvp(pcarray[0].pcommand[0],pcarray[0].pcommand);
				}
				else{
					printf("**%s***\n",pcarray[1].pcommand[0]);
					//dup2(in,1);
					wait(NULL);
					close(out);
					close(in);

				}

			}
			i=0;ii=0;
			strcpy(new,str1);
			pch = strtok (str1," \t");
			while (pch != NULL)
			{
				flag++;
				k[i++]=pch;
				pch = strtok (NULL, " \t");
			}
			k[i]=(char *)NULL;
			if(k[0][1]!='!')
				strcpy(vv[fp].nam,new);
			//		printf("lol");
		}
		int ret;
		if(gl==0)
		{
			printf("rest ma");
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
				printf("haan");
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
	}
	return 0;
}
