#include <stdio.h>
#include "stdbool.h"


// TODO update with head and tail pointers instead of start and end index...
//      apparently, head is supposed to increment
//      if head == tail and !full then buffer is empty
/*
	1	Implement a circular buffer with the following functions:
	•	void cb_init(CircularBuffer *cb, int size): Initializes the 
    circular buffer with the given size.
	•	int cb_write(CircularBuffer *cb, char data): Writes data to 
    the buffer. Returns 0 if successful, -1 if the buffer is full.
	•	int cb_read(CircularBuffer *cb, char *data): Reads data from the buffer. Returns 0 if successful,     -1 if the buffer is empty.
  */

typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned char uint8_t;

#define CIRCULAR_BUFFER_SIZE 8

typedef enum {
  BUFFER_EMPTY = -1,
  BUFFER_FULL = -1,
  SUCCESS = 0
}circular_ret_t;

typedef struct circularBuffer {
  uint8_t buffer[CIRCULAR_BUFFER_SIZE];
  uint8_t head;
  uint8_t tail;
  bool full;
}CircularBuffer;

// Initializes the circular buffer with the given size.
void cb_init(CircularBuffer *cb, int size);

// Writes data to the buffer. Returns 0 if successful, -1 if the buffer is full.
circular_ret_t cb_write(CircularBuffer *cb, char data);

// Reads data from the buffer. Returns 0 if successful, -1 if the buffer is empty.
circular_ret_t cb_read(CircularBuffer *cb, char *data);

void advance_pointer(CircularBuffer *cb);

void retreat_pointer(CircularBuffer *cb);

// create our buffer
CircularBuffer cBuffer;

int main(void) {

  printf("Init buffer!\n");
  cb_init(&cBuffer, CIRCULAR_BUFFER_SIZE);

  printf("Writing buffer!\n");

  char data[CIRCULAR_BUFFER_SIZE] = {0xA, 0xB, 0xC, 0xD, 0x1, 0x2, 0x3, 0x4};
  
  for(int i = 0; i < CIRCULAR_BUFFER_SIZE; ++i)
  {
    printf("write %d\n", data[i]);
    cb_write(&cBuffer, data[i]);
  }
  
  // write half of the buffer again see the buffers loop
  for(int i = 0; i < 4; ++i)
  {
    printf("write: %d\n", data[i]);
    cb_write(&cBuffer, data[i]);
  }

  printf("Reading buffer!\n");
  char readData = 0;
  for(int i = 0; i < CIRCULAR_BUFFER_SIZE; ++i)
  {
    cb_read(&cBuffer, &readData);
    printf("read: %x\n", readData);
  }

  // additional reads shouldn't do anything because the buffer is empty
  for(int i = 0; i < CIRCULAR_BUFFER_SIZE; ++i)
  {
    cb_read(&cBuffer, &readData);
    printf("read: %x\n", readData);
  }
  
  printf("Done!\n");
  return 0;
}

// Initializes the circular buffer with the given size.
void cb_init(CircularBuffer *cb, int size)
{
  // clear the contents of our circular buffer
  for(int i = 0; i < size; i++)
  {
    cb->buffer[i] = 0; 
  }
  cb->head = 0;
  cb->tail = 0;
  cb->full = false;
}

// Writes data to the buffer. Returns 0 if successful, -1 if the buffer is full.
circular_ret_t cb_write(CircularBuffer *cb, char data)
{
  circular_ret_t ret = SUCCESS;
  if(cb->full)
  {
    ret = BUFFER_FULL;
  }
  
  // printf("Writing %d to index %d\n", data, cb->head);
  cb->buffer[cb->head] = data;
  advance_pointer(cb);
  
  cb->full = cb->head == cb->tail;
  
  return ret;
}

// Reads data from the buffer. Returns 0 if successful, -1 if the buffer is empty.
circular_ret_t cb_read(CircularBuffer *cb, char *data)
{
  // check if buffer is empty
  if((cb->head == cb->tail) && !cb->full)
  {
    return BUFFER_EMPTY;
  }
  *data = cb->buffer[cb->tail];
  // printf("Reading %d from index %d\n", *data , cb->tail);
  retreat_pointer(cb);
}

void advance_pointer(CircularBuffer *cb)
{
  if(cb->full)
  {
    if(++cb->tail== CIRCULAR_BUFFER_SIZE)
    {
      cb->tail = 0;
    }
  }
  if(++cb->head == CIRCULAR_BUFFER_SIZE)
  {
    cb->head = 0;
  }
}

void retreat_pointer(CircularBuffer *cb)
{
  cb->full = false;
  if(++cb->tail== CIRCULAR_BUFFER_SIZE)
  {
    cb->tail = 0;
  }
}