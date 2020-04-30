#include <string.h>
#include <stdio.h>
#include <iostream>

using namespace std;

#define BlockOfMemory 512 //Block memory
#define allMemory 32768 //32 kb
//blockBuff[0] = allProgMemory;
//blockBuffSize[0] = avaibleMemory;
char allProgMemory[allMemory];   //§Ó§ã§ñ §á§Ñ§Þ§ñ§ä§î §Ó§í§Õ§Ö§Ý§Ö§ß§ß§Ñ§ñ §Õ§Ý§ñ §á§â§à§Ô§â§Ñ§Þ§Þ§í
unsigned long avaibleMemory = allMemory; //§Õ§à§ã§ä§å§á§ß§í§Û, §Ó §Õ§Ñ§ß§ß§í§Û §Þ§à§Þ§Ö§ß§ä, §à§Ò§ì§Ö§Þ §á§Ñ§Þ§ñ§ä§Ú
char* blockBuff[BlockOfMemory];  //§Ò§Ý§à§Ü §á§Ñ§Þ§ñ§ä§Ú
unsigned int blockBuffCounter = 1; //§ã§é§Ö§ä§é§Ú§Ü §Ò§Ý§à§Ü§à§Ó §á§Ñ§Þ§ñ§ä§Ú
unsigned int blockBuffSize[BlockOfMemory]; //§Ò§Ý§à§Ü §á§Ñ§Þ§ñ§ä§Ú

void* arrayOfPointer[64]; //§Þ§Ñ§ã§ã§Ú§Ó §ß§Ñ§ê§Ú§ç §Ò§Ý§à§Ü§à§Ó §á§Ñ§Þ§ñ§ä§Ú

char* useBuff[BlockOfMemory]; //§Ú§ã§á§à§Ý§î§Ù§à§Ó§Ñ§ß§ß§í§Ö §Ò§Ý§à§Ü§Ú §á§Ñ§Þ§ñ§ä§Ú
unsigned int useBuffCounter = 0; //§ã§é§Ö§ä§é§Ú§Ü §Ú§ã§á§à§Ý§î§Ù§à§Ó§Ñ§ß§ß§í§ç §Ò§Ý§à§Ü§à§Ó §á§Ñ§Þ§ñ§ä§Ú
unsigned int block_size[BlockOfMemory]; //§â§Ñ§Ù§Þ§Ö§â §Ò§Ý§à§Ü§Ñ
unsigned int lastblockUsebuff; //§á§à§ã§Ý§Ö§Õ§ß§Ú§Û §Ò§Ý§à§Ü §Ó §á§Ñ§Þ§ñ§ä§Ú

void init()
{
	blockBuff[0] = allProgMemory;
	blockBuffSize[0] = avaibleMemory;
}

void* myMalloc(unsigned long size)
{
	unsigned long int last;
	char *tempArray;

	if (size > avaibleMemory)  //§á§â§à§Ó§Ö§â§Ü§Ñ §ß§Ñ §ß§Ñ§Ý§Ú§é§Ú§Ö §á§Ñ§Þ§ñ§ä§Ú
		return NULL;

	tempArray = NULL;

	for (int i = 0; i < blockBuffCounter; i++)
		if (size <= blockBuffSize[i]) //§Ö§ã§Ý§Ú §â§Ñ§Ù§Þ§Ö§â §Þ§Ñ§ã§ã§Ú§Ó§Ñ §Þ§Ö§ß§î§ê§Ö §â§Ñ§Ù§Þ§Ö§â§Ñ §Ò§Ý§à§Ü§Ñ §á§Ñ§Þ§ñ§ä§Ú, §ä§à §Þ§Ñ§ã§ã§Ú§Ó §Ó§Þ§Ö§ë§Ñ§Ö§ä§ã§ñ
		{
			tempArray = blockBuff[i]; //§Ù§Ñ§á§à§Þ§Ú§ß§Ñ§Ö§Þ §á§à§ã§Ý§Ö§Õ§ß§Ú§Û §Ò§Ý§à§Ü
			last = i;                 //§Ù§Ñ§á§à§Þ§Ú§ß§Ñ§Ö§Þ §á§à§ã§Ý§Ö§Õ§ß§Ú§Û §ï§Ý§Ö§Þ§Ö§ß§ä
			break;
		}

	if (!tempArray)  //§Ö§ã§Ý§Ú §ß§Ö§ä §ã§Ó§à§Ò§à§Õ§ß§à§Û §á§Ñ§Þ§ñ§ä§Ú(§ß§Ö §ã§à§Ù§Õ§Ñ§Ý§ã§ñ §Ó§â§Ö§Þ§Ö§ß§ß§í§Û §Þ§Ñ§ã§ã§Ú§Ó
		return NULL;

	useBuff[useBuffCounter] = tempArray;            //§Ù§Ñ§á§Ú§ã§í§Ó§Ñ§Ö§Þ §Ó§â§Ö§Þ§Ö§ß§ß§í§Û §Þ§Ñ§ã§ã§Ú§Ó §Ó §á§Ñ§Þ§ñ§ä§î
	block_size[useBuffCounter] = size;			    //§Ù§Ñ§á§à§Þ§Ú§ß§Ñ§Ö§Þ §â§Ñ§Ù§Þ§Ö§â
	useBuffCounter++;								//§å§Ó§Ö§Ý§Ú§é§Ú§Ó§Ñ§Ö§Þ §ã§é§Ö§ä§é§Ú§Ü
	lastblockUsebuff++;								//§Ù§Ñ§á§à§Þ§Ú§ß§Ñ§Ö§Þ §á§à§ã§Ý§Ö§Õ§ß§Ú§Û §ï§Ý§Ö§Þ§Ö§ß§ä
	blockBuff[last] = (char*)(last + size + 1);		//§á§Ö§â§Ö§Ó§à§Õ §Ó char 
	blockBuff[last] = blockBuff[last] - size;

	avaibleMemory -= size;
	return tempArray;
}

int myFree(void* memblock)
{
	unsigned int last;
	char* tempArray = 0;
	for (int i = 0; i < lastblockUsebuff; i++)		//§à§ã§Ó§à§Ò§à§Ø§Õ§Ö§ß§Ú§Ö §á§Ñ§Þ§ñ§ä§Ú
		if (memblock == useBuff[i])
		{
			tempArray = useBuff[i];
			last = i;
			break;
		}

	if (!tempArray)								//§á§â§à§Ó§Ö§â§Ü§Ñ
		return -1;

	useBuff[last] = 0;       //§Ú§ã§á§à§Ý§î§Ù§à§Ó§Ñ§ß§ß§í§Û §Ò§å§æ§Ö§â = 0
	useBuffCounter--; //§å§Þ§Ö§ß§î§ê§Ö§ß§Ú§Ö §ã§é§Ö§ä§é§Ú§Ü§Ñ
	blockBuff[blockBuffCounter] = (char*)memblock; //§á§Ö§â§Ö§Ó§à§Õ§Ú§ä§ã§ñ §Ó char 
	blockBuffSize[blockBuffCounter] = block_size[last];//§à§ã§Ó§à§Ò§à§Ø§Õ§Ñ§Ö§ä§ã§ñ
	blockBuffCounter++;//
	avaibleMemory += block_size[last];
	return 0;
}

void* myRealloc(void* memory_block, int size)
{
	unsigned int i, last;
	char* tempArray = 0;

	for (i = 0; i < lastblockUsebuff; i++)
		if (memory_block == useBuff[i])		//§Ö§ã§Ý§Ú §â§Ñ§Ù§Þ§Ö§â §â§Ñ§Ó§Ö§ß §á§â§Ö§Õ§í§Õ§å§ë§Ö§Þ§å
		//	if (useBuff[i] == NULL)			//§Ú §Ö§ã§Ý§Ú §â§Ñ§Ù§Þ§Ö§â §â§Ñ§Ó§Ö§ß §ß§å§Ý§ð
			return myMalloc(size);


	if (size == 0)				//§Ö§ã§Ý§Ú §â§Ñ§Ù§Þ§Ö§â §ß§å§Ý§Ö§Ó§à§Û
	{
		myFree(memory_block);  //§à§ã§Ó§à§Ò§à§Ø§Õ§Ñ§Ö§Þ §á§Ñ§Þ§ñ§ä§î
		return NULL;
	}

	for (i = 0; i < lastblockUsebuff; i++)  //§á§Ö§â§Ö§Ù§Ñ§á§Ú§ã§î
		if (memory_block == useBuff[i])
		{
			tempArray = useBuff[i];
			last = i;
			break;
		}

	if (!tempArray)
		return NULL;

	myFree(memory_block);  //§à§é§Ú§ë§Ñ§Ö§Þ §Ó§ä§à§â§à§Û §Ò§Ý§à§Ü 
	return myMalloc(size); //§Ó§à§Ó§Ù§â§Ñ§ë§Ñ§Ö§Þ §Ó§Ú§Õ§à§Ú§Ù§Þ§Ö§ß§Ö§ß§ß§í§Û §Ò§Ý§à§Ü
}


int main()
{

	init();
	int choose;
	int size;
	int index;
	while (true)
	{
		cout << "1) malloc\n";
		cout << "2) free\n";
		cout << "3) realloc\n";
		cout << "4) quit\n";
		cin >> choose;
		switch (choose)
		{
		case 1:
		{
			cout << "Input index: ";
			cin >> index;
			if (index > 64)  //§Ú§ß§Õ§Ö§Ü§ã §ß§Ö §Þ§à§Ø§Ö§ä §Ò§í§ä§î §Ò§à§Ý§î§ê§Ö 64 (§Ó§ã§Ö§Ô§à 64 §Ò§Ý§à§Ü§Ñ §á§Ñ§Þ§ñ§ä§Ú)
			{
				cout << "Wrong index!\n";
				break;
			}
			if (arrayOfPointer[index] == NULL)
			{
				cout << "Input size: ";
				cin >> size;
				arrayOfPointer[index] = myMalloc(size);
				if (arrayOfPointer[index] == NULL)
				{
					cout << "Error! No memory!\n";
					return 1;
				}
				cout << "Use malloc:\n";
				printf("Use blocks - %d\n", useBuffCounter);
			}
			else {
				cout << "Error!\n";
				break;
			}
			break;
		}
		case 2:
		{
			cout << "Input index: ";
			cin >> index;
			if (myFree(arrayOfPointer[index]) == -1)
			{
				cout << "Our block no found\n";
				break;
			}
			cout << "Use free:\n";
			printf("Use blocks - %d\n", useBuffCounter);
			break;
		}
		case 3:
		{
			cout << "Input index: ";
			cin >> index;
			cout << "Input size: ";
			cin >> size;
			arrayOfPointer[index] = myRealloc(arrayOfPointer[index], size);
			if (arrayOfPointer[index] == NULL)
			{
				cout << "Error! No memory!\n";
				return 1;
			}
			cout << "Use realloc:\n";
			printf("Use blocks - %d\n", useBuffCounter);
			break;
		}
		case 4:
			return 0;
		default:
			break;
		}
	}
}

