# -*- coding: utf-8 -*-
"""
Created on: 13/11/2013

@author: lessig

Sarah Köhler (sarah.c.koehler@gmail.com)
Dora Szücs (szuucs.dora@gmail.com)
"""

import os
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import pylab
from scipy import misc
import math as m
from PIL import Image

def loadImages( path, pattern) :
    """
    Load all images in path that match pattern.

    Inputs:
    path : path of directory containing image files
    pattern : string that has to be contained in image files

    Return:
    imgs : list of images (each image as numpy array)
    dim_x : size of images in x direction
    dim_y : size of images in y direction
    """

    # read images from disk
    imgs = []
    for subdir, dirs, files in os.walk(path):
        for file in files:
            if pattern not in file :
                continue
            imgs.append(misc.imread(path+file, False)) 

    
    dim_y = imgs[0].shape[0]
    dim_x = imgs[0].shape[1]    

    return imgs, dim_x, dim_y


def dataMatrix( imgs) :
    """
    Create data matrix out of list of 2D data sets.

    Inputs:
    imgs : list of 2D data sets (assumed to be all homogeneous of the same size)

    Return:
    D : data matrix
    """

    # compute data matrix
    columns = imgs[0].shape[0]*imgs[0].shape[1]
    rows = len(imgs)
    D = np.zeros((rows, columns))
    for i in range(0, rows) :
        D[i,:] = np.reshape(imgs[i], (1, columns))

    return D


def pca( D) :
    """
    Perform PCA for given data matrix.

    Inputs:
    D : data matrix of size n x k where n is the number of
        experiments/observations and k the number of variables observed

    Return:
    pcs : matrix containing principal components as rows
    svals : singular values associated with principle components
    mean_data : mean that was subtracted from data
    """


    # Compute PCA
    # Hint: set full_matrices = 0 in linalg.svd method to save some computation time.
    mean_data = D.mean()
    D -= mean_data
    U, svals2, V = np.linalg.svd(D, False, True)
    rows = D.shape[0]
    columns = D.shape[1]
    svals = np.copy(svals2)
    arr_std = np.zeros((1, len(svals)))
    pcs = np.zeros((len(svals), columns))
    for i in range(0, len(svals)) :
        svals[i] = (m.sqrt(((svals[i])**2) / (columns-1)))
        pcs[i,:] = (V[i,:] * svals[i])

    return pcs, svals2, mean_data


def visualizeEigenFaces( n, pcs, en, dim_x, dim_y) :
    """
    Visualize eigenfaces.

    Inputs:
    n : number of eigenfaces to draw
    pcs : principal component matrix whose rows are the eigenfaces
    """

    fig = pylab.figure( figsize=(15,8))
    m = int(np.ceil(n/2))
    n = 2 * m

    for i in range(n) :

        fig.add_subplot(2, m, i+1)
        eface = pcs[i,:].reshape( (dim_y, dim_x))
        plt.imshow( eface, cmap="Greys_r")
        plt.title( 'sigma = %.f' % en[i])

    plt.savefig( './figures/eigenfaces_efaces.pdf')
    plt.show()


def accumulatedEn( en, threshold = 0.9) :
    """
    Compute index k so that threshold percent of energy is contained in
    first k singular vectors.

    Inputs:
    en : vector containing singular values
    threshold : threshold for determining k (default = 0.9)

    Return:
    k : index so that threshold percent of energy are contained in first k singular vectors
    """
    sumEnergy = np.sum(en)
    energy = 0.0
    k = 0
    # accumulate energy and find k
    while energy < threshold * sumEnergy :
        energy += en[k]
        k += 1
    return k


def plotSingularValuesEn( en, k) :
    """
    Plot singular values and accumulated energy.

    Input:
    en : vector containing singular values
    k : index for threshold for energy

    Return:
    -

    Side Effects:
    - Generate './figures/eigenfaces_energy.pdf'
    """

    en_cum = en.cumsum()

    fig = pylab.figure(figsize=(15,8))

    fig.add_subplot( 1, 2, 2)
    plt.plot( en)
    plt.vlines( k, 0.0, max(en), colors='r', linestyles='solid')
    plt.xlim( 0, len(en))
    plt.ylim( 0.0, max(en))
    plt.xlabel('Index of singular value')
    plt.ylabel('Magnitude singular value')

    fig.add_subplot( 1, 2, 1)
    plt.plot( en_cum)
    plt.vlines( k, 0.0, max(en_cum), colors='r', linestyles='solid')
    plt.xlim( 0, len(en_cum))
    plt.ylim( 0.0, max(en_cum))
    plt.ylabel('Accumulated singular values')
    plt.xlabel('Number of first singular value in accumulation.')

    plt.savefig( './figures/eigenfaces_energy.pdf')
    plt.show()


def projectFaces( pcs, imgs, mean_data) :
    """
    Project given image set into basis.

    Input:
    pcs : matrix containing principal components / eigenfunctions as rows
    imgs : original input images from which pcs were created
    mean_data : mean data that was subtracted before computation of SVD/PCA

    Return:
    coeffs : basis function coefficients for input images
    """
    pcs = np.transpose(pcs) + mean_data
    coeffs = np.zeros((pcs.shape[1], len(imgs)))
    k = 0
    for img in imgs:
        img = np.reshape(img, (-1,1)) 
        x, res, rank, s = np.linalg.lstsq(pcs, img)
        print("index: ", k )
        print(" Shape x : ", x.shape[0] )
        coeffs[:,k] = x[:,0] 
        k += 1
    return coeffs

def identifyPerson( imgs_train, imgs_test, coeffs_train, coeffs_test) :
    """
    Perform face recognition for test images assumed to contain faces
    of the same individuals as in the training data set.

    For each image in the test data set the closest match in the training data
    set is shown. The distance between images is given by the angle between their
    coefficient vectors.

    Inputs:
    imgs_train : training images
    imgs_test : test images
    coeffs_train : coefficients for training images
    coeffs_test : coefficients for test images

    Return:
    -

    Side Effects:
    - Generate './figures/eigenfaces_identify%i.pdf' % i
      (where i is the image number in the test data set)
    """

    # TODO: compute scores between faces
    scores = np.zeros((len(imgs_train), len(imgs_test))) 
    for k in range (0, len(imgs_train)):  
        for j in range (0, len(imgs_test)):
            x = coeffs_train[:,k]
            y = coeffs_test[:,j]          
            scalarp = np.dot(y,x)
            scalarp /= np.linalg.norm(x) * np.linalg.norm(y)
            scores[k,j] = np.rad2deg(np.arccos(scalarp))  
        

    # find best match, compute confidence and plot
    for i in range( scores.shape[1]) :

        # TODO: Find index of best match
        j = 0
        jval = scores[i, 0]
        for l in range (0, scores.shape[0]):
            if scores[l,i] < jval:
                j = l
                jval = scores[l,i]        

        fig = pylab.figure()

        fig.add_subplot(1, 2, 2)
        plt.imshow( imgs_train[j], cmap="Greys_r")
        plt.xlabel('Identified person')

        fig.add_subplot(1, 2, 1)
        plt.imshow( imgs_test[i], cmap="Greys_r")
        plt.xlabel('Query image')

        plt.savefig( './figures/eigenfaces_identify%i.pdf' % i )
        plt.show()


def recognizeFace( imgs, coeffs, pcs, mean_data) :
    """
    Perform recognition if an image contains a face or not. The confidence of
    an image being a face is given by the maximum of the norm of the difference between
    reconstructed and input image.

    For each image in the test data set the projection into the eigenface basis is
    shown next to the original image. We also give a face confidence score that is
    determined by || image_reconst - image || / || image || where all images are
    interpreted as vectors.

    Inputs:
    imgs : set of test images
    coeffs : coefficient vector for test images
    pcs : eigenfaces as rows of a matrix
    mean_data : data mean

    Return:
    -

    Side Effects:
    - Generate './figures/eigenfaces_face_query%i.pdf', i
      (where i is the image number in the other data set)

    """

    # Compute face scores.
    img_scores = []
    imgs_reconst = []

    for k in range(len(imgs)):
        dim_img = imgs[0].shape
        img = np.reshape(imgs[k], (-1, 1)) + mean_data
        recon_img = np.reshape(coeffs[:,k], (1, -1)) + mean_data
        recon_img = np.dot(recon_img, pcs)
        print("img: ", img.shape)
        print("mean: ", mean_data)
        print("img_recon: ", recon_img.shape)
        print("pcs: ", pcs.shape)
        img_score = np.linalg.norm(img - recon_img) / (np.linalg.norm(img)) 
        print("norm img-rec: ", np.linalg.norm(img - recon_img))
        print("norm img: ", (np.linalg.norm(img)))
        recon_img = np.reshape(recon_img, (dim_img[0], dim_img[1]))
        imgs_reconst.append(recon_img)
        img_scores.append(img_score)


    # plot images and face score
    for j in range( len(imgs)) :

        fig = pylab.figure()
        #plt.title( 'face score = %.3f' % img_scores[j])

        fig.add_subplot(1, 2, 2)
        plt.imshow( imgs[j], cmap="Greys_r")
        plt.xlabel('Face error = %.3f' % img_scores[j])

        fig.add_subplot(1, 2, 1)
        plt.imshow( imgs_reconst[j], cmap="Greys_r")
        plt.xlabel('Reconstruction')

        plt.savefig( './figures/eigenfaces_face_query%i.pdf' % j)
        plt.show()



def main() :

    # read training data set
    imgs_train, dim_x, dim_y = loadImages( './data/train/', 'subject')

    # compute data matrix and perform PCA
    D = dataMatrix( imgs_train)
    pcs, en, mean_data = pca(D)
    # plot the first eigenfaces
   # visualizeEigenFaces( 10, pcs, en, dim_x, dim_y)
    
    # compute threshold for 80% of spectral energy
    k = accumulatedEn( en, 0.8)
   # plotSingularValuesEn( en, k)

    # cut off number of pcs if desired
    pcs = pcs[0:k,:]

    # compute coefficients of input in eigenbasis
    coeffs_train = projectFaces( pcs, imgs_train, mean_data)


    # load test data set
    imgs_test, dim_x, dim_y = loadImages( './data/test/', 'subject')
    # project test data set into eigenbasis
    coeffs_test = projectFaces( pcs, imgs_test, mean_data)

    # perform classical face recognition
    # identifyPerson( imgs_train, imgs_test, coeffs_train, coeffs_test)


    # load other image collection
    imgs_other, dim_x, dim_y = loadImages( './data/other/', 'other')
    # project test data set into eigenbasis
    coeffs_other = projectFaces( pcs, imgs_other, mean_data)

    # identify which of the images contains a face
    recognizeFace( imgs_other, coeffs_other, pcs, mean_data)

# run program
if __name__ == "__main__":
    main()




