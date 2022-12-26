
#include "pseudoThread.hpp"

#include <map>

// Pseudo threads
/// @brief Prints the prompt if periond_ms since last print has left.
/// @param prompt The text to print.
/// @param period_ms The time period in ms, that should left to print the next message.


/***********************************************************************
* Simple method that generates a unique identifier to the pseudo thread.
***********************************************************************/
unsigned long getIdentifier()
{
  static unsigned long lastIdentifier = 0;
  return ++lastIdentifier;
}

void periodicallyPrint(const char* prompt, unsigned int period_ms)
{
  static unsigned long lastCheck = 0;
  unsigned long now = millis();
  if (now - lastCheck > period_ms) {
    lastCheck = now;
    Serial.println(prompt);
  }
  return;
}

/***********************************************************************
* Executes method foo every period_ms seconds.
* This function is able to manage only one method in one time.
***********************************************************************/
bool periodicallyExecute(void foo(void), unsigned long period_ms) {
  static unsigned long lastCheck = 0;
  unsigned long now = millis();
  if (now - lastCheck > period_ms) {
    lastCheck = now;
    foo();
    return true;
  }
  return false;
}

/***********************************************************************
* Executes method foo every period_ms seconds.
* This function is able to manage multiple methods.
***********************************************************************/
bool m_periodicallyExecute(void foo(void), unsigned long period_ms) {
  static std::map<void (*)(), unsigned long> m;
  unsigned long now = millis();
  unsigned long lastCheck = m[foo];
  if (now - lastCheck > period_ms) {
    m[foo] = now;
    foo();
    return true;
  }
  return false;
}

/***********************************************************************
* Executes method foo every period_ms seconds.
* This function is able to manage multiple methods using
* pseudoThreadId as an pseidoTherad identifier
***********************************************************************/
bool m_std_periodicallyExecute(unsigned long pseudoThrId, std::function<void(void)> fun, unsigned long period_ms) {
  static std::map<unsigned long, unsigned long> m;
  unsigned long now = millis();
  unsigned long lastCheck = m[pseudoThrId];
  if (now - lastCheck > period_ms) {
    m[pseudoThrId] = now;
    fun();
    return true;
  }
  return false;
}
