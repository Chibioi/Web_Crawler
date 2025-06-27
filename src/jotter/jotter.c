#include <stdio.h>

#define min(a, b) (((a) < (b)) ? a : b)

int main() {
    int a = 27;
    int b = 30;

    printf("Minimum: %d\n", min(a, b));
    return 0;
        
}

