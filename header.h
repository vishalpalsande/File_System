#include<stdio.h>
#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>

#define MAXFILES 50
#define FILESIZE 1024

#define READ 4
#define WRITE 2

#define REGULAR 1
#define SPECIAL 2

bool CheckFile(char *);
void CreateUFDT();
void CreateDILB();
void CreateSuperBlock();
void SetEnvironment();
int CreateFile(char *,int );
void DeleteFile(char *);
void LS();
int WriteFile(int );
void DisplayHelp();
void ManPage(char *);
void ShowCommand();

