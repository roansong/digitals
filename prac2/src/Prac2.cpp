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



int* bubbleSort(int* arr, int length)
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




int medFilter(int* mask, int length)
{
  // printf("Bubble Sort\n");


  mask = bubbleSort(mask, length);


  // printf("[");
  // for(int i = 0; i < MASK_SIZE*MASK_SIZE; i++){
    
  //   printf("%d,",mask[i]);
  // }
  // printf("]\n");


  return mask[(int)length/2];
}



int main(int argc, char** argv){
  int j;
  int size = MASK_SIZE;
  int side = (int)(size-1)/2;
  int mask[size*size]; // each pixel has 3 components
  // Initialise everything that requires initialisation
  tic();
  pthread_mutex_init(&Mutex, 0);

  // Read the input image
  if(!Input.Read("Data/fly.jpg")){
    printf("Cannot read image\n");
    return -1;
  }

  // Allocated RAM for the output image
  if(!Output.Allocate(Input.Width, Input.Height, Input.Components)) return -2;

  // This is example code of how to copy image files ----------------------------
  // printf("Filling mask for each pixel\n");

  printf("Input Height: %d\nInput Width: %d\n",Input.Height,Input.Width );

  tic();
  int x,y,b,a,yind,xind,median;
  for(y = 0; y < Input.Height; y++){
    // printf("y: %d\n",y);
    for(x = 0; x < Input.Width*Input.Components;x++){
        // printf("x: %d\n",x);
        // printf("%d\n",Input.Components);
        
          
        for(b = -side; b <= side; b++){
          for(a = -side; a <= side; a++){
            yind = y+b;
            xind = x+a*Input.Components;
            // printf("yind: %d\n",yind);
            // printf("b: %d\n",b );
            if((yind < 0)||(xind < 0)||(yind >= Input.Height)||(xind >= Input.Width*Input.Components))
              mask[(b+side)*size + a+side] = Input.Rows[y][x];
              // mask[(b+side)*size + a+side] = 0;
            else
              mask[(b+side)*size + a+side] = Input.Rows[yind][xind];
          }

        median = medFilter(mask,sizeof(mask)/sizeof(mask[0]));
        Output.Rows[y][x] = median;
        // printf("Median: %d\n",median);

        }
        
        // Output.Rows[y][x+r] = medFilter(mask[r]);

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
  if(!Output.Write("Data/fly9x9.jpg")){
    printf("Cannot write image\n");
    return -3;
  }

  // Clean-up
  pthread_mutex_destroy(&Mutex);
  return 0;
}
//------------------------------------------------------------------------------
