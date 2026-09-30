

---

💬 **Question**: How is UART different from a synchronous interface?
⭐️ **Answer**: **Universal Asynchronous Receiver-Transmitter (UART)** is asynchronous because it does not use a shared clock line.

- The sender and receiver agree beforehand on the **baud rate**, which is the number of symbols sent per second, along with the number of data bits, parity, and stop bits.
- Each character usually starts with a **start bit**, followed by the data bits, an optional parity bit, and one or more **stop bits**.
- The receiver detects the start bit and then uses its own clock to sample the data at the expected bit times. Therefore, both sides must have sufficiently accurate timing.
- Unlike I2C or SPI, UART is normally a simple point-to-point connection using transmit and receive lines, plus a common ground. It does not naturally use device addresses or a shared clock.

---

💬 **Question**: What happens in UART if the receiver cannot accept data immediately?

⭐️ **Answer**: UART itself does not provide flow control, so the sender may continue transmitting and the receiver can lose data or report an overrun error.

- The receiver normally has a small hardware buffer or **first-in, first-out (FIFO)** buffer. If software does not read the data quickly enough and the buffer becomes full, a **receive overrun** can occur.
- In that case, the receiver may discard the new byte, or the hardware may indicate an error. The exact behavior depends on the microcontroller or UART controller.
- To prevent this, UART can use **flow control**:
  - With hardware flow control, the receiver uses **Request to Send (RTS)** and the sender uses **Clear to Send (CTS)**. When the receiver is nearly full, it deasserts RTS, and the sender pauses.
  - With software flow control, special characters such as **XON** and **XOFF** are sent to resume or pause transmission.
- At the software level, I would also use an interrupt or Direct Memory Access (DMA) receive path with a ring buffer, process data from that buffer in a task, and monitor overrun and framing errors.
- Without flow control, the sender and receiver must be designed for the same data rate, or the protocol must include acknowledgments, retries, or buffering at a higher level.