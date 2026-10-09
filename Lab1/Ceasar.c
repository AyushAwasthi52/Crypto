#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SIZE 100

typedef struct {
  char data[MAX_SIZE];
  size_t length;
} String;

void secure_claer(void *v, size_t n) {
  volatile unsigned char *p = v;
  while (n--) {
    *p++ = 0;
  }
}

void inputString(String *s) {
  if (fgets(s->data, MAX_SIZE, stdin) != NULL) {
    s->data[strcspn(s->data, "\n")] = '\0';
    s->length = strlen(s->data);
  }
}

void clear_input_buffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
}

String encryptCeasar(String plaintext, int key) {
  String ciphertext;
  for (int i=0 ; i<plaintext.length ; i++) {
    if (plaintext.data[i] >= 97) {
      int ind = plaintext.data[i] - 'a';
      ind = (ind + key) % 26;
      ciphertext.data[i] = 'a'+ind;
    } else {
      int ind = plaintext.data[i] - 'A';
      ind = (ind + key) % 26;
      ciphertext.data[i] = 'A'+ind;
    }
  }
  ciphertext.data[plaintext.length] = '\0';
  ciphertext.length = plaintext.length;
  return ciphertext;
}

String decryptCeasar(String ciphertext, int key) {
  String plaintext;
  for (int i=0 ; i<ciphertext.length ; i++) {
    if (ciphertext.data[i] >= 97) {
      int ind = ciphertext.data[i] - 'a';
      ind = (ind +26 - key) % 26;
      plaintext.data[i] = 'a'+ind;
    } else {
      int ind = ciphertext.data[i] - 'A';
      ind = (ind + 26 - key) % 26;
      plaintext.data[i] = 'A'+ind;
    }
  }
  plaintext.data[ciphertext.length] = '\0';
  plaintext.length = ciphertext.length;
  return plaintext;
}

int main() {
  while (true) {
    printf("1: Encrypt,   2: Decrypt,   Anything Else: Quit\n");
    int choice;
    scanf("%d", &choice);
    clear_input_buffer();
    if (choice == 1) {
      String plaintext;
      inputString(&plaintext);
      int key;
      scanf("%d", &key);
      clear_input_buffer();
      String ciphertext = encryptCeasar(plaintext, key);
      secure_claer(plaintext.data, sizeof(plaintext.data));
      plaintext.length = 0;
      key = 0;
      printf("%s\n", ciphertext.data);
    } else if (choice == 2) {
      String ciphertext;
      inputString(&ciphertext);
      int key;
      scanf("%d", &key);
      clear_input_buffer();
      String plaintext = decryptCeasar(ciphertext, key);
      printf("%s\n", plaintext.data);
      secure_claer(plaintext.data, sizeof(plaintext.data));
      plaintext.length = 0;
      key = 0;
    } else {
      break;
    }
  }
  return 0;
}
