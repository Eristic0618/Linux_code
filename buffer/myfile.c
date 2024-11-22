#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main()
{
    const char *fstr = "fwrite\n";
    const char *str = "write\n";

    printf("printf\n");
    fprintf(stdout, "fprintf\n");
    fwrite(fstr, strlen(fstr), 1, stdout);
    write(1, str, strlen(str));
    close(1);

    return 0;
}