#include "fileio.h"
#include <dirent.h> // ls
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> // работа с процессами

void console() {
  // режим консоли
  char *buf = malloc(255);
  printf("\nВведите новую команду:\n");

  fgets(buf, 255, stdin);
  // printf("%s\n", buf);

  char **param = malloc(255);
  for (int i = 0; i < 255; i++) {
    param[i] = malloc(255);
  }

  strcpy(param[0], "./bin/proc");

  char *istr;
  int j = 1;
  istr = strtok(buf, " ");
  while (istr != NULL) {
    strcpy(param[j], istr);
    // printf("%s\n", param[j]);
    istr = strtok(NULL, " ");
    j++;
  }
  if (j > 1) {
    int count = strlen(param[j - 1]);
    param[j - 1][count - 1] = '\0';
    // printf("count = %d\n", count);
  }
  param[j] = NULL;

  execv("./bin/proc", param);
  // конец режима консоли
}

void log_out(int signo) {
  printf("%d\n", signo);
  signal(20, log_out);
}

int create(char *name, char *args[]) {
  printf("name = %s\n", name);

  int m = 0;
  while (args[m] != NULL) {
    printf("args[%d] = %s\n", m, args[m]);
    m++;
  }

  int child_status;
  pid_t child = fork();
  if (child == 0) {
    if (execv(name, args) == -1) {
      fprintf(stderr, "Невозможные параметры для запуска процесса\n");
      return 1;
    }
  } else {
    wait(&child_status);
    printf("done\n");
  }
  // printf("hello 2 %d\n", getpid());
  // printf("hello 3 %d\n", getppid());
  return 0;
}

char *searchPath(char *name) {
  char *path = getenv("PATH");
  // printf("PATH :%s\n", (path != NULL) ? path : "getenv returned NULL");

  char **path_s = malloc(255);
  for (int i = 0; i < 255; i++) {
    path_s[i] = malloc(255);
  }

  char *istr;
  int j = 0;
  istr = strtok(path, ":");
  while (istr != NULL) {
    DIR *dir;
    struct dirent *ent;

    strcpy(path_s[j], istr);
    printf("PAAAAAAAAAAAAAAAAAAAAAAAATH%s\n", path_s[j]);
    if ((dir = opendir(path_s[j])) != NULL) {
      while ((ent = readdir(dir)) != NULL) {
        if (!strcmp(ent->d_name, name)) {
          strcat(path_s[j], "/");
          strcat(path_s[j], name);
          printf("%s\n", path_s[j]);
          // printf("%d\n", execl(path_s[j], "java", "-version", NULL));
          return path_s[j];
        }
      }
      closedir(dir);
    }

    istr = strtok(NULL, ":");
    j++;
  }

  return NULL;
}

int main(int argc, char **argv) {

  int m = 0;
  while (argv[m] != NULL) {
    printf("argv[%d] = %s\n", m, argv[m]);
    m++;
  }
  if (argc < 2) {
    fprintf(
        stderr,
        "Не хватает аргументов командной строки, воспользуйтесь --help|-h\n");
    return 0;
  }

  if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
    printf(
        "Авторы: Виктория Гацуля\n"
        "Утилита proc предназначена для работы с файловой системой, а именно:\n"
        "--help \t\t\t\t\t вывод меню помощи\n"
        "--move ./file.txt ./bin/file.txt \t переместить файл "
        "file.txt из текущей директории в папку bin\n"
        "--copy ./file.txt ./bin/file.txt \t копировать файл "
        "file.txt из текущей директории в папку bin\n"
        "--delete ./file.txt \t\t\t удалить файл file.txt из текущей "
        "директории\n"
        "--scope ./file.txt \t\t\t узнать размер файла file.txt\n"
        "--scope ./bin \t\t\t\t узнать размер директории bin\n"
        "--ls ./bin \t\t\t\t отобразить название всех файлов в "
        "директории bin\n"
        "--procfs \t\t\t\t отобразить все процессы файловой системы "
        "procfs\n");

  } else if (!strcmp(argv[1], "--move") || !strcmp(argv[1], "-m")) {
    if (argc < 4) {
      fprintf(stderr, "Недостаточно аргументов\n");
      return 0;
    }

    move(argv[2], argv[3]);

  } else if (!strcmp(argv[1], "--copy") || !strcmp(argv[1], "-c")) {
    if (argc < 4) {
      fprintf(stderr, "Недостаточно аргументов\n");
      return 0;
    }

    copy(argv[2], argv[3]);

  } else if (!strcmp(argv[1], "--delete") || !strcmp(argv[1], "-d")) {
    if (argc < 3) {
      fprintf(stderr, "Недостаточно аргументов\n");
      return 0;
    }

    delete (argv[2]);

  } else if (!strcmp(argv[1], "--scope") || !strcmp(argv[1], "-s")) {
    if (argc < 3) {
      fprintf(stderr, "Недостаточно аргументов\n");
      return 0;
    }

    scope(argv[2]);

  } else if (!strcmp(argv[1], "--ls") || !strcmp(argv[1], "-l")) {
    if (argc < 3) {
      argv[2] = "./";
    }

    ls(argv[2]);

  } else if (!strcmp(argv[1], "--procfs") || !strcmp(argv[1], "-p")) {

    procfs();

  } else if (!strcmp(argv[1], "--create") || !strcmp(argv[1], "-r")) {
    if (argc < 3) {
      fprintf(stderr, "Недостаточно аргументов\n");
      return 0;
    }

    if (argc >= 4) {
      if (!strcmp(argv[2], "--fone") || !strcmp(argv[2], "-f")) {
        int count = argc;
        char **buf = malloc(count);
        for (int i = 0; i < count; i++) {
          buf[i] = malloc(255);
        }
        buf[0] = "nohup";
        strcpy(buf[1], searchPath(argv[3]));
        for (int i = 0; i < (argc - 4); i++) {
          strcpy(buf[i + 2], argv[i + 4]);
        }
        strcpy(buf[count - 2], "&");
        buf[count - 1] = 0;

        create("/usr/bin/nohup", buf);
      } else {

        char **buf = malloc(255);
        for (int i = 0; i < argc - 1; i++) {
          buf[i] = malloc(255);
        }

        int y = 0;
        while (argv[y + 2] != NULL) {
          strcpy(buf[y], argv[y + 2]);
          y++;
        }
        buf[y] = NULL;
        create(searchPath(argv[2]), buf);
      }
    }

  } else if (!strcmp(argv[1], "--console") || !strcmp(argv[1], "-n")) {

  } else {
    printf("Неизвестная опция %s, воспользуйтесь --help|-h\n", argv[1]);
  }

  console();

  while (1) {
    // обработка сигналов
    signal(20, log_out);
  }
  return 0;
}

/*
С полным путём: нужно мочь программно находить путь which java (dir open и так
далее) Сделать строку ввода команд sigaction - брать любой из сигналов


jobs - посмотреть запущенные процессы в этой оболочке
nohup ./example/hello & - запустить процесс в фоновом режиме
jobs
fg n - вернуться к процессу, который сейчас в фоне (n - номер процесса в списке)
*/
