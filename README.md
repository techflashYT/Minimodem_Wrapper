# Minimodem_Wrapper
A wrapper for the `minimodem` program that implements a few new features, like:
 - Handshaking
 - Packet Format that allows for resends (see [PACKET_FMT.md](PACKET_FMT.md)) <== Planned, but not started yet!
 - Error correction using OpenFEC
 - **The below is also planned, but not implemented yet!**
 - Renegotiating baud rate on the fly to allow for either increasing speed, or stability, depending on which is more important at the time.
   - If we haven't had 100% success over time, try lowering it to see if we can get it slightly better.
   - If we've had 100% success rate for a while, try to increase the baud rate, and take note of whether it was successful
     - If it was, great!  Try to maintain that baud rate, and repeat the loop.
     - If not, take note, and go back to the original.  Try again later, in case the environment changes.
