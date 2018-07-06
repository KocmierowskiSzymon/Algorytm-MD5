#include <iostream>

#include <stdio.h>

#include <string>

#include <string.h>

#include <vector>

#include <algorithm>

#include <stdint.h>

using namespace std;





/*----------------------    Zmienne globalne    ------------------------------*/


	uint32_t a0 = 0x67452301;

	uint32_t b0 = 0xefcdab89;

	uint32_t c0 = 0x98badcfe;

	uint32_t d0 = 0x10325476;

	uint32_t A = a0, 
			 B = b0,
			 C = c0,
			 D = d0,
			 T, M [16];


/*---------------------- tablica z wstepnie obliczonymi wartoscimi na runde-------------*/

 	 uint32_t K [64] = { 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    	                0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        	            0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
            	        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
                		0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
                	    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        	            0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
                	    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        	            0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
                	    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
       	       	    	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
             	    	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
            	        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        	            0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
             	    	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
          	        	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };
                    
/*------------------------------------------------------------------------------*/




/*------------------ tablica z wartosciami przesuniec na runde -------------------*/
    
 	 uint32_t s [64]= {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
 					  5, 9,  14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5, 9,  14, 20, 
					  4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
					  6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };
				  
/*---------------------------------------------------------------------------------*/

				  
/*----------------------------------------------------------------------------*/
  
  
  
  
  
/*--------------- Funkcje zwracajace wyniki operacji logicznych na bitach ---------------*/

	uint32_t F(uint32_t B, uint32_t C, uint32_t D)
	{
    	return (B & C) | (~B & D);         
	}



	uint32_t G(uint32_t B, uint32_t C, uint32_t D)
	{
		return (B & D) | (C & ~D);         
	}



	uint32_t H(uint32_t B, uint32_t C, uint32_t D)
	{
		return B ^ C ^ D;         
	}



	uint32_t I(uint32_t B, uint32_t C, uint32_t D)
	{
    	return C ^ (B | ~D);         
	}

/*--------------------------------------------------------------------------------------*/





/*---------------------- Funkcje rundy funckji hashujacej md5 ------------------------*/

/*  W kazedj rundzie wykonywanie sa nastepujace kroki:

	-Wykonanie operacji logicznej na bitach
	
	-Dodanie wartosci z odpowiedniej komorki wiadomosci
	
	-Dodanie odpowiedniej wartosci z tabklicy z wstepnie obliczonymi wartoscimi
	
	-Przesuniecie cykliczne w lewo o konkretna wartosc z tablicy z wartosciami przesuniec
	
	-Dodanie nastepnego bloku bitow   												  */



	void round_F(uint32_t &A,uint32_t &B, uint32_t &C, uint32_t &D,
				 const uint32_t Mi, const uint32_t Ki, uint32_t S)
	{
	
	    A += F(B, C, D);
    
	    A += Mi;
    
	    A += Ki;
    
	    A = (A << S) ^ (A >> 32 - S);
    
	    A += B;
    
	}



	void round_G (uint32_t &A,uint32_t &B, uint32_t &C, uint32_t &D, 
				  const uint32_t Mi, const uint32_t Ki, uint32_t S)
	{
	    A += G(B, C, D);
    
	    A += Mi;
    
	    A += Ki;
    
	    A = (A << S) ^ (A >> 32 - S);
    
	    A += B;
    
	}



	void round_H (uint32_t &A,uint32_t &B, uint32_t &C, uint32_t &D,
				  const uint32_t Mi, const uint32_t Ki, uint32_t S)
	{
	
	    A += H(B, C, D);
    
	    A += Mi;
    
	    A += Ki;
    
	    A = (A << S) ^ (A >> 32 - S);
    
	    A += B;
    
	}



	void round_I (uint32_t &A, uint32_t &B, uint32_t &C, uint32_t &D,
				  const uint32_t Mi, const uint32_t Ki, uint32_t S)
	{
	    A += I(B, C, D);
    
	    A += Mi;
    
	    A += Ki;
    
    	A = (A << S) ^ (A >> 32 - S);
    
    	A += B;
    
	}

/*-----------------------------------------------------------------------------------*/


/*------------------------- funkcja ustawiajace dlugosc wiadomosci -----------------*/

void set_data_length(uint64_t &length) 
{
	
	/*----- zmienna pomocnicza -----*/
	
    	uint8_t leng [8];
    
    /*------------------------------*/
    
    	/* W tej petli sa przepisywane ostatnie 8 bitow z zmiennej odpowiadajacej
	
		   za dlugosc wiadomosci do kolrjnych komorek w tablicy, po czym nastepuje
	   
		    przeuniecie logiczne w prawo bitow w zmiennej dlugosci               */
    
    	for (int j = 0; j < 8; j++)
    	{
    	
    	    leng [j] = (uint8_t) length;
        
    	    if (j != 7) 
    	    {
        	
				length >>= 8;
			
			}
		
		}
	
	/*---------------------------*/
	
	
	/*-----------------------------------*/
	
		/* W tych petlach jest wykonywana operacja logiczna OR
	
		   na 8 bitach przedostatniego i ostatniego segmentu bitiow 
	   
		   wiadomosci, a odpowiednim segmentem bitow dlugosci wiadomosci,
	   
		   po czym bity wiadomosci sa przesowane legicznie w lewo o 8  */
	   
	 
	
    	for (int j = 0; j < 4; j++)
    	{
    	
    	    M[14] |= leng [j];
        
   		    if (j != 3) 
    	    {
    	    	
				M [14] <<= 8;
			
			}
		
    	}
    
    	for (int j = 4; j < 8; j++)
    	{
    	
    	    M [15] |= leng [j];
        
    	    if (j != 7) 
			{
				
				M [15] <<= 8;
				
			}
		
    	}
    
    /*-----------------------------------*/
}

/*------------ Funkcja zmieniajaca oznaczenia segmentow bitow ----------*/

void move()
{
	
	 T = D;
	 
     D = C;
     
     C = B;
     
     B = A;
     
     A = T;	
     
}

/*------------------------------------------------------------*/


/*------ Funkcja przestawiajaca bity z big endian na little endian, a little endian na big endian -------*/

void to_LE(uint32_t *tab, size_t seg32_num) 
{
	
	/*------ Zmienna pomocnicza -----*/
	
	    uint8_t temp [4];
    
    /*-------------------------------*/
    
    
    
    /*---------------------------------------------------------------------*/
    
    	/* W pierwszej petli wewnetrznej nastepuje przepisanie segmentu bitow
	
	 	   wiadomosci na 4 mniejsze segmenty bitow przez przepisywnanie 8 
	   
	 	   ostatnich bitow z segmentu 32 bitowego i przesunieciu logicznie o 8 
	   
	 	   bitow w segmencie 32 bitowym , przed druga petla wewnetrzna, dany segment 
	   
	       wiadomosci jest zerowany, w drugiej zas nastepuje wykonanie operacji 
	   
	  	   logicznej OR miedzy 8 ostatnimi bitami segmentu 32 bitowego wiadomosci,
	   
	       a 8 bitowymi segmentami przepisanymi do tablicy, po czym wykonywana 
		
  	   	   jest operacja logicznego przesuniecia bitow o 8 w lewo. Czynnosci te sa
		
	   	   powtarzane dla nastepnych segmentow wiadomosci. Zewnetrzna petla
		
		   jest zalezna od ilosci segmentow 32 zmiennej, badz tablicy na ktorej
		   
		   ma byc wykonana funkcja  */
	   
		   
    	for (int i = 0; i < seg32_num; i++)
   		{
    	
    		for (int j = 0; j < 4; j++)
        	{
        	
            	temp [j] = (uint8_t) tab [i];
            
            	if (j != 3) 
				{
				
					tab [i] >>= 8;
				
				}
			
        	}
        
 	        tab [i] = 0;
        
 	        for (int j = 0; j < 4; j++)
   	        {
        	
  	        	tab [i] |= temp [j];
 	           
  	        	if (j != 3) 
		        {
					
			    	tab [i] <<= 8;
					
			    }
				
  	        }
        
		}
	
	/*---------------------------------------------------------------*/
	
}

/*------------------------------------------------------------------------------------*/



/*--------------------------- Funkcja dodajaca jedynke bitowa do wiadomosci -----------------*/

void add_one(int length, int row, int column)
{
	
	/*--------------------------------------------*/
	
		/* W petli nastepuje sprawdzenie w ktory z 32 bitowych segmentow
	
		   wiadomosci jest ostatni */
	
		for(int i = 0; i < 16; i++)
		{
		
			if( (length > i *4)  && (length <= ((i + 1) * 4)) ) row = i;
		
		}
	
	/*-----------------------------------------------*/
	
	
	
	/*------------------------------------------------------*/
	
		/* W zmiennej column wyliczane jest, ktory segment 8 bitowy z segmentu
	
		   32 bitowego jest ostatnim segmentem wiadomosci. Jezeli ostatni segment
	   
		   w 32 bitowym segmencie jest ostatnim segmentem wiadomosci, to zwiekszamy 
	   
		   wartosc zmiennej row o 1 i zerujemy zmienna column. W kazdym innym wypadku
	   
		   zwiekszamy wartosc column o 1. Dzieki temu wiemy w ktorym miejscu 
	   
		   segmencie segmentu 32 bitowego dodamy jedynke bitowa     */
	   
	   
		column = (length - 1) % 4;
	
		if(column == 3)
		{
			
			row++;
			
			column = 0;
			
		}
	
		else
		{
			
			column++;
		
		}
	
	/*----------------------------------------------------------------*/
	
	
	
	/*----------------------------------------------------------------*/
	
		/* Tworzona jest zmienna pomocnicza one majaca wartosc 1, 
		
		   nastepnie od w zaleznosci od wyliczonej wartosci column
	   
		   nastepuje przesuniecie logiczne w lewo o konkretna liczbe bitow
		   
		   i jest wykonywana operacja logiczna OR z znalezionym wczesniej
	   
		   segmentem 32 bitowym     */
	   
	
		uint32_t one = 0x00000001;
	
		if(column==0) one <<= 31;
		
		if(column==1) one <<= 23;
	
		if(column==2) one <<= 15;
	
		if(column==3) one <<= 7;
	
		M [row] |= one;
	
	/*---------------------------------------------------------------*/
}

/*--------------------------------------------------------------------------------------*/




/*------------------------------- Algorytm MD5 -------------------------------------*/

	void md5(string message)
	{
		
		/*----------------------------------------------------------*/
		
			/* Pierwsza petla czysci tablice, w ktorej ma znajdowac sie
			   
			   znajdowac sie zapisana biotwo wiadomosc. nastepnie tworzone 
			   
			   sa zmienne pomocniczne tem, mowiacy w ktorym segmencie
			   
			   8 bitowym segmentu 32 bitowego sie znajdujemy i position, mowiacy 
			   
			   ktory segment 32 bitowy bedzie teraz zapisywany. Nastepna petla jest 
			   
			   uzalezniona od dlugosci wiadomosci i ma na celu dzieki operacji
			   
			   logicznej OR i przesunieciach logicznych w lewo o 8 przepisanie
			   
			   wiadomosci do wczesniej wyczyszczonej tablicy. Przy przechodzeniu
			   
			   do nastepnego segmentu 32 bitowego jest zmniejszany licznik petli,
			   
			   aby nie pominac ani jednego przepisania wiadomosci  */
		
		
			for(int i = 0; i < 16; i++) M [i] = 0;
	 		
			int tem = 0;
	 		
			int position = 0;
	 		
			 
			for(int i = 0; i < message.length(); i++)
	 		{
	 			
	 			if(tem < 4)
	 			{
	 				
	 				M [position] |= message [i];
	 				
					if(tem != 3) M [position] <<= 8;
	 				
					tem++;
					
				}
				
				else
				{
					
		 			tem = 0;
		 			
			 		position++;
			 		
			 		i--;
			 		
				}
				
 			}
	 
		/*--------------------------------------------------------*/ 
	 	
	 	
	 	
	 	
	 	
	 	/*------------------ Zmienne pomocnicze ------------------*/
	 	
			int row = 0;
			
			int column = 0;
			
		/*---------------------------------------------------------*/
	
		
		
		
		
		/*------------------ Sprawdzenie czy wiadomosc jest niepusta --------------*/
		
			/* Jesli nie jest to ma sie wykonac funkcja dodanie jedynki */
			
			if(!(message.length() == 0))
			{	
		
				add_one(message.length(), row, column);
				
			} 
			
		/*---------------------------------------------------------------------------*/	
		
		
		
		/*----------------------- Zmienna pomocnicza -----------------------------*/
			
	 		uint64_t length = message.length() * 8;
	 		
	 	/*------------------------------------------------------------------------*/
		 
		 
		 
		 
		/*------- Uzycie funkcji ustawienia dlugosci wiadomosci i przestawiania endian-ow -------------*/	
	 		
			set_data_length(length);
			
	 		to_LE(M, 16);
	 		
	 	/*----------------------------------------------------------------------------------------------*/	
	
		
		
		
		/*----------------------------------------------------------------------------------------*/
		
			/* W czterych petlach wykonywane sa rundy na odpowiednich
			   
			   segmentach wiadomosci wstepnie przyjetych przez algorytm
				
			   wraz z wykorzystanie kolejnych wrtosci z tablicy z wstepnie
				
			   obliczonymi wartosciami i tablicy z wartosciami przesuniec 
				
			   na runde. Po kazdej rundzie wywolywana jest funkcja move  */
		
		
			for(int i = 0; i < 16; i++)
			{
				
				round_F(A, B, C, D, M [i], K [i], s [i]);
				
				move();
				
			}
		
		
		
			for(int i = 0; i < 16; i++)
			{
				
				round_G(A, B, C, D, M [(5 * i + 1) % 16], K [i + 16], s [i + 16]);
				
				move();
				
			}
		
			for(int i=0; i<16; i++)
			{
				
				round_H(A, B, C, D, M [(3 * i + 5) % 16], K [i + 32], s [i + 32]);
				
				move();
				
			}
		
			for(int i=0; i<16; i++)
			{
				
				round_I(A, B, C, D, M [(i * 7) % 16], K [i + 48], s [i + 48]);
				
				move();
				
			}
			
		/*--------------------------------------------------------------------------------------------*/
		
		
		
		
		/*--------------------------------------------------------------*/
		
			/* Dodanie do zmiennych wyliczonych wartosci z rund
			
			   i uzycie na nich funkcji do zamiany endian-ow*/
		
	   		
			   
			a0 += A;
			
	    	b0 += B;
	    	
	    	c0 += C;
	    	
	    	d0 += D; 
	    
	    	
			to_LE(&a0, 1);
			
	    	to_LE(&b0, 1);
	    	
	    	to_LE(&c0, 1);
	    	
	    	to_LE(&d0, 1);
	    	
	    /*----------------------------------------------------------*/
	    
		
		std :: cout << "Md5 hash: ";
		
	    std :: cout << hex << a0 << " " << b0 << " " << c0 << " " << d0 <<endl;
	    
	    system("PAUSE");
	    
	}

/*------------------------------------------------------------------------------------*/



/*---------------------------- Glowna funkcja programu ------------------------------------*/

	int main()
	{
		
		/*----------------------------------------------------*/
		
			/* Wprowadznie  wprowadzenie wiadomosci i wywolania funkcji hashujacej md5 */
	
	
			string message;
		
			cout << "Enter a message: "; 
		
			cin >> message;
		
			md5(message);
		
			return 0;
		
	}

/*-----------------------------------------------------------------------------------*/	
