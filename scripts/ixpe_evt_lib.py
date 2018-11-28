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

import struct
import numpy
import os
import logging as logger

# python2/3 compatibility fix
try:
	xrange
except NameError:
	xrange = range
	
# color core fror creen printout, work only on posix
class ixpeAnsiColors:

	HEADER = '\033[95m'
	BLUE = '\033[94m'
	GREEN = '\033[92m'
	YELLOW = '\033[93m'
	RED = '\033[91m'
	ENDC = '\033[0m'
	BOLD = '\033[1m'
	UNDERLINE = '\033[4m'

#
# Useful constants
#

XPOL_NUM_COLUMNS = 300
XPOL_NUM_ROWS    = 352
XPOL_NUM_PIXELS  = XPOL_NUM_COLUMNS*XPOL_NUM_ROWS
	
#
# Class for a windowed event
#	
class ixpeEventWindowed:
	
	"""Basic class representing an event aquired in windowed mode.
	"""
	
	HEADER_MARKER = 65535
	HEADER_LENGTH = 20
	
	def __init__(self, xmin, xmax, ymin, ymax, buffer_id, t1, t2, s1, s2,
				 adc_values):
		"""Constructor.
		"""
		self.xmin = xmin
		self.xmax = xmax
		self.ymin = ymin
		self.ymax = ymax
		self.buffer_id = buffer_id
		self.microseconds = (t1 + t2*65534)*0.8
		self.adc_values = adc_values

	def size(self):
		"""Return the total number of bytes in the event.
		"""
		return self.HEADER_LENGTH + 2*self.num_pixels()

	def num_columns(self):
		"""Return the number of columns.
		"""
		return (self.xmax - self.xmin + 1)

	def num_rows(self):
		"""Return the number of rows.
		"""
		return (self.ymax - self.ymin + 1)

	def num_pixels(self):
		"""Return the total number of pixels in the window.
		"""
		return self.num_rows()*self.num_columns()
		
	def adc_value(self, col, row):
		"""Return the pulse height for a given pixel in the window.
		"""
		return self.adc_values[col, row]

	def highest_pixel(self):
		"""Return the coordinats of the pixel with the maximum value of
		ADC counts.
		"""
		return numpy.unravel_index(numpy.argmax(self.adc_values),
								   self.adc_values.shape)

	def highest_adc_value(self):
		"""Return the maximum value of ADC counts for the pixels in the event.
		"""
		return self.adc_values.max()
		
	def ascii(self, zero_suppression=5, max_threshold=0.75, width=4,
			  color=True):
		"""Return a pretty-printed ASCII representation of the event.
		"""
		if os.name != 'posix':
			color = False
		_fmt = '%%%dd' % width
		_max = self.highest_adc_value()
		text = ''
		text += ' '*(2*width + 2)
		for col in xrange(self.num_columns()):
			text += _fmt % (col + self.xmin)
		text += '\n'
		text += ' '*(2*width + 2)
		for col in xrange(self.num_columns()):
			text += _fmt % col
		text += '\n'
		text += ' '*(2*width + 1) + '+' + '-'*(width*self.num_columns()) + '\n'
		for row in xrange(self.num_rows()):
			text += (_fmt % (row + self.ymin)) + ' ' + (_fmt % row) + '|'
			for col in xrange(self.num_columns()):
				adc = self.adc_value(col, row)
				pix = _fmt % adc
				if color and adc == _max:
					pix = '%s%s%s' %\
						  (ixpeAnsiColors.RED, pix, ixpeAnsiColors.ENDC)
				elif color and adc >= max_threshold*_max:
					pix = '%s%s%s' %\
						  (ixpeAnsiColors.YELLOW, pix, ixpeAnsiColors.ENDC)
				elif color and adc > zero_suppression:
					pix = '%s%s%s' %\
						  (ixpeAnsiColors.GREEN, pix, ixpeAnsiColors.ENDC)
				text += pix
			text += '\n%s|\n' % (' '*(2*width + 1))
		return text

	def draw_ascii(self, zero_suppression=5):
		"""Print the ASCII representation of the event.
		"""
		print(self.ascii(zero_suppression))
		
	def __str__(self):
		"""String representation.
		"""
		text = 'buffer %5d, w(%3d, %3d)--(%3d, %3d), %d px, t = %d us' %\
				(self.buffer_id, self.xmin, self.ymin, self.xmax, self.ymax,
				self.num_pixels(), self.microseconds)
		return text


#
# Class for a windowed file
#	
class ixpeBinaryFileWindowed:
	
	"""Binary file acquired in windowed mode.
	"""
	def __init__(self, filePath):
		"""Constructor.
		"""
		logger.info('Opening input binary file %s...' % filePath)
		self.__file = open(filePath, 'rb')

	def seek(self, offset):
		""" redefine seek
		"""
		self.__file.seek(offset)

	def read(self, n):
		""" redefine read
		"""
		return self.__file.read(n)

	def close(self):
		""" redefine
		"""
		self.__file.close()

	def read_word(self):
		"""Read and byte-swap a single 2-bytes binary word from file.

		Note that struct.unpack returns a tuple even when we read a single
		number, and here we're returning the first (and only) element of the
		tuple.
		"""
		return struct.unpack('H', self.read(2))[0]

	def read_words(self, num_words):
		"""Read and byte-swap a fixed number of 2-bytes binary words from file.
		
		Args
		----
		num_words : int
			The number of words to be read from the input file.
		"""
		return struct.unpack('%dH' % num_words, self.read(2*num_words))

	def read_adc_word(self):
		"""Read and byte-swap a single 2-bytes binary word from file.

		Same as read word, but adc value are now signed
		"""
		return struct.unpack('h', self.read(2))[0]

	def read_adc_words(self, num_words):
		"""Read and byte-swap a fixed number of 2-bytes binary words from file.
		Same as read_words, but adc values are now signed.
		
		Args
		----
		num_words : int
			The number of words to be read from the input file.
		"""
		return struct.unpack('%dh' % num_words, self.read(2*num_words))
		
	def __iter__(self):
		"""Basic iterator implementation.
		"""
		return self
		
	def next(self):
		"""Read the next event in the file.
		"""
		try:
			header = self.read_word()
		except Exception:
			raise StopIteration()
		if header != ixpeEventWindowed.HEADER_MARKER:
			msg = 'Event header mismatch at byte %d' % self.tell()
			msg += ' (expected %s, got %s).' %\
				   (hex(ixpeEventWindowed.HEADER_MARKER), hex(header))
			logger.error(msg)
			logger.info('Moving ahead to the next event header...')
			while header != ixpeEventWindowed.HEADER_MARKER:
				header = self.read_word()
			logger.info('Got back in synch at byte %d.' % self.tell())
		xmin, xmax, ymin, ymax, buf_id, t1, t2, s1, s2 = self.read_words(9)
		num_columns = (xmax - xmin + 1)
		num_rows = (ymax - ymin + 1)
		data = self.read_adc_words(num_rows*num_columns)
		adc = numpy.array(data).reshape((num_rows, num_columns)).T
		return ixpeEventWindowed(xmin, xmax, ymin, ymax, buf_id, t1, t2, s1, s2,
								adc)

if __name__ == '__main__':
	import argparse
	formatter = argparse.ArgumentDefaultsHelpFormatter
	parser = argparse.ArgumentParser(formatter_class=formatter)
	parser.add_argument('infile', type=str,
						help='the input binary file')
	parser.add_argument('-n', '--num_events', type=int, default=10,
						help = 'number of events to be processed')
	args = parser.parse_args()
	#test_windowed
	input_file = ixpeBinaryFileWindowed(args.infile)
	for i in xrange(args.num_events):
		event = input_file.next()
		print (event)
		event.draw_ascii()
		try:
			input("e2g")
		except NameError:
			raw_input("e2g")

	
	