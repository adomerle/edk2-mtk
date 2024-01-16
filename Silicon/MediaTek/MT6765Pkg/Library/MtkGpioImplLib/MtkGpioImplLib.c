/*
 * MT6765 GPIO Configuration
 */

#include <Protocol/MtkGpio.h>

MTK_GPIO_PLATFORM_INFO PlatformInfo = {
  0x10005000, /* Base             */
  0x4,        /* Set Reg Offset   */ 
  0x8,        /* Reset Reg Offset */
  0x0,        /* DIR Offset       */
  0x100,      /* DataOut Offset   */
  0x200,      /* DataIn Offset    */
  0x300       /* Mode Offset      */
};
