#include "HCSR04.h"
#include "../../util/DWT_Delay.h"

/*
 * No quería pero... we'll grow on sanity definiendo una estructura...
 * Estructura innecesariamente larga...
 */
typedef struct
{
	GPIO_TypeDef * GPIx_TRIGGER;
	uint16_t       GPIO_PIN_TRIGGER;
	TIM_TypeDef*   TIMER_INSTANCE;
	uint32_t       IC_TIMER_CHANNEL;
	uint32_t       TIMER_CLK_MHz;
	float distance;
	uint32_t difference;
	int EDGE_STATE;
	uint16_t OverFlow;
	uint32_t TIMER_ARR; //Auto-Reload-Register
	uint32_t TIMER_PSC; //Prescaler
	uint32_t TS1; //TimeStamp
	uint32_t TS2; //TimeStamp
}HCSR04_Data;

static HCSR04_Data My_HCSR04_Data = {0};

void HCSR04_Init(TIM_HandleTypeDef* htim, GPIO_TypeDef* GPIOx_Trigg, uint16_t GPIO_Pin_Trigg, uint32_t IC_Tim_Channel, uint32_t Clock_MHz){
	My_HCSR04_Data.TIMER_INSTANCE  = htim->Instance;
	My_HCSR04_Data.GPIx_TRIGGER = GPIOx_Trigg;
	My_HCSR04_Data.GPIO_PIN_TRIGGER = GPIO_Pin_Trigg;
	My_HCSR04_Data.IC_TIMER_CHANNEL = IC_Tim_Channel;
	My_HCSR04_Data.TIMER_CLK_MHz = Clock_MHz;
	My_HCSR04_Data.TIMER_ARR = My_HCSR04_Data.TIMER_INSTANCE->ARR;
	My_HCSR04_Data.TIMER_PSC = My_HCSR04_Data.TIMER_INSTANCE->PSC;

	DWT_Delay_Init();

	if(My_HCSR04_Data.TIMER_ARR == 0){
		My_HCSR04_Data.TIMER_ARR = 65535;
	}
}

float HCSR04_Read(void){
	return My_HCSR04_Data.distance;
}

void HCSR04_Trigger(void){
	HAL_GPIO_WritePin(My_HCSR04_Data.GPIx_TRIGGER, My_HCSR04_Data.GPIO_PIN_TRIGGER, 1);
	DWT_Delay_us(10);
	HAL_GPIO_WritePin(My_HCSR04_Data.GPIx_TRIGGER, My_HCSR04_Data.GPIO_PIN_TRIGGER, 0);
}

void HCSR04_TMR_OVF_ISR(void){
	My_HCSR04_Data.OverFlow++;
}

void HCSR04_IC_ISR(TIM_HandleTypeDef* htim){
	uint32_t PS = 0;
	if(My_HCSR04_Data.EDGE_STATE == 0){
		//Obtenemos el TS1
		My_HCSR04_Data.TS1 = HAL_TIM_ReadCapturedValue(htim, My_HCSR04_Data.IC_TIMER_CHANNEL);
		//Cambiamos la polaridad del IC
		My_HCSR04_Data.EDGE_STATE = 1;
		__HAL_TIM_SET_CAPTUREPOLARITY(htim, My_HCSR04_Data.IC_TIMER_CHANNEL, TIM_INPUTCHANNELPOLARITY_FALLING);
		//Seteamos Overflow a 0, esto nos ayuda a contar cuantos OVF tuvimos para luego calcular TS2
		My_HCSR04_Data.OverFlow = 0;
	}
	else{
		PS = My_HCSR04_Data.TIMER_INSTANCE->PSC;
		My_HCSR04_Data.TIMER_ARR = My_HCSR04_Data.TIMER_INSTANCE->ARR;
		//Obtemos el TS2
		My_HCSR04_Data.TS2 = HAL_TIM_ReadCapturedValue(htim, My_HCSR04_Data.IC_TIMER_CHANNEL);
		My_HCSR04_Data.TS2 += (My_HCSR04_Data.OverFlow * (My_HCSR04_Data.TIMER_ARR + 1));
		My_HCSR04_Data.difference = My_HCSR04_Data.TS2 - My_HCSR04_Data.TS1;
		//Calculamos la Distancia
		My_HCSR04_Data.distance = (My_HCSR04_Data.difference * 0.017)/(My_HCSR04_Data.TIMER_CLK_MHz/(PS+1));
		//Cambiamos la polaridad del IC preparándolo para la captura de una nueva onda
		My_HCSR04_Data.EDGE_STATE = 0;
		__HAL_TIM_SET_CAPTUREPOLARITY(htim, My_HCSR04_Data.IC_TIMER_CHANNEL, TIM_INPUTCHANNELPOLARITY_RISING);
	}
}
