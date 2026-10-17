/* 输入两个浮点数，输出它们中的大数 */
#include <stdio.h>
int main() //函数定义要有类型
{
	double x,y,c;				/* 变量定义 *///我喜欢双精度浮点数
	printf("Please input two real numbers:\n");	/* 提示用户输入数据 */
	scanf("%f%f",&x,&y);
	if(x>y){
		c=x;
	}
	else
		c=y;
	
	/* 计算c=max(x,y) */
	printf("MAX of (%f,%f) is %f",x,y,c);	/* 输出c */
	return 0;//返回！！！
}
