import numpy as np
import sys as sys

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
	row_rule = tmp_rule[:int(size)]
	col_rule = tmp_rule[int(size):]
	print (row_rule)
	print (col_rule)


def main():
	if(len(sys.argv)!=3):
		print("Input format: python3 nonogram.py [data file path] [nonogram size]")
		exit(1)
	else:
		read_rule(sys.argv[1], sys.argv[2])


if __name__ == '__main__':
	main()