#ifndef ISR_H
#define ISR_H

// https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf#page=49&zoom=140,45,199

#define _ISR( n ) __attribute__( ( signal ) ) void __vector_##n()

#define ISR_RESET                       _ISR( 0 )
#define ISR_EXTERNAL_0                  _ISR( 1 )
#define ISR_EXTERNAL_1                  _ISR( 2 )
#define ISR_PIN_CHANGE_0                _ISR( 3 )
#define ISR_PIN_CHANGE_1                _ISR( 4 )
#define ISR_PIN_CHANGE_2                _ISR( 5 )
#define ISR_WATCHDOG_TIMEOUT            _ISR( 6 )
#define ISR_TC2_COMPARE_MATCH_A         _ISR( 7 )
#define ISR_TC2_COMPARE_MATCH_B         _ISR( 8 )
#define ISR_TC2_OVERFLOW                _ISR( 9 )
#define ISR_TC1_CAPTURE                 _ISR( 10 )
#define ISR_TC1_COMPARE_MATCH_A         _ISR( 11 )
#define ISR_TC1_COMPARE_MATCH_B         _ISR( 12 )
#define ISR_TC1_OVERFLOW                _ISR( 13 )
#define ISR_TC0_COMPARE_MATCH_A         _ISR( 14 )
#define ISR_TC0_COMPARE_MATCH_B         _ISR( 15 )
#define ISR_TC0_OVERFLOW                _ISR( 16 )
#define ISR_SPI_TRANSFER_COMPLETE       _ISR( 17 )
#define ISR_USART_RECEPTION_COMPLETE    _ISR( 18 )
#define ISR_USART_DATA_REGISTER_EMPTY   _ISR( 19 )
#define ISR_USART_TRANSMISSION_COMPLETE _ISR( 20 )
#define ISR_ADC_CONVERSION_COMPLETE     _ISR( 21 )
#define ISR_EEPROM_READY                _ISR( 22 )
#define ISR_ANALOG_COMPARATOR           _ISR( 23 )
#define ISR_TWI                         _ISR( 24 )
#define ISR_SPM_READY                   _ISR( 25 )

#endif
