#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_rule_and_solve(char *);
void initial_rule();
void initial_ans_map();
void rough_col_initial();
void modify_ans_format();
void solve_nonogram(int);
int check_answer();


int question_num = 1;
int nonogram_size = 5;

int col_rule[15][9];
int row_rule[15][9];
int transition_ans_map[15][15];
int ans_map[15][15];

int main(int argc, char *argv[]){
	if(argc != 3){
		printf("Format: ./[filename] [file_path] [size_of_nonogram]\n");
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
}

void initial_ans_map(){
	for (int i=0; i<15; i++){
		for (int j=0; j<15; j++){
			transition_ans_map[i][j] = 0;
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
    modify_ans_format();
	if(check_answer()==1){
		printf("Find the answer!\n");
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
			if (transition_ans_map[i][j] != 0){
				row_cnt ++;
			}
			if (transition_ans_map[j][i] != 0){
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
			if(transition_ans_map[i][j] == 0){
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
			if(transition_ans_map[j][i] == 0){
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

void rough_col_initial(){
	int k, now_row;
	for (int i=0; i<nonogram_size; i++){
		k=2;
		now_row = 0;
		if(col_rule[i][1] != 0){
			for (int m=0; m<col_rule[i][1]; m++){
				transition_ans_map[now_row+m][i] = 1;
			}
			now_row += col_rule[i][1];
		}
		while(col_rule[i][k] != 0){
			for (int m=0; m<col_rule[i][k]+1; m++){
				transition_ans_map[now_row+m][i] = k;
			}
			now_row += col_rule[i][k]+1;
			k++;
		}
	}

	printf("Transition_Map:\n");
	for (int i=0; i<nonogram_size; i++){
		for (int j=0; j<nonogram_size; j++){
			printf("%d ", transition_ans_map[i][j]);
		}
		printf("\n");
	}
}

void modify_ans_format(){

	for (int j=0; j<nonogram_size; j++){
		int pre = 1;
		for (int i=0; i<nonogram_size; i++){
			if(transition_ans_map[i][j]==1){
				ans_map[i][j] = 1;
			}
			else if(transition_ans_map[i][j]!=0 && transition_ans_map[i][j]==pre){
				ans_map[i][j] = 1;
			}
			pre = transition_ans_map[i][j];
		}
	}

	printf("Ans_Map:\n");
	for (int i=0; i<nonogram_size; i++){
		for (int j=0; j<nonogram_size; j++){
			printf("%d ", ans_map[i][j]);
		}
		printf("\n");
	}
}