#include "header.h"

#define FILES_COUNT 3

char *files[FILES_COUNT] = { "Files\\1", "Files\\2", "Files\\3" };                                              //���ѧ�ѧݧ�� ��ѧۧݧ��

DWORD WINAPI threadReader(LPVOID t)
{
	struct Info *info = (struct Info *)t;                                                                                   //����ڧӧ֧է֧ߧڧ� �� ��ڧ�� �ѧ�ԧ�ާ֧ߧ�� ���ߧܧ�ڧ�
	ReadDataFromFile readDataFromFile = (ReadDataFromFile)GetProcAddress(info->library, "ReadDataFromFile");				//�����ѧߧ�ӧܧ� ��ܧѧ٧ѧ�֧ݧ� �ߧ� ���ߧܧ�ڧ� �ڧ� �ҧڧҧݧڧ��֧ܧ�

	for (int i = 0; i < FILES_COUNT; i++)
	{
		while (info->flag) {};																							    //���اڧէѧ֧� ��ݧѧ�
		WaitForSingleObject(info->mutex, INFINITE);																		    //���ѧ�ӧѧ� �ާ���֧ܧ��
		HANDLE hFile = CreateFile(info->fileNames[i], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);    //����ܧ��ӧѧ֧� ����֧��ӧ���ڧ� ��ѧۧ� �էݧ� ���֧ߧڧ�
		(*readDataFromFile)(hFile, info->buffer);                                                                           //���ڧ�ѧ֧� ��ѧۧ� ���ߧܧ�ڧ֧� �ڧ� �ҧڧҧݧڧ��֧ܧ�
		CloseHandle(hFile);																									//���ѧܧ��ӧѧ֧� ��ѧۧ�
		info->flag = 1;																										//�����ѧߧѧӧݧڧӧѧ֧� ��ݧѧ�
		ReleaseMutex(info->mutex);																						    //�������ܧѧ֧� �ާ���֧ܧ�
	}
	info->threadsNumber--;
	return 0;
}

DWORD WINAPI threadWriter(LPVOID t)
{
	struct Info *info = (struct Info *)t;
	WaitForSingleObject(info->mutex, INFINITE);		//���ѧ�ӧѧ� �ާ���֧ܧ��

			//����ܧ��ӧѧ֧� ��ѧۧ� �էݧ� �٧ѧ�ڧ��
	HANDLE hFile = CreateFile("Files\\OUTPUT",  //�ڧާ� ��ѧۧݧ�
		GENERIC_WRITE,							//��֧اڧ� �է������
		0,										//���ӧާ֧��ߧ�� �է�����
		NULL,									//�է֧�ܧ�ڧ���� �٧ѧ�ڧ��
		CREATE_ALWAYS,							//��֧اڧ� ��ѧҧ���
		FILE_FLAG_OVERLAPPED,					//�ѧ��ڧҧ�� ��ѧۧݧ�
		NULL);									//�է֧�ܧ�ڧ���� ��ѧҧݧ�ߧ� ��ѧۧݧ�
	
	WriteDataInFile writeDataInFile = (WriteDataInFile)GetProcAddress(info->library, "WriteDataInFile");					//���ܧѧ٧ѧ�֧ݧ� �ߧ� ���ߧܧ�ڧ� �٧ѧ�ڧ��
	ReleaseMutex(info->mutex);					//������ܧѧ֧� �ާ���֧ܧ�

	for (int i = 0; i < FILES_COUNT; i++)
	{
		while (!info->flag) {};									 //��اڧէѧ֧� ��ݧѧ�
		WaitForSingleObject(info->mutex, INFINITE);				 //���ѧ�ӧѧ� �ާ���֧ܧ��
		(*writeDataInFile)(hFile, info->buffer);				 //�� ���ާ���� ���ߧܧ�ڧ� �ҧڧҧݧڧ��֧ܧ� �٧ѧ�ڧ��ӧѧ֧� �էѧߧߧ�� �� ��ѧۧ�
		ReleaseMutex(info->mutex);								 //������ܧѧ֧� �ާ���֧ܧ�
		info->flag = 0;											 //����ѧߧѧӧݧڧӧѧ֧� ��ݧѧ� �� 0
	}
	CloseHandle(hFile);											//�٧ѧܧ��ӧѧ֧� ��ѧۧ�
	info->threadsNumber--;										//�٧ѧӧ֧��ѧ֧� ��ѧҧ��� �����ܧ�
	return 0;
}

void initInfo(struct Info *info)
{
	info->fileNames = files;
	if (!(info->library = LoadLibrary("library.dll")))           //����ܧ��ӧѧ֧� �ҧڧҧݧڧ��֧ܧ�
	{
		printf("library.dll is not found\n");
		exit(-3);
	}
	info->threadsNumber = 0;
	info->flag = 0;

}

void createMutex(struct Info *info)
{
	info->mutex = CreateMutex
		(NULL, //�ѧ��ڧҧ�� �ҧ֧٧��ѧ��ߧ����
		FALSE, //��ݧѧ� �ߧѧ�ѧݧ�ߧ�ԧ� �ӧݧѧէ֧ݧ���
		NULL); //�ڧާ� ��ҧ�֧ܧ��
}

void runThreads(struct Info *info)
{		/*���٧էѧ֧� ������ ���֧ߧڧ�*/
	info->readThread = CreateThread(NULL,	//�ѧ��ڧҧ��� �ҧ֧٧��ѧ��ߧ���� �����ܧ�									
		0,									//��ѧ٧ާ֧� ���֧ܧ�, �ӧ�է֧ݧ�֧ާ�ԧ� ���� ������
		threadReader,					    //�ѧէ�֧� �����ܧ�ӧ�� ���ߧܧ�ڧ�
		(void *)info,						//��ܧѧ٧ѧ�֧ݧ� �ߧ� ��ѧ�ѧާ֧���, ��֧�֧էѧӧѧ֧ާ�� ���ߧܧ�ڧ� �����ܧ� ���� �֧ԧ� ���٧էѧߧڧ�
		0,									//��ݧѧ� ���٧էѧߧڧ� �����ܧ� (������ �٧ѧ���ܧѧ֧��� �ߧ֧ާ֧էݧ֧ߧߧ� �ڧݧ� ���٧էѧ֧��� �� ����ѧߧ�ӧݧ֧ߧߧ�� �������ߧڧ�
		NULL);								//��ܧѧ٧ѧ�֧ݧ�, ��� �ܧ�����ާ� �ҧ�է֧� �٧ѧ�ڧ�ѧ� �ڧէ֧ߧ�ڧ�ڧܧѧ��� ���٧էѧߧߧ�ԧ� �����ܧ�											
	info->threadsNumber++;
	info->writeThread = CreateThread(NULL, 0, threadWriter, (void *)info, 0, NULL);										     //������ �٧ѧ�ڧ��
	info->threadsNumber++;
}

void waitThreads(struct Info *info)
{
	while (info->threadsNumber);
	CloseHandle(info->readThread);
	CloseHandle(info->writeThread);
	CloseHandle(info->mutex);
	FreeLibrary(info->library);
	printf("OK\n");
}