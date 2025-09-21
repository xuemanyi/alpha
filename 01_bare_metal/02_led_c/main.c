#include "main.h"

void enable_clk(void)
{
    CCM_CCGR0 = 0xffffffff;
    CCM_CCGR1 = 0xffffffff;
    CCM_CCGR2 = 0xffffffff;
    CCM_CCGR3 = 0xffffffff;    
    CCM_CCGR4 = 0xffffffff;
    CCM_CCGR5 = 0xffffffff;
    CCM_CCGR6 = 0xffffffff;
}

void init_led(void)
{
    SW_MUX_GPIO1_IO03 = 0x5;
    SW_PAD_GPIO1_IO03 = 0X10B0;

    GPIO1_DR = 0X0;
}

void turn_on_led(void)
{
    GPIO1_DR &= ~(1 << 3);
}

void turn_off_led(void)
{
    GPIO1_DR |= (1 << 3);
}

void delay_short(volatile unsigned int n)
{
    while (n--)
    {

    }
}

void delay(volatile unsigned int n)
{
    while (n--)
    {

    }
}

int main(void)
{
    enable_clk();
    init_led();

    while (1)
    {
        turn_off_led();
        delay(500);

        turn_on_led();
        delay(500);
    }

    return 0;
}