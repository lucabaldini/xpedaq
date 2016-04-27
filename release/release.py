#!/usr/bin/env python
# Copyright (C) 2007--2016 the X-ray Polarimetry Explorer (XPE) team.
#
# For the license terms see the file LICENSE, distributed along with this
# software.
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 2 of the License, or (at your
# option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.


import os
import time
import logging

logging.basicConfig(level=logging.DEBUG)

PACKAGE_NAME = 'xpedaq'
XPEDAQ_ROOT = os.environ['XPOL_DAQ_ROOT']
VERSION_FILE_PATH = os.path.join(XPEDAQ_ROOT, '__version__.h')
RELEASE_NOTES_FILE_PATH = os.path.join(XPEDAQ_ROOT, 'doc', 'release.notes')
BUILD_DATE = time.strftime('%a, %d %b %Y %H:%M:%S %z')
TAG_MODES = ['major', 'minor', 'patch']


def cmd(command):
    """Execute a system command.
    """
    logging.info('About to execute "%s"...' % command)
    os.system(command)
    logging.info('Done.')

def readTag():
    """ Read the current tag from the c++ version header.
    """
    return open(VERSION_FILE_PATH).readline().split()[-1].strip('"')

def updateVersionFile(mode):
    """ Update the version files with the new tag and build date.
    """
    tag = readTag()
    logging.info('Previous tag was %s...' % tag)
    version, release, patch = [int(item) for item in tag.split('.')]
    if mode == 'major':
        version += 1
        release = 0
        patch = 0
    elif mode == 'minor':
        release += 1
        patch = 0
    elif mode == 'patch':
        patch += 1
    else:
        abort('Unknown release mode %s.' % mode)
    nextTag = '%s.%s.%s' % (version, release, patch)
    logging.info('Writing new tag (%s) to %s...' % (nextTag, VERSION_FILE_PATH))
    outputFile = open(VERSION_FILE_PATH, 'w')
    outputFile.writelines('#define __XPEDAQ_VERSION__ "%s"\n' % nextTag)
    outputFile.writelines('#define __XPEDAQ_BUILD_DATE__ "%s"\n' % BUILD_DATE)
    outputFile.close()
    logging.info('Done.')
    return nextTag

def updateReleaseNotes(tag):
    """ Write the new tag and build date on top of the release notes.
    """
    hline = '-'*79
    logging.info('Reading in %s...' % RELEASE_NOTES_FILE_PATH)
    notes = open(RELEASE_NOTES_FILE_PATH).read().strip('\n')
    logging.info('Writing out %s...' % RELEASE_NOTES_FILE_PATH)
    info = '\n%s\n%s (%s) - %s\n%s\n' %\
           (hline, PACKAGE_NAME, tag, BUILD_DATE, hline)
    outputFile = open(RELEASE_NOTES_FILE_PATH, 'w')
    outputFile.writelines(info)
    outputFile.writelines(notes)
    outputFile.close()
    logging.info('Done.')

def tag(mode):
    """ Tag the package.
    """
    cmd('git pull')
    cmd('git status')
    tag = updateVersionFile(mode)
    updateReleaseNotes(tag)
    msg = 'Prepare for tag %s.' % tag
    cmd('git commit -a -m "%s"' % msg)
    cmd('git push')
    cmd('hg tag %s' % tag, verbose = True, dryRun = dryRun)
    msg = 'tagging version %s' % tag
    cmd('git tag -a %s -m "%s"' % (tag, msg))
    cmd('git push --tags')
    cmd('git status')
    #compilePackage()
    return tag



if __name__ == '__main__':
    from optparse import OptionParser
    parser = OptionParser()
    parser.add_option('-t', dest = 'tagmode', type = str, default = None,
                      help = 'The release tag mode %s.' % TAG_MODES)
    parser.add_option('-n', action = 'store_true', dest = 'dryrun',
                      help = 'Dry run (i.e. do not actually do anything).')
    (opts, args) = parser.parse_args()
    if not opts.tagmode:
        parser.print_help()
        parser.error('Please specify at least one valid option.')    
    #if compilePackage():
    #    abort('Make sure the code compiles before generating a release.')
    if opts.tagmode is not None:
        if opts.tagmode not in TAG_MODES:
            parser.error('Invalid tag mode %s (allowed: %s)' %\
                             (opts.tagmode, TAG_MODES))
        tag = tag(opts.tagmode)

