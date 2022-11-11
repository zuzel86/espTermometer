#include <ESP8266WiFi.h>

void periodicallyPrint(char* prompt, unsigned int period_ms);
bool periodicallyExecute(void foo(void), unsigned long period_ms);
bool m_periodicallyExecute(void foo(void), unsigned long period_ms);
