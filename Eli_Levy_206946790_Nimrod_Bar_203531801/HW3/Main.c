#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 255
void cleanBuffer();
void checkAllocation(const void* p);
void removeNewLine(char* s);
void charToBinary(char c);
void printStringBinary(char* buffer, long fileSize);
int compareBits(const void* e1, const void* e2);
int compareBSearch(const void* e1, const void* e2);
char modifyBits(char ch);

void cleanBuffer()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}
void removeNewLine(char* s) {
	char* p;
	if ((p = strchr(s, '\n')) != NULL)
		*p = 0;
}
void checkAllocation(const void* p)
{
	if (p == NULL)
	{
		printf("\nNot enough memory!Exiting.");
		exit(1);
	}
}
void charToBinary(char c)
{
	int i;
	for (i = 7; i >= 0; i--)
	{
		printf("%d", (c >> i) & 1);
	}
}
void printStringBinary(char* buffer, long fileSize)
{
	for (int i = 0; i < fileSize; i++)
	{
		charToBinary(buffer[i]);
		printf("\n");
	}
}

int compareBSearch(const void* e1, const void* e2)
{
	const unsigned int seqLen = *(const unsigned int*)e1;
	const unsigned char buffer = *(const unsigned char*)e2;
	int counter = 0 ,max = 0;
	
	for (int i = 7; i >= 0; i--)
	{
		int bit = (buffer >> i) & 1;

		if (bit == 1)
		{
			counter++;
			if (counter > max)
				max = counter;
		}
		else
		{
			counter = 0;
		}
	}
	if (max == seqLen)
	{
		return 0;
	}
	if (max != seqLen)
	{
		printf("Cannot find such sequence!");
		exit(0);
	}
	return seqLen - max;
}
int compareBits(const void* e1, const void* e2)
{
	const unsigned char* c1 = (const unsigned char*)e1;
	const unsigned char* c2 = (const unsigned char*)e2;
	int counterBitX = 0, maxCountBitX = 0, counterBitY = 0, maxCountBitY = 0;

		for (int i = 7; i >= 0; i--)
		{
			int bitX = (*c1 >> i) & 1;
			if (bitX == 1)
			{
				counterBitX++;
				if (counterBitX > maxCountBitX)
				{
					maxCountBitX = counterBitX;
				}
			}
			else
			{
				counterBitX = 0;
			}
			int bitY = (*c2 >> i) & 1;
			if (bitY == 1)
			{
				counterBitY++;
				if (counterBitY > maxCountBitY)
				{
					maxCountBitY = counterBitY;
				}
			}
			else
			{
				counterBitY = 0;
			}
		}
		return maxCountBitX - maxCountBitY;
}
char modifyBits(char ch) {
	ch |= 0x55555555;
	return ch;
}

void main()
{
	unsigned char str[LEN];
	FILE* f;

	// Q9
	printf("Please enter a string:");
	fgets(str, LEN, stdin);

	// Q10
	int len = strlen(str);
	removeNewLine(str);
	str[len - 1] = '\0';

	// Q11 Write to binary file.
	f = fopen("1.dat", "wb");
	if (f == NULL)
	{
		printf("Failed opening the file for writing. Exiting\n");
		return 1;
	}
	if (fwrite(str, sizeof(char), len, f) != len)
	{
		printf("Error writing to the file. Exiting\n");
		fclose(f);
		return 1;
	}
	fclose(f);

	// Q12 Read from binary file.
	f = fopen("1.dat", "rb");
	if (f == NULL)
	{
		printf("Failed opening the file for reading. Exiting\n");
		exit(1);
	}
	fseek(f, 0, SEEK_END);
	long fileSize = ftell(f);
	rewind(f);
	char* buffer = (char*)malloc(sizeof(char) * fileSize);
	checkAllocation(buffer);

	if (fread(buffer, sizeof(char), fileSize, f) != fileSize)
	{
		printf("Error reading from the file. Exiting\n");
		free(buffer);
		exit(1);
	}
	fclose(f);

	// Q13 + Q14
	printf("The bits in the array, BEFORE sorting:\n");
	printStringBinary(buffer, fileSize);

	// Q15
	printf("The bits in the array, AFTER sorting:\n");
	qsort(buffer, fileSize - 1, sizeof(char), compareBits);

	// Q16 + Q17
	printStringBinary(buffer, fileSize);

	// Q18 + Q19
	printf("And the string, AFTER the sort is:\n");
	printf("%s\n", buffer);

	// Q20+Q21
	unsigned int seqLen;
	printf("Please enter the sequence length to search for (unsigned number): ");
	scanf_s("%d", &seqLen);

	unsigned char* found = (unsigned char*)bsearch(&seqLen, buffer, fileSize - 1, sizeof(char), compareBSearch);
	// Q22
	if (*found != NULL)
	{
		int index = (int)(found - buffer);
		printf("The character '%c' has been found! \n", *found);
		buffer[index] = modifyBits(buffer[index]);
		//Q23
		printf("The bits in the array, AFTER THE BITS UPDATE, are:\n");
		printStringBinary(buffer, fileSize);
		//Q24
		printf("And the string AFTER THE BITS UPDATE is:\n%s \n", buffer);
	}
	free(buffer);
}