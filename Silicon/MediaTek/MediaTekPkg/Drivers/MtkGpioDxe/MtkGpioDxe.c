#include <Library/IoLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MtkGpioImplLib.h>
#include <Library/DebugLib.h>

#include <Protocol/MtkGpio.h>

#define PIN_SG_OFFSET(p) ((p/32)*0x10) /* Offset for Set|Get values */

// Internal functions
VOID
GpioWrite (
  IN MTK_GPIO *This,
  IN UINT32    Reg,
  IN UINT32    Value
  )
{
  UINT32 GpioAddr = This->PlatformInfo->BaseAddr + Reg;

  MmioWrite32(GpioAddr, Value);
}

VOID
GpioRead (
  IN  MTK_GPIO *This,
  IN  UINT32    Reg,
  OUT UINT32   *Value
  )
{
  UINT32 GpioAddr = This->PlatformInfo->BaseAddr + Reg;

  *Value = MmioRead32(GpioAddr);
}

EFI_STATUS
GetMode (
  IN  MTK_GPIO       *This,
  IN  UINT32          Pin,
  OUT MTK_GPIO_MODE  *Mode
  )
{
  UINT32 Reg, PinMask, DirValue;

  PinMask = 1U << (Pin % 32);
  Reg = This->PlatformInfo->DirOffset + PIN_SG_OFFSET(Pin);

  GpioRead(This, Reg, &DirValue);

  if (DirValue & PinMask)
  {
    *Mode = GPIO_MODE_OUTPUT;
  }
  else
  {
    *Mode = GPIO_MODE_INPUT;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
Get (
  IN  MTK_GPIO      *This,
  IN  UINT32         Pin,
  OUT UINT32        *Value
  )
{
  UINT32 PinMask, Reg, BankValue, Dir, Mode;

  Dir = This->PlatformInfo->DataOutOffset;

  GetMode(This, Pin, &Mode);

  if (Mode == GPIO_MODE_INPUT)
  {
    Dir = This->PlatformInfo->DataInOffset;
  }

  PinMask = 1U << (Pin % 32);
  Reg = Dir + PIN_SG_OFFSET(Pin);
  
  GpioRead(This, Reg, &BankValue);

  if (BankValue & PinMask)
  {
    *Value = 0;
  }
  else
  {
    *Value = 1;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
Set (
  IN MTK_GPIO      *This,
  IN UINT32         Pin,
  IN MTK_GPIO_MODE  Mode
  )
{
  if (Mode < 8)
  {
    UINT32 Reg, ModeBit, DataSet, DataReset;

    Reg = This->PlatformInfo->ModeOffset + ((Pin / 8) * 0x010);
    ModeBit = Pin > 8 ? (Pin % 8) * 4 : Pin * 4;

    GpioRead(This, Reg + This->PlatformInfo->SetOffset, &DataSet);
    GpioRead(This, Reg + This->PlatformInfo->ResetOffset, &DataReset);

    for (int i = 0; i < 3; i++)
    {
      if (Mode & (1 << i))
      {
        DataSet |= (1U << (ModeBit + i));
      }
      else
      {
        DataReset |= (1U << (ModeBit + i));
      }
    }

    GpioWrite(This, Reg + This->PlatformInfo->SetOffset, DataSet);
    GpioWrite(This, Reg + This->PlatformInfo->ResetOffset, DataReset);
  }
  else
  {
    UINT32 PinMask, Reg, BankValue;

    PinMask = 1U << (Pin % 32);
    Reg = This->PlatformInfo->DirOffset + PIN_SG_OFFSET(Pin);

    if (Mode == GPIO_MODE_INPUT)
    {
      Reg += This->PlatformInfo->ResetOffset;
    }
    if (Mode == GPIO_MODE_OUTPUT)
    {
      Reg += This->PlatformInfo->SetOffset;
    }

    GpioRead(This, Reg, &BankValue);

    BankValue |= PinMask;

    GpioWrite(This, Reg, BankValue);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
SetPull (
  IN MTK_GPIO      *This,
  IN UINT32         Pin,
  IN MTK_GPIO_PULL  State
  )
{
  UINT32 PinMask, Reg, BankValue;

  PinMask = 1U << (Pin % 32);
  Reg = This->PlatformInfo->DataOutOffset + PIN_SG_OFFSET(Pin);

  if (State == GPIO_PULL_DOWN)
  {
    Reg += This->PlatformInfo->ResetOffset;
  }
  if (State == GPIO_PULL_UP)
  {
    Reg += This->PlatformInfo->SetOffset;
  }

  GpioRead(This, Reg, &BankValue);

  BankValue |= PinMask;

  GpioWrite(This, Reg, BankValue);

  return EFI_SUCCESS;
}


MTK_GPIO MtkGpio = {
  Get,
  Set,
  GetMode,
  SetPull,
  &PlatformInfo
};

EFI_STATUS
EFIAPI
MtkGpioInitialize(
  IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  Status = gBS->InstallMultipleProtocolInterfaces(
    &ImageHandle,
    &gMtkGpioProtocolGuid,
    &MtkGpio,
    NULL);

  return Status;
}
