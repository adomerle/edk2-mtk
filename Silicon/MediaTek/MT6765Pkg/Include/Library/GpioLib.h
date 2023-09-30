#ifndef _GPIO_LIB_H_
#define _GPIO_LIB_H_

#define GPIO_BASE  (0x10005000)
#define GPIO_SET   (0x4)
#define GPIO_RESET (0x8)

#define GPIO_DIR  (0x0)
#define GPIO_DOUT (0x100)
#define GPIO_DIN  (0x200)
#define GPIO_MODE (0x300)

typedef enum {
	GPIO_LEVEL_LOW   = 0,
	GPIO_LEVEL_HIGH  = 1
} GPIO_LEVEL;

typedef enum {
	GPIO_DIR_INPUT  = 0,
	GPIO_DIR_OUTPUT = 1
} GPIO_DIR_MODE;

typedef enum {
	GPIO_FUNC_GPIO = 0,
	GPIO_FUNC1 = 1,
	GPIO_FUNC2 = 2,
	GPIO_FUNC3 = 3,
	GPIO_FUNC4 = 4,
	GPIO_FUNC5 = 5,
	GPIO_FUNC6 = 6,
	GPIO_FUNC7 = 7
} GPIO_FUNC;

VOID
GpioSetOut(UINT32 pin, GPIO_LEVEL out);

VOID
GpioSetDir(UINT32 pin, GPIO_DIR_MODE dir);

VOID
GpioSetMode(UINT32 pin, GPIO_FUNC func);

GPIO_LEVEL
GpioGetOut(UINT32 pin);

GPIO_LEVEL
GpioGetIn(UINT32 pin);

GPIO_DIR_MODE
GpioGetDir(UINT32 pin);

#endif // _GPIO_LIB_H_
