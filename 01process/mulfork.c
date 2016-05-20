#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void TestFunc(int loopnum)
{
    printf("loopnum:%d\n", loopnum);
}

int main()
{
    int pronum = 10;
    int loopnum = 100;

    pid_t pid;
    
    int i, j;
    for(i = 0; i < pronum; i++)
    {
        pid = fork();
        if(pid == 0)
        {
            for(j = 0; j < loopnum; j++)
            {
                TestFunc(j);
            }
            exit(0);
        }
    }
    printf("hello\n");  
    return 0;
}

