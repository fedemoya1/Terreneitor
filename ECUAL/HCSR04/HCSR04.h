#ifndef HCSR04_H_
#define HCSR04_H_

#include "stm32f1xx_hal.h"

//Inicializa la estructura encargada de manejar toda la info
void HCSR04_Init(TIM_HandleTypeDef* htim, GPIO_TypeDef* GPIOx_Trigg, uint16_t GPIO_Pin_Trigg, uint32_t IC_Tim_Channel, uint32_t Clock_MHz);

//Devuelve la distancia calculada
float HCSR04_Read(void);

//Envia un pulso al Trigger del sensor de ultrasonido
void HCSR04_Trigger(void);

//Aumenta el contador de Overflow que usamos para calculos el TimeStamp2 necesario para el calculo de la distancia
void HCSR04_TMR_OVF_ISR(void);

//Manejo de la lógica dentro de la función callback de IC
void HCSR04_IC_ISR(TIM_HandleTypeDef* htim);


#endif
