
# [Micro-Controllers]
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


## Quick Comparison

| Feature                  | MCU                         | MPU                               | DSP                              | FPGA                          | ASIC                                    |
| ------------------------ | --------------------------- | --------------------------------- | -------------------------------- | ----------------------------- | --------------------------------------- |
| Full form                | Microcontroller Unit        | Microprocessor Unit               | Digital Signal Processor         | Field-Programmable Gate Array | Application-Specific Integrated Circuit |
| Basic concept            | Small computer for control  | Application processor             | Signal-processing processor      | Programmable hardware         | Custom fixed hardware                   |
| Main purpose             | Embedded control            | Complex applications / OS         | Signal & mathematical processing | Parallel/custom processing    | Application-specific optimization       |
| Executes software        | Yes                         | Yes                               | Yes                              | Not traditionally             | No                                      |
| Programmable hardware    | No                          | No                                | No                               | **Yes**                       | Design-time only                        |
| Reprogrammable           | Yes, software               | Yes, software                     | Yes, software                    | **Yes**                       | **No**                                  |
| CPU                      | Yes                         | Yes                               | Yes                              | Optional                      | Optional                                |
| Typical architecture     | Cortex-M, RISC-V MCU        | Cortex-A, RISC-V, x86             | DSP cores                        | FPGA fabric + optional CPU    | Custom                                  |
| RAM                      | Usually on-chip SRAM        | Usually external DDR/LPDDR        | Internal/external                | BRAM + external memory        | Application-specific                    |
| Flash/Storage            | Usually integrated Flash    | Usually external eMMC/NAND/NOR/SD | Depends                          | External/internal             | Application-specific                    |
| MMU                      | Usually no                  | **Usually yes**                   | Depends                          | Depends on CPU                | Depends on design                       |
| Cache                    | Small/optional              | **Large cache common**            | Common                           | Optional                      | Designed as needed                      |
| Linux                    | Usually no                  | **Yes**                           | Some devices                     | Possible with CPU/SoC         | Possible if designed                    |
| RTOS                     | **Very common**             | Possible                          | **Very common**                  | Possible                      | Depends                                 |
| Parallelism              | Limited                     | Moderate                          | Moderate–high                    | **Excellent**                 | **Excellent**                           |
| Real-time                | **Excellent**               | Lower with Linux                  | **Excellent**                    | **Excellent**                 | **Excellent**                           |
| Signal processing        | Basic–moderate              | Good                              | **Excellent**                    | **Excellent**                 | **Excellent**                           |
| Performance              | Low–medium                  | **High**                          | High for DSP workloads           | **Very high**                 | **Very high**                           |
| Power                    | **Very low**                | Higher                            | Low–medium                       | Medium                        | **Can be extremely low**                |
| Flexibility              | High                        | High                              | High                             | **Very high**                 | **Very low**                            |
| Development complexity   | Low                         | Medium–high                       | Medium                           | **High**                      | **Very high**                           |
| HDL required             | No                          | No                                | No                               | **Yes**                       | **Yes**                                 |
| Hardware design required | No                          | Usually no                        | No                               | **Yes**                       | **Yes**                                 |
| Development cost         | Low                         | Medium                            | Medium                           | High                          | **Very high**                           |
| NRE                      | Very low                    | Low–medium                        | Medium                           | Medium–high                   | **Extremely high**                      |
| Unit cost                | Low                         | Medium                            | Medium                           | Medium–high                   | **Low at high volume**                  |
| Time to market           | **Fast**                    | Fast–medium                       | Medium                           | Medium                        | **Slow**                                |
| Typical applications     | Sensors, motors, appliances | Linux gateways, HMI, routers      | Audio, radar, telecom            | SDR, video, networking        | CPUs, GPUs, modems, AI chips            |

---

