
What is SPI and how does it work?
⭐ Answer

SPI, or Serial Peripheral Interface, is a synchronous serial communication protocol used for communication between a master/controller and one or more slave/peripheral devices.

SPI normally uses four signals:

SCK  → Serial Clock
MOSI → Master Out Slave In
MISO → Master In Slave Out
CS   → Chip Select

The master generates the clock and selects the slave using CS. Data is shifted between the master and slave on MOSI and MISO.

SPI is normally full-duplex, meaning the master can transmit and receive data simultaneously during the same clock cycles.

---

Yes. We’ll do **Q1 → Q15 in order**, with the same interview-preparation style you used for the battery-parser question:

* ⭐ **Answer**
* 🔍 **Step-by-step explanation**
* 💻 **C code**
* 🎯 **Interviewer follow-up questions**
* 🧠 **Key points to remember**

I’ll keep the C code **generic/MCU-oriented**, because actual SPI register names differ between microcontrollers.

---

# Q1. What is SPI and how does it work?

## ⭐ Answer

**SPI, or Serial Peripheral Interface, is a synchronous serial communication protocol used for communication between a master/controller and one or more slave/peripheral devices.**

SPI normally uses four signals:

```text
SCK  → Serial Clock
MOSI → Master Out Slave In
MISO → Master In Slave Out
CS   → Chip Select
```

The master generates the clock and selects the slave using CS. Data is shifted between the master and slave on MOSI and MISO.

SPI is normally **full-duplex**, meaning the master can transmit and receive data simultaneously during the same clock cycles.

---

## 🔍 Step-by-step

Suppose the master wants to send `0xA5` to a sensor.

### Step 1 — Select slave

The master pulls CS LOW:

```text
CS = LOW
```

The slave becomes active.

### Step 2 — Generate clock

The master generates SCK pulses.

```text
SCK:  _|‾|_|‾|_|‾|_|‾|_
```

### Step 3 — Send data

The master shifts bits onto MOSI.

`0xA5`:

```text
1010 0101
```

### Step 4 — Slave receives

The selected slave samples MOSI according to the configured SPI mode.

### Step 5 — Slave can simultaneously transmit

The slave can put response data on MISO.

### Step 6 — Finish

The master releases CS:

```text
CS = HIGH
```

---

## 💻 C code

A generic SPI byte transfer looks like this:

```c
#include <stdint.h>

uint8_t SPI_Transfer(uint8_t tx_data)
{
    uint8_t rx_data;

    /*
     * Write transmit data to SPI hardware.
     */
    SPI_TX_REGISTER = tx_data;

    /*
     * Wait until transfer is complete.
     */
    while ((SPI_STATUS_REGISTER & SPI_TRANSFER_COMPLETE) == 0U)
    {
        /* Wait */
    }

    /*
     * Read received data.
     */
    rx_data = SPI_RX_REGISTER;

    return rx_data;
}
```

Conceptually:

```text
              MASTER
                |
          +-----+-----+
          |           |
        MOSI         MISO
          |           |
          v           ^
        SLAVE <-------+
          |
         SCK
          |
       generated
       by master
```

Because SPI is full duplex:

```c
rx_data = SPI_Transfer(tx_data);
```

means:

```text
tx_data → slave
rx_data ← slave
```

during the same transfer.

---

## 🎯 Interviewer follow-up questions

**Q:** Is SPI synchronous or asynchronous?

**A:** Synchronous. The master provides the clock.

**Q:** Is SPI full duplex?

**A:** Normally yes. MOSI and MISO operate simultaneously.

**Q:** Who generates SCK?

**A:** The master/controller.

**Q:** Does SPI define a standard maximum speed?

**A:** No universal speed applies to every SPI device. The practical limit depends on the controller, peripheral, board, signal integrity, and slave datasheet.

**Q:** Does SPI have addressing like I2C?

**A:** Normally no. Slave selection is usually done using chip-select signals.

---

## 🧠 Remember

```text
SPI = Synchronous + Serial + Usually Full Duplex

Master:
    generates SCK
    controls CS
    sends MOSI
    receives MISO
```

---

# Q2. Explain SCK, MOSI, MISO and CS.

## ⭐ Answer

SPI normally has four main signals:

| Signal | Meaning             | Direction      |
| ------ | ------------------- | -------------- |
| SCK    | Serial Clock        | Master → Slave |
| MOSI   | Master Out Slave In | Master → Slave |
| MISO   | Master In Slave Out | Slave → Master |
| CS/SS  | Chip Select         | Master → Slave |

---

## 🔍 Step-by-step

### 1. SCK

SCK is the serial clock.

The master generates it.

```text
Master
   |
   | SCK
   v
Slave
```

The clock determines when bits are shifted and sampled.

---

### 2. MOSI

MOSI means:

```text
Master Out
Slave In
```

So:

```text
Master ───── MOSI ─────> Slave
```

Example:

```text
Master sends:

10100101
```

---

### 3. MISO

MISO means:

```text
Master In
Slave Out
```

So:

```text
Master <──── MISO ───── Slave
```

The slave sends response data through MISO.

---

### 4. CS

CS selects the slave.

Usually:

```text
CS = LOW → selected
CS = HIGH → deselected
```

For example:

```text
Master
 |
 +---- CS0 ----> Sensor
 |
 +---- CS1 ----> EEPROM
 |
 +---- CS2 ----> ADC
```

---

## 💻 C code

Conceptually:

```c
void SPI_Select(void)
{
    /*
     * Active-low chip select.
     */
    CS_LOW();
}


void SPI_Deselect(void)
{
    /*
     * Return CS to inactive state.
     */
    CS_HIGH();
}


uint8_t SPI_Transfer(uint8_t tx_data)
{
    /*
     * SPI hardware handles:
     *
     * MOSI -> transmit
     * MISO -> receive
     * SCK  -> generated by master
     */

    SPI_TX_REGISTER = tx_data;

    while ((SPI_STATUS_REGISTER &
            SPI_TRANSFER_COMPLETE) == 0U)
    {
        /* Wait */
    }

    return SPI_RX_REGISTER;
}
```

A complete transaction:

```c
CS_LOW();

uint8_t rx = SPI_Transfer(0xA5);

CS_HIGH();
```

---

## 🎯 Follow-up questions

**Q:** Can MOSI and MISO be shared by multiple slaves?

**A:** Yes, normally.

**Q:** Can SCK be shared?

**A:** Yes.

**Q:** Why does each slave usually need a separate CS?

**A:** To select exactly one slave for a transaction.

**Q:** Can CS be active HIGH?

**A:** Yes. Active-low is common, but the actual polarity is determined by the device/interface design.

---

## 🧠 Remember

```text
SCK  → Clock
MOSI → Master → Slave
MISO → Slave → Master
CS   → Select slave
```

---

# Q3. How does SPI support multiple slaves?

## ⭐ Answer

SPI supports multiple slaves by **sharing SCK, MOSI and usually MISO**, while providing a separate chip-select line for each slave.

For example:

```text
                 +---- Slave 0
                 |
SCK -------------+---- Slave 1
                 |
                 +---- Slave 2


                 +---- Slave 0
                 |
MOSI ------------+---- Slave 1
                 |
                 +---- Slave 2


                 +---- Slave 0
                 |
MISO ------------+---- Slave 1
                 |
                 +---- Slave 2


CS0 ------------------> Slave 0
CS1 ------------------> Slave 1
CS2 ------------------> Slave 2
```

Only the selected slave should actively drive MISO.

---

## 🔍 Step-by-step

Suppose we want Slave 1.

Initially:

```text
CS0 = HIGH
CS1 = HIGH
CS2 = HIGH
```

Select Slave 1:

```text
CS0 = HIGH
CS1 = LOW
CS2 = HIGH
```

Now:

```text
Slave 0 → inactive
Slave 1 → active
Slave 2 → inactive
```

The master can communicate with Slave 1.

---

## 💻 C code

```c
#define SLAVE_0    0U
#define SLAVE_1    1U
#define SLAVE_2    2U


void SPI_SelectSlave(uint8_t slave)
{
    /*
     * First deselect every slave.
     */
    CS0_HIGH();
    CS1_HIGH();
    CS2_HIGH();


    /*
     * Select only the requested slave.
     */
    switch (slave)
    {
        case SLAVE_0:
            CS0_LOW();
            break;

        case SLAVE_1:
            CS1_LOW();
            break;

        case SLAVE_2:
            CS2_LOW();
            break;

        default:
            /*
             * Invalid slave.
             */
            break;
    }
}
```

Then:

```c
SPI_SelectSlave(SLAVE_1);

SPI_Transfer(0x55);

SPI_DeselectAll();
```

---

## 🎯 Follow-up questions

**Q:** What happens if two slaves are selected simultaneously?

**A:** They may both attempt to drive MISO, causing bus contention if both outputs are actively driven.

**Q:** What does an unselected slave do with MISO?

**A:** Normally it places its MISO output in high-impedance state.

**Q:** Can eight slaves share one SPI bus?

**A:** Yes, provided the electrical loading, CS availability, timing, and device requirements are handled.

**Q:** How can you create more CS signals?

**A:** GPIO expansion, decoder/demultiplexer, or suitable hardware.

---

# Q4. How can an SPI slave send data to the master?

## ⭐ Answer

An SPI slave sends data to the master through **MISO**, but the **master must provide the clock on SCK**.

The slave normally cannot initiate an SPI transfer by itself.

---

## 🔍 Step-by-step

Suppose the master wants to read register `0x20`.

### Step 1

Master selects slave:

```text
CS = LOW
```

### Step 2

Master sends:

```text
READ command
REGISTER = 0x20
```

through MOSI.

### Step 3

Master generates additional SCK pulses.

### Step 4

Slave shifts register data onto MISO.

### Step 5

Master samples MISO.

---

## 💻 C code

```c
uint8_t SPI_ReadRegister(uint8_t register_address)
{
    uint8_t data;


    /*
     * Select slave.
     */
    CS_LOW();


    /*
     * Send register address.
     */
    SPI_Transfer(register_address);


    /*
     * Generate clock pulses.
     *
     * The transmitted 0xFF is dummy data.
     *
     * During these clock cycles, the slave
     * shifts its response onto MISO.
     */
    data = SPI_Transfer(0xFFU);


    /*
     * Deselect slave.
     */
    CS_HIGH();


    return data;
}
```

The important concept:

```text
              MOSI
Master -----------------> Slave
       READ + ADDRESS

              SCK
Master -----------------> Slave
       CLOCK CLOCK CLOCK

              MISO
Master <----------------- Slave
             DATA
```

---

## 🎯 Follow-up questions

**Q:** Why does the master send `0xFF`?

**A:** To generate clock pulses while receiving the slave's data. The actual dummy value depends on the device/protocol.

**Q:** Can the slave generate SCK?

**A:** In normal SPI master-slave operation, no. The master generates SCK.

**Q:** Which signal carries slave-to-master data?

**A:** MISO.

---

# Q5. Why does the master generate clock during a read?

## ⭐ Answer

SPI is a synchronous shift-register protocol. A slave shifts its data out only when clock edges occur.

Therefore, even when the master is only interested in receiving data, it must generate SCK pulses.

---

## 🔍 Step-by-step

Suppose the slave wants to send:

```text
10110010
```

The slave cannot simply put all eight bits onto MISO at once.

Instead:

```text
Clock 1 → bit 7
Clock 2 → bit 6
Clock 3 → bit 5
...
Clock 8 → bit 0
```

Therefore:

```text
Master generates 8 clocks
          ↓
Slave shifts 8 bits
          ↓
Master receives 8 bits
```

---

## 💻 C code

```c
uint8_t SPI_ReadByte(void)
{
    /*
     * Send a dummy byte.
     *
     * Sending 8 bits causes the SPI controller
     * to generate 8 SCK clock cycles.
     *
     * The slave uses those clocks to shift
     * its response onto MISO.
     */

    return SPI_Transfer(0xFFU);
}
```

Conceptually:

```text
TX:
11111111
^^^^^^^^
8 clock cycles

RX:
ABCDEFGH
```

The TX value is not necessarily meaningful; its purpose is to keep the clock running.

---

## 🎯 Follow-up questions

**Q:** What happens if the master doesn't generate clocks?

**A:** The slave normally cannot shift out the requested data.

**Q:** Is the dummy byte always `0xFF`?

**A:** No. The value can depend on the controller/device. The important purpose is generating clocks.

**Q:** How many clocks are required to receive 16 bits?

**A:** Normally 16 clock cycles.

---

# Q6. What is SPI full-duplex communication?

## ⭐ Answer

SPI is normally full-duplex because the master can transmit data through MOSI and receive data through MISO **during the same clock cycles**.

For every transmitted bit, a received bit can also be shifted.

---

## 🔍 Step-by-step

Suppose:

```text
Master TX = 0xA5
Slave TX  = 0x3C
```

During the transfer:

```text
Master → MOSI → Slave
0xA5

Master ← MISO ← Slave
0x3C
```

So:

```c
rx = SPI_Transfer(tx);
```

performs both operations.

---

## 💻 C code

```c
uint8_t SPI_Transfer(uint8_t tx_data)
{
    /*
     * Put transmit byte into SPI hardware.
     */
    SPI_TX_REGISTER = tx_data;


    /*
     * Hardware generates 8 clock cycles.
     *
     * MOSI:
     *     tx_data -> slave
     *
     * MISO:
     *     slave -> receive register
     */

    while ((SPI_STATUS_REGISTER &
            SPI_TRANSFER_COMPLETE) == 0U)
    {
        /* Wait */
    }


    /*
     * Return simultaneously received byte.
     */
    return SPI_RX_REGISTER;
}
```

Example:

```c
uint8_t received;

received = SPI_Transfer(0xA5);
```

If the slave returns `0x3C`:

```text
TX = 0xA5
RX = 0x3C
```

---

## 🎯 Follow-up questions

**Q:** Is SPI always full duplex?

**A:** Standard SPI commonly supports simultaneous transmit and receive, although specific peripherals/controllers may have different operating modes.

**Q:** What happens during a transmit-only operation?

**A:** Received data may be ignored or discarded.

**Q:** What happens during a receive-only operation?

**A:** The master sends dummy data to generate clocks.

---

# Q7. What are CPOL and CPHA?

## ⭐ Answer

**CPOL and CPHA define the SPI clock behavior and determine when data is changed and sampled.**

* **CPOL** = Clock Polarity
* **CPHA** = Clock Phase

Together they determine the SPI mode.

---

## 🔍 Step-by-step

### CPOL

CPOL determines the **idle level of SCK**.

```text
CPOL = 0
Clock idle = LOW

CPOL = 1
Clock idle = HIGH
```

---

### CPHA

CPHA determines which clock edge is used for sampling/shifting.

The exact terminology around "first/second edge" should be understood together with the controller/device datasheet, but conceptually:

```text
CPHA = 0 → sample on first active edge
CPHA = 1 → sample on second active edge
```

---

## 💻 C code

A generic configuration might look like:

```c
typedef struct
{
    uint8_t cpol;
    uint8_t cpha;
} SPI_ModeConfig;


SPI_ModeConfig config;


/*
 * Example: SPI Mode 0
 *
 * CPOL = 0
 * CPHA = 0
 */

config.cpol = 0U;
config.cpha = 0U;
```

Then the actual driver would program the MCU's SPI control register.

---

## 🎯 Follow-up questions

**Q:** What are the four SPI modes?

**A:**

```text
Mode 0 = CPOL 0, CPHA 0
Mode 1 = CPOL 0, CPHA 1
Mode 2 = CPOL 1, CPHA 0
Mode 3 = CPOL 1, CPHA 1
```

**Q:** What happens if CPOL/CPHA is wrong?

**A:** Data can be sampled on the wrong edge, causing corrupted or shifted data.

---

# Q8. Explain all four SPI modes.

## ⭐ Answer

SPI has four standard combinations of CPOL and CPHA.

```text
Mode     CPOL     CPHA
------------------------
0         0        0
1         0        1
2         1        0
3         1        1
```

---

## 🔍 Step-by-step

### Mode 0

```text
CPOL = 0
CPHA = 0
```

Clock idle:

```text
LOW
```

---

### Mode 1

```text
CPOL = 0
CPHA = 1
```

Clock idle:

```text
LOW
```

But sampling phase differs from Mode 0.

---

### Mode 2

```text
CPOL = 1
CPHA = 0
```

Clock idle:

```text
HIGH
```

---

### Mode 3

```text
CPOL = 1
CPHA = 1
```

Clock idle:

```text
HIGH
```

---

## 💻 C code

You could represent the configuration:

```c
#define SPI_MODE_0   0U
#define SPI_MODE_1   1U
#define SPI_MODE_2   2U
#define SPI_MODE_3   3U


void SPI_SetMode(uint8_t mode)
{
    switch (mode)
    {
        case SPI_MODE_0:
            /*
             * CPOL = 0
             * CPHA = 0
             */
            break;

        case SPI_MODE_1:
            /*
             * CPOL = 0
             * CPHA = 1
             */
            break;

        case SPI_MODE_2:
            /*
             * CPOL = 1
             * CPHA = 0
             */
            break;

        case SPI_MODE_3:
            /*
             * CPOL = 1
             * CPHA = 1
             */
            break;

        default:
            /*
             * Invalid mode.
             */
            break;
    }
}
```

---

## 🎯 Follow-up questions

**Q:** How do you determine the required mode?

**A:** Check the slave datasheet and timing diagram.

**Q:** Can two slaves use different modes on the same SPI controller?

**A:** Yes, if the controller/driver can reconfigure the mode between transactions and CS is handled correctly.

**Q:** Can they use different clock frequencies?

**A:** Yes, provided the controller supports reconfiguration and each slave's limits are respected.

---

# Q9. What happens if master and slave use different SPI modes?

## ⭐ Answer

If the master and slave use different CPOL/CPHA settings, they may sample or change data at different clock edges.

The result can be:

* corrupted data
* shifted bits
* incorrect bytes
* intermittent communication

---

## 🔍 Step-by-step

Suppose the slave expects:

```text
Mode 0
CPOL = 0
CPHA = 0
```

But master uses:

```text
Mode 3
CPOL = 1
CPHA = 1
```

Now the clock idle state and sampling behavior differ.

The slave may sample before the master has placed valid data on the line.

---

## 💻 C example

```c
void Sensor_Init(void)
{
    /*
     * Sensor datasheet says:
     *
     * SPI Mode = 0
     */

    SPI_SetMode(SPI_MODE_0);

    /*
     * Configure suitable clock frequency.
     */
    SPI_SetClock(1000000U);
}
```

The important point is that:

```text
Master configuration
        ↓
must match
        ↓
Slave timing requirement
```

---

## 🎯 Follow-up questions

**Q:** How would you debug this problem?

**A:**

```text
Check datasheet
     ↓
Check CPOL
     ↓
Check CPHA
     ↓
Check waveform using logic analyzer
     ↓
Check transmitted/received bits
```

**Q:** What else besides CPOL/CPHA should you check?

**A:**

```text
Clock frequency
Bit order
CS timing
Word length
Voltage levels
Setup/hold timing
```

---

# Q10. What happens if two SPI slaves are selected simultaneously?

## ⭐ Answer

If two slaves are selected simultaneously and both actively drive the shared MISO line, they can cause **bus contention**.

For example:

```text
Slave 0 drives MISO = 1
Slave 1 drives MISO = 0
```

The master cannot reliably determine the intended value, and electrical contention may occur.

---

## 🔍 Step-by-step

Normally:

```text
CS0 = LOW
CS1 = HIGH
```

Only Slave 0 drives MISO.

If accidentally:

```text
CS0 = LOW
CS1 = LOW
```

both slaves may become active.

Potentially:

```text
Slave 0 ----\
             +---- MISO ---- Master
Slave 1 ----/
```

If their outputs disagree, there is a conflict.

---

## 💻 C code

A safer selection function first deselects all devices:

```c
void SPI_SelectSlave(uint8_t slave)
{
    /*
     * Deselect everything first.
     */
    CS0_HIGH();
    CS1_HIGH();
    CS2_HIGH();


    /*
     * Select exactly one slave.
     */
    switch (slave)
    {
        case 0U:
            CS0_LOW();
            break;

        case 1U:
            CS1_LOW();
            break;

        case 2U:
            CS2_LOW();
            break;

        default:
            /*
             * Invalid slave.
             */
            break;
    }
}
```

---

## 🎯 Follow-up questions

**Q:** Why doesn't this normally happen when slaves are correctly designed?

**A:** Non-selected slaves normally place MISO in high impedance.

**Q:** Is high impedance the same as logic HIGH?

**A:** No.

```text
HIGH = actively driving 1

Hi-Z = electrically disconnected from the signal
```

This distinction is important.

---

# Q11. What happens to MISO when a slave is deselected?

## ⭐ Answer

Normally, a deselected SPI slave places its MISO output into a **high-impedance state**, allowing another selected slave to drive the shared MISO line.

---

## 🔍 Step-by-step

Suppose:

```text
CS0 = HIGH
CS1 = LOW
```

Then:

```text
Slave 0 → MISO = Hi-Z
Slave 1 → MISO = active
```

Therefore:

```text
Slave 1 controls MISO.
```

---

## 💻 C concept

This is normally handled by the SPI peripheral hardware inside the slave.

Conceptually:

```c
void SPI_Slave_Select(bool selected)
{
    if (selected)
    {
        /*
         * Enable SPI output.
         */
        MISO_OUTPUT_ENABLE();
    }
    else
    {
        /*
         * Put MISO into high impedance.
         */
        MISO_OUTPUT_DISABLE();
    }
}
```

This is conceptual; actual implementation depends on the MCU/peripheral.

---

## 🎯 Follow-up questions

**Q:** Why is Hi-Z required?

**A:** So multiple slaves can share the same MISO bus without driving against each other.

**Q:** What if a slave doesn't release MISO?

**A:** It can interfere with other slaves and cause communication problems.

**Q:** How would you diagnose that?

**A:** Use a logic analyzer/oscilloscope and check MISO behavior while toggling each CS.

---

# Q12. Why do we send a dummy byte during an SPI read?

## ⭐ Answer

Because the SPI master must generate clock pulses to receive data.

The dummy byte is transmitted only to make the SPI controller generate the required SCK cycles.

---

## 🔍 Step-by-step

Suppose we want one byte from a sensor.

We need:

```text
8 clock cycles
```

The master can perform:

```c
rx = SPI_Transfer(0xFF);
```

The `0xFF` is transmitted through MOSI, but the important thing is that it causes the controller to generate the clocks.

During those clocks:

```text
MISO → RX register
```

---

## 💻 C code

```c
uint8_t SPI_ReadByte(void)
{
    uint8_t rx_data;

    /*
     * Generate 8 SCK cycles.
     *
     * 0xFF is dummy TX data.
     */
    rx_data = SPI_Transfer(0xFFU);

    return rx_data;
}
```

A complete register read:

```c
uint8_t SPI_ReadRegister(uint8_t reg)
{
    uint8_t data;

    CS_LOW();

    /*
     * Send register address.
     */
    SPI_Transfer(reg);

    /*
     * Generate clock while receiving data.
     */
    data = SPI_Transfer(0xFFU);

    CS_HIGH();

    return data;
}
```

---

## 🎯 Follow-up questions

**Q:** Why not just call a receive function?

**A:** At the SPI bus level, receiving requires clock transitions, and the master generates those clocks through a transfer.

**Q:** Does dummy data always have to be `0xFF`?

**A:** No. `0xFF` is common, but the appropriate dummy value depends on the controller/device.

---

# Q13. How would you debug SPI when MISO is always HIGH?

## ⭐ Answer

I would debug it systematically instead of immediately assuming a software problem.

```text
Power
 ↓
CS
 ↓
SCK
 ↓
MOSI
 ↓
MISO
 ↓
SPI mode
 ↓
Clock frequency
 ↓
Device configuration
```

---

## 🔍 Step-by-step

### Step 1 — Check power

Verify:

```text
VCC
GND
I/O voltage
```

---

### Step 2 — Check CS

Verify that the correct slave's CS actually goes active.

```text
CS:
HIGH ────────┐
             └──────── LOW
```

---

### Step 3 — Check SCK

Use a logic analyzer.

Confirm that clock pulses are present.

---

### Step 4 — Check MOSI

Verify that the expected command/register address is being transmitted.

---

### Step 5 — Check MISO

If MISO remains:

```text
11111111
```

possible causes include:

```text
Slave not selected
Wrong CS
Slave held in reset
Wrong SPI mode
Wrong command
Wrong register
MISO not connected
MISO stuck high
Incorrect pin mux
```

---

### Step 6 — Check CPOL/CPHA

Verify the SPI mode from the slave datasheet.

---

### Step 7 — Check clock frequency

Try a lower frequency.

---

## 💻 C debugging example

```c
uint8_t SPI_ReadRegister(uint8_t reg)
{
    uint8_t rx;

    /*
     * Select device.
     */
    CS_LOW();

    /*
     * Send register address.
     */
    SPI_Transfer(reg);

    /*
     * Receive response.
     */
    rx = SPI_Transfer(0xFFU);

    /*
     * Deselect device.
     */
    CS_HIGH();

    return rx;
}
```

When debugging, inspect:

```text
CS
SCK
MOSI
MISO
```

on a logic analyzer.

---

## 🎯 Follow-up questions

**Q:** What if MOSI is correct but MISO is always `0xFF`?

**A:** Check CS, slave state, command/address, MISO connection, pin mux, SPI mode, and whether the slave releases/drives MISO correctly.

**Q:** What if MISO is always `0x00`?

**A:** Similar checks, plus verify pull-downs, power, output-enable behavior, and slave state.

---

# Q14. Why would SPI work at 1 MHz but fail at 20 MHz?

## ⭐ Answer

At a higher SPI frequency, the timing margin becomes smaller. Signal integrity, setup/hold timing, PCB characteristics, clock/data skew, and the slave's maximum supported frequency can become limiting factors.

---

## 🔍 Step-by-step

At:

```text
1 MHz
```

one clock period is:

```text
1 µs
```

At:

```text
20 MHz
```

one clock period is:

```text
50 ns
```

The available timing margin is therefore much smaller.

Possible causes:

```text
Slave maximum frequency exceeded
        ↓
Setup/hold violation
        ↓
Signal integrity problem
        ↓
Long PCB traces
        ↓
High capacitive loading
        ↓
Poor rise/fall times
        ↓
Clock/data skew
```

---

## 💻 C example

A driver might allow configurable clock speed:

```c
void SPI_SetSpeed(uint32_t frequency)
{
    /*
     * Program SPI baud-rate divider
     * according to MCU hardware.
     */

    SPI_BAUD_REGISTER = CalculateDivider(frequency);
}
```

For debugging:

```c
SPI_SetSpeed(1000000U);
```

If it works, try:

```c
SPI_SetSpeed(5000000U);
```

then:

```c
SPI_SetSpeed(10000000U);
```

and finally:

```c
SPI_SetSpeed(20000000U);
```

This can help identify a frequency-dependent problem.

---

## 🎯 Follow-up questions

**Q:** What would you check first?

**A:**

```text
Slave datasheet maximum SPI frequency
```

**Q:** What hardware tool would you use?

**A:**

```text
Oscilloscope
Logic analyzer
```

**Q:** What signals would you inspect?

**A:**

```text
SCK
MOSI
MISO
CS
```

**Q:** Why might a shorter PCB trace help?

**A:** It can reduce parasitic capacitance/inductance and signal-integrity problems.

---

# Q15. How would you implement SPI communication using DMA?

## ⭐ Answer

DMA can transfer SPI data between memory and the SPI peripheral without requiring the CPU to handle every transmitted or received byte.

For a large SPI transfer:

```text
Memory
  |
  | DMA
  v
SPI TX register
  |
  v
MOSI
```

and:

```text
MISO
  |
  v
SPI RX register
  |
  | DMA
  v
Memory
```

This reduces CPU overhead.

---

## 🔍 Step-by-step

Suppose we want to transmit:

```text
1000 bytes
```

### Without DMA

CPU repeatedly handles:

```text
write byte
wait
write byte
wait
write byte
...
```

This consumes CPU time.

---

### With DMA

CPU configures DMA:

```text
Source:
TX buffer

Destination:
SPI TX register

Length:
1000 bytes
```

Then starts the transfer.

DMA handles the movement.

---

## 💻 Conceptual C code

```c
#include <stdint.h>


#define SPI_BUFFER_SIZE    100U


static uint8_t tx_buffer[SPI_BUFFER_SIZE];
static uint8_t rx_buffer[SPI_BUFFER_SIZE];


void SPI_DMA_Transfer(uint8_t *tx,
                      uint8_t *rx,
                      uint32_t length)
{
    /*
     * ------------------------------------------------------------
     * Step 1: Configure TX DMA
     * ------------------------------------------------------------
     *
     * Memory:
     *     tx buffer
     *
     * Destination:
     *     SPI TX register
     */

    DMA_TX_SOURCE      = (uint32_t)tx;
    DMA_TX_DESTINATION = (uint32_t)&SPI_TX_REGISTER;
    DMA_TX_LENGTH      = length;


    /*
     * ------------------------------------------------------------
     * Step 2: Configure RX DMA
     * ------------------------------------------------------------
     *
     * Source:
     *     SPI RX register
     *
     * Destination:
     *     rx buffer
     */

    DMA_RX_SOURCE      = (uint32_t)&SPI_RX_REGISTER;
    DMA_RX_DESTINATION = (uint32_t)rx;
    DMA_RX_LENGTH      = length;


    /*
     * ------------------------------------------------------------
     * Step 3: Enable DMA channels
     * ------------------------------------------------------------
     */

    DMA_RX_ENABLE();
    DMA_TX_ENABLE();


    /*
     * ------------------------------------------------------------
     * Step 4: Select SPI slave
     * ------------------------------------------------------------
     */

    CS_LOW();


    /*
     * ------------------------------------------------------------
     * Step 5: Start SPI DMA transfer
     * ------------------------------------------------------------
     */

    SPI_DMA_ENABLE();


    /*
     * CPU can now perform other work.
     *
     * DMA transfers the data.
     */
}
```

When DMA finishes:

```c
void SPI_DMA_CompleteISR(void)
{
    /*
     * Transfer is complete.
     *
     * Disable SPI DMA.
     */
    SPI_DMA_DISABLE();


    /*
     * Deselect slave.
     */
    CS_HIGH();


    /*
     * Application can now process rx_buffer.
     */
}
```

---

## Important DMA concept

For SPI receive:

```text
The master still needs to generate SCK.
```

Therefore, a receive transfer generally needs a transmit side as well:

```text
TX DMA
   |
   v
Dummy data
   |
   v
SPI
   |
   +---- SCK generated
   |
   v
RX DMA
   |
   v
Memory
```

For example:

```c
tx_buffer[i] = 0xFFU;
```

can be used as dummy transmit data when reading.

---

## 🎯 Follow-up questions

**Q:** Why use DMA for SPI?

**A:** To reduce CPU involvement and efficiently handle larger transfers.

**Q:** Does DMA generate the SPI clock?

**A:** No. The SPI controller generates SCK. DMA moves data between memory and the SPI peripheral.

**Q:** What happens when DMA transfer completes?

**A:** Typically a DMA completion interrupt is generated, and the driver can finalize the transaction.

**Q:** When should CS be released?

**A:** After the complete SPI transaction has finished, including the required DMA/SPI completion conditions.

**Q:** What if CS goes HIGH before DMA finishes?

**A:** The slave may terminate the transaction or interpret it as an incomplete command, resulting in corrupted/incomplete communication.

---

# 🧠 Q1–Q15 — Final Revision Sheet

```text
Q1  What is SPI?
    → Synchronous serial, normally full duplex.

Q2  Explain SPI signals.
    → SCK, MOSI, MISO, CS.

Q3  Multiple slaves?
    → Shared SCK/MOSI/MISO + separate CS.

Q4  How does slave send data?
    → Slave sends through MISO while master generates SCK.

Q5  Why clock during read?
    → Clock is required to shift slave data out.

Q6  What is full duplex?
    → TX and RX occur simultaneously.

Q7  What are CPOL and CPHA?
    → Define clock polarity and sampling/phase behavior.

Q8  Four SPI modes?
    → 00, 01, 10, 11 combinations of CPOL/CPHA.

Q9  Wrong SPI mode?
    → Incorrect sampling/shift timing → corrupted data.

Q10 Two slaves selected?
    → Possible MISO contention.

Q11 Deselected slave MISO?
    → Normally Hi-Z.

Q12 Dummy byte?
    → Generates clocks while receiving.

Q13 MISO always HIGH?
    → Debug CS, SCK, MOSI, mode, command,
      pin mux, wiring, slave state, MISO.

Q14 Works at 1 MHz but not 20 MHz?
    → Check slave limit, timing and signal integrity.

Q15 SPI + DMA?
    → DMA moves data between memory and SPI;
      SPI hardware still generates SCK.
```

**Best way to prepare these for an interview:** don't memorize the C functions as hardware-specific code. Memorize the **data flow**:

```text
              SCK
               |
               v
MASTER -----> SPI CONTROLLER
   |               |
   | MOSI          | MISO
   v               ^
SLAVE <------------+
   |
   +---- CS selects the slave
```

Then be able to translate that understanding into C using **GPIO control + SPI registers + polling/interrupt/DMA**, depending on what the interviewer asks.


---

Question: How does SPI support multiple slave devices?

⭐️ Answer: Serial Peripheral Interface (SPI) normally has four signal lines: clock, two data lines, and chip select.

SCK is the serial clock generated by the master.
MOSI, or Master Out Slave In, carries data from the master to the slave.
MISO, or Master In Slave Out, carries data from the slave to the master.
CS, or Chip Select, enables one slave device.
For multiple slaves, the clock, MOSI, and MISO lines can be shared by all devices. The master provides a separate chip-select line for each slave, such as CS0, CS1, and CS2. To communicate with one device, the master pulls only that device’s chip-select line active, usually low. The other slaves keep their chip-select lines inactive and do not drive the MISO line. Then the master sends and receives data with the selected slave.

SPI is usually full duplex, so data can move in both directions during the same clock cycle.
It normally uses a single master, although some systems can support more complex arrangements.
Each slave can have its own clock speed and clock mode, defined by clock polarity and clock phase.


---

Question: How can an SPI slave send data to the master?

⭐️ Answer: An SPI slave sends data back through the Master In Slave Out (MISO) line while the master supplies the clock.

The master first selects the required slave by making its chip-select line active, usually low.
The master generates clock pulses on SCK. A slave normally sends data only when the master provides these clock pulses.
On each clock cycle, the selected slave shifts one bit onto the MISO line, and the master samples that bit.
At the same time, the master may send bits to the slave through Master Out Slave In (MOSI), so SPI is usually full-duplex.
When the slave is not selected, it places its MISO output in a high-impedance state. This allows several slaves to share the same MISO, MOSI, and clock lines without conflict.
For example, to read a sensor register, the master selects the sensor, sends a read command and register address on MOSI, then continues generating clock pulses. The sensor returns the register value on MISO during those clock cycles.

---


💬 **Question**: Why does SPI need four clock modes instead of using one common mode?

⭐️ **Answer**: SPI devices use different clock modes because each device may sample and change data on different clock edges.

- **Clock polarity (CPOL)** defines the clock’s idle level:
  - `CPOL = 0`: clock is idle low.
  - `CPOL = 1`: clock is idle high.
- **Clock phase (CPHA)** defines which edge is used to sample data and which edge changes data.
- The four combinations allow the SPI master to match the timing required by the connected slave:
  - **Mode 0:** `CPOL=0`, `CPHA=0`
  - **Mode 1:** `CPOL=0`, `CPHA=1`
  - **Mode 2:** `CPOL=1`, `CPHA=0`
  - **Mode 3:** `CPOL=1`, `CPHA=1`
- A device’s internal logic is designed for one particular timing relationship. If the master uses the wrong mode, the slave may sample data at the wrong edge, causing shifted bits, incorrect commands, or corrupted data.
- This is mainly due to compatibility with different chip designs and older SPI devices. There is no single SPI standard that forces all manufacturers to use one mode.
- In practice, **mode 0 and mode 3** are commonly used. The required mode is given in the device data sheet and configured in Linux using properties such as `spi-cpol` and `spi-cpha`.
- The mode is not selected randomly for every transfer. Usually, each SPI device has a fixed required mode, and the SPI controller configures that mode when communicating with that device.

---

💬 **Question**: If every manufacturer agreed to use only SPI mode 0, would that work?

⭐️ **Answer**: Yes, if every SPI device truly used the same timing as mode 0, then mode 0 would be enough for basic SPI communication.

- In **mode 0**, the clock is idle low. The device samples data on the rising edge and changes data on the falling edge.
- The master and slave would then have one common timing rule, so there would be no need for modes 1, 2, or 3.
- The different modes exist mainly because SPI was designed to be flexible and does not define one mandatory clock timing for every device.
- However, choosing mode 0 alone would not solve every interface difference. Devices could still require different:
  - Maximum clock frequencies
  - Setup and hold times
  - Bit order
  - Chip-select timing
  - Word lengths
  - Full-duplex or half-duplex transfers
- So, under your assumption, **yes, mode 0 would be sufficient for clock polarity and phase**. The four modes are needed only because real devices were designed with different timing requirements and must remain compatible with those devices.