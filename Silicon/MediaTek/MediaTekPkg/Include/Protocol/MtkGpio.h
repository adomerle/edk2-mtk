#ifndef __MTK_GPIO_H__
#define __MTK_GPIO_H__
//
// Protocol interface structure
//
typedef struct _MTK_GPIO MTK_GPIO;

//
// Data Types
//
typedef enum {
  GPIO_MODE_INPUT              = 0,
  GPIO_MODE_OUTPUT             = 8,
  GPIO_MODE_SPECIAL_FUNCTION_1 = 1,
  GPIO_MODE_SPECIAL_FUNCTION_2 = 2,
  GPIO_MODE_SPECIAL_FUNCTION_3 = 3,
  GPIO_MODE_SPECIAL_FUNCTION_4 = 4,
  GPIO_MODE_SPECIAL_FUNCTION_5 = 5,
  GPIO_MODE_SPECIAL_FUNCTION_6 = 6,
  GPIO_MODE_SPECIAL_FUNCTION_7 = 7
} MTK_GPIO_MODE;

typedef struct {
  UINT32 BaseAddr;
  UINT32 SetOffset;
  UINT32 ResetOffset;
  UINT32 DirOffset;
  UINT32 DataOutOffset;
  UINT32 DataInOffset;
  UINT32 ModeOffset;
} MTK_GPIO_PLATFORM_INFO;

typedef enum {
  GPIO_PULL_NONE,
  GPIO_PULL_UP,
  GPIO_PULL_DOWN
} MTK_GPIO_PULL;

//
// Function Prototypes
//
typedef
EFI_STATUS
(EFIAPI *MTK_GPIO_GET)(
  IN  MTK_GPIO      *This,
  IN  UINT32         Pin,
  OUT UINT32        *Value
  );

/*++

Routine Description:

  Gets the state of a GPIO pin

Arguments:

  This  - pointer to protocol
  Pin   - which pin to read
  Value - state of the pin

Returns:

  EFI_SUCCESS - GPIO state returned in Value

--*/

typedef
EFI_STATUS
(EFIAPI *MTK_GPIO_SET)(
  IN MTK_GPIO      *This,
  IN UINT32         Pin,
  IN MTK_GPIO_MODE Mode
  );

/*++

Routine Description:

  Sets the state of a GPIO pin

Arguments:

  This  - pointer to protocol
  Pin   - which pin to modify
  Mode  - mode to set

Returns:

  EFI_SUCCESS - GPIO set as requested

--*/

typedef
EFI_STATUS
(EFIAPI *MTK_GPIO_GET_MODE)(
  IN  MTK_GPIO         *This,
  IN  UINT32            Pin,
  OUT MTK_GPIO_MODE    *Mode
  );

/*++

Routine Description:

  Gets the mode (function) of a GPIO pin

Arguments:

  This  - pointer to protocol
  Pin   - which pin
  Mode  - pointer to output mode value

Returns:

  EFI_SUCCESS - mode value retrieved

--*/

typedef
EFI_STATUS
(EFIAPI *MTK_GPIO_SET_PULL)(
  IN  MTK_GPIO      *This,
  IN  UINT32         Pin,
  IN  MTK_GPIO_PULL Direction
  );

/*++

Routine Description:

  Sets the pull-up / pull-down resistor of a GPIO pin

Arguments:

  This  - pointer to protocol
  Pin   - which pin
  Direction - pull-up, pull-down, or none

Returns:

  EFI_SUCCESS - pin was set

--*/

struct _MTK_GPIO {
  MTK_GPIO_GET              Get;
  MTK_GPIO_SET              Set;
  MTK_GPIO_GET_MODE         GetMode;
  MTK_GPIO_SET_PULL         SetPull;
  MTK_GPIO_PLATFORM_INFO   *PlatformInfo;
};

extern EFI_GUID  gMtkGpioProtocolGuid;

typedef struct _GPIO_CONTROLLER          GPIO_CONTROLLER;
typedef struct _PLATFORM_GPIO_CONTROLLER PLATFORM_GPIO_CONTROLLER;

struct _GPIO_CONTROLLER {
  UINTN    RegisterBase;
  UINTN    GpioIndex;
  UINTN    InternalGpioCount;
};

struct _PLATFORM_GPIO_CONTROLLER {
  UINTN              GpioCount;
  UINTN              GpioControllerCount;
  GPIO_CONTROLLER    *GpioController;
};

extern EFI_GUID  gPlatformGpioProtocolGuid;

#endif
