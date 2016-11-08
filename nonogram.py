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
		cnt = 0
		for i in range(row_num):
			cnt = cnt+1 if row_now[i][id]==0 else cnt
		if zero_num < cnt:
			return False

	for id, col in enumerate(col_rule):
		ptr = 0
		cnt = 0
		for i in range(row_num):
			reset = False
			if(ptr>len(col)-1):
				if row_now[i][id]==0:
					reset = True
					continue
				else:
					return False
			elif (row_now[i][id]==0):
				if (cnt != 0 and col[ptr] < cnt):
					return False
				elif(cnt != 0):
					ptr += 1
					reset = True
					cnt = 0
			else:
				cnt += 1
		ptr = ptr-1 if reset else ptr
		if (col[ptr] < cnt):
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

solution = np.zeros(15, dtype=int)

def backtrack(row_per_num, dimention, col_rule, row_permutation):
	row_now = []
	nono_size = len(col_rule)
	for r in range(dimention):
			c = solution[r]
			row_now.append(row_permutation[r][c])

	if(dimention==nono_size):
		if checker(col_rule, row_now, nono_size):
			ans = normalizer(row_now,nono_size)
			for i in ans:
				print(i)
			exit(1)
		return

	elif dimention>0:
		if not checker(col_rule, row_now, dimention):
			return

	for i in range(row_per_num[dimention]):
		solution[dimention] = i
		backtrack(row_per_num, dimention+1, col_rule, row_permutation)
	

def main():
	if(len(sys.argv)!=3):
		print("Input format: python3 nonogram.py [data file path] [nonogram size]")
		exit(1)
	else:
		col_rule, row_rule = read_rule(sys.argv[1], sys.argv[2])
	
	row_permutation = build_tree(row_rule)
	row_per_num = np.zeros(int(sys.argv[2]), dtype=int)
	for id, per in enumerate(row_permutation):
		row_per_num[id] = len(per)
	backtrack(row_per_num, 0, col_rule, row_permutation)
	

if __name__ == '__main__':
	main()