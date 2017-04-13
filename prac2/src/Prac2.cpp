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
#define INPUT_FILE "Data/small.jpg"
#define OUTPUT_FILE "Data/test.jpg"
#define Thread_Count 1
//------------------------------------------------------------------------------

int golden_measure(void);
void* Thread_main(void* argument);
void bubbleSort(int * arr, int length, JPEG& Output,int y, int x);
void medFilter(int xmin,int xmax,int ymin,int ymax);

struct param_struct{
  int xmin;
  int xmax;
  int ymin;
  int ymax;
};


void* Thread_main(void* argument){
  struct param_struct *params = (struct param_struct*)argument;
  medFilter(params->xmin,params->xmax,params->ymin,params->ymax);
 return 0;
}
//------------------------------------------------------------------------------

void bubbleSort(int * arr, int length, JPEG& Output,int y, int x){
  int out[3] = {0};
  int n = length;
  bool swap = 1;
  int temp;
  while(swap == 1){
    swap = 0;
    for(int i =3; i < n; i++){
      if(arr[i-3] > arr[i]){
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

void medFilter(int xmin=0,int xmax=Input.Width-1,int ymin=0,int ymax=Input.Height-1)
{
  int size = MASK_SIZE;
  int side = (int)(size-1)/2;
  int mask[size*size*Input.Components] = {0}; // each pixel has 3 components
  int x,y,r,b,a;
  for(y = ymin; y <=ymax; y++){
    for(x = xmin; x <= xmax*Input.Components; x+=Input.Components){
      int mask[size*size*Input.Components] = {0};
      for(r = 0; r < Input.Components; r++){
        for(b = y-side; b <= y+side; b++){
          for(a = x-(side*Input.Components)+r; a <= x+(side*Input.Components)+r; a += Input.Components){
              if(a<0)
                mask[(b+side-y)*size*Input.Components+a+Input.Components-x]=0;
              else if(b<0)
                mask[(b+side-y)*size*Input.Components+a+Input.Components-x]=0;
              else if(a>=Input.Width*Input.Components)
                mask[(b+side-y)*size*Input.Components+a+Input.Components-x]=0;
              else if(b>=Input.Height)
                mask[(b+side-y)*size*Input.Components+a+Input.Components-x]=0;
              else
                mask[(b+side-y)*size*Input.Components+a+Input.Components-x]=Input.Rows[b][a];
          }
        }      
      }
      bubbleSort(mask,sizeof(mask)/sizeof(mask[0]),Output,y,x);
    }
  }
}

int golden_measure(void)
{
  int size = MASK_SIZE;
  int side = (int)(size-1)/2;
  int mask[size*size*Input.Components] = {0}; // each pixel has 3 components

  printf("Input Height: %d\nInput Width: %d\n",Input.Height,Input.Width );
  printf("Mask size: %d (%d per pixel)\n",(int)(sizeof(mask)/sizeof(mask[0])),(int)MASK_SIZE*MASK_SIZE );
  int x,y,r,b,a;
  for(y = 0; y < Input.Height; y++){
    for(x = 0; x < Input.Width*Input.Components; x+=Input.Components){
      int mask[size*size*Input.Components] = {0};
      for(r = 0; r < Input.Components; r++){
        for(b = y-side; b <= y+side; b++){
          for(a = x-(side*Input.Components)+r; a <= x+(side*Input.Components)+r; a += Input.Components){
              if(a<0)
                mask[(b+side-y)*size*Input.Components+a+Input.Components-x]=0;
              else if(b<0)
                mask[(b+side-y)*size*Input.Components+a+Input.Components-x]=0;
              else if(a>=Input.Width*Input.Components)
                mask[(b+side-y)*size*Input.Components+a+Input.Components-x]=0;
              else if(b>=Input.Height)
                mask[(b+side-y)*size*Input.Components+a+Input.Components-x]=0;
              else
                mask[(b+side-y)*size*Input.Components+a+Input.Components-x]=Input.Rows[b][a];
          }
        }      
      }
      bubbleSort(mask,sizeof(mask)/sizeof(mask[0]),Output,y,x);
    }
  }
}

int main(int argc, char** argv){
  
  tic();
  pthread_mutex_init(&Mutex, 0);

  // Read the input image
  if(!Input.Read(INPUT_FILE)){
    printf("Cannot read image\n");
    return -1;
  }

  if(!Output.Allocate(Input.Width, Input.Height, Input.Components)) return -2;
  // golden_measure();
  printf("Input Height: %d\nInput Width: %d\n",Input.Height,Input.Width );
  printf("Mask size: %d (%d per pixel)\n",(int)MASK_SIZE*MASK_SIZE*Input.Components,(int)MASK_SIZE*MASK_SIZE );
  printf("Thread Count: %d\n", Thread_Count);

  tic();

  // Spawn threads...
  pthread_t Thread   [Thread_Count]; // pThreads structure for thread admin
  struct param_struct structs[Thread_Count];
  int j,ind0,ind1,xmin,xmax,ymin,ymax;

  int perthread = Input.Height/Thread_Count;
  int leftover = Input.Height%Thread_Count;
  int temp =0;
  for(j = 0; j < Thread_Count; j++){
  ind0 = temp;
  ind1 = temp+(perthread)-1;
  if(leftover > 0){
    ind1++;
    leftover--;
  }

  temp = ind1+1;
  structs[j].xmin = 0;
  structs[j].xmax = Input.Width-1;
  structs[j].ymin = ind0;
  structs[j].ymax = ind1;
  pthread_create(Thread+j, 0, Thread_main, &structs[j]);

  }

  // Printing stuff is a critical section...
  pthread_mutex_lock(&Mutex);
  printf("Threads created :-)\n");
  printf("Thread overheads = %lg ms\n", (double)toc()/1e-3);
  pthread_mutex_unlock(&Mutex);

  tic();
 // Wait for threads to finish
 for(j = 0; j < Thread_Count; j++){
  if(pthread_join(Thread[j], 0)){
   pthread_mutex_lock(&Mutex);
    printf("Problem joining thread %d\n", j);
   pthread_mutex_unlock(&Mutex);
  }
 }
  // No more active threads, so no more critical sections required
  printf("Time 2 = %lg ms\n", toc()/1e-3);

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
