#include <stdio.h>

struct A {
    int x;
};

void increment(struct A a, struct A *b) {
    a.x++;
    b->x++;
}

int main() {

    struct A a = {10};
    struct A b = {10};

    increment(a, &b);

    printf("a.x: %d \tb.x: %d\n", a.x, b.x);

    return 0;
}
