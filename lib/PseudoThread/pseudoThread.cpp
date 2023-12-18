
#include "pseudoThread.hpp"

#include <map>
#include <functional>


unsigned long getIdentifier()
{
  static unsigned long lastIdentifier = 0;
  return ++lastIdentifier;
}


bool executeIfTimeLeft(unsigned int pseudoThrId, unsigned long periodMs, voidFuntion timeLeftProcedure, 
                       voidFuntion otherwiseProcedure, std::function<unsigned long()> getTime) {
  static std::map<unsigned long, unsigned long> m;
  unsigned long now = getTime();
  unsigned long lastCheck = m[pseudoThrId];
  if (now - lastCheck > periodMs) {
    m[pseudoThrId] = now;
    timeLeftProcedure();
    return true;
  } else {
    if (otherwiseProcedure) otherwiseProcedure();
  }
  return false;
}