#include "fileio.h"
#include <dirent.h> // ls
#include <fcntl.h>  // scope
#include <pwd.h> // для определения пользователя по UID
#include <stddef.h> // scope
#include <stdio.h>
#include <stdlib.h> // scope
#include <string.h>
#include <sys/stat.h> // scope
#include <sys/types.h> // scope // для определения пользователя по UID
#include <time.h>   // миги в секунды
#include <unistd.h> // scope

int move(char *src, char *dest) {
  printf("Перемещение файла из %s в %s\n", src, dest);
  if (rename(src, dest) == -1) {
    fprintf(stderr, "Ошибка перемещения файла\n");
    return 1;
  }
  return 0;
}

int copy(char *src, char *dest) {
  printf("Копирование файла из %s в %s\n", src, dest);
  FILE *srcFile = fopen(src, "r");
  if (!srcFile) {
    fprintf(stderr, "Файла не существует\n");
    return 1;
  }
  FILE *destFile = fopen(dest, "w+");
  if (!destFile) {
    fprintf(stderr, "Ошибка создания файла\n");
    return 1;
  }
  char c;
  while ((c = fgetc(srcFile)) != EOF) {
    fputc(c, destFile);
  }
  fclose(srcFile);
  fclose(destFile);
  return 0;
}

int delete (char *file) {
  printf("Удаление файла %s\n", file);
  if (remove(file) == -1) {
    fprintf(stderr, "Ошибка удаления файла\n");
    return 1;
  }
  return 0;
}

long scope(char *fileName) {
  printf("Узнать размер %s\n", fileName);
  long size = 0;

  int descriptor = open(fileName, O_RDONLY);

  if (descriptor != -1) {
    FILE *file = fdopen(descriptor, "rb");

    if (file) {
      struct stat statistics;

      if (fstat(descriptor, &statistics) != -1) {
        if (S_ISDIR(statistics.st_mode)) {
          printf("Это директория\n");

          DIR *dir;
          struct dirent *ent;
          if ((dir = opendir(fileName)) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
              if (!((strcmp(ent->d_name, ".") == 0) ||
                    (strcmp(ent->d_name, "..") == 0))) {
                char buf_path[256] = "";
                strcat(buf_path, fileName);
                strcat(buf_path, ent->d_name);
                size += scope(buf_path);
                printf("size %s = %ld\n", ent->d_name, size);
              }
            }
            closedir(dir);
          } else {
            fprintf(stderr, "Ошибка поиска файлов в директории\n");
            return -1;
          }

          printf("Размер директории = %ld байт\n", size);
          return size;
        }
        size = statistics.st_size;
        printf("Размер файла = %ld байт\n", statistics.st_size);
      }
      fclose(file);
    }
    close(descriptor);
  } else {
    fprintf(stderr, "Ошибка получения размера файла %s\n", fileName);
    return -1;
  }

  return size;
}

int ls(char *name) {
  printf("Отображение всех файлов в указанной директории\n");
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir(name)) != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      printf("%s\n", ent->d_name);
    }
    closedir(dir);
  } else {
    fprintf(stderr, "Ошибка поиска файлов в директории\n");
    return -1;
  }
  return 0;
}

int procfs() {
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir("/proc/")) != NULL) {
    printf("UID\tPID\tPPID\tC\tTIME\tSTIME\tCMD\n");
    while ((ent = readdir(dir)) != NULL) {
      if (atoi(ent->d_name) != 0) {
        char pwd[256] = "/proc/";
        strcat(pwd, ent->d_name);

        FILE *tmp;
        char pwd_tmp[256] = "";
        strcat(pwd_tmp, pwd);
        tmp = fopen(strcat(pwd_tmp, "/loginuid"), "r");
        if (!tmp) {
          fprintf(stderr, "Ошибка в получении uid, неверный файл %s\n", pwd);
          return 1;
        }
        int uid;
        if (fscanf(tmp, "%d", &uid) == EOF) {
          fprintf(stderr, "Ошибка в получении uid\n");
          return 1;
        }
        if (uid < 0)
          uid = 0;
        struct passwd *newPasswd;

        newPasswd = getpwuid(uid);

        char *name = malloc(sizeof(char) * 256);
        strcpy(name, newPasswd->pw_name);

        int pid;
        int ppid;
        long c;
        unsigned long time;
        unsigned long stime;
        char *cmd = malloc(sizeof(char) * 256);

        strcpy(pwd_tmp, pwd);
        tmp = fopen(strcat(pwd_tmp, "/stat"), "r");
        if (!tmp) {
          fprintf(stderr, "Ошибка в получении pid, неверный файл %s\n", pwd);
          return 1;
        }

        int buf1;
        long buf2;
        char buf3;
        char buf4[256];
        if (fscanf(tmp,
                   "%d %s %c %d %d %d %d %d %lu %lu %lu %lu %lu %lu %lu %ld "
                   "%ld %ld %ld",
                   &pid, buf4, &buf3, &ppid, &buf1, &buf1, &buf1, &buf1, &buf2,
                   &buf2, &buf2, &buf2, &buf2, &time, &stime, &buf2, &buf2,
                   &buf2, &c) == EOF) {
          fprintf(stderr, "Ошибка в получении pid\n");
          return 1;
        }
        fclose(tmp);

        strcpy(pwd_tmp, pwd);
        tmp = fopen(strcat(pwd, "/cmdline"), "r");
        if (!tmp) {
          fprintf(stderr, "Ошибка в получении cmd, неверный файл %s\n", pwd);
          return 1;
        }
        if (fscanf(tmp, "%s", cmd) == EOF) {
          cmd = "";
        }

        printf("%s\t%d\t%d\t%ld\t%lu s\t%lu s\t%s\n", name, pid, ppid, c,
               time / sysconf(_SC_CLK_TCK), stime / sysconf(_SC_CLK_TCK), cmd);
      }
    }
    closedir(dir);
  } else {
    fprintf(stderr, "Ошибка в procfs\n");
    return -1;
  }
  return 0;
}
