#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#define N 150//每行的最大长度-1
#define M 300//最大符号数

//The Parser Module
int hasMoreCommands(FILE *fp,char command[N])//-1误，0正常
{
	if(fgets(command,N-1,fp)==NULL)
		return -1;
	else return 0;
}
void commandType(char command[N],char *type)
{
	char temp[3]={'\0'};
	temp[0]=command[0];
	temp[1]=command[1];
	if(!strcmp(temp,"pu"))
		strcpy(type,"C_PUSH");
	else if(!strcmp(temp,"po"))
		strcpy(type,"C_POP");
	else if(!strcmp(temp,"la"))
		strcpy(type,"C_LABEL");
	else if(!strcmp(temp,"go"))
		strcpy(type,"C_GOTO");
	else if(!strcmp(temp,"if"))
		strcpy(type,"C_IF");
	else if(!strcmp(temp,"fu"))
		strcpy(type,"C_FUNCTION");
	else if(!strcmp(temp,"re"))
		strcpy(type,"C_RETURN");
	else if(!strcmp(temp,"ca"))
		strcpy(type,"C_CALL");
	else strcpy(type,"C_ARITHMETIC");
}
int CutString(char command[N],int i)//以空格为界切断代码，并取出第i段起始偏移(首字母)
{
	int k;
	if(command[0]!=' ')i--;
	for(k=0;k<(N-1)&&i>0&&command[k+1]!='\0'&&command[k+1]!='\n';k++)
	{
		if(command[k]==' '&&command[k+1]!=' ')
			i--;
	}
	return k;
}
void arg1(char command[N],char *argone)
{
	int i;
	char type[15];
	commandType(command,type);
	if(type[2]=='A')
	{
		for(i=CutString(command,1);command[i]!=' '&&command[i]!='\0'&&command[i]!='\n';i++)
		{
			argone[i-CutString(command,1)]=command[i];
		}
		argone[i-CutString(command,1)]='\0';
	}
	else
	{
		for(i=CutString(command,2);command[i]!=' '&&command[i]!='\0'&&command[i]!='\n';i++)
		{
			argone[i-CutString(command,2)]=command[i];
		}
		argone[i-CutString(command,2)]='\0';
	}
}
int arg2(char command[N])
{
	char temp[10];
	int i;
	for(i=CutString(command,3);command[i]!=' '&&command[i]!='\0'&&command[i]!='\n';i++)
	{
		temp[i-CutString(command,3)]=command[i];
	}
	temp[i-CutString(command,3)]='\0';
	return atoi(temp);
}

//The CodeWriter Module
void WriteArithmetic(char command[N],FILE *fp)
{
	char arg[15];
	int argp=0;
	static int logicindex=0;
	arg1(command,arg);
	if(!strcmp(arg,"add"))
	{
		fprintf(fp,"@SP\n");
		fprintf(fp,"A=M-1\n");
		fprintf(fp,"D=M\n");
		fprintf(fp,"A=A-1\n");
		fprintf(fp,"M=D+M\n");
		fprintf(fp,"@SP\n");
		fprintf(fp,"M=M-1\n");
	}
	if(!strcmp(arg,"sub"))
	{
		fprintf(fp,"@SP\n");
		fprintf(fp,"A=M-1\n");
		fprintf(fp,"D=-M\n");
		fprintf(fp,"A=A-1\n");
		fprintf(fp,"M=D+M\n");
		fprintf(fp,"@SP\n");
		fprintf(fp,"M=M-1\n");
	}
	if(!strcmp(arg,"neg"))
	{
		fprintf(fp,"@SP\n");
		fprintf(fp,"A=M-1\n");
		fprintf(fp,"M=-M\n");
	}
	if(!strcmp(arg,"eq"))
	{
		fprintf(fp,"@SP\n");
		fprintf(fp,"A=M-1\n");
		fprintf(fp,"D=-M\n");
		fprintf(fp,"A=A-1\n");
		fprintf(fp,"D=D+M\n");
		fprintf(fp,"M=D\n");
		fprintf(fp,"M=D-M\n");
		fprintf(fp,"@SP\n");
		fprintf(fp,"M=M-1\n");
		fprintf(fp,"@END_%d\n",logicindex);
		fprintf(fp,"D;JNE\n");//if false remain 0
		fprintf(fp,"@SP\n");
		fprintf(fp,"A=M-1\n");
		fprintf(fp,"M=!M\n");
		fprintf(fp,"(END_%d)\n",logicindex);				
		logicindex++;
	}
	if(!strcmp(arg,"gt"))
	{
		fprintf(fp,"@SP\n");
		fprintf(fp,"A=M-1\n");
		fprintf(fp,"D=-M\n");
		fprintf(fp,"A=A-1\n");
		fprintf(fp,"D=D+M\n");
		fprintf(fp,"M=D\n");
		fprintf(fp,"M=D-M\n");
		fprintf(fp,"@SP\n");
		fprintf(fp,"M=M-1\n");
		fprintf(fp,"@END_%d\n",logicindex);
		fprintf(fp,"D;JLE\n");//if false remain 0
		fprintf(fp,"@SP\n");
		fprintf(fp,"A=M-1\n");
		fprintf(fp,"M=!M\n");
		fprintf(fp,"(END_%d)\n",logicindex);				
		logicindex++;
	}
	if(!strcmp(arg,"lt"))
	{
		fprintf(fp,"@SP\n");
		fprintf(fp,"A=M-1\n");
		fprintf(fp,"D=-M\n");
		fprintf(fp,"A=A-1\n");
		fprintf(fp,"D=D+M\n");
		fprintf(fp,"M=D\n");
		fprintf(fp,"M=D-M\n");
		fprintf(fp,"@SP\n");
		fprintf(fp,"M=M-1\n");
		fprintf(fp,"@END_%d\n",logicindex);
		fprintf(fp,"D;JGE\n");//if false remain 0
		fprintf(fp,"@SP\n");
		fprintf(fp,"A=M-1\n");
		fprintf(fp,"M=!M\n");
		fprintf(fp,"(END_%d)\n",logicindex);				
		logicindex++;
	}
	if(!strcmp(arg,"and"))
	{
		fprintf(fp,"@SP\n");
		fprintf(fp,"A=M-1\n");
		fprintf(fp,"D=M\n");
		fprintf(fp,"A=A-1\n");
		fprintf(fp,"M=D&M\n");
		fprintf(fp,"@SP\n");
		fprintf(fp,"M=M-1\n");
	}
	if(!strcmp(arg,"or"))
	{
		fprintf(fp,"@SP\n");
		fprintf(fp,"A=M-1\n");
		fprintf(fp,"D=M\n");
		fprintf(fp,"A=A-1\n");
		fprintf(fp,"M=D|M\n");
		fprintf(fp,"@SP\n");
		fprintf(fp,"M=M-1\n");
	}
	if(!strcmp(arg,"not"))
	{
		fprintf(fp,"@SP\n");
		fprintf(fp,"A=M-1\n");
		fprintf(fp,"M=!M\n");
	}
}
void WritePushPop(char command[N],FILE *fp,char *type,char *filename)
{
	char arg[15];
	int argp=0;
	arg1(command,arg);
	argp=arg2(command);
	if(type[3]=='U')
	{
		if(!strcmp(arg,"constant"))
		{
			fprintf(fp,"@%d\n",argp);
			fprintf(fp,"D=A\n");
			fprintf(fp,"@SP\n");
			fprintf(fp,"A=M\n");
			fprintf(fp,"M=D\n");
			fprintf(fp,"@SP\n");
			fprintf(fp,"M=M+1\n");
		}
		else if(!strcmp(arg,"local"))
		{
			fprintf(fp,"@LCL\n");
			fprintf(fp,"D=M\n");
			fprintf(fp,"@%d\n",argp);
			fprintf(fp,"A=D+A\n");
			fprintf(fp,"D=M\n");
			fprintf(fp,"@SP\n");
			fprintf(fp,"A=M\n");
			fprintf(fp,"M=D\n");
			fprintf(fp,"@SP\n");
			fprintf(fp,"M=M+1\n");
		}
		else if(!strcmp(arg,"argument"))
		{
			fprintf(fp,"@ARG\n");
			fprintf(fp,"D=M\n");
			fprintf(fp,"@%d\n",argp);
			fprintf(fp,"A=D+A\n");
			fprintf(fp,"D=M\n");
			fprintf(fp,"@SP\n");
			fprintf(fp,"A=M\n");
			fprintf(fp,"M=D\n");
			fprintf(fp,"@SP\n");
			fprintf(fp,"M=M+1\n");
		}
		else if(!strcmp(arg,"this"))
		{
			fprintf(fp,"@THIS\n");
			fprintf(fp,"D=M\n");
			fprintf(fp,"@%d\n",argp);
			fprintf(fp,"A=D+A\n");
			fprintf(fp,"D=M\n");
			fprintf(fp,"@SP\n");
			fprintf(fp,"A=M\n");
			fprintf(fp,"M=D\n");
			fprintf(fp,"@SP\n");
			fprintf(fp,"M=M+1\n");
		}
		else if(!strcmp(arg,"that"))
		{
			fprintf(fp,"@THAT\n");
			fprintf(fp,"D=M\n");
			fprintf(fp,"@%d\n",argp);
			fprintf(fp,"A=D+A\n");
			fprintf(fp,"D=M\n");
			fprintf(fp,"@SP\n");
			fprintf(fp,"A=M\n");
			fprintf(fp,"M=D\n");
			fprintf(fp,"@SP\n");
			fprintf(fp,"M=M+1\n");
		}
		else if(!strcmp(arg,"pointer"))
		{
			fprintf(fp,"@%d\n",3+argp);
			fprintf(fp,"D=M\n");
			fprintf(fp,"@SP\n");
			fprintf(fp,"A=M\n");
			fprintf(fp,"M=D\n");
			fprintf(fp,"@SP\n");
			fprintf(fp,"M=M+1\n");
		}
		else if(!strcmp(arg,"temp"))
		{
			fprintf(fp,"@%d\n",5+argp);
			fprintf(fp,"D=M\n");
			fprintf(fp,"@SP\n");
			fprintf(fp,"A=M\n");
			fprintf(fp,"M=D\n");
			fprintf(fp,"@SP\n");
			fprintf(fp,"M=M+1\n");		
		}
		else if(!strcmp(arg,"static"))
		{
			fprintf(fp,"@%s.%d\n",filename,argp);
			fprintf(fp,"D=M\n");
			fprintf(fp,"@SP\n");
			fprintf(fp,"A=M\n");
			fprintf(fp,"M=D\n");
			fprintf(fp,"@SP\n");
			fprintf(fp,"M=M+1\n");
		}
	}
	else//pop
	{
		if(!strcmp(arg,"local"))
		{
			fprintf(fp,"@LCL\n");
			fprintf(fp,"D=M\n");
			fprintf(fp,"@%d\n",argp);
			fprintf(fp,"D=D+A\n");
			fprintf(fp,"@SP\n");
			fprintf(fp,"A=M\n");
			fprintf(fp,"M=D\n");
			fprintf(fp,"A=A-1\n");
			fprintf(fp,"D=M\n");
			fprintf(fp,"A=A+1\n");
			fprintf(fp,"A=M\n");
			fprintf(fp,"M=D\n");
			fprintf(fp,"@SP\n");
			fprintf(fp,"M=M-1\n");	
		}
		else if(!strcmp(arg,"argument"))
		{
			fprintf(fp,"@ARG\n");
			fprintf(fp,"D=M\n");
			fprintf(fp,"@%d\n",argp);
			fprintf(fp,"D=D+A\n");
			fprintf(fp,"@SP\n");
			fprintf(fp,"A=M\n");
			fprintf(fp,"M=D\n");
			fprintf(fp,"A=A-1\n");
			fprintf(fp,"D=M\n");
			fprintf(fp,"A=A+1\n");
			fprintf(fp,"A=M\n");
			fprintf(fp,"M=D\n");
			fprintf(fp,"@SP\n");
			fprintf(fp,"M=M-1\n");	
		}
		else if(!strcmp(arg,"this"))
		{
			fprintf(fp,"@THIS\n");
			fprintf(fp,"D=M\n");
			fprintf(fp,"@%d\n",argp);
			fprintf(fp,"D=D+A\n");
			fprintf(fp,"@SP\n");
			fprintf(fp,"A=M\n");
			fprintf(fp,"M=D\n");
			fprintf(fp,"A=A-1\n");
			fprintf(fp,"D=M\n");
			fprintf(fp,"A=A+1\n");
			fprintf(fp,"A=M\n");
			fprintf(fp,"M=D\n");
			fprintf(fp,"@SP\n");
			fprintf(fp,"M=M-1\n");			
		}
		else if(!strcmp(arg,"that"))
		{
			fprintf(fp,"@THAT\n");
			fprintf(fp,"D=M\n");
			fprintf(fp,"@%d\n",argp);
			fprintf(fp,"D=D+A\n");
			fprintf(fp,"@SP\n");
			fprintf(fp,"A=M\n");
			fprintf(fp,"M=D\n");
			fprintf(fp,"A=A-1\n");
			fprintf(fp,"D=M\n");
			fprintf(fp,"A=A+1\n");
			fprintf(fp,"A=M\n");
			fprintf(fp,"M=D\n");
			fprintf(fp,"@SP\n");
			fprintf(fp,"M=M-1\n");	
		}
		else if(!strcmp(arg,"pointer"))
		{
			fprintf(fp,"@SP\n");
			fprintf(fp,"M=M-1\n");
			fprintf(fp,"A=M\n");
			fprintf(fp,"D=M\n");
			fprintf(fp,"@%d\n",argp+3);
			fprintf(fp,"M=D\n");
		}
		else if(!strcmp(arg,"temp"))
		{
			fprintf(fp,"@SP\n");
			fprintf(fp,"M=M-1\n");
			fprintf(fp,"A=M\n");
			fprintf(fp,"D=M\n");
			fprintf(fp,"@%d\n",argp+5);
			fprintf(fp,"M=D\n");
		}
		else if(!strcmp(arg,"static"))
		{
			fprintf(fp,"@SP\n");
			fprintf(fp,"M=M-1\n");
			fprintf(fp,"A=M\n");
			fprintf(fp,"D=M\n");
			fprintf(fp,"@%s.%d\n",filename,argp);
			fprintf(fp,"M=D\n");
		}
	}
}

//for single file .vm
void translator(FILE *in,FILE *out,char *filename)//边读边写
{
	char command[N],type[15];
	while(!hasMoreCommands(in,command))
	{
		commandType(command,type);
		switch(type[2])
		{
		case 'A':
			WriteArithmetic(command,out);
			break;
		case 'P':
			WritePushPop(command,out,type,filename);
			break;
		}
	}
}
//main program
void main()
{
	int hasMoreCommands(FILE *fp,char command[N]);
	void commandType(char command[N],char *type);
	int CutString(char command[N],int i);
	void arg1(char command[N],char *argone);
	void arg1(char command[N],char *argone);
	int arg2(char command[N]);
	void WriteArithmetic(char command[N],FILE *fp);
	void WritePushPop(char command[N],FILE *fp,char *filename);
	void translator(FILE *in,FILE *out,char *filename);
	//记得去注释空行(未完成)

	FILE *fin,*fout;
	if((fin=fopen("D:\\vmtri.vm","r"))==NULL)printf("Error1");
	else
	{
		if((fout=fopen("D:\\vmout.asm","w+"))==NULL)printf("Error2");
		else
		{
			translator(fin,fout,"vmtri");
		}
	}
	fclose(fin);
	fclose(fout);
}