//#include<stdio.h>
//#include<conio.h>
//#include<math.h>
//#include<stdlib.h>
//#include"list.h"
//#include"implicant.h"
//#include"CONSTANTS.h"
////=================================================================================================================
//
//
//struct minterm
//{
//	char *literal;							//
//	int bits;
//	int checked;
//	unsigned int InDecimal;
//	int alive;
//	int coveredCount;
//	int dCare;
//	List CoveredTerms;
//    Position p;
//};
////=================================================================================================================
//struct chart
//{
//	int *covered;
//	unsigned int decimal;
//	int alive;
//};
////=================================================================================================================
//typedef struct chart Chart;                      //create alias of struct chart
//typedef struct minterm Minterm;                  //create alias
//static Minterm *pImplicant = NULL;               //make primeIMplicant global and static
//static int pIndex = 0 , pSize = 0;
//static int INPUTS = 0;
////=================================================================================================================
//int CountLiterals(char *path)
//{
//	FILE  *file;
//	int counter = 0;
//	char c;
//    file = fopen(path, "r");
//    if (file == NULL)
//	{
//		printf("File doesn't exist\n");
//		exit(EXIT_FAILURE);
//	}
//	else
//	{
//		while((c = fgetc( file ))  != EOF)
//		{
//			if( c ==';')
//				counter++;
//		}
//		fclose(file);
//	}
//
//	return counter;
//}
////=================================================================================================================
//void CopyLiteral(char *file, Minterm *min , int minSize)
//{
//	int i = 0;
//	int j = 0;
//	char c;
//	FILE *fp;
//	fp = fopen(file,"r");
//
//	for(j = 0 ; j < minSize ; j++)		//set Dont care terms to 0
//		min[i].dCare = FALSE;
//
//	j = 0;
//
//	while((c = fgetc( fp ) ) != EOF )
//	{
//		if(c == '\n')
//			continue;
//		if(c == ' ')
//			continue;
//		
//		
//
//		if(c == 'd' || c == 'D')
//		{
//			min[i].dCare = TRUE;
//			continue;
//		} 
//
//
//		if( c == ';')
//		{
//			min[i].literal[j] = '\0';  // place null character at the end of literal array
//
//
//			i++;
//			j = 0;
//		}
//		else if(c == 'x' || c == 'X')
//		{
//			min[i].literal[j] = '-';
//			j++;
//		}
//		else
//		{
//			min[i].literal[j] = c;
//			j++;
//		}
//	}
//
//	fclose(fp);
//
//	
//}
////=================================================================================================================
//void setBitsNDeci(Minterm *min , int count)
//{
//	int bit = 0;
//    float deci = 0;
//	int i = 0, j = 0;
//	int power = INPUTS;
//	float val = 2.0;
//	for(j = 0 ; j < count ; j++)
//	{
//		power = INPUTS-1;                                 //renew power for next calcutation
//		bit = 0;										  //make these 0 for next calcuation
//	    deci = 0;
//		
//		for(i = 0 ; i < INPUTS ; i++)
//		{
//			if(min[j].literal[i] == '1')
//			{
//				deci += pow(val,power);
//				bit++;
//			}
//			else if(min[j].literal[i] == 'x' || min[j].literal[i] == 'X')
//			{
//				val = 3.0;
//				deci += pow(val,power);
//				bit++;
//				val = 2.0;
//			}
//			power--;
//
//		}
//
//		min[j].alive = TRUE;
//		min[j].bits = bit;
//		min[j].checked = FALSE;
//		min[j].InDecimal = (unsigned int)deci;
//		min[j].CoveredTerms = MakeEmpty(NULL);          //create list
//		min[j].p = Header(min[j].CoveredTerms);
//
//		
//		Insert(deci,min[j].CoveredTerms,min[j].p);      //insert decimal value in list
//		min[j].p = Advance(min[j].p);                   // advance the pointer()
//						
//	}
//}
////=================================================================================================================
//void setBits(Minterm *min, int count)
//{
//	int bit = 0;
//	int i = 0, j = 0;
//	for(j = 0 ; j < count ; j++)
//	{
//        bit = 0;										//make these 0 for next calcuation
//		for(i = 0 ; i < INPUTS ; i++)
//		{
//			if(min[j].literal[i] == '1')
//			{
//				bit++;
//			}
//
//		}
//
//		min[j].alive = TRUE;
//		min[j].bits = bit;
//		min[j].checked = FALSE;
//				
//	}
//}
////=================================================================================================================
//int getRequiredMemory(Minterm *min , int arraySize)
//{
//	int count = 0 , i = 0 , j = 0;
//	if(min == NULL)		return;
//	for(i = 0 ; i < arraySize - 1 ; i++)
//	{
//		for(j = i+1 ; j < arraySize ; j++)
//		{
//			if(abs((min[i].bits - min[j].bits)) == 1)
//			{
//				if(checkDashAlignment(min[i].literal , min[j].literal) == TRUE)
//					if(isOneBitChange(min[i].literal,min[j].literal) == TRUE)
//						count++;
//			}
//				
//		}
//	}
//	return count;
//}
////=================================================================================================================
//void combineTerms(Minterm *min,int minSize, Minterm *nextMin )
//{
//	int index = 0 , i = 0 , j = 0 , k = 0;
//	for(i = 0 ; i < minSize - 1 ; i++)
//	{
//		for(j = i+1 ; j < minSize ; j++)
//		{
//			if(abs((min[i].bits - min[j].bits)) == 1)
//			{
//
//				if(checkDashAlignment(min[i].literal,min[j].literal) == TRUE)
//				{
//				
//					if(isOneBitChange(min[i].literal,min[j].literal) == FALSE)
//						continue;
//					min[i].checked = TRUE;                                     //mark those term as checked which are combined
//					min[j].checked = TRUE;
//
//					for(k = 0 ; k < INPUTS ; k++)
//					{
//						if(min[i].literal[k] == min[j].literal[k])
//							nextMin[index].literal[k] = min[i].literal[k];
//						else
//							nextMin[index].literal[k] = '-';
//					}
//					k = INPUTS;
//					nextMin[index].literal[k] = '\0';
//				
//					nextMin[index].CoveredTerms = MakeEmpty(NULL);                      //create empty list
//					nextMin[index].p = Header(nextMin[index].CoveredTerms);             //make list pointer
//			
//
//					if(!IsEmpty(min[i].CoveredTerms))                                   //these lines move covered terms in next step
//					{
//				 		min[i].p = Header(min[i].CoveredTerms);
//						do
//						{
//							min[i].p = Advance(min[i].p);                  //advance the pointer
//							Insert(Retrieve(min[i].p),nextMin[index].CoveredTerms,nextMin[index].p);
//							nextMin[index].p = Advance(nextMin[index].p);
//						}while(!IsLast( min[i].p, min[i].CoveredTerms ));
//					}
//
//					if(!IsEmpty(min[j].CoveredTerms))
//					{
//						min[j].p = Header(min[j].CoveredTerms);
//						do
//						{
//							min[j].p = Advance(min[j].p);
//							Insert(Retrieve(min[j].p),nextMin[index].CoveredTerms,nextMin[index].p);
//							nextMin[index].p = Advance(nextMin[index].p);
//						}while(!IsLast( min[j].p, min[j].CoveredTerms ));
//					}
//
//					index++;
//				}
//			}
//				
//		}
//	}
//}
////=================================================================================================================
//int checkDashAlignment(char *a, char *b)
//{
//	int status = FALSE , i = 0 , count = 0 , state1 = 0 , state2 = 0;
//	for(i = 0 ; i < INPUTS ; i++)
//	{
//		if(a[i] == '-')		state1++;
//		if(b[i] == '-')		state2++;
//		if(a[i] == '-' && b[i] == '-')    count++;
//	}
//	
//	if(state1 == state2)
//		if(count == state2)
//			status = TRUE;
//
//	return status;
//}
////=================================================================================================================
//void AllocateLiterals(Minterm *min , int minSize)
//{
//	int i = 0;
//	for(i = 0 ; i < minSize ; i++)
//	{
//		min[i].literal = (char *)malloc(sizeof(char) * (INPUTS + 1));
//		if(min[i].literal == NULL)
//		{
//			printf("Memory Allocation fails");
//			exit(EXIT_FAILURE);
//		}
//	}
//}
////=================================================================================================================
//int isSimilar(char *a, char *b)
//{
//	int yes = FALSE, i = 0;
//
//	for(i = 0 ; i < INPUTS ; i++)
//		if(a[i] == b[i])	yes++;
//
//	if(yes == INPUTS)	yes = TRUE;
//	else yes = FALSE;
//
//	return yes;
//}
////=================================================================================================================
//int isOneBitChange(char *a, char *b)
//{
//	int i = 0 , yes = 0;
//	for(i = 0 ; i < INPUTS ; i++)
//	{
//		if(a[i] != b[i])	yes++;
//	}
//	return (yes == 1 ? TRUE : FALSE);
//
//}
////=================================================================================================================
//void removeSimilar(Minterm *min , int arraySize)
//{
//	int i = 0 , j = 0;
//	for(i = 0 ; i < arraySize - 1 ; i++)
//	{
//		for(j = i+1 ; j < arraySize ; j++)
//		{
//			if(abs((min[i].bits - min[j].bits)) == 0)                        //only process those which has only same bits
//			{
//				if(isSimilar(min[i].literal , min[j].literal) == TRUE)
//					min[j].alive = FALSE;
//			}
//
//		}
//	}
//			
//}
////=================================================================================================================
//int getPrimeImplicantSize(Minterm *min , int minSize)
//{
//	int count = 0, i = 0;
//	for(i = 0 ; i < minSize ; i++)
//	{
//		if(min[i].alive == TRUE)
//		{
//			if(min[i].checked == FALSE)
//				count++;
//		}
//	}
//	return count++;
//}
////=================================================================================================================
//void getPrimeImplicant(Minterm *min , int minSize)
//{
//	Position p;
//	int i = 0 , k = 0;
//	for(i = 0 ; i < minSize ; i++)
//	{
//		if(min[i].alive == FALSE)
//			continue;
//		if(min[i].checked == TRUE)
//			continue;
//		if(! IsEmpty(min[i].CoveredTerms))
//		{
//			pImplicant[pIndex].CoveredTerms = MakeEmpty(NULL);						//Make Empty List
//			pImplicant[pIndex].p = Header(pImplicant[pIndex].CoveredTerms);         //Make Pointer to this list
//
//			p = Header((min[i].CoveredTerms));
//			do
//			{
//				p = Advance(p);
//				Insert(Retrieve(p),pImplicant[pIndex].CoveredTerms,pImplicant[pIndex].p);
//				pImplicant[pIndex].p = Advance(pImplicant[pIndex].p);
//			}while(!IsLast(p,min[i].CoveredTerms));
//			
//			pImplicant[pIndex].alive = TRUE;				//set alive true
//			pImplicant[pIndex].bits = min[i].bits;
//			pImplicant[pIndex].checked = FALSE;             // checked false
//			pImplicant[pIndex].coveredCount = 0;        // coveredCount = 0 (used in chart solution)
//			for(k = 0 ; k < INPUTS ; k++)
//			{
//				pImplicant[pIndex].literal[k] = min[i].literal[k];
//			}
//			k = INPUTS ;
//			pImplicant[pIndex].literal[k] = '\0';
//			pIndex++;
//		}
//	}
//}
//void freeLiterals(Minterm *min , int minSize)
//{
//	int i = 0;
//	for(i = 0 ; i < minSize ; i++)
//		free(min[i].literal);
//}
//int getInputSize(char *path)
//{
//	FILE  *file;
//	int counter = 0;
//	char c;
//    file = fopen(path, "r");
//    if (file == NULL)
//	{
//		printf("File doesn't exist\n");
//		exit(EXIT_FAILURE);
//	}
//	else
//	{
//		while((c = fgetc( file )))
//		{
//			if(c == ' ')	continue;
//			if(c == '\n')	continue;
//			if( c ==';')	break;
//				
//			counter++;
//		}
//		fclose(file);
//	}
//
//	return counter;
//}
////=================================================================================================================
//int getChartSize(Minterm *min , int minSize)
//{
//	int i = 0 , count = 0;;
//	for(i = 0; i < minSize ; i++)
//	{
//		if((min[i].alive == FALSE) || (min[i].dCare == TRUE))
//			continue;
//
//		count++;
//	}
//	return count;
//}
////=================================================================================================================
//void setChart(Chart *c , int cSize , Minterm *min, int minSize)
//{
//	int i = 0 , j = 0 , k = 0 ;
//	unsigned int reterivedVal = 0;
//	Position p;
//	for(i = 0; i < cSize ; i++)
//	{
//		c[i].covered = (int *)malloc(sizeof(int) * pSize);        //prime Implicant size is global so direct acess it
//		if(c[i].covered == NULL)
//		{
//			printf("Memory Allocation fail");
//			exit(EXIT_FAILURE);
//		}
//	}
//	
//	i = 0 ;
//
//	for(j = 0 ; j < minSize ; j++)								          // initiliazing chart Array or copy literals in chart
//	{
//		if((min[j].alive == FALSE) || (min[j].dCare == TRUE))			  //Dont copy minterm to chart if it is dead or it is dont care
//			continue;
//
//		c[i].decimal = min[j].InDecimal;
//		c[i].alive = TRUE;
//		for(k = 0 ; k < pSize ; k++ )
//			c[i].covered[k] = 0;
//		i++;
//	}
//
//	for(j = 0 ; j < pSize ; j++)
//	{
//		p = Header(pImplicant[j].CoveredTerms);                           // set pointer to prime Implicant coverd Terms
//		
//		do
//		{
//			p = Advance(p);
//			reterivedVal = Retrieve(p);
//			for(i = 0 ; i < cSize ; i++)
//			{
//				if(reterivedVal == c[i].decimal)
//					c[i].covered[j] = 1;
//			}
//			 
//		}while(!IsLast(p,pImplicant[j].CoveredTerms));
//		
//		
//	}
//}
////=================================================================================================================
//int isSolvedChart(Chart *c , int cSize)
//{
//	int done = 0 , i = 0;
//	
//	for(i = 0 ; i < cSize ; i++)
//	{
//		if(c[i].alive == TRUE)
//			done++;
//	}
//
//	return done;                              //if done == 0 than our chart is solved 
//}
////=================================================================================================================
//int findMax(Minterm *min , int mSize)
//{
//	int i = 0 , max = 0 , index = 0;
//	
//	for(i = 0 ; i < mSize ; i++)                  //these line assign max value to max
//	{
//		if(min[i].checked == TRUE)			continue;
//
//		if(min[i].coveredCount > max)
//			max = min[i].coveredCount;
//	}
//	
//	for(i = 0 ; i < mSize ; i++)                  //these line find index of max
//	{
//		if(min[i].checked == TRUE)			continue;
//
//		if(max == min[i].coveredCount)
//		{
//			index = i ;
//			break;
//		}
//	}
//	return index;
//
//}
////=================================================================================================================
//void solveChart(Chart *c , int cSize)
//{
//	int i = 0, j = 0, single = 0;
//	Position p;
//
//	for(i = 0 ; i < cSize ; i++)
//	{
//		for(j = 0 ; j < pSize ; j++)                     //checking for those which are covered by one 
//		{
//			if(c[i].covered[j] == 1)
//				single++;
//		}
//	
//		if(single == 1)									//those which are covered once qualified for final answer
//		{
//			for(j = 0 ; j < pSize ; j++)
//			{
//				if(c[i].covered[j] == 1)
//				{
//					pImplicant[j].checked = TRUE;
//					break;
//				}
//			}
//			c[i].alive = FALSE;
//			
//		}
//		single = 0;
//	}
//
//
//	
//	
//	for(j = 0 ; j < pSize ; j++)                                //This part eliminate those terms which are coverd once
//	{
//		if(pImplicant[j].checked == FALSE)	continue;		    // Only process those which are qualified for final answer
//
//		p = Header(pImplicant[j].CoveredTerms);                 // set pointer to prime Implicant coverd Terms
//		
//		do
//		{
//			p = Advance(p);
//			
//			for(i = 0 ; i < cSize ; i++)
//			{
//				if(c[i].alive == FALSE)    continue;
//
//				if(Retrieve(p) == c[i].decimal)
//					c[i].alive = FALSE;
//			}
//			 
//		}while(!IsLast(p,pImplicant[j].CoveredTerms));
//	}
//
//
//
//	while(isSolvedChart(c,cSize) > 0)                              //loop until chart is not solved
//	{
//		for(j = 0 ; j < pSize ; j++)
//		{
//			if(pImplicant[j].checked == TRUE)		continue;          //if prime implicant is checked dont process it
//
//			for(i = 0 ; i < cSize ; i++)
//			{
//				if(c[i].alive == FALSE)			continue;              // if chart member is not alive dont process it
//
//				if(c[i].covered[j] == TRUE)
//					pImplicant[j].coveredCount++;
//			}
//		}
//
//
//
//
//		i = findMax(pImplicant,pSize);                       // i contains the index of prime Implicant which is coverd by max after 1st step
//		pImplicant[i].checked = TRUE;
//		p = Header(pImplicant[i].CoveredTerms);               // set pointer to prime Implicant coverd Terms
//		
//		do                                                    // these lines find remaning terms which are covered by more than once
//		{
//			p = Advance(p);
//			
//			for(i = 0 ; i < cSize ; i++)
//			{
//				if(c[i].alive == FALSE)    continue;
//
//				if(Retrieve(p) == c[i].decimal)
//					c[i].alive = FALSE;
//			}
//			 
//		}while(!IsLast(p,pImplicant[j].CoveredTerms));
//	}
//
//
//}
////=================================================================================================================
//void freeChart(Chart *c , int chartSize)
//{
//	int i = 0 ;
//	for(i = 0 ; i < chartSize ; i++)
//	{
//		free(c[i].covered);
//	}
//}
////=================================================================================================================
//void PrintList( const List L )
//{
//    Position P = Header( L );
//
//    if( IsEmpty( L ) )
//        printf( "Empty list\n" );
//    else
//    {
//        do
//        {
//            P = Advance( P );
//            printf( "%u ", Retrieve( P ) );
//        } while( !IsLast( P, L ) );
//    }
//}
////=================================================================================================================
//void PrintInfo()
//{
//	printf( "Dear User, Let me tell you How to Use the Program. The Program has 2 keywords:-\n\n"
//			"[project name] "
//			"[file name]");
//		printf("\n\nOpen your comand Prompt\n"
//			"enter : [your Projectname.exe] spacce [filename] \n"
//			   "Example:- MinterSolver.exe input.txt \n\n");
//        printf("Make sure that you have entered the correct path and name of input file \n");
//}