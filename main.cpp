
import numpy as np
import cv2

import sys
import argparse

offx=0
offy=0

class QTree:

    def qt(self, inImg, minStd, minSize, offX, offY):
        h,w = inImg.shape[0], inImg.shape[1]  
        m,s = cv2.meanStdDev(inImg)
        
        if s>=minStd and max(h,w)>minSize:
            oX1,oY1,im1, oX2,oY2,im2 = splitImage(inImg)
            
            self.qt(im1, minStd, minSize, offX+oX1, offY+oY1)
            self.qt(im2, minStd, minSize, offX+oX2, offY+oY2)
        else:
            self.listRoi.append([offX,offY,w,h,m,s])


    def __init__(self, img, stdmin, sizemin):
        self.listRoi = []
        self.qt(img, stdmin, sizemin, 0, 0)

def splitImage(inImg):
    
    h,w = inImg.shape[0], inImg.shape[1]
    off1X=0
    off1Y=0
    off2X=0
    off2Y=0
    
    if w >= h:  #split X
        off1X=0
        off2X=w/2
        img1 = inImg[0:h, 0:off2X]
        img2 = inImg[0:h, off2X:w]
    else:       #split Y
        off1Y=0
        off2Y=h/2
        img1 = inImg[0:off2Y, 0:w]
        img2 = inImg[off2Y:h, 0:w]
    
    return off1X,off1Y,img1, off2X,off2Y,img2

def quadtree(inImg, minStd, minSize, offX, offY, roiList):
    h,w = inImg.shape[0], inImg.shape[1]  
    m,s = cv2.meanStdDev(inImg)
    
    if s>=minStd and max(h,w)>minSize:
        oX1,oY1,im1, oX2,oY2,im2 = splitImage(inImg)
        
        quadtree(im1, minStd, minSize,offX+oX1,offY+oY1, roiList)
        quadtree(im2, minStd, minSize,offX+oX2,offY+oY2, roiList)
    else:
        roiList.append([offX,offY,w,h,m,s])



def main(file_):
    
    parser = argparse.ArgumentParser(description='Calcula los mejores cortes de barras')

    parser.add_argument('-img', metavar='', type=str, help='cargar imagen')

    parser.add_argument('-sz', metavar='', type=int, help='quadtree tam min ')
    
    parser.add_argument('-std', metavar='', type=float, help='desviación para dividir')

    args = parser.parse_args()

    #input valores
    IMAGE_TO_LOAD = args.img

    minDev        = args.sz
    minSz         = args.std
    
    raw = cv2.imread(IMAGE_TO_LOAD)
    
    if not type(raw)==type(None):
        if raw.ndim > 1 :
            img = cv2.cvtColor(raw, cv2.COLOR_BGR2GRAY)
        else :
            img = raw
    else :
        print 'Error al cargar imagen: ', IMAGE_TO_LOAD
        exit();
        
    print 'executando...'
    
    cv2.imshow('IMAGEN ORIGINAL',img)
    h,w = img.shape[0], img.shape[1]
    
    m,s = cv2.meanStdDev(img)
        
    qt=QTree(img,minDev,minSz)
    rois = qt.listRoi

    imgOut=img
    for e in rois:
        
        col=255
        if e[5]<minDev:
            col=0
        
        cv2.rectangle(imgOut, (e[0],e[1]), (e[0]+e[2],e[1]+e[3]), col, 1)
        
    cv2.imshow('IMAGEN QUADTREE',imgOut)

    cv2.waitKey(0)
    cv2.destroyAllWindows()


if __name__ == '__main__':
    main( sys.argv[1] )

