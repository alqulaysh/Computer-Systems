/*-------------------------------------------------------------------------*
 *---                                                                   ---*
 *---		trainLine.cpp                                               ---*
 *---                                                                   ---*
 *---	    This file defines code the implements 2 Station instances,	---*
 *---	 and some number of Train objects that transit between them.	---*
 *---                                                                   ---*
 *---	----	----	----	----	----	----	----	----	---*
 *---                                                                   ---*
 *---	Version 2.0		Joseph Phillips		2016 May 5                  ---*
 *---                                                                   ---*
 *-------------------------------------------------------------------------*/

//  Compile with: $ g++ trainLine.cpp -o trainLine -lpthread -g




/*
 
 Student Name: Abdulaziz Alqulaysh
 
 
 B- What do you notice?
 It works with no problems but it crashes at the 3rd try.
 
 
 ———————————————————————————————————————————————————————————————
 D- What do you notice?
 It works fine with no problems but in the 5th try it produced infinite results! I had to force quite the program to stop it.
 Because it has a lock and nothing can get inside the methods arrive() and leave().
 
 
 
 ———————————————————————————————————————————————————————————————
 
 F- What is the minimum number of conditions that you need? How does it behave?
 
 The minimum number of conditions is one. It works fine with no crashes and no infinite results.
 
 
 
 */



#include    <cstdlib>
#include	<string>
#include	<iostream>
#include    <unistd.h>
#include    <pthread.h>
#include    "Train.h"
#include    "Station.h"

const int	NUM_TRAINS		= 4; 
const int	NUM_NORTHERNLY_TRAINS	= NUM_TRAINS / 2;
const int	NUM_SOUTHERNLY_TRAINS	= NUM_TRAINS - NUM_NORTHERNLY_TRAINS;
const int       NUM_LOOPS		= 16;
const char*	TRAIN_NAME_ARRAY[NUM_TRAINS]
					= { "Thomas the Tank-Engine",
					    "Engine No. 9",
					    "California Zephyr",
					    "Tokaido Shinkansen"
					  };
const int	TRAIN_TRANSIT_TIME_MSECS[NUM_TRAINS]
					= { 10000,	// Thomas
					    10000,	// Engine no. 9
					     5000,	// CA Zephyr
					     1000	// Bullet train
					   };

Station		northTerminal("North Station");
Station		southTerminal("South Station");
 
//  PURPOSE:  To make the Train object pointed to by 'vPtr' do 'NUM_LOOP' times:
//	* arrive at 'northTerminal'
//	* leave at 'northTerminal'
//	* pause
//	* arrive at 'southTerminal'
//	* leave at 'southTerminal'
//	* pause
//	Returns a pointer to the Train it used.
void*   initiallyNorthernly (void*  vPtr)
{ 

  Train*        trainPtr = (Train*)vPtr;  

  for(int i = 0; i < NUM_LOOPS; i++)
  {
    northTerminal.arrive(trainPtr);
    northTerminal.leave();
    trainPtr->pause();
    southTerminal.arrive(trainPtr);
    southTerminal.leave();
    trainPtr->pause();
  }

 return((void*)trainPtr); 
}   
    

//  PURPOSE:  To make the Train object pointed to by 'vPtr' do 'NUM_LOOP' times:
//	* arrive at 'southTerminal'
//	* leave at 'southTerminal'
//	* pause
//	* arrive at 'northTerminal'
//	* leave at 'northTerminal'
//	* pause
//	Returns a pointer to the Train it used.
void*   initiallySouthernly (void*  vPtr)
{
     
  Train*        trainPtr = (Train*)vPtr;
  
  for(int i = 0; i < NUM_LOOPS; i++)
    {
      southTerminal.arrive(trainPtr);
      southTerminal.leave();
      trainPtr->pause();
      northTerminal.arrive(trainPtr);
      northTerminal.leave();
      trainPtr->pause();
    }

  return((void*)trainPtr);  
}   


int     main    (int		argc,
		 char*		argv[]
		)
{
  if  (argc > 1)
    srand(strtol(argv[1],NULL,10));

  pthread_t     tidArray[NUM_TRAINS];
  Train*	trainArray[NUM_TRAINS];

  for  (int i = 0;  i < NUM_TRAINS;  i++)
    trainArray[i] = new Train(TRAIN_NAME_ARRAY[i],TRAIN_TRANSIT_TIME_MSECS[i]);

  int trainInd	  = 0;

  //  Please make 'NUM_NORTHERNLY_TRAINS' threads (tidArray[0] to
  //  tidArray[NUM_NORTHERNLY_TRAINS-1]) run 'initiallyNorthernly()'.
  //  Each thread should get a pointer to its own Train instance
  //  (trainArray[0] to trainArray[NUM_NORTHERNLY_TRAINS-1], this is
  //   an array of POINTERS to trains, not trains themselves).

  for  (trainInd = 0; trainInd < NUM_NORTHERNLY_TRAINS ; trainInd++)
    pthread_create(&tidArray[trainInd],NULL,initiallyNorthernly,(void*)trainArray[trainInd]);

  //  Please make 'NUM_SOUTHERNLY_TRAINS' threads
  //  (tidArray[NUM_NORTHERNLY_TRAINS] to tidArray[NUM_TRAINS-1]) run
  //   'initiallySouthernly()'.  Each thread should get a pointer to its own
  //  Train instance (trainArray[NUM_NORTHERNLY_TRAINS] to
  //   trainArray[NUM_TRAINS-1], this is an array of POINTERS to trains, not
  //   trains themselves).

  for  ( ; trainInd < NUM_TRAINS ; trainInd++)
    pthread_create(&tidArray[trainInd],NULL,initiallySouthernly,(void*)trainArray[trainInd]);

  //  Leave this loop here.  I want to make sure you get the Train pointers
  //  back from initiallyNorthernly and initiallySouthernly().
  for  (int i = 0;  i < NUM_TRAINS;  i++)
    trainArray[i] = NULL;

  //  Wait for all Train threads.  Also, get the pointers to the Train objects
  //  and delete() them because they were created by 'new'
  
  for  (int i = 0; i < NUM_TRAINS ; i++)
    {
      pthread_join(tidArray[i],(void**)&trainArray[i]);
      delete(trainArray[i]);
    }

  return(EXIT_SUCCESS);
}
