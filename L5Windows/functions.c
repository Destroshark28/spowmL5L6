#include "header.h"

#define FILES_COUNT 3

char *files[FILES_COUNT] = { "Files\\1", "Files\\2", "Files\\3" };                                              //§¬§Ñ§ä§Ñ§Ý§à§Ô §æ§Ñ§Û§Ý§à§Ó

DWORD WINAPI threadReader(LPVOID t)
{
	struct Info *info = (struct Info *)t;                                                                                   //§±§â§Ú§Ó§Ö§Õ§Ö§ß§Ú§Ö §Ü §ä§Ú§á§å §Ñ§â§Ô§å§Þ§Ö§ß§ä§Ñ §æ§å§ß§Ü§è§Ú§Ú
	ReadDataFromFile readDataFromFile = (ReadDataFromFile)GetProcAddress(info->library, "ReadDataFromFile");				//§µ§ã§ä§Ñ§ß§à§Ó§Ü§Ñ §å§Ü§Ñ§Ù§Ñ§ä§Ö§Ý§ñ §ß§Ñ §æ§å§ß§Ü§è§Ú§ð §Ú§Ù §Ò§Ú§Ò§Ý§Ú§à§ä§Ö§Ü§Ú

	for (int i = 0; i < FILES_COUNT; i++)
	{
		while (info->flag) {};																							    //§°§Ø§Ú§Õ§Ñ§Ö§Þ §æ§Ý§Ñ§Ô
		WaitForSingleObject(info->mutex, INFINITE);																		    //§©§Ñ§ç§Ó§Ñ§ä §Þ§î§ð§ä§Ö§Ü§ã§Ñ
		HANDLE hFile = CreateFile(info->fileNames[i], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);    //§°§ä§Ü§â§í§Ó§Ñ§Ö§Þ §ã§å§ë§Ö§ã§ä§Ó§å§ð§ë§Ú§Û §æ§Ñ§Û§Ý §Õ§Ý§ñ §é§ä§Ö§ß§Ú§ñ
		(*readDataFromFile)(hFile, info->buffer);                                                                           //§¹§Ú§ä§Ñ§Ö§Þ §æ§Ñ§Û§Ý §æ§å§ß§Ü§è§Ú§Ö§Û §Ú§Ù §Ò§Ú§Ò§Ý§Ú§à§ä§Ö§Ü§Ú
		CloseHandle(hFile);																									//§©§Ñ§Ü§â§í§Ó§Ñ§Ö§Þ §æ§Ñ§Û§Ý
		info->flag = 1;																										//§µ§ã§ä§Ñ§ß§Ñ§Ó§Ý§Ú§Ó§Ñ§Ö§Þ §æ§Ý§Ñ§Ô
		ReleaseMutex(info->mutex);																						    //§°§ä§á§å§ã§Ü§Ñ§Ö§Þ §Þ§î§ð§ä§Ö§Ü§ã
	}
	info->threadsNumber--;
	return 0;
}

DWORD WINAPI threadWriter(LPVOID t)
{
	struct Info *info = (struct Info *)t;
	WaitForSingleObject(info->mutex, INFINITE);		//§©§Ñ§ç§Ó§Ñ§ä §Þ§î§ð§ä§Ö§Ü§ã§Ñ

			//§°§ä§Ü§â§í§Ó§Ñ§Ö§Þ §æ§Ñ§Û§Ý §Õ§Ý§ñ §Ù§Ñ§á§Ú§ã§Ú
	HANDLE hFile = CreateFile("Files\\OUTPUT",  //§Ú§Þ§ñ §æ§Ñ§Û§Ý§Ñ
		GENERIC_WRITE,							//§â§Ö§Ø§Ú§Þ §Õ§à§ã§ä§å§á§Ñ
		0,										//§ã§à§Ó§Þ§Ö§ã§ä§ß§í§Û §Õ§à§ã§ä§å§á
		NULL,									//§Õ§Ö§ã§Ü§â§Ú§á§ä§à§â §Ù§Ñ§ë§Ú§ä§í
		CREATE_ALWAYS,							//§â§Ö§Ø§Ú§Þ §â§Ñ§Ò§à§ä§í
		FILE_FLAG_OVERLAPPED,					//§Ñ§ä§â§Ú§Ò§å§ä §æ§Ñ§Û§Ý§Ñ
		NULL);									//§Õ§Ö§ã§Ü§â§Ú§á§ä§à§â §ê§Ñ§Ò§Ý§à§ß§Ñ §æ§Ñ§Û§Ý§Ñ
	
	WriteDataInFile writeDataInFile = (WriteDataInFile)GetProcAddress(info->library, "WriteDataInFile");					//§µ§Ü§Ñ§Ù§Ñ§ä§Ö§Ý§î §ß§Ñ §æ§å§ß§Ü§è§Ú§ð §Ù§Ñ§á§Ú§ã§Ú
	ReleaseMutex(info->mutex);					//§à§ä§á§å§ã§Ü§Ñ§Ö§Þ §Þ§î§ð§ä§Ö§Ü§ã

	for (int i = 0; i < FILES_COUNT; i++)
	{
		while (!info->flag) {};									 //§à§Ø§Ú§Õ§Ñ§Ö§Þ §æ§Ý§Ñ§Ô
		WaitForSingleObject(info->mutex, INFINITE);				 //§©§Ñ§ç§Ó§Ñ§ä §Þ§î§ð§ä§Ö§Ü§ã§Ñ
		(*writeDataInFile)(hFile, info->buffer);				 //§ã §á§à§Þ§à§ë§î§ð §æ§å§ß§Ü§è§Ú§Ú §Ò§Ú§Ò§Ý§Ú§à§ä§Ö§Ü§Ú §Ù§Ñ§á§Ú§ã§í§Ó§Ñ§Ö§Þ §Õ§Ñ§ß§ß§í§Ö §Ó §æ§Ñ§Û§Ý
		ReleaseMutex(info->mutex);								 //§à§ä§á§å§ã§Ü§Ñ§Ö§Þ §Þ§î§ð§ä§Ö§Ü§ã
		info->flag = 0;											 //§å§ã§ä§Ñ§ß§Ñ§Ó§Ý§Ú§Ó§Ñ§Ö§Þ §æ§Ý§Ñ§Ô §Ó 0
	}
	CloseHandle(hFile);											//§Ù§Ñ§Ü§â§í§Ó§Ñ§Ö§Þ §æ§Ñ§Û§Ý
	info->threadsNumber--;										//§Ù§Ñ§Ó§Ö§â§ê§Ñ§Ö§Þ §â§Ñ§Ò§à§ä§å §á§à§ä§à§Ü§Ñ
	return 0;
}

void initInfo(struct Info *info)
{
	info->fileNames = files;
	if (!(info->library = LoadLibrary("library.dll")))           //§°§ä§Ü§â§í§Ó§Ñ§Ö§Þ §Ò§Ú§Ò§Ý§Ú§à§ä§Ö§Ü§å
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
		(NULL, //§Ñ§ä§â§Ú§Ò§å§ä §Ò§Ö§Ù§à§á§Ñ§ã§ä§ß§à§ã§ä§Ú
		FALSE, //§æ§Ý§Ñ§Ô §ß§Ñ§é§Ñ§Ý§î§ß§à§Ô§à §Ó§Ý§Ñ§Õ§Ö§Ý§î§è§Ñ
		NULL); //§Ú§Þ§ñ §à§Ò§ì§Ö§Ü§ä§Ñ
}

void runThreads(struct Info *info)
{		/*§ã§à§Ù§Õ§Ñ§Ö§Þ §á§à§ä§à§Ü §é§ä§Ö§ß§Ú§ñ*/
	info->readThread = CreateThread(NULL,	//§Ñ§ä§â§Ú§Ò§å§ä§í §Ò§Ö§Ù§à§á§Ñ§ã§ä§ß§à§ã§ä§Ú §á§à§ä§à§Ü§Ñ									
		0,									//§â§Ñ§Ù§Þ§Ö§â §ã§ä§Ö§Ü§Ñ, §Ó§í§Õ§Ö§Ý§ñ§Ö§Þ§à§Ô§à §á§à§Õ §á§à§ä§à§Þ
		threadReader,					    //§Ñ§Õ§â§Ö§ã §á§à§ä§à§Ü§à§Ó§à§Û §æ§å§ß§Ü§è§Ú§Ú
		(void *)info,						//§å§Ü§Ñ§Ù§Ñ§ä§Ö§Ý§î §ß§Ñ §á§Ñ§â§Ñ§Þ§Ö§ä§â§í, §á§Ö§â§Ö§Õ§Ñ§Ó§Ñ§Ö§Þ§í§Ö §æ§å§ß§Ü§è§Ú§Ú §á§à§ä§à§Ü§Ñ §á§â§Ú §Ö§Ô§à §ã§à§Ù§Õ§Ñ§ß§Ú§Ú
		0,									//§æ§Ý§Ñ§Ô §ã§à§Ù§Õ§Ñ§ß§Ú§ñ §á§à§ä§à§Ü§Ñ (§á§à§ä§à§Ü §Ù§Ñ§á§å§ã§Ü§Ñ§Ö§ä§ã§ñ §ß§Ö§Þ§Ö§Õ§Ý§Ö§ß§ß§à §Ú§Ý§Ú §ã§à§Ù§Õ§Ñ§Ö§ä§ã§ñ §Ó §à§ã§ä§Ñ§ß§à§Ó§Ý§Ö§ß§ß§à§Þ §ã§à§ã§ä§à§ñ§ß§Ú§Ú
		NULL);								//§å§Ü§Ñ§Ù§Ñ§ä§Ö§Ý§î, §á§à §Ü§à§ä§à§â§à§Þ§å §Ò§å§Õ§Ö§ä §Ù§Ñ§á§Ú§ã§Ñ§ß §Ú§Õ§Ö§ß§ä§Ú§æ§Ú§Ü§Ñ§ä§à§â §ã§à§Ù§Õ§Ñ§ß§ß§à§Ô§à §á§à§ä§à§Ü§Ñ											
	info->threadsNumber++;
	info->writeThread = CreateThread(NULL, 0, threadWriter, (void *)info, 0, NULL);										     //§á§à§ä§à§Ü §Ù§Ñ§á§Ú§ã§Ú
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