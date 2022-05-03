#include<stdio.h> // define the header file  
#include<stdlib.h>
#include<string.h>



/*********************************************************************
The Allocator Method is responsible for dealiing with new data
the comes into the Router.  When new data[] is sent into this method,
it will output data[] with the correct desitnation based on
its current address and on whether it is propagating
fowards or backwards.  
How is it new data?  As the developer, you can decide however you want
But we decided that the destination bits will be maxed out to
indicate this.  
 - In our case, it is new data if: 
 	data[4] = 15 (4 bits all 1)
 	data[5] = 7 (3 bits all 1)
To run this function, you need to run a loop of i = 1:NPL
where DSNCount is i.  During the loop, save each indiviual output.
This method make NPL copies of the data, but with a correct destination

Paramters:
 - data[]: data must be an array of length 7 with the following components
 	 - data[0] = foward/backward propagation flag [can only be 0 or 1]
 	 	     0 indicates foward, 1 indicates backward
 	 - data[1] = buffer Counter
 	 - data[2] = destination router (here it MUST be the condition for new data)
 	 - data[3] = destination DSN (here it MUST be the condition for new data)
 	 - data[4] = current router
 	 - data[5] = current DSN
 	 - data[6] = info
 - NPL: The amount of Neurons per Layer.  Must be a power of 2 (1, 2, 4, 8...)
 - NPR: The amount of Neurons per Router.  Must be a power of 2 (1, 2, 4, 8...)
 - DSNCount: should be the variable used to loop this method.
 
Output:
 - The function will return a pointer to an array of length 7
   It has the same components of the data[] parameter
   
Warning!
 - data[] must follow the given paramerters
 - NPL and NPR must be powers of 2 (this is a hardware constraint)
 - The foward back parameter can only be 0 or 1
 - This method is meant to be looped, DSNCount should be the incrementer
 - This method returns a POINTER, pointers are dumb and C is dumb, be careful
 
Example use:
	for(int i = 0; i < NPL; i++) {
		int *newData = Allocator(data, NPL, NPR, i);
		...
		...
	}
*********************************************************************/

int *Allocator(int data[], int NPL, int NPR, int DSNCount) { 
	
	int *newData = (int*) malloc (sizeof (int) *7); //make new array for output, dont want pointer issues
	int fowardBack = data[0];
	int currentRouter = data[4];
	int currentDSN = data[5];
	int nextLayer;
	
	//equation for converting from router,dsn to layer, neuron
	int currentLayerNeuron = NPR * currentRouter + currentDSN; // NPR is always 8, bitshift by 3 in hardware
	int currentNeuron = currentLayerNeuron % NPL; 
	int currentLayer = (currentLayerNeuron - currentNeuron) / NPL; // NPL will be a of power of 2, bitshift in hardware
	
	//if foward, add 1, if back add -1
	if(fowardBack == 0) {
		nextLayer = currentLayer + 1;
	}
	else {
		nextLayer = currentLayer - 1;
	}
	
	//equation for converting from layer, nueron to router, dsn
	int nextRouterDSN = nextLayer * NPL + DSNCount; // NPL will be a parameter of power of 2, bitshift in hardware
	int nextDSN = nextRouterDSN % NPR; 
	int nextRouter = (nextRouterDSN - nextDSN) / NPR; // NPR is always 8, bitshift by 3 in hardware
	
	//assigning data to output
	newData[0] = data[0];
	newData[1] = data[1];
	newData[2] = nextRouter;
	newData[3] = nextDSN;
	newData[4] = data[4];
	newData[5] = data[5];
	newData[6] = data[6];
	
	return newData;
}

/**********************************************************************
The Print Network Method is responsible for printing a demo
for the allocator method.  This method prints out a 
demonstration of a neuron in a neural network, and which neurons
it sends its data to.  
In a neural network, the question is
"Which neuron do I fire to?  I fire to all of the neurons in the next layer"

Parameters:
 - inData[]: an array of length 7.  Must follow constraints from alllocator 
 	     method.  Follow instructions from above
 - NPL: Neurons per layer (must be power of 2)
 - NPR: Nuerons per Router (must be power of 2)
 - totalRouters: How many total Routers there are.  Must be a squared number
 - destRouterArray[]: an array of length NPL.  THis MUST be the output
 		      to the int *runAllocator Method
 - thisRouter: local address of the router this function is running in

Output:
 - Prints example and result to the console

Warning!
 - Follow all warnings and constraints from previous methods
 - totalRouters must be a square number (1, 4, 9, 25...)
********************************************************************/
void printNetwork(int inData[], int NPL, int NPR, int totalRouters, int destRoutArray[], int thisRout) {
	//initiallizing variables required
	int currRout = inData[4];
	int currDSN = inData[5];
	int totalNeurons = NPR * totalRouters;
	int totalLayers = totalNeurons / NPL;
	bool foundDest = false;
	int destLayer = 100;
	bool newData = true;
	
	if(destRoutArray[2] == 2000) { //if not new data
		newData = false;
		printf("\nThis is not New Data\n");
		printf("The currentRouter has been changed from %d to %d\n\n", currRout, destRoutArray[0]);
	} for(int i = 0; i < NPL; i++) { //sweeping veritcally
		for(int j = 0; j < totalLayers; j++) { //sweeping horizontally
			//in each new layer, checks to see if this is the dest layer
			for (int k = 0; k < NPL && !foundDest; k++) {
				if((NPL * j + i) / NPR == destRoutArray[k]) {
					foundDest = true;
					destLayer = j;
				}
			} if((NPL * j + i) == (currRout * NPR + currDSN) && !newData) //only if not new data
				printf("\t{.}");
			else if((NPL * j + i) == (currRout * NPR + currDSN) && newData) //current nueron location
				printf("\t(.)");
			else if(foundDest && destLayer == j && newData) //destination neuron location
				printf("\t[.]");
			else //other nuerons
				printf("\t .");
		} printf("\n");
	} switch(inData[0]) {
		case 0: printf("\n\t\tFoward Propagation is Occuring");
			printf("\n\t\t\t------->");
			break;
		case 1: printf("\n\t\tBackward Propagation is Occuring");
			printf("\n\t\t\t<-------");
			break;
		default: printf("\n\tNot New Data");
	} printf("\n\n---------------------------------------------------------------\n\n");

}

/***************************************************************
The Run Allocator Method runs the the Allocator Method above
So look, heres the deal.  C is pretty dumb.  2D arrays are pretty
hard to return and deal with.  So, if you're playing with this
code, and you know how to do this, go right ahead.  This was 
the way how I figured out how to deal with it.  Normally,
this method should return a 2D array which would be outData[NPL][7],
but I couldnt get it.  Realistically, the only thing I needed
for this simulation was the destination routers of all of the new 
info created.  So, thats what I did.  

Parameters:
 - inData[]: Array of length 7, follow instructions for this param in previous methods
 - NPL: Neurons per layer (power of 2)
 - NPR: Neurons per Router (power of 2)
 - thisRout: Local location of the router this is happening in
 
Output:
 - *destRoutArray: This is a POINTER to an array of length NPL.
 		   This "array" stores the values of all of the destinations
 		   of the new data being created.  If it is not new data, only
 		   [0] slot will be filled with actual data.  You can tell
 		   that it is not new data if [2] slot is 2000.  This is just
 		   a dummy flag

Warning!
 - follow all parameter requirements from above
 - C is very dumb with arrays.  If you know what you're doing, have fun
   If not, goodluck soldier
 - New data is determined if destRouter = 15 and destDSN = 7, anything else is old data
***************************************************************/
int *runAllocator(int inData[], int NPL, int NPR, int thisRout) {
	//make array for output, avoids pointer issues
	int *destRoutArray = (int*) malloc (sizeof (int) *NPL);  
	int *outData; //var for output of Allocator()
	
	if(inData[2] == 15 && inData[3] == 7) { //condition for new data
		for(int i = 0; i < NPL; ++i) { 
			outData = Allocator(inData, NPL, NPR, i);
			destRoutArray[i] = outData[2];
		}
	}
	else { //not new data, just pass along and change current router
		outData = inData;
		outData[2] = thisRout;
		destRoutArray[0] = outData[2];
		destRoutArray[2] = 2000;
	}
	return destRoutArray;
} 

/****************************************************************************
This is the main method of the Allocator Demonstration.  
This method runs the runAllocator and printNetwork method to demonstrate
the capabilities of the allocator.  This is soley meant for demo purposes.

If you would like to use or make more examples, just follow the format
that I have used, and copy and paste an new example.  

Output:
 - Will print the given examples and their results to the console

Warning!
 - Follow all warnings from above when using
 - If you copy and paste an new example, make sure to change the num of the variable
 - Have fun!
****************************************************************************/
int main() {
	int NPR = 8;
	int totalRouters = 9;
	int thisRout = 2;
	
	printf("For our case: \n\tThere will always be %d total Routers\n\tThere will always be %d Neurons per Router\n", totalRouters, NPR);
	printf("(.) Represents the Current Neuron that made the New Data\n");
	printf("[.] Represents the Destination Neurons of the New Data\n");
	printf("{.} Represents the Current Neuron, but it is Not New Data\n\n");
	

	int NPL1 = 8;
	int inData1[] = {0, 5, 15, 7, 2, 4, 127}; //foback, buff, desRout, desDSN, currRout, currDSN, inf
	int *destRoutArray1 = runAllocator(inData1, NPL1, NPR, thisRout);
	printNetwork(inData1, NPL1, NPR, totalRouters, destRoutArray1, thisRout);
	
	int NPL2 = 8;
	int inData2[] = {1, 5, 15, 7, 2, 4, 127}; //foback, buff, desRout, desDSN, currRout, currDSN, inf
	int *destRoutArray2 = runAllocator(inData2, NPL2, NPR, thisRout);
	printNetwork(inData2, NPL2, NPR, totalRouters, destRoutArray2, thisRout);
	
	int NPL3 = 16;
	int inData3[] = {0, 5, 15, 7, 3, 5, 127}; //foback, buff, desRout, desDSN, currRout, currDSN, inf
	int *destRoutArray3 = runAllocator(inData3, NPL3, NPR, thisRout);
	printNetwork(inData3, NPL3, NPR, totalRouters, destRoutArray3, thisRout);
	
	int NPL4 = 16;
	int inData4[] = {1, 5, 15, 7, 3, 5, 127}; //foback, buff, desRout, desDSN, currRout, currDSN, inf
	int *destRoutArray4 = runAllocator(inData4, NPL4, NPR, thisRout);
	printNetwork(inData4, NPL4, NPR, totalRouters, destRoutArray4, thisRout);	
	
	
	int NPL5 = 4;
	int inData5[] = {0, 5, 15, 7, 4, 6, 127}; //foback, buff, desRout, desDSN, currRout, currDSN, inf
	int *destRoutArray5 = runAllocator(inData5, NPL5, NPR, thisRout);
	printNetwork(inData5, NPL5, NPR, totalRouters, destRoutArray5, thisRout);

	int NPL6 = 4;
	int inData6[] = {1, 5, 15, 7, 4, 6, 127}; //foback, buff, desRout, desDSN, currRout, currDSN, inf
	int *destRoutArray6 = runAllocator(inData6, NPL6, NPR, thisRout);
	printNetwork(inData6, NPL6, NPR, totalRouters, destRoutArray6, thisRout);

	int NPL7 = 8;
	int inData7[] = {1, 5, 1, 7, 3, 6, 127}; //foback, buff, desRout, desDSN, currRout, currDSN, inf
	int *destRoutArray7 = runAllocator(inData7, NPL7, NPR, thisRout);
	printNetwork(inData7, NPL7, NPR, totalRouters, destRoutArray7, thisRout);
}
