/*输入一个正整数 n。
对于每个整数 i（从 1 到 n），计算从 1 到 i 的累加和，并将这个结果累加到总和 sum 中。*/
// 通过直接使用数学公式代替循环，减少了时间复杂度,降到了 O(1)

#include <stdio.h>

int main() {
    int n;
    long sum = 0;

    printf("Please input a number to n:\n");
    scanf("%d", &n);

    if (n < 1) {
        printf("The n must not be less than 1!\n");
        return 1;
    }

    // 使用公式计算总和
    sum = (n * (n + 1) * (n + 2)) / 6;

    printf("The sum of the sequence(%d) is %ld\n", n, sum);
    return 0;
}
