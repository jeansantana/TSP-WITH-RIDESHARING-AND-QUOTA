from subprocess import check_output
# irace --parallel 8 > file.txt

def getFileName(i):
	if i == 1:
		return "RESULT_GEN_SPLIT.txt"
	elif i == 2:
		return "RESULT_GEN_SCX.txt"
	elif i == 3:
		return "RESULT_GEN_SCX_BASED.txt"
	elif i == 4:
		return "RESULT_MEM_SPLIT.txt"
	elif i == 5:
		return "RESULT_MEM_SCX.txt"
	elif i == 6:
		return "RESULT_MEM_SCX_BASED.txt"

for i in xrange(1,2):
	outfile = getFileName(i)
	content = check_output(["irace", "--parallel", "8"])

	f = open(outfile, 'w')
	f.write(content)
	f.close()

	check_output(["rm", "target-runner"])
	fileToRename = str(i + 1) + "target-runner"
	# print fileToRename
	check_output(["mv", fileToRename, "target-runner"])