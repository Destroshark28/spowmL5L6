#include <string.h>
#include <stdio.h>
#include <iostream>

using namespace std;

#define BlockOfMemory 512 //Block memory
#define allMemory 32768 //32 kb
//blockBuff[0] = allProgMemory;
//blockBuffSize[0] = avaibleMemory;
char allProgMemory[allMemory];   //�ӧ�� ��ѧާ��� �ӧ�է֧ݧ֧ߧߧѧ� �էݧ� ����ԧ�ѧާާ�
unsigned long avaibleMemory = allMemory; //�է�����ߧ��, �� �էѧߧߧ�� �ާ�ާ֧ߧ�, ��ҧ�֧� ��ѧާ���
char* blockBuff[BlockOfMemory];  //�ҧݧ�� ��ѧާ���
unsigned int blockBuffCounter = 1; //���֧��ڧ� �ҧݧ�ܧ�� ��ѧާ���
unsigned int blockBuffSize[BlockOfMemory]; //�ҧݧ�� ��ѧާ���

void* arrayOfPointer[64]; //�ާѧ��ڧ� �ߧѧ�ڧ� �ҧݧ�ܧ�� ��ѧާ���

char* useBuff[BlockOfMemory]; //�ڧ���ݧ�٧�ӧѧߧߧ�� �ҧݧ�ܧ� ��ѧާ���
unsigned int useBuffCounter = 0; //���֧��ڧ� �ڧ���ݧ�٧�ӧѧߧߧ�� �ҧݧ�ܧ�� ��ѧާ���
unsigned int block_size[BlockOfMemory]; //��ѧ٧ާ֧� �ҧݧ�ܧ�
unsigned int lastblockUsebuff; //����ݧ֧էߧڧ� �ҧݧ�� �� ��ѧާ���

void init()
{
	blockBuff[0] = allProgMemory;
	blockBuffSize[0] = avaibleMemory;
}

void* myMalloc(unsigned long size)
{
	unsigned long int last;
	char *tempArray;

	if (size > avaibleMemory)  //����ӧ֧�ܧ� �ߧ� �ߧѧݧڧ�ڧ� ��ѧާ���
		return NULL;

	tempArray = NULL;

	for (int i = 0; i < blockBuffCounter; i++)
		if (size <= blockBuffSize[i]) //�֧�ݧ� ��ѧ٧ާ֧� �ާѧ��ڧӧ� �ާ֧ߧ��� ��ѧ٧ާ֧�� �ҧݧ�ܧ� ��ѧާ���, ��� �ާѧ��ڧ� �ӧާ֧�ѧ֧���
		{
			tempArray = blockBuff[i]; //�٧ѧ��ާڧߧѧ֧� ����ݧ֧էߧڧ� �ҧݧ��
			last = i;                 //�٧ѧ��ާڧߧѧ֧� ����ݧ֧էߧڧ� ��ݧ֧ާ֧ߧ�
			break;
		}

	if (!tempArray)  //�֧�ݧ� �ߧ֧� ��ӧ�ҧ�էߧ�� ��ѧާ���(�ߧ� ���٧էѧݧ�� �ӧ�֧ާ֧ߧߧ�� �ާѧ��ڧ�
		return NULL;

	useBuff[useBuffCounter] = tempArray;            //�٧ѧ�ڧ��ӧѧ֧� �ӧ�֧ާ֧ߧߧ�� �ާѧ��ڧ� �� ��ѧާ���
	block_size[useBuffCounter] = size;			    //�٧ѧ��ާڧߧѧ֧� ��ѧ٧ާ֧�
	useBuffCounter++;								//��ӧ֧ݧڧ�ڧӧѧ֧� ���֧��ڧ�
	lastblockUsebuff++;								//�٧ѧ��ާڧߧѧ֧� ����ݧ֧էߧڧ� ��ݧ֧ާ֧ߧ�
	blockBuff[last] = (char*)(last + size + 1);		//��֧�֧ӧ�� �� char 
	blockBuff[last] = blockBuff[last] - size;

	avaibleMemory -= size;
	return tempArray;
}

int myFree(void* memblock)
{
	unsigned int last;
	char* tempArray = 0;
	for (int i = 0; i < lastblockUsebuff; i++)		//���ӧ�ҧ�اէ֧ߧڧ� ��ѧާ���
		if (memblock == useBuff[i])
		{
			tempArray = useBuff[i];
			last = i;
			break;
		}

	if (!tempArray)								//����ӧ֧�ܧ�
		return -1;

	useBuff[last] = 0;       //�ڧ���ݧ�٧�ӧѧߧߧ�� �ҧ��֧� = 0
	useBuffCounter--; //��ާ֧ߧ��֧ߧڧ� ���֧��ڧܧ�
	blockBuff[blockBuffCounter] = (char*)memblock; //��֧�֧ӧ�էڧ��� �� char 
	blockBuffSize[blockBuffCounter] = block_size[last];//���ӧ�ҧ�اէѧ֧���
	blockBuffCounter++;//
	avaibleMemory += block_size[last];
	return 0;
}

void* myRealloc(void* memory_block, int size)
{
	unsigned int i, last;
	char* tempArray = 0;

	for (i = 0; i < lastblockUsebuff; i++)
		if (memory_block == useBuff[i])		//�֧�ݧ� ��ѧ٧ާ֧� ��ѧӧ֧� ���֧է�է��֧ާ�
		//	if (useBuff[i] == NULL)			//�� �֧�ݧ� ��ѧ٧ާ֧� ��ѧӧ֧� �ߧ�ݧ�
			return myMalloc(size);


	if (size == 0)				//�֧�ݧ� ��ѧ٧ާ֧� �ߧ�ݧ֧ӧ��
	{
		myFree(memory_block);  //���ӧ�ҧ�اէѧ֧� ��ѧާ���
		return NULL;
	}

	for (i = 0; i < lastblockUsebuff; i++)  //��֧�֧٧ѧ�ڧ��
		if (memory_block == useBuff[i])
		{
			tempArray = useBuff[i];
			last = i;
			break;
		}

	if (!tempArray)
		return NULL;

	myFree(memory_block);  //���ڧ�ѧ֧� �ӧ����� �ҧݧ�� 
	return myMalloc(size); //�ӧ�ӧ٧�ѧ�ѧ֧� �ӧڧէ�ڧ٧ާ֧ߧ֧ߧߧ�� �ҧݧ��
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
			if (index > 64)  //�ڧߧէ֧ܧ� �ߧ� �ާ�ا֧� �ҧ��� �ҧ�ݧ��� 64 (�ӧ�֧ԧ� 64 �ҧݧ�ܧ� ��ѧާ���)
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

