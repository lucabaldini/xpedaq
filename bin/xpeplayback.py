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


from pyxpe.event import *


if __name__ == '__main__':
    import argparse
    formatter = argparse.ArgumentDefaultsHelpFormatter
    parser = argparse.ArgumentParser(formatter_class=formatter)
    parser.add_argument('binfile', type=str,
                        help='the input binary file')
    parser.add_argument('-o', '--offset', type=int, default=0,
                        help='offset (in bytes) of the desired event header')
    parser.add_argument('-n', '--numevents', type=int, default=10,
                        help='number of events to be read')
    args = parser.parse_args()

    input_file = pXpeBinaryFile(args.binfile)
    input_file.seek(args.offset)
    for evt in xrange(args.numevents):
        print input_file.next()
