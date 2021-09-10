#include <stdio.h>

int main()
{
  int i, y = 0;
  for (i = 0 ; i < 256 ; ++i) {
    int y0 = (y & 1) ^ ((y >> 3) & 1) ^ ((y >> 7) & 1) ^ 1;
    y = ((y << 1) & 0xff) | y0;
    printf("%d\n", y);
  }
}
