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
    printf("Здесь будут подсказки\n");
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
