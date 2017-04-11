from subprocess import check_output
import sys

if ( len(sys.argv) == 4 ):
    
    EXEC_PATH  = sys.argv[1]
    INSTS_PATH = sys.argv[2]
    SAVE_PATH = sys.argv[3]
    
    try:

        ls = check_output(['ls', INSTS_PATH])
        instances = ls.split('\n')
        instances = instances[0:len(instances)-1]
        # print (instances) 

        for i in instances:
            print ('setting up instance ' + i + ' ...')
            cmd = []
            cmd.append('./' + EXEC_PATH)
            cmd.append(INSTS_PATH + i)
            print ('executing instance ' + i + ' ...')
            out = check_output(cmd)
            # Instance format is filename.in, so that why is len(i) - 3
            res_filename = i[0:len(i)-3]
            print ('creating output file...')
            f = open(SAVE_PATH + res_filename + ".txt", 'w')
            f.write(out)
            f.close()
            print ('done!')

    except:
        print ('Invalid path format or the paths not exists.')

else:
    message = 'You must to pass exactally three parameters:\n'
    message+= '\t1. EXEC_PATH - The executable file path\n' 
    message+= '\t2. INSTS_PATH - The instances directory path\n'
    message+= '\t3. SAVE_PATH - The solutions directory path\n'
    print (message)