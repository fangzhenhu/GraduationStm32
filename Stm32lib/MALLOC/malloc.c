#include "malloc.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//�ڴ���� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//�ڴ��(4�ֽڶ���)
__align(4) u8 mem1base[MEM1_MAX_SIZE];							//�ڲ�SRAM�ڴ�� ��СΪ 40*1024��
__align(4) u8 mem2base[MEM2_MAX_SIZE] __attribute__((at(0X68000000)));					//�ⲿSRAM�ڴ��  ��СΪ	 192*1024
//�ڴ�����
u16 mem1mapbase[MEM1_ALLOC_TABLE_SIZE];	//1280 ���ڴ�飬��Ӧ1280�� ������һ�����Ĵ�СΪ 2�ֽڡ�		//�ڲ�SRAM�ڴ�� ��MAP
u16 mem2mapbase[MEM2_ALLOC_TABLE_SIZE] __attribute__((at(0X68000000+MEM2_MAX_SIZE)));	//�ⲿSRAM�ڴ��  ��MAP
//�ڴ�������	    ��Щ����  ���� �ڲ�sram,���ⲿsram ��һ��ġ�
const u32 memtablesize[2]={MEM1_ALLOC_TABLE_SIZE,MEM2_ALLOC_TABLE_SIZE};		//�ڴ���С ����1280,192*1024/32= ���
const u32 memblksize[2]={MEM1_BLOCK_SIZE,MEM2_BLOCK_SIZE};					//�ڴ�ֿ��С ����32 �ֽڡ�
const u32 memsize[2]={MEM1_MAX_SIZE,MEM2_MAX_SIZE};							//�ڴ��ܴ�С		


//�ڴ���������
struct _m_mallco_dev mallco_dev=
{
	mem_init,				//�ڴ��ʼ��
	mem_perused,			//�ڴ�ʹ����
	mem1base,mem2base,		//�ڴ��  
	mem1mapbase,mem2mapbase,//�ڴ����״̬�� �������� �����ڴ��   ������;
	0,0,  					//�ڴ����δ����
};

//�����ڴ�
//*des:Ŀ�ĵ�ַ
//*src:Դ��ַ
//n:��Ҫ���Ƶ��ڴ泤��(�ֽ�Ϊ��λ)
void mymemcpy(void *des,void *src,u32 n)  
{  
    u8 *xdes=des;
	u8 *xsrc=src; 
    while(n--)*xdes++=*xsrc++;  
}  
//�����ڴ�
//*s:�ڴ��׵�ַ
//c :Ҫ���õ�ֵ
//count:��Ҫ���õ��ڴ��С(�ֽ�Ϊ��λ)
void mymemset(void *s,u8 c,u32 count)  // ��֮�����е㲻���׵ĵط��� �����ⲿ sram ΪʲôҲ���� 8λ������
{										// ���˰ɣ��һ����˽��ˣ����Ը��� UB ��LB  OE,WE �����ơ�
    u8 *xs = s;  
    while(count--)*xs++=c;  
}	   
//�ڴ�����ʼ��  
//memx:�����ڴ��
void mem_init(u8 memx)  //memmap  memx ����<2;
{  
    mymemset(mallco_dev.memmap[memx], 0,memtablesize[memx]*2);//�ڴ�״̬����������  
	mymemset(mallco_dev.membase[memx], 0,memsize[memx]);	//�ڴ��������������  
	mallco_dev.memrdy[memx]=1;								//�ڴ�����ʼ��OK  
}  
//��ȡ�ڴ�ʹ���� 
//memx:�����ڴ��    memx ����<2; Ҳ����  ���ⲿ�ڴ棬�����ڲ� �ڴ档
//����ֵ:ʹ����(0~100)
u8 mem_perused(u8 memx)  
{  
    u32 used=0;  
    u32 i;  
    for(i=0;i<memtablesize[memx];i++)  
    {  
        if(mallco_dev.memmap[memx][i])used++; // ������ֵ�����ʱ�� ��������Ӧ���ڴ���Ѿ���ռ��
    } 
    return (used*100)/(memtablesize[memx]);  
}  
//�ڴ����(�ڲ�����)
//memx:�����ڴ��
//size:Ҫ������ڴ��С(�ֽ�)
//����ֵ:0XFFFFFFFF,�������;����,�ڴ�ƫ�Ƶ�ַ 
u32 mem_malloc(u8 memx,u32 size)  //������ڴ������ �Ӹ��ڴ��ַ�������ڴ��ַ��
{  
    signed long offset=0;  
    u16 nmemb;	//��Ҫ���ڴ����  
	u16 cmemb=0;//�������ڴ����
    u32 i;  
    if(!mallco_dev.memrdy[memx])mallco_dev.init(memx);//δ��ʼ��,��ִ�г�ʼ�� 
    if(size==0)return 0XFFFFFFFF;//����Ҫ����

    nmemb=size/memblksize[memx];  	//��ȡ��Ҫ����������ڴ����
    if(size%memblksize[memx])nmemb++;  // ������� ��һ����
    for(offset=memtablesize[memx]-1;offset>=0;offset--)//���������ڴ������  
    {     
		if(!mallco_dev.memmap[memx][offset])cmemb++;//�������ڴ��������
		else cmemb=0;								//�����ڴ�����㣬 ��Ϊû���ҵ����о����ڴ���Ƭ��
		if(cmemb==nmemb)							//�ҵ�������nmemb�����ڴ��
		{
            for(i=0;i<nmemb;i++)  					//��ע�ڴ��ǿ� 
            {  
                mallco_dev.memmap[memx][offset+i]=nmemb;  //��ָ�� p ���� malloc �����ڴ��ʱ�����ж� p Ҫ������ڴ������m�� ��Ȼ��ӵ� n �ʼ�����²��ң�ֱ���ҵ� m �������Ŀ��ڴ�飨����Ӧ�ڴ�������Ϊ 0�� ��Ȼ���� m ���� ���������ֵ������Ϊ m����Ǳ�ռ�ã� ��
            }  
            return (offset*memblksize[memx]);//����ƫ�Ƶ�ַ  
		}
    }  
    return 0XFFFFFFFF;//δ�ҵ����Ϸ����������ڴ��  
}  
//�ͷ��ڴ�(�ڲ�����)  ��������ָ���ڲ����ⲿ  ���� ���еĺ��� �� �ڲ����ã��� ���ⲿ���� 
//memx:�����ڴ��
//offset:�ڴ��ַƫ��
//����ֵ:0,�ͷųɹ�;1,�ͷ�ʧ��;  
u8 mem_free(u8 memx,u32 offset)  
{  
    int i;  
    if(!mallco_dev.memrdy[memx])//δ��ʼ��,��ִ�г�ʼ��
	{
		mallco_dev.init(memx);    
        return 1;//δ��ʼ��  
    }  
    if(offset<memsize[memx])//ƫ�����ڴ����. 
    {  
        int index=offset/memblksize[memx];			//ƫ�������ڴ�����  
        int nmemb=mallco_dev.memmap[memx][index];	//�ڴ������
        for(i=0;i<nmemb;i++)  						//�ڴ������
        {  
            mallco_dev.memmap[memx][index+i]=0;  //���·��䣬��Ӧ�������ͷţ�
        }  
        return 0;  
    }else return 2;//ƫ�Ƴ�����.  
}  
//�ͷ��ڴ�(�ⲿ����) 
//memx:�����ڴ��
//ptr:�ڴ��׵�ַ 
void myfree(u8 memx,void *ptr)  
{  
	u32 offset;  
    if(ptr==NULL)return;//��ַΪ0.    
 	offset=(u32)ptr-(u32)mallco_dev.membase[memx];  
    mem_free(memx,offset);//�ͷ��ڴ�     
}  
//�����ڴ�(�ⲿ����)
//memx:�����ڴ��
//size:�ڴ��С(�ֽ�)
//����ֵ:���䵽���ڴ��׵�ַ.
void *mymalloc(u8 memx,u32 size)  
{  
    u32 offset;  									      
	offset=mem_malloc(memx,size);  	   				   
    if(offset==0XFFFFFFFF)return NULL;  
    else return (void*)((u32)mallco_dev.membase[memx]+offset);  
}  
//���·����ڴ�(�ⲿ����)
//memx:�����ڴ��
//*ptr:���ڴ��׵�ַ
//size:Ҫ������ڴ��С(�ֽ�)
//����ֵ:�·��䵽���ڴ��׵�ַ.
void *myrealloc(u8 memx,void *ptr,u32 size)  //���� �򵥵��߳��� ��Ҫ���� �ڴ渴�ơ�
{  
    u32 offset;  
    offset=mem_malloc(memx,size);  
    if(offset==0XFFFFFFFF)return NULL;     
    else  
    {  									   
	    mymemcpy((void*)((u32)mallco_dev.membase[memx]+offset),ptr,size);	//�������ڴ����ݵ����ڴ�   
        myfree(memx,ptr);  											  		//�ͷž��ڴ�
        return (void*)((u32)mallco_dev.membase[memx]+offset);  				//�������ڴ��׵�ַ
    }  
} 












