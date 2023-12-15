/*
 * system.c
 *
 *  Created on: Nov 25, 2023
 *      Author: engmo
 */

#include <stdint.h>
#include <system.h>

#define SYSTEM_HSI_FREQ     8000000UL //8MHz Internal clock oscillator
#define SYSTEM_HSE_FREQ     8000000UL //8MHz External Crystal oscillator


typedef struct
{
	uint8_t bus_num;
	uint32_t peripherial_msk;
} system_PeripheralBit_t;

static uint32_t system_SYSCLK   =   0;
static uint32_t system_HCLK     =   0;
static uint32_t system_PLLCLK   =   0;
static uint32_t system_PLLXTRE  =   0;
static uint32_t system_PCLK1    =   0;
static uint32_t system_PCLK2    =   0;


static system_PeripheralBit_t system_PeripheralGet (system_Peripherals_e peripheral )
{
	system_PeripheralBit_t per =
		{ 0, 0 };
	switch (peripheral)
	{
		// APB1 bus Peripherals
		case Peripherals_Timer2:
			per.peripherial_msk = RCC_APB1ENR_TIM2EN;
			per.bus_num = 2;
			break;
		case Peripherals_Timer3:
			per.peripherial_msk = RCC_APB1ENR_TIM3EN;
			per.bus_num = 2;
			break;
		case Peripherals_Timer4:
			per.peripherial_msk = RCC_APB1ENR_TIM4EN;
			per.bus_num = 2;
			break;
		case Peripherals_Spi2:
			per.peripherial_msk = RCC_APB1ENR_SPI2EN;
			per.bus_num = 2;
			break;
		case Peripherals_Usart2:
			per.peripherial_msk = RCC_APB1ENR_USART2EN;
			per.bus_num = 2;
			break;
		case Peripherals_Usart3:
			per.peripherial_msk = RCC_APB1ENR_USART3EN;
			per.bus_num = 2;
			break;
		case Peripherals_I2c1:
			per.peripherial_msk = RCC_APB1ENR_I2C1EN;
			per.bus_num = 2;
			break;
		case Peripherals_I2c2:
			per.peripherial_msk = RCC_APB1ENR_I2C2EN;
			per.bus_num = 2;
			break;
		case Peripherals_Can1:
			per.peripherial_msk = RCC_APB1ENR_CAN1EN;
			per.bus_num = 2;
			break;

			// APB2 bus Peripherals
		case Peripherals_Afio:
			per.peripherial_msk = RCC_APB2ENR_AFIOEN;
			per.bus_num = 3;
			break;
		case Peripherals_GpioA:
			per.peripherial_msk = RCC_APB2ENR_IOPAEN;
			per.bus_num = 3;
			break;
		case Peripherals_GpioB:
			per.peripherial_msk = RCC_APB2ENR_IOPBEN;
			per.bus_num = 3;
			break;
		case Peripherals_GpioC:
			per.peripherial_msk = RCC_APB2ENR_IOPCEN;
			per.bus_num = 3;
			break;
		case Peripherals_GpioD:
			per.peripherial_msk = RCC_APB2ENR_IOPDEN;
			per.bus_num = 3;
			break;
		case Peripherals_GpioE:
			per.peripherial_msk = RCC_APB2ENR_IOPEEN;
			per.bus_num = 3;
			break;
		case Peripherals_Adc1:
			per.peripherial_msk = RCC_APB2ENR_ADC1EN;
			per.bus_num = 3;
			break;
		case Peripherals_Adc2:
			per.peripherial_msk = RCC_APB2ENR_ADC2EN;
			per.bus_num = 3;
			break;
		case Peripherals_Timer1:
			per.peripherial_msk = RCC_APB2ENR_TIM1EN;
			per.bus_num = 3;
			break;
		case Peripherals_Spi1:
			per.peripherial_msk = RCC_APB2ENR_SPI1EN;
			per.bus_num = 3;
			break;
		case Peripherals_Usart1:
			per.peripherial_msk = RCC_APB2ENR_USART1EN;
			per.bus_num = 3;
			break;

		default:
			per.peripherial_msk = 0;
			per.bus_num = 9;
			break;
	}

	return per;
}

system_Response_e system_PeripheralEnable(system_Peripherals_e peripheral )
{
	system_Response_e ret = Response_Error;
	// Check if peripheral exist
	system_PeripheralBit_t peri = system_PeripheralGet(peripheral);
	if ((peri.bus_num > 0) & (peri.peripherial_msk > 0))
	{
		if (peri.bus_num == 2)     // APB1 bus
		{
			RCC->APB1ENR |= peri.peripherial_msk;
			ret = Response_OK;
		}
		else if (peri.bus_num == 3)     // APB2 bus
		{
			RCC->APB2ENR |= peri.peripherial_msk;
			ret = Response_OK;
		}
		else if (peri.bus_num == 1)
		{
			ret = Response_NotImplemented;
		}
		else
		{
			ret = Response_NotImplemented;
		}
	}
	else
	{
		ret = Response_Error;
	}
	return ret;
}

system_Response_e system_PeripheralDisable(system_Peripherals_e peripheral)
{
	system_Response_e ret = Response_Error;
	// Check if peripheral exist
	system_PeripheralBit_t peri = system_PeripheralGet(peripheral);
	if ((peri.bus_num > 0) & (peri.peripherial_msk > 0))
	{
		peri.peripherial_msk = ~(peri.peripherial_msk);

		if (peri.bus_num == 2)     // APB1 bus
		{
			RCC->APB1ENR &= peri.peripherial_msk;
			ret = Response_OK;
		}
		else if (peri.peripherial_msk == 3)     // APB2 bus
		{
			RCC->APB2ENR &= peri.peripherial_msk;
			ret = Response_OK;
		}
		else
		{
			ret = Response_NotImplemented;
		}
	}
	else
	{
		ret = Response_Error;
	}
	return ret;
}

void system_Calculate_Sysclk(void)
{
    uint32_t temp = 0; //for holding temporary data
    uint32_t pllclk = 0;

    //PLLXTPRE: "HSE Divider for PLL entry"
    //1.Read PLLXTPRE[17] in RCC_CFGR
    temp = (((RCC->CFGR) & (RCC_CFGR_PLLXTPRE_Msk)) >> RCC_CFGR_PLLXTPRE_Pos);
    //  a.If PLLXTPRE = 0 then HSE is not Divided.
    if (temp == 0)
    {
        system_PLLXTRE = SYSTEM_HSE_FREQ;
    }
    //  b.If PLLXTPRE = 1 then HSE is Divided by 2.
    else
    {
        system_PLLXTRE = SYSTEM_HSE_FREQ / 2;
    }

    //PLLCLK:
    //1.Read PLLSRC[16] in RCC_CFGR to define the source of PLL
    temp = (((RCC->CFGR) & (RCC_CFGR_PLLSRC_Msk)) >> RCC_CFGR_PLLSRC_Pos);
    //  a.IF PLLSRC = 0 the PLL source is from HSI/2 "HSI divided by 2" ie. 4MHz
    if (temp == 0)
    {
        pllclk = SYSTEM_HSI_FREQ / 2;
    }
    //  b.If PLLSRC = 1 the PLL source is derived from HSE Divider "PLLXTPRE"
    else
    {
        pllclk = system_PLLXTRE;
    }

    //2.Read PLLMUL[21:18] in RCC_CFGR to find the PLL multiplication factor
    temp = (((RCC->CFGR) & (RCC_CFGR_PLLMULL_Msk)) >> RCC_CFGR_PLLMULL_Pos);
    uint8_t multiplier = 0;
    switch (temp & 0X0F)
    {
        case 0:
            multiplier = 2;
            break;
        case 1:
            multiplier = 3;
            break;
        case 2:
            multiplier = 4;
            break;
        case 3:
            multiplier = 5;
            break;
        case 4:
            multiplier = 6;
            break;
        case 5:
            multiplier = 7;
            break;
        case 6:
            multiplier = 8;
            break;
        case 7:
            multiplier = 9;
            break;
        case 8:
            multiplier = 10;
            break;
        case 9:
            multiplier = 11;
            break;
        case 10:
            multiplier = 12;
            break;
        case 11:
            multiplier = 13;
            break;
        case 12:
            multiplier = 14;
            break;
        case 13:
            multiplier = 15;
            break;
        case 14:
        case 15:
            multiplier = 16;
            break;
    }
    system_PLLCLK = pllclk * multiplier;

    //SYSCLK:
    //1.Read SWS[3:2] in RCC_CFGR to determine SYSCLK source
    temp = (((RCC->CFGR) & (RCC_CFGR_SWS_Msk)) >> RCC_CFGR_SWS_Pos);
    //  a. if SWS = (0x00)HSI  then SYSCLK is clocked by 8MHz
    if (temp == 0)
    {
        system_SYSCLK = SYSTEM_HSI_FREQ;
    }
    //  b. if SWS = (0x01)HSE  then SYSCLK is clocked directly from the external oscillator
    //      note: need a definition for external crystal frequency
    else if (temp == 1)
    {
        system_SYSCLK = SYSTEM_HSE_FREQ;
    }
    //  c. if SWS = (0x10)PLL  then we need to calculate PLLCLK.
    else if (temp == 2)
    {
        system_SYSCLK = system_PLLCLK;
    }
    //      note: Review the CSS enable and flags to detect if a clock failure is detected and a change in setting is made.
    else
    {
        // Return an error
    }

    //AHB clock: "FCLK - HCLK"
    //1.Read HPRE[7:4] in RCC_CFGR to find the AHB prescaler
//    system_HCLK
    temp = (((RCC->CFGR) & (RCC_CFGR_HPRE_Msk)) >> RCC_CFGR_HPRE_Pos);
    //  a. If HPRE = 0b0xxx then AHB_CLK = SYSCLK with no division.
    if ((temp & 0b1000) == 0)
    {
        system_HCLK = system_SYSCLK;
    }
    else
    {
        switch (temp)
        {
            //  b. IF HPRE = 0b1000 then AHB_CLK = SYSCLK divided by 2.
            case (0b1000):
                system_HCLK = system_SYSCLK / 2;
                break;
                //  c. IF HPRE = 0b1001 then AHB_CLK = SYSCLK divided by 4.
            case (0b1001):
                system_HCLK = system_SYSCLK / 4;
                break;
                //  d. IF HPRE = 0b1010 then AHB_CLK = SYSCLK divided by 8.
            case (0b1010):
                system_HCLK = system_SYSCLK / 8;
                break;
                //  e. IF HPRE = 0b1011 then AHB_CLK = SYSCLK divided by 16.
            case (0b1011):
                system_HCLK = system_SYSCLK / 16;
                break;
                //  f. IF HPRE = 0b1100 then AHB_CLK = SYSCLK divided by 64.
            case (0b1100):
                system_HCLK = system_SYSCLK / 64;
                break;
                //  g. IF HPRE = 0b1101 then AHB_CLK = SYSCLK divided by 128.
            case (0b1101):
                system_HCLK = system_SYSCLK / 128;
                break;
                //  h. IF HPRE = 0b1110 then AHB_CLK = SYSCLK divided by 256.
            case (0b1110):
                system_HCLK = system_SYSCLK / 256;
                break;
                //  i. IF HPRE = 0b1111 then AHB_CLK = SYSCLK divided by 512.
            case (0b1111):
                system_HCLK = system_SYSCLK / 512;
                break;
        }
    }

    //APB1 Clock: "PCLK1" (Maximum Clock frequency is 36MHz)
    //1.Read PPRE1[10:8] in RCC_CFGR to find AB1 clock prescaler.
    temp = (((RCC->CFGR) & (RCC_CFGR_PPRE1_Msk)) >> RCC_CFGR_PPRE1_Pos);
    //  a.IF PPRE1 = 0b0xx then APB1 clock = HCLK with no division.
    if ((temp & 0b100) == 0)
    {
        system_PCLK1 = system_HCLK;
    }
    else
    {
        switch (temp)
        {
            //  b.IF PPRE1 = 0b100 then APB1 clock = HCLK divided by 2.
            case (0b100):
                system_PCLK1 = system_HCLK / 2;
                break;
                //  c.IF PPRE1 = 0b101 then APB1 clock = HCLK divided by 4.
            case (0b101):
                system_PCLK1 = system_HCLK / 4;
                break;
                //  d.IF PPRE1 = 0b110 then APB1 clock = HCLK divided by 8.
            case (0b110):
                system_PCLK1 = system_HCLK / 8;
                break;
                //  e.IF PPRE1 = 0b111 then APB1 clock = HCLK divided by 16.
            case (0b111):
                system_PCLK1 = system_HCLK / 16;
                break;
        }
    }

    //APB2 Clock: "PCLK2" (Maximum Clock frequency is 72MHz)
    //1.Read PPRE2[13:11] in RCC_CFGR to find AB2 clock prescaler.
    temp = (((RCC->CFGR) & (RCC_CFGR_PPRE2_Msk)) >> RCC_CFGR_PPRE2_Pos);
    //  a.IF PPRE2 = 0b0xx then APB2 clock = HCLK with no division.
    if ((temp & 0b100) == 0)
    {
        system_PCLK2 = system_HCLK;
    }
    else
    {
        switch (temp)
        {
            //  b.IF PPRE2 = 0b100 then APB2 clock = HCLK divided by 2.
            case (0b100):
                system_PCLK2 = system_HCLK / 2;
                break;

                //  c.IF PPRE2 = 0b101 then APB2 clock = HCLK divided by 4.
            case (0b101):
                system_PCLK2 = system_HCLK / 4;
                break;

                //  d.IF PPRE2 = 0b110 then APB2 clock = HCLK divided by 8.
            case (0b110):
                system_PCLK2 = system_HCLK / 8;
                break;

                //  e.IF PPRE2 = 0b111 then APB2 clock = HCLK divided by 16.
            case (0b111):
                system_PCLK2 = system_HCLK / 16;
                break;
        }
    }
}

void system_getBusFrequency(system_Clock_t* bus)
{
//    uint32_t value;
    switch (bus->bus_name)
    {
        case (BusClock_HCLK):
            bus->frequency = system_HCLK;
            break;
        case (BusClock_PCLK1):
            bus->frequency = system_PCLK1;
            break;
        case (BusClock_PCLK2):
            bus->frequency = system_PCLK2;
            break;
        case (BusClock_SYSCLK):
            bus->frequency = system_SYSCLK;
            break;
    }
}

