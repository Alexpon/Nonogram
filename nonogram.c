#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_rule_and_solve(char *);
void initial_rule();
void initial_ans_map();
void rough_col_initial();
void modify_ans_format(int *);
void bfs_solve(int);
void find_next_step(int *);
int check_answer();
void write_answer(int);
int check_close_list(int *);

void init_queue();
void enqueue(int *);
int *dequeue();

void dfs_solve(int);
void push(int *);
int *pop();
void find_next_state(int *);


struct node
{
	int *map;
	struct node *next;
};
typedef struct node Node;
Node *front, *rear, *top;

int question_num = 1;
int nonogram_size = 10;
int close_size=150;

int col_rule[15][9];
int row_rule[15][9];
int ans_map[15][15];
int *close_list[150];
int close_idx=0;

int main(int argc, char *argv[]){
	if(argc != 3){
		printf("Format: ./[exe_filename] [data_file_path] [size_of_nonogram]\n");
		exit(1);
	}
	nonogram_size = atoi(argv[2]);
	read_rule_and_solve(argv[1]);

    return 0;
}

void initial_rule(){
	for (int i=0; i<15; i++){
		for(int j=0; j<9; j++){
			col_rule[i][j] = 0;
			row_rule[i][j] = 0;
		}
	}
	for (int i=0; i<50; i++){
		close_list[i]=NULL;
	}
}

void initial_ans_map(){
	for (int i=0; i<15; i++){
		for (int j=0; j<15; j++){
			ans_map[i][j] = 0;
		}
	}
}

void read_rule_and_solve(char *file){
	initial_rule();
	initial_ans_map();
	FILE *fptr = fopen(file, "r");
	char tmp[16];
	char *cut;
	int flag = 0;
	int q_num = 0;
	int rule_size = 0;
	int col_rule_num;
	int row_rule_num;
	int col_cnt = 0;
	int row_cnt = 0;
	int select = 1;
	if (fptr==NULL){
        printf ("Error in reading the file!\n");
        exit(1);
    }

    while (fgets(tmp, 16, fptr) != NULL){
    	if (tmp[0] == '$' && flag==1){
    		q_num = tmp[1]-49;	//ASCII to int
    	    bfs_solve(q_num); 
    		initial_rule();
    		rule_size = 0;
    	}
    	else if(flag==1){
    		cut = strtok(tmp, "\t");
    		col_rule_num = 1;
    		row_rule_num = 1;
    		while (cut != NULL){
    			
    			if(rule_size<nonogram_size){
    				col_rule[rule_size][col_rule_num] = atoi(cut);
    				col_rule[rule_size][0] += atoi(cut);
    				col_rule_num++;
    			}
    			else{
    				row_rule[rule_size-nonogram_size][row_rule_num]=atoi(cut);
    				row_rule[rule_size-nonogram_size][0] += atoi(cut);   				
    				row_rule_num++;
    			}
    			cut = strtok(NULL, "\t");
    		}
	    	rule_size++;
    	}
    	else{
    		flag = 1;
    	}

    }
    bfs_solve(q_num); 
}

void bfs_solve(int question_num){
	printf("BFS Starting......\n");
	initial_ans_map();
	init_queue();
    rough_col_initial();
    int *demap = NULL;

    int test=1;
    while(front->next){
    	printf("%d\n", test);
    	test ++;
    	demap = dequeue();
    	modify_ans_format(demap);
		if(check_answer()==1){
			printf("%d\n", test);
			printf("Find the answer!\n");
			write_answer(question_num);
			break;
		}
		else{
			find_next_step(demap);
		}
	}
}

void push(int *map){
	Node *new_node = (Node*) malloc(sizeof(Node));
	new_node->map = map;
	new_node->next = top;
	top = new_node;
}

int *pop(){
	if(top==NULL)
		return NULL;
	int *map;
	Node *p;
	p = top;
	map = p->map;
	top = top->next;
	free(p);
	return map;
}

void rough_col_initial(){
	int *ptr_map = malloc((nonogram_size*nonogram_size+1)*sizeof(int));
	int k, now_row;
	for (int i=0; i<nonogram_size; i++){
		k=2;
		now_row = 0;
		if(col_rule[i][0] == 0){
			for (int j=0; j<nonogram_size; j++){
					*(ptr_map+j*nonogram_size+i) = 0;
			}
		}
		else{
			if(col_rule[i][1] != 0){
				for (int m=0; m<col_rule[i][1]; m++){
					*(ptr_map+(now_row+m)*nonogram_size+i) = 1;
				}
				now_row += col_rule[i][1];
			}
			while(col_rule[i][k] != 0){
				for (int m=0; m<col_rule[i][k]+1; m++){
					*(ptr_map + (now_row+m)*nonogram_size+i) = k;
				}
				now_row += col_rule[i][k]+1;
				k++;
			}
		}
	}
	*(ptr_map+nonogram_size*nonogram_size) = 0;
	enqueue(ptr_map);
	push(ptr_map);	
}

void modify_ans_format(int *ptr_ans_map){
	initial_ans_map();
	for (int j=0; j<nonogram_size; j++){
		int pre = 1;
		for (int i=0; i<nonogram_size; i++){
			if(*(ptr_ans_map+i*nonogram_size+j)==1){
				ans_map[i][j] = 1;
			}
			else if(*(ptr_ans_map+i*nonogram_size+j)!=0 && *(ptr_ans_map+i*nonogram_size+j)==pre){
				ans_map[i][j] = 1;
			}
			pre = *(ptr_ans_map+i*nonogram_size+j);
		}
	}
}


void find_next_step(int *pre_map){
	int pre, now;
	int *new_map;
	int i;
	int last = nonogram_size*nonogram_size;
	for (int j= *(pre_map+last); j<nonogram_size; j++){
		i = nonogram_size-2;
		while(i>=0){
			pre = *(pre_map+(i+1)*nonogram_size+j);
			now = *(pre_map+i*nonogram_size+j);
			if(pre==0 && now!=0 && now<10){
				new_map = malloc((nonogram_size*nonogram_size+1)*sizeof(int));
				memcpy(new_map, pre_map, nonogram_size*nonogram_size*sizeof(int));
				*(new_map+(i+1)*nonogram_size+j) = now;
				while(i>0 && *(pre_map+(i-1)*nonogram_size+j)==now){
					*(new_map+i*nonogram_size+j) = now;
					i--;
					if(i<1){
						break;
					}
				}
				*(new_map+i*nonogram_size+j) = 0;
				*(new_map+last) = j;
				if(check_close_list(new_map)==1)
					enqueue(new_map);
			}
			i--;
		}

	}
}


int check_close_list(int *new_map){
	int flag;
	for (int k=0; k<close_size; k++){
		if(close_list[k]!=NULL){
			flag = 0;
			for (int i=0; i<nonogram_size; i++){
				for (int j=0; j<nonogram_size; j++){
						if(*(close_list[k]+i*nonogram_size+j)!=*(new_map+i*nonogram_size+j)){
							flag = 1;
							i=nonogram_size;
							j=nonogram_size;     
						}
				}
			}
			if(flag==0)
				return 0;
		}
	}
	close_list[close_idx] = new_map;
	close_idx=(close_idx+1)%close_size;
	return 1;
}

void init_queue(){
	front = (Node*) malloc(sizeof(Node));
	rear = (Node*) malloc(sizeof(Node));
	front->next = NULL;
	rear->next = NULL;
}

void enqueue(int *map){
	Node *new_node = (Node*) malloc(sizeof(Node));
	
	if(!new_node)
		exit(1);
	if(front->next == NULL){
		front->next = new_node;
	}
	
	new_node->map = map;
	new_node->next = NULL;
	rear->next = new_node;
	rear = new_node;
}

int *dequeue(){
	if(!front->next){
		printf("Queue is empty!\n");
		return NULL;
	}
	else{
		Node *p = front->next;
		int *map = p->map;
		front->next = p->next;
		free(p);
		return map;
	}
}

int check_answer(){
	// check total num of row or col
	int row_cnt;
	for (int i=0; i<nonogram_size; i++){
		row_cnt = 0;
		for (int j=0; j<nonogram_size; j++){
			if (ans_map[i][j] != 0){
				row_cnt ++;
			}
		}
		if (row_rule[i][0] != row_cnt){
			return 0;
		}
	}

	// check row in detail
	int rule_ptr;
	int cnt;
	for (int i=0; i<nonogram_size; i++){
		rule_ptr = 1;
		cnt = 0;
		for (int j=0; j<nonogram_size; j++){
			if(ans_map[i][j] == 0){
				if(cnt != 0 && row_rule[i][rule_ptr] != cnt){
					return 0;
				}
				else if(cnt != 0){
					rule_ptr ++;
					cnt = 0;
				}
			}
			else{
				cnt ++;
			}
		}
	}

	return 1;
}

void write_answer(int question_num){
	FILE *fptr;
	fptr = fopen("answer.txt","a");
	if(!fptr){
		printf("Fail in opening the file!\n");
		exit(1);
	}
	fprintf(fptr, "$%d\n", question_num);
	for (int i=0; i<nonogram_size; i++){
		for (int j=0; j<nonogram_size; j++){
			fprintf(fptr, "%d\t", ans_map[i][j]);
		}
		fprintf(fptr, "\n");
	}
}