/*
 * fib.c
 * 
 * Reads n from a file, calculates nth fib, writes result to a file
 * At least, that is what it ought to do.
 *
 * March 19, 2014 James Marshall
 */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include "force.h"

#define FAB_FIB 39
#define COUNT 5
#define BUFF_SIZE 100

// Find the nth fibonacci number
unsigned long fib(int n) {
  if (n == 1 || n == 2) {
    return 1;
  } else {
    return fib(n-1) + fib(n-2);
  }
}

int main(int argc, char** argv) {
  int wo_fd;
  int wo_flags = O_CREAT | O_WRONLY;
  mode_t wo_mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  char *wo_filename = "./out.txt";
  int count = 0;
  long result;
  char buffer[BUFF_SIZE] = {0};
  pid_t currentPID = 0;

  double AHHAHAHA[4096];
  void* MUHAHAHA;

  // open file with n's -- skip for now, just write outputs
  int ns[COUNT] = {FAB_FIB, FAB_FIB, FAB_FIB, FAB_FIB, FAB_FIB};

  MUHAHAHA = malloc(sizeof(double) * 32768);

  //mlockall(MCL_CURRENT);
  mlockall(MCL_FUTURE);

  //  printMaps();

  currentPID = fork();
  if (currentPID == 0) {
    forceMaps();
  }

  // open file to write to
  wo_fd = open(wo_filename, wo_flags, wo_mode);
  if (wo_fd < 0) {
    perror("opening file problem");
  }
  printf("fd is: %d", wo_fd);

  // DELETE THIS
  //  wo_fd = open("another.txt", wo_flags, wo_mode);
  //  printf("fd is: %d", wo_fd);
  //  wo_fd = open("thisisannoying.txt", wo_flags, wo_mode);
  //  printf("fd is: %d", wo_fd);

  // while a number is left
  while (count < COUNT) {
    result = fib(ns[count]);
    count++;
    snprintf(buffer, BUFF_SIZE, "%lu ", result);
    //    write(wo_fd, buffer, BUFF_SIZE);
    memset(buffer, 0, BUFF_SIZE);
  }

  // Clean up
  close(wo_fd);

  printMaps();

  while (1) {
    // for reasons!
  }
  return 0;
}
