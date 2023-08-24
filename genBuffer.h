#ifndef genBuffer_h
#define genBuffer_h
#include "Arduino.h"
#include <stdlib.h> 
#include <stdio.h>


typedef struct bufferNode{ 
  unsigned short int _nodeID;
  uint8_t* _data;
  unsigned short int _len;
  bufferNode* nextNode; 
  
  ~bufferNode()
  {
    delete[] _data;
    _data = nullptr;
  }
}bufferNode;

class genBuffer
{
  public:
    genBuffer();
    void add(uint8_t* data, unsigned short int length);
	  uint8_t* getData();
    unsigned short int getDataLen();
    bufferNode* findPrev(unsigned short int  nodeID);
    bufferNode* findLast();
    void deleteFirst();
	  void deleteNode(unsigned short int nodeID);
    bufferNode* findByID(unsigned short int nodeID);
	
    bufferNode* startPointer;
    unsigned short int totalNodes;
    unsigned short int nodeIndex;
    
	unsigned short int counter;
	unsigned short int packetIndex;
	
};

#endif
