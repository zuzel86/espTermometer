#include <ESP8266WiFi.h>

unsigned long getIdentifier();
void periodicallyPrint(const char* prompt, unsigned int period_ms);
bool periodicallyExecute(void foo(void), unsigned long period_ms);
bool m_periodicallyExecute(void foo(void), unsigned long period_ms);
bool m_std_periodicallyExecute(unsigned long pseudoThrId, std::function<void(void)> fun, unsigned long period_ms);
