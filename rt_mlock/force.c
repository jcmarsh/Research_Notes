/*
 * I need to figure out how to force a child process to copy all
 * pages on creation (instead of copy-on-write).
 *
 * The idea is simple: dummy writes to every page.
 * Need to be able to find each page the process has r/w access to
 * Aught to be somewhere in /proc/<pid>/?
 *
 * author: James Marshall jcmarsh@gwmail.gwu.edu April 27, 2014
 */

#include "force.h"

int parseLine(char* line) {
  char start_s[17]; // I don't think that the address can be longer than 16 (64 bits in hex). One for \0
  char end_s[17];
  char permissions[5]; // read, write, execute, private (copy on write!)
  unsigned long start = 0;
  unsigned long end = 0;
  int index = 0;
  int e_index = 0;
  char current;
  bool readable = false, writable = false;
  unsigned long current_address = 0;
  char* address;
  char single_byte;

  // Do I need to mess with the offset?

  // Read start / convert to number
  current = line[index];
  while (current != '-') {
    start_s[index] = current;
    index++;
    current = line[index];
  }
  start_s[index] = '\0';
  start = strtoull(start_s, NULL, 16);

  // Read end / convert to number
  index++; // skip the '-'
  current = line[index];
  while (current != ' ') {
    end_s[e_index] = current;
    index++;
    e_index++;
    current = line[index];
  }
  end_s[e_index] = '\0';
  end = strtoull(end_s, NULL, 16);

  // printf("Start-End: %s - %s\n", start_s, end_s);
  // printf("\tStart-End: 0x%lX - 0x%lX\n", start, end);

  // read permission for just rw to start
  index++; // skip the ' '
  current = line[index];
  if (current == 'r') {
    readable = true;
    // read permissions
  }
  index++;
  current = line[index];
  if (current == 'w') {
    if (readable) {
      //      printf("Cand:");
    }
    writable = true;
    // write permissions
  }  
  index++;
  current = line[index];
  if (current == 'x') {
    // execute permissions
  }
  index++;
  current = line[index];
  if (current == 'p') {
    //    printf("Private: ");
    // private (copy on write) permissions
  }

  // if rw, then 
  if (readable && writable) {
    current_address = start;
    while (current_address < end) {
      // printf("\tRW: address 0x%lX\n", current_address);
      // read a byte, write a byte
      single_byte = *((char *)current_address);
      *((char*)current_address) = single_byte;
      // increase address by page size   
      // TODO: must be a better way to get this. Also, what about huge pages?
      current_address += 0x800;
    }
  }

  return 0;
}

int forceMaps( ){
  FILE* mapsfile;
  char line[200] = {0}; // 100 was too small
  size_t len = 0;
  ssize_t read;

  // open /proc/self/maps
  mapsfile = fopen("/proc/self/maps", "r");
  if (mapsfile == NULL) {
    printf("Failed to open /proc/self/maps\n");
    return -1;
  }

  // parse line by line
  // while ((read = getline(&line, &len, mapsfile)) != -1) {
  while (fgets(line, 200, mapsfile) != NULL) {
    parseLine(line);
  }

  fclose(mapsfile);

  return 0;
}

int lockItUp() {
  // Must be super user.
  if (mlockall(MCL_CURRENT | MCL_FUTURE) == -1) { // Lock current pages and future ones.
    perror("mlockall failed");
    return -1;
  }

  return 0;
}
