#define byte uint8_t   //байт

#define RX_PIN 17  //rx 2
#define TX_PIN 16  //tx 2
#define DD_PIN 2   //данные
#define LED_PIN 13 //светодиод на 13 линии

#define SECOND 1000  //секунда
#define MINUTE 60000 //минута

#define _CLAMP(a, minimum, maximum) ((a < minimum) ? minimum : (a > maximum) ? maximum : a) //сравнение и выбор максимума и минимума, если превышает