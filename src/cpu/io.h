// #pragma once
// #include <stdint.h>

// /**
//  * @brief Shoves one byte of data out to the specific port
//  * 
//  * @param port The port to shove the data to
//  * @param val  The value than needs to be shoved out
//  */
// void outb(uint16_t port, uint8_t val);

// /**
//  * @brief Reads one byte of data from a specific port
//  * 
//  * @param port The port to read the data from
//  * @return One byte of the data
//  */
// uint8_t inb(uint16_t port);

// /**
//  * @brief Shoves 4 bytes of data out to the specific port
//  * 
//  * @param port The port to shove the data to
//  * @param val  The value that needs to be shoved out
//  */
// void outl(uint16_t port, uint32_t data);

// /**
//  * @brief Reads 4 bytes of data from a specific port
//  * 
//  * @param port The port to read the data from
//  * @return 4 bytes of data
//  */
// uint32_t inl(uint16_t port);