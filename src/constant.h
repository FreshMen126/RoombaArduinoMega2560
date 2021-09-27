#define byte uint8_t   //байт

#define RX_PIN 17  //rx 2
#define TX_PIN 16  //tx 2
#define DD_PIN 2   //данные
#define LED_PIN 13 //светодиод на 13 линии

#define TRIG_PIN_1 40
#define TRIG_PIN_2 44
#define TRIG_PIN_3 48
#define TRIG_PIN_4 30
#define TRIG_PIN_5 34
#define TRIG_PIN_6 38
#define ECHO_PIN_1 42
#define ECHO_PIN_2 46
#define ECHO_PIN_3 50
#define ECHO_PIN_4 28
#define ECHO_PIN_5 32
#define ECHO_PIN_6 36

#define SECOND 1000  //секунда
#define MINUTE 60000 //минута

#define _CLAMP(a, minimum, maximum) ((a < minimum) ? minimum : (a > maximum) ? maximum : a) //сравнение и выбор максимума и минимума, если превышает