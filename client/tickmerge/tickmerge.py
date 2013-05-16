import sys
import os.path
import re
import string

def EnterDir(arg, dirname, names):
    valid = re.match('[0-9]{8}', os.path.basename(dirname))
    if valid:
        #print dirname
        maxsize = 0
        maxpath = ''
        reexp = arg + '[XYxy]?[0-9]{2}'
        
        for file in names:
            if re.match(reexp, file):
                pathname = dirname + '\\' + file
                if(os.stat(pathname).st_size > maxsize):
                    maxsize = os.stat(pathname).st_size
                    maxpath = pathname

        if maxsize > 0 :
            print maxpath + ' size:' + str(maxsize)
            
if len(sys.argv) == 3:
    os.path.walk(sys.argv[1], EnterDir, sys.argv[2])
else:
    print 'usage : tickmege path varname' 
