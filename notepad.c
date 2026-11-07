
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef char *PATH;

#define SIZEOFINPUTBUFFER 100

char mynotes[100][100] = {};
PATH FILEPATH = "/Users/sam/Documents/projects/notepad/cache/cache.txt";

// function to read from a file that stores my current notes on a paticular
// project

bool readFromFile(const char *filepath) {
  FILE *file = fopen(filepath, "r");
  if (!file) {
    return false;
  }

  char line[100];
  size_t index = 0;
  while (fgets(line, sizeof(line), file) &&
         index < sizeof(mynotes) / sizeof(mynotes[0])) {
    strncpy(mynotes[index], line, sizeof(mynotes[index]) - 1);
    mynotes[index][sizeof(mynotes[index]) - 1] =
        '\0'; // Ensure null-termination
    index++;
  }
  fclose(file);
  return true;
}

bool writeToFile(const char *input_buffer, const char *filepath) {
  FILE *file = fopen(filepath, "a+");
  if (!file) {
    return false;
  }

  if (fseek(file, 0, SEEK_END) != 0) {
    fclose(file);
    return false;
  }

  long file_size = ftell(file);
  if (file_size < 0) {
    fclose(file);
    return false;
  }

  if (file_size > 0) {
    if (fseek(file, -1, SEEK_END) != 0) {
      fclose(file);
      return false;
    }

    int last_char = fgetc(file);
    if (last_char == EOF) {
      fclose(file);
      return false;
    }
    if (last_char != '\n') {
      if (fseek(file, 0, SEEK_END) != 0 || fputc('\n', file) == EOF) {
        fclose(file);
        return false;
      }
    }
  }

  if (fseek(file, 0, SEEK_END) != 0) {
    fclose(file);
    return false;
  }

  size_t input_length = strlen(input_buffer);
  if (input_length > 0 &&
      fwrite(input_buffer, 1, input_length, file) != input_length) {
    fclose(file);
    return false;
  }
  if (input_length == 0 || input_buffer[input_length - 1] != '\n') {
    if (fputc('\n', file) == EOF) {
      fclose(file);
      return false;
    }
  }

  return fclose(file) == 0;
}

// main function
int main(int argc, char *argv[]) {
  printf("Welcome to notepad\n");

  bool value = readFromFile(FILEPATH);

  if (value) {

    char input_buffer[SIZEOFINPUTBUFFER];
    // prints out the notes gotten from the files with a check box next to each

    for (size_t i = 0; i < sizeof(mynotes) / sizeof(mynotes[0]); i++) {
      if (mynotes[i][0] != '\0') {
        printf("%s", mynotes[i]);
      }
    }

    fgets(input_buffer, SIZEOFINPUTBUFFER, stdin);
    writeToFile(input_buffer, FILEPATH);
  } else {
    fprintf(stderr, "Unable to read from the designated path file: %s\n",
            FILEPATH);
  }
}
