#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>


int cache_miss;
int cache_hit;
int memory_write;
int memory_read;
struct cachesim** cache;
unsigned long int count;


typedef struct cachesim{
	int check;
	unsigned long int point;
	unsigned long int tag;
}cachesim;


cachesim** createcache(int set_range,int assoc) {
	int p;
	int q;
	cache = (cachesim**)malloc(set_range * sizeof(cachesim*));

	for(p = 0; p < set_range; p++) {
		cache[p]=(cachesim*)malloc((assoc) * sizeof(cachesim));
	}

	for(p = 0; p < set_range; p++) {
		for(q = 0; q < assoc; q++) {
			cache[p][q].check=0;
		}
	}
	return cache;
}

void fifo_pref_ww(unsigned long int index_of_tag, unsigned long int index_of_set, int assoc){
	
	int p;
	int q;
	int min;

	for(p = 0; p < assoc; p++){
		if(cache[index_of_set][p].check == 0){
			memory_read++;
			count++;
			cache[index_of_set][p].tag = index_of_tag;
			cache[index_of_set][p].point = count;
			cache[index_of_set][p].check = 1;
			return;
		}else{
			if(cache[index_of_set][p].tag == index_of_tag){
				return;
			}
				
			if(p == (assoc-1)){
				memory_read++;			
				min = 0;
				for(q = 0; q < assoc; q++){
					if(cache[index_of_set][q].point <= cache[index_of_set][min].point){
						min = q;
					}	
				}

				cache[index_of_set][min].tag = index_of_tag;
				count++;
				cache[index_of_set][min].point = count;
				cache[index_of_set][min].check = 1;
				return;
			}	
		}	
	}

	printf("error");
	return;
}

void fifo_pref_rr(unsigned long int index_of_tag, unsigned long int index_of_set, int assoc){
	
	int p;
	int q;
	int min;

	for(p = 0; p < assoc; p++){
		if(cache[index_of_set][p].check == 0) {
			memory_read++;
			count++;	
			cache[index_of_set][p].tag = index_of_tag;
			cache[index_of_set][p].point = count;
			cache[index_of_set][p].check = 1;
			return;
		}else {
			if(cache[index_of_set][p].tag == index_of_tag) {
				return;
			}	
			if(p == (assoc-1)){
				memory_read++;
				min = 0;
				for(q = 0; q < assoc; q++){
					if(cache[index_of_set][q].point <= cache[index_of_set][min].point){
						min = q;
					}	
				}
				cache[index_of_set][min].tag = index_of_tag;
				count++;
				cache[index_of_set][min].point = count;
				cache[index_of_set][min].check = 1;
				return;
			}	
		}	
	}
	printf("error");
	return;
}	
	
void fifo_pref_write(unsigned long int index_of_tag,unsigned long int index_of_set, int assoc, unsigned long int n_index_tag, unsigned long int n_index_set){
	
	int p;
	int q;
	int min;

	for(p = 0; p < assoc; p++){
		if(cache[index_of_set][p].check == 0){
			cache_miss++;
			memory_read++;
			memory_write++;
			count++;
			cache[index_of_set][p].check = 1;
			cache[index_of_set][p].tag = index_of_tag;
			cache[index_of_set][p].point = count;
			fifo_pref_ww(n_index_tag,n_index_set,assoc);
			return;
		}else{
			if(cache[index_of_set][p].tag == index_of_tag){
				cache_hit++;
				memory_write++;
				return;
			}	
			if(p == (assoc - 1)){
				cache_miss++;
				memory_read++;
				memory_write++;
				min = 0;
				for(q = 0; q < assoc; q++){
					if(cache[index_of_set][q].point <= cache[index_of_set][min].point){
						min = q;
					}	
				}
				
				cache[index_of_set][min].tag = index_of_tag;
				count++;
				cache[index_of_set][min].point = count;
				cache[index_of_set][min].check = 1;
				fifo_pref_ww(n_index_tag, n_index_set,assoc);
				return;
			}	
		}	
	}
	printf("error");
	return;
}

void fifo_pref_read(unsigned long int index_of_tag, unsigned long int index_of_set, int assoc, unsigned long int n_index_tag, unsigned long int n_index_set) {
	
	int p;
	int q;
	int min;

	for(p = 0; p < assoc; p++){
		if(cache[index_of_set][p].check == 0){
			count++;
			cache_miss++;
			memory_read++;			
			cache[index_of_set][p].tag = index_of_tag;
			cache[index_of_set][p].point = count;
			cache[index_of_set][p].check = 1;
			fifo_pref_rr(n_index_tag,n_index_set,assoc);
			return;
		}else{
			if(cache[index_of_set][p].tag == index_of_tag){
				cache_hit++;
				return;
			}	
			if(p == (assoc-1)){
				cache_miss++;
				memory_read++;
				min=0;
				for(q = 0; q < assoc; q++){
					if(cache[index_of_set][q].point <= cache[index_of_set][min].point){
						min = q;
					}	
				}
				
				cache[index_of_set][min].tag = index_of_tag;
				count++;
				cache[index_of_set][min].point = count;
				cache[index_of_set][min].check = 1;
				fifo_pref_rr(n_index_tag,n_index_set,assoc);
				return;
			}
		}		
	}
	printf("error");
	return;		
}

void fifo_simulator_read(unsigned long int index_of_tag, unsigned long int index_of_set, int assoc) {

	int p;
	int q;
	int min;

	for(p = 0; p < assoc; p++) {
		if(cache[index_of_set][p].check == 0) {
			count++;
			cache_miss++;
			memory_read++;
			cache[index_of_set][p].tag = index_of_tag;
			cache[index_of_set][p].point = count;
			cache[index_of_set][p].check = 1;
			return;
		}else {
			if(cache[index_of_set][p].tag == index_of_tag){
				cache_hit++;
				return;
			}	
			if(p == (assoc - 1)){
				cache_miss++;
				memory_read++;
				min = 0;
				for(q = 0; q < assoc; q++){
					if(cache[index_of_set][q].point <= cache[index_of_set][min].point){
						min = q;
					}	
				}
				count++;
				cache[index_of_set][min].check = 1;
				cache[index_of_set][min].tag = index_of_tag;
				cache[index_of_set][min].point = count;
				return;
			}
		}	
	}
	printf("error");
	return;
}

void fifo_simulator_write(unsigned long int index_of_tag, unsigned long int index_of_set, int assoc) {

	int p;
	int q;
	int min;

	for(p=0;p<assoc;p++){
		if(cache[index_of_set][p].check == 0){
			count++;
			cache_miss++;
			memory_read++;
			memory_write++;			
			cache[index_of_set][p].tag = index_of_tag;
			cache[index_of_set][p].point = count;
			cache[index_of_set][p].check = 1;			
			return;
		}else{			
			if(cache[index_of_set][p].tag == index_of_tag){
				cache_hit++;
				memory_write++;
				return;
			}	
			if(p == (assoc - 1)){
				cache_miss++;
				memory_read++;
				memory_write++;
				min = 0;
				for(q = 0; q < assoc; q++) {
					if(cache[index_of_set][q].point <= cache[index_of_set][min].point){
						min = q;
					}	
				}
				count++;
				cache[index_of_set][min].check = 1;
				cache[index_of_set][min].tag = index_of_tag;
				cache[index_of_set][min].point = count;
				return;
			}	
		}			
	}
	printf("error");
	return;
}



void lru_pref_ww(unsigned long int index_of_tag,unsigned long int index_of_set,int assoc){
	
	int p;
	int q;
	int min;

	for(p = 0; p < assoc; p++) {
		if(cache[index_of_set][p].check == 0){
			memory_read++;
			count++;
			cache[index_of_set][p].tag = index_of_tag;
			cache[index_of_set][p].point = count;
			cache[index_of_set][p].check = 1;
			return;

		}else{
			if(cache[index_of_set][p].tag == index_of_tag){
				return;
			}
				
			if(p == (assoc-1)){
				
				memory_read++;
				min = 0;
				for(q = 0; q < assoc; q++){
					if(cache[index_of_set][q].point <= cache[index_of_set][min].point){
						min = q;
					}	
				}
				
				cache[index_of_set][min].tag = index_of_tag;
				count++;
				cache[index_of_set][min].point = count;
				cache[index_of_set][min].check = 1;
				return;
			}	
		}		
	}

	printf("error");
	return;
}

void lru_pref_rr(unsigned long int index_of_tag,unsigned long int index_of_set,int assoc){
	
	int p;
	int q;
	int min;

	for(p = 0; p < assoc; p++){
		if(cache[index_of_set][p].check == 0){
			memory_read++;
			count++;
			
			cache[index_of_set][p].tag = index_of_tag;
			cache[index_of_set][p].point = count;
			cache[index_of_set][p].check = 1;
			return;

		}else{
			if(cache[index_of_set][p].tag == index_of_tag){	
				return;
			}	
			if(p == (assoc-1)){
				memory_read++;
				min = 0;
				for(q =0; q < assoc; q++){
					if(cache[index_of_set][q].point <= cache[index_of_set][min].point){
						min = q;
					}	
				}
				
				cache[index_of_set][min].tag = index_of_tag;
				count++;
				cache[index_of_set][min].point = count;
				cache[index_of_set][min].check = 1;
				return;
			}
		}		
	}
	printf("error");
	return;
}
		
void lru_pref_write(unsigned long int index_of_tag,unsigned long int index_of_set,int assoc,unsigned long int n_index_tag,unsigned long int n_index_set) {
	
	int p;
	int q;
	int min;

	for(p = 0; p < assoc; p++) {
		if(cache[index_of_set][p].check == 0){
			cache_miss++;
			memory_read++;
			memory_write++;
			count++;
			
			cache[index_of_set][p].tag = index_of_tag;
			cache[index_of_set][p].point = count;
			cache[index_of_set][p].check = 1;
			fifo_pref_ww(n_index_tag, n_index_set, assoc);
			return;
		}else{
			if(cache[index_of_set][p].tag == index_of_tag){
				cache_hit++;
				memory_write++;
				count++;
				cache[index_of_set][p].point = count;
				return;
			}	
			if(p == (assoc-1)){
				cache_miss++;
				memory_read++;
				memory_write++;
				min = 0;
				for(q = 0; q < assoc; q++){
					if(cache[index_of_set][q].point <= cache[index_of_set][min].point){
						min = q;
					}	
				}
				
				cache[index_of_set][min].tag = index_of_tag;
				count++;
				cache[index_of_set][min].point = count;
				cache[index_of_set][min].check = 1;
				fifo_pref_ww(n_index_tag, n_index_set, assoc);
				return;
			}
		}	
	}

	printf("error");
	return;	
}

void lru_pref_read(unsigned long int index_of_tag,unsigned long int index_of_set,int assoc,unsigned long int n_index_tag,unsigned long int n_index_set){

	int p;
	int q;
	int min;

	for(p = 0; p < assoc; p++){
		if(cache[index_of_set][p].check == 0){
			cache_miss++;
			memory_read++;
			count++;
			cache[index_of_set][p].tag = index_of_tag;
			cache[index_of_set][p].point = count;
			cache[index_of_set][p].check = 1;
			fifo_pref_rr(n_index_tag,n_index_set,assoc);
			return;
		}else{
			if(cache[index_of_set][p].tag == index_of_tag){
				cache_hit++;
				count++;
				cache[index_of_set][p].point = count;
				return;
			}	
			if(p == (assoc-1)){
				cache_miss++;
				memory_read++;
				min = 0;
				for(q = 0; q < assoc; q++){
					if(cache[index_of_set][q].point <= cache[index_of_set][min].point){
						min = q;
					}	
				}
				cache[index_of_set][min].check = 1;
				cache[index_of_set][min].tag = index_of_tag;
				count++;
				cache[index_of_set][min].point = count;
				fifo_pref_rr(n_index_tag, n_index_set, assoc);
				return;
			}
		}	
	}

	printf("error");
	return;
}

void lru_simulator_read(unsigned long int index_of_tag,unsigned long int index_of_set,int assoc) {

	int p;
	int q;
	int min;

	for(p = 0; p < assoc; p++){
		if(cache[index_of_set][p].check == 0){
			cache_miss++;
			memory_read++;
			count++;
			cache[index_of_set][p].tag = index_of_tag;
			cache[index_of_set][p].point = count;
			cache[index_of_set][p].check = 1;
			return;

		}else {
			if(cache[index_of_set][p].tag == index_of_tag){
				cache_hit++;
				count++;
				cache[index_of_set][p].point = count;
				return;
			}
				
			if(p == (assoc-1)){
				cache_miss++;
				memory_read++;
				min=0;
				for(q = 0; q < assoc; q++){
					if(cache[index_of_set][q].point <= cache[index_of_set][min].point){
						min = q; 
					}	
				}
				cache[index_of_set][min].tag=index_of_tag;
				count++;
				cache[index_of_set][min].point=count;
				cache[index_of_set][min].check=1;
				
				return;
			}
		}	
	}
	printf("error");
	return;

}

void lru_simulator_write(unsigned long int index_of_tag,unsigned long int index_of_set,int assoc){

	int p;
	int q;
	int min;

	for(p = 0; p < assoc; p++){
		if(cache[index_of_set][p].check == 0){
			cache_miss++;
			memory_read++;
			memory_write++;
			count++;
			
			cache[index_of_set][p].tag = index_of_tag;
			cache[index_of_set][p].point = count;
			cache[index_of_set][p].check = 1;
			return;
		}else{
			if(cache[index_of_set][p].tag == index_of_tag){
				cache_hit++;
				memory_write++;
				count++;
				cache[index_of_set][p].point = count;
				return;
			}
				
			if(p == (assoc-1)){
				cache_miss++;
				memory_read++;
				memory_write++;
				min = 0;
				for(q = 0; q < assoc; q++){
					if(cache[index_of_set][q].point <= cache[index_of_set][min].point){
						min = q;
					}	
				}
				
				cache[index_of_set][min].tag = index_of_tag;
				count++;
				cache[index_of_set][min].point = count;
				cache[index_of_set][min].check = 1;
				return;
			}
		}	
	}

	printf("error");
	return;
}

void free_cache(int set_range, int assoc){

	int p;
	int q;
	for(p = 0; p < set_range; p++) {
		for(q = 0; q < assoc; q++) {
			cache[p][q].tag = 0;
			cache[p][q].point = 0;
			cache[p][q].check = 0;
		}
	}
	
	memory_read = 0;
	memory_write = 0;
	cache_miss = 0;
	cache_hit = 0;
	count = 0;
}


int main(int argc, char** argv){

	int cachesize = atoi(argv[1]);
	int blocksize = atoi(argv[4]);
	int a;                                   // assoc;
	int set_range;
	int assoc;

	int b;
	int s;

	char read_or_write;
	unsigned long int hex_address;
	unsigned long int new_hex_address;
	unsigned long int block_mask;
	
	unsigned long int index_of_tag;
	unsigned long int index_of_set;
	unsigned long int n_index_tag;
	unsigned long int n_index_set;
	

	//fifo statement
	if(argv[3][0]=='f'){

		FILE* fl;
  		fl=fopen(argv[5],"r");

  		if(fl==NULL){
			printf("tracefile does not exist\n");
			return 0;
  		}
		

		//assoc and set_range for (dir, full,set asso)
		if(argv[2][0]=='d') {                      //direct map
			assoc = 1;
			set_range = cachesize/blocksize;
		}else if(argv[2][5]!=':') {                //fully assoc
			set_range = 1;
			assoc = cachesize/blocksize;
		}else {                                    //n way assoc
			sscanf(argv[2],"assoc:%d",&a);
			assoc = a;
			set_range = cachesize/blocksize/a;
		}

		b = log(blocksize)/log(2);
		s = log(set_range)/log(2);

		block_mask = ((1<<s)-1);

		
		cache = createcache(set_range,assoc);


		//with prefetch read file
		while(fscanf(fl, "%*x: %c %lx", &read_or_write, &hex_address)==2){
			index_of_set = (hex_address>>b)&block_mask;
			index_of_tag = hex_address>>(b+s);
			
			if(read_or_write=='R'){
				fifo_simulator_read(index_of_tag, index_of_set, assoc);
			}else if(read_or_write=='W'){
				fifo_simulator_write(index_of_tag, index_of_set, assoc);
			}
		}
		fclose(fl);

		fl = fopen(argv[5],"r");

		if(fl == NULL){
			printf("tracefile does not exist\n");
			return 0;
		}
		
		printf("Prefetch 0\n");
		printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n",memory_read,memory_write,cache_hit,cache_miss);

		//free cache
		free_cache(set_range, assoc);

		
		while(fscanf(fl, "%*x: %c %lx", &read_or_write, &hex_address) == 2){

			index_of_set = (hex_address>>b)&block_mask;
			index_of_tag = hex_address>>(b+s);

			new_hex_address = hex_address + blocksize;
			n_index_set = (new_hex_address>>b)&block_mask;
			n_index_tag = new_hex_address>>(b+s);

			if(read_or_write=='R'){
				fifo_pref_read(index_of_tag, index_of_set, assoc, n_index_tag, n_index_set);

			}else if(read_or_write=='W'){
				fifo_pref_write(index_of_tag, index_of_set, assoc, n_index_tag, n_index_set);
			}
		}

		printf("Prefetch 1\n");
		printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n",memory_read,memory_write,cache_hit,cache_miss);
	
	//lru statement
	}else if(argv[3][0]=='l') {  
	
		FILE* fl;
		fl=fopen(argv[5],"r");

		if(fl==NULL){
			printf("tracefile does not exist\n");
			return 0;
		}

		//assoc and set_range for (dir, full,set asso)
		if(argv[2][0]=='d'){                       //direct map

		assoc=1;
		set_range=cachesize/blocksize;

		}else if(argv[2][5]!=':'){                  //fullassoc
		set_range=1;
		assoc=cachesize/blocksize;
		}else{                                       //n way assoc

			sscanf(argv[2],"assoc:%d",&a);
			assoc=a;
			set_range=cachesize/blocksize/a;
		}

		b = log(blocksize)/log(2);
		s = log(set_range)/log(2);

		block_mask=((1<<s)-1);
		
		cache = createcache(set_range,assoc);


		//without prefetch read file
		while(fscanf(fl, "%*x: %c %lx", &read_or_write, &hex_address)==2) {

			index_of_set=(hex_address>>b)&block_mask;
			index_of_tag=hex_address>>(b+s);

			if(read_or_write=='R'){
				lru_simulator_read(index_of_tag,index_of_set,assoc);
			}else if(read_or_write=='W'){
				lru_simulator_write(index_of_tag,index_of_set,assoc);
			}
		}

		fclose(fl);

		fl = fopen(argv[5],"r");
		if(fl == NULL){
			printf("tracefile does not exist\n");
			return 0;
		}

		printf("Prefetch 0\n");
		printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n",memory_read,memory_write,cache_hit,cache_miss);

		//free cache
		free_cache(set_range, assoc);


		//with prefetch read file
		while(fscanf(fl, "%*x: %c %lx", &read_or_write, &hex_address)==2) {
		
			index_of_set = (hex_address>>b) & block_mask;
			index_of_tag = hex_address>>(b+s);

			new_hex_address = hex_address + blocksize;
			n_index_set = (new_hex_address>>b) & block_mask;
			n_index_tag = new_hex_address>>(b+s);

			if(read_or_write=='R'){	
				lru_pref_read(index_of_tag,index_of_set,assoc,n_index_tag,n_index_set);
			}else if(read_or_write=='W'){
				lru_pref_write(index_of_tag,index_of_set,assoc,n_index_tag,n_index_set);
			}

		}

		printf("Prefetch 1\n");
		printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n",memory_read,memory_write,cache_hit,cache_miss);

	}else {
		printf("replacement policy is neither fifo or lru");
	}

	return 0;
}