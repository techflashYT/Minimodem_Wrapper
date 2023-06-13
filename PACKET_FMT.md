# Packet Format
This file outlines the format used for the packets sent over the audio signal.  

Each packet will contain a header with the following info:
| Position | Size & type            | Name of data                  |
|----------|------------------------|-------------------------------|
| 0-1      | 2 (2 char)             | Magic bytes (ASCII 'TF')      |
| 2        | 1 (uint8_t)            | Packet Size (multiples of 64) |
| 3        | 1 (uint8_t)            | ECC Data % of regular data    |
| 4-7      | 4 (uint32_t)           | CRC32 Checksum                |
| 7-end    | Any (array of uint8_t) | Actual Data + ECC             |
