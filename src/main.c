#include "fileio.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {

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
    }

    copy(argv[2], argv[3]);
  } else if (!strcmp(argv[1], "--delete") || !strcmp(argv[1], "-d")) {
    if (argc < 3) {
      fprintf(stderr, "Недостаточно аргументов\n");
    }

    delete (argv[2]);
  } else if (!strcmp(argv[1], "--scope") || !strcmp(argv[1], "-s")) {
    if (argc < 3) {
      fprintf(stderr, "Недостаточно аргументов\n");
    }

    scope(argv[2]);
  } else if (!strcmp(argv[1], "--ls") || !strcmp(argv[1], "-l")) {

    ls(argv[2]);
  } else if (!strcmp(argv[1], "--procfs") || !strcmp(argv[1], "-p")) {

    procfs(argv[2]);
  } else {
    printf("Неизвестная опция %s, воспользуйтесь --help|-h\n", argv[1]);
  }

  return 0;
}
