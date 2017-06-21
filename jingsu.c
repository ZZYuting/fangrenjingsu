
void sort(int * arr,int length)
{
	char i,j;
	char tempIndex;
	int temp;
	
	for(i=0;i<length;i++)
	{
		tempIndex=i;
		
		for(j=i+1;j<length;j++)
		{
			if(arr[j]<arr[tempIndex])
			{
				tempIndex=j;
			}
		}
		
		temp = arr[i];
		arr[i]=arr[tempIndex];
		arr[tempIndex]=temp;
	}

}
/**
*处理程序
*对数据进行处理
*/

int getAverage(int a ,int b, int c)//对数据处理 
{
	int max,min,medum;
	
	int arr[3];
	
	arr[0]=a;
	arr[1]=b;
	arr[2]=c;

	sort(arr,3);
	
	max		=arr[2];
	min		=arr[0];
	medum	=arr[1];
		
	printf("min=%d  medum=%d  max=%d\r\n",min,medum,max);	
	
	printf("max-medum=%d  medum-min=%d   (max+medum)/2=%d \r\n",max-medum,medum-min,(max+medum)/2);

	if(max-medum<20&& medum-min<20)
	{
		return (max+medum+min)/3;
	}
	
	if(max-medum<20)
	{
		return (max+medum)/2; 
	}
	
	
	if(medum-min<20)
	{
		return (medum+min)/2;
	}
	
	return 0;	

}

 /**函数名：char biaozhun(void)
 *	1 大左拐
 *	2 小左拐
 *	3 直行
 *  4 小右拐
 *  5 大右拐
 */
char biaozhun(void)
{
	int slope =0;//斜率	    
	int i=0,j=0;
	int a=0,b=0,c=0;
	unsigned short int Camera_Data,B,G,R,flageColor;
	char blackPeixs=0;
	char scanfControler= 0;//扫描控制   
	char lineIndex=1;//控制取样的线序列 
	char blackLinesEachLine = 0;//每一行扫描到的黑线数 
	char jIndex =0; 
	
	int mapGetCenterJ[4][3]={{0,0,0},{0,0,0},{0,0,0},{0,0,0}};//每一条黑线的中点 
	int jtemp1=0;
	int jtemp2=0;
	 
	printf("\r\n");

	for(i=0;i<240;i++)
	{
	
		if(i==48*lineIndex)//取样 取四条线 
		{
			scanfControler=1;
		} 
		
		if(i==48*lineIndex+1)//如果是要取样的线的下一条线 即：需要扫描的线已经完成 
		{
			printf("第%d条扫描线扫描到%d条黑线\r\n",lineIndex,blackLinesEachLine);
			lineIndex++;
			scanfControler =0;
			blackLinesEachLine = 0;
			blackPeixs=0;
		} 
		
			for(j=0;j<320;j++)
			{
				
				READ_FIFO_PIXEL(Camera_Data);
				
				B=Camera_Data & 0x001f;
				G=(Camera_Data>>5)&0x003f;
				R=(Camera_Data>>11)&0x001f;
			
				if(B >= 12 || G >= 25 || R >= 12)//如果是白色 
				{
					flageColor=0xffff;			
						if(scanfControler)
						{					
							if(blackPeixs>8&&blackPeixs<30)
							{
							 	jtemp2=j;
								mapGetCenterJ[lineIndex-1][blackLinesEachLine]=(jtemp1+jtemp2)/2;
								blackLinesEachLine++;
							}
								
							blackPeixs=0;	
						}
							
				}
				else//如果是黑色 
				{
					flageColor =0x0000;
					
					if(scanfControler)
					{
						if(blackPeixs==0)//第一次遇到黑线 
						{
							jtemp1 = j; 
						}
						
						blackPeixs++;//黑点的数量增加
						flageColor =0xff00;	
						if(blackPeixs>6&&blackPeixs<30)
						{
						 	flageColor =0x00ff;
						}
					}
					 
				}
					//Camera_Data
					//flageColor
					LCD_WR_Data(flageColor);
			}
	}
	for(i=0;i<4;i++)
	{	
		printf("%d\t%d\t%d\r\n",mapGetCenterJ[i][0],mapGetCenterJ[i][1],mapGetCenterJ[i][2]);
	}
	
	a = mapGetCenterJ[1][0]-mapGetCenterJ[0][0];
	b = mapGetCenterJ[2][0]-mapGetCenterJ[1][0];
	c = mapGetCenterJ[3][0]-mapGetCenterJ[2][0];
	
	slope=getAverage(a,b,c);
	
	printf("a=%d  b=%d  c=%d   slope=%d \r\n",a,b,c,slope);
	
	if(slope>15&&slope<=80)//大左拐 
	{
		return 1;
	}
	
	if(slope>6&&slope<=15)//小左拐 
	{
		return 2;
	}
	
	if(-6<slope&&slope<=6)//直行 
	{
		return 3;
	}
	
	if(slope>-15&&slope<=-6)//小右拐 
	{
		return 4;
	}
	
	if(slope>-80&&slope<=-15)//大右拐 
	{
		return 5;
	}
	
	return 3;
}


int dealArr1(int * arr)
{
	
	int part1[6]={0,0,0,0,0,0};
	char part1Length =0;
	int part2[6]={0,0,0,0,0,0} ;
	char part2Length =0;
	char i=1;
	int result =0;
	
	for(i=0;i<5;i++)
	{	 
		part1Length++;
		part1[i]=arr[i];

		
		if(arr[i+1]-arr[i]>10)//如果数据差太大	<-此处需要调整-> 
		{
			i++;
			break;
		}
		
	}
	
	for(i;i<5;i++)
	
	{
		if(arr[i+1]-arr[i]>10)//如果数据差太大	<-此处需要调整-> 
		{
			continue;	
		}		
		part2[part2Length]=arr[i];
		part2Length++;
	}

	printf("\r\n");

   	for(i=0;i<part1Length;i++)
	{
		printf("part1[%d]=%d\t",i,part1[i]);
	}
	
	
	printf("\r\n");


	for(i=0;i<part2Length;i++)
	{
		printf("part2[%d]=%d\t",i,part2[i]);
	}

	printf("\r\n");

	if(part1Length>part2Length)//哪一条线归纳的数据多 
	{
		
		for(i=0;i<part1Length;i++)
		{
			result+=part1[i];
		}

		result/=part1Length;
	
	}
	else
	{
			for(i=0;i<part2Length;i++)
			{
				result+=part2[i];
			}

			result/=part2Length;
	}
	
	printf("resultDeal=%d",result);
	printf("\r\n");

	return result;
}
/**
*char moveByBarrier(void)
*根据障碍调整自己的位置
*返回值：	0直行
*			1向右微调
*			2向左微调 
*/
 
char moveByBarrier(void)
{
	int i,j;
	unsigned short int Camera_Data,B,G,R,flageColor;	
	int j1temps[7]={0,0,0,0,0,0,0};//存放第一次遇到黑线时的j的坐标 
	int j2temps[7]={0,0,0,0,0,0,0};//存放离开黑线时j的坐标
	int slopes[6]={0,0,0,0,0,0};//存放斜率,即:blackCenter[i+1]- blackCenter[i]
	char blackPeixs[7]={0,0,0,0,0,0,0};//存放黑线的宽度
	int blackCenter[7]={0,0,0,0,0,0,0};//存放检测到的每一条黑线的中点 
	char flageIndex[7]={1,1,1,1,1,1,1};//每一条的扫描线都只要一条数据，

	char scanfControl =0; //处理控制 
	char lineIndex =0;//扫描的第几条线 
	int result = 0;

	for(i=0;i<240;i++)
	{
			for(j=0;j<320;j++)
			{
				
				if( j%40==0 && j!=0 && j!=320)//当j为 40 80 120 160 200 240 280  时有效 
				
				{
					scanfControl=1; 
					lineIndex =j/40-1;
				}
				else
				{
					scanfControl=0;
				} 
				
				READ_FIFO_PIXEL(Camera_Data);
				
				B=Camera_Data & 0x001f;
				G=(Camera_Data>>5)&0x003f;
				R=(Camera_Data>>11)&0x001f;

				if(B >= 12|| G >= 25|| R >= 12)//如果是白色 
				{
										
					flageColor =0xffff;
					
					if(scanfControl)
					{
						if( flageIndex[lineIndex] && blackPeixs[lineIndex]>10 && blackPeixs[lineIndex]<30)//如果宽度恰好  
						{
							j2temps[lineIndex] =i;
							flageIndex[lineIndex]=0;
						}
						
					}
				
				}
				else//如果是黑色 
				{					
					flageColor =0x0000;//黑色	 
					
					if(scanfControl)
					{
						
						flageColor = 0xff00;//标记色 
						
						if(blackPeixs[lineIndex]==0)//如果是第一次遇到黑点 
						{
							 j1temps[lineIndex]=i;
						} 
						
						blackPeixs[lineIndex]++;//积累黑点 
						
						if(i==239 && flageIndex[lineIndex] && blackPeixs[lineIndex]>10 && blackPeixs[lineIndex]<30)//黑点在最底部时不再可能遇到白点 
						{
							j2temps[lineIndex]=239;
							flageIndex[lineIndex]=0;
						}
					}
				
				}
					LCD_WR_Data(flageColor);
			}
	}
	for(i=0;i<7;i++)
	{
		if(flageIndex[i] && j1temps[lineIndex]>210)
		{
			j2temps[i]=240;
		} 
		
	}

	printf("%d\t %d\t %d\t %d\t %d\t %d\t %d\t \r\n",j1temps[0],j1temps[1],j1temps[2],j1temps[3],j1temps[4],j1temps[5],j1temps[6]);
	printf("%d\t %d\t %d\t %d\t %d\t %d\t %d\t \r\n",j2temps[0],j2temps[1],j2temps[2],j2temps[3],j2temps[4],j2temps[5],j2temps[6]);
	
	for(i=0;i<7;i++)
	{
	
		blackCenter[i] =(j1temps[i]+j2temps[i])/2;
		printf("%d\t",blackCenter[i]); 
	}
	
	printf("\r\n");

	printf("排序前\t");

	for(i=0;i<6;i++)
	{
		slopes[i]=blackCenter[i+1]-blackCenter[i];
		printf("%d\t",slopes[i]);
	}
	
	printf("\r\n");
	
	sort(slopes,6);
	
	printf("排序后\t");

	for(i=0;i<6;i++)
	{
		printf("%d\t",slopes[i]);
	}
	
	printf("\r\n");
	result = dealArr1(slopes);
	printf("result=%d\r\n",result);
	
	if(result>=8) //向右微调
	{
		return 1;
	}
	if(result<-8) //向左微调
	{
		return 2;
	}
	return 0;	  //直行
	
}

/**
*函数名：char getDistance(void)
*返回值 240 说明没有检测到障碍 
*
*/ 
int dealArr2(int * arr)//对数组进行处理为了去除噪点 
{
	
	int part1[7]={0,0,0,0,0,0,0};
	char part1Length =0;
	int part2[7]={0,0,0,0,0,0,0} ;
	char part2Length =0;
	char i=1;
	int result =0;
	
	for(i=0;i<6;i++)
	{
		 if(arr[i]==0)
		 {
		 	continue;
		 }
		 
		part1Length++;
		part1[i]=arr[i];

		
		if(arr[i+1]-arr[i]>10)//如果数据差太大 
		{
			i++;
			break;
		}
		
		
	}
	
	for(i;i<6;i++)
	{

		if(arr[i]==0)
		{
		 	continue;
		}

		
		if(arr[i+1]-arr[i]>10)//如果数据差太大 
		{
			continue;	
		}		


		part2[part2Length]=arr[i];
		part2Length++;
	}
	
	
	printf("\r\n");


   	for(i=0;i<part1Length;i++)
	{
		printf("part1[%d]=%d\t",i,part1[i]);
	}
	
	
	printf("\r\n");


	for(i=0;i<part2Length;i++)
	{
		printf("part2[%d]=%d\t",i,part2[i]);
	}

	printf("\r\n");


	if(part1Length>part2Length)//哪一条线归纳的数据多 
	{
		
		for(i=0;i<part1Length;i++)
		{
			result+=part1[i];
		}

		result/=part1Length;
	
	}
	else
	{
			for(i=0;i<part2Length;i++)
			{
				result+=part2[i];
			}

			result/=part2Length;
	}
	
	
	printf("resultDeal=%d",result);
	printf("\r\n");

	return result;
	
	
}
	/**
	*函数名：char getDistance(void)
	*返回值 240 说明没有检测到障碍 
	*/
	 
char getDistance(void)
{
	int i,j;
	
	unsigned short int Camera_Data,B,G,R,flageColor;	
	
	int j1temps[7]={0,0,0,0,0,0,0};//存放第一次遇到黑线时的j的坐标 
	int j2temps[7]={0,0,0,0,0,0,0};//存放离开黑线时j的坐标
	
	char blackPeixs[7]={0,0,0,0,0,0,0};//存放黑线的宽度
	int blackCenter[7]={0,0,0,0,0,0,0};//存放检测到的每一条黑线的中点 
	
	char flageIndex[7]={1,1,1,1,1,1,1};//每一条的扫描线都只要一条数据，
	
	char scanfControl =0; //处理控制 
	char lineIndex =0;//扫描的第几条线 
	char usefulLineCount=0; 
	int result = 0;
	 
	for(i=0;i<240;i++)
	{
			
			
			for(j=0;j<320;j++)
			{
				
				
				if( j%40==0 && j!=0 && j!=320)//当j为 40 80 120 160 200 240 280  时有效 
				
				{
					scanfControl=1; 
					lineIndex =j/40-1;
				}
				
				else
				
				{
					scanfControl=0;
				} 
				
				READ_FIFO_PIXEL(Camera_Data);
				
				B=Camera_Data & 0x001f;
				G=(Camera_Data>>5)&0x003f;
				R=(Camera_Data>>11)&0x001f;
				
				
				
				
				if(B >= 12|| G >= 25|| R >= 12)//如果是白色 
				{
											
					flageColor =0xffff;
					
					if(scanfControl)
					{
						if( flageIndex[lineIndex] && blackPeixs[lineIndex]>10 && blackPeixs[lineIndex]<30)//如果宽度恰好  
						{
							j2temps[lineIndex] =i;
							flageIndex[lineIndex]=0;
						}
					
					}
					
				}
				else//如果是黑色 
				{					
					flageColor =0x0000;//黑色	 
					
					if(scanfControl)
					{
						
						flageColor = 0xff00;//标记色 
						
						if(blackPeixs[lineIndex]==0)//如果是第一次遇到黑点 
						{
							 j1temps[lineIndex]=i;
						} 
						
						blackPeixs[lineIndex]++;//积累黑点 
						
					}

					if(i==239 && flageIndex[lineIndex] && blackPeixs[lineIndex]>10 && blackPeixs[lineIndex]<30 && j1temps[lineIndex]>210)
					{
					
						j2temps[lineIndex]=239;
						flageIndex[lineIndex]=0;
					}
				
				}
					 //Camera_Data
					 //flageColor
					LCD_WR_Data(flageColor);
			}
	}
	
	for(i=0;i<7;i++)
	{
		if(flageIndex[i] && j1temps[i]>210)
		{
			j2temps[i]=240;
			flageIndex[i]=0;
		} 
		
	}

	printf("%d\t %d\t %d\t %d\t %d\t %d\t %d\t \r\n",j1temps[0],j1temps[1],j1temps[2],j1temps[3],j1temps[4],j1temps[5],j1temps[6]);

	printf("%d\t %d\t %d\t %d\t %d\t %d\t %d\t \r\n",j2temps[0],j2temps[1],j2temps[2],j2temps[3],j2temps[4],j2temps[5],j2temps[6]);




	printf("排序前\t");	
	
	for(i=0;i<7;i++)
	{
	
		blackCenter[i] =(j1temps[i]+j2temps[i])/2;
		
		printf("%d\t",blackCenter[i]);
		
		if(blackCenter[i])
		{
			usefulLineCount ++;
		}
		
	}
	
	
	printf("\r\n");
	
	if(usefulLineCount<=5)//如果拍到的线少于5条，就认定为没有拍到黑线 
	{
		return 240;
	}
	sort(blackCenter,7);
	
	printf("排序后\t"); 
	
	for(i=0;i<7;i++)
	{
		printf("%d\t",blackCenter[i]);
	}


	result =240-dealArr(blackCenter);
	
	
	printf("result=%d\r\n",result);
	
	
	return result;
	
}

