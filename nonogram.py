import numpy as np
import sys as sys
import itertools as it

def read_rule(filename, size):
	rf = open(filename)
	lines = rf.readlines()
	rf.close()
	for line in lines:
		if(line[0]=='$'):
			tmp_rule = []
			print (line)
		else:
			tmp = line.splitlines()
			for i in tmp:
				tmp_rule.append(i.split('\t'))
	col_rule = tmp_rule[:int(size)]
	row_rule = tmp_rule[int(size):]
	for i, col in enumerate(col_rule):
		col_rule[i] = list(map(int, col))
	for i, row in enumerate(row_rule):
		row_rule[i] = list(map(int, row))
		
	return col_rule, row_rule
	
def build_tree(row_rule):
	nono_size = len(row_rule)
	row_permutation = []
	for id, row in enumerate(row_rule):
		zero_num = nono_size - sum(row) - len(row) + 1
		row_tmp = []
		for i in it.combinations_with_replacement(range(len(row)+1), zero_num):
			now = 0
			tmp = row.copy()
			for ele in i[::-1]:
				tmp.insert(ele, 0)
			pre_idx = 0
			for j in range(len(row)-1):
				idx = tmp[pre_idx:].index(row[j])+1
				tmp.insert(pre_idx + idx, 0)
				pre_idx = pre_idx+idx+1
			row_tmp.append(tmp)
		row_permutation.append(row_tmp)
	return row_permutation

def checker(col_rule, row_now, row_num):
	row_now = normalizer(row_now, row_num)
	nono_size = len(col_rule)
	for id, col in enumerate(col_rule):
		zero_num = nono_size - sum(col)
		print(id, col, zero_num)
		cnt = 0
		for i in range(row_num):
			cnt = cnt+1 if row_now[i][id]==0 else cnt
		if zero_num < cnt:
			return False

	for id, col in enumerate(col_rule):
		ptr = 0
		cnt = 0
		for i in range(row_num):
			if (row_now[i][id] == 0):
				if (cnt != 0 and col[ptr] < cnt):
					return False
				elif(cnt != 0):
					ptr += 1
					cnt = 0
			else:
				cnt += 1
		if (col[ptr-1] < cnt):
			return False
			
	return True;

def normalizer(row_now, row_num):
	nor_row = []
	for id, row in enumerate(row_now):
		tmp = []
		for ele in row:
			if(ele==0):
				tmp.append(0)
			else:
				for i in range(ele):
					tmp.append(1)
		nor_row.append(tmp)
	return nor_row

def main():
	if(len(sys.argv)!=3):
		print("Input format: python3 nonogram.py [data file path] [nonogram size]")
		exit(1)
	else:
		col_rule, row_rule = read_rule(sys.argv[1], sys.argv[2])
	
	row_permutation = build_tree(row_rule)
	test = []
	test.append(row_permutation[0][2])
	test.append(row_permutation[1][1])
	test.append(row_permutation[2][1])
	test.append(row_permutation[3][0])
	test.append(row_permutation[4][0])
	print (checker(col_rule, test, 5))
	

if __name__ == '__main__':
	main()