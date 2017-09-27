// Based off of the example code from Rijndael's reference AES implementation.

#include <stdio.h>
#include <string.h>
#include "rijndael.h"

#define KEYBITS 256

int main (int argc, char** argv) {
  int i;
  int nrounds;
  char *password = "SuperSecret!";
  unsigned long rk[RKLENGTH(KEYBITS)];
  unsigned char key[KEYLENGTH(KEYBITS)];

  unsigned char message[] = "The brown cow sleeps in Moscow. The time in Greenwich is unkind. Two eagles can not share a pillory.";
  unsigned char encrypted[] = "The brown cow sleeps in Moscow. The time in Greenwich is unkind. Two eagles can not share a pillory.                 "; // Will be overwritten. Extra spaces are for potential padding.
  int message_bytes = strlen(message);

  
  for (i = 0; i < sizeof(key); i++) {
    key[i] = *password != 0 ? *password++ : 0;
  }
  
  nrounds = rijndaelSetupEncrypt(rk, key, 256);
  int m_index = 0;
  while (m_index < message_bytes + (message_bytes % 16)) {
    unsigned char plaintext[16];
    unsigned char ciphertext[16];
    int j;
    for (j = 0; j < sizeof(plaintext); j++)
    {
      m_index++;
      if (m_index > message_bytes) {
        plaintext[j] = ' ';
      } else {
        plaintext[j] = message[m_index];
      }
    }
    if (j == 0)
      break;
    for (; j < sizeof(plaintext); j++) {
      plaintext[j] = ' ';
    }
    rijndaelEncrypt(rk, nrounds, plaintext, ciphertext);
    for (j = 0; j < sizeof(plaintext); j++) {
      encrypted[m_index - sizeof(plaintext) + j] = ciphertext[j];
    }
  }

  printf("Message to encrypt:\n\t%s\n", message);
  printf("Encrypted:\n\t%s\n", encrypted);


  // Decrypt time
  
  nrounds = rijndaelSetupDecrypt(rk, key, 256);
  m_index = 0;
  while (m_index < message_bytes + (message_bytes % 16)) {
    printf("m_index: %d less than %d, message_bytes: %d\n", m_index, message_bytes + (message_bytes % 16), message_bytes);
    unsigned char plaintext[16];
    unsigned char ciphertext[16];
    int j;

    for (j = 0; j < sizeof(ciphertext); j++) {
      int c = encrypted[m_index];
      m_index++;
      ciphertext[j] = c;
    }
    rijndaelDecrypt(rk, nrounds, ciphertext, plaintext);
    for (j = 0; j < sizeof(ciphertext); j++) {
      encrypted[m_index - sizeof(ciphertext) + j] = plaintext[j];
    }
  }

  printf("Decrypted:\n\t%s\n", encrypted);
  
  return 0;
}


