import sys
import os
import re
import string

def EnterDir(arg, dirname, names):
    valid = re.match('[0-9]{8}', os.path.basename(dirname))
    if valid:
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

        with open(maxpath) as f:
            for line in f:
                m.write(line)
        f.close()

if len(sys.argv) == 3:
    mergefile = sys.argv[1] + '\\' + sys.argv[2] + '.merge.txt'
    m = open(mergefile, 'w')
    os.path.walk(sys.argv[1], EnterDir, sys.argv[2])
    m.close()
else:
    print 'usage : tickmege path varname' 
