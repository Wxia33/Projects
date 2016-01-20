import time as t
from os import path

def createFile(dest):
    '''
    Creates a file
    '''
    name = '%s.doc'%('Word')

    if not (path.isfile(dest+name)):
        f = open(dest+name,'w')
        f.write('\n'*30)
        f.close()

if __name__ == '__main__':
    destination = 'C:\\Users\\HomePC\\Desktop\\'
    createFile(destination)
    raw_input('done')
