# -*- coding: utf-8 -*-
"""
Created on Thu Dec 12 14:52:29 2013

@author: lessig
"""

# import modules
import numpy as np
import scipy.io.wavfile as wavio


def readAudioData( fname) :
    """
    Read audio data from file and return numpy array representation.

    Inputs:
    fname : filename of audio file

    Return:
    adata : audio data as numpy array (shape=(n,), dtype=float64)
    rate : audio parameters (useful for generating output matching input)
    """

    (rate,adata_uint) = wavio.read( fname)

    # cast to float64 to perform subsequent computation in convenient
    # floating point format
    adata = np.asarray( adata_uint, dtype='float64')
    # for symmetry with writeAudioData(); scaling in geneal unclear    
    adata /= (2**15 - 1)

    return adata, rate


def writeAudioData( fname, data, rate) :
    """
    Write audio data given as numpy array to fname in WAV format

    Inputs:
    fname : name of WAV audio file to be written.
    data : audio data to be written (shape=(n,), dtype=float64)
    rate : sampling rate per second

    Return:
    -

    Side effects:
    Creates WAV file fname.
    """

    scaled_data = np.int16( data / np.max(np.abs(data)) * (2**15 - 1))
    wavio.write( fname, rate, scaled_data)

