# Caeser-Cipher-encryption-and-Decryption-using-Socket-Programming.
This project implements Caesar cipher encryption and decryption using client-server architecture in C. The server sends a plain text message from a file to the client, the client performs encryption or decryption using a user-provided key, and the result is sent back to the server and saved to a file.
# Precautions to be followed while running the code:
1. While running the code, first open the " Command Prompt " in the server computer and use the command "ipconfig" and remember that both devices should be connected to the same network and paste the IPV4 address of the server computer in the client code. 
2. if you are connected to wireless WIFI , then enter that IPV4 address in the client code. 
3. If you are running or building the code in the "Code::Blocks" platform then create a new project and at the top click project option and click on Build Options and go to linker settings and click on add option and add the library called (ws2_32). This library allows Winsock Programming in that platform.
