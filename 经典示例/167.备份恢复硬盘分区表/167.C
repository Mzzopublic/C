#include <stdio.h>
#include <bios.h>
#include <fcntl.h>
#include <sys\types.h>
#include <sys\stat.h>

void helpmsg(void);          /*自定义函数原型说明，用来显示提示信息*/

int main(int argc,char * argv[])
{
  int result;
  char buffer[512];
  FILE *fp;
  if (argc == 1) helpmsg();
  if (* argv[1] == 's' || *argv[1] == 'S')		/*保存分区表功能*/
  {
    result = biosdisk(2,0x80,0,0,0,1,buffer);   /*调用biosdisk，将错误类型返回给result,内容存在buffer中*/
	if (!result){								/*如果成功读取分区表*/
		printf(" Read partition table successfully!\n");
		if (( fp == fopen("c:part.doc","wb+"))==NULL)     /*将结果写到C:\part.doc文件中*/
		{
			fprintf(stderr," Can't creat file:c:\\part.doc \n"); /*如果写文件失败*/
			exit(1);
		}
		fwrite(buffer,1,512,fp);
		fclose(fp);
		printf(" Partition table save successfully!\n");
		return 0;
	}
	else {										/*如果读取失败*/
		fprintf(stderr," Fail to read partition table!");
		exit(1);
	}
  }
  if ( * argv[1] == 'r' || * argv[1] == 'S')	/*恢复分区表功能，顺序相反，操作与上面类似*/
  {
	  if ((fp == fopen("c:part.doc","rb+"))==NULL)
	  {
		  fprintf(stderr," Can't open file!");
		  exit(1);
	  }
	  fread (buffer,1,512,fp);
	  result = biosdisk(3,0x80,0,0,1,1,buffer);
	  if (! result){
		  printf(" Partition table restore successfully!\n");
		  fclose(fp);
		  return 0;
	  }
	  else{
		  fprintf(stderr," Failt to restore partition table!");
		  fclose(fp);
		  exit(1);
	  }
  }
  printf("\n Press any key to quit...");
  getch();
  return 0;
}

void helpmsg(void)
{
	puts(" The correction method using this program is : program S or program R");
	puts("   S---save partition table to file part.doc in c disk");
	puts("   R---restore partion table from file part.doc in c disk");
	exit(0);
}