#include <oI2C.h>
#include "stm32f103xb.h"
#include <stdio.h>
#include "stdlib.h"
#include <stdbool.h>
#include "i2c.h"
#include "LED.h"
#include "uart.h"
#include "ssd1306.h"
#include "ssd1306_test.h"

/* Include for some fonts*/
#include "font.h"


void delay(void );
void delay2(void );
void delay3(void);
void delay4(void);
void ssd1306_test();
void eeprom_write_test();
void eeprom_read_test();
void hmc5883l_init();
void hmc5883l_test();
void init_clock(void);
void testSystemFunctions();






//int __io_putchar(int ch)
//{
//	uart_Send((unsigned char) (ch & 0xFF));
//	return ch;
//}

int __io_putchar(int ch )
{
	ssd1306_PutC((unsigned char) (ch & 0xFF));
	return ch;
}

i2c_Descriptor_t myI2c;

int main(void )
{
	/*Initialize LED pin*/
	led_Init();
	/*Initialize USART1*/
	uart_TX_Init();
	/*Initialize I2C1*/
//	i2c1_Init();
	myI2c.descreptor_Option= i2c_Option_enMasterMode;
	i2c_Init(&myI2c, Peripherals_I2c1, false);
//

	/*Make Indication the code is live*/
	/*Start Toggling some LEDs*/
	red_On();
	green_On();
	delay2();
	green_Off();
	red_Off();
	delay2();


	uart_Send((unsigned char) ('H'));
	uart_Send((unsigned char) ('e'));
	uart_Send((unsigned char) ('l'));
	uart_Send((unsigned char) ('l'));
	uart_Send((unsigned char) ('o'));
	uart_Send((unsigned char) ('\n'));

	/* Write some data in AT24C04*/
//	eeprom_write_test();

	/*Initialize OLED display SSD1306*/
	ssd1306_I2C_Init();

	testSystemFunctions();
    printf("Increasing Speed\n");
    ssd1306_Write_Partial_Frame();


    hmc5883l_init();
	init_clock();
	while (1)
	{
	    testSystemFunctions();
	    hmc5883l_test();
	    delay();

		/* AT24C04 Reading Test */
		eeprom_read_test();

		/* SSD1306 Testing */
		ssd1306_test();
		/* End of SSD1306 Testing*/
		delay();
		delay();
		red_On();
		delay2();
	}
	return 0;
}

void ssd1306_test()
{
	ssd1306_Test_Text();
	ssd1306_Test_ProgressBar();
	ssd1306_Test_round_rect();
	ssd1306_Test__filled_round_rect();
	ssd1306_Test_Draw_Image();
	ssd1306_test_circle();//
	 ssd1306_Test_Rect();//
	 ssd1306_Test_Lines();//
	 ssd1306_Test_Draw_Line_V_1();//
	 ssd1306_Test_Draw_Line_V_1_1();//
	 ssd1306_Test_Draw_Line_H_3();//
	 ssd1306_Test_Draw_Line_H_2();//
	 ssd1306_Test_Draw_Line_H_1();//
	 ssd1306_Test_Lines_2();//
	 ssd1306_Test_Draw_Line_V_2();
	ssd1306_Test_Filled_Circle();
	ssd1306_Test_Draw_Line_V_2();//
//	ssd1306_Test_Draw_Line_H_3();
//	ssd1306_Test_Lines();
//	ssd1306_Test_Rect();
//	ssd1306_Test_Draw_Image2();
//	ssd1306_Clear_Frame();
//	ssd1306_Test_Fill();
//	ssd1306_Test_Frame_Buffer();
//	ssd1306_Test_Draw_Pixel_2();
//	ssd1306_Test_Draw_Line_H_1();
//	ssd1306_Test_Draw_Line_H_2();
//	ssd1306_Test_Draw_Line_V_1();
//	ssd1306_Test_Draw_Line_V_1_1();
//	ssd1306_Test_Filled_Rect();
	ssd1306_I2C_Clear();
}

void eeprom_read_test()
{
	uint8_t eeprom[32];
	ssd1306_Clear_Frame();
	ssd1306_Set_Font(&font_7x10);
	ssd1306_Set_Cursor(0, 0);
	ssd1306_PutSTR("Hello, World!\n");
	ssd1306_Write_Partial_Frame();
	printf("Testing AT24c04\n:Reading...\n");
	ssd1306_Write_Partial_Frame();
	delay();
	I2C1->CR1 |= I2C_CR1_ACK; /*Enable ACK bit*/
	i2c1_Write_Begin(0xA0, 0x00);
	i2c_Begin(0xA1);
	for (uint8_t i=0; i<32;i++)
		 i2c_Read( &eeprom[i] );
	I2C1->CR1 &= ~I2C_CR1_ACK;
	i2c1_End();
	ssd1306_Clear_Frame();
	for (uint8_t i=0; i<32;i++)
	{
		ssd1306_Set_Cursor(0, 0);
		printf("#%i 0x%X  \n",i,eeprom[i]);
		ssd1306_Write_Partial_Frame();
		delay2();
		ssd1306_Clear_Frame();
	}
	ssd1306_PutSTR("Pass\n");
	ssd1306_Write_Partial_Frame();
	delay();
}

void eeprom_write_test()
{
//	i2c1_Write_Begin(0xA0, 0x00);
//	for (uint8_t i = 0; i < 16; i++)
//		i2c1_write(i);
//	i2c1_End();
//	delay2();
//	i2c1_Write_Begin(0xA0, 0x10);
//	for (uint8_t i = 16; i < 32; i++)
//		i2c1_write(i);
//	i2c1_End();
}

void hmc5883l_init(void)
{
    //    send 0x3C 0x00 0x70
        i2c1_Write_Begin(0x3C, 0x00);
        i2c1_write(0x70);
        i2c1_End();
    //    send 0x3C 0x01 0xA0
        i2c1_Write_Begin(0x3C, 0x01);
        i2c1_write(0x80);
        i2c1_End();
    //    send 0x3C 0x02 0x00
        i2c1_Write_Begin(0x3C, 0x02);
        i2c1_write(0x00);
        i2c1_End();

}

void hmc5883l_test()
{
    ssd1306_Clear_Frame();
    ssd1306_Set_Font(&font_5x7);
    ssd1306_Set_Cursor(0, 0);
    ssd1306_Write_Partial_Frame();
    delay2();

//    Loop
//        ssd1306_Set_Font(&font_5x7);
//    Send 0x3D 0x06
    uint8_t loop = 10;
    while(loop)
    {
//
//        delay3();
        i2c1_Write_Begin(0x3C, 0x03);
        i2c1_End();
        delay2();
//    Read all 6 bytes
        I2C1->CR1 |= I2C_CR1_ACK; /*Enable ACK bit*/
        i2c_Begin(0x3D);
        int16_t x_axis;
        int16_t y_axis;
        int16_t z_axis;
        uint8_t buffer[6];

        for (uint8_t i = 0; i < 5; i++)
        {
            i2c_Read(&buffer[i]);
        }
        I2C1->CR1 &= ~I2C_CR1_ACK;
        i2c_Read(&buffer[5]);
        i2c1_End();
        x_axis = buffer[1] | (buffer[0] << 8);
        y_axis = buffer[3] | (buffer[2] << 8);
        z_axis = buffer[5] | (buffer[4] << 8);
//        ssd1306_Write_Partial_Frame();


        ssd1306_Clear_Frame();
        ssd1306_Set_Cursor(0, 0);
        printf("   Digital Compass\n\n");
        printf("0x%02X%02X, 0x%02X%02X 0x%02X%02X\n\n",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5]);
        printf("X-Axis = %d\n", x_axis);
        printf("Y-Axis = %d\n", y_axis);
        printf("Z-Axis = %d\n", z_axis);
        ssd1306_Write_Partial_Frame();
        delay();
//        delay();
//        delay();
        loop--;
    }


//    ssd1306_PutSTR("Testing HM5883L\n3-Axis Digital Compass\n");
//    ssd1306_Write_Partial_Frame();
////    printf("Testing AT24c04\n:Reading...\n");
//    uint8_t hm5883l_id[3];
////    I2C1->CR1 |= I2C_CR1_ACK; /*Enable ACK bit*/
//    i2c1_Write_Begin(0x3C, 0x0A);
//    i2c1_End();
//    ssd1306_PutSTR("HM5883L Responded\n");
//    ssd1306_Write_Partial_Frame();
//
//    I2C1->CR1 |= I2C_CR1_ACK; /*Enable ACK bit*/
//    i2c_Begin(0x3D);
//    i2c_Read(&hm5883l_id[0]);
//    i2c_Read(&hm5883l_id[1]);
//    I2C1->CR1 &= ~I2C_CR1_ACK;
//    i2c_Read(&hm5883l_id[2]);
//    i2c1_End();
//    ssd1306_PutSTR("Complete \nID:[");
//    ssd1306_PutC(hm5883l_id[0]);
////    ssd1306_PutC(',');
//    ssd1306_PutC(hm5883l_id[1]);
////    ssd1306_PutC(',');
//    ssd1306_PutC(hm5883l_id[2]);
//    ssd1306_PutC(']');
//    ssd1306_Write_Partial_Frame();
//    delay2();
//    delay2();
//    delay2();


    /*
//     * 1. Write CRA (00) – send 0x3C 0x00 0x70 (8-average, 15 Hz default, normal measurement)
//2. Write CRB (01) – send 0x3C 0x01 0xA0 (Gain=5, or any other desired gain)
//3. Write Mode (02) – send 0x3C 0x02 0x00 (Continuous-measurement mode)
//4. Wait 6 ms or monitor status register or DRDY hardware interrupt pin
//5. Loop
//Send 0x3D 0x06 (Read all 6 bytes. If gain is changed then this data set is using previous gain)
Convert three 16-bit 2’s compliment hex values to decimal values and assign to X, Z, Y, respectively.
Send 0x3C 0x03 (point to first data register 03)
Wait about 67 ms (if 15 Hz rate) or monitor status register or DRDY hardware interrupt pin
End_loop
     * */

}

void testSystemFunctions()
{
    system_Calculate_Sysclk();
    system_Clock_t myClock;
    myClock.bus_name = BusClock_HCLK;
    system_getBusFrequency(&myClock);
    uint8_t freq = myClock.frequency / 1000000;
    ssd1306_Clear_Frame();
    ssd1306_Set_Font(&font_5x7);
    ssd1306_Set_Cursor(0, 0);
    printf("   System Frequency\n         %dMHz\n",freq);
    printf("       %02ld Hz\n",myClock.frequency);
    ssd1306_Write_Partial_Frame();
    delay();
    delay();
    delay();
    delay();
    delay();

}

/*--------------------------------------------------------------------------*\

 Function:      init_clock()

 Description:   configure SysClock to run at 72MHz

 Parameters:    void
 Returns:       void

\*--------------------------------------------------------------------------*/

void init_clock(void)
{
    // Conf clock : 72MHz using HSE 8MHz crystal w/ PLL X 9 (8MHz x 9 = 72MHz)
    FLASH->ACR      |= FLASH_ACR_LATENCY_2; // Two wait states, per datasheet
    RCC->CFGR       |= RCC_CFGR_PPRE1_2;    // prescale AHB1 = HCLK/2
    RCC->CR         |= RCC_CR_HSEON;        // enable HSE clock
    while( !(RCC->CR & RCC_CR_HSERDY) );    // wait for the HSEREADY flag

    RCC->CFGR       |= RCC_CFGR_PLLSRC;     // set PLL source to HSE
    RCC->CFGR       |= RCC_CFGR_PLLMULL9;   // multiply by 9
    RCC->CR         |= RCC_CR_PLLON;        // enable the PLL
    while( !(RCC->CR & RCC_CR_PLLRDY) );    // wait for the PLLRDY flag

    RCC->CFGR       |= RCC_CFGR_SW_PLL;     // set clock source to pll

    while( !(RCC->CFGR & RCC_CFGR_SWS_PLL) );    // wait for PLL to be CLK

//    SystemCoreClockUpdate();                // calculate the SYSCLOCK value
}
