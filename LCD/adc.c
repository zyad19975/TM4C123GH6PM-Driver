

#include "adc.h"



/*This 2-D array holds the base addresse and the pin number of the Analog Input pins of tm4 microcontroller*/
static uint32_t ADC_AnalogInputPin[12][2]=
{
   {GPIO_PORT_E_APB_BASE_ADDRESSE,0X3},
   {GPIO_PORT_E_APB_BASE_ADDRESSE,0X2},
   {GPIO_PORT_E_APB_BASE_ADDRESSE,0X1},
   {GPIO_PORT_E_APB_BASE_ADDRESSE,0X0},
   {GPIO_PORT_D_APB_BASE_ADDRESSE,0X3},
   {GPIO_PORT_D_APB_BASE_ADDRESSE,0X2},
   {GPIO_PORT_D_APB_BASE_ADDRESSE,0X1},
   {GPIO_PORT_D_APB_BASE_ADDRESSE,0X0},
	 {GPIO_PORT_E_APB_BASE_ADDRESSE,0X5},
   {GPIO_PORT_E_APB_BASE_ADDRESSE,0X4},
   {GPIO_PORT_B_APB_BASE_ADDRESSE,0X4},
   {GPIO_PORT_B_APB_BASE_ADDRESSE,0X5}
};


static uint32_t ADC_BaseAdresse[]=
{
	ADC0_R_BASE_ADDRESSE,
	ADC1_R_BASE_ADDRESSE
};

static uint32_t ADC_SequenceControlRegister_Offset[]=
{
   ADC_Sample_Sequencer0_Control_R_OFFSET,
	 ADC_Sample_Sequencer1_Control_R_OFFSET,
	 ADC_Sample_Sequencer2_Control_R_OFFSET,
	 ADC_Sample_Sequencer3_Control_R_OFFSET

};



static uint32_t ADC_SequenceMultiplexerSelect_Offset[]=
{
   ADC_Sample_Sequencer0_Mux_Select_R_OFFSET,
   ADC_Sample_Sequencer1_Mux_Select_R_OFFSET,
   ADC_Sample_Sequencer2_Mux_Select_R_OFFSET,
   ADC_Sample_Sequencer3_Mux_Select_R_OFFSET

};





/*These pointers to function will be used to call the interrupt user defined function inside the ISRs*/
/*Note: We wil only use the first sequencers of the two ADCs*/
static void(*ADC0_Seq0_CallBackPtr)(void)=NULL_PTR;
static void(*ADC1_Seq0_CallBackPtr)(void)=NULL_PTR;





/*ISRs*/



/*ADC0 Seq0 Handler*/
void ADC0SS0_Handler()
{
 
	if(ADC0_Seq0_CallBackPtr != NULL_PTR) 
				ADC0_Seq0_CallBackPtr();
	
	/*Clear The Interrupt flag of sequencer0 in ADC0 */
	/*Permission:RW1C*/
		SET_BIT(ADC0_ISC_R,BIT_0);

}


/*ADC1 Seq0 Handler*/
void ADC1SS0_Handler()
{
 
	if(ADC1_Seq0_CallBackPtr != NULL_PTR) 
				ADC1_Seq0_CallBackPtr();
	
	/*Clear The Interrupt flag of sequencer0 in ADC0 */
	/*Permission:RW1C*/
		SET_BIT(ADC1_ISC_R,BIT_0); 

}




/*********************************************************************/
/*********************************************************************/


void ADC_init(const ADC_ConfigureStruct* configStruct_ptr)
{

		uint32_t Port_baseAddresse,Adc_CurrentBaseAddresse;
	  uint8_t  Adc_SequencerControlOffset,Adc_SequencerMuxOffset,Pin_number,i;
	
	
	
	
/*Configure The corrospnding GPIO*/ 

	/*Enable The GPIO Clock*/
	SYSCTL_RCGCGPIO_R|=configStruct_ptr->PortGPIOClock_Mask;
	
	/*No need to Wait for GPIO to be ready*/
	
	/*No need to Unlock The Port*/
	
	
	
	/*Loop To Configure All The Required Analog Pins*/
	
	for( i=0; i<17;i++)
	{
	

	/*To Check if the 12 channels of the ADC are used or not*/
	/*To use only part of the inputs , set the input after the last one with a negative number*/
	 if((configStruct_ptr->Samples[i].SampleNumber)< 0) break;
		
		
	/*Port base address*/
	 Port_baseAddresse=ADC_AnalogInputPin[configStruct_ptr->Samples[i].AnalogInput][0];
	 Pin_number=ADC_AnalogInputPin[configStruct_ptr->Samples[i].AnalogInput][1];
	
	
	/*Set the pin directions to input*/
    (*((volatile uint32_t*)( Port_baseAddresse + GPIO_PORT_DIRECTION_R_OFFSET )))&=~(1<<Pin_number);
	
	
	/*Disable The Digital Functionality*/
    (*((volatile uint32_t*)( Port_baseAddresse + GPIO_PORT_DEN_R_OFFSET  )))&=~(1<<Pin_number);


	/*Enable The Analog Functionality*/
	  (*((volatile uint32_t*)( Port_baseAddresse + GPIO_PORT_AFSEL_R_OFFSET )))|=(1<<Pin_number);

	/*Enable The Altertnative Function*/
	  (*((volatile uint32_t*)( Port_baseAddresse + GPIO_PORT_AMSEL_R_OFFSET )))|=(1<<Pin_number);

	}

	
	
	

/*Configure The ADC module*/

	/*Enable The ADC Clock*/
   SYSCTL_RCGCADC_R|=(1<<configStruct_ptr->AN);
		
	/*Check if The ADC registers can be accessed or not yet*/
	 while(BIT_IS_CLEAR(SYSCTL_PRADC_R,1<<configStruct_ptr->AN));
	

	/*Get the current ADC base addresse from the configure structure into a local variable*/
   Adc_CurrentBaseAddresse=ADC_BaseAdresse[configStruct_ptr->AN];

	
  /*Disable The ADC Sequencers to set the settings*/
	(*((volatile uint32_t*)( Adc_CurrentBaseAddresse + ADC_R_Active_Sequencer_OFFSET )))&=~(configStruct_ptr->ActiveSequencer_Mask);	
	


   /*Set The trigger source for the four sequencers*/
	 /*If the sequencer is disabled (No effect)*/
	 
	if((configStruct_ptr->AN)==ADC_0)
  {
		/*ADC0*/
	(*((volatile uint32_t*)( Adc_CurrentBaseAddresse + ADC_R_Event_Multiplexer_Select_OFFSET )))=ADC0_SAMPLE_SEQUNECERS_SOURCE_SELECT;
	}
	else
	{
		/*ADC1*/
	(*((volatile uint32_t*)( Adc_CurrentBaseAddresse + ADC_R_Event_Multiplexer_Select_OFFSET )))=ADC1_SAMPLE_SEQUNECERS_SOURCE_SELECT;
	}


	
	/*Check if the samples array is not empty first*/
	if(configStruct_ptr->Samples != NULL_PTR)
	{
		
	/*For Each Sample in each Sequencer */
	/*total number of samples presented in an adc module are 17 samples in all sequencers*/
	for(i=0;i<17;i++)
	{
		
		/*Check if not all samples are needed or not*/
	  if( configStruct_ptr->Samples[i].SampleNumber <0) break;
		   
		
		 /*Get the offset of the Sequencer control register to the corrosponding sequencer number*/
		 Adc_SequencerControlOffset=ADC_SequenceControlRegister_Offset[configStruct_ptr->Samples[i].SequencerNumber];
		 Adc_SequencerMuxOffset=ADC_SequenceMultiplexerSelect_Offset[configStruct_ptr->Samples[i].SequencerNumber];
		 
		 
		/*Set the settings for each Sequence*/
		
		/*Set the number of analog input for each sample*/
		(*((volatile uint32_t*)( Adc_CurrentBaseAddresse+Adc_SequencerMuxOffset)))|=((configStruct_ptr->Samples[i].AnalogInput & 0x0F)<<configStruct_ptr->Samples[i].SampleNumber);
		
		/*Set internal temp sensor or analog input*/
		(*((volatile uint32_t*)( Adc_CurrentBaseAddresse+Adc_SequencerControlOffset)))|=(configStruct_ptr->Samples[i].T_OR_N<<SHIFT(i,BIT_3));
			
		/*Set interrupt enable or disable*/
		(*((volatile uint32_t*)( Adc_CurrentBaseAddresse+Adc_SequencerControlOffset)))|=(configStruct_ptr->Samples[i].IS<<SHIFT(i,BIT_2));
		
		/*Set the end of sequence in a sequencer*/
		(*((volatile uint32_t*)( Adc_CurrentBaseAddresse+Adc_SequencerControlOffset)))|=(configStruct_ptr->Samples[i].LastSample<<SHIFT(i,BIT_1));
		 
	
	}
	
 }
	
 

	/*INTERRUPT Enable For each Sequencer*/
	(*((volatile uint32_t*)( Adc_CurrentBaseAddresse+ADC_Interrupt_Mask_R_OFFSET)))|=configStruct_ptr->InterruptSelect_Mask;
 
  
 /*Enable The ADC Sequencers*/
 (*((volatile uint32_t*)( Adc_CurrentBaseAddresse + ADC_R_Active_Sequencer_OFFSET )))|=(configStruct_ptr->ActiveSequencer_Mask);	

 

}

	




uint16_t ADC_readChannel(ADC_Number AN){

	uint16_t result;
	
	/*First start convertion be setting PSSI bit 0 for ss0 by one*/
	(*((volatile uint32_t*)(ADC_BaseAdresse[AN] + ADC_PSSI_OFFSET )))  |= 1;

	/*check if RIS is equale 0 wait untill it be one indicate the end of conversion*/
	while (	((*((volatile uint32_t*)(ADC_BaseAdresse[AN] + ADC_RIS_OFFSET  ))) & 1) == 0);

 result = (*((volatile uint32_t*)(ADC_BaseAdresse[AN] + ADC_SSFIFO0_OFFSET)));

	/*reset the ADC for new conversion*/
	/*result is taken into variable first because setting ISC clear the data from FIFO0 reg*/

	(*((volatile uint32_t*)(ADC_BaseAdresse[AN] + ADC_ISC_OFFSET ))) |= 1;
	return result;

}





void ADC_setISRCallBack(void(*callBack_ptr)(void),ADC_Number AN)
{

 if(AN)	ADC1_Seq0_CallBackPtr=callBack_ptr;

 else 	ADC0_Seq0_CallBackPtr=callBack_ptr;
 
}

