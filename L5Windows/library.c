#include <windows.h>

#define BUFFER_SIZE 512

void WriteDataInFile(HANDLE hFile, char* buff)
{
	struct _OVERLAPPED overlapped;                                        //���ܧѧ٧ѧ�֧ݧ� �ߧ� �����ܧ���� overlapped
	overlapped.Offset = 0xFFFFFFFF;										  //���֧�����ݧ�ا֧ߧڧ� ��ѧۧݧ�, �� �ܧ������ �ߧѧ�ڧߧѧ֧��� ��֧�֧էѧ��
	overlapped.OffsetHigh = 0xFFFFFFFF;									  //����ѧ��֧� ��ݧ�ӧ� ���٧ڧ�ڧ� ��ѧۧݧ�, �� �ܧ������ �ߧѧ�ڧߧѧ֧��� ��֧�֧էѧ��
	overlapped.hEvent = NULL;											  //���֧�ܧ�ڧ���� ���ҧ��ڧ�, �ܧ������ �է�ݧاߧ� �ҧ��� ����ѧߧ�ӧݧ֧ߧ� �� ��ڧԧߧѧݧ�ߧ�� �������ߧڧ�, �ܧ�ԧէ� ���֧�ѧ�ڧ� �٧ѧӧ֧��ڧݧѧ��
	DWORD cbWritten;												      //���ڧ�ݧ� �٧ѧ�ڧ�ѧߧߧ�� �ҧѧۧ�										  
	WriteFile(hFile,                                                      //���֧�ܧ�ڧ���� ��ѧۧݧ�
		buff,														      //�����֧� �էѧߧߧ��
		strlen(buff), 													  //���ڧ�ݧ� �ҧѧۧ� �էݧ� �٧ѧ�ڧ��
		&cbWritten, 													  //���ڧ�ݧ� �٧ѧ�ڧ�ѧߧߧ�� �ҧѧۧ�
		&overlapped);													  //����ڧߧ���ߧߧߧ�� �ҧ��֧�
	WaitForSingleObject(hFile, INFINITE);								  //���اڧէѧߧڧ� ��ѧۧݧ�ӧ�ԧ� �է֧�ܧ�ڧ�����
}

void ReadDataFromFile(HANDLE hFile, char* buff)
{
	memset(buff, 0, BUFFER_SIZE * sizeof(char));    //����է֧ݧ֧ߧڧ� ��ѧާ��� ���� �ҧ��֧�
	struct _OVERLAPPED overlapped;
	overlapped.Offset = 0; 							//���֧�����ݧ�ا֧ߧڧ� ��ѧۧݧ�, �� �ܧ������ �ߧѧ�ڧߧѧ֧��� ��֧�֧էѧ��
	overlapped.OffsetHigh = 0;
	overlapped.hEvent = NULL;						
	DWORD cbRead;									//���ڧ�ݧ� �����ڧ�ѧߧߧ�� �ҧѧۧ�
	ReadFile(hFile, 								//���֧�ܧ�ڧ���� ��ѧۧݧ�
		buff,										//�����֧� �էѧߧߧ��
		BUFFER_SIZE, 								//���ڧ�ݧ� �ҧѧۧ� �էݧ� ���֧ߧڧ�
		&cbRead, 									//���ڧ�ݧ� �����ڧ�ѧߧߧ�� �ҧѧۧ�
		&overlapped);								//����ڧߧ���ߧߧ�� �ҧ��֧�
}