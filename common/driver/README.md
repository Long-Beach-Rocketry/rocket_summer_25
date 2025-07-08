# Drivers

## Introduction
This directory contains abstract device driver interfaces for on-chip peripherals. These are intended to act as a common interface that each hardware target (i.e. microcontrollers, microprocessors) can encapsulate their own specific driver within, but plug into any higher level piece of software that accepts and uses the defined interface.

## Subdirectories
**[bus](./bus/)**: Contains interfaces for serial communication protocols, such as SPI and UART.  
**[io](./io/)**: Contains interfaces for I/O style hardware, such as GPIO, PWM, and Encoders.  
**[memory](./memory/)**: Contains interfaces for embedded memory access, like flash.  
**[platform](./platform/)**: Contains sets of factory functions and drivers for each hardware target, abiding by the above interfaces. For each unique hardware target selected during the build process (other than native as of now), there should be a corresponding directory in this folder that implements all of the available hardware on the chip.  

## The Driver Model
The first step to write a good driver is to have a well defined model. This includes deciding what level of access is necessary through the API, and what common functionalities might be necessary across different implementations of the same driver. Looking at a UART driver for example, it's a basic stream-based peripheral that can be generalized to two functions: `send(data, num_bytes)` and `recv(data, num_bytes)`.  There might be details like baudrate, parity, or other configurations you'd like to implement from device to device, but seeing that those are configurations rather than runtime actions, we can leave those out. In most cases, it's easiest to be able to cleanly split configurations (whether it's device specific or not) from the runtime functionality (which must be the same for it to be considered the same driver). 

Next, let's look at an example in C++:

```Cpp
struct Usart
{
    virtual bool send(span<uint8_t> data);
    virtual bool recv(span<uint8_t data>);
};
```

In C++, polymorphism is easy. Virtual base classes provide compile time checks and handle callbacks through a virtual function table at runtime. There's a bit of overhead, but it's often worth it for the enforced structure it provides.

In C, there are unfortunately no objects. Structs can contain values and functions, but inheritance does not exist and we cannot leverage it to encapsulate data for varying implementations and multiple instances of each. So, we have to do some extra work to create this abstraction. In the below example, we define an equivalent interface to the above C++ code in C. It uses a `void*` to allow concrete implementations to provide a type-specific struct that holds data relevant to its operations.

```C
// usart.h

typedef struct Usart Usart;

struct Usart
{
    bool (*send)(Usart* usart, const uint8_t* data, size_t size);
    bool (*recv)(Usart* usart, uint8_t* data, size_t size);

    void* priv;
};
```

In order to access the private data in each concrete implementation without polluting the interface with details of any one implementation, we can do the following:

```C
//my_usart.h/.c
typedef struct
{
    uint32_t important_val;
    uint32_t index;
} MyUart;

void MyUsartInit(Usart* usart, MyUart* my_uart, uint32_t index)
{
    my_uart->important_val = 42;
    my_uart->index = index;

    usart->priv = (void*)my_uart;
    usart->send = MyUartSend;
    usart->recv = MyUartRecv;
}

bool MyUsartSend(Usart* usart, const uint8_t* data, size_t size)
{
    MyUart* dev = (MyUart*)usart->priv;

    // Send data
    uint32_t some_action = dev->important_val + index;
    
    // Do other stuff...
    
    return true;
}
```

What we have now is the ability to call`MyUsartInit` in some device specific initialization code, but then pass the initialized `Usart` struct interface as a pointer to some common code that can accept any implementation of the same driver. This way, our runtimes can be unit tested separately and have a high probability of behaving similarly even as we swap the low level drivers beneath them. To see the actual driver this example is based on, see [here](./platform/stm32l4/st_usart.h).

You may notice that this makes initialization significantly less pleasant than it is in C++, which is correct. There are a couple ways to try to tackle this (so far none of them are perfect), but one is factory functions. You can see an example of this [here](./platform/stm32l4/st_usart_fctry.c).

To see examples of actual applications of this driver model, see:
- [Initialization in a Board Support Package](../../app/cli_app/BSP_L476/cli_app_bsp.c)
- [Use of common interfaces in Application Code](../app/freertos/cli/debug_app.c)
- [A 'decorator' style use of interface in a peripheral IC driver](../core/periph/rs485/snx5176b.c)
