/*
 * ================================================================
 * Battery Telemetry Packet Parser
 * ================================================================
 *
 * Problem:
 * -------
 * We receive a raw 10-byte battery telemetry packet.
 *
 * Packet format:
 *
 * Byte       Field
 * ------------------------------------------------
 * 0          Sync MSB
 * 1          Sync LSB
 * 2          Cell 1 voltage MSB
 * 3          Cell 1 voltage LSB
 * 4          Cell 2 voltage MSB
 * 5          Cell 2 voltage LSB
 * 6          Cell 3 voltage MSB
 * 7          Cell 3 voltage LSB
 * 8          Temperature MSB
 * 9          Temperature LSB
 *
 * All multi-byte values are BIG-ENDIAN.
 *
 *
 * Example packet:
 *
 *   AA 55  0F A0  10 68  0B B8  00 46
 *   |----| |----| |----| |----| |----|
 *    Sync  Cell1  Cell2  Cell3  Temp
 *
 *
 * Expected result:
 *
 *   Sync       = 0xAA55
 *   Cell 1     = 4000 mV
 *   Cell 2     = 4200 mV
 *   Cell 3     = 3000 mV
 *   Temperature = 70
 *
 * If any safety limit is violated, the corresponding
 * status bit is set.
 *
 * ================================================================
 */

#include <stdint.h>
#include <stdbool.h>


/*
 * ================================================================
 * 1. CONSTANTS
 * ================================================================
 */


/*
 * Synchronization word.
 *
 * Packet starts with:
 *
 *     AA 55
 *
 * Since the packet is BIG-ENDIAN:
 *
 *     0xAA 0x55
 *        ↓
 *     0xAA55
 */
#define SYNC_WORD              0xAA55U


/*
 * Number of battery cells.
 */
#define NUM_CELLS              3U


/*
 * Cell voltage limits in millivolts.
 *
 * Example:
 *
 *     Minimum = 2800 mV
 *     Maximum = 4200 mV
 */
#define MIN_CELL_VOLTAGE       2800U
#define MAX_CELL_VOLTAGE       4200U


/*
 * Maximum allowed temperature.
 */
#define MAX_TEMPERATURE        60


/*
 * ================================================================
 * 2. STATUS FLAGS
 * ================================================================
 *
 * We store multiple safety conditions inside one byte.
 *
 * Bit 0 -> Over voltage
 * Bit 1 -> Under voltage
 * Bit 2 -> Over temperature
 *
 *
 * Binary representation:
 *
 * FLAG_OVER_VOLTAGE
 *
 *     1 << 0
 *
 *     00000001
 *
 *
 * FLAG_UNDER_VOLTAGE
 *
 *     1 << 1
 *
 *     00000010
 *
 *
 * FLAG_OVER_TEMP
 *
 *     1 << 2
 *
 *     00000100
 */


/*
 * Bit 0
 */
#define FLAG_OVER_VOLTAGE      (1U << 0)


/*
 * Bit 1
 */
#define FLAG_UNDER_VOLTAGE     (1U << 1)


/*
 * Bit 2
 */
#define FLAG_OVER_TEMP         (1U << 2)



/*
 * ================================================================
 * 3. BIG-ENDIAN HELPER FUNCTION
 * ================================================================
 *
 * Function:
 *
 *     read_be_u16()
 *
 * Purpose:
 *
 *     Read TWO bytes from the packet and convert them into
 *     one 16-bit unsigned integer.
 *
 *
 * Example:
 *
 *     Memory:
 *
 *       p[0] = 0x0F
 *       p[1] = 0xA0
 *
 *
 *     BIG-ENDIAN means:
 *
 *       0x0F = MSB
 *       0xA0 = LSB
 *
 *
 *     Result:
 *
 *       0x0FA0
 *
 *
 * ================================================================
 */

static uint16_t read_be_u16(const uint8_t *p)
{
    /*
     * First byte is the MSB.
     *
     * Convert p[0] to uint16_t and shift it 8 bits
     * to the left.
     *
     *
     * Example:
     *
     *     p[0] = 0x0F
     *
     *     0x000F << 8
     *
     *          ↓
     *
     *     0x0F00
     */
    uint16_t high_byte = (uint16_t)p[0] << 8;


    /*
     * Second byte is the LSB.
     *
     * Example:
     *
     *     p[1] = 0xA0
     *
     *     0x00A0
     */
    uint16_t low_byte = (uint16_t)p[1];


    /*
     * Combine the two bytes using bitwise OR.
     *
     *
     *     0x0F00
     *        |
     *     0x00A0
     *     ------
     *     0x0FA0
     */
    return high_byte | low_byte;
}



/*
 * ================================================================
 * 4. SIGNED BIG-ENDIAN HELPER FUNCTION
 * ================================================================
 *
 * Temperature is treated as a signed 16-bit value.
 *
 * Example:
 *
 *     00 46
 *
 *     = 0x0046
 *     = 70
 *
 * Negative temperatures can also be represented using
 * two's complement.
 *
 * ================================================================
 */

static int16_t read_be_i16(const uint8_t *p)
{
    /*
     * First read the raw 16-bit bit pattern.
     */
    uint16_t raw_value = read_be_u16(p);


    /*
     * Interpret that same 16-bit pattern as signed.
     */
    return (int16_t)raw_value;
}



/*
 * ================================================================
 * 5. MAIN PARSER FUNCTION
 * ================================================================
 *
 * Function arguments:
 *
 *     buffer
 *         Pointer to the received telemetry packet.
 *
 *     status_bits
 *         Pointer where safety status flags will be stored.
 *
 *
 * Return value:
 *
 *     true
 *         Packet format is valid.
 *
 *     false
 *         Invalid input or invalid synchronization word.
 *
 * ================================================================
 */

bool unpack_battery_packet(const uint8_t *buffer,
                           uint8_t *status_bits)
{
    /*
     * Variable to store synchronization word.
     */
    uint16_t sync_word;


    /*
     * Variable to store the current cell voltage.
     */
    uint16_t cell_voltage;


    /*
     * Variable to store temperature.
     */
    int16_t temperature_c;


    /*
     * Local variable used to accumulate safety flags.
     *
     * Initially:
     *
     *     00000000
     */
    uint8_t flags = 0U;



    /*
     * ============================================================
     * STEP 1: CHECK POINTERS
     * ============================================================
     *
     * We must make sure that the pointers are valid before
     * accessing memory.
     *
     * Otherwise:
     *
     *     buffer[0]
     *
     * could cause invalid memory access if buffer == NULL.
     */

    if ((buffer == NULL) || (status_bits == NULL)) {

        /*
         * Invalid input.
         */
        return false;
    }



    /*
     * ============================================================
     * STEP 2: CLEAR OUTPUT STATUS
     * ============================================================
     *
     * We don't want an old status value from the caller.
     *
     * Therefore:
     *
     *     status_bits = 00000000
     */

    *status_bits = 0U;



    /*
     * ============================================================
     * STEP 3: READ SYNCHRONIZATION WORD
     * ============================================================
     *
     * Packet:
     *
     *     Byte 0 = AA
     *     Byte 1 = 55
     *
     * We pass the address of byte 0:
     *
     *     &buffer[0]
     *
     * to read_be_u16().
     *
     *
     * Internally:
     *
     *     AA << 8
     *       |
     *     55
     *
     * gives:
     *
     *     AA55
     */

    sync_word = read_be_u16(&buffer[0]);



    /*
     * ============================================================
     * STEP 4: VALIDATE SYNCHRONIZATION WORD
     * ============================================================
     */

    if (sync_word != SYNC_WORD) {

        /*
         * Packet does not start with the expected sync word.
         *
         * Therefore, reject the packet.
         */
        return false;
    }



    /*
     * ============================================================
     * STEP 5: READ THREE CELL VOLTAGES
     * ============================================================
     *
     * Packet layout:
     *
     *
     * Byte:
     *
     *       0     1     2     3     4     5     6     7
     *      AA    55    0F    A0    10    68    0B    B8
     *      |-----|     |-----|     |-----|     |-----|
     *       Sync        Cell1       Cell2       Cell3
     *
     *
     * Cell 1 starts at byte 2.
     * Cell 2 starts at byte 4.
     * Cell 3 starts at byte 6.
     *
     *
     * Formula:
     *
     *     start_index = 2 + (2 * i)
     *
     *
     * For i = 0:
     *
     *     2 + (2 * 0)
     *     = 2
     *
     * Cell 1 -> buffer[2], buffer[3]
     *
     *
     * For i = 1:
     *
     *     2 + (2 * 1)
     *     = 4
     *
     * Cell 2 -> buffer[4], buffer[5]
     *
     *
     * For i = 2:
     *
     *     2 + (2 * 2)
     *     = 6
     *
     * Cell 3 -> buffer[6], buffer[7]
     *
     * ============================================================
     */

    for (unsigned int i = 0U; i < NUM_CELLS; ++i) {


        /*
         * Read two bytes and convert from BIG-ENDIAN.
         *
         *
         * Cell 1:
         *
         *     buffer[2] = 0F
         *     buffer[3] = A0
         *
         *     0F A0
         *      ↓
         *     0x0FA0
         *      ↓
         *     4000 mV
         *
         */

        cell_voltage =
            read_be_u16(&buffer[2U + (2U * i)]);



        /*
         * ========================================================
         * STEP 6: UNDER-VOLTAGE CHECK
         * ========================================================
         *
         * If voltage is below 2800 mV:
         *
         *     set FLAG_UNDER_VOLTAGE
         *
         *
         * Example:
         *
         *     cell_voltage = 2500
         *
         *     2500 < 2800
         *
         *     TRUE
         */

        if (cell_voltage < MIN_CELL_VOLTAGE) {

            /*
             * Set bit 1.
             *
             *     flags |= 00000010
             */
            flags |= FLAG_UNDER_VOLTAGE;
        }



        /*
         * ========================================================
         * STEP 7: OVER-VOLTAGE CHECK
         * ========================================================
         *
         * If voltage is above 4200 mV:
         *
         *     set FLAG_OVER_VOLTAGE
         */

        if (cell_voltage > MAX_CELL_VOLTAGE) {

            /*
             * Set bit 0.
             *
             *     flags |= 00000001
             */
            flags |= FLAG_OVER_VOLTAGE;
        }
    }



    /*
     * ============================================================
     * STEP 8: READ TEMPERATURE
     * ============================================================
     *
     * Temperature is stored in:
     *
     *     Byte 8
     *     Byte 9
     *
     *
     * Example:
     *
     *     00 46
     *
     *     0x0046
     *
     *     = 70
     */

    temperature_c = read_be_i16(&buffer[8]);



    /*
     * ============================================================
     * STEP 9: CHECK TEMPERATURE
     * ============================================================
     *
     * Maximum allowed temperature:
     *
     *     60
     *
     * If:
     *
     *     temperature > 60
     *
     * set over-temperature flag.
     */

    if (temperature_c > MAX_TEMPERATURE) {

        /*
         * Set bit 2.
         *
         *     flags |= 00000100
         */
        flags |= FLAG_OVER_TEMP;
    }



    /*
     * ============================================================
     * STEP 10: RETURN STATUS
     * ============================================================
     *
     * Copy our locally accumulated flags to the caller.
     */

    *status_bits = flags;



    /*
     * Packet was structurally valid.
     *
     * IMPORTANT:
     *
     * true does NOT mean that the battery is completely safe.
     *
     * Safety problems are reported through status_bits.
     */

    return true;
}



/*
 * ================================================================
 * 6. EXAMPLE USAGE
 * ================================================================
 */

int main(void)
{
    /*
     * Example 10-byte packet.
     *
     *
     * Byte 0-1:
     *
     *     AA 55
     *     -> Sync = 0xAA55
     *
     *
     * Byte 2-3:
     *
     *     0F A0
     *     -> Cell 1 = 4000 mV
     *
     *
     * Byte 4-5:
     *
     *     10 68
     *     -> Cell 2 = 4200 mV
     *
     *
     * Byte 6-7:
     *
     *     0B B8
     *     -> Cell 3 = 3000 mV
     *
     *
     * Byte 8-9:
     *
     *     00 46
     *     -> Temperature = 70
     *
     *     70 > 60
     *     -> Over temperature
     */

    uint8_t battery_packet[10] =
    {
        0xAA, 0x55,
        0x0F, 0xA0,
        0x10, 0x68,
        0x0B, 0xB8,
        0x00, 0x46
    };


    /*
     * Variable to receive safety status.
     */
    uint8_t status = 0U;


    /*
     * Call the parser.
     */

    bool result =
        unpack_battery_packet(battery_packet, &status);



    /*
     * Result:
     *
     *     result = true
     *
     * because the synchronization word is valid.
     *
     *
     * status:
     *
     *     Cell 1 = OK
     *     Cell 2 = OK
     *     Cell 3 = OK
     *     Temperature = 70
     *
     * Therefore:
     *
     *     FLAG_OVER_TEMP = 00000100
     *
     * So:
     *
     *     status = 0x04
     */


    (void)result;
    (void)status;


    return 0;
}