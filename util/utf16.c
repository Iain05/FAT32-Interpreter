#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uchar.h>

void print_utf16(const char16_t *buffer, size_t len) {
  mbstate_t state;
  memset(&state, 0, sizeof(state));
  char mb[MB_CUR_MAX];

  for (size_t i = 0; i < len && buffer[i] != 0; i++) {
    size_t rc = c16rtomb(mb, buffer[i], &state);
    if (rc != (size_t)-1) {
      fwrite(mb, 1, rc, stdout);
    }
  }
}
