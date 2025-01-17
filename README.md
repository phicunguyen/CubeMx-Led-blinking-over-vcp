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

This is the freertos task on window to send the the led blinking packet to stm32 over serial port every 200ms.

      void vLedTask(void *arg) {
        uint8_t buf[4];                      //buffer to hold led packet
        bool led=false;
        arg = arg;                           //avoid warning
        buf[0] = SERIAL_PACKET__LED;         //opcode low byte
        buf[1] = 0x00;                       //opcode high byte
        buf[2] = 0x01;                       //led number
        while (true) {
          buf[3] = led ? 0x01 : 0x0;         //led on/off
          led = !led;
          adapter.write(&adapter, buf, 4);   //this will form a packet with '[' and ']' and send it
          vTaskDelay(200);                   //delay 200ms
          if (_kbhit() != 0) {               //exit the program if any key is pressed.
            printf("exit\n");
            vTaskEndScheduler();
          }
        }
      }
  
This code does not have any flow control. it could cause the data overwritten if the sending is faster than the receving.
Every packet send should have a response packet. That will be my goal.
