from subprocess import check_output
import sys
import time
import numpy as np
import pandas as pd
from decimal import Decimal

alldata = []
allinstances = []
Nsol = {}

if ( len(sys.argv) == 4 ):
    
    EXEC_PATH  = sys.argv[1]
    INSTS_PATH = sys.argv[2]
    SAVE_PATH = sys.argv[3]
    
    try:
        # # cleanning the directory
        # option = raw_input('The directory ' + SAVE_PATH + ' content will be erased. Are you sure you want to delete its content? Yes (\'Y\') or No (\'n\')')
        # if option == 'y' || option == 'Y' :
        #     check_output(['r']);
 
        ls = check_output(['ls', INSTS_PATH])
        instances = ls.split('\n')
        instances = instances[0:len(instances)-1]
        # print (instances) 
        summary = []
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
            # summary = []
            # running the instance
            print ('executing instance ' + i + ' ...')
            
            # time init
            start_time = time.time()
            # excution #j
            out = check_output(cmd)
            # finish time
            process_time = time.time() - start_time
            out+= '\nProcess Time: ' + format(process_time, '.2f') + '\n';

            #stroring the number of times that each solutions was reached
            cost = out[out.find('Cost: ') + len('Cost: '):out.find('\nLower')]
            
            # store the information for summary file  
            summary.append( [ i,  cost, format(process_time, '.2f') ] )
            print ('creating output file...')
            f = open(SAVE_PATH + res_filename + '/' + res_filename + '.txt', 'w')
            f.write(out)
            f.close()
            print ('done!')

        print summary
        names =  ['cost', 'worst cost', 'time']
        dataframe = pd.DataFrame(summary, columns=names)
        dataframe.to_csv(SAVE_PATH + 'results' + '.csv', index=True, header=True, sep=',')
        

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
    print (message)