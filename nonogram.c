#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_rule_and_solve(char *);
void initial_rule();
void initial_ans_map();
void rough_col_initial();
void modify_ans_format(int *);
void solve_nonogram(int);
void find_next_step(int *);
int check_answer();
void init_queue();
int queue_empty();
void enqueue(int *);
int *dequeue();


struct node
{
	int *map;
	struct node *next;
};
typedef struct node Node;
Node *front, *rear;

int question_num = 1;
int nonogram_size = 5;

int col_rule[15][9];
int row_rule[15][9];
int ans_map[15][15];

int main(int argc, char *argv[]){
	if(argc != 3){
		printf("Format: ./[filename] [file_path] [size_of_nonogram]\n");
		exit(1);
	}
	nonogram_size = atoi(argv[2]);
	init_queue();
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

	if (fptr==NULL){
        printf ("Error in reading the file!\n");
        exit(1);
    }
    while (fgets(tmp, 16, fptr) != NULL){
    	if (tmp[0] == '$' && flag==1){
    		q_num = tmp[1]-49;	//ASCII to int
    		solve_nonogram(q_num);
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
    solve_nonogram(q_num+1);
}

void solve_nonogram(int question_num){
	initial_ans_map();
    rough_col_initial();
    int *demap = NULL;
    //int test_cnt = 20;
    while(queue_empty()!=1){
    	//test_cnt--;
   		demap = dequeue();
    	modify_ans_format(demap);
		if(check_answer()==1){
			printf("Hey! Find the answer!\n");
			exit(1);
			break;
			//print the answer
		}
		else{
			find_next_step(demap);
		}
	}
}

void rough_col_initial(){
	////// Pointer base
	int *ptr_map = malloc(nonogram_size*nonogram_size*sizeof(int));
	int k, now_row;
	for (int i=0; i<nonogram_size; i++){
		k=2;
		now_row = 0;
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

	enqueue(ptr_map);	
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
	printf("Before_Map:\n");
	for (int i=0; i<nonogram_size; i++){
		for (int j=0; j<nonogram_size; j++){
			printf("%d ", *(ptr_ans_map+i*nonogram_size+j));
		}
		printf("\n");
	}

	printf("Ans_Map:\n");
	for (int i=0; i<nonogram_size; i++){
		for (int j=0; j<nonogram_size; j++){
			printf("%d ", ans_map[i][j]);
		}
		printf("\n");
	}

}

void find_next_step(int *pre_map){
	int pre, now;
	int *new_map;

	for (int j=0; j<nonogram_size; j++){
		new_map = malloc(nonogram_size*nonogram_size*sizeof(int));
		memcpy(new_map, pre_map, nonogram_size*nonogram_size*sizeof(int));
		for (int i=nonogram_size-2; i>=0; i--){
			pre = *(pre_map+(i+1)*nonogram_size+j);
			now = *(pre_map+i*nonogram_size+j);
			if(pre==0 && now!=0){
				*(new_map+(i+1)*nonogram_size+j) = now;
				
				while(*(pre_map+(i-1)*nonogram_size+j)==now){
					*(new_map+i*nonogram_size+j) = now;
					i--;
				}
				*(new_map+i*nonogram_size+j) = 0;
				enqueue(new_map);
				break;
			}
		}
	}
}

void init_queue(){
	front = (Node*) malloc(sizeof(Node));
	rear = (Node*) malloc(sizeof(Node));
	front->next = NULL;
	rear->next = NULL;
}

int queue_empty(){
	if(front->next == NULL)
		return 1;
	else
		return 0;
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
	if(queue_empty()==1){
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
	int col_cnt;
	for (int i=0; i<nonogram_size; i++){
		row_cnt = 0;
		col_cnt = 0;
		for (int j=0; j<nonogram_size; j++){
			if (ans_map[i][j] != 0){
				row_cnt ++;
			}
			if (ans_map[j][i] != 0){
				col_cnt ++;
			}
		}
		if (row_rule[i][0] != row_cnt || col_rule[i][0] != col_cnt){
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

	// check col in detail
	for (int i=0; i<nonogram_size; i++){
		rule_ptr = 1;
		cnt = 0;
		for (int j=0; j<nonogram_size; j++){
			if(ans_map[j][i] == 0){
				if(cnt != 0 && col_rule[i][rule_ptr] != cnt){
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