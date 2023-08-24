#include "genBuffer.h"


genBuffer::genBuffer()
{  
  startPointer = NULL;
  totalNodes = 0;
  nodeIndex = 0;
}


void genBuffer::add(uint8_t* data, unsigned short int length)
{
	bufferNode* nodePointer;
	bufferNode* prevNode;
	
	nodePointer = (bufferNode*) malloc(sizeof(bufferNode));
	
	
	nodePointer->_data = new uint8_t[length];
	nodePointer->_len = length;
 nodePointer->_nodeID = nodeIndex;
 nodeIndex++;
	memcpy(nodePointer->_data, data, length);

	//Set up Linked List stuff
	if(totalNodes==0)
	{
		startPointer = nodePointer;
		nodePointer->nextNode = NULL;
	}
	else
	{
		prevNode = findLast();
		prevNode->nextNode = nodePointer;
		nodePointer->nextNode = NULL;
	}
	totalNodes++;
}

uint8_t* genBuffer::getData()
{
	bufferNode* nodePointer = startPointer;
	if(nodePointer!=NULL)
  {
	  return nodePointer->_data;
  }
  else
  {
    return NULL;
  }
}
unsigned short int genBuffer::getDataLen()
{
  bufferNode* nodePointer = startPointer;
  if(nodePointer!=NULL)
  {
    return nodePointer->_len;
  }
  else
  {
    return NULL;
  }
}


bufferNode* genBuffer::findByID(unsigned short int nodeID)
{
  bufferNode* nodePointer = startPointer;
  while(nodePointer != NULL)
  { 
      if(nodePointer->_nodeID==nodeID)
      {
        return nodePointer;
      }
      nodePointer = nodePointer->nextNode; 
   } 
   return NULL;
}


void genBuffer::deleteFirst()
{
  bufferNode* nodePointer = startPointer;
  if(nodePointer != NULL)
  { 
      deleteNode(nodePointer->_nodeID);
   } 
}

bufferNode* genBuffer::findLast()
{
  bufferNode* nodePointer = startPointer;
  while(nodePointer != NULL)
  { 
      if(nodePointer->nextNode==NULL)
      {
        return nodePointer;
      }
      nodePointer = nodePointer->nextNode; 
   } 
   return NULL;
}

bufferNode* genBuffer::findPrev(unsigned short int  nodeID)
{
  bufferNode* currentNode = startPointer;
  bufferNode* nextNode = NULL;
  
  while(currentNode != NULL)
  { 
    nextNode = currentNode->nextNode;
    if(nextNode!=NULL)
    {
      if(nextNode->_nodeID==nodeID)
      {
        return currentNode;
      }
    }
    currentNode = nextNode; 
  } 
  return NULL;
}

void genBuffer::deleteNode(unsigned short int nodeID)
{
	bufferNode* currentNode = findByID(nodeID);
  if(currentNode==NULL)
  {
    return;
  }
  bufferNode* prevNode = findPrev(nodeID);
  

  //check if Prev node exists or its the last node
  if(totalNodes==1)
  {
    currentNode = startPointer;
    delete currentNode;
    startPointer = NULL;
    totalNodes=0;
  }
  else if(prevNode==NULL)
  {
    startPointer = currentNode->nextNode;
    delete currentNode;
    totalNodes--;
  }
  else
  {
	  prevNode->nextNode = currentNode->nextNode;
    delete currentNode;
    totalNodes--;
  }
}
