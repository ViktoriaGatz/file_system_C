#include "fileio.h"
#include <dirent.h> // ls
#include <fcntl.h>  // scope
#include <stddef.h> // scope
#include <stdio.h>
#include <stdlib.h>    // scope
#include <sys/stat.h>  // scope
#include <sys/types.h> // scope
#include <unistd.h>    // scope

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
  printf("Узнать размер файла %s\n", fileName);
  long size = 0;

  int descriptor = open(fileName, O_RDONLY);

  if (descriptor != -1) {
    FILE *file = fdopen(descriptor, "rb");

    if (file) {
      struct stat statistics;

      if (fstat(descriptor, &statistics) != -1) {
        size = statistics.st_size;
        printf("Размер файла/директории = %ld байт\n", statistics.st_size);
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
  // DIR *dir;
  // struct dirent *ent;
  // if ((dir = opendir(name)) != NULL) {
  //   while ((ent = readdir(dir)) != NULL) {
  //     if (atoi(ent->d_name) != 0) {
  //       // help
  //     }
  //   }
  //   closedir(dir);
  // } else {
  //   fprintf(stderr, "Ошибка в procfs\n");
  //   return -1;
  // }
  return 0;
}
