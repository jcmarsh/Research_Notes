/*
 * main.c
 * 
 * Removes CCSDS headers from a binary file... or something. Not sure yet.
 *
 * February 9th, 2018 James Marshall
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define PRI_HEADER_SIZE 6
#define SEC_HEADER_SIZE 6
//#define PACKET_SIZE 1651
#define PACKET_MAX_SIZE 309

int main(int argc, char** argv) {
  int input_fd;
  int input_flags = O_RDONLY;
  int output_fd;

  int file_io_ret;
  unsigned char primary_header[PRI_HEADER_SIZE];

  bool sec_header_present;
  short unsigned int app_id;
  short unsigned int pkt_length;

  unsigned char secondary_header[SEC_HEADER_SIZE];

  unsigned char packet_buffer[PACKET_MAX_SIZE];

  int successful_count = 0;
  
  if (argc != 3) {
    printf("Call like this: ccsds_strip input_file output_file\n");
    exit(0);
  }
  
  input_fd = open(argv[1], input_flags);
  if (input_fd < 0) {
    perror("opening input file problem");
    goto end;
  }
  printf("fd is: %d\n", input_fd);

  output_fd = open(argv[2], O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (input_fd < 0) {
    perror("opening output file problem");
    goto cleanup_input;
  }
  printf("fd is: %d\n", output_fd);  

  while(true) {
    file_io_ret = read(input_fd, primary_header, PRI_HEADER_SIZE);
    if (file_io_ret != PRI_HEADER_SIZE) {
      perror("Failed to read full header");
      goto cleanup_file;
    }

    printf("\n\nCCSDS Header:\n");
    printf("\tFirst    2: 0x%02X%02X\n", primary_header[0], primary_header[1]);
    printf("\tSeq Ctl  2: 0x%02X%02X\n", primary_header[2], primary_header[3]);
    printf("\tPacket Len: 0x%02X%02X\n", primary_header[4], primary_header[5]);

    sec_header_present = (primary_header[0] & 0x08) >> 3;
    app_id = ((primary_header[0] << 8) + primary_header[1]) & 0x07FF;
    pkt_length = (primary_header[4] << 8) + primary_header[5];

    printf("Secondary Header? 0x%0X\n", sec_header_present);
    printf("App ID: 0x%03X (%d)\n", app_id, app_id);
    printf("Length: 0x%04X (%d)\n", pkt_length, pkt_length);

    if (sec_header_present) {
      file_io_ret = read(input_fd, secondary_header, SEC_HEADER_SIZE);
      if (file_io_ret != SEC_HEADER_SIZE) {
        perror("Failed to read secondary header");
        goto cleanup_file;
      }
      // Ignore secondary header
    } else {
      printf("Ignore packets without secondary headers\n");
      int toss_bytes = (pkt_length + 1);
      printf("Tossing %d bytes and tyring next packet\n", toss_bytes);
      
      file_io_ret = read(input_fd, packet_buffer, toss_bytes);
      if (file_io_ret != toss_bytes) {
        perror("Failed to read toss bytes\n");
        goto cleanup_file;
      }
      continue;
    }

    if ((pkt_length + 1) > (PACKET_MAX_SIZE + SEC_HEADER_SIZE)) {
      printf("Packet is not of the expected size\n");
      int toss_bytes = (pkt_length + 1) - SEC_HEADER_SIZE;
      printf("Tossing %d bytes and trying next packet\n", toss_bytes);

      file_io_ret = read(input_fd, packet_buffer, toss_bytes);
      if (file_io_ret != toss_bytes) {
        perror("Failed to read toss bytes\n");
        goto cleanup_file;
      }
      continue;
    }

    // Read raw data into packet buffer
    file_io_ret = read(input_fd, packet_buffer, (pkt_length + 1) - SEC_HEADER_SIZE);
    if (file_io_ret != (pkt_length + 1) - SEC_HEADER_SIZE) {
      perror("Failed to read full packet");
      goto cleanup_file;
    }

    printf("File segment Number: 0x%04X\n", (packet_buffer[1] << 8) + packet_buffer[0]);
    int seg_size = (packet_buffer[3] << 8) + packet_buffer[2];
    printf("File segment datasize: 0x%04X\n", seg_size);

    int index;
    for (index = 0; index < seg_size; index++) {
      printf("\tFD: 0x%02X\n", packet_buffer[4 + index]);
    }
    
    
    // write data to output file
    file_io_ret = write(output_fd, packet_buffer, (pkt_length + 1) - SEC_HEADER_SIZE);
    if (file_io_ret != (pkt_length + 1) - SEC_HEADER_SIZE) {
      perror("Failed to write to output file");
      goto cleanup_file;
    }

    successful_count++;
  }

 cleanup_file:
  printf("Read %d packets!\n", successful_count);
  close(output_fd);
 cleanup_input:
  close(input_fd);
 end:
  exit(0);
}
