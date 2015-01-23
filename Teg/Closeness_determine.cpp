
#include "Closeness_determine.h"

Closeness_determine::Closeness_determine()
{
  closeness = 0;
}

int Closeness_determine::determine(int distance)
{
  if(distance > 50 || distance == 0)
    closeness = 0;
  /*else if(distance <= 50 && distance > 20)
    closeness = 1;*/
  else if(distance <20 && distance != 0)
    closeness = 1;

  return closeness;
}



