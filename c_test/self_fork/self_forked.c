/*
 * Artificial Potential controller (instead of Threaded Driver)
 */

#include <libplayerc/playerc.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Configuration parameters
#define VEL_SCALE 1
#define DIST_EPSILON .1
#define GOAL_RADIUS 0
#define GOAL_EXTENT 1
#define GOAL_SCALE 1
#define OBST_RADIUS 0
#define OBST_EXTENT 1
#define OBST_SCALE .3

// player interfaces
//playerc_client_t *client;
//playerc_position2d_t *pos2d; // Check position, send velocity command
//playerc_planner_t *planner; // Check for new position goals
//playerc_laser_t *laser; // laser sensor readings

// Controller state
bool active_goal;
double goal_x, goal_y, goal_a;

char ip[17];
char port[10]; // I don't know the maximum length for a port
int id;

void restartHandler(int signo) {
  // Need to restart a replica with (id + 1) % REP_COUNT
  printf("Shit... this may work\n");
}

int setupArtPot(int argc, const char **argv) {
  int i;

  if (signal(SIGUSR1, restartHandler) == SIG_ERR) {
    puts("Failed to register the restart handler");
    return -1;
  }

  if (argc < 4) {
    puts("Usage: art_pot <ip_address> <port> <position2d id>");
    return -1;
  }

  id = atoi(argv[3]);
  strncpy(ip, argv[1], sizeof(ip));
  strncpy(port, argv[2], sizeof(port));

  printf("Args parsed: \n");
  printf("\tip: %s\n", ip);
  printf("\tport: %s\n", port);
  printf("\tnum: %d\n", id);
}

void shutdownArtPot() {

}

int main(int argc, const char **argv) {

  if (setupArtPot(argc, argv) < 0) {
    puts("ERROR: failure in setup function.");
    return -1;
  }

  // should loop until waypoints are received? Or does this block?

  while(1) { // while something else.

  }

  shutdownArtPot();
  return 0;
}

