/*
* This program solve Minterm upto 32 bit variable. It is Quine McCluskey Algrothim implementaion
* This algrothim has two part 
    -> 1 : Finding prime Implicants 
	-> 2 : Finding Essential prime implicants
* In the end find final Answer from step 1 and 2
*/
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"
#include "implicant.h"
#include "CONSTANTS.h"
//=================================================================================================================
struct minterm
{
	char *literal;							// contain literal
	int bits;								// total no of ones in literal
	int checked;							// check wthere this literal make pair or not
	unsigned long InDecimal;			    // contain decimal value of literal
	int alive;								// set literal to died which is elimamited or occurs more thann one
	int coveredCount;						// used for solving chart
	int dCare;								// dCare = 1 means don't care  
	List CoveredTerms;					    // 
    Position p;
};
//=================================================================================================================
struct chart
{
	int *covered;							// 
	unsigned int decimal;
	int alive;
};
//=================================================================================================================
typedef struct chart Chart;                      // create alias of struct chart
typedef struct minterm Minterm;                  // create alias
static Minterm *pImplicant = NULL;               // make primeIMplicant global and static
static int pIndex = 0 , pSize = 0;
static int INPUTS = 0;							 // Contain length of input (for 4 bit literal INPUTAS = 4)
//=================================================================================================================
int CountLiterals(char *path)
{
	FILE  *file;
	int counter = 0;
	char c;
    file = fopen(path, "r");
    if (file == NULL)
	{
		printf("File doesn't exist\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		while((c = fgetc( file ))  != EOF)
		{
			if( c ==';' || c == ',')
				counter++;
		}
		fclose(file);
	}

	return counter;
}
//=================================================================================================================
void CopyLiteral(char *file, Minterm *min , int minSize)
{
	int i = 0;
	int j = 0;
	char c;
	FILE *fp;
	fp = fopen(file,"r");

	for(i = 0 ; i < minSize ; i++)		// set Dont care terms to 0
		min[i].dCare = FALSE;

	i = 0;

	while((c = fgetc( fp ) ) != EOF )
	{
		if(c == '\n')
			continue;
		if(c == ' ')
			continue;
		
		

		if(c == 'd' || c == 'D')
		{
			min[i].dCare = TRUE;
			continue;
		} 


		if( c == ';' || c == ',')
		{
			min[i].literal[j] = '\0';  // place null character at the end of literal array


			i++;					   // 
			j = 0;
		}
		else if(c == 'x' || c == 'X')
		{
			min[i].literal[j] = '-';
			j++;
		}
		else
		{
			min[i].literal[j] = c;
			j++;
		}
	}

	fclose(fp);

	
}
//=================================================================================================================
void setBitsNDeci(Minterm *min , int count)
{
	int bit = 0;
    double deci = 0;									  // counts decimal value
	int i = 0, j = 0;
	int power = INPUTS;									  // set power = input size
	float val = 2.0;
	for(j = 0 ; j < count ; j++)
	{
		power = INPUTS - 1;                               // renew power for next calcutation
		bit = 0;	   									  // make these 0 for next calcuation
	    deci = 0;
		
		for(i = 0 ; i < INPUTS ; i++)
		{
			if(min[j].literal[i] == '1')
			{
				deci += (float)pow(val,power);              // binary to deci conversion
				bit++;										// increments bit
			}
			else if(min[j].literal[i] == 'x' || min[j].literal[i] == 'X') 
			{
				val = 3.0;   // for dont care bits change base value to generate some random deci as we know that deci 
				deci += (float)pow(val,power);            // has no effcet on final ans only take part in chart solving
				// bit++;
				val = 2.0;
			}
			power--;

		}

		min[j].alive = TRUE;
		min[j].bits = bit;
		min[j].checked = FALSE;
		min[j].InDecimal = (ElementType)deci;
		min[j].CoveredTerms = MakeEmpty(NULL);          // create empty list 
		min[j].p = Header(min[j].CoveredTerms);			// set pointer to header node

		
		Insert((ElementType)deci,min[j].CoveredTerms,min[j].p);      // insert decimal value in list
		min[j].p = Advance(min[j].p);                                // advance the pointer
						
	}
}
//=================================================================================================================
void setBits(Minterm *min, int count)
{
	int bit = 0;
	int i = 0, j = 0;
	for(j = 0 ; j < count ; j++)
	{
        bit = 0;										// make these 0 for next calcuation
		for(i = 0 ; i < INPUTS ; i++)
		{
			if(min[j].literal[i] == '1')
			{
				bit++;
			}

		}

		min[j].alive = TRUE;
		min[j].bits = bit;
		min[j].checked = FALSE;
				
	}
}
//=================================================================================================================
int isOneBitChange(char *a, char *b)
{
	int i = 0 , yes = 0;
	for(i = 0 ; i < INPUTS ; i++)
	{
		if(a[i] != b[i])	yes++;
	}
	return (yes == 1 ? TRUE : FALSE);

}
//=================================================================================================================
int checkDashAlignment(char *a, char *b)
{
	int status = FALSE , i = 0 , count = 0 , state1 = 0 , state2 = 0;
	for(i = 0 ; i < INPUTS ; i++)
	{
		if(a[i] == '-')		state1++;
		if(b[i] == '-')		state2++;
		if(a[i] == '-' && b[i] == '-')    count++;
	}
	
	if(state1 == state2)
		if(count == state2)
			status = TRUE;

	return status;
}
//=================================================================================================================
int getRequiredMemory(Minterm *min , int arraySize)
{
	int count = 0 , i = 0 , j = 0;
	if(min == NULL)		return 0;
	for(i = 0 ; i < arraySize - 1 ; i++)
	{
		for(j = i + 1 ; j < arraySize ; j++)
		{
			if(abs((min[i].bits - min[j].bits)) == 1)          // differing in one bit
			{
				if(checkDashAlignment(min[i].literal , min[j].literal) == TRUE)
					if(isOneBitChange(min[i].literal,min[j].literal) == TRUE)
						count++;
			}
				
		}
	}
	return count;
}
//=================================================================================================================
void combineTerms(Minterm *min,int minSize, Minterm *nextMin )
{
	int index = 0 , i = 0 , j = 0 , k = 0;
	for(i = 0 ; i < minSize - 1 ; i++)
	{
		for(j = i + 1 ; j < minSize ; j++)
		{
			if(abs((min[i].bits - min[j].bits)) == 1)				// differing in one bit
			{

				if(checkDashAlignment(min[i].literal,min[j].literal) == TRUE)
				{
				
					if(isOneBitChange(min[i].literal,min[j].literal) == FALSE)
						continue;                       
					min[i].checked = TRUE;                                     // mark those term as checked which are combined
					min[j].checked = TRUE;

					for(k = 0 ; k < INPUTS ; k++)
					{
						if(min[i].literal[k] == min[j].literal[k])
							nextMin[index].literal[k] = min[i].literal[k];
						else
							nextMin[index].literal[k] = '-';
					}
					k = INPUTS;
					nextMin[index].literal[k] = '\0';
				
					nextMin[index].CoveredTerms = MakeEmpty(NULL);                      // create empty list
					nextMin[index].p = Header(nextMin[index].CoveredTerms);             // make list pointer
			

					if(!IsEmpty(min[i].CoveredTerms))                                   // these lines move covered terms in next step
					{
				 		min[i].p = Header(min[i].CoveredTerms);
						do
						{
							min[i].p = Advance(min[i].p);                  // advance the pointer
							Insert(Retrieve(min[i].p),nextMin[index].CoveredTerms,nextMin[index].p);
							nextMin[index].p = Advance(nextMin[index].p);
						}while(!IsLast( min[i].p, min[i].CoveredTerms ));
					}

					if(!IsEmpty(min[j].CoveredTerms))
					{
						min[j].p = Header(min[j].CoveredTerms);
						do
						{
							min[j].p = Advance(min[j].p);
							Insert(Retrieve(min[j].p),nextMin[index].CoveredTerms,nextMin[index].p);
							nextMin[index].p = Advance(nextMin[index].p);
						}while(!IsLast( min[j].p, min[j].CoveredTerms ));
					}

					index++;
				}
			}
				
		}
	}
}
//=================================================================================================================
void AllocateLiterals(Minterm *min , int minSize)
{
	int i = 0;
	for(i = 0 ; i < minSize ; i++)
	{
		min[i].literal = (char *)malloc(sizeof(char) * (INPUTS + 1));  // allocate one more char for null character
		if(min[i].literal == NULL)
		{
			printf("Memory Allocation fails");
			exit(EXIT_FAILURE);
		}
	}
}
//=================================================================================================================
int isSimilar(char *a, char *b)
{
	int yes = FALSE, i = 0;

	for(i = 0 ; i < INPUTS ; i++)
		if(a[i] == b[i])	yes++;

	if(yes == INPUTS)	yes = TRUE;
	else yes = FALSE;

	return yes;
}
//=================================================================================================================
void removeSimilar(Minterm *min , int arraySize)
{
	int i = 0 , j = 0;
	for(i = 0 ; i < arraySize - 1 ; i++)
	{
		for(j = i + 1 ; j < arraySize ; j++)
		{
			if(abs((min[i].bits - min[j].bits)) == 0)                        // only process those which has only same bits
			{
				if(isSimilar(min[i].literal , min[j].literal) == TRUE)
					min[j].alive = FALSE;
			}

		}
	}
			
}
//=================================================================================================================
int getPrimeImplicantSize(Minterm *min , int minSize)
{
	int count = 0, i = 0;
	for(i = 0 ; i < minSize ; i++)
	{
		if(min[i].alive == TRUE)
		{
			if(min[i].checked == FALSE)
				count++;
		}
	}
	return count++;
}
//=================================================================================================================
void getPrimeImplicant(Minterm *min , int minSize)
{
	Position p;
	int i = 0 , k = 0;
	for(i = 0 ; i < minSize ; i++)
	{
		if(min[i].alive == FALSE)
			continue;
		if(min[i].checked == TRUE)
			continue;
		if(! IsEmpty(min[i].CoveredTerms))
		{
			pImplicant[pIndex].CoveredTerms = MakeEmpty(NULL);						// Make Empty List
			pImplicant[pIndex].p = Header(pImplicant[pIndex].CoveredTerms);         // Make Pointer to this list

			p = Header((min[i].CoveredTerms));
			do
			{
				p = Advance(p);
				Insert(Retrieve(p),pImplicant[pIndex].CoveredTerms,pImplicant[pIndex].p);
				pImplicant[pIndex].p = Advance(pImplicant[pIndex].p);
			}while(!IsLast(p,min[i].CoveredTerms));
			
			pImplicant[pIndex].alive = TRUE;				// set alive true
			pImplicant[pIndex].bits = min[i].bits;
			pImplicant[pIndex].checked = FALSE;             // checked false
			pImplicant[pIndex].coveredCount = 0;            // coveredCount = 0 (used in chart solution)
			for(k = 0 ; k < INPUTS ; k++)
			{
				pImplicant[pIndex].literal[k] = min[i].literal[k];
			}
			k = INPUTS ;
			pImplicant[pIndex].literal[k] = '\0';
			pIndex++;
		}
	}
}
//=================================================================================================================
void freeLiterals(Minterm *min , int minSize)
{
	int i = 0;
	for(i = 0 ; i < minSize ; i++)
		free(min[i].literal);
}
//=================================================================================================================
int getInputSize(char *path)
{
	FILE  *file;
	int counter = 0;
	char c;
    file = fopen(path, "r");
    if (file == NULL)
	{
		printf("File doesn't exist\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		while((c = fgetc( file )))
		{
			if(c == ' ')			continue;
			if(c == '\n')			continue;
			if( c ==';' || c ==',')	break;
				
			counter++;
		}
		fclose(file);
	}

	return counter;
}
//=================================================================================================================
int getChartSize(Minterm *min , int minSize)
{
	int i = 0 , count = 0;;
	for(i = 0; i < minSize ; i++)
	{
		if((min[i].alive == FALSE) || (min[i].dCare == TRUE))
			continue;

		count++;
	}
	return count;
}
//=================================================================================================================
void setChart(Chart *c , int cSize , Minterm *min, int minSize)
{
	int i = 0 , j = 0 , k = 0 ;
	unsigned int reterivedVal = 0;
	Position p;
	for(i = 0; i < cSize ; i++)
	{
		c[i].covered = (int *)malloc(sizeof(int) * pSize);        // prime Implicant size is global so direct acess it
		if(c[i].covered == NULL)
		{
			printf("Memory Allocation fail");
			exit(EXIT_FAILURE);
		}
	}
	
	i = 0 ;

	for(j = 0 ; j < minSize ; j++)								          // initiliazing chart Array or copy literals in chart
	{
		if((min[j].alive == FALSE) || (min[j].dCare == TRUE))			  // Dont copy minterm to chart if it is dead or it is dont care
			continue;

		c[i].decimal = min[j].InDecimal;
		c[i].alive = TRUE;
		for(k = 0 ; k < pSize ; k++ )          // initlize chart to 0 
			c[i].covered[k] = 0;
		i++;
	}

	for(j = 0 ; j < pSize ; j++)
	{
		p = Header(pImplicant[j].CoveredTerms);                           // set pointer to prime Implicant coverd Terms
		
		do
		{
			p = Advance(p);
			reterivedVal = Retrieve(p);
			for(i = 0 ; i < cSize ; i++)
			{
				if(reterivedVal == c[i].decimal)
					c[i].covered[j] = 1;
			}
			 
		}while(!IsLast(p,pImplicant[j].CoveredTerms));
		
		
	}
}
//=================================================================================================================
int isSolvedChart(Chart *c , int cSize)
{
	int done = 0 , i = 0;
	
	for(i = 0 ; i < cSize ; i++)
	{
		if(c[i].alive == TRUE)
			done++;
	}

	return done;                              // if done == 0 than our chart is solved 
}
//=================================================================================================================
int findMax(Minterm *min , int mSize)
{
	int i = 0 , max = 0 , index = 0;
	
	for(i = 0 ; i < mSize ; i++)                  // these line assign max value to max
	{
		if(min[i].checked == TRUE)			continue;

		if(min[i].coveredCount > max)
			max = min[i].coveredCount;
	}
	
	for(i = 0 ; i < mSize ; i++)                  // these line find index of max
	{
		if(min[i].checked == TRUE)			continue;

		if(max == min[i].coveredCount)
		{
			index = i ;
			break;
		}
	}
	return index;

}
//=================================================================================================================
void solveChart(Chart *c , int cSize)
{
	int i = 0, j = 0, single = 0;
	Position p;

	for(i = 0 ; i < cSize ; i++)
	{
		for(j = 0 ; j < pSize ; j++)                     // checking for those which are covered by one 
		{
			if(c[i].covered[j] == 1)
				single++;
		}
	
		if(single == 1)									// those which are covered once qualified for final answer
		{
			for(j = 0 ; j < pSize ; j++)
			{
				if(c[i].covered[j] == 1)
				{
					pImplicant[j].checked = TRUE;
					break;
				}
			}
			c[i].alive = FALSE;
			
		}
		single = 0;
	}


	
	
	for(j = 0 ; j < pSize ; j++)                                // This part eliminate those terms which are coverd once
	{
		if(pImplicant[j].checked == FALSE)	continue;		    // Only process those which are qualified for final answer

		p = Header(pImplicant[j].CoveredTerms);                 // set pointer to prime Implicant coverd Terms
		
		do
		{
			p = Advance(p);
			
			for(i = 0 ; i < cSize ; i++)
			{
				if(c[i].alive == FALSE)    continue;

				if(Retrieve(p) == c[i].decimal)
					c[i].alive = FALSE;
			}
			 
		}while(!IsLast(p,pImplicant[j].CoveredTerms));
	}



	while(isSolvedChart(c,cSize) > 0)								   // loop until chart is not solved
	{
		for(j = 0 ; j < pSize ; j++)
		{
			if(pImplicant[j].checked == TRUE)		continue;          // if prime implicant is checked dont process it

			for(i = 0 ; i < cSize ; i++)
			{
				if(c[i].alive == FALSE)			    continue;          // if chart member is not alive dont process it

				if(c[i].covered[j] == TRUE)
					pImplicant[j].coveredCount++;
			}
		}




		i = findMax(pImplicant,pSize);                       // i contains the index of prime Implicant which is coverd by max after 1st step
		pImplicant[i].checked = TRUE;
		p = Header(pImplicant[i].CoveredTerms);              // set pointer to prime Implicant coverd Terms
		
		do                                                   // these lines find remaning terms which are covered by more than once
		{
			p = Advance(p);
			
			for(i = 0 ; i < cSize ; i++)
			{
				if(c[i].alive == FALSE)    continue;

				if(Retrieve(p) == c[i].decimal)
					c[i].alive = FALSE;
			}
			 
		}while(!IsLast(p,pImplicant[j].CoveredTerms));
	}


}
//=================================================================================================================
void freeChart(Chart *c , int chartSize)
{
	int i = 0 ;
	for(i = 0 ; i < chartSize ; i++)
	{
		free(c[i].covered);
	}
}
//=================================================================================================================
void PrintList( const List L )
{
    Position P = Header( L );

    if( IsEmpty( L ) )
        printf( "Empty list\n" );
    else
    {
        do
        {
            P = Advance( P );
            printf( "%u ", Retrieve( P ) );
        } while( !IsLast( P, L ) );
    }
}
//=================================================================================================================
void PrintInfo()
{
	printf( "Dear User, Let me tell you How to Use the Program. The Program has 2 keywords:-\n\n"
			"[project name] "
			"[file name]");
		printf("\n\nOpen your comand Prompt\n"
			"enter : [your Projectname.exe] spacce [filename] \n"
			   "Example:- MinterSolver.exe input.txt \n\n");
        printf("Make sure that you have entered the correct path and name of input file \n");
}
//=================================================================================================================
int main(int argc , char *argv[])
{
	clock_t t;				   // for calculating time 
	int j = 0 , k = 0 , i = 0;
	Minterm *Min[33];	       // Change 33 to your desired bit minterms
	int *MinSize;              // array of pointers used for dynamic allocation of array of Minterm at each index
	Chart *chartArray;		   // for dynamically allocation array of chart
	int chartSize = 0;
	char *Alpha[32] = {"A'","B'","C'","D'","E'","F'","G'","H'","I'","J'","K'","L'","M'","N'","O'","P'","Q'","R'","S'","T'",
		               "U'","V'","W'","X'","Y'","Z'","A1'","B1'","C1'","D1'","E1'","F1'"};
	char *Alpha2[32] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T",
		               "U","V","W","X","Y","Z","A1","B1","C1","D1","E1","F1"};
	if( argc != 2 ) /* argc should be 2 for correct execution */
    {
		//PrintInfo();      // unComment these if running program from cmd Prompt  
		//return;			
    }

	t = clock();
//	INPUTS = getInputSize(argv[1]);                            // File name (uncomment this if you run from terminal )
	INPUTS = getInputSize("input.txt");						   // read the length of input (comment this if you run from terminal)
	if(INPUTS > 32)
	{
		printf("\nInput Size exceeds 32 bits ");
		return;		     //terminate the program
	}
	printf("\n\n\t\t\tINPUT SIZE (%d)\n\n",INPUTS);
	printf("________________________________________________________________________________");
	//========================================================================================================//
	//                                        for Step 1                                                 
	//========================================================================================================//
	MinSize = (int *)malloc(sizeof(int) * INPUTS + 1);
	if(MinSize == NULL)
	{
		printf("Memory allocation fails\n");
		exit(EXIT_FAILURE);
	}
	MinSize[0] = CountLiterals("input.txt");
//	MinSize[0] = CountLiterals(argv[1]);							
	if(MinSize[0] > 0)
	{
		Min[0] = (Minterm *)malloc(sizeof(Minterm) * MinSize[0]);
		
		if(Min[0] == NULL)
		{
			printf("Memory Allocation fails");
			exit(EXIT_FAILURE);
		}
		AllocateLiterals(Min[0] , MinSize[0]);
//		CopyLiteral(argv[1],Min[0],MinSize[0]);                      // reading text input
		CopyLiteral("input.txt",Min[0],MinSize[0]);                 // reading text input
		setBitsNDeci(Min[0],MinSize[0]);							// Convert binary i/p to decimal and count ones
		removeSimilar(Min[0],MinSize[0]);
	}

	for(i = 1 ; i < INPUTS + 1 ; i++)
	{
		MinSize[i] = getRequiredMemory(Min[i-1],MinSize[i-1]);				   // determine next step array size
		
		Min[i] = (Minterm *)malloc(sizeof(Minterm) * MinSize[i]);			   // dynamically allocate step2 array
		if(Min[i] == NULL)
		{
			printf("Memory Allocation fails");
			exit(EXIT_FAILURE);
		}
		AllocateLiterals(Min[i] , MinSize[i]);
		combineTerms(Min[i-1],MinSize[i-1],Min[i]);							   // step1 combine terms having one bit change
		setBits(Min[i],MinSize[i]);                                            // counts bits of step2 array
		removeSimilar(Min[i],MinSize[i]);
	}

	//========================================================================================================//
	//                                  Determinig PrimeImplicant array size                                                 
	//========================================================================================================//
	for(i = 0 ; i < INPUTS + 1 ; i++)
	{
		if(Min[i] != NULL)
			pSize = pSize + getPrimeImplicantSize(Min[i] , MinSize[i]);
	}
	pImplicant = (Minterm *)malloc(sizeof(Minterm) * pSize);                    // Allocating memory for PrmieImplicant Array
	AllocateLiterals(pImplicant , pSize);
	//========================================================================================================//
	//                              Moving PrimeImplicants in PrimeImplicant Array
	//========================================================================================================//
	for(i = 0 ; i < INPUTS + 1  ; i++)
	{
		if(Min[i] != NULL)
			getPrimeImplicant(Min[i] , MinSize[i]);
	}
	//========================================================================================================//
	//                   Finding Essential prime Implicants (Final Answer)                                                 
	//========================================================================================================//
	chartSize = getChartSize(Min[0],MinSize[0]);
	if(chartSize > 0)
	{
		chartArray = (Chart *)malloc(sizeof(Chart) * chartSize); 
		setChart(chartArray,chartSize,Min[0],MinSize[0]);
		solveChart(chartArray, chartSize);
	}


	t = clock() - t ;               // calculating time difference b/t calculation

	//========================================================================================================//
	//                                  printing data                                                 
	//========================================================================================================//
	for(j = 0; j < INPUTS + 1 ; j++)
		{
			
			if(MinSize[j] == 0)		continue;
			printf("\t\t\t\tStep%d\n\n",j+1);
			for(k = 0 ; k < MinSize[j] ; k++)
			{ 
				if(Min[j][k].alive == 0)
					continue;
				printf(Min[j][k].literal);
				printf("  Ones%4d",Min[j][k].bits);
				printf("  Checked  %d",Min[j][k].checked);
				printf("  CoveredTerms : ");
				PrintList(Min[j][k].CoveredTerms);
				printf("\n");
			}
			printf("________________________________________________________________________________");
		}
	//========================================================================================================//
	//                                  print prime Implicants                                                 
	//========================================================================================================//
	if(pSize > 0)
	{
		printf("\n\t\t\t\tPrime Implicants\n\n");
		for(j = 0 ; j < pSize ; j++)
		{
			if(pImplicant[j].alive == FALSE)
				continue;
			printf(pImplicant[j].literal);
			printf("  Ones%4d",pImplicant[j].bits);
			printf("  Checked  %d",pImplicant[j].checked);
			printf("  CoveredTerms  :  ");
			PrintList(pImplicant[j].CoveredTerms);
			printf("\n");
		}
		printf("________________________________________________________________________________");
	}
	//========================================================================================================//
	//                      print Essential prime Implicants {Final Answer}                                                 
	//========================================================================================================//
	if(pSize > 0)
	{
		printf("\n\t\tEssential Prime Implicants (FINAL ANSWER)\n\n");
		for(j = 0 ; j < pSize ; j++)
		{
			if(pImplicant[j].alive == FALSE)
				continue;
			if(pImplicant[j].checked == FALSE)
				continue;

			printf(pImplicant[j].literal);
			printf("  Ones%4d",pImplicant[j].bits);
			printf("  Checked  %d",pImplicant[j].checked);
			printf("  CoverdTerms  :  ");
			PrintList(pImplicant[j].CoveredTerms);
			printf("\n");	
		}
		printf("________________________________________________________________________________");
	}
	//========================================================================================================//
	//										print Final Expression                                                 
	//========================================================================================================//
	if(pSize > 0 )	
	{
		printf("\n\t\t\t\tFinal Expression \n\n");
		for(j = 0 ; j < pSize ; j++)
		{
			if(pImplicant[j].checked == 0)				//
				continue;
			for(i = 0 ; i < INPUTS + 1 ; i++)
			{
				
				if(pImplicant[j].literal[i] == '-')		//
					continue;
				if(pImplicant[j].literal[i] == '1')
					printf(Alpha2[i]);
				else if(pImplicant[j].literal[i] == '0')
					printf(Alpha[i]);
			}
			if(j != pSize - 1)
				printf(" + ");
		}
		printf("\n\n");
	}


	printf("\nIt took me %d clicks (%f seconds)",t,((float)t)/CLOCKS_PER_SEC);


	//========================================================================================================//
	//                                  Releasing Memory                                                 
	//========================================================================================================//
	for(i = 0 ; i < INPUTS + 1; i++)
	{
		if(Min[i] != NULL)
		{
			freeLiterals(Min[i],MinSize[i]);
			free(Min[i] );
		}
	}
	if(pImplicant != NULL)	    freeLiterals(pImplicant,pSize),         free(pImplicant);
	if(chartArray != NULL)		freeChart(chartArray, chartSize),		free(chartArray);
	getchar();
	return 0;
}
//=================================================================================================================