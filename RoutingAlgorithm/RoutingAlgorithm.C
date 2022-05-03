#include<stdio.h> 
#include<stdlib.h>

/************************************************************
The routingAlgorithm function is used to determine which router
the Information should be sent to next based on the parameters
of the data and the fill states of the routers around it

Parameters: 
 - data[]: must be an array of length 7 where
 	- data[0] must be the FowardBack Flag
 	- data[1] must be the bufferCount
 	- data[2] must be the routerDest
 	- data[3] must be the DSNDest
 	- data[4] must be the routerCurr
 	- data[5] must be the DSNCurr
 	- data[6] must be the info
 - fill[]: must be an array of length 4 where
 	- fill[0] must be the North Full Flag
 	- fill[1] must be the South Full Flag
 	- fill[2] must be the East Full Flag
 	- fill[3] must be the West Full Flag
 - totalCol: an int describing the amount of collumns
  	     the router layout will have
 - vth: The average voltage threshold of the DSN's in this router
 
 Output:
  - an int ranging from 0 - 4 where
  	- 0 indicates North
  	- 1 indicates South
  	- 2 indicated East
  	- 3 indicates West
 	- 4 indicated Local
 	
 Example Use:
 int direction = routingAlgorithm(data, fill, totalCol, vth);
 
 Warning!
 - If all of the data in the fill[] array is True
   The algorithm will loop forever
   This is designed on purpose
   The router is designed to wait until there is an open
   spot in an adjacent Router to send the information to
   This is to avoid sending information to router that has
   no storage to hold the new incoming info
*************************************************************/
int routingAlgorithm(int data[], bool fill[], int totalCol, int vth)  {  
	//initiallizing variables needed
	bool repeat = true;
	int buffCount = data[1];
	int routerDest = data[2];
	int routerCurr = data[4];
	int info = data[6];
	bool nf = fill[0];
	bool sf = fill[1];
	bool ef = fill[2];
	bool wf = fill[3];
	int result = 0; //needs to be initialized to something at first
	bool vertical = false;
	bool horizontal = false;
	
	if(routerDest ==  routerCurr) { //if we made it
		result = 4; //send to local
	} else { //if we didnt make it
	    
	    int routerDestRow = routerDest / totalCol; //can be done with bit shift
	    int routerDestCol = routerDest % totalCol; //can be done with x % y = x AND (y-1)
	    int routerCurrRow = routerCurr / totalCol;
	    int routerCurrCol = routerCurr % totalCol;


	    //calculates the signed distance to the desired router in both x and y direction
	    int yDistance = routerDestRow - routerCurrRow;
	    int xDistance = routerDestCol - routerCurrCol;
	    
	    // if stuff starts here
	    int longx = xDistance;
	    int longy = yDistance;
	    int shortx = xDistance;
	    int shorty = yDistance;
	    
	    //x if block
	    //finds the longest x distance and shortest x distance
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
	    //finds the longest y distance and shortest y distance
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
	    if(info > 2 * vth) { //if the info is very big
	    	if(buffCount < 5) { //if the information has travelled a minimal amount
	    		if(!nf && !ef && !sf && !wf) { //if all of the surrounding routers are empty
	    			shortx = longx; //short distance is now set to long diestance
	    			shorty = longy;
	    			//if the shortx does not equal the short y, will skip the checks further down
	    			if(shortx > shorty) {
	    				vertical = true;
	    				repeat = false;
	    			} else if(shorty > shortx) {
	    				horizontal = true;
	    				repeat = false;
	    			}
	    		}
	    	}
	    }
	    //logic for determining direction
	    int horDirection = shortx/abs(shortx);
	    int vertDirection = shorty/abs(shorty);	
	    // direction block
	    
	    if(shortx == 0) { //go vertical if nowhere to go in x
	    vertical = true;
	    } else if(shorty == 0) { //horizontal if nowhere to go in y
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
	    		} else if(horDirection == -1 && wf) { //if going to the left but west is filled
	    			vertical = true;
	    			repeat = false; 
	    		} else if(vertDirection == -1 && nf) { //if going to the up but north is filled
	    		//NOTE THIS CHANGED
	    			horizontal = true;
	    			repeat = false;
	    		} else if(vertDirection == 1 && sf) { //if going to the down but south is filled
	    		//NOTE THIS CHANGED
	    			horizontal = true;
	    			repeat = false;   
	    		} else if(!nf && !sf && ef && wf) { //if north and south are not full, but east and west are
	    			vertical = true;
	    			repeat = false;
	    		} else if(nf && sf && !ef && !wf) { //if north and south are full, but east and west are not
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

/*****************************************************
Print Dots is a method used to print a square of dots
representing the router pattern.  This will print
to the console.

Parameters:
 - pointA: number representing current location of router
 - pointB: number representing desintation location of router
 - totalCol: number representing the amount of collumns in a 
 	     square array of routers
Output:
(.) represents the current router location
[.] represents the destination router location
{.} represents if the current location is the destination location

Example Use:
printDots(0, 7, 3);

Result:

	(.)	 .	 .
	 .	 . 	 .
	 .	[.]	 .
	 
Warning!
 - Router layout must be a square
 - The counting of arrays starts at 0, not 1
*****************************************************/
int printDots(int pointA, int pointB, int totalCol) {
	for (int i = 0; i < totalCol; i++) {
		printf("\n\t\t");
		for (int j = 0; j < totalCol; j++) {
		
			if (i*(totalCol) + j == pointA && i*(totalCol) + j == pointB) printf("{.}\t");
			else if (i*(totalCol) + j == pointA) printf("(.)\t");
			else if (i*(totalCol) + j == pointB) printf("[.]\t");
			else printf(" .\t");
		
		}
	}
	printf("\n");
	return 0;
}

/************************************************************
Print Arrow is a method used to print an arrow
based on the direction that it is given.  
Theres not a lot to this one

Parameters:
 - result: Result from the routing algoritm
 	   Must be between 0 and 4
 	   
Output:
 - If 0: print an up arrow
 - If 1: print a down arrow
 - If 2: print a right arrow
 - If 3: print a left arrow
 - If 4: print Go Local!

Warning!
 - anything but 0-4 will print "Invalid"
*************************************************************/
int printArrow(int result) {
	printf("\n");
	switch (result) {
		case 0:
			printf("\n\t\t\tGo North!");
			printf("\n\t\t\t    ^");
			printf("\n\t\t\t    |");
			printf("\n\t\t\t    |\n");
			break;
		case 1:
			printf("\n\t\t\tGo South!");
			printf("\n\t\t\t    |");
			printf("\n\t\t\t    |");
			printf("\n\t\t\t    v\n");
			break;
		case 2:
			printf("\n\t\t\tGo East!");
			printf("\n\t\t\t  --->\n");
			break;
		case 3:
			printf("\n\t\t\tGo West!");
			printf("\n\t\t\t  <---\n");
			break;
		case 4:
			printf("\n\t\t\tGo Local!\n");
			break;
		default:
			printf("\nInvalid\n");
			break;
	
	}
	return 0;
}

/*********************************************************
Printing is a method that combines printDots and and printArrow
This will also print some additional information to help with 
the demonstration

Parameters: 
 - The parameters are the same for the routingAlgorithm
   Look above for the listed paramters
 
Output:
 - Will print the array of routers along with the
   direction that the routing algorithm has chosen

Warning!
 - Must be able to work with the routing algorithm and its specifications
   Look above for those warnings
*********************************************************/
int printing(int data[], bool fill[], int totalCol, int vth) {
	printf("\n");
	printDots(data[4], data[2], totalCol);
	printf("\n\tThe BufferCount is: \t%d", data[1]);
	printf("\n\tThe Info is: \t\t%d", data[6]);
	printf("\n\tNorth Router is full: \t%s", fill[0] ? "True" : "False");
	printf("\n\tSouth Router is full: \t%s", fill[1] ? "True" : "False");
	printf("\n\tEast Router is full: \t%s", fill[2] ? "True" : "False");
	printf("\n\tWest Router is full: \t%s", fill[3] ? "True" : "False");
	
	int result = routingAlgorithm(data, fill, totalCol, vth);
	printArrow(result);
	printf("\n");
	printf("------------------------------------------------------------\n");
	return 0;
}

/************************************************************
The main method will run the printing method for demonstration
If you are using this, you can change the vth and the totalCol 
for your specifiactions

If you want to add more examples, jsut copy the existing format
for setting up data and fill
Just make sure to add a number to the variable
C is stupid...
************************************************************/
int main() {
	printf("(.) Indicates the Current Router Location\n");
	printf("[.] Indicates the Destination Router Location\n");
	printf("{.} Indicates shows if the information is at the desired Location\n");
	int totalCol = 3;
	int vth = 127;
	
	
	int data[7] = {0, 6, 7, 3, 6, 7, 50};
	// {foback, buffCount, routerDest, DSNDest, routerCurr, DSNCurr, info}
	bool fill[4] = {false, false, false ,false};
	// {nf, sf, ef, wf};
	printing(data, fill, totalCol, vth);
	
	int data2[7] = {0, 6, 6, 3, 7, 7, 50};
	// {foback, buffCount, routerDest, DSNDest, routerCurr, DSNCurr, info}
	bool fill2[4] = {false, false, false ,false};
	// {nf, sf, ef, wf};
	printing(data2, fill2, totalCol, vth);
	
	int data3[7] = {0, 6, 8, 3, 6, 7, 50};
	// {foback, buffCount, routerDest, DSNDest, routerCurr, DSNCurr, info}
	bool  fill3[4] = {false, false, false ,false};
	// {nf, sf, ef, wf};
	printing(data3, fill3, totalCol, vth);
	
	
	int data4[7] = {0, 6, 0, 3, 6, 7, 50};
	// {foback, buffCount, routerDest, DSNDest, routerCurr, DSNCurr, info}
	bool fill4[4] = {false, false, false ,false};
	// {nf, sf, ef, wf};
	printing(data4, fill4, totalCol, vth);
	
	int data5[7] = {0, 6, 6, 3, 0, 7, 50};
	// {foback, buffCount, routerDest, DSNDest, routerCurr, DSNCurr, info}
	bool fill5[4] = {false, false, false ,false};
	// {nf, sf, ef, wf};
	printing(data5, fill5, totalCol, vth);
	
	int data6[7] = {0, 6, 2, 3, 0, 0, 50};
	// {foback, buffCount, routerDest, DSNDest, routerCurr, DSNCurr, info}
	bool fill6[4] = {false, false, false ,false};
	// {nf, sf, ef, wf};
	printing(data6, fill6, totalCol, vth);
	
	int data7[7] = {0, 6, 0, 3, 2, 7, 50};
	// {foback, buffCount, routerDest, DSNDest, routerCurr, DSNCurr, info}
	bool fill7[4] = {false, false, false ,false};
	// {nf, sf, ef, wf};
	printing(data7, fill7, totalCol, vth);
	
	int data8[7] = {0, 6, 0, 3, 5, 7, 50};
	// {foback, buffCount, routerDest, DSNDest, routerCurr, DSNCurr, info}
	bool fill8[4] = {false, false, false ,false};
	// {nf, sf, ef, wf};
	printing(data8, fill8, totalCol, vth);
	
	int data9[7] = {0, 6, 0, 3, 8, 7, 50};
	// {foback, buffCount, routerDest, DSNDest, routerCurr, DSNCurr, info}
	bool fill9[4] = {false, false, false ,false};
	// {nf, sf, ef, wf};
	printing(data9, fill9, totalCol, vth);
	
	int data10[7] = {0, 1, 7, 3, 1, 7, 500};
	// {foback, buffCount, routerDest, DSNDest, routerCurr, DSNCurr, info}
	bool fill10[4] = {false, false, false ,false};
	// {nf, sf, ef, wf};
	printing(data10, fill10, totalCol, vth);
	
	int data11[7] = {0, 6, 5, 3, 3, 7, 500};
	// {foback, buffCount, routerDest, DSNDest, routerCurr, DSNCurr, info}
	bool fill11[4] = {false, false, false ,false};
	// {nf, sf, ef, wf};
	printing(data11, fill11, totalCol, vth);
	
	int data12[7] = {0, 6, 5, 3, 5, 7, 500};
	// {foback, buffCount, routerDest, DSNDest, routerCurr, DSNCurr, info}
	bool fill12[4] = {false, false, false ,false};
	// {nf, sf, ef, wf};
	printing(data12, fill12, totalCol, vth);
	
	int data13[7] = {0, 6, 5, 3, 1, 7, 200};
	// {foback, buffCount, routerDest, DSNDest, routerCurr, DSNCurr, info}
	bool fill13[4] = {false, false, true ,false};
	// {nf, sf, ef, wf};
	printing(data13, fill13, totalCol, vth);
	
	int data14[7] = {0, 6, 1, 3, 5, 7, 200};
	// {foback, buffCount, routerDest, DSNDest, routerCurr, DSNCurr, info}
	bool fill14[4] = {false, false, false , true};
	// {nf, sf, ef, wf};
	printing(data14, fill14, totalCol, vth);
	
	int data15[7] = {0, 6, 1, 3, 5, 7, 200};
	// {foback, buffCount, routerDest, DSNDest, routerCurr, DSNCurr, info}
	bool fill15[4] = {true, false, false ,false};
	// {nf, sf, ef, wf};
	printing(data15, fill15, totalCol, vth);
	
	int data16[7] = {0, 6, 5, 3, 1, 7, 200};
	// {foback, buffCount, routerDest, DSNDest, routerCurr, DSNCurr, info}
	bool fill16[4] = {false, true, false ,false};
	// {nf, sf, ef, wf};
	printing(data16, fill16, totalCol, vth);
	
	int data17[7] = {0, 6, 5, 3, 1, 7, 200};
	// {foback, buffCount, routerDest, DSNDest, routerCurr, DSNCurr, info}
	bool fill17[4] = {false, false, true , true};
	// {nf, sf, ef, wf};
	printing(data17, fill17, totalCol, vth);
	
	int data18[7] = {0, 6, 5, 3, 1, 7, 200};
	// {foback, buffCount, routerDest, DSNDest, routerCurr, DSNCurr, info}
	bool fill18[4] = {true, true, false ,false};
	// {nf, sf, ef, wf};
	printing(data18, fill18, totalCol, vth);	
													
	return 0;
}
