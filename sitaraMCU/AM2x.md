# AM263x Real-Time MCU Architecture & Peripheral Guide

---

## 1. Start With the Big Picture

The easiest way to understand AM263x is to divide the device into a few functional layers:

```text
                         AM263x
                           |
        +------------------+------------------+
        |                  |                  |
   System Foundation   Processing       Specialized Hardware
        |                  |                  |
   Power / Reset       R5FSS cores       CONTROLSS
   Clocking            HSM               PRU-ICSS
   Initialization      IPC               MCAN / LIN
   Interconnect        Interrupts        ADC / ePWM
   MPU / Firewalls     Memory            EDMA
                                           I2C / UART
                                           Timers
                                           Debug
```

At a system level:

- **Power, reset and clocks** establish the hardware operating environment.
- **Initialization and boot** bring the device from reset into application execution.
- **R5FSS** provides the main real-time processing capability.
- **CONTROLSS** provides tightly coupled sensing and actuator/control functions.
- **PRU-ICSS** provides programmable, deterministic real-time processing for industrial communication and I/O.
- **MCAN, LIN, I2C and UART** provide different communication mechanisms.
- **EDMA** moves data with reduced CPU involvement.
- **Interrupts** notify processors about asynchronous events.
- **IPC** allows processors/subsystems to exchange information.
- **MPU/firewalls and HSM** provide isolation and security.
- **On-chip debug** provides visibility and control during development.

---

# 2. System Bring-Up: What Happens Before the Application Runs?

A useful first question is:

> What has to happen before my R5F application can actually run?

Think of startup as a chain:

```text
Power becomes valid
        |
        v
Power-on reset / reset logic
        |
        v
Clock sources become available
        |
        v
Boot ROM / boot process
        |
        v
Security / boot decisions
        |
        v
System initialization
        |
        v
R5F application starts
```

The TRM treats **Power, Reset and Clocking** as part of Device Configuration and separately describes Initialization and Boot Process.

---

## 2.1 Power Management

### Why it exists

Digital logic, SRAM, analog circuitry, PLLs and I/O require appropriate supply domains.

AM263x therefore contains a Power Management Unit (PMU), power-control circuitry, monitors and internally generated supply resources.

### Important concept

Power management is not simply "turning power on."

The PMU also participates in:

- supply monitoring
- power-up sequencing
- reset generation
- reference generation
- temperature monitoring
- power-saving features

The TRM describes a **Reference System** and a **Safety System** within the PMU.

The Reference System handles reference/power sequencing and reset-related checks. The Safety System monitors supplies and temperature and reports safety-related problems through the Error Signaling Module.

### Power-up mental model

```text
External supplies
      |
      v
Supply validity
      |
      v
PORz held active
      |
      v
References / monitors
      |
      v
VDD / analog supplies ready
      |
      v
Clock startup
      |
      v
CPU reset release
```

### Why it matters

A processor must not start executing normally while its required supplies and references are unstable.

---

# 3. Reset

Reset answers:

> How do I put the device or a subsystem into a known state?

The TRM explicitly describes reset as a mechanism for bringing a device or subsystem to a predetermined/known state.

## Main idea

There are multiple reset sources and reset scopes.

Examples include:

- Power-On Reset
- Warm Reset
- software reset requests
- watchdog-related resets
- debug-related reset sources
- subsystem/module resets

### Power-On Reset

POR is the fundamental startup reset.

The TRM describes PORz as externally controlled and states that it keeps the device in reset until required external supplies are stable.

### Warm Reset

Warm reset does not necessarily reset every part of the device.

For example, the TRM notes that memories are generally not affected by warm reset, with specific exceptions such as MCAN message RAM and PRU DRAM.

### Mental model

```text
                    Reset Sources
                         |
       +-----------------+----------------+
       |                 |                |
      POR             Warm Reset       Module Reset
       |                 |                |
       +-----------------+----------------+
                         |
                         v
                  Reset Sequencer
                         |
                         v
                 Selected hardware
                         |
                         v
                  Known state
```

### Why it matters

Reset scope is important when debugging startup failures or recovering a subsystem without resetting the entire device.

---

# 4. Clocking

Clocking answers:

> What timing source makes this processor or peripheral operate?

AM263x has multiple clock sources and clock generators.

The TRM identifies:

- external crystal/reference clock
- internal oscillators
- CORE PLL
- PER PLL
- dividers
- IP-specific clock selection/distribution
- clock gating

### Basic flow

```text
Reference Clock
      |
      v
     PLL
      |
      v
   Divider
      |
      v
 Clock Mux / Distribution
      |
      +----> R5F
      +----> Peripheral
      +----> CONTROLSS
      +----> Other IP
```

A PLL allows a lower-frequency reference to be converted into the required operating frequency.

The resulting clocks are then selected/divided for individual IP blocks.

### Clock gating

Clock gating allows clocks to be disabled for blocks that do not need to operate.

This is primarily useful for power management.

### Important relationship

```text
Power
  |
Reset
  |
Clock
  |
Initialization
  |
Software execution
```

These are not independent topics. They form the hardware foundation on which the rest of the system operates.

---

# 5. Initialization and Boot

Initialization answers:

> How does the device move from reset into a usable software environment?

The TRM has a dedicated Initialization chapter covering:

- Initialization Overview
- ROM Code Overview
- Bootloader Modes
- Boot Terminology
- Boot Process

A simplified conceptual sequence is:

```text
Power / Reset
      |
      v
Boot ROM
      |
      v
Boot mode
      |
      v
Boot processing
      |
      v
System initialization
      |
      v
Application / secondary boot software
      |
      v
R5F execution
```

The exact boot sequence depends on the selected boot mode and device/security state.

### Security connection

On a security-enabled device, boot is not merely "load an image and run it."

The HSM/security subsystem participates in establishing the security state and boot behavior.

---

# 6. HSM — Hardware Security Module

The HSM should be understood as a **separate security domain**, not simply as a collection of cryptographic functions.

The TRM states that the HSM is responsible for:

- booting the device
- enabling the main R5FSS core
- defining/controlling overall security according to boot options
- providing the DTHE crypto/hash-related functionality

The HSM is based around a dedicated Cortex-M4 security controller and isolated/secured RAM.

## Security functions

The TRM describes support for concepts such as:

- Hardware Root of Trust
- authenticated boot
- encrypted boot
- anti-rollback
- secure debug
- device identification
- key management
- AES
- SHA
- random-number generation
- public-key acceleration
- firewall support

### Conceptual architecture

```text
                  HSM
                   |
       +-----------+-----------+
       |           |           |
    Secure       Crypto     Security
     Boot       Engines     Control
       |           |           |
       +-----------+-----------+
                   |
                   v
             Device Security
```

### Why this is important

Security is established below the normal application layer.

The application does not need to implement the complete security root-of-trust mechanism itself.

---

# 7. System MPU / Firewalls

This is another area where it is important to understand the architecture rather than only the terminology.

The **system MPU/firewalls are not the same as the MPU inside the R5FSS**.

The TRM describes multiple system MPUs placed in the interconnect.

Their purpose can be:

- security
- freedom from interference (FFI) in safety applications

## What is actually checked?

A bus access reaches an MPU.

The MPU evaluates information associated with the initiator and the requested access.

The TRM describes:

- programmable address ranges
- secure/debug privileges
- read/write/execute permissions
- initiator identification through Privilege ID
- privilege levels
- protection/addressing violation reporting

### Basic operation

```text
Initiator
   |
   | Bus Access
   v
MPU / Firewall
   |
   +---- Permission OK ----> Target
   |
   +---- Permission Fail --> Fault / Error
```

### Why multiple firewalls?

They are placed at different locations in the interconnect.

The TRM distinguishes:

- **Initiator-side firewalls** — positioned near initiators and used to control access from them.
- **Target-side firewalls** — positioned near targets and used to protect target resources.

### Result

A processor/DMA/other initiator cannot automatically access every system resource merely because the address exists.

---

# 8. R5FSS as the Main Processing Element

The requested peripherals make more sense once the R5FSS is viewed as the main software execution engine.

The R5F executes:

- boot/application software
- real-time control algorithms
- communication software
- diagnostic logic
- peripheral drivers
- safety/security-related application logic

But the R5F does not need to perform every hardware operation in software.

Dedicated hardware performs:

```text
ADC conversion
PWM generation
DMA transfers
CAN protocol handling
LIN communication
UART serialization
I2C transactions
timer counting
interrupt routing
industrial real-time processing
```

This division is one of the central ideas behind a real-time MCU.

---

# 9. CONTROLSS — Real-Time Control Subsystem

CONTROLSS is best understood as a **hardware-assisted closed-loop control environment**.

The TRM describes four major parts of a real-time control system:

```text
1. Sensing
2. Processing
3. Control
4. Interface
```

### Closed-loop view

```text
       Physical System
              |
              v
          Measurement
              |
              v
             ADC
              |
              v
       Control Algorithm
              |
              v
            ePWM
              |
              v
           Actuator
              |
              +----------> Physical System
```

The loop repeats continuously.

### Why CONTROLSS exists

Timing matters in a control loop.

You often want:

- measurement at a precise point in time
- deterministic processing
- precisely timed actuator output
- hardware connections between peripherals
- minimal software latency

CONTROLSS integrates these functions closely.

---

# 10. ADC

The AM263x ADC is a **successive-approximation (SAR) ADC** with an analog core and digital wrapper.

The analog portion contains elements such as:

- input channel selection
- sample-and-hold
- successive-approximation circuitry
- voltage references

The digital wrapper controls conversions and handles results/interfaces.

## Conversion concept

```text
Analog Signal
      |
      v
Input Selection
      |
      v
Sample & Hold
      |
      v
SAR Conversion
      |
      v
Digital Result
```

### SOC concept

The AM263x ADC uses a **Start-of-Conversion (SOC)** based architecture.

A SOC specifies a conversion operation.

The ADC can receive conversion triggers from multiple sources. The TRM specifically describes ePWM and eCAP-related trigger sources.

This is important for real-time control because an ePWM event can trigger an ADC conversion at a controlled point in the PWM cycle.

### Example

```text
ePWM event
    |
    v
ADC SOC trigger
    |
    v
ADC conversion
    |
    v
Result
    |
    v
Control algorithm
```

This creates a deterministic relationship between actuator timing and measurement timing.

---

# 11. ePWM

ePWM is the control-side waveform generation mechanism.

Instead of asking the CPU to toggle a GPIO repeatedly:

```text
CPU -> GPIO -> CPU -> GPIO -> CPU ...
```

the PWM hardware generates the waveform from configured timing information.

Conceptually:

```text
Clock
  |
  v
Time Base
  |
  v
Compare / Action Logic
  |
  v
PWM Output
```

The duty cycle and timing are determined by the PWM configuration.

## CONTROLSS relationship

ePWM is not isolated.

It can interact with:

- ADC triggering
- synchronization
- trip/protection paths
- crossbar signal routing

This allows a control loop such as:

```text
ePWM
  |
  | trigger
  v
ADC
  |
  | measurement
  v
R5F
  |
  | new control value
  v
ePWM
```

---

# 12. XBAR / Signal Interconnection

CONTROLSS contains flexible signal interconnection.

The TRM shows ePWM, ADC, comparator, SDFM, FSI and other signals participating in crossbar/interconnection structures.

The important idea is:

> Hardware signals do not always have to be permanently connected to only one destination.

A crossbar can route selected internal events/signals to other control peripherals.

This is useful when building deterministic hardware event chains.

---

# 13. Timers

Timers provide hardware-based time measurement and event generation.

They are useful for:

- periodic processing
- timeouts
- scheduling
- measuring elapsed time
- watchdog-related functions
- generating interrupts
- timestamp/event handling

Conceptually:

```text
Clock
  |
  v
Counter
  |
  +---- Compare / Event
  |
  v
Interrupt / Action
```

The key difference from a software delay is that a hardware timer continues counting independently of normal instruction execution.

For a real-time application, timers are therefore commonly used as the basis for periodic activities rather than busy-wait loops.

---

# 14. Interrupt Architecture

An interrupt is a hardware event that requests processor attention.

The important architecture is:

```text
Peripheral
    |
    v
Event / Interrupt Source
    |
    v
Interrupt Router / Aggregation
    |
    v
R5F Interrupt Controller / VIM
    |
    v
ISR
```

The TRM's interrupt chapter contains interrupt architecture, interrupt controllers and the R5F VIM.

## Why interrupts are used

Without interrupts:

```text
CPU
 |
 +-- check ADC?
 |
 +-- check CAN?
 |
 +-- check UART?
 |
 +-- check timer?
 |
 +-- check DMA?
 |
 +-- repeat...
```

With interrupts:

```text
CPU executes useful work
          |
          v
       Event
          |
          v
      Interrupt
          |
          v
         ISR
          |
          v
      Handle event
```

This is especially important for real-time response.

---

# 15. EDMA — Data Movement Without Continuous CPU Copying

EDMA is the hardware data-movement engine.

The TRM describes an EDMA architecture containing:

- TPCC — transfer/channel/event control
- TPTC — transfer controller
- event sources
- transfer parameters
- transfer completion/error signaling

## Basic idea

Without DMA:

```text
Peripheral -> CPU -> Memory
```

The CPU repeatedly reads/writes data.

With EDMA:

```text
Peripheral ---------> EDMA ---------> Memory
```

The CPU mainly configures the transfer and responds to completion/error events.

## Why this matters

EDMA can perform structured transfers including multi-dimensional transfers, linking and chaining.

A common real-time example:

```text
ADC
 |
 | data event
 v
EDMA
 |
 v
RAM buffer
 |
 v
R5F processes block
```

This allows data acquisition and CPU processing to proceed with less CPU overhead.

---

# 16. IPC — Interprocessor Communication

When more than one processor/core is present, they need a controlled way to exchange information.

AM263x provides IPC mechanisms including **Mailbox** and **Spinlock**.

## Mailbox

A mailbox provides a hardware-supported mechanism for exchanging messages/events between processors.

Conceptually:

```text
R5F Core 0
    |
    | message/event
    v
 Mailbox
    |
    v
R5F Core 1
```

The receiver can then process the message.

Typical uses include:

- command passing
- status notification
- coordination
- data/event signaling
- boot/startup coordination

## Spinlock

A spinlock is used for synchronization when multiple processors need controlled access to a shared resource.

The idea is:

```text
Core 0 ----\
            > Shared Resource
Core 1 ----/
```

The synchronization mechanism prevents both processors from entering a protected critical section simultaneously.

---

# 17. Memory Controllers

Memory is not simply "RAM connected to the CPU."

The SoC contains different memory resources and system interconnect paths.

A memory controller/interface provides the required hardware path and control for accessing the associated memory.

Think:

```text
R5F / EDMA
     |
     v
Interconnect
     |
     v
Memory Controller / Interface
     |
     v
Memory
```

This becomes important when considering:

- CPU access
- DMA access
- memory bandwidth
- protection
- ECC/error handling where supported
- initialization
- shared-memory communication

---

# 18. MCAN

MCAN provides hardware support for CAN communication.

The key idea is that the CAN controller performs protocol-related work so application software does not need to implement CAN bit-level operation itself.

## Transmit

```text
Application
    |
    v
CAN message
    |
    v
MCAN Tx resources
    |
    v
CAN controller
    |
    v
CAN bus
```

## Receive

```text
CAN bus
   |
   v
MCAN
   |
   v
Acceptance/filtering
   |
   v
Rx resources
   |
   v
Application
```

The AM263x family supports CAN/CAN FD.

### What the controller handles

At a high level, hardware handles the CAN protocol operation while software handles the message/application meaning.

This separation is important:

```text
MCAN = CAN protocol/interface hardware
Application = what the CAN message means
```

---

# 19. LIN

LIN is intended for simpler, lower-cost serial networking.

The important architectural concept is its master-controlled communication model.

```text
          Master
            |
       LIN Header
            |
    +-------+-------+
    |       |       |
 Slave A  Slave B  Slave C
```

The master schedules communication.

A typical exchange consists of:

```text
Header
  |
Identifier
  |
Response
  |
Data
```

LIN is therefore conceptually different from CAN:

| CAN | LIN |
|---|---|
| Multi-node CAN bus | Master-controlled LIN network |
| Higher performance | Lower-cost/simple communication |
| CAN/CAN FD controller | LIN controller |
| Arbitration-based bus operation | Master schedules frames |

---

# 20. I2C

I2C is a shared synchronous serial bus.

The fundamental signals are:

```text
SCL = Clock
SDA = Data
```

Multiple devices can share the same bus.

```text
                 +---- Sensor
                 |
Controller ------+---- EEPROM
                 |
                 +---- Other peripheral
```

A transaction conceptually contains:

```text
START
  |
Address
  |
Read/Write
  |
Data
  |
ACK
  |
STOP
```

The controller initiates communication and addresses the intended target.

### Typical embedded use

I2C is commonly appropriate for configuration/status communication with relatively simple external devices.

---

# 21. UART

UART is an asynchronous serial interface.

There is no separate clock line between the communicating devices.

Both sides agree on the communication timing, typically through a baud-rate configuration.

## Frame concept

```text
Idle
 |
Start
 |
Data bits
 |
Parity (optional)
 |
Stop
 |
Idle
```

The UART converts between:

```text
Parallel software data
        |
        v
Serial bit stream
```

and the reverse operation during reception.

### Common role

UART is particularly useful for:

- bootloader communication
- console
- debugging
- simple device communication

---

# 22. PRU-ICSS

PRU-ICSS should be understood separately from the normal R5F software path.

It is a **programmable real-time subsystem** intended for deterministic processing and industrial communication.

TI's current Industrial Communications SDK describes PRU-ICSS as the typical subsystem used for real-time industrial communications.

## Why it exists

Some communication/control tasks need:

- predictable timing
- direct/close access to I/O
- protocol-specific processing
- operation independent of the main CPU workload

PRU-ICSS provides programmable real-time processing resources for these requirements.

Conceptually:

```text
                 R5F
                  |
          Application / System
                  |
        +---------+---------+
        |                   |
      Normal              PRU-ICSS
    processing          Real-time I/O
                            |
                            v
                  Industrial Interface
```

### Important mental model

Do not think of PRU-ICSS as simply "another CPU."

Think:

> **A programmable real-time I/O and communication engine designed to handle deterministic tasks alongside the main processor.**

---

# 23. On-Chip Debug

Debug hardware provides a controlled path into the processor/system.

Conceptually:

```text
External Debugger
        |
        v
 Debug Access Path
        |
        v
 Processor / Debug Resources
```

It allows developers to perform operations such as:

- halt/resume
- single-step
- inspect registers
- inspect memory
- breakpoints
- watchpoints
- trace/debug-related operations where supported

### Security connection

Debug is not automatically unrestricted.

The HSM/security architecture can control debug access on secure devices.

So:

```text
Debug hardware
      +
Security policy
      |
      v
Allowed / restricted debug access
```

---

# 24. How the Major Blocks Work Together

A real embedded application usually uses several of these blocks at the same time.

## Example: Motor Control

```text
                  Motor
                    ^
                    |
                 ePWM
                    ^
                    |
              Control Algorithm
                    ^
                    |
                   ADC
                    ^
                    |
                 Sensor
```

Supporting the control loop:

```text
Timer
  |
  +---- periodic timing

Interrupts
  |
  +---- notify R5F of events

EDMA
  |
  +---- move sampled data

Clock
  |
  +---- provide deterministic timing

Power / Reset
  |
  +---- establish valid operating state

MPU / Firewall
  |
  +---- control access

HSM
  |
  +---- security

IPC
  |
  +---- communicate with another core
```

---

# 25. Example: Automotive/Industrial ECU Communication

A communication-oriented application may look like:

```text
                 R5F Application
                       |
          +------------+------------+
          |            |            |
         MCAN         LIN          UART
          |            |            |
        CAN Bus      LIN Bus      Console
```

I2C may be used for board-level peripheral/configuration communication:

```text
R5F
 |
I2C
 |
+---- Sensor
+---- EEPROM
+---- PMIC-related device
```

EDMA can reduce CPU involvement when large/repeated data movement is required.

Interrupts notify the R5F when communication events occur.

---

# 26. Example: Real-Time Industrial Communication

```text
                    R5F
                     |
             System/Application
                     |
              +------+------+
              |             |
          PRU-ICSS       Other IP
              |
              v
       Deterministic
       I/O / Protocol
              |
              v
      Industrial Network
```

The R5F can remain responsible for system/application-level functions while PRU-ICSS handles timing-sensitive communication/I/O processing.

---

# 27. Security + Safety + Real-Time Behavior

These topics should not be studied independently.

A real-time MCU has to solve several problems simultaneously:

```text
                 Real-Time System
                       |
       +---------------+---------------+
       |               |               |
    Timing           Security        Safety
       |               |               |
  Clock/Timer      HSM/Firewall      MPU
  ADC/ePWM         Secure Boot       Error handling
  Interrupts       Secure Debug      Isolation
  EDMA
```

### Timing

The system must respond at the required time.

### Security

Only authorized software/resources should perform protected operations.

### Safety

Faults and interference should be detected/contained as required by the application.

The AM263x architecture provides hardware mechanisms across all three areas.

---

# 28. The Most Important Relationships to Remember

## Clock → Peripheral

A peripheral needs an appropriate clock before it can operate.

```text
Clock source
   |
PLL / divider
   |
Peripheral clock
   |
Peripheral operates
```

## Reset → Initialization

Reset places hardware in a known state.

```text
Reset
  |
Known hardware state
  |
Initialization
  |
Application
```

## Interrupt → ISR

Hardware detects an event and notifies the processor.

```text
Peripheral
  |
Event
  |
Interrupt
  |
VIM / interrupt path
  |
ISR
```

## EDMA → Data movement

Hardware transfers data without requiring the CPU to copy every item.

```text
Peripheral
   |
 EDMA
   |
Memory
```

## ADC → Control

ADC turns a physical measurement into digital information.

```text
Sensor
  |
ADC
  |
Control algorithm
```

## ePWM → Actuator

ePWM converts the control command into a precisely timed waveform.

```text
Control algorithm
       |
      ePWM
       |
    Actuator
```

## IPC → Multi-core coordination

```text
Core 0
  |
Mailbox / synchronization
  |
Core 1
```

## MPU → Access control

```text
Initiator
   |
MPU / Firewall
   |
Allowed -> Target
Denied  -> Fault
```

## HSM → Root of trust/security

```text
Boot
 |
HSM / Security
 |
Authentication / security state
 |
Trusted execution
```

---