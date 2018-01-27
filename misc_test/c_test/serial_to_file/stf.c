// stf.c
// Read from a serial port until the specified string is detected.
//
// Buffer size is set to 1024. Works if stop string is split between reads.
// (tested with buffer size of 1)
//
// James Marshall

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>

#define BUFF_SIZE 1024

// Nabbing some of this code (especiall set_interfac_attribs) from wallyk https://stackoverflow.com/a/6947758/1601162

int set_interface_attribs(int fd, int speed, int parity) {
  struct termios tty;
  memset (&tty, 0, sizeof tty);
  if (tcgetattr (fd, &tty) != 0) {
    printf("error from tcgetattr\n");
    return -1;
  }

  cfsetospeed (&tty, speed);
  cfsetispeed (&tty, speed);

  tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
  // disable IGNBRK for mismatched speed tests; otherwise receive break
  // as \000 chars
  tty.c_iflag &= ~IGNBRK;         // disable break processing
  tty.c_lflag = 0;                // no signaling chars, no echo,
  // no canonical processing
  tty.c_oflag = 0;                // no remapping, no delays
  tty.c_cc[VMIN]  = 0;            // read doesn't block
  tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

  tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
  // enable reading
  tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
  tty.c_cflag |= parity;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CRTSCTS;

  if (tcsetattr (fd, TCSANOW, &tty) != 0) {
    printf("error from tcsetattr\n");
    return -1;
  }
  return 0;
}

int main(int argc, char ** argv) {

  if (argc < 3) {
    fprintf(stderr, "Usage: serial_to_file <serial_port> <stop_string>\n");
    return -1;
  }

  char *portname = argv[1];
  char *search_string = argv[2];

  int serial_fd = open(portname, O_RDONLY | O_NOCTTY | O_SYNC);
  if (serial_fd < 0) {
    fprintf(stderr, "Failed to open serial port\n");
    return -1;
  }

  set_interface_attribs(serial_fd, B460800, 0);  // set speed to 115,200 bps, 8n1 (no parity)
  //  set_blocking (fd, 0);                // set no blocking

  int read_cnt = 0;
  int write_cnt = 0;
  char buff[BUFF_SIZE];
  int index = 0;
  int found_index = 0;
  bool done = false;

  while (!done) {
    read_cnt = read(serial_fd, buff, sizeof(buff));
    if (read_cnt) {

      write_cnt = write(STDOUT_FILENO, buff, read_cnt);
      if (write_cnt != read_cnt) {
        fprintf(stderr, "\n\nRead / Write Error\n\n");
        return -1;
      }

      // See if the stop string is in buffer... keeping in mind it might be between buffers.
      // Either I'm too clever or I'm too tired: I think this code elegantly handles the corner case.
      for (index = 0; index < BUFF_SIZE; index++) {
        if (buff[index] == search_string[found_index]) {
          printf("\nFound a %c\n", search_string[found_index]);
          found_index++;
          if (found_index == strlen(search_string)) {
            done = true;
          }
        } else {
          found_index = 0;
        }
      }
    }
  }

  return 0;
}
