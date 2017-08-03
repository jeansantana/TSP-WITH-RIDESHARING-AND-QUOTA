from subprocess import check_output
import sys
import numpy as np
import pandas as pd

if ( len(sys.argv) == 3 ):
    INSTS_PATH = sys.argv[1]
    SAVE_PATH = sys.argv[2]

    try:
        ls = check_output(['ls', INSTS_PATH])
        instances = ls.split('\n')
        instances = instances[0:len(instances)-1]
        allinstances = []
        sols = []
        for i in instances:
            print ('extracting solution from ' + INSTS_PATH + str(i))
            ftext =  open(INSTS_PATH + str(i), 'r')
            sol = []
            allinstances.append(i)
            for line in ftext.readlines():
                cost = line.find('Cost: ')
                lb = line.find('Lower Bound: ')
                if cost != -1:
                    # print (line)
                    sol.append(float(line[len('Cost: '):len(line)]))
                elif lb != -1:
                    # print (line)                    
                    sol.append(float(line[len('Lower Bound: '):len(line)]))
            
            sols.append(sol)

        resume_arr = np.array(sols)
        names = ['cost', 'lower bound']
        dataframe = pd.DataFrame(resume_arr, index=allinstances, columns=names)
        dataframe.to_csv(SAVE_PATH + 'results' + '.csv', index=True, header=True, sep=',')

        # print allinstances
            
    except:
        for err in sys.exc_info():
            print (err)

else:
    message = 'You must to pass exactally two parameters:\n'
    message+= '\t1. INSTS_PATH - The instances directory path\n'
    message+= '\t2. SAVE_PATH - The solutions directory path\n'
    print (message)