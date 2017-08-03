from subprocess import check_output
import sys
import time
import numpy as np
import pandas as pd
from decimal import Decimal

class SortIntanceFiles(object):
  def __init__(self, files):
    self.files = files

  def getSortedFiles(self):
    # File format TICXX-XX-XX-X.in
    groups = {10: [], 20: [], 30: [], 40: [], 50: []}

    for f in self.files:
      parts = f.split('-')
      groups[int(parts[1])].append(f)
    
    gFiles = []
    for key in sorted(groups):
      gFiles.append( sorted(groups[key]) )
    
    return gFiles

alldata = []
allinstances = []
Nsol = {}

def createCSV(array, path, filename):
    # csv = np.array(array, dtype=np.dtype('d'))
    print 'crating csv summary file...'

    data = np.array(array, dtype=np.dtype('d'))
    # print data
    names = ['cost', 'time']
    index = ['#' + str(i) for i in range(1, data.shape[0] + 1)] # list compression
    index.append('median')
    index.append('avg')
    
    avgs = [] 
    avgs.append( np.average(data[:, 0]) )
    avgs.append( np.average(data[:, 1]) )

    mds = []
    mds.append( np.median(data[:, 0]) )
    mds.append( np.median(data[:, 1]) )

    # 'best cost', 'worst cost', 'avg cost', 'median cost', 'avg time'

    best = np.min(data[:, 0])
    tpl = []
    tpl.append(best)
    tpl.append(Nsol[best])
    tpl.append(np.max(data[:, 0]))
    tpl.append(avgs[0])
    tpl.append(mds[0])
    tpl.append(avgs[1])


    alldata.append(tpl)
    allinstances.append(filename)

    # print avgs
    data = np.vstack([ data, [mds, avgs] ])

    df = pd.DataFrame(data, index=index, columns=names)
    path+= filename + '/'

    df.to_csv(path + filename + '.csv', index=True, header=True, sep=',')
    # np.savetxt(path + filename + '.csv', csv, delimiter=",", fmt='%.2f' )

    print 'done.'

if ( len(sys.argv) == 5 ):
    
    EXEC_PATH  = sys.argv[1]
    INSTS_PATH = sys.argv[2]
    SAVE_PATH = sys.argv[3]
    RUNS = sys.argv[4]
    
    try:
        # # cleanning the directory
        # option = raw_input('The directory ' + SAVE_PATH + ' content will be erased. Are you sure you want to delete its content? Yes (\'Y\') or No (\'n\')')
        # if option == 'y' || option == 'Y' :
        #     check_output(['r']);
 
        '''ls = check_output(['ls', INSTS_PATH])
        instances = ls.split('\n')
        instances = instances[0:len(instances)-1]
        # print (instances) 

        sif = SortIntanceFiles(instances)
        instances = sif.getSortedFiles()'''

        # print (instances) 

        instances = ['TIC03-40-40-3.in',
                     'TIC08-40-40-3.in',
                     'TIC11-40-40-4.in',
                     'TIC40-40-40-4.in',
                     'TIC44-40-40-3.in',
                     'TIC52-40-40-4.in',
                     'TIC54-40-40-4.in',
                     'TIC58-40-40-4.in']
 

        for i in instances:
            # Preparing the cmd to run the instance
            print ('setting up instance ' + i + ' ...')
            cmd = []
            cmd.append('./' + EXEC_PATH)
            cmd.append(INSTS_PATH + i)
            cmd.append('1')
            cmd.append('171')
            
            # Instance format is filename.in, so that why is len(i) - 3
            res_filename = i[0:len(i)-3]
            # create folder to hold the files for each of #RUNS execution
            message = 'Creating folder ' + SAVE_PATH + res_filename
            print message
            check_output(['mkdir', SAVE_PATH + res_filename])

            # summary = [['custo', 'tempo']]
            summary = []
            # running the instance
            print ('executing instance ' + i + ' ...')
            for j in range(1, int(RUNS) + 1):
                print ('running execution #' + str(j) + ' ...')
                # time init
                start_time = time.time()
                # excution #j
                out = check_output(cmd)
                # finish time
                process_time = time.time() - start_time
                out+= '\nProcess Time: ' + format(process_time, '.2f') + '\n';

                #stroring the number of times that each solutions was reached
                cost = out[out.find('Cost: ') + len('Cost: '):out.find('\nLower')]
                _cost = float(cost)
                if (Nsol.get(_cost, False)):
                    # print "aqui"
                    Nsol[_cost] = Nsol[_cost] + 1
                else:
                    # print "aqui2"
                    Nsol[_cost] = 1
                
                # store the information for summary file  
                summary.append( [ cost, format(process_time, '.2f') ] )
                print ('creating output file...')
                f = open(SAVE_PATH + res_filename + '/' + res_filename + '_' + str(j) + '.txt', 'w')
                f.write(out)
                f.close()
                print ('done!')
            createCSV(summary, SAVE_PATH, res_filename)
            Nsol = {}

        # creating the all file
        resume_arr = np.array(alldata)
        names = ['best cost', 'Nsol', 'worst cost', 'avg cost', 'median cost', 'avg time']
        dataframe = pd.DataFrame(resume_arr, index=allinstances, columns=names)
        dataframe.to_csv(SAVE_PATH + 'results40' + '.csv', index=True, header=True, sep=',')

    except:
        # print ('Invalid path format or the paths not exists.')
        # print sys.exc_info()[0]
        for err in sys.exc_info():
            print err

else:
    message = 'You must to pass exactally three parameters:\n'
    message+= '\t1. EXEC_PATH - The executable file path\n' 
    message+= '\t2. INSTS_PATH - The instances directory path\n'
    message+= '\t3. SAVE_PATH - The solutions directory path\n'
    message+= '\t4. RUNS - How many times this program will run\n'
    print (message)