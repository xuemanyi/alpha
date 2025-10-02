#include "drv.h"

#define DTSLED_CNT  1
#define DTSLED_NAME "dtsled"
#define LEDOFF      0
#define LEDON       1

static void __iomem *IMX6U_CCM_CCGR1;
static void __iomem *SW_MUX_GPIO1_IO03;
static void __iomem *SW_PAD_GPIO1_IO03;
static void __iomem *GPIO1_DR;
static void __iomem *GPIO1_GDIR;