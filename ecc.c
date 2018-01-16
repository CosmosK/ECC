//代码：likuo
#include<stdio.h>		//头文件
#include<string.h>
#include<stdlib.h>
#include<math.h>
#define N 200;			//保存x的值
int Genr_x[200];				//定义全局的倍点
int Genr_y[200];
int rank;				//定义全局变量表示阶
int Ep;					//全局变量用来记录P的值
int Ea;					//全局变量用来记录A的值
int Eb;					//全局变量用来记录B的值
int G_x;				//全局变量用来记录生成元Gx的值
int G_y;				//全局变量用来记录生成元Gy的值
int Genr[2] = {0};	//记录倍点集Genr[0]=x,Ger[1]=y,输出数组
/*
判断是否是素数
更快速的判断方法；
首先看一个关于质数分布的规律：大于等于5的质数一定和6的倍数相邻。例如5和7，11和13,17和19等等；
证明：令x≥1，将大于等于5的自然数表示如下：
······ 6x-1，6x，6x+1，6x+2，6x+3，6x+4，6x+5，6(x+1），6(x+1)+1 ······
可以看到，不在6的倍数两侧，即6x两侧的数为6x+2，6x+3，6x+4，由于2(3x+1)，3(2x+1)，2(3x+2)，所以它们一定不是素数，再除去6x本身，显然，素数要出现只可能出现在6x的相邻两侧。这里有个题外话，关于孪生素数，有兴趣的道友可以再另行了解一下，由于与我们主题无关，暂且跳过。这里要注意的一点是，在6的倍数相邻两侧并不是一定就是质数。
根据以上规律，判断质数可以6个为单元快进，即将方法（2）循环中i++步长加大为6，加快判断速度
 sqrt(n)中每6个数只比较2个（i, i+2）。
 时间复杂度是O(sqrt(n)/3), 因此它是一种pruning，而没有降低阶数。
 */
int numPrime(int num)
{
	int i;
	double temp;
	int pm;
	//两个较小数另处理
	if (num == 2 || num == 3)
	{
		return -1;
	}
	//不在6的倍数两侧一定不是质数
	if (num % 6 != 1 && num % 6 != 5)
	{
		return 0;
	}
	temp = sqrt(num);
	pm = (int)temp;
	//在6的倍数两侧也可能不是质数
	for (i = 5; i <= temp; i += 6)
	{
		if (num % i == 0 || num % (i + 2) == 0)
		{
			return 0;
		}
	}
	//排除所有可能，剩下的是质数

}

/*
取模函数
传入参数R-有y^2 
输出模运算的结果
 */
int modulo(int r)
{
	int i;	//保存R/p的倍数
	int mo;	//模运算的结果
	i = r / Ep;
	mo = r - i * Ep;
	if (mo >= 0)
	{
		return mo;
	}
	else
	{
		return mo + Ep;
	}

}

/*
判断一个数是否有整数的平方根
思路：首先sqrt开平方，然后把数值转换成int类型
转换后的值的平方是否等于之前
 */
int mosqrt(int mo)
{
	double mod = sqrt(mo);
	int moi = (int)mod;
	if (moi * moi == mo)
	{
		return moi;
	}
	else
	{
		return -1;
	}
}

/*
展示点集
传入参数一维x的坐标
二维y的坐标
 */
void showpoint(int x[], int y[], int a)
{
	rank = a;
	int i;						//用来循环x的坐标
	int j;						//用来循环y对应的两个值
	printf("椭圆曲线E%d(%d,%d)上所有的点，共%d个\n",Ep,Ea,Eb,rank );
	for (i = 0; i < rank; i++)
	{
		if (i % 8 == 0)
		{
			printf("\n");
		}
		printf("(%d,%d)\t", x[i], y[i]);
	}
}
/*
计算椭圆曲线上在有限域GF(P)上的点
y^2=x^3+Ax+B(mod P)
传入ECC参数A,B和参数P
 */
void getECCSet()
{
	int i = 0;				//用来循环x
	int j = 0;				//用来保存y的值
	int m;				//用来保存y^2/P的值
	int n = 0;				//用来循环P-1
	int r;				//用来保存有y^2的值
	int q;				//用来记录模+P
	int t = 0;
	int z;
	int x = 0;
	int y = 0;
	int mo;				//取模的值
	int msqt;			//模平方根
	int point_x[100] = {0};	//记录点集
	int point_y[100] = {0};
	//要去4a^3+27b^2!=0
	if (4 * Ea * Ea * Ea + 27 * Eb * Eb != 0)
	{
		for (x = 0; x < Ep - 1; x++)
		{
			//计算y^2=x^3+Ax+B
			r = x * x * x + Ea * x + Eb;
			//如果r大于0,存在相应x的点
			if (r >= 0)
			{
				//取模运算
				mo = modulo(r);
				while (y <= Ep - 1)
				{
					//每次加一个P,进行再次判断
					q = mo + n * Ep;
					msqt = mosqrt(q);
					//如果模数正好可以开平方根，即返回msqt大于0
					if (msqt >= 0)
					{
						y = (int)sqrt(q);
						point_x[i++] = x;
						point_y[j++] = y;

					}
					n++;
					y++;
				}//while

			}//if
			n = 0;		//重置0
			y = 0;
			t = i;		//把有限域上的点数传给展示函数

		}
		//展示点集
		showpoint(point_x, point_y, t);
	}
}
/*
实现逆运算inverse operation
5
传入p和2y的值
 */
int invope(int dy)
{
	int r = 0;		//用来保存对pmod的结果
	int iv = 0;		//用来保存逆元的值
	do {
		r = iv * dy % Ep;
		iv++;
	} while (r != 1);
	iv=iv-1;
	return iv;
}
/*
实现倍点运算.即计算2P
传入参数生成元的坐标G(x,y)
返回数组指针
 */
void abel()
{
	int r = 0;			//保存没对p取余之前的值
	int l = 0;			//再进行取余
	int Sx = 0;			//生成新的点的x坐标
	int Sy = 0;			//生成新的点的y坐标
	int x = 0;			//生成元点的x坐标
	int y = 0;			//生成元点的y坐标
	//int Genr[2] = {0};	//记录倍点集Genr[0]=x,Ger[1]=y,输出数组
	int G_y1;
	//先进行取逆
	G_y1=2*G_y;
	//对2y取逆
	int g=invope(G_y1);
	r = (3 * G_x * G_x + Ea) * g;
	//模运算求斜率
	l = modulo(r);
	//x=l*l-2Gx
	Sx = l * l - 2 * G_x;
	//生成最终对P取余之后的x坐标
	x = modulo(Sx);
	Sy = l*(G_x - x) - G_y;
	//生成最终对P取余之后的y坐标
	y = modulo(Sy);
	Genr[0] = x;
	Genr[1] = y;
}
/*
点乘运算
传入参数生成元的坐标G(x,y),n
 */
/*int *poiMul(int n)
{
	int i=0;
	int Gepo[2];	//记录生成点集
	if(n==1)
	{
		Gepo[i]=G_x;
		i++;
		Gepo[i]=G_y;
	}else if(n==2)
	{
		abel()
	}
}*/
/*
allgenerater函数的作用是计算所有的生成元并输出
在计算它的阶。当满足到生成元n*G为(0,0),即原点也是无穷远点
此时n便是阶（n是正整数）
 */
int allgenerater()
{
	int n=2;			//用来表示阶(n>=2)
	//int *dou;			//定义一个指针来接受倍点运算
	int temp_x;			//倍点递归计算的x
	int temp_y;			//倍点递归计算的y
	int tempx;
	int tempy;
	//首先计算2p
	abel();
	temp_x=Genr[0];
	temp_y=Genr[1];
	Genr_x[1]=G_x;
	Genr_y[1]=G_y;
	Genr_x[2]=temp_x;
	Genr_y[2]=temp_y;
	int k=0;			//表示倍点的斜率
	//当坐标都为0时，即找到有限域中的阶
	while(temp_x!=0&&temp_y!=0){
		//如果x1>x2,则l大于0
		if((temp_x-G_x)>=0)
		{
			k=(temp_y-G_y)*invope(temp_x-G_x);
			//模运算后求斜率
			k=modulo(k);
			temp_x=k*k-temp_x-G_x;
			//生成新的x
			temp_x=modulo(temp_x);
			temp_y=k*(G_x-temp_x)-G_y;
			//生成新的y
			temp_y=modulo(temp_y);
		}
		else
		{
			k=(G_y-temp_y)*invope(G_x-temp_x);
			k=modulo(k);
			tempx=k*k-temp_x-G_x;
			tempx=modulo(tempx);
			tempy=k*(temp_x-tempx)-temp_y;
			tempy=modulo(tempy);
			temp_x=tempx;
			temp_y=tempy;
		}
		n++;
		Genr_x[n]=temp_x;
		Genr_y[n]=temp_y;
	}
	n++;				//符号∞
	Genr_x[n]=0;
	Genr_y[n]=0;
	return n;

}
void showGenerator(int steps)
{
	int i=1;
	int j=0;
	for(i=1;i<=steps;i++)
	{
		if(j%8==0)
		{
			printf("\n");
		}
		printf("(%d,%d)\t",Genr_x[i],Genr_y[i]);
		j++;
	}
}
void main()
{
	char choose;				//用来记录用户的选择
	int steps;					//用来记录生成元的阶
	printf("Option A:是否计算椭圆曲线上有限域GF(P)上所有的点？(y|n)\n");
	printf("Your choice:");
	scanf("%c", &choose);
	while (choose != 'y'&& choose != 'n')
	{
		printf("选择错误！请重新选择-_-\n");
		scanf("%c", &choose);
	}
	getchar();
	if (choose == 'y')
	{
		printf("Input P:");
		scanf("%d", &Ep);

		printf("Input A:");
		scanf("%d", &Ea);

		printf("Input B:");
		scanf("%d", &Eb);
		getECCSet();
		printf("\n");
		getchar();
		printf("\n");
		printf("Option B:是否在E%d(%d,%d)选择生成元G(x,y)并计算它的阶N？(y|n)\n",Ep,Ea,Eb);
		printf("Your choice:");
		scanf("%c", &choose);
		while (choose != 'y'&& choose != 'n')
		{
		printf("选择错误！请重新选择-_-\n");
		scanf("%c", &choose);
		}
		getchar();
		if (choose == 'y')
		{
			printf("Input x:");
			scanf("%d", &G_x);

			printf("Input Y:");
			scanf("%d", &G_y);
			//生成所有生成元G并求它们的阶
			steps=allgenerater();
			getchar();
			printf("\n");
			printf("经计算在E%d(%d,%d)上选择生成元G(%d,%d)的阶为%d\n",Ep,Ea,Eb,G_x,G_y,steps);
			printf("\n");
			printf("Option C:是否显示生成元所有点乘集？(y|n)\n");
			printf("Your choice:");
			scanf("%c", &choose);
			while (choose != 'y'&& choose != 'n')
			{
			printf("选择错误！请重新选择-_-\n");
			scanf("%c", &choose);
			}
			getchar();
			if(choose=='y')
			{
				showGenerator(steps);
				getchar();
			}else if(choose=='n')
			{
				exit(0);
			}
		}else if(choose=='n')
		{
			exit(0);
		}
	}else if(choose=='n')
	{
		exit(0);
	}

	
}
