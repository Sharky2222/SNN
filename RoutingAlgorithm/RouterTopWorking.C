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

int routingAlgorithm(int data[], bool fill[], int totalCol, int vth)  
	{  
		bool repeat = true;
		
		int buffCount = data[1];
		int routerDest = data[2];
		int routerCurr = data[4];
		int info = data[6];
		bool nf = fill[0];
		bool sf = fill[1];
		bool ef = fill[2];
		bool wf = fill[3];
		
		
		
		int result = 0;
		if(routerDest ==  routerCurr) { //if we made it
			result = 4;
		} else { //if we didnt make it
		    // multiple parameters instead of one data word as an array ^^
		    //start here
		    
		    int routerDestRow = routerDest / totalCol;
		    int routerDestCol = routerDest % totalCol;
		    int routerCurrRow = routerCurr / totalCol;
		    int routerCurrCol = routerCurr % totalCol;
		    
		    bool vertical = false;
		    bool horizontal = false;


		    
		    int yDistance = routerDestRow - routerCurrRow;
		    int xDistance = routerDestCol - routerCurrCol;
		    
		    // if stuff starts here
		    int longx = xDistance;
		    int longy = yDistance;
		    int shortx = xDistance;
		    int shorty = yDistance;
		    
		    //x if block
		    if(xDistance == 0) {
		    	longx = totalCol;
		    	shortx = 0;
		    } else if(abs(xDistance) > (totalCol / 2)) {
		    	longx = xDistance;
		    	shortx = (totalCol - abs(xDistance)) * (-1 * xDistance) / abs(xDistance);
		    } else {
		    	longx = (totalCol - abs(xDistance)) * (-1 * xDistance) / abs(xDistance);
		    	shortx = xDistance;
		    	}
		    
		    //y if block
		    if(yDistance == 0) {
		    	longy = totalCol;
		    	shorty = 0;
		    	} else if(abs(yDistance) > (totalCol / 2)) {
		    	longy = yDistance;
		    	shorty = (totalCol - abs(yDistance)) * (-1 * yDistance) / abs(yDistance);
		    } else {
		    	longy = (totalCol - abs(yDistance)) * (-1 * yDistance) / abs(yDistance);
		    	shorty = yDistance;
		    	}
		   
		    // go long if block
		    if(info > 2 * vth) {
		    	if(buffCount < 5) {
		    		if(!nf && !ef && !sf && !wf) {
		    			shortx = longx;
		    			shorty = longy;
		    			if(shortx > shorty) { // new fix for go long not working
		    				vertical = true;
		    				repeat = false;
		    			} else if(shorty > shortx) {
		    				horizontal = true;
		    				repeat = false;
		    			}
		    		}
		    	}
		    }
		
		    // direction block
		    //logic for determining direction
		    int horDirection = shortx/abs(shortx);
		    int vertDirection = shorty/abs(shorty);
		    
		
		    if(shortx == 0) {
		    vertical = true;
		    } else if(shorty == 0) {
		    horizontal = true;
		    } else {
		    	while(repeat) {
		    		if(!nf && !ef && !sf && !wf) { //fuck it, go vertical
		    			vertical == true;
		    			repeat = false;  
		    		} else if(nf && ef && sf && wf) { // if all are full repeat
		    			repeat = true;
		    		} else if(horDirection == 1 && ef) { //if going to the right but east is filled
		    			vertical = true;
		    			repeat = false;
		    		} else if(horDirection == -1 && wf) { //if going to the right but east is filled
		    			vertical = true;
		    			repeat = false; 
		    		} else if(vertDirection == 1 && nf) { //if going to the right but east is filled
		    			horizontal = true;
		    			repeat = false;
		    		} else if(vertDirection == -1 && sf) { //if going to the right but east is filled
		    			horizontal = true;
		    			repeat = false;   
		    		}
		    	}
		    }
		  
		    //choosing the direction block
		    if(horizontal) {
		    	if(horDirection == 1) {
		    		result = 2; //east
		    	} else {
		    		result = 3; // west
		    	}
		    } else {
		    	if(vertDirection == 1) {
		    		result = 1; // south
		    	} else {
		    		result = 0; // north
		    	}
		    }
		    
	    }
	    return result;	    
	} 

int main() {


// FROM ROUTING ALGORITHM: int data[], bool fill[], int totalCol, int vth
// FROM ALLOCATOR MAIN
	int foback = 1; // 0 = forward, 1 = backward
	int buff = 10;
	int currRout = 6;
	int currDSN = 0;
	int desRout = 15;
	int desDSN = 7;
	int inf = 150;
	bool fill[] = {false, false , false, false}; // N S E W
	int totalCol = 4;
	int vth = 127;
	int dest;
	
	int NPL = 8;
	int NPR = 8;
	int thisRout = 6;
	//initializing data array for incoming info
	int inData[] = {foback, buff, desRout, desDSN, currRout, currDSN, inf};
	int *outData;
		
	//if new data processed from synapse
	if(desRout == 15 && desDSN == 7) {
		for(int i = 0; i < NPL; ++i) { 
			outData = Allocator(inData, NPL, NPR, i);
		}
	}
	else { //not new data, just pass along and change current router
		int zero = inData[0];
		int one = inData[1];
		int two = inData[2];
		int three = inData[3];
		int four = inData[4];
		int five = inData[5];
		int six = inData[6];
		outData[0] = zero;
		outData[1] = one;
		outData[2] = two;
		outData[3] = three;
		outData[4] = thisRout;
		outData[5] = five;
		outData[6] = six;
		
	}
// END ALLOCATOR MAIN
	
	//int routingAlgorithm(int data[], bool fill[], int totalCol, int vth)  
	
	dest = routingAlgorithm(outData, fill, totalCol, vth);
	printf("The data started as:\t");
	for(int i = 0; i < (sizeof(inData) / sizeof(inData[0])); i++) {
	 	printf("%d ", inData[i]);
	}
	printf("\nThe data is now:\t");
	for(int j = 0; j < (sizeof(outData)) - 1; j++) {
	 	printf("%d ", outData[j]);
	}
	printf("\nand the data will be heading to: %d\n", dest); // N S E W L : 0 1 2 3
	

}
