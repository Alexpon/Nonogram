#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_rule_and_solve(char *);
void initial_rule();
void solve_nonogram(int);
int check_answer(int[][15]);

int question_num = 0;
int nonogram_size;

int col_rule[15][9];
int row_rule[15][9];
int ans[15][15] = {	{1,0,1,0,0,1,1,1,1,0,0,0,0,1,0},
					{1,0,0,0,1,0,0,1,1,0,1,0,1,0,1},
					{0,1,0,1,0,1,1,1,0,0,1,0,0,1,0},
					{0,0,1,1,0,1,1,1,0,0,0,0,1,0,0},
					{0,0,1,0,0,1,1,1,0,0,0,1,0,0,1},
					{0,1,1,1,0,1,0,1,0,0,0,1,0,0,1},
					{1,1,1,0,0,0,1,0,0,0,0,1,0,0,0},
					{1,1,1,0,1,0,1,1,1,0,1,0,1,1,0},
					{1,1,0,0,0,1,1,1,0,0,0,1,1,0,0},
					{1,1,1,1,0,0,1,1,1,1,0,1,0,1,1},
					{1,1,1,1,0,1,1,0,0,0,0,1,0,1,1},
					{0,0,0,1,0,0,1,1,0,1,1,1,1,0,0},
					{0,0,0,0,0,1,1,0,1,0,1,0,1,0,0},
					{0,1,0,0,0,1,1,1,1,0,1,1,1,0,1},
					{0,1,0,1,0,0,1,1,0,1,1,1,0,1,1},
				};

int ans2[15][15] = {{0,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
					{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
					{0,1,0,0,1,0,0,0,0,0,0,0,0,0,0},
					{0,1,0,0,1,0,0,0,0,0,0,0,0,0,0},
					{1,0,1,1,1,0,0,0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
					{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
				};

int main(int argc, char *argv[]){
	if(argc != 3){
		printf("Format: ./[filename] [file_path] [size_of_nonogram]\n");
		exit(1);
	}
	nonogram_size = atoi(argv[2]);
	read_rule_and_solve(argv[1]);
	int a = check_answer(ans);
    printf("A = %d\n", a);
    int b = check_answer(ans2);
    printf("B = %d\n", b);
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

void read_rule_and_solve(char *file){
	initial_rule();

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
	printf("Question %d\n", question_num);
	printf("Col Rule:\n");
    for(int i=0; i<nonogram_size; i++){
    	for(int j=0; j<9; j++){
    		printf("%d ", col_rule[i][j]);
    	}
    	printf("\n");
    }

    printf("Row Rule:\n");
    for(int i=0; i<nonogram_size; i++){
    	for(int j=0; j<9; j++){
    		printf("%d ", row_rule[i][j]);
    	}
    	printf("\n");
    }
}

int check_answer(int ans_map[][15]){
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
