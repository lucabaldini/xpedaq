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


import logging
logging.basicConfig(level=logging.DEBUG)


import struct


class pXpeEvent:
    
    """
    """
    
    HEADER = 65535
    HEADER_LEN = 20
    
    def __init__(self, xmin, xmax, ymin, ymax, buffer_id, t1, t2, s1, s2,
                 adc_counts):
        """Constructor.
        """
        self.xmin = xmin
        self.xmax = xmax
        self.ymin = ymin
        self.ymax = ymax
        self.buffer_id = buffer_id
        self.t1 = t1
        self.t2 = t2
        self.s1 = s1
        self.s2 = s2
        self.adc_counts = adc_counts

    def num_bytes(self):
        """Return the total number of bytes in the event.
        """
        return self.HEADER_LEN + 2*self.num_pixels()

    def num_pixels(self):
        """Return the total number of pixels in the window.
        """
        return (self.xmax - self.xmin + 1)*(self.ymax - self.ymin + 1)

    def timestamp(self):
        """Return the timestamp from the FPGA.
        """
        return (self.t1 + self.t2*65534)*0.8e-6

    def start_seconds(self):
        """Return the seconds from the start run.
        """
        return (self.s2 + self.s1*65536)

    def __str__(self):
        """
        """
        text = 'buffer %5d, w(%3d, %3d)--(%3d, %3d), %d px, t = %d + %.5f s' %\
               (self.buffer_id, self.xmin, self.ymin, self.xmax, self.ymax,
                self.num_pixels(), self.start_seconds(), self.timestamp())
        return text


class pXpeBinaryFile(file):
    
    """Small class representing a binary file from the DAQ.
    """

    def __init__(self, filePath):
        """Constructor.
        """
        logging.info('Opening input binary file %s...' % filePath)
        file.__init__(self, filePath, 'rb')

    def read_word(self):
        """Read a single 2-bytes binary word from the file.
        """
        return struct.unpack('H', self.read(2))[0]

    def __iter__(self):
        """Iterator implementation.
        """
        return self

    def next(self):
        """Read the next event in the file.
        """
        try:
            header = self.read_word()
        except Exception, e:
            print(e)
            raise StopIteration()
        if header != pXpeEvent.HEADER:
            logging.error('Event header mismatch (got %s).' % hex(header))
            raise StopIteration()
        xmin = self.read_word()
        xmax = self.read_word()
        ymin = self.read_word()
        ymax = self.read_word()
        num_pixels = (xmax - xmin + 1)*(ymax - ymin + 1)
        buffer_id = self.read_word()
        t1 = self.read_word()
        t2 = self.read_word()
        s1 = self.read_word()
        s2 = self.read_word()
        adc_counts = []
        for i in xrange(num_pixels):
            adc_counts.append(self.read_word())
        return pXpeEvent(xmin, xmax, ymin, ymax, buffer_id, t1, t2, s1, s2,
                         adc_counts)



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
