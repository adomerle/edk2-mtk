[Defines]
  PLATFORM_NAME                  = roo
  PLATFORM_GUID                  = 54d47bf2-32f2-4532-9438-1b6971dc59c0
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010019
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = Platform/MediaTek/mt6765/mt6765.fdf
  DEVICE_DXE_FV_COMPONENTS       = Platform/MediaTek/mt6765/mt6765.fdf.inc

!include Platform/MediaTek/mt6765/mt6765.dsc

[BuildOptions.common]
  GCC:*_*_AARCH64_CC_FLAGS = -DENABLE_SIMPLE_INIT

[PcdsFixedAtBuild.common]
  gMediaTekTokenSpaceGuid.PcdMipiFrameBufferWidth|736
  gMediaTekTokenSpaceGuid.PcdMipiFrameBufferHeight|1440
  gMediaTekTokenSpaceGuid.PcdMipiFrameBufferAddress|0x7bee0000

  # Simple Init
  gSimpleInitTokenSpaceGuid.PcdGuiDefaultDPI|400

  gRenegadePkgTokenSpaceGuid.PcdDeviceVendor|"HMD Global"
  gRenegadePkgTokenSpaceGuid.PcdDeviceProduct|"Nokia 3.1 Plus"
  gRenegadePkgTokenSpaceGuid.PcdDeviceCodeName|"roo"
