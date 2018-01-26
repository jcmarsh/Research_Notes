// stf.c
// Read from a serial port to a file until the specified string is detected.
// James Marshall

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include <stdio.h>

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

#define BUFF_SIZE 1024

int main(int argc, char ** argv) {
  // TODO: args should be the port, output file name, and search string
  char *portname = "/dev/ttyUSB1";
  char *search_string = "safeword";
  char *output_fn = "stf_output.txt";
  
  int output_fd = open(output_fn, O_CREAT | O_WRONLY);
  if (output_fd < 0) {
    perror("Yeah?");
    printf("Failed to open output text file\n");
    return -1;
  }

  int serial_fd = open(portname, O_RDONLY | O_NOCTTY | O_SYNC);
  if (serial_fd < 0) {
    printf("Failed to open serial port\n");
    return -1;
  }

  set_interface_attribs(serial_fd, B460800, 0);  // set speed to 115,200 bps, 8n1 (no parity)
  //  set_blocking (fd, 0);                // set no blocking

  //write (fd, "hello!\n", 7);           // send 7 character greeting

  //usleep ((7 + 25) * 100);             // sleep enough to transmit the 7 plus
                                       // receive 25:  approx 100 uS per char transmit

  int n = 0;
  char buf[BUFF_SIZE];

  while (1) {
    n = read(serial_fd, buf, sizeof(buf));  // read up to 100 characters if ready
    if (n) {
      printf("n: %d\n", n);
      printf("%s\n", buf);
    }
  }

  printf("Output: %s\n", buf);

  return 0;
}
