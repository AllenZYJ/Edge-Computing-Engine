#include"includes.h"
#define TASK_STK_SIZE  512

OS_STK StartTaskStk[TASK_STK_SIZE];
OS_STK MyTaskStk[TASK_STK_SIZE];//定义任务栈
OS_MEM *IntBuffer;

INT16S key;//退出系统的作用
INT8U IntPart[50][64];
INT8U  *IntBlkPtr;
char * s1="MyTask is running now" ;
INT8U err;
INT8U y=0;
void StartTask(void *pdata);
void MyTask(void *pdata);

void main(void)
{
	OSInit();
	PC_DOSSaveReturn();//退出pc
	PC_VectSet(uCOS,OSCtxSw);//设置中断向量
	IntBuffer = OSMemCreate(IntPart,50,64,&err);//创建内存块，输入为二维数组起始地址。
	OSTaskCreate(StartTask,
					(void*)0,//（(void *) 0) 这个表达式。表示把0强制转换为空字符
					&StartTaskStk[TASK_STK_SIZE-1],//栈顶指针
                    //StartTaskStack数组最后一个元素的地址
					0);//创建任务
	//启动任务
	OSStart();
}
//任务自身代码
void StartTask(void* pdata) //传递参数
{
	#if OS_CRITICAL_METHOD==3
	 OS_CPU_SR cpu_sr;//处理器状态字保存在局部变量
	 #endif//预编译指令
	 INT8U err;//定义错误信息
	 OS_ENTER_CRITICAL();//关闭被打断功能，在需要连续性工作的时候
	 PC_VectSet(0x08,OSTickISR);//设置中断向量，ostickisr：中断服务程序
	 PC_SetTickRate(OS_TICKS_PER_SEC);//设置每秒节拍
	 OS_EXIT_CRITICAL();//退出中断
	 OSStatInit();//初始化
	 OSTaskCreate(
					  MyTask,
					  (void*)0,
					  &MyTaskStk[TASK_STK_SIZE-1],
					  3);//创建任务
	 for(;;)
	 {
	  if(PC_GetKey(&key)==TRUE)
	  {
		 if(key==0x1B)//退出
			{
				PC_DOSReturn();//esc退出
			}
	  }
	  OSTimeDlyHMSM(0,0,3,0);//3秒延迟
	  }
}
void MyTask(void* pdata) //传递参数
{
#if OS_CRITICAL_METHOD==3
	 OS_CPU_SR cpu_sr;//预编译指令
#endif
	pdata=pdata;
	 for(;;)
	 {
	 IntBlkPtr=OSMemGet(
								IntBuffer,
								&err);//获取内存
/*请求内存块从已经建立的内存分区中申请一个内存块
//函数的参数为指向内存分区的指针，上面已经创建了内存分区IntBuffer
//函数的返回值为内存块指针，上面定义了INT8U*IntBlkPtr;//*/
	 *IntBlkPtr=1;//在申请到的内存块中存入1，这个是一个标志，当前内存使用量
     //应用程序在使用内存块时，必须知道内存块的大小，在使用时不能超过该容量
	 PC_DispStr(*IntBlkPtr*10,++y,
						s1,
						DISP_FGND_YELLOW+DISP_BGND_BLUE);
	 *++IntBlkPtr=2;//先+1，再赋值2
	 PC_DispStr(*IntBlkPtr*10,++y,
						s1,
						DISP_FGND_YELLOW+DISP_BGND_BLUE);
	 IntBlkPtr--;//这里应该是它带释放内存-1
	 OSMemPut(IntBuffer,
					IntBlkPtr);
		OSTimeDlyHMSM(0,0,1,0);//延时
    //函数中的第一个参数IntBuffer为内存块所属的内存分区的指针，IntBlkPtr为待释放内存块指针
	  }
}