/* 用二维数组实现矩阵的转置 */
#include <stdio.h>
#define ROW 3
#define COL 4
main()
{
	int matrixA[ROW][COL],matrixB[COL][ROW];
	int i,j;

	clrscr();
	printf("Enter elements of the matrixA,");
	printf("%d*%d:\n",ROW,COL);
	for( i=0; i<ROW; i++ )
	{
		for( j=0; j<COL; j++ )
		{
			scanf("%d",&matrixA[i][j]);
		}
	}

	for( i=0; i<ROW; i++ )
	{
		for( j=0; j<COL; j++ )
		{
			matrixB[j][i] = matrixA[i][j];
		}
	}

	printf("MatrixB,");
	printf("%d*%d:\n",COL,ROW);
	for( i=0; i<COL; i++ )
	{
		for( j=0; j<ROW; j++ )
		{
			printf("%8d",matrixB[i][j]);
		}
		printf("\n");
	}
	printf("\n  Press Any Key to Quit...  \n");
	getch();
}
