#include <stdio.h>
#include <stdlib.h>

#define N 7
#define OVERFLOW 0
#define OK 1

typedef struct LNode {
    int password;
    int order;
    struct LNode *next;
} LNode, *LinkList;

// 创建循环链表
LinkList createCircularList(const int *passwords, int n) {
    LinkList head = (LinkList)malloc(sizeof(LNode)); // 动态分配头节点内存
    if (!head) return NULL; // 如果分配失败，返回NULL
    head->password = passwords[0];
    head->order = 1;
    head->next = NULL;
    LinkList current = head;
    
    // 改动1：使用循环创建链表节点，并检查内存分配是否成功
    for (int i = 1; i < n; ++i) {
        LinkList newNode = (LinkList)malloc(sizeof(LNode)); // 动态分配新节点内存
        if (!newNode) return NULL; // 如果分配失败，返回NULL
        newNode->password = passwords[i];
        newNode->order = i + 1;
        current->next = newNode; // 链接新节点
        current = newNode; // 更新当前节点指针
    }
    current->next = head; // 形成循环链表
    return head;
}

// 约瑟夫环问题的核心算法
void josephus(LinkList head, int step, int count) {
    if (count <= 0) return; // 如果链表为空，直接返回
    
    // 改动2：对步长进行求余处理，并处理特殊情况
    step %= count; // m对x求余
    if (step == 0) step = count; // 如果刚好整除，设置为最后一位
    
    LinkList current = head;
    for (int i = 1; i < step; ++i) {
        current = current->next; // 寻找要删除节点的前一个节点
    }
    
    // 改动3：删除节点并释放内存
    LinkList toRemove = current->next;
    printf("%d ", toRemove->order); // 输出被删除节点的order值
    current->next = toRemove->next; // 从循环链表中删除节点
    free(toRemove); // 释放被删除节点的内存
    
    // 改动4：递归调用自身
    josephus(current->next, toRemove->password, --count); // 递归调用自身
}

int main() {
    int PassW[N] = {3, 1, 7, 2, 4, 8, 4};
    int m;

    // 改动5：增加输入验证
    printf("请输入第一次计数值m: \n");
    if (scanf("%d", &m) != 1 || m < 1) { // 输入验证
        printf("无效的输入。\n");
        return OVERFLOW;
    }
    printf("第一次计数值m= %d \n", m);

    // 改动6：检查头节点是否创建成功
    LinkList head = createCircularList(PassW, N);
    if (!head) { // 检查头节点是否创建成功
        printf("内存分配失败。\n");
        return OVERFLOW;
    }

    printf("约瑟夫环问题的结果顺序是：\n");
    josephus(head, m, N);
    printf("\n");

    // 改动7：释放剩余的内存
    LinkList temp = head;
    do {
        LinkList next = temp->next;
        free(temp); // 释放当前节点内存
        temp = next; // 移动到下一个节点
    } while (temp != head);

    return OK;
}