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
#define MASK_SIZE 11
#define INPUT_FILE "Data/small.jpg"
#define OUTPUT_FILE "Data/stef9.jpg"
#define Thread_Count 10000
//------------------------------------------------------------------------------

int golden_measure(void);
void* medFilter(void* argument);
void bubbleSort(int * arr, int length, JPEG& Output,int y, int x);
int golden_measure_threaded(int xmin,int xmax,int ymin,int ymax);

struct param_struct{
  JPEG * in_img;
  JPEG * out_img;
  int xmin;
  int xmax;
  int ymin;
  int ymax;
  int ID;
};




// This is each thread's "main" function.  It receives a unique ID
void* medFilter(void* argument){
  

  struct param_struct *params = (struct param_struct*)argument;

  // pthread_mutex_lock(&Mutex);
  // printf("Hello from thread %d\n", params->ID);
  // printf("xmin: %d; xmax: %d; ymin: %d; ymax: %d\n", params->xmin,params->xmax,params->ymin,params->ymax);
  // pthread_mutex_unlock(&Mutex);


  golden_measure_threaded(params->xmin,params->xmax,params->ymin,params->ymax);

 // Do some heavy calculation

  // pthread_mutex_lock(&Mutex);
  // printf("Thread %d: I QUIT!\n", params->ID);
  // pthread_mutex_unlock(&Mutex);

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
  pthread_mutex_lock(&Mutex);
  Output.Rows[y][x] = arr[temp-1];
  Output.Rows[y][x+1] = arr[temp];
  Output.Rows[y][x+2] = arr[temp+1];
  pthread_mutex_unlock(&Mutex);
  return;
}

int golden_measure_threaded(int xmin,int xmax,int ymin,int ymax)
{
  int size = MASK_SIZE;
  int side = (int)(size-1)/2;
  int mask[size*size*Input.Components] = {0}; // each pixel has 3 components
  
  
  tic();
  int x,y,r,b,a,yind,xind;
  for(y = ymin; y <=ymax; y++){
    for(x = xmin; x <= xmax*Input.Components; x+=Input.Components){
      int mask[size*size*Input.Components] = {0};
      for(r = 0; r < Input.Components; r++){
        for(b = y-side; b <= y+side; b++){
          for(a = x-(side*Input.Components)+r; a <= x+(side*Input.Components)+r; a += Input.Components){
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
  tic();
  int x,y,r,b,a,yind,xind;
  for(y = 0; y < Input.Height; y++){
    for(x = 0; x < Input.Width*Input.Components; x+=Input.Components){
      int mask[size*size*Input.Components] = {0};
      for(r = 0; r < Input.Components; r++){
        for(b = y-side; b <= y+side; b++){
          for(a = x-(side*Input.Components)+r; a <= x+(side*Input.Components)+r; a += Input.Components){
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
  // golden_measure();
  printf("Time 1 = %lg ms\n", (double)toc()/1e-3);
  
  
  printf("End of SECTION 1...\n\n");
  // End of example -------------------------------------------------------------


  tic();

 // Spawn threads...
 pthread_t Thread   [Thread_Count]; // pThreads structure for thread admin
 int j;


// struct param_struct{
//   JPEG * in_img;
//   JPEG * out_img;
//   int xmin;
//   int xmax;
//   int ymin;
//   int ymax;
//   int ID;
// };


  struct param_struct structs[Thread_Count];
  int ind0,ind1,xmin,xmax,ymin,ymax;
  
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

  structs[j].in_img = &Input; 
  structs[j].out_img = &Output;
  structs[j].xmin = 0;
  structs[j].xmax = Input.Width-1;
  structs[j].ymin = ind0;
  structs[j].ymax = ind1;
  structs[j].ID = j;

  // printf("x: %d-%d; y: %d-%d\n",structs[j].xmin,structs[j].xmax,structs[j].ymin,structs[j].ymax);

  pthread_create(Thread+j, 0, medFilter, &structs[j]);
  
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
