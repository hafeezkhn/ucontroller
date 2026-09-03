
  **[⬅️Prev](stm32/STM32x.md)** [Micro-Controllers] **[Next➡️](sitaraMCU/AM2x.md)** 
  
# MCU — Microcontroller Unit

## Basic Idea

> **MCU = Control**

An MCU combines the processor, memory and peripherals into a single chip.

```text
             MCU
    +----------------------+
    |       CPU            |
    |                      |
    |      SRAM            |
    |      Flash           |
    |                      |
    | GPIO  UART  SPI      |
    | I2C   CAN   ADC      |
    | PWM   Timer          |
    +----------------------+
```

## Typical Software

```text
Power ON
   |
Startup Code
   |
main()
   |
Application / RTOS
```

## Common Applications

* Sensor control
* Motor control
* GPIO
* ADC
* PWM
* UART
* SPI
* I2C
* CAN
* Battery-powered systems
* Appliances
* Industrial controllers

## Examples

* STM32
* NXP Kinetis
* Microchip PIC
* AVR
* TI MSPM0

> An MCU is a low-power embedded controller that integrates CPU, memory and peripherals. It is mainly used for control-oriented applications where low cost, low power and deterministic real-time behavior are important.


# Quick Comparison Guide

| Feature | MCU | MPU | DSP | FPGA | ASIC | SoC | CPLD |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **What it is** | A compact integrated circuit designed for specific control applications. | The core computing engine that executes instructions. | A processor optimized for high-speed numeric processing. | Reconfigurable hardware that can implement any digital logic. | A chip custom-designed for a specific task. | A single chip integrating a processor, memory, I/O, and sometimes GPU, DSP, etc. | A programmable logic device, simpler than an FPGA. |
| **Includes** | Processor (CPU) + memory + I/O ports + peripherals (like timers, ADCs). | Only the processing unit (sometimes with cache). | Special hardware for fast multiply-accumulate, circular buffers, etc. | An array of programmable logic blocks and routing. | Whatever components are needed for its dedicated function. | CPU + GPU + memory + other peripherals. | Logic blocks interconnected with a predictable structure. |
| **Full form** | Microcontroller Unit | Microprocessor Unit | Digital Signal Processor | Field-Programmable Gate Array | Application-Specific Integrated Circuit | System on Chip | Complex Programmable Logic Device |
| **Basic concept** | Small computer for control | Application processor | Signal-processing processor | Programmable hardware | Custom fixed hardware | Integrated system on one die | Simple programmable hardware |
| **Main purpose** | Embedded control | Complex applications / OS | Signal & mathematical processing | Parallel/custom processing | Application-specific optimization | Complete system operation | Simple logic, glue logic |
| **Executes software** | Yes | Yes | Yes | Not traditionally | No | Yes | No |
| **Programmable hardware**| No | No | No | **Yes** | Design-time only | No | **Yes** |
| **Reprogrammable** | Yes, software | Yes, software | Yes, software | **Yes** | **No** | Yes, software | **Yes** |
| **CPU** | Yes | Yes | Yes | Optional | Optional | Yes | No |
| **Typical architecture** | Cortex-M, RISC-V MCU | Cortex-A, RISC-V, x86 | DSP cores | FPGA fabric + optional CPU | Custom | ARM Cortex-A, Apple Silicon | Macrocells |
| **RAM** | Usually on-chip SRAM | Usually external DDR/LPDDR | Internal/external | BRAM + external memory | Application-specific | Unified or dedicated | Limited/None |
| **Flash/Storage** | Usually integrated Flash | Usually external eMMC/NAND/NOR/SD | Depends | External/internal | Application-specific | Integrated or external | Integrated |
| **MMU** | Usually no | **Usually yes** | Depends | Depends on CPU | Depends on design | **Yes** | No |
| **Cache** | Small/optional | **Large cache common** | Common | Optional | Designed as needed | **Large** | No |
| **Linux** | Usually no | **Yes** | Some devices | Possible with CPU/SoC | Possible if designed | **Yes** | No |
| **RTOS** | **Very common** | Possible | **Very common** | Possible | Depends | Possible | No |
| **Parallelism** | Limited | Moderate | Moderate–high | **Excellent** | **Excellent** | Moderate-High | Moderate |
| **Real-time** | **Excellent** | Lower with Linux | **Excellent** | **Excellent** | **Excellent** | Varies | **Excellent** |
| **Signal processing** | Basic–moderate | Good | **Excellent** | **Excellent** | **Excellent** | Good | Basic |
| **Performance** | Low–medium | **High** | High for DSP workloads | **Very high** | **Very high** | **Very High** | Low-Medium |
| **Power** | **Very low** | Higher | Low–medium | Medium | **Can be extremely low** | Varies | Low |
| **Flexibility** | High | High | High | **Very high** | **Very low** | High | Medium |
| **Development complexity**| Low | Medium–high | Medium | **High** | **Very high** | High | Low |
| **HDL required** | No | No | No | **Yes** | **Yes** | No | **Yes** |
| **Hardware design required**| No | Usually no | No | **Yes** | **Yes** | No | **Yes** |
| **Development cost** | Low | Medium | Medium | High | **Very high** | High | Low |
| **NRE** | Very low | Low–medium | Medium | Medium–high | **Extremely high** | High | Very low |
| **Unit cost** | Low | Medium | Medium | Medium–high | **Low at high volume** | Medium-High | Low |
| **Time to market** | **Fast** | Fast–medium | Medium | Medium | **Slow** | Fast-Medium | **Fast** |
| **Typical applications** | Sensors, motors, appliances | Linux gateways, HMI, routers | Audio, radar, telecom | SDR, video, networking | CPUs, GPUs, modems, AI chips | Smartphones, tablets, IoT | Glue logic, power sequencing |
| **Example** | Atmel ATmega328 (used in Arduino). | Intel Core i7, ARM Cortex-A72. | TI TMS320, Analog Devices SHARC. | Xilinx Spartan, Intel Cyclone. | Apple's A-series chips, Bitcoin mining chips. | Qualcomm Snapdragon, Apple M1. | Xilinx XC9500, Altera MAX series. |


> Microcontrollers <br>
● MCU architecture
● Cortex-M
● Registers
● Stack pointer
● Vector table
● Reset handler
● Startup code
● Memory layout
● Flash/RAM
● Interrupt handling
● GPIO
● Timers
● UART
● SPI
● I2C
● CAN
● ADC/DAC

> Bare-Metal Programming <br>
● Startup code
● Linker scripts
● Vector tables
● Register-level programming
● Peripheral initialization
● Interrupt handlers
● DMA
● Timers
● Watchdog
● Boot flow
● Firmware update mechanisms

> RTOS <br>
● FreeRTOS
● Real-time scheduling
● Tasks/threads
● Mutexes
● Semaphores
● Message queues
● Event flags
● Interrupts
● Timers
● Priority inversion
● Priority inheritance
● Deterministic execution
● ISR vs task context
● Memory management
● RTOS architecture

