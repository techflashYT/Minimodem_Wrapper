#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <minimodem.h>
uint8_t globalParityOffset = 25;
bool minimodem(uint8_t *data, size_t size, bool mode, uint16_t baudRate, double confidence) {
    // Create ECC object
    ecc_t ecc = eccCreate(globalParityOffset, size);

    // determine the mode
    char modeStr[3] = { '-', 'A', '\0' };
    if (mode == MODE_TRANSMIT) {
        modeStr[1] = 't';
    } else if (mode == MODE_RECEIVE) {
        modeStr[1] = 'r';
    }

    // Build the command string
    char command[256];
    snprintf(command, sizeof(command), "minimodem --rx-one --startbits 2 --stopbits 2 %s %u -c %.3f", modeStr, baudRate, confidence);

    // Open the command as a bidirectional stream
    FILE *stream = popen(command, mode == MODE_TRANSMIT ? "w" : "r");
    if (stream == NULL) {
        perror("popen");
        exit(1);
    }

    if (mode == MODE_TRANSMIT) {
        // Transmit mode - encode data and write to the process's standard input
        uint8_t *encodedData;
        size_t encodedSize = eccEncode(ecc, data, size, &encodedData);
        fwrite(encodedData, sizeof(uint8_t), encodedSize, stream);
        fflush(stream);

        // Free the encoded data memory
        free(encodedData);
    } else if (mode == MODE_RECEIVE) {
        // Receive mode - read data from the process's standard output
        size_t totalBytesRead = 0;
        size_t bytesRead;
        while ((bytesRead = fread(readBuf, sizeof(uint8_t), 16384, stream)) > 0) {
            // Decode received data
            uint8_t *decodedData;
            eccDecode(ecc, readBuf, bytesRead, &decodedData);

            // Process the decoded data as needed
            // ...

            // Free the decoded data memory
            free(decodedData);

            totalBytesRead += bytesRead;
        }
    }

    // Close the stream
    int closeStatus = pclose(stream);
    if (closeStatus == -1) {
        perror("pclose");
        exit(1);
    }

    // Cleanup ECC object
    eccDestroy(ecc);

    // Wait for the child process to complete
    pid_t childPid = -1;
    int status;
    childPid = fork();
    if (childPid == -1) {
        perror("fork");
        exit(1);
    } else if (childPid == 0) {
        // Child process
        // ...
        exit(0);
    } else {
        // Parent process
        childPid = waitpid(childPid, &status, 0);
        if (childPid == -1) {
            perror("waitpid");
            exit(1);
        }
    }

    return false;
}
