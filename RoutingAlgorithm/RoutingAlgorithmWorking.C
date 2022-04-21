#include<stdio.h> // define the header file  
#include<stdlib.h>




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
	
	int data[7] = {0, 10, 7, 3, 6, 7, 127};
	// {foback, buffCount, routerDest, DSNDest, routerCurr, DSNCurr, info}
	bool fill[4] = {true, false, false ,false};
	// {nf, sf, ef, wf};
	
	int totalCol = 4;
	int vth = 127;
	
	int result = routingAlgorithm(data, fill, totalCol, vth);
	
	printf("testing routing algorithm where buff count = %d and all buffs are empty\n routerDest = %d, routerCurr = %d, result = %d\n", data[1], data[2], data[4],  result);
	return 0;
}
