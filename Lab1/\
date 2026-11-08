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

int extended_gcd(int a, int b, int* x, int* y) {
  if (a == 0) {
    *x = 0;
    *y = 1;
    return b;
  }
  int x1, y1;
  int gcd = extended_gcd(b%a, a, &x1, &y1);
  *x = y1 - (b/a) * x1;
  *y = x1;
  return gcd;
}

int inverse(int a, int m) {
  int x, y;
  int gcd = extended_gcd(a, m, &x, &y);
  if (gcd != 1) return -1;
  return (x%m + m) % m;
}

String encryptAffine(String plaintext, int a, int b) {
  String ciphertext;
  for (int i=0 ; i<plaintext.length ; i++) {
    if (plaintext.data[i] >= 97) {
      int ind = plaintext.data[i] - 'a';
      ind = (a*ind + b) % 26;
      ciphertext.data[i] = 'a'+ind;
    } else {
      int ind = plaintext.data[i] - 'A';
      ind = (a*ind + b) % 26;
      ciphertext.data[i] = 'A'+ind;
    }
  }
  ciphertext.data[plaintext.length] = '\0';
  ciphertext.length = plaintext.length;
  return ciphertext;
}

String decryptAffine(String ciphertext, int a, int b) {
  String plaintext;
  int a_inverse = inverse(a, 26);
  for (int i=0 ; i<ciphertext.length ; i++) {
    if (ciphertext.data[i] >= 97) {
      int ind = ciphertext.data[i] - 'a';
      ind = (((ind - b + 26)%26)*a_inverse) % 26;
      plaintext.data[i] = 'a'+ind;
    } else {
      int ind = ciphertext.data[i] - 'A';
      ind = (((ind - b + 26)%26)*a_inverse) % 26;
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
      int a, b;
      scanf("%d %d", &a, &b);
      clear_input_buffer();
      String ciphertext = encryptAffine(plaintext, a, b);
      secure_claer(plaintext.data, sizeof(plaintext.data));
      plaintext.length = 0;
      a = 0, b = 0;
      printf("%s\n", ciphertext.data);
    } else if (choice == 2) {
      String ciphertext;
      inputString(&ciphertext);
      int a, b;
      scanf("%d %d", &a, &b);
      clear_input_buffer();
      String plaintext = decryptAffine(ciphertext, a, b);
      printf("%s\n", plaintext.data);
      secure_claer(plaintext.data, sizeof(plaintext.data));
      plaintext.length = 0;
      a = 0, b = 0;
    } else if(choice != 1 || choice != 2) {
      break;
    }
  }
  return 0;
}
