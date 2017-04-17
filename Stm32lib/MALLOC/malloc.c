#include "malloc.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//内存管理 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/16
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//内存池(4字节对齐)
__align(4) u8 mem1base[MEM1_MAX_SIZE];							//内部SRAM内存池 大小为 40*1024；
__align(4) u8 mem2base[MEM2_MAX_SIZE] __attribute__((at(0X68000000)));					//外部SRAM内存池  大小为	 192*1024
//内存管理表
u16 mem1mapbase[MEM1_ALLOC_TABLE_SIZE];	//1280 个内存块，对应1280个 管理表格，一个表格的大小为 2字节。		//内部SRAM内存池 的MAP
u16 mem2mapbase[MEM2_ALLOC_TABLE_SIZE] __attribute__((at(0X68000000+MEM2_MAX_SIZE)));	//外部SRAM内存池  的MAP
//内存管理参数	    这些参数  都是 内部sram,和外部sram 在一起的。
const u32 memtablesize[2]={MEM1_ALLOC_TABLE_SIZE,MEM2_ALLOC_TABLE_SIZE};		//内存表大小 比如1280,192*1024/32= 这个
const u32 memblksize[2]={MEM1_BLOCK_SIZE,MEM2_BLOCK_SIZE};					//内存分块大小 都是32 字节。
const u32 memsize[2]={MEM1_MAX_SIZE,MEM2_MAX_SIZE};							//内存总大小		


//内存管理控制器
struct _m_mallco_dev mallco_dev=
{
	mem_init,				//内存初始化
	mem_perused,			//内存使用率
	mem1base,mem2base,		//内存池  
	mem1mapbase,mem2mapbase,//内存管理状态表 （这里是 两个内存的   管理表格）;
	0,0,  					//内存管理未就绪
};

//复制内存
//*des:目的地址
//*src:源地址
//n:需要复制的内存长度(字节为单位)
void mymemcpy(void *des,void *src,u32 n)  
{  
    u8 *xdes=des;
	u8 *xsrc=src; 
    while(n--)*xdes++=*xsrc++;  
}  
//设置内存
//*s:内存首地址
//c :要设置的值
//count:需要设置的内存大小(字节为单位)
void mymemset(void *s,u8 c,u32 count)  // 总之，我有点不明白的地方是 对于外部 sram 为什么也可以 8位操作。
{										// 算了吧，我还是了解了，可以根据 UB 和LB  OE,WE 来控制。
    u8 *xs = s;  
    while(count--)*xs++=c;  
}	   
//内存管理初始化  
//memx:所属内存块
void mem_init(u8 memx)  //memmap  memx 必须<2;
{  
    mymemset(mallco_dev.memmap[memx], 0,memtablesize[memx]*2);//内存状态表数据清零  
	mymemset(mallco_dev.membase[memx], 0,memsize[memx]);	//内存池所有数据清零  
	mallco_dev.memrdy[memx]=1;								//内存管理初始化OK  
}  
//获取内存使用率 
//memx:所属内存块    memx 必须<2; 也就是  是外部内存，还是内部 内存。
//返回值:使用率(0~100)
u8 mem_perused(u8 memx)  
{  
    u32 used=0;  
    u32 i;  
    for(i=0;i<memtablesize[memx];i++)  
    {  
        if(mallco_dev.memmap[memx][i])used++; // 当该项值非零的时候， 代表该项对应的内存块已经被占用
    } 
    return (used*100)/(memtablesize[memx]);  
}  
//内存分配(内部调用)
//memx:所属内存块
//size:要分配的内存大小(字节)
//返回值:0XFFFFFFFF,代表错误;其他,内存偏移地址 
u32 mem_malloc(u8 memx,u32 size)  //这里的内存分配是 从高内存地址，到低内存地址。
{  
    signed long offset=0;  
    u16 nmemb;	//需要的内存块数  
	u16 cmemb=0;//连续空内存块数
    u32 i;  
    if(!mallco_dev.memrdy[memx])mallco_dev.init(memx);//未初始化,先执行初始化 
    if(size==0)return 0XFFFFFFFF;//不需要分配

    nmemb=size/memblksize[memx];  	//获取需要分配的连续内存块数
    if(size%memblksize[memx])nmemb++;  // 这个属于 进一法。
    for(offset=memtablesize[memx]-1;offset>=0;offset--)//搜索整个内存控制区  
    {     
		if(!mallco_dev.memmap[memx][offset])cmemb++;//连续空内存块数增加
		else cmemb=0;								//连续内存块清零， 因为没有找到，感觉有内存碎片。
		if(cmemb==nmemb)							//找到了连续nmemb个空内存块
		{
            for(i=0;i<nmemb;i++)  					//标注内存块非空 
            {  
                mallco_dev.memmap[memx][offset+i]=nmemb;  //当指针 p 调用 malloc 申请内存的时候，先判断 p 要分配的内存块数（m） ，然后从第 n 项开始，向下查找，直到找到 m 块连续的空内存块（即对应内存管理表项为 0） ，然后将这 m 个内 存管理表项的值都设置为 m（标记被占用） ，
            }  
            return (offset*memblksize[memx]);//返回偏移地址  
		}
    }  
    return 0XFFFFFFFF;//未找到符合分配条件的内存块  
}  
//释放内存(内部调用)  这里面所指的内部和外部  就像 类中的函数 在 内部调用，和 在外部调用 
//memx:所属内存块
//offset:内存地址偏移
//返回值:0,释放成功;1,释放失败;  
u8 mem_free(u8 memx,u32 offset)  
{  
    int i;  
    if(!mallco_dev.memrdy[memx])//未初始化,先执行初始化
	{
		mallco_dev.init(memx);    
        return 1;//未初始化  
    }  
    if(offset<memsize[memx])//偏移在内存池内. 
    {  
        int index=offset/memblksize[memx];			//偏移所在内存块号码  
        int nmemb=mallco_dev.memmap[memx][index];	//内存块数量
        for(i=0;i<nmemb;i++)  						//内存块清零
        {  
            mallco_dev.memmap[memx][index+i]=0;  //向下分配，就应该向上释放；
        }  
        return 0;  
    }else return 2;//偏移超区了.  
}  
//释放内存(外部调用) 
//memx:所属内存块
//ptr:内存首地址 
void myfree(u8 memx,void *ptr)  
{  
	u32 offset;  
    if(ptr==NULL)return;//地址为0.    
 	offset=(u32)ptr-(u32)mallco_dev.membase[memx];  
    mem_free(memx,offset);//释放内存     
}  
//分配内存(外部调用)
//memx:所属内存块
//size:内存大小(字节)
//返回值:分配到的内存首地址.
void *mymalloc(u8 memx,u32 size)  
{  
    u32 offset;  									      
	offset=mem_malloc(memx,size);  	   				   
    if(offset==0XFFFFFFFF)return NULL;  
    else return (void*)((u32)mallco_dev.membase[memx]+offset);  
}  
//重新分配内存(外部调用)
//memx:所属内存块
//*ptr:旧内存首地址
//size:要分配的内存大小(字节)
//返回值:新分配到的内存首地址.
void *myrealloc(u8 memx,void *ptr,u32 size)  //不是 简单的檫除； 还要进行 内存复制。
{  
    u32 offset;  
    offset=mem_malloc(memx,size);  
    if(offset==0XFFFFFFFF)return NULL;     
    else  
    {  									   
	    mymemcpy((void*)((u32)mallco_dev.membase[memx]+offset),ptr,size);	//拷贝旧内存内容到新内存   
        myfree(memx,ptr);  											  		//释放旧内存
        return (void*)((u32)mallco_dev.membase[memx]+offset);  				//返回新内存首地址
    }  
} 












