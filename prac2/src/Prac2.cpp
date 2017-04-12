//==============================================================================
// Copyright (C) John-Philip Taylor
// tyljoh010@myuct.ac.za
//
// This file is part of the EEE4084F Course
//
// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>
//==============================================================================




#include "Prac2.h"
#define MASK_SIZE 9
#define INPUT_FILE "Data/fly.jpg"
#define OUTPUT_FILE "Data/fly9x9v2.jpg"
//------------------------------------------------------------------------------

// This is each thread's "main" function.  It receives a unique ID
void* Thread_Main(void* Parameter){
  int ID = *((int*)Parameter);

  pthread_mutex_lock(&Mutex);
  printf("Hello from thread %d\n", ID);
  pthread_mutex_unlock(&Mutex);

  for(int j = 0; j < 1e6; j++); // Do some heavy calculation

  pthread_mutex_lock(&Mutex);
  printf("Thread %d: I QUIT!\n", ID);
  pthread_mutex_unlock(&Mutex);

 return 0;
}
//------------------------------------------------------------------------------




int* bubbleSortOld(int* arr, int length)
{
  int n = length;

  bool swap = 1;
  int temp;

  while(swap == 1)
  {
    
    swap = 0;
    for(int i =1; i < n; i++)
    {
      if(arr[i-1] > arr[i])
      {
        temp = arr[i-1];
        arr[i-1] = arr[i];
        arr[i] = temp;
        swap = 1;
      }
    }
    n = n-1;
  }

  return arr;
}


void bubbleSort(int * arr, int length, JPEG& Output,int y, int x)
{
  int out[3] = {0};
  int n = length;
  bool swap = 1;
  int temp;
  while(swap == 1)
  {
    swap = 0;
    for(int i =3; i < n; i++)
    {
      if(arr[i-3] > arr[i])
      {
        temp = arr[i-3];
        arr[i-3] = arr[i];
        arr[i] = temp;
        swap = 1;
      }
    }
    n = n-1;
  }

  temp = (int)length/2;
  Output.Rows[y][x] = arr[temp-1];
  Output.Rows[y][x+1] = arr[temp];
  Output.Rows[y][x+2] = arr[temp+1];

  return;
}





// int medFilter(int& mask, int length,JPEG& Output)
// {
//   // printf("Bubble Sort\n");


//   bubbleSort(*mask, length, Output);


//   // printf("[");
//   // for(int i = 0; i < MASK_SIZE*MASK_SIZE; i++){
    
//   //   printf("%d,",mask[i]);
//   // }
//   // printf("]\n");

//   // printf("%d\n",Output.Rows[0][0] );

//   return 0;
//   return mask[(int)length/2];
// }



int main(int argc, char** argv){
  int j;
  int size = MASK_SIZE;
  int side = (int)(size-1)/2;


  // Initialise everything that requires initialisation
  tic();
  pthread_mutex_init(&Mutex, 0);

  // Read the input image
  if(!Input.Read(INPUT_FILE)){
    printf("Cannot read image\n");
    return -1;
  }

  int mask[size*size*Input.Components] = {0}; // each pixel has 3 components

  printf("[");
            for(int i = 0; i < MASK_SIZE*MASK_SIZE*Input.Components; i++){
              
              printf("%d,",mask[i]);
            }
            printf("]\n");

  // Allocated RAM for the output image
  if(!Output.Allocate(Input.Width, Input.Height, Input.Components)) return -2;

  // printf("Filling mask for each pixel\n");

  printf("Input Height: %d\nInput Width: %d\n",Input.Height,Input.Width );
  printf("Mask size: %d (%d per pixel)\n",sizeof(mask)/sizeof(mask[0]),MASK_SIZE*MASK_SIZE );

  tic();

  int x,y,r,b,a,yind,xind,median;
  for(y = 0; y < Input.Height; y++)
  {
    for(x = 0; x < Input.Width*Input.Components; x+=Input.Components)
    {

      int mask[size*size*Input.Components] = {0};

      // printf("x: %d; y: %d\n",x,y );

      // run for each color
      for(r = 0; r < Input.Components; r++)
      {
        for(b = y-side; b <= y+side; b++)
        {
          for(a = x-(side*Input.Components)+r; a <= x+(side*Input.Components)+r; a += Input.Components)
          {
              // printf("x: %d; y: %d; a: %d; b: %d; r: %d; size: %d; side: %d; Input: %d\n",x,y,a,b,r,size,side,Input.Rows[y][x+r]);

              // printf("[");
              // for(int i = 0; i < MASK_SIZE*MASK_SIZE*Input.Components; i++){
                
              //   printf("%d,",mask[i]);
              // }
              // printf("]\n");

              if(a<0)
                mask[(b+side-y)*size*Input.Components+a+Input.Components-x]=Input.Rows[y][x+r];
              else if(b<0)
                mask[(b+side-y)*size*Input.Components+a+Input.Components-x]=Input.Rows[y][x+r];
              else if(a>=Input.Width*Input.Components)
                mask[(b+side-y)*size*Input.Components+a+Input.Components-x]=Input.Rows[y][x+r];
              else if(b>=Input.Height)
                mask[(b+side-y)*size*Input.Components+a+Input.Components-x]=Input.Rows[y][x+r];
              else
                mask[(b+side-y)*size*Input.Components+a+Input.Components-x]=Input.Rows[b][a];
              // printf("[");
              // for(int i = 0; i < MASK_SIZE*MASK_SIZE*Input.Components; i++){
                
              //   printf("%d,",mask[i]);
              // }
              // printf("]\n");
          }
          
        }
          
      }

      bubbleSort(mask,sizeof(mask)/sizeof(mask[0]),Output,y,x);

      // median = medFilter(mask,sizeof(mask)/sizeof(mask[0]),Output);

    }
  }

  printf("Time = %lg ms\n", (double)toc()/1e-3);
  
  printf("End of example code...\n\n");
  // End of example -------------------------------------------------------------



  // Printing stuff is a critical section...
  pthread_mutex_lock(&Mutex);
  printf("Threads created :-)\n");
  pthread_mutex_unlock(&Mutex);

  tic();


  // No more active threads, so no more critical sections required

  printf("Time taken for golden measure to run = %lg ms\n", toc()/1e-3);

  // Write the output image
  if(!Output.Write(OUTPUT_FILE)){
    printf("Cannot write image\n");
    return -3;
  }

  // Clean-up
  pthread_mutex_destroy(&Mutex);
  return 0;
}
//------------------------------------------------------------------------------
