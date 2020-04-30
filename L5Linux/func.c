#include "header.h"

#define FILES_COUNTER 5

char *files[FILES_COUNTER] = {"./Files/1", "./Files/2", "./Files/3", "./Files/4", "./Files/5"};     //Каталог файлов для чтения

void *threadReader(void *arg)
{
  struct Info *info = (struct Info *)arg;                                              //Приводим к типу аргумент функции
  void (*lib_function)(int, char *);                                                   //Создаем указатель на функцию из библиотеки
  *(void **)(&lib_function) = dlsym(info->library, "readFromFile");                    //Загружаем функцию readFromFile из библиотеки

  for (int i = 0; i < FILES_COUNTER; i++)                                              //Цикл прохода по файлам      
  {
    while (info->flag);                                                                //Ожидаем флаг
    pthread_mutex_lock(&info->mutex);                                                  //Захватываем мьютекс
    printf("File name: %s\n", info->fileNames[i]);                                     //Вывод информации на экран
    int fd = open(info->fileNames[i], O_RDONLY);                                       //Открываем файл для чтения
    if (fd < 0)                                                                        //Если файл не открылся
    {
      printf("File not found");
      exit(-1);
    }
    (*lib_function)(fd, info->buffer);                                                 //используем нашу функцию чтения
    close(fd);                                                                         //Закрываем файл
    pthread_mutex_unlock(&info->mutex);                                                //Отпускаем мьютекс
    info->size++;
    info->flag = 1;
    usleep(1);
  }
  info->threadsNumber--;
  return NULL;
}

void *threadWriter(void *arg)
{
  struct Info *info = (struct Info *)arg;                             //Приводим к типу аргумент функции
  void (*lib_function)(int, char *);
  *(void **)(&lib_function) = dlsym(info->library, "writeToFile");    //Загружаем функцию writeToFile из библиотеки
  pthread_mutex_lock(&info->mutex);				      //Захватываем мьютекс

			/*Открываем файл для записи с флагами доступа*/
/*
O_WRONLY - открыть файл только для записи
O_CREAT - если файл не существует то он будет создан
O_APPEND - Файл открывается в режиме добавления
O_TRUNC - если файл уже существует, он является обычным файлом и режим позволяет записывать в этот файл
S_IRUSR - пользователь имеет права на чтение файла
S_IWUSR - пользователь имеет права на запись информации в файл
S_IRGRP - группа имеет права на чтение файла
S_IROTH - все остальные имеют права на чтение файла
*/
  int fd = open("./Files/OUTPUT", O_WRONLY | O_CREAT | O_APPEND                                      
                                | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);


  pthread_mutex_unlock(&info->mutex);    	                //Освобождаем мьютекс
  usleep(1);
  while (!info->flag);
  for (int i = 0; i < FILES_COUNTER; i++)
  {
    while (!info->flag);
    pthread_mutex_lock(&info->mutex);				//захватываем мьютекс
    printf("Write message %s\n", info->buffer);			//выводим наше сообщение на экран
    (*lib_function)(fd, info->buffer);                          //Записываем данные
    pthread_mutex_unlock(&info->mutex);				//освобождаем мьютекс
    info->flag = 0;
    usleep(1);
  }
  close(fd);
  info->threadsNumber--;
  return NULL;
}

void initInfo(struct Info *info)
{
  info->fileNames = files;
  info->size = 0;
  if (!(info->library = dlopen("./library.so", RTLD_LAZY)))             //Открываем библиотеку
  {
    printf("Library.so is not found\n");
    exit(-3);								//выход в случае ошибки
  }
  info->threadsNumber = 0;                                              //Устанавливаем количество потоков в 0
  info->flag = 0;							//устанавливаем флаг в 0
}

void createMutex(struct Info *info)	//Создание мьютекса
{
  pthread_mutex_init(&info->mutex, 	//указатель на мьютекс	
			     NULL);     //атрибут мьютекса, в данном случае - по умолчанию                               
}

void runThreads(struct Info *info)
{

	//Создание потока чтения
  pthread_create(&info->readThread, //адрес для хранения идентификатора создаваемого потока
NULL,  				    //атрибуты по умолчанию
&threadReader, 
info);                              //указатель на аргументы потока
  info->threadsNumber++;
	//создания потока записи
  pthread_create(&info->writeThread, NULL, &threadWriter, info);
  info->threadsNumber++;
}


void waitThreads(struct Info *info)
{
  while (info->threadsNumber);                                                                     //Ожидаем завершение потоков
  printf("OK\n");
  dlclose(info->library);                                                                          //Закрываем библиотеку
}
