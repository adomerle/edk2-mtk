#include <Library/ArmLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/GpioLib.h>

VOID GpioWrite(
	UINT32 offset,
	UINT32 value
) {
	UINTN gpioAddr = GPIO_BASE + offset;

	MmioWrite32(gpioAddr, value);
}

UINT32 GpioRead(
	UINT32 offset
) {
	UINTN gpioAddr = GPIO_BASE + offset;

	return MmioRead32(gpioAddr);
}

UINT32
GpioGetDOffset(
	UINT32 pin
) {
	return ((pin / 32) * 0x10);
}

UINT32
GpioGetModeBit(
	UINT32 pin
) {
	if (pin > 8)
	{
		pin = pin % 8;
	}
	
	return pin * 4;
}

UINT32
GpioGetModeOffset(
	UINT32 pin
) {
	return (pin / 8) * 0x010;
}

VOID
GpioSetOut(
	UINT32 pin,
	GPIO_LEVEL out
) {
	UINT32 offset = GPIO_DOUT + GpioGetDOffset(pin);
	UINT32 bitMask = 1U << (pin % 32);

	if (out == GPIO_LEVEL_LOW)
	{
		offset += GPIO_RESET;
		
	}
	else
	{
		offset += GPIO_SET;
	}
	
	GpioWrite(offset, GpioRead(offset) | bitMask);
}

VOID
GpioSetDir(
	UINT32 pin,
	GPIO_DIR_MODE dir
) {
	UINT32 offset = GPIO_DIR + GpioGetDOffset(pin);
	UINT32 bitMask = 1U << (pin % 32);

	if (dir == GPIO_DIR_INPUT)
	{
		offset += GPIO_RESET;
	}
	else
	{
		offset += GPIO_SET;
		
	}

	GpioWrite(offset, GpioRead(offset) | bitMask);
}

VOID
GpioSetMode(
	UINT32 pin,
	GPIO_FUNC func
) {
	UINT32 offset = GPIO_MODE + GpioGetModeOffset(pin);
	UINT32 bit = GpioGetModeBit(pin);
	UINT32 funcNum = (int) func;

	UINT32 data_set = GpioRead(offset + GPIO_SET);
	UINT32 data_reset = GpioRead(offset + GPIO_RESET);

	for (int i = 0; i < 3; i++)
	{
		if (funcNum & (1 << i))
		{
			data_set |= (1U << (bit + i));
		}
		else
		{
			data_reset |= (1U << (bit + i));
		}
	}

	GpioWrite(offset + GPIO_SET, data_set);
	GpioWrite(offset + GPIO_RESET, data_reset);
}

GPIO_LEVEL
GpioGetOut(
	UINT32 pin
) {
	UINT32 offset = GPIO_DOUT + GpioGetDOffset(pin);
	UINT32 bitMask = 1U << (pin % 32);
	UINT32 gpioValue = GpioRead(offset);

	if (gpioValue & bitMask)
	{
		return GPIO_LEVEL_HIGH;
	}
	else
	{
		return GPIO_LEVEL_LOW;
	}
}

GPIO_LEVEL
GpioGetIn(
	UINT32 pin
) {
	UINT32 offset = GPIO_DIN + GpioGetDOffset(pin);
	UINT32 bitMask = 1U << (pin % 32);
	UINT32 gpioValue = GpioRead(offset);

	if (gpioValue & bitMask)
	{
		return GPIO_LEVEL_HIGH;
	}
	else
	{
		return GPIO_LEVEL_LOW;
	}
}

GPIO_DIR_MODE
GpioGetDir(
	UINT32 pin
) {
	UINT32 offset = GPIO_DIN + GpioGetDOffset(pin);
	UINT32 bitMask = 1U << (pin % 32);
	UINT32 gpioValue = GpioRead(offset);

	if (gpioValue & bitMask)
	{
		return GPIO_DIR_OUTPUT;
	}
	else
	{
		return GPIO_DIR_INPUT;
	}
}
