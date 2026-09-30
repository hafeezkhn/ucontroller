### I2C - comonly used by an MCU to communicate with external sensors,memory,i/o extenders,displays and others

### Advantages
- you can communicate with multiple devices with just two signals (and ground)
- Built-in error detection , use ACKs to confirm correct operation.
- Handles framing (message framing dont have to code that determines starting and ending message).

### Disadvantages
- speeds are not great, mainly due to use of open drain signals 
- Generally need external pull-up resisters(due to open drain)
- possible address conflict 

## I2C Terminologies

- A transaction is the transfer of information on the I2C bus between devices - it can consist of one or more "read" and "write" operations
    - there are markers at the start and end of a transaction

- To distinguish I2C devices on the bus , each has a unique I2C address
    - the first byte written to the bus in an I2C transaction contains the address of the target device.
        + The selected device will participate.
        + All other devices on the bus will stay silent.
- Addresses are normally 7 bits in length - 

- for particular transaction, there is a master that initiates the transaction ,, and a slave that reacts to it,

    - At the start of transactions, master sends an address to select a particular slave on the bus.
    - it is most common that an MCU is the master, and some external device like sensor is the slave.
        + however , an MCU might act as a slave for some applications.
    - it is most common that there. is a single master on the bus.
        + A bus with multiple master is possible 


## open drain 

- The bus signal is at high level by default , due to the pull up resistor
- devices leave the signal alone for it to be high, or pull it down for it to be low
- I2C protocol determine which device controls signals at any given time.

## I2C BUS Operations



---
💬 **Question**: What is the fundamental difference between I2C, SPI, and UART?

⭐️ **Answer**: The main difference is in the number of wires, how devices are connected, and how data is synchronized.

- **Inter-Integrated Circuit (I2C)** is a two-wire, synchronous bus. It uses a clock line, called serial clock (SCL), and a data line, called serial data (SDA). One master can communicate with multiple slave devices using addresses. It supports bidirectional communication on the same data line, but it is slower than SPI. It is commonly used for sensors, electrically erasable programmable read-only memory (EEPROM), and monitoring devices. Because the bus is shared, devices need unique addresses.

- **Serial Peripheral Interface (SPI)** is also synchronous, but it normally uses separate lines for clock, data in, data out, and chip select. The usual signals are serial clock, master-out slave-in, master-in slave-out, and chip select. It is generally faster than I2C and supports full-duplex communication. Each slave normally needs a separate chip-select line, so it uses more pins. In Linux, the controller driver manages the SPI clock and chip select, while a protocol driver handles the connected device.

- **Universal Asynchronous Receiver-Transmitter (UART)** is asynchronous and normally uses transmit and receive lines, with a common ground. It does not have a clock line. Both sides must be configured with the same baud rate, data bits, parity, and stop bits. It is usually a point-to-point connection, so it does not naturally support multiple devices on the same bus. It is simple and commonly used for console output, debugging, and device-to-device communication.

- In short, I would choose **I2C** when I need several low-speed devices with few pins, **SPI** when I need higher speed and full-duplex transfers, and **UART** when I need a simple point-to-point asynchronous link.

---


💬 **Question**: What does synchronous mean in I2C?
⭐️ **Answer**: **Synchronous** means that the sender and receiver use a shared clock to control when each bit is sent and sampled.

- In I2C, the master generates the **serial clock line**, or SCL, and the data travels on the **serial data line**, or SDA.
- For each clock pulse, the data transmitter places one bit on SDA, and the receiver samples that bit at the correct clock level. This keeps both sides synchronized.
- The data line normally changes when the clock is low and remains stable while the clock is high, except for special conditions such as the start and stop signals.
- Because the master controls the clock, the device does not need to guess the timing from the data signal. This is different from **Universal Asynchronous Receiver-Transmitter (UART)**, which has no shared clock; both sides must be configured with the same baud rate and timing.
- The clock also allows the I2C master to control the transfer speed and, when needed, slow the transfer using clock stretching.

---


💬 **Question**: What happens if an I2C or SPI receiver is busy?

⭐️ **Answer**: I2C and SPI handle this differently because I2C has built-in acknowledgment and clock control, while SPI usually needs a device-specific method.

- In **Inter-Integrated Circuit (I2C)**, the receiver sends an **acknowledge (ACK)** bit after each byte. If it is busy or cannot accept more data, it can send a **not acknowledge (NACK)**. The master then detects that response and can wait, retry, or report an error.
- Some I2C slave devices also use **clock stretching**. The slave holds the serial clock line, or SCL, low until it is ready. The master must wait before continuing. The master should also use a timeout so that a faulty device cannot block the bus forever.
- In I2C, the master controls the transfer, so a slave cannot simply transmit whenever it wants. For example, a sensor may NACK while it is completing an internal measurement, and the driver can retry after a suitable delay.
- In **Serial Peripheral Interface (SPI)**, there is normally no standard ACK, NACK, or flow-control mechanism. The master controls the clock and chip-select line, so it must know whether the slave is ready.
- SPI devices commonly use a dedicated **ready**, **busy**, or **interrupt** pin. The master waits for that signal before starting the transfer. Some devices also define a status command or use a dummy transfer to read the status.
- If the receiver is still busy and the master ignores the protocol, the data may be invalid. Therefore, the driver should check the ready signal or status, apply a timeout, and retry or return an error.