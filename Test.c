#include <stdlib.h>

int main() {
    int a, *b;
    
    b = malloc(sizeof(int) * 4);
    
    free(b);
    
    return 0;
}
