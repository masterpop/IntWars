#include "Stats.h"

using namespace std;

float Stats::getStat(uint8 blockId, uint32 stat) const {
   int block = -1;
   while(blockId) {
      blockId = blockId >> 1;
      ++block;
   }
   auto it = stats[block].find(stat);
   if(it != stats[block].end()) {
      return it->second;
   }
   
   return 0;
}

void Stats::setStat(uint8 blockId, uint32 stat, float value) {
   int block = -1;
   updatedStats.insert(make_pair(blockId, stat));
   
   while(blockId) {
      blockId = blockId >> 1;
      ++block;
   }
   stats[block][stat] = value;
}

bool Stats::isFloat(uint8 blockId, uint32 stat) {
   switch(blockId) {
   case MM_One:
      switch(stat) {
      case FM1_SPELL:
         return false;
      
      }
      
   case MM_Four:
      switch(stat) {
      case FM4_Level:
         return false;
      
      }
   }

   return true;
}