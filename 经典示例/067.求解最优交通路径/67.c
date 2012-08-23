/*交通图最短路径程序*/

#include "string.h" 
#include "stdio.h" 

typedef struct ArcCell{
	int adj;  /*相邻接的城市序号*/
}ArcCell; /*定义边的类型*/

typedef struct VertexType{
	int number;    /*城市序号*/
	char *city;   /*城市名称*/
}VertexType; /*定义顶点的类型*/

typedef struct{
	VertexType vex[25];  /*图中的顶点，即为城市*/
	ArcCell arcs[25][25]; /*图中的边，即为城市间的距离*/
	int vexnum,arcnum; /*顶点数，边数*/
}MGraph; /*定义图的类型*/

MGraph G; /*把图定义为全局变量*/

int P[25][25]; 
long int D[25];

void CreateUDN(v,a) /*造图函数*/
int v,a;
{ int i,j;
	G.vexnum=v;
	G.arcnum=a;
	for(i=0;i<G.vexnum;++i) G.vex[i].number=i;
	/*下边是城市名*/
	G.vex[0].city="乌鲁木齐"; 
	G.vex[1].city="西宁";
	G.vex[2].city="兰州";
	G.vex[3].city="呼和浩特";
	G.vex[4].city="北京";
	G.vex[5].city="天津";
	G.vex[6].city="沈阳";
	G.vex[7].city="长春";
	G.vex[8].city="哈尔滨";
	G.vex[9].city="大连";
	G.vex[10].city="西安";
	G.vex[11].city="郑州";
	G.vex[12].city="徐州";
	G.vex[13].city="成都";
	G.vex[14].city="武汉";
	G.vex[15].city="上海";
	G.vex[16].city="昆明";
	G.vex[17].city="贵州";
	G.vex[18].city="株洲";
	G.vex[19].city="南昌";
	G.vex[20].city="福州";
	G.vex[21].city="柳州";
	G.vex[22].city="南宁";
	G.vex[23].city="广州";
	G.vex[24].city="深圳";
	/*这里把所有的边假定为20000，含义是城市间不可到达*/
	for(i=0;i<G.vexnum;++i)
		for(j=0;j<G.vexnum;++j) 
			G.arcs[i][j].adj=20000;
	/*下边是可直接到达的城市间的距离，由于两个城市间距离是互相的，
		所以要对图中对称的边同时赋值。*/
	G.arcs[0][2].adj=G.arcs[2][0].adj=1892;
	G.arcs[1][2].adj=G.arcs[2][1].adj=216;
	G.arcs[2][3].adj=G.arcs[3][2].adj=1145;
	G.arcs[2][10].adj=G.arcs[10][2].adj=676;
	G.arcs[3][4].adj=G.arcs[4][3].adj=668;
	G.arcs[4][5].adj=G.arcs[5][4].adj=137;
	G.arcs[5][6].adj=G.arcs[6][5].adj=704;
	G.arcs[6][7].adj=G.arcs[7][6].adj=305;
	G.arcs[7][8].adj=G.arcs[8][7].adj=242;
	G.arcs[6][9].adj=G.arcs[9][6].adj=397;
	G.arcs[4][11].adj=G.arcs[11][4].adj=695;
	G.arcs[5][12].adj=G.arcs[12][5].adj=674;
	G.arcs[10][13].adj=G.arcs[13][10].adj=842;
	G.arcs[11][14].adj=G.arcs[14][11].adj=534;
	G.arcs[12][15].adj=G.arcs[15][12].adj=651;
	G.arcs[13][16].adj=G.arcs[16][13].adj=1100;
	G.arcs[13][17].adj=G.arcs[17][13].adj=967;
	G.arcs[14][18].adj=G.arcs[18][14].adj=409;
	G.arcs[17][18].adj=G.arcs[18][17].adj=902;
	G.arcs[15][19].adj=G.arcs[19][15].adj=825;
	G.arcs[18][19].adj=G.arcs[19][18].adj=367;
	G.arcs[19][20].adj=G.arcs[20][19].adj=622;
	G.arcs[17][21].adj=G.arcs[21][17].adj=607;
	G.arcs[18][21].adj=G.arcs[21][18].adj=672;
	G.arcs[21][22].adj=G.arcs[22][21].adj=255;
	G.arcs[18][23].adj=G.arcs[23][18].adj=675;
	G.arcs[23][24].adj=G.arcs[24][23].adj=140;
	G.arcs[16][17].adj=G.arcs[17][16].adj=639;
	G.arcs[10][11].adj=G.arcs[11][10].adj=511;
	G.arcs[11][12].adj=G.arcs[12][11].adj=349;
}
void narrate() /*说明函数*/
{
	int i,k=0;
	printf("\n*****************欢迎使用最优交通路径程序!***************\n");
	printf("\n城市列表如下:\n\n");
	for(i=0;i<25;i++)
	{
		printf("(%2d)%-10s",i,G.vex[i].city); /*输出城市列表*/
		k=k+1;
		if(k%4==0) printf("\n");
	}
}

void ShortestPath(num) /*最短路径函数*/
int num;
{ 
	int v,w,i,t;
	int final[25];
	int min;
	for(v=0;v<25;++v)
	{
		final[v]=0;D[v]=G.arcs[num][v].adj;
		for(w=0;w<25;++w) P[v][w]=0;
		if(D[v]<20000) {P[v][num]=1;P[v][v]=1;}
	}
	D[num]=0;final[num]=1;
	for(i=0;i<25;++i)
	{
		min=20000;
		for(w=0;w<25;++w)
			if(!final[w])
				if(D[w]<min){v=w;min=D[w];}
		final[v]=1;
		for(w=0;w<25;++w)
			if(!final[w]&&((min+G.arcs[v][w].adj)<D[w]))
			{
				D[w]=min+G.arcs[v][w].adj;
				for(t=0;t<25;t++) P[w][t]=P[v][t];
				P[w][w]=1;
			}
	}
}

void output(city1,city2) /*输出函数*/
int city1;
int city2;
{
	int a,b,c,d,q=0;
	a=city2;
	if(a!=city1)
	{
		printf("\n从%s到%s的最短路径是",G.vex[city1].city,G.vex[city2].city);
		printf("(最短距离为 %dkm.)\n\t",D[a]);
		printf("%s",G.vex[city1].city);
		d=city1;
		for(c=0;c<25;++c)
		{
gate:; /*标号，可以作为goto语句跳转的位置*/
	 P[a][city1]=0;
	 for(b=0;b<25;b++)
	 {
		 if(G.arcs[d][b].adj<20000&&P[a][b])
		 {
			 printf("-->%s",G.vex[b].city);q=q+1;
			 P[a][b]=0;
			 d=b;
			 if(q%8==0) printf("\n");
			 goto gate;
		 }
	 }
		}
	}
	
}
void main() /*主函数*/
{
	int v0,v1;
	CreateUDN(25,30);
	
	narrate();
	printf("\n\n请选择起点城市（0～24）：\n");
	scanf("%d",&v0);
	printf("请选择终点城市（0～24）：\n");
	scanf("%d",&v1);
	ShortestPath(v0);  /*计算两个城市之间的最短路径*/
	output(v0,v1);     /*输出结果*/
	printf("\n");
	printf("\n 请按任意键退出...\n");
	getch();
}