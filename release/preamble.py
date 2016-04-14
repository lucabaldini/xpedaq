#! /bin/env python

import os
import sys

SOURCE_FOLDERS = ['../gui',
                  '../daq',
                  '../utils',
                  '../xsrv',
                  '../unittest/test_env',
                  '../unittest/test_header']
SOURCE_EXTENSIONS = ['h', 'cpp']
PREAMBLE = file('preamble').readlines()
PREAMBLE_OPENING = '/*'
PREAMBLE_CLOSING = '*/'

def compareLists(list1, list2):
    if len(list1) != len(list2):
        return False
    for i in range(len(list1)):
        if list1[i] != list2[i]:
            return False
    return True

def updateFile(filePath, testMode = False):
    print 'Opening %s...' % filePath
    content = file(filePath).readlines()
    if content == []:
        return
    elif PREAMBLE_OPENING in content[0]:
        numPreambleLines = 1
        while PREAMBLE_CLOSING not in content[numPreambleLines]:
            numPreambleLines += 1
        numPreambleLines += 1
    else:
        numPreambleLines = 0
    (preamble, source) = (content[:numPreambleLines],
                          content[numPreambleLines:])
    if source != [] and source[0] != '\n':
        source = ['\n'] + source
    if compareLists(preamble, PREAMBLE):
        print 'Header is up to date.'
    else:
        print 'Updating header (%d lines)...' % len(preamble)
        cmd = 'cp %s %s.bak' % (filePath, filePath)
        if not testMode:
            os.system(cmd)
        else:
            print 'Skipping in test mode.'
        print cmd
        print 'Writing file...'
        if not testMode:
            outputFile = file(filePath, 'w')
            outputFile.writelines(PREAMBLE)
            outputFile.writelines(source)
            outputFile.close()
        else:
            print 'Skipping in test mode.'
        print

def update(testMode = False):
    for folder in SOURCE_FOLDERS:
        print 'Updating %s' % folder
        for fileName in os.listdir(folder):
            if fileName.split('.')[-1] in SOURCE_EXTENSIONS:
                filePath = os.path.join(folder, fileName)
                updateFile(filePath, testMode)

def cleanup(testMode = False):
    for folder in SOURCE_FOLDERS:
        print 'Cleaning up %s' % folder
        cmd = 'cd %s; rm -f *.bak; cd -;' % folder
        print cmd
        if not testMode:
            os.system(cmd)
        else:
            print 'Skipping in test mode.'

def restore(testMode = False):
    for folder in SOURCE_FOLDERS:
        print 'Restoring %s' % folder
        for fileName in os.listdir(folder):
            if fileName.split('.')[-1] in SOURCE_EXTENSIONS:
                filePath = os.path.join(folder, fileName)
                cmd = 'cd %s; mv %s.bak %s; cd -;' %\
                    (folder, filePath, filePath)
                print cmd
                if not testMode:
                    os.system(cmd)
                else:
                    print 'Skipping in test mode.'


if __name__ == '__main__':
    from optparse import OptionParser
    parser = OptionParser(usage = 'usage: %prog [options]')
    parser.add_option('-u', '--update', dest = 'u',
                      default = False, action = 'store_true',
                      help = 'update the preambles')
    parser.add_option('-c', '--cleanup', dest = 'c',
                      default = False, action = 'store_true',
                      help = 'remove the backup files created in the update')
    parser.add_option('-r', '--restore', dest = 'r',
                      default = False, action = 'store_true',
                      help = 'rerstore the backup files')
    parser.add_option('-t', '--test-mode', dest = 't',
                      default = False, action = 'store_true',
                      help = 'dry run (test mode)')
    (opts, args) = parser.parse_args()
    if not opts.u and not opts.c and not opts.r:
        parser.print_help()
        parser.error('Specify at least one option among "-u" and "-c".')
    if opts.u:
        update(opts.t)
    if opts.c:
        cleanup(opts.t)
    if opts.r:
        restore(opts.t)
    
