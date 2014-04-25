/*
 * I need to figure out how to force a child process to copy all
 * pages on creation (instead of copy-on-write).
 *
 * The idea is simple: dummy writes to every page.
 * Need to be able to find each page the process has r/w access to
 * Aught to be somewhere in /proc/<pid>/?
 */

#include "force.h"

/* An example of /proc/self/maps
00400000-0040b000 r-xp 00000000 08:01 142307                             /bin/cat
0060a000-0060b000 r--p 0000a000 08:01 142307                             /bin/cat
0060b000-0060c000 rw-p 0000b000 08:01 142307                             /bin/cat
01c98000-01cb9000 rw-p 00000000 00:00 0                                  [heap]
7fc177955000-7fc178038000 r--p 00000000 08:01 13769022                   /usr/lib/locale/locale-archive
7fc178038000-7fc1781ed000 r-xp 00000000 08:01 9446173                    /lib/x86_64-linux-gnu/libc-2.15.so
7fc1781ed000-7fc1783ed000 ---p 001b5000 08:01 9446173                    /lib/x86_64-linux-gnu/libc-2.15.so
7fc1783ed000-7fc1783f1000 r--p 001b5000 08:01 9446173                    /lib/x86_64-linux-gnu/libc-2.15.so
7fc1783f1000-7fc1783f3000 rw-p 001b9000 08:01 9446173                    /lib/x86_64-linux-gnu/libc-2.15.so
7fc1783f3000-7fc1783f8000 rw-p 00000000 00:00 0 
7fc1783f8000-7fc17841a000 r-xp 00000000 08:01 9446185                    /lib/x86_64-linux-gnu/ld-2.15.so
7fc1785f5000-7fc1785f8000 rw-p 00000000 00:00 0 
7fc178618000-7fc17861a000 rw-p 00000000 00:00 0 
7ffffcafb000-7ffffcafc000 r-xp 00000000 00:00 0                          [vdso]
ffffffffff600000-ffffffffff601000 r-xp 00000000 00:00 0                  [vsyscall]
*/
int parseLine(char* line, ssize_t length) {
  char start_s[17]; // I don't think that the address can be longer than 16 (64 bits in hex). One for \0
  char end_s[17];
  char permissions[5]; // read, write, execute, private (copy on write!)
  long start = 0;
  long end = 0;
  int index = 0;
  int e_index = 0;
  char current;
  bool readable = false, writable = false;
  long current_address = 0;
  char* address;
  char single_byte;

  // Do I need to mess with the offset?

    return 0;

  // Read start / convert to number
  current = line[index];
  while (current != '-') {
    start_s[index] = current;
    index++;
    current = line[index];
  }
  start_s[index] = '\0';
  start = strtol(start_s, NULL, 16);

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
  end = strtol(end_s, NULL, 16);

  //  printf("Start-End: %s - %s\n", start_s, end_s);
  //  printf("Start-End: %ld - %ld\n", start, end);

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

  //  printf("%s", line);
  // Offset needed?

  // if rw (or just p?), then 
  if (readable && writable) {
    current_address = start;
    while (current_address < end) {
      printf("Dummy write: %p\n", (char *)current_address);
      single_byte = *((char *)current_address);
      single_byte ^ 1;
      single_byte ^ 1;
      *((char*)current_address) = single_byte;
      current_address += 0x800;
    }
  }
  //         read a byte, write a byte
  //         increase address by page size


  // Simple, right?
  return 0;
}

int printMaps( ){
  FILE* mapsfile;
  char* line = NULL;
  size_t len = 0;
  ssize_t read;

  // open /proc/self/maps
  mapsfile = fopen("/proc/self/maps", "r");
  if (mapsfile == NULL) {
    printf("Failed to open /proc/self/maps\n");
    return -1;
  }

  // parse line by line
  while ((read = getline(&line, &len, mapsfile)) != -1) {
    printf("%s", line);
  }
  printf("\n\n");

  if (line) {
    free(line);
  }
}

int forceMaps( ){
  FILE* mapsfile;
  char* line = NULL;
  size_t len = 0;
  ssize_t read;

  // open /proc/self/maps
  mapsfile = fopen("/proc/self/maps", "r");
  if (mapsfile == NULL) {
    printf("Failed to open /proc/self/maps\n");
    return -1;
  }

  // parse line by line
  while ((read = getline(&line, &len, mapsfile)) != -1) {
    parseLine(line, read);
  }
  printf("\n\n");

  if (line) {
    free(line);
  }
}


//int main(int argc, char** argv) {
  // Need own pid
  // OR just looko in /proc/self

  // look in /proc/self/mem or /proc/self/maps or /proc/self/pagemap
  // pagemap: https://www.kernel.org/doc/Documentation/vm/pagemap.txt
  // Looks like mem or maps
  // mem is only memory that is paged in... should be fine: using mlockall
  // I think I should be able to do it with just /maps

//  printMaps();


//  return 0;

  // How do I verify this? Fork and then examine /mems?
//}
