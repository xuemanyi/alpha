#ifndef __IMX6UL_H__
#define __IMX6UL_H__

/* base addr of reg */
#define CCM_BASE            (0X020C4000)
#define CCM_ABALOG_BASE     (0X020C8000)
#define IOMUX_SW_MUX_BASE   (0X020E0014)
#define IOMUX_SW_PAD_BASE   (0X020E0204)
#define GPIO1_BASE          (0X0209C000)
#define GPIO4_BASE          (0x020A8000)
#define GPIO5_BASE          (0x020AC000)
#define GPIO3_BASE          (0x020A4000)
#define GPIO2_BASE          (0x020A0000)

typedef struct
{
    volatile unsigned int BOOT_MODE0;
    volatile unsigned int BOOT_MODE1;
    volatile unsigned int SNVS_TAMPER0;

    volatile unsigned int CSI_DATA05;
    volatile unsigned int CSI_DATA06;
    volatile unsigned int CSI_DATA07;
    volatile unsigned int CSI_DATA04;
}IOMUX_SW_MUX_Type;

typedef struct
{
    volatile unsigned int BOOT_MODE1;
    volatile unsigned int SNVS_TAMPER0;
    volatile unsigned int BOOT_MODE0;
    
    volatile unsigned int CSI_DATA05;
    volatile unsigned int CSI_DATA06;
    volatile unsigned int CSI_DATA07;
    volatile unsigned int CSI_DATA04;
}IOMUX_SW_MUX_Type;

typed struct
{
    
}

#endif