# -*- coding: utf-8 -*-

"""modules contenant des fonctions utiles de géométrie"""

# produit scalaire : np.vdot
# produit vectoriel : np.cross

#from mpl_toolkits.mplot3d import Axes3D
#from mpl_toolkits.mplot3d.art3d import Poly3DCollection
#import matplotlib.pyplot as plt
import numpy as np


def conc(A):
    """Fonction pour écrire moins"""

    return(np.concatenate(A))


def distancePointPoint(A, B):
    return np.linalg.norm(np.subtract(B, A))


def distanceDroitePoint(point, origine, vecteur):
    """fonction qui renvoie la distance entre un point et une droite en METRE
    la droite est définie par une origine et un vecteur"""

    vecBA = np.subtract(point, origine)
    prodVec = np.cross(vecBA, vecteur)
    return (10**(-2))*np.linalg.norm(prodVec)/np.linalg.norm(vecteur)


def projeteOrtho(point, origine, vecteur):
    """fonction qui renvoie les coordonnées du projeté orthogonal d'un point sur
    une droite.
    droite : origine et vecteur"""

    d = -np.vdot(vecteur, point)
    a, b, c = 1.*vecteur[0], 1.*vecteur[1], 1.*vecteur[2]
    x, y, z = 1.*origine[0], 1.*origine[1], 1.*origine[2]
    t = -(a*x + b*y + c*z + d)/(a**2 + b**2 + c**2)
    return [a*t+x, b*t+y, c*t+z]


# see ray casting algorithm
def rotTriangle(listePieds, point):
    """fonction qui renvoit l'origine et l'axe directeur de l'axe de rotation
    liste = [[numerodupieds, coordonnees]]
    renvoie : [True, num d'un pied impliqué, num de l'autre pied impliqué]"""

    triangle = [elt[1] for elt in listePieds]
    referencePied = dict()
    referencePied[0] = listePieds[0][0]
    referencePied[1] = listePieds[1][0]
    referencePied[2] = listePieds[2][0]
    direction=[0,1]#direction choisie arbitrairement, notée ici mais pas utilisée
    count=0
    listeVecteurs=[[triangle[0],triangle[1]],[triangle[1],triangle[2]],[triangle[2],triangle[0]]]
    for vecteur in listeVecteurs :
        if vecteur[1][1]>vecteur[0][1]:
            A=[vecteur[0][0],vecteur[0][1]]
            B=[vecteur[1][0],vecteur[1][1]]
        else:
            A=[vecteur[1][0],vecteur[1][1]]
            B=[vecteur[0][0],vecteur[0][1]]
        if max(vecteur[0][0],vecteur[1][0])>point[0] and min(vecteur[0][0],vecteur[1][0])<point[0]:
            if A[1]>point[1] :
                count+=1
            elif B[1]>point[1] :
                if A[0]!=B[0]:
                    pente1=abs((B[1]-A[1])/(B[0]-A[0]))
                    pente2=abs((point[1]-A[1])/(point[0]-A[0]))
                    if pente2<=pente1 :
                        count+=1
    if count%2==1:# Point dans le triangle-->stable
        return [False,None,None]
    else :
        normaux=[]
        print(triangle[2]+[0],triangle[0]+[0],conc([np.subtract(triangle[1],triangle[0]),[0]]))#/!\ passage en 3D pour le projete
        normaux.append([projeteOrtho(conc([triangle[2], [0]]), conc([triangle[0], [0]]), conc([np.subtract(triangle[1],triangle[0]),[0]])),triangle[2]+[0]])#segment sommet-projection ortho
        normaux.append([projeteOrtho(conc([triangle[0], [0]]), conc([triangle[1], [0]]), conc([np.subtract(triangle[2],triangle[1]),[0]])),triangle[0]+[0]])#segment sommet-projection ortho
        normaux.append([projeteOrtho(conc([triangle[1], [0]]), conc([triangle[2], [0]]), conc([np.subtract(triangle[0],triangle[2]),[0]])),triangle[1]+[0]])#segment sommet-projection ortho
        print(normaux)
        for i in range(3):
            normaux[i]=np.subtract(normaux[i][1],normaux[i][0][0:2])#passage d'un segment formé de 2 points à un vecteur et on se replace en 2D
        print(normaux)
        print(normaux[0],np.subtract(point,triangle[1]))
        vec1=np.cross(normaux[0],np.subtract(point,triangle[1]))
        vec2=np.cross(normaux[1],np.subtract(point,triangle[1]))
        print(vec1)
        print(vec2)
        if np.dot(vec1,vec2)<0:
            return [True, referencePied[2], referencePied[0]]

        vec1=np.cross(normaux[1],np.subtract(point,triangle[2]))
        vec2=np.cross(normaux[2],np.subtract(point,triangle[2]))

        if np.dot(vec1,vec2)<0:
            return [True, referencePied[0], referencePied[1]]

        vec1=np.cross(normaux[2],np.subtract(point,triangle[0]))
        vec2=np.cross(normaux[0],np.subtract(point,triangle[0]))
        return [True, referencePied[1], referencePied[2]]
