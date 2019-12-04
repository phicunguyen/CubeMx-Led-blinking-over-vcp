# CubeMx-Led-blinking-over-vcp
This demo will show you how to send commands from window serial port to stm32f407 over VCP (virtual com port) to blink a led every 200ms.

How to run it:
  
    1. Use stm32 st-link utility to program the stm32f407-disco board with stm32f407disco_vcp_led.hex.
    2. From window, run SerialLedBlinking.exe. 

The stm32 firmware will blink the green led every 200ms and the SerialLedBlinking will blink the red led very 200ms. 

You need to select the comport for your stm32f407-disco board in my case it's COM72.

    list of stm32 comports:
    0: STMicroelectronics STLink Virtual COM Port (COM44)
    1: STMicroelectronics Virtual COM Port (COM72)
    Please select comport: 1
    Serial port open successfully
    press any key to exit:

If you stop the SerialLedBlinking.exe then the red led is also stop but the green led is still blinking (the green led is updated by the idle task in stm32 (freertos default task).
   
The packet sending from window to stm32 as below.
     
              [ascii]
    
      1. '[' indicates start of packet.
      2. ']' indicates end of packet.
      3. First 2 bytes is opcode.
      4. follow bytes are data. 
      5. All the data bytes and including the opcode will be convert to ascii except the '[' and ']'
      6. Every hex byte now become two ascii bytes.


  
