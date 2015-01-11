# -*- coding: utf-8 -*-
"""
Created on Thu Dec 12 14:52:29 2013

@author: lessig
"""

# imports, system
import numpy as np
import matplotlib.pyplot as plt

# imports, user
import audio
import dft



def midC() :
    """
    Generate mid C (f = 263.63 Hz) and write to audio file.

    Inputs:
    -

    Return:
    -

    Side effects:
    - Generates
    """

    num_samples = 44100
    duration = 1

    x = np.linspace(0.0,2.0*np.pi, num_samples * duration)

    # generate mid C
    f = 261.63
    data = np.sin( f * x)

    audio.writeAudioData( './data/mid-c.wav', data, num_samples)


def filterHighs( adata, bandlimit = 5000) :
    """
    Filter high frequencies above bandlimit.

    Inputs:
    adata : data to be filtered
    bandlimit : bandlimit above which to cut off frequencies

    Return:
    adata_filtered : filtered data

    """

    # compute Fourier transform of input data
    fdata = dft.fft( adata)

    fig = plt.figure()
    fig.add_subplot(2, 1, 1)
    plt.plot( np.real(fdata))
    plt.xlim( [0,adata.size])

    # set high frequencies above bandlimit to zero, make sure the almost symmetry of the
    # transform is respected
    fdata[ bandlimit+1 : fdata.size-1-bandlimit+1] = 0

    fig.add_subplot(2, 1, 2)
    plt.plot( np.real(fdata))
    plt.xlim( [0,adata.size])
    plt.show()

    # compute inverse transform
    adata_filtered = dft.fft( fdata, True)
    adata_filtered = np.real(adata_filtered)

    return adata_filtered


def main() :

    # run 3.1
    midC()

    # load signal for 3.2
    # see http://www.ee.columbia.edu/~dpwe/sounds for more example sounds
    (adata,rate) = audio.readAudioData( './data/speech.wav')
    adata = adata[0:2**15];

    # run filter for 3.2
    adata_filtered = filterHighs( adata)

    # save result for 3.2
    audio.writeAudioData( 'data/speech-filtered.wav', adata_filtered, rate)

# run Ex. 3
main()
