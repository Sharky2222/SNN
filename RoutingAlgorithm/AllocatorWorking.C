#include<stdio.h> // define the header file  
#include<stdlib.h>
#include<string.h>



// in order to use this method, you need to repeat it NPL times and increase the counter each time
int *Allocator(int data[], int NPL, int NPR, int DSNCount) { 
	
	int *newData = (int*) malloc (sizeof (int) *7); //make new array for output, dont want pointer issues
	int fowardBack = data[0];
	int currentRouter = data[4];
	int currentDSN = data[5];
	int nextLayer;
	//equation for converting from router,dsn to layer, neuron
	//double currentLayerNeuron = (NPR * currentRouter / ( (double) NPL)) + (currentDSN / ( (double) NPL));
	//int currentLayer = (int) currentLayerNeuron;
	//int currentNeuron = (int) (currentLayerNeuron - currentLayer) * NPL;
	int currentLayerNeuron = NPR * currentRouter + currentDSN; // NPR is always 8, bitshift by 3 in hardware
	int currentNeuron = currentLayerNeuron % NPL; 
	int currentLayer = (currentLayerNeuron - currentNeuron) / NPL; // NPL will be a parameter of power of 2, bitshift in hardware
	
	//if foward, add 1, if back add -1
	if(fowardBack == 0) {
		nextLayer = currentLayer + 1;
	}
	else {
		nextLayer = currentLayer - 1;
	}
	
	//equation for converting from layer, nueron to router, dsn
	//double nextRouterDSN = nextLayer * NPL / ( (double) NPR) + (DSNCount /  (double) NPR) ;
	int nextRouterDSN = nextLayer * NPL + DSNCount; // NPL will be a parameter of power of 2, bitshift in hardware
	int nextDSN = nextRouterDSN % NPR; // NPR is always 8, bitshift by 3 in hardware
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


int main() {
	//initializing vars for incoming data
	int foback = 1; // 0 = forward, 1 = backward
	int buff = 5;
	int currRout = 2;
	int currDSN = 2;
	int desRout = 15;
	int desDSN = 7;
	int inf = 127;
	
	int NPL = 4;
	int NPR = 8;
	int thisRout = 2;
	//initializing data array for incoming info
	int inData[] = {foback, buff, desRout, desDSN, currRout, currDSN, inf};
	
	//if new data processed from synapse
	if(desRout == 15 && desDSN == 7) {
		int *outData;
		for(int i = 0; i < NPL; ++i) { 
			outData = Allocator(inData, NPL, NPR, i);
			printf("The current router is: %d\nThe current DSN is: %d\n", currRout, currDSN);
			printf("The destination router is: %d\nThe destination DSN is: %d\n\n", outData[2], outData[3]);
		}
	}
	else { //not new data, just pass along and change current router
		inData[2] = thisRout;
		printf("The current router is: %d\nThe current DSN is: %d\n", currRout, currDSN);
		printf("The destination router is: %d\nThe destination DSN is: %d\n", inData[2], inData[3]);
		
	}
}
