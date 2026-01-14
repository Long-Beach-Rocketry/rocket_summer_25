#include "arm32_gpio.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Linux GPIO file paths
#define GPIO_BASE_PATH "/sys/class/gpio"
#define GPIO_EXPORT_PATH "/sys/class/gpio/export"
#define GPIO_UNEXPORT_PATH "/sys/class/gpio/unexport"

// Tell Linux we want to use this GPIO pin
static bool ArmGpioExport(uint8_t pin_num)
{
    char buffer[10];
    int fd = open(GPIO_EXPORT_PATH, O_WRONLY);  // Open export file
    if (fd < 0)
    {
        return false;  // Failed to open
    }

    snprintf(buffer, sizeof(buffer), "%d",
             pin_num);  // Convert pin number to string
    ssize_t bytes_written =
        write(fd, buffer, strlen(buffer));  // Write pin number to file
    close(fd);

    return bytes_written > 0;  // Success if we wrote something
}

// Build path and open GPIO files like /sys/class/gpio/gpio18/value
static int ArmGpioOpenFile(uint8_t pin_num, const char* file, int flags)
{
    char path[64];
    snprintf(path, sizeof(path), "%s/gpio%d/%s", GPIO_BASE_PATH, pin_num,
             file);            // Build full path
    return open(path, flags);  // Open the file
}

// Check if output pin is currently high or low
static bool ArmGpioOutStatus(Gpio* gpio)
{
    ArmPrivGpio* dev = (ArmPrivGpio*)gpio->priv;  // Get our private data
    char value;

    if (dev->value_fd < 0)
    {
        return false;  // File not open
    }

    lseek(dev->value_fd, 0, SEEK_SET);  // Go to start of file
    if (read(dev->value_fd, &value, 1) != 1)
    {  // Read one character
        return false;
    }

    return (value == '1');  // Return true if we read '1'
}

// Initialize GPIO - set up function pointers and export pin
bool ArmGpioInit(Gpio* gpio, ArmGpioParams* params)
{
    if (params->pin_num >= ARM_GPIO_MAX_PINS)
    {
        return false;  // Pin number too high
    }

    // Set up our private data
    params->priv.pin_num = params->pin_num;
    params->priv.value_fd = -1;  // No files open yet
    params->priv.direction_fd = -1;
    params->priv.exported = false;
    params->priv.config = &params->conf;

    // Tell gpio struct which functions to call
    gpio->priv = (void*)&params->priv;
    gpio->config = ArmGpioSetup;
    gpio->toggle = ArmGpioToggle;
    gpio->set = ArmGpioSet;
    gpio->read = ArmGpioRead;

    ArmGpioExport(params->pin_num);  // Tell Linux we want this pin

    usleep(100000);  // Wait for Linux to create files
    // Possible remove on this

    params->priv.exported = true;
    return true;
}

// Configure pin direction and open files for control
void ArmGpioSetup(Gpio* gpio)
{
    ArmPrivGpio* dev = (ArmPrivGpio*)gpio->priv;  // Get our data

    // Open direction file to set input/output
    dev->direction_fd = ArmGpioOpenFile(dev->pin_num, "direction", O_WRONLY);
    if (dev->direction_fd < 0)
    {
        return;  // Failed to open
    }

    // Write "out" or "in" to direction file
    const char* direction =
        (dev->config->mode == ARM_GPIO_OUTPUT) ? "out" : "in";
    write(dev->direction_fd, direction, strlen(direction));

    // Open value file to control the pin
    int value_flags =
        (dev->config->mode == ARM_GPIO_OUTPUT) ? O_RDWR : O_RDONLY;
    dev->value_fd = ArmGpioOpenFile(dev->pin_num, "value", value_flags);
}

// Flip pin state (on->off, off->on)
bool ArmGpioToggle(Gpio* gpio)
{
    bool state = ArmGpioOutStatus(gpio);  // Get current state

    ArmGpioSet(gpio, !state);  // Set opposite state

    return !state;  // Return new state
}

// Set pin high (true) or low (false)
void ArmGpioSet(Gpio* gpio, bool active)
{
    ArmPrivGpio* dev = (ArmPrivGpio*)gpio->priv;

    if (dev->value_fd < 0)
    {
        return;  // File not open
    }

    char value = active ? '1' : '0';    // Convert bool to character
    lseek(dev->value_fd, 0, SEEK_SET);  // Go to start of file
    write(dev->value_fd, &value, 1);    // Write '1' or '0'
}

// Read current pin state
bool ArmGpioRead(Gpio* gpio)
{
    ArmPrivGpio* dev = (ArmPrivGpio*)gpio->priv;
    char value;

    if (dev->value_fd < 0)
    {
        return false;  // File not open
    }

    lseek(dev->value_fd, 0, SEEK_SET);  // Go to start of file
    if (read(dev->value_fd, &value, 1) != 1)
    {  // Read one character
        return false;
    }

    return (value == '1');  // Return true if '1'
}