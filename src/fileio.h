#ifndef FILEIO_H
#define FILEIO_H

int move(char *src, char *dest);
int copy(char *src, char *dest);
int delete (char *file);
long scope(char *fileName);
int ls(char *name);
int procfs();

#endif
