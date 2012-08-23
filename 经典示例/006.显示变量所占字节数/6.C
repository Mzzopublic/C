/* 输出不同类型所占的字节数*/
#include <stdio.h>
void main()
{
    /* sizeof()是保留字，它的作用是求某类型或某变量类型的字节数, */
    /* 括号中可以是类型保留字或变量。*/
    /*int型在不同的机器，不同的编译器中的字节数不一样,*/
    /*一般来说在TC2.0编译器中字节数为2,在VC编译器中字节数为4 */
    printf("The bytes of the variables are:\n");
    printf("int:%d bytes\n",sizeof(int));
    /* char型的字节数为1 */
    printf("char:%d byte\n",sizeof(char));
    /* short型的字节数为2 */
    printf("short:%d bytes\n",sizeof(short));
    /* long型的字节数为4 */
    printf("long:%d bytes\n",sizeof(long));
    /* float型的字节数为4 */
    printf("float:%d bytes\n",sizeof(float));
    /* double型的字节数为8 */
    printf("double:%d bytes\n",sizeof(double));
    /* long double型的字节数为8或10或12 */
    printf("long double:%d bytes\n",sizeof(long double));
    getchar();

}


