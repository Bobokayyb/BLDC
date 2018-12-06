#ifndef USER_NVIC_C
#define USER_NVIC_C

#include"user_nvic.h"

void NVIC_Configuration(void);

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  
	#ifdef  VECT_TAB_RAM  
		/* Set the Vector Table base location at 0x20000000 */ 
		NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
	#else  /* VECT_TAB_FLASH  */
		/* Set the Vector Table base location at 0x08000000 */ 
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
	#endif

	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

}


#endif

