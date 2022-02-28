#include "bsp_lcd.h"
#include "bsp_gpio.h"
#include "stdio.h"
#include "bsp_delay.h"
/*屏幕参数结构体变量*/
lcd_tft_config_t lcd_tft_dev;
void lcd_init(void)
{
    unsigned short lcd_id = 0;
    lcd_id = lcd_panelID_read();
    printf("LCD ID = %#x\r\n",lcd_id);

    /*初始化屏幕IO*/
    lcd_gpio_init();
    lcd_reset();
    delay_ms(10);
    lcd_noreset();

    /*根据不同的屏幕ID设置屏幕参数*/
    if(lcd_id == ATK4342)
    {
        lcd_tft_dev.height = 272;
        lcd_tft_dev.width = 480;
        lcd_tft_dev.vspw = 0;
        lcd_tft_dev.vbpd = 8;
        lcd_tft_dev.vfpd = 8;
        lcd_tft_dev.hspw = 0;
        lcd_tft_dev.hbpd = 40;
        lcd_tft_dev.hfpd = 5;
    }
    lcd_tft_dev.pixsize = 4;    /*每个像素4个字节*/
    lcd_tft_dev.framebuffer = LCD_FRAMEBUFF_ADDR;
    lcd_tft_dev.forecolor = LCD_BLACK;  /*前景色为黑色*/
    lcd_tft_dev.backcolor = LCD_WHITE;  /*背景色为白色*/
    
}
/*复位LCD控制器*/
void lcd_reset(void)
{
    LCDIF->CTRL |= 1<<31;
}
/*停止复位*/
void lcd_noreset(void)
{
    LCDIF->CTRL &= ~(1<<31);
}
/*使能LCD控制器*/
void lcd_enable(void)
{
    LCDIF->CTRL |= (1<<0);
}
/*
 * 读取屏幕ID，
 * 描述：LCD_DATA23=R7(M0);LCD_DATA15=G7(M1);LCD_DATA07=B7(M2);
 * 		M2:M1:M0
 *		0 :0 :0	//4.3寸480*272 RGB屏,ID=0X4342
 *		0 :0 :1	//7寸800*480 RGB屏,ID=0X7084
 *	 	0 :1 :0	//7寸1024*600 RGB屏,ID=0X7016
 *  	1 :0 :1	//10.1寸1280*800,RGB屏,ID=0X1018
 *		1 :0 :0	//4.3寸800*480 RGB屏,ID=0X4384
 * @param 		: 无
 * @return 		: 屏幕ID
 */
unsigned short lcd_panelID_read(void)
{
    unsigned char IDX = 0;
    /*打开模拟开关，设置LCD_VSYNC为高电平*/
    gpio_pin_config_t lcd_config;
    IOMUXC_SetPinMux(IOMUXC_LCD_VSYNC_GPIO3_IO03,0);
    IOMUXC_SetPinConfig(IOMUXC_LCD_VSYNC_GPIO3_IO03,0x10b0);
    /*GPIO初始化*/
    lcd_config.direction = kGPIO_DigitalOutput;
    lcd_config.outputLogic = 1;
    gpio_init(GPIO3, 3,&lcd_config);

    /*读取屏幕ID*/
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA07_GPIO3_IO12,0);   /*B7(M2)*/
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA15_GPIO3_IO20,0);   /*G7(M1)*/
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA23_GPIO3_IO28,0);   /*R7(M0)*/

    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA07_GPIO3_IO12,0xf080);   /*B7(M2)*/
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA15_GPIO3_IO20,0xf080);   /*G7(M1)*/
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA23_GPIO3_IO28,0xf080);   /*R7(M0)*/
    lcd_config.direction = kGPIO_DigitalInput;
    gpio_init(GPIO3,12,&lcd_config);
    gpio_init(GPIO3,20,&lcd_config);
    gpio_init(GPIO3,28,&lcd_config);

    IDX = (unsigned char)gpio_pin_read(GPIO3,28);
    IDX |= (unsigned char)gpio_pin_read(GPIO3,20)<<1;
    IDX |= (unsigned char)gpio_pin_read(GPIO3,12)<<2;

    if(IDX == 0) return ATK4342;
    else if(IDX == 1) return ATK7084;
    else if(IDX == 2) return ATK7016;
    else if(IDX == 4) return ATK4384;
    else if(IDX == 5) return ATK1018;
    else return 0;
}
/*屏幕IO初始化*/
void lcd_gpio_init(void)
{
    /*IO复用*/
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA00_LCDIF_DATA00,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA01_LCDIF_DATA01,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA02_LCDIF_DATA02,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA03_LCDIF_DATA03,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA04_LCDIF_DATA04,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA05_LCDIF_DATA05,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA06_LCDIF_DATA06,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA07_LCDIF_DATA07,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA08_LCDIF_DATA08,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA09_LCDIF_DATA09,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA10_LCDIF_DATA10,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA11_LCDIF_DATA11,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA12_LCDIF_DATA12,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA13_LCDIF_DATA13,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA14_LCDIF_DATA14,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA15_LCDIF_DATA15,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA16_LCDIF_DATA16,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA17_LCDIF_DATA17,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA18_LCDIF_DATA18,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA19_LCDIF_DATA19,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA20_LCDIF_DATA20,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA21_LCDIF_DATA21,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA22_LCDIF_DATA22,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA23_LCDIF_DATA23,0);

    IOMUXC_SetPinMux(IOMUXC_LCD_CLK_LCDIF_CLK,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_ENABLE_LCDIF_ENABLE,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_HSYNC_LCDIF_HSYNC,0);
    IOMUXC_SetPinMux(IOMUXC_LCD_VSYNC_LCDIF_VSYNC,0);
    
    /*IO电气属性
     *bit[16]：       0   HYS关闭
     *bit[15:14]:    0   默认22K下拉
     *bit[13]:       0   pull功能
     *bit[12]:       0   pull/keeper使能
     *bit[11]：      0   关闭开路输出
     *bit[7:6]:     10  速度100MHz
     *bit[5:3]:     111 驱动能力为R0/7
     *bit[0]:       1   高转换率
    */
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA00_LCDIF_DATA00,0xb9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA01_LCDIF_DATA01,0xb9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA02_LCDIF_DATA02,0xb9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA03_LCDIF_DATA03,0xb9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA04_LCDIF_DATA04,0xb9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA05_LCDIF_DATA05,0xb9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA06_LCDIF_DATA06,0xb9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA07_LCDIF_DATA07,0xb9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA08_LCDIF_DATA08,0xb9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA09_LCDIF_DATA09,0xb9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA10_LCDIF_DATA10,0xb9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA11_LCDIF_DATA11,0xb9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA12_LCDIF_DATA12,0xb9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA13_LCDIF_DATA13,0xb9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA14_LCDIF_DATA14,0xb9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA15_LCDIF_DATA15,0xb9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA16_LCDIF_DATA16,0xb9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA17_LCDIF_DATA17,0xb9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA18_LCDIF_DATA18,0xb9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA19_LCDIF_DATA19,0xb9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA20_LCDIF_DATA20,0xb9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA21_LCDIF_DATA21,0xb9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA22_LCDIF_DATA22,0xb9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA23_LCDIF_DATA23,0xb9);

    IOMUXC_SetPinConfig(IOMUXC_LCD_CLK_LCDIF_CLK,0xb9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_ENABLE_LCDIF_ENABLE,0xb9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_HSYNC_LCDIF_HSYNC,0xb9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_VSYNC_LCDIF_VSYNC,0xb9);

    /*背光IO初始化*/
    gpio_pin_config_t lcd_bl_config;
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO08_GPIO1_IO08,0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO08_GPIO1_IO08,0x10b0);
    lcd_bl_config.direction = kGPIO_DigitalOutput;
    lcd_bl_config.outputLogic = 1;
    gpio_init(GPIO1,8,&lcd_bl_config);

}