#include "header.h"

int main()
{
	//variable declaration
	char str[80];
	char command[4][80];
	int count=0;
	
	//printf("\nCustomised Virtual File System\n");
	
	printf("\n-------------------------------------> Vishal File System <-------------------------------------\n");
	
	printf("\nUsing 'show' command view all commands for use in this File System\n\n");
	
	SetEnvironment();
	
	while(1)
	{
		printf("\nViSHAL VFS :> ");
		
		//Accept Command from user
		fgets(str,80,stdin);			//use like a scanf
		fflush(stdin); 				//keyboard input flush(while(1))
		
		//break command into token
		count= sscanf(str,"%s %s %s %s",command[0],command[1],command[2],command[3]); 
		
		if(count == 1)
		{
			if(strcmp(command[0],"show") == 0)
			{
				ShowCommand();
			}
			else
			if(strcmp(command[0],"help") == 0)
			{
				DisplayHelp();
			}
			else
			if(strcmp(command[0],"exit") == 0)
			{
				printf("\n________________Thank you using this Vishal File System________________\n");
				break;
			}
			else
			if(strcmp(command[0],"clear") == 0)
			{
				system("clear");
			}
			else
			if(strcmp(command[0],"ls") == 0)
			{
				LS();
			}
			else
			{
			printf("Command Not Found\n");
			}
		}
		else 
		if(count == 2)
		{
			if(strcmp(command[0],"stat") == 0)
			{
				Stat_File(command[1]);
			}
			else
			if(strcmp(command[0],"man") == 0)
			{
				ManPage(command[1]);
			}
			else
			if(strcmp(command[0],"rm") == 0)
			{
				DeleteFile(command[1]);
			}
			else
			if(strcmp(command[0],"write") == 0)
			{

				
				int ret = WriteFile(atoi(command[1]));
				if(ret != -1)
				{
					printf("%d byte gets written successfully in the file\n",ret);
				}
				else
				{
					printf("Data not written in the file\n");
				}
			}
			else
			{
				printf("Command not found\n");
			}
		}
		else
		if(count == 3)	
		{
			if(strcmp(command[0],"creat") == 0)	//eg:- creat demo.txt 6
			{
				int fd=0;	
				fd=CreateFile(command[1],atoi(command[2]));	//atoi use for string to integer number
				if(fd == -1)
				{
					printf("Unable to create file\n");
				}
				else
				{
					printf("Files successfully opened with FD %d\n",fd);
				}
			}
			else
			{
				printf("Command not Found\n");
			}
		}
		else
		if(count == 4)
		{
		}
		else
		{
			printf("Bad Command Entered\n");
		}
				
	}
	
	return 0;
	
}







