# MCU
MCU = cortex-m0plus
ARMV = 6
USE_FPU = no
MCU_FAMILY = HT32
MCU_SERIES = HT32F523xx
MCU_LDSCRIPT = HT32F52352_ANNEPRO2D
MCU_STARTUP = ht32f523xx

CONSOLE_ENABLE=yes

BOARD = ANNEPRO2D

# Bootloader selection
BOOTLOADER = custom
PROGRAM_CMD = annepro2_tools --boot $(BUILD_DIR)/$(TARGET).bin

# RGB matrix (direct-drive AW20216S on SPI0) is enabled via keyboard.json.

# BLE module UART + local LED map
SRC = \
	annepro2_ble.c \
	led.c \
