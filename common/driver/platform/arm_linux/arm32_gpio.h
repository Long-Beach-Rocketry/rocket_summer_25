/**
 * @file arm32_gpio.h
 * @brief ARM32 Linux GPIO Driver Implementation
 * 
 * Provides a hardware abstraction layer for controlling GPIO pins on ARM32 
 * Linux systems using the sysfs interface (/sys/class/gpio). This driver 
 * implements the generic GPIO interface for seamless integration with 
 * hardware-agnostic applications.
 * 
 * @author Ayleen Pezz
 * @date 2025
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

/** @brief Maximum number of GPIO pins supported by this driver */
#define ARM_GPIO_MAX_PINS 28

/**
 * @brief GPIO pin direction modes.
 * 
 * Defines whether a GPIO pin is configured for input or output operations.
 */
typedef enum
{
    ARM_GPIO_INPUT = 0, /**< Pin configured as input (read-only) */
    ARM_GPIO_OUTPUT = 1 /**< Pin configured as output (read/write) */
} ArmGpioMode;

/**
 * @brief GPIO pin configuration parameters.
 * 
 * Contains all configuration settings for a GPIO pin including direction
 * and pull-up/pull-down resistor settings.
 */
typedef struct
{
    ArmGpioMode mode; /**< Pin direction (input/output) */
    bool pull_up;     /**< Enable internal pull-up resistor */
    bool pull_down;   /**< Enable internal pull-down resistor */
} ArmGpioConfig;

/**
 * @brief Private GPIO implementation data.
 * 
 * Contains Linux sysfs-specific data and file descriptors for efficient
 * GPIO operations. This structure is used internally by the driver.
 */
typedef struct
{
    uint8_t pin_num;       /**< GPIO pin number */
    int value_fd;          /**< File descriptor for value file */
    int direction_fd;      /**< File descriptor for direction file */
    bool exported;         /**< Whether pin is exported to sysfs */
    ArmGpioConfig* config; /**< Pointer to pin configuration */
} ArmPrivGpio;

/**
 * @brief GPIO initialization parameters.
 * 
 * Contains all parameters needed to initialize a GPIO pin, including
 * hardware-specific settings and configuration data.
 */
typedef struct
{
    uint32_t
        base_addr; /**< Pretty sure this isn't needed but I kept for consistentcy */
    uint8_t pin_num;    /**< GPIO pin number (0-27) */
    ArmGpioConfig conf; /**< Pin configuration settings */
    ArmPrivGpio priv;   /**< Private implementation data */
} ArmGpioParams;

/**
 * @brief Generic GPIO interface structure.
 * 
 * Provides a hardware-agnostic interface for GPIO operations using function
 * pointers. This allows the same application code to work with different
 * GPIO implementations (STM32, Linux, etc.).
 */
typedef struct Gpio
{
    void* priv; /**< Private implementation data pointer */
    void (*config)(struct Gpio* gpio); /**< Configuration function pointer */
    bool (*toggle)(struct Gpio* gpio); /**< Toggle function pointer */
    void (*set)(struct Gpio* gpio,
                bool active);        /**< Set value function pointer */
    bool (*read)(struct Gpio* gpio); /**< Read value function pointer */
} Gpio;

/**
 * @brief Initializes an ARM32 Linux GPIO pin.
 * 
 * Sets up the GPIO pin using Linux sysfs interface, exports the pin,
 * and assigns the appropriate function pointers to the GPIO interface.
 * 
 * @param gpio Pointer to GPIO interface structure to initialize.
 * @param params Pointer to ARM32-specific initialization parameters.
 * @return true if initialization successful, false on error.
 */
bool ArmGpioInit(Gpio* gpio, ArmGpioParams* params);

/**
 * @brief Configures the GPIO pin direction and settings.
 * 
 * Applies the configuration settings specified in the initialization
 * parameters, including setting pin direction and opening appropriate
 * file descriptors for efficient I/O operations.
 * 
 * @param gpio Pointer to initialized GPIO interface.
 */
void ArmGpioSetup(Gpio* gpio);

/**
 * @brief Toggles the GPIO pin output state.
 * 
 * Reads the current output state and inverts it. Only works for
 * pins configured as outputs.
 * 
 * @param gpio Pointer to configured GPIO interface.
 * @return New state after toggle (true = high, false = low).
 */
bool ArmGpioToggle(Gpio* gpio);

/**
 * @brief Sets the GPIO pin output value.
 * 
 * Drives the pin high or low. Only effective for pins configured
 * as outputs.
 * 
 * @param gpio Pointer to configured GPIO interface.
 * @param active true to drive pin high, false to drive pin low.
 */
void ArmGpioSet(Gpio* gpio, bool active);

/**
 * @brief Reads the current GPIO pin value.
 * 
 * Returns the current logical state of the pin. Works for both
 * input and output pins.
 * 
 * @param gpio Pointer to configured GPIO interface.
 * @return true if pin is high, false if pin is low.
 */
bool ArmGpioRead(Gpio* gpio);