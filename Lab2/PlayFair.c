#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_SIZE 100

typedef struct {
  char data[MAX_SIZE];
  size_t length;
} String;

void secure_clear(void *v, size_t n) {
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

void playFairMatrix(String key, char matrix[5][5], int location[26][2]) {
  int len = key.length;
  int col = 0, row = 0;
  for (int i=0 ; i<len ; ++i) {
    char c = key.data[i];
    if (!isalpha((unsigned char)c)) continue;
    if (c == 'j' || c == 'J') c = 'I';
    c = toupper((unsigned char)c);
    int ascii_of_c = c - 'A';
    if (location[ascii_of_c][0] == -1) {
      matrix[row][col] = c;
      location[ascii_of_c][0] = row, location[ascii_of_c][1] = col;
      if (c == 'I') location[ascii_of_c+1][0] = row, location[ascii_of_c+1][1] = col;
      col++;
      if (col == 5) col = 0, row++;
    }
  }
  for (char c='A' ; c<='Z' ; ++c) {
    int ascii_of_c = c - 'A';
    if (location[ascii_of_c][0] == -1) {
      matrix[row][col] = c;
      location[ascii_of_c][0] = row, location[ascii_of_c][1] = col; 
      if (c == 'I') location[ascii_of_c+1][0] = row, location[ascii_of_c+1][1] = col;
      col++;
      if (col == 5) col = 0, row++;
      if (row == 5) break;
    }
  }
}

String encryptPlayfair(String plaintext, String key) {
  String ciphertext = {0};
  char matrix[5][5];
  int location[26][2];
  for (size_t i=0 ; i<26 ; i++) location[i][0] = -1, location[i][1] = -1;
  playFairMatrix(key, matrix, location);
  String processed = {0};
  for (size_t i=0 ; i<plaintext.length ; ++i) {
    char c = plaintext.data[i];
    if (!isalpha((unsigned char)c)) continue;
    c = toupper((unsigned char)c);
    if (c == 'J') c = 'I';
    if (processed.length < MAX_SIZE - 2)
      processed.data[processed.length++] = c;
  }
  for (size_t i=0 ; i<processed.length ; ) {
    char a = processed.data[i];
    char b;
    if (i+1 >= processed.length || a == processed.data[i+1]) {
      b = (a == 'X') ? 'Q' : 'X';
      i++;
    } else {
      b = processed.data[i+1];
      i += 2;
    }
    int r1 = location[a-'A'][0];
    int c1 = location[a-'A'][1];
    int r2 = location[b-'A'][0];
    int c2 = location[b-'A'][1];
    if (r1 == r2) {
      ciphertext.data[ciphertext.length++] = matrix[r1][(c1+1)%5];
      ciphertext.data[ciphertext.length++] = matrix[r2][(c2+1)%5];
    } else if (c1 == c2) {
      ciphertext.data[ciphertext.length++] = matrix[(r1+1)%5][c1];
      ciphertext.data[ciphertext.length++] = matrix[(r2+1)%5][c2];
    } else {
      ciphertext.data[ciphertext.length++] = matrix[r1][c2];
      ciphertext.data[ciphertext.length++] = matrix[r2][c1];
    }
  }
  ciphertext.data[ciphertext.length] = '\0';
  secure_clear(processed.data, sizeof(processed.data));
  return ciphertext;
}

String decryptPlayfair(String ciphertext, String key) {
  String plaintext = {0};
  char matrix[5][5];
  int location[26][2];
  for (size_t i=0 ; i<26 ; i++) location[i][0] = -1, location[i][1] = -1;
  playFairMatrix(key, matrix, location);
  String processed = {0};
  for (size_t i=0 ; i<ciphertext.length ; ++i) {
    char c = ciphertext.data[i];
    if (!isalpha((unsigned char)c)) continue;
    c = toupper((unsigned char)c);
    if (c == 'J') c = 'I';
    if (processed.length < MAX_SIZE - 1)
      processed.data[processed.length++] = c;
  }
  if (processed.length % 2 != 0) {
    secure_clear(processed.data, sizeof(processed.data));
    return plaintext;
  }
  for (size_t i=0 ; i<processed.length ; i+=2) {
    char a = processed.data[i];
    char b = processed.data[i+1];
    int r1 = location[a-'A'][0];
    int c1 = location[a-'A'][1];
    int r2 = location[b-'A'][0];
    int c2 = location[b-'A'][1];
    if (r1 == r2) {
      plaintext.data[plaintext.length++] = matrix[r1][(c1+4)%5];
      plaintext.data[plaintext.length++] = matrix[r2][(c2+4)%5];
    } else if (c1 == c2) {
      plaintext.data[plaintext.length++] = matrix[(r1+4)%5][c1];
      plaintext.data[plaintext.length++] = matrix[(r2+4)%5][c2];
    } else {
      plaintext.data[plaintext.length++] = matrix[r1][c2];
      plaintext.data[plaintext.length++] = matrix[r2][c1];
    }
  }
  plaintext.data[plaintext.length] = '\0';
  secure_clear(processed.data, sizeof(processed.data));
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
      String key;
      inputString(&key);
      String ciphertext = encryptPlayfair(plaintext, key);
      secure_clear(plaintext.data, sizeof(plaintext.data));
      plaintext.length = 0;
      secure_clear(key.data, sizeof(key.data));
      key.length = 0;
      printf("%s\n", ciphertext.data);
    } else if (choice == 2) {
      String ciphertext;
      inputString(&ciphertext);
      String key;
      inputString(&key);
      String plaintext = decryptPlayfair(ciphertext, key);
      printf("%s\n", plaintext.data);
      secure_clear(plaintext.data, sizeof(plaintext.data));
      plaintext.length = 0;
      secure_clear(key.data, sizeof(key.data));
      key.length = 0;
    } else if(choice != 1 && choice != 2) {
      break;
    }
  }
  return 0;
}
