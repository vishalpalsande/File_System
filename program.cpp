#include "header.h"

struct SuperBlock
{
	int TotalInodes;
	int FreeInodes;
}Obj_Super;		//create object for super block

struct inode
{
	char File_name[50];
	int Inode_number;
	int File_Size;
	int File_Type;	//if it is 0 means file is deleted
	int ActualFileSize;
	int Link_Count;
	int Reference_Count;
	char *Buffer_Cache;		//actual data stored in this string (1024 byte dynamic memory allocate)
	struct inode *next;
};

typedef struct inode INODE;
typedef struct inode * PINODE;
typedef struct inode ** PPINODE;

struct FileTable
{
	int ReadOffSet;
	int WriteOffSet;
	int Count;
	PINODE iptr;
	int Mode;
};

typedef FileTable FILETABLE;
typedef FileTable * PFILETABLE;

struct UFDT
{
	PFILETABLE ufdt[MAXFILES];
}UFDTObj;


PINODE Head =NULL; //Global pointer of inode

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//It is used to check the file is already present or not
bool CheckFile(char *name)
{
	PINODE temp = Head;
	while(temp != NULL)
	{
		if(temp->File_Type != 0)
		{
			if(strcmp(temp->File_name, name) == 0)
			{
				break;
			}
		}
		temp = temp->next;
	}
	
	if(temp == NULL)
	{
		return false;
	}
	else
	{
		return true;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CreateUFDT()
{
	int i=0;
	for(i=0 ; i<MAXFILES; i++)
	{
		UFDTObj.ufdt[i] = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//create linked list of inode
void CreateDILB()
{
	int i=1;
	PINODE newn = NULL;
	PINODE temp = Head;
	
	while(i<= MAXFILES)	//loop iterates 50 times
	{
		newn = (PINODE)malloc(sizeof(INODE));
		newn -> Inode_number = i;
		newn -> File_Size = FILESIZE;
		newn -> File_Type = 0;
		newn -> ActualFileSize = 0;
		newn -> Link_Count = 0;
		newn -> Reference_Count = 0;
		newn ->Buffer_Cache = NULL;
		
		if(Head == NULL)	//First Inode
		{
			Head = newn;
			temp = Head;
		}
		else		// iNode second onwords
		{
			temp -> next = newn;
			temp = temp ->next;		
		}
		
		i++;
	}
	
	printf("DILB created successfully\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CreateSuperBlock()
{
	Obj_Super.TotalInodes = MAXFILES;
	Obj_Super.FreeInodes = MAXFILES;
	
	printf("Super Block Created Successfully\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SetEnvironment()
{
	CreateDILB();
	CreateSuperBlock();
	CreateUFDT();
	printf("Environment for the virtual file system is set..\n\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CreateFile(char *name,int Permission)	//creat()
{
	bool bret = false;
	
	if((name == NULL) || (Permission > READ+WRITE) || (Permission < WRITE))
	{
		return -1;
	}
	
	bret = CheckFile(name);
	if(bret == true)
	{
		printf("File is Already present\n");
		return -1;
	}
	
	if(Obj_Super.FreeInodes == 0 )
	{
		printf("Thre is no inode to creat the file\n");
		return -1;
	}
	
	int i=0;
	//Search for empty entry from UFDT
	for(i = 0; i < MAXFILES; i++)
	{
		if(UFDTObj.ufdt[i] == NULL)
		{
			break;
		}
	}
	
	if(i == MAXFILES)
	{
		printf("Unable to get entry in UFDT\n");
		return -1;
	}
	
	//file table memory allocate
	UFDTObj.ufdt[i] = (PFILETABLE)malloc(sizeof(FILETABLE));

	//Initialise the file table
	UFDTObj.ufdt[i] -> ReadOffSet = 0 ;
	UFDTObj.ufdt[i] -> WriteOffSet = 0 ;
	UFDTObj.ufdt[i] -> Mode = Permission ;
	UFDTObj.ufdt[i] -> Count = 1 ;
	
	//Search Empty Inode
	
	PINODE temp= Head;
	while(temp!= NULL)
	{
		if(temp->File_Type == 0)
		{
			break;
		}
		temp=temp->next;
	}
	
	UFDTObj.ufdt[i] -> iptr = temp ;
	strcpy(UFDTObj.ufdt[i] -> iptr -> File_name,name);
	UFDTObj.ufdt[i] -> iptr -> File_Type = REGULAR;
	UFDTObj.ufdt[i] -> iptr ->ActualFileSize = 0;
	UFDTObj.ufdt[i] -> iptr -> Link_Count =1;
	UFDTObj.ufdt[i] -> iptr -> Reference_Count =1;
	
	//Allocate memory for files data
	UFDTObj.ufdt[i] -> iptr -> Buffer_Cache = (char*)malloc(sizeof(FILESIZE));
	
	Obj_Super.FreeInodes--;
		
	return i;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DeleteFile(char *name)	//rm() function
{

	bool bret = false;
	
	if( name == NULL )
	{
		return ;
	}
	
	bret = CheckFile(name);
	if(bret == false)
	{
		printf("There is no such file\n");
		return ;
	}
	
	//search UFDT entry
	int i=0;
	for(i = 0; i<MAXFILES;i++)
	{
		 if(strcmp(UFDTObj.ufdt[i]->iptr->File_name, name) ==0)
		 {
		 	break;
		 }
	}
	
	strcpy(UFDTObj.ufdt[i] -> iptr -> File_name,"");
	UFDTObj.ufdt[i] -> iptr -> File_Type = 0;
	UFDTObj.ufdt[i] -> iptr ->ActualFileSize = 0;
	UFDTObj.ufdt[i] -> iptr -> Link_Count =0;
	UFDTObj.ufdt[i] -> iptr -> Reference_Count =0;
	
	//free memory for file
	free(UFDTObj.ufdt[i] -> iptr -> Buffer_Cache);
	
	free(UFDTObj.ufdt[i]);
	
	UFDTObj.ufdt[i] = NULL;
	
	Obj_Super.FreeInodes++;
	
	printf("File Successfully Deleted\n");
	
}	
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LS()
{
	PINODE temp = Head;
	
	while(temp != NULL)
	{
		if(temp -> File_Type != 0)
		{
			printf("%s\n",temp->File_name);
		}
		temp=temp->next;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int WriteFile(int fd)		//write() function
{
	if(UFDTObj.ufdt[fd] == NULL)
	{
		printf("Invalid File Descriptor\n");
		return -1;
	}

	if(UFDTObj.ufdt[fd]->Mode == READ)
	{
		printf("There is no write permission\n");
		return -1;
	}
	
	char arr[1024];
	printf("Please enter any data to write in the file:\n");
	fgets(arr,1024,stdin);
	fflush(stdin);
	
	int size=strlen(arr)-1;
	
	
	// strncpy:- number(n) of letter copy from one string to another string.
	// ((UFDTObj.ufdt[fd] -> iptr ->Buffer_Cache) + (UFDTObj.ufdt[fd] -> WriteOffSet):-  destination string address
	// arr:- 	source string address
	// size:- size of string
	strncpy(((UFDTObj.ufdt[fd] -> iptr ->Buffer_Cache) + (UFDTObj.ufdt[fd] -> WriteOffSet)),arr,size);
	
	UFDTObj.ufdt[fd]->WriteOffSet = UFDTObj.ufdt[fd] -> WriteOffSet + size;
	
	return size;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DisplayHelp()
{
	printf("------------------------------------------------------------------------------------------------\n");
	printf("open: It is used to open the existing file\n");
	printf("close: It is used to close the open file\n");
	printf("read: It is used to read the contents of file\n");
	printf("write: It is used to write data into file\n");
	printf("lseek: It is used to change the offset of file\n");
	printf("stat: It is used to display the information of file\n");
	printf("fstat: It is used to display the information of opened file\n");
	printf("creat: It is used to create new regular file\n");
	printf("ls: It os used to display all names of files\n");
	printf("rm: It is delete existing regular file\n");	
	printf("------------------------------------------------------------------------------------------------\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ManPage(char *str)
{
	if(strcmp(str,"open") == 0)
	{
		printf("Description: It is used to open an existing fle\n");
		printf("Command: open file_name Mode\n\n");
	}
	else
	if(strcmp(str,"close") == 0)
	{
		printf("Description: It is used to closed existing file\n");
		printf("Command: close file_name\n\n");
	}
	else
	if(strcmp(str,"ls") == 0)
	{
		printf("Description: It is used to list out all name of the files\n");
		printf("Command: ls\n\n");
	}
	else
	if(strcmp(str,"creat") == 0)
	{
		printf("Description: It is used to create new regular file\n");
		printf("Command: creat File_name Permission\n");
	}
	else
	if(strcmp(str,"rm") == 0)
	{
		printf("Description: It is used to delete existing regular file\n");
		printf("Command: rm File_name");
	}
	else
	if(strcmp(str,"write") == 0)
	{
		printf("Discription: It is used to write data into file\n");
		printf("Command: write File_Descriptor\n");
		printf("After the command please enter the data\n");
	}
	else
	{
		printf("Man page not found\n\n");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ShowCommand()
{
	printf("\nAll Commands are followings: \n\n");
	printf("1> man\t\t\t\t command:> man command_name\n");
	printf("2> create file\t\t\t command:> creat file_name permission\n");
	printf("3> write in file\t\t command:> write file_discriptor\n");
	printf("4> remove file\t\t\t command:> rm file_name\n");
	printf("5> show files\t\t\t command:> ls\n");
	printf("6> clear page\t\t\t command:> clear\n");
	printf("7> exit FileSystem\t\t command:> exit\n");
}

