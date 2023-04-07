#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapify(int arr[], int N, int i)
{
    // variables for max heap
	int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

	// if root < left child, note
    if (left < N && arr[left] > arr[largest])
        largest = left;
 
	// if root < right child, note
    if (right < N && arr[right] > arr[largest])
        largest = right;
 
	// if largest has changed, swap
    if (largest != i) {
        int temp = arr[i];
		arr[i] = arr[largest];
		arr[largest] = temp;

		// repeat until heap is proper
        heapify(arr, N, largest);
    }
}
 
void heapSort(int arr[], int n)
{
    // heapify the array
	for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
 
	// root of max heap is removed and placed at the end of array
    for (int i = n - 1; i >= 0; i--) {
		int temp = arr[0];
		arr[0] = arr[i];
		arr[i] = temp;
        
		// heapify again (root becomes largest)
        heapify(arr, i, 0);
    }
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	if (l < r) {
		int m = (l + r) / 2;
		// sorts left side
		mergeSort(pData, l, m);
		// sorts right side
		mergeSort(pData, m + 1, r);
		
		int i, j, k;
		// # terms in left side
		int n1 = m - l + 1;
		// # terms in right side
		int n2 = r - m;

		// create separate arrays to sort
		int *L = (int*) malloc(n1 * sizeof(int));
		int *R = (int*) malloc(n2 * sizeof(int));
		extraMemoryAllocated = (n1 + n2);

		// copy data into new arrays
		for (i = 0; i < n1; i++)
			L[i] = pData[l + i];
		for (j = 0; j < n2; j++)
			R[j] = pData[m + 1+ j];

		i = 0;
		j = 0;
		k = l;
		while (i < n1 && j < n2) {
			if (L[i] <= R[j]) {
				pData[k] = L[i];
				i++;
			}
			else {
				pData[k] = R[j];
				j++;
			}
			k++;
		}

		// copy remaining elements
		while (i < n1) {
			pData[k] = L[i];
			i++;
			k++;
		}
		while (j < n2) {
			pData[k] = R[j];
			j++;
			k++;
		}

		// free allocated memory
		free(L);
		free(R);
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}