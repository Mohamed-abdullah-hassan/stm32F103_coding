1   VBAT                    S           VBAT
2   PC13 - TAMPERRTC(5)     I/O         PC13(6)     TAMPER-RTC 
3   PC14-OSC32_IN(5)        I/O         PC14(6)     OSC32_IN 
4   PC15-OSC32_OUT(5)       I/O         PC15(6)     OSC32_OUT
5   OSC_IN                  I           OSC_IN                              PD0(7)
6   OSC_OUT                 O           OSC_OUT                             PD1(7)
7   NRST                    I/O         NRST
8   VSSA                    S           VSSA
9   VDDA                    S           VDDA
10  PA0-WKUP                I/O         PA0         WKUP/               
                                                    USART2_CTS(9)/
                                                    ADC12_IN0/
                                                    TIM2_CH1_ETR(9)

11  PA1                     I/O         PA1         USART2_RTS(9)/
                                                    ADC12_IN1/
                                                    TIM2_CH2(9)

12  PA2                     I/O         PA2         USART2_TX(9)/
                                                    ADC12_IN2/
                                                    TIM2_CH3(9)

13  PA3                     I/O         PA3         USART2_RX(9)/
                                                    ADC12_IN3/
                                                    TIM2_CH4(9)
                                                    
14  PA4                     I/O         PA4         SPI1_NSS(9)/
                                                    USART2_CK(9)/
                                                    ADC12_IN4

15  PA5                     I/O         PA5         SPI1_SCK(9)/
                                                    ADC12_IN5

16  PA6                     I/O         PA6         SPI1_MISO(9)/      TIM1_BKIN
                                                    ADC12_IN6/
                                                    TIM3_CH1(9)
                                                    
K3 M4 17 H4 23 32 14 PA7 I/O - PA7
SPI1_MOSI(9)/
ADC12_IN7/
TIM3_CH2(9)
TIM1_CH1N
G4 K5 - H5 24 33 PC4 I/O - PC4 ADC12_IN14 -
H4 L5 - H6 25 34 PC5 I/O - PC5 ADC12_IN15 -
J4 M5 18 F5 26 35 15 PB0 I/O - PB0 ADC12_IN8/
TIM3_CH3(9) TIM1_CH2N
K4 M6 19 G5 27 36 16 PB1 I/O - PB1 ADC12_IN9/
TIM3_CH4(9) TIM1_CH3N

