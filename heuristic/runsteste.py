import numpy as np

def createCSV(array, path, filename):
	csv = np.array(array, dtype=np.dtype('d'))
	np.savetxt(path + filename + '.csv', csv, delimiter=",", fmt='%.2f')

# use example
# ar = [ [1,2.6666,3.897], [4,5,6.84], [0.012,"0.199","9"] ]
# createCSV(ar, '', 'foo')


