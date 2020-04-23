/* =================================================================== *
*                    Grocery Store Receipt Generator                   *
*                                                                      *
*  Name: Lab2.cs                                                       *
*                                                                      *
*  Written by: Brianna Drew - February 2020                            *
*                                                                      *
*  Purpose: To read in text files representing shopping lists from a   *
*  grocery store and use a "database" containing the store's products  *
*  to generate a receipt for the shopping list.                        *
*                                                                      *
*  Description of Parameters:                                          *
*  argv[1] - name of text file to read in (shopping list, e.g.         *
*                                           item_list_1.txt)           *
*                                                                      *
*  Subroutines/Libraries Required:                                     *
*      See include statements.                                         *
* ==================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Grocery Record Structure
struct sku
{
	char description[40];			// name of product
	char unitofmeasure[15];			// unit product is measured in (e.g. lb, g, etc.)
	float price;			// price of product
} sku_record;

char entire_row1[1];
int item, qty;		// item id number and item quantity
int  i=0, r;
float total;       // cost per item * units of item
float Cost;        // overall calculated price
int itemcount;     // keeps track of # of items being processed
int itemflag = 0;  // flag to trip if item doesn't exist

// main program
int main (int argc, char *argv[])
{
   system("clear");  // clear screen
   FILE *fp; // file to open (text file of requested items)
   char *filename; // file name of requested items file
   char ch;

  // Check if a filename has been specified in the command and warn and halt program
  if (argc < 2)  // if no file name has been specified for the shopping list parameter...
   {
		    // output error statement
        printf("\033[1;31m");
        printf("\n******* WARNING*******");
        printf("\nMissing Filename!!\n");
        printf("Format should be: @loki$ executable argument\n");
        printf("example: @loki$ ./Lab2 item_list_1.txt\n\n");
        printf("\033[0m");
        return(1);
   }
   else
  {
        filename = argv[1];  // filename variable gets first argument
   }

  // Set up Date and Time
  time_t timer;
  time_t offset = 5;
  char buffer[26];
  struct tm* tm_info;
  setenv("TZ", "GMT+5", 1);
  tzset();
  timer = time(NULL);
  tm_info = localtime(&timer);
  strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);

    // set up variables to allow reading of grocery database file
    int num;
    int i;
    FILE *infile; // to be grocery database file
    struct sku input;
    int RecSize= sizeof(struct sku);

    infile = fopen ("grocery_items.db", "r");   // open grocery database file
    if (infile == NULL)    // if the grocery database file cannot be found...
	{
		fprintf(stderr, "\nError opening file\n");    // output error statement
		exit (1);   // exit program
	}
    fseek(infile, 0L, SEEK_END);

    // calculating the size of the file
    long int res = ftell(infile);

    // read item/qty file for processing
    fp = fopen(filename, "r");		// open the shopping list text file
    if (fp == NULL)			// if the gicen shopping list text file does not exist...
        {
					// output error statement
           printf("\033[1;31m");
           printf("\n******* WARNING*******");
           printf("\n%s%s%s","File: '",filename,"' does not exist!!\n");
           printf("Format should be: @loki$ executable argument\n");
           printf("example: @loki$ ./Lab2 item_list_1.txt\n\n");
           printf("\033[0m");
           exit(1);    // exit program
        }
    // print receipt
    printf ("\n=======================================================================================\n"  );
    printf ("\n                           Welcome to Brianna's Food Mart!\n"  );
    printf("%69c");
    puts(buffer);
    printf ("\n                                              Unit of        unit     # of     Extended");
    printf ("\nDescription                                   Measure        Price    items    price"  );
    printf ("\n--------------------------                    -------        -----    -----    --------"  );

    while ((r = fscanf(fp, "%d %d", &item, &qty)) != EOF)    // go through entire shopping list
        {
            if (r == 2) {
                fseek(infile,(item+1)*RecSize-RecSize,SEEK_SET); // seek product in database of the current item from shopping list
                fread(&input, sizeof(struct sku), 1, infile);     // reads in product info from database for the current item from shopping list

                // output error message if item on request list is not in Grocery DataBase and carry on ....
                if (((item+1)*RecSize) > res)
                {
                   itemflag = 1;
                   printf("\033[1;31m");
                   printf("\n%s%-3d%s","---------------------------WARNING!! Item:", (item), " doesn't exist----------------------------");
                   printf("\033[0m");
                   itemflag = 0;
                }
                else    // if the item does exist in the database, print item details
                {
                    total=qty*input.price; 		// calculate total cost of current item
                    printf ("\n%-46s%-15s$%-8.2f%-9d$%-8.2f", input.description, input.unitofmeasure, input.price, qty, total);
                    itemcount ++;
                    Cost=Cost+total;    // increase total cost
                }
          }
       }

    printf("\n%87s","--------");
    printf("\n%79s$%-4.2f\n","Total:  ", Cost);  // print total cost
    printf("\n                 Thank you for purchasing %d items from our fine store.\n", itemcount);    // print number of items purchased
    printf ("                                   Have a good day eh?");
    printf ("\n=======================================================================================\n\n\n"  );
    fclose(fp);			// close shooping list text file
    fclose (infile);				// close grocery database file
    return 0;
}
