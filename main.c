/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65
        Device            :  PIC16F1454
        Driver Version    :  2.00
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/
#include <string.h>
#include "mcc_generated_files/mcc.h"
#include "userDefinitions.h"

/** VARIABLES ******************************************************/

static bool buttonPressed;
static uint8_t readBuffer[CDC_DATA_OUT_EP_SIZE];
static uint8_t writeBuffer[CDC_DATA_IN_EP_SIZE];


void UserApplication(void);

/*
                         Main application
 */
void main(void)
{
    
    // initialize the device
    SYSTEM_Initialize();
    CDCInitEP();
    RC3 = 0;
    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    
    
    while(1)
    {
        USBDeviceTasks(); //Takes care of enumeration and other USB events
        
        

        //Otherwise we are free to run USB and non-USB related user 
        //application code.
        UserApplication();
            
        CDCTxService();
    }
}

void UserApplication(void) 
{
     /* If the USB device isn't configured yet, we can't really do anything
     * else since we don't have a host to talk to.  So jump back to the
     * top of the while loop. 
     * OR If we are currently suspended, then we need to see if we need to
     * issue a remote wakeup.  In either case, we shouldn't process any
     * keyboard commands since we aren't currently communicating to the host
     * thus just continue back to the start of the while loop. */
    if( USBGetDeviceState() < CONFIGURED_STATE || USBIsDeviceSuspended()== true )
    {
        return;
    }
    
    if( USBUSARTIsTxTrfReady() == true)
    {
        uint8_t i;
        uint8_t numBytesRead;

        numBytesRead = getsUSBUSART(readBuffer, sizeof(readBuffer));
        if(numBytesRead > 0) 
        {
            //Slaar paa LED om string inn inneholder "ON" eller "OFF" (foerste instans)
            if(strstr(readBuffer, "ON")) {
                RC3 = 1;
                strcpy(writeBuffer, readBuffer); //Bruk heller forløkke tror jeg, bruker færre ord
                putrsUSBUSART(writeBuffer);
            }
            if(strstr(readBuffer, "OFF")) RC3 = 0;
           
            //TODO : Håndtere åpning av I2C
            // Sette riktig kanal i i2c multiplexer
            // Konfigurere hver enkel sensor
            // Starte avlesning for hver sensor, basert på kommando, og skrive det til COM-port
            // Alt dette på de resterende 1300 ord.. evt droppe USB firmware
            // Evt bruke FTDI USB-2-serial chip (FT232RL eller liknednde)
            
            
        }
        
        
          
       
    }
    /*
    // Check to see if there is a transmission in progress, if there isn't, then
    // we can see about performing an echo response to data received.
    if( USBUSARTIsTxTrfReady() == true)
    {
        uint8_t i;
        uint8_t numBytesRead;

        numBytesRead = getsUSBUSART(readBuffer, sizeof(readBuffer));

        // For every byte that was read... 
        for(i=0; i<numBytesRead; i++)
        {
            switch(readBuffer[i])
            {
                //If we receive new line or line feed commands, just echo
                // them direct.
                //
                case 0x0A:
                case 0x0D:
                    writeBuffer[i] = readBuffer[i];
                    break;

                // If we receive something else, then echo it plus one
                // so that if we receive 'a', we echo 'b' so that the
                // user knows that it isn't the echo enabled on their
                // terminal program.
                //
                default:
                    writeBuffer[i] = readBuffer[i] + 1;
                    break;
            }
        }

        if(numBytesRead > 0)
        {
            // After processing all of the received data, we need to send out
            // the "echo" data now.
            //
            putUSBUSART(writeBuffer,numBytesRead);
        }
    }*/

    
}

/**
 End of File
*/