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
import numpy



PIXELS_PER_BUFFER = 13200
NUM_BUFFERS = 8
NUM_PIXELS = PIXELS_PER_BUFFER*NUM_BUFFERS


class pXpeEventBase:

    """
    """

    pass



class pXpeEventFullFrame(pXpeEventBase):

    """
    """

    pass



class pXpeEventWindowed(pXpeEventBase):
    
    """
    """
    
    HEADER_MARKER = 65535
    HEADER_LENGTH = 20
    
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

    def size(self):
        """Return the total number of bytes in the event.
        """
        return self.HEADER_LENGTH + 2*self.num_pixels()

    def num_rows(self):
        """
        """
        return (self.xmax - self.xmin + 1)

    def num_columns(self):
        """
        """
        return (self.ymax - self.ymin + 1)

    def num_pixels(self):
        """Return the total number of pixels in the window.
        """
        return self.num_rows()*self.num_columns()

    def microseconds(self):
        """Return the timestamp from the FPGA.
        """
        return (self.t1 + self.t2*65534)*0.8e-6

    def start_seconds(self):
        """Return the seconds from the start run.
        """
        return (self.s2 + self.s1*65536)
        
    def adc_value(self, row, column):
        """Return the pulse height for a given pixel in the window.
        """
        return self.adc_counts[row, column]

    def draw_ascii(self):
        """
        """
        # This should be done properly
        numpy.set_printoptions(linewidth=100)
        print self.adc_counts

    def __str__(self):
        """
        """
        text = 'buffer %5d, w(%3d, %3d)--(%3d, %3d), %d px, t = %d + %.5f s' %\
               (self.buffer_id, self.xmin, self.ymin, self.xmax, self.ymax,
                self.num_pixels(), self.start_seconds(), self.microseconds())
        return text



class pXpeBinaryFileBase(file):

    """ Base class for a xpedaq binary file.
    """

    def __init__(self, filePath):
        """Constructor.
        """
        logging.info('Opening input binary file %s...' % filePath)
        file.__init__(self, filePath, 'rb')

    def read_word(self):
        """Read and byte-swap a single 2-bytes binary word from file.

        Note that struct.unpack returns a tuple even when we read a single
        number, and here we're returning the first (and only) element of the
        tuple.
        """
        return struct.unpack('H', self.read(2))[0]

    def read_words(self, num_words):
        """Read and byte-swap a fixed number of 2-bytes binary words from file.
        """
        return struct.unpack('%dH' % num_words, self.read(2*num_words))

    def __iter__(self):
        """Iterator implementation.
        """
        return self

    def next(self):
        """Do-nothing next() method to be reimplemented in the derived classes.
        """
        pass

        

class pXpeBinaryFileFullFrame(pXpeBinaryFileBase):
    
    """Binary file acquired in full-frame mode.
    """

    def next(self):
        """Read the next event in the file.
        """
        data = self.read_words(NUM_PIXELS)
        adc_counts = numpy.array(data).reshape(NUM_BUFFERS, PIXELS_PER_BUFFER)
        return adc_counts
            


class pXpeBinaryFileWindowed(pXpeBinaryFileBase):
    
    """Binary file acquired in windowed mode.
    """

    def next(self):
        """Read the next event in the file.
        """
        try:
            header = self.read_word()
        except Exception, e:
            print(e)
            raise StopIteration()
        if header != pXpeEventWindowed.HEADER_MARKER:
            logging.error('Event header mismatch (got %s).' % hex(header))
            raise StopIteration()
        ymin, ymax, xmin, xmax, buffer_id, t1, t2, s1, s2 = self.read_words(9)
        num_rows = (xmax - xmin + 1)
        num_columns = (ymax - ymin + 1)
        data = self.read_words(num_rows*num_columns)
        adc_counts = numpy.array(data).reshape((num_rows, num_columns))
        return pXpeEventWindowed(xmin, xmax, ymin, ymax,
                                 buffer_id, t1, t2, s1, s2, adc_counts)



def test_windowed(filePath, num_events):
    """
    """
    input_file = pXpeBinaryFileWindowed(filePath)
    for i in xrange(args.num_events):
        event = input_file.next()
        print event
        event.draw_ascii()
    

        
if __name__ == '__main__':
    import argparse
    formatter = argparse.ArgumentDefaultsHelpFormatter
    parser = argparse.ArgumentParser(formatter_class=formatter)
    parser.add_argument('infile', type=str,
                        help='the input binary file')
    parser.add_argument('-o', '--offset', type=int, default=0,
                        help='offset (in bytes) of the desired event header')
    parser.add_argument('-n', '--num-events', type=int, default=10,
                        help='number of events to be read')
    args = parser.parse_args()
    print args
    test_windowed(args.infile, args.num_events)
