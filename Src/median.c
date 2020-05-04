#include "median.h"


int arr[MEDIAN_LEN];

int current = 0;

void median_add(int num){
	arr[current] = num;
	current++;
}

int median_current_size(){
	return current + 1;
}

int median_value(){
	// bubble sort
	for(int i = 0; i < MEDIAN_LEN; i++){
		for(int j = 0; j < MEDIAN_LEN; j++) {
			if(arr[i] > arr[j]){
				int tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
	current = 0;
	return arr[MEDIAN_LEN/2];
}
