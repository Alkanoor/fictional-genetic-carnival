# -*- coding: utf-8 -*-
"""
Éditeur de Spyder

Ce script temporaire est sauvegardé ici :
/home/romain/.spyder2/.temp.py
"""
afficher = False

if afficher:
    from mpl_toolkits.mplot3d import Axes3D
    from mpl_toolkits.mplot3d.art3d import Poly3DCollection
    import matplotlib.pyplot as plt

import numpy as np
from geometrie import*


def move(vector, ObjetsABouger):
    for i in range(len(ObjetsABouger)):
        for j in range(len(ObjetsABouger[i])):
            ObjetsABouger[i][j]=np.add(ObjetsABouger[i][j], vector)
    # update(listeObjets)


def rotation(origine, vector, w, objetsAPivoter): # w = vitesse de rotation, vector= vecteur directeur de l'axe de rotation
    vector = normalized(vector)
    vector=np.multiply(vector, w*dt)
    for j in range(len(objetsAPivoter)):
        for m in range(len(objetsAPivoter[j])):#changement de référentiel
            objetsAPivoter[j][m]=np.subtract(objetsAPivoter[j][m], origine)
        for n in range(len(objetsAPivoter[j])):#on decompose la rotation en 3 rotations
            Rot=np.array([[1,0,0], [0,np.cos(vector[0]),np.sin(vector[0])], [0,-np.sin(vector[0]),np.cos(vector[0])]]) # matrice de rotation dans le plan y,z
            objetsAPivoter[j][n]=np.dot(Rot,objetsAPivoter[j][n])
            Rot=np.array([[np.cos(vector[1]), 0, -np.sin(vector[1])],[0,1,0], [np.sin(vector[1]), 0, np.cos(vector[1])]]) # matrice de rotation dans le plan z,x
            objetsAPivoter[j][n]=np.dot(Rot,objetsAPivoter[j][n])
            Rot=np.array([[np.cos(vector[2]),np.sin(vector[2]),0],[-np.sin(vector[2]),np.cos(vector[2]),0],[0,0,1]])#matrice de rotation dans le plan x,y
            objetsAPivoter[j][n]=np.dot(Rot,objetsAPivoter[j][n])
        for p in range(len(objetsAPivoter[j])):#changement de référentiel inverse
            objetsAPivoter[j][p]=objetsAPivoter[j][p]+origine
    #update(listeObjets)


def update(listeObjets):
    ax.cla()
    for objet in listeObjets:
        Coord=[]
        for a in range(3):
            Coord.append([])
            for b in range(len(objet)):
                Coord[a].append(objet[b][a])

        ax.add_collection3d(Poly3DCollection([zip(Coord[0],Coord[1],Coord[2])]))
    ax.autoscale_view(True,True,True,True)
    ax.set_xlim3d(-axlim,axlim)
    ax.set_ylim3d(-axlim,axlim)
    ax.set_zlim3d(-axlim,axlim)
    plt.pause(0.00000001)

def normalized(vector):
    return(vector/np.linalg.norm(vector))

mot0Speed=0
mot1Speed=0

def Time(iterations):

    for i in range(iterations):
        if np.sign(motSpeedList[0])==np.sign(motSpeedList[1]):
            delta=abs(motSpeedList[0]-motSpeedList[1])
            liste=[abs(motSpeedList[0]),abs(motSpeedList[1])]
            if delta>0:
                rotation(ObjetParNom["fixRoue"+str(np.argmin(liste))][0],[0,0,1],delta*np.sign(abs(motSpeedList[0])-abs(motSpeedList[1]))*np.sign(motSpeedList[0]),listeObjets)
            speed=max(abs(motSpeedList[0]),abs(motSpeedList[1]))-delta
            vec=np.subtract(ObjetParNom["plateformeSup"][0],ObjetParNom["plateformeSup"][3])
            vec=normalized(vec)
            vec=np.multiply(vec,speed*np.sign(motSpeedList[0])*rayonRoues*dt)
            move(vec,listeObjets)
        else:
            liste=[abs(motSpeedList[0]),abs(motSpeedList[1])]
            delta=abs(liste[0]-liste[1])
            if delta>0:
                rotation(ObjetParNom["fixRoue"+str(np.argmin(liste))][0],[0,0,1],-delta*np.sign(motSpeedList[1]-motSpeedList[0]),listeObjets)
            speed=max(liste)-delta
            speed=speed*np.sign(motSpeedList[0])
            alpha=rayonRoues*2./largeur
            if speed!=0:
                rotation(ObjetParNom["centre"][0],[0,0,1],speed*alpha,listeObjets)
        if afficher:
            update(listeObjets)




def testPosMot(a,i):
    global motAngleList,motAngleLim
    if motAngleList[a][i]>motAngleLim[i][0]-abs(motSpeedList[a][i]*dt) or motAngleList[a][i]<motAngleLim[i][1]+abs(motSpeedList[a][i]*dt):#limite avec marge d'erreur car calcul temps discret
        motSpeedList[a][i]*=-1


def sequenceBuilder(N):
    seqLeft=[]
    seqRight=[]
    for i in range(N):
        mot=input("Moteur : ")
        if mot == 0:
            speed=input("Speed : ")
            duration=input("Duration : ")
            seqLeft.append([speed,duration])
        elif mot == 1:
            speed=input("Speed : ")
            duration=input("Duration : ")
            seqRight.append([speed,duration])
        else :
            print("Error")
            return
    return([seqLeft,seqRight])




seqLeft=[[10,30],[20,40]]
seqRight=[[10,30],[20,20]]
seq=[seqLeft,seqRight]


def sequence():
    if len(seq[0])>0:
        if seq[0][0][1]>0:
            motSpeedList[0]=seq[0][0][0]
            seq[0][0][1]-=1
        else:
            seq[0].remove(seq[0][0])
            if len(seq[0])>0:
                motSpeedList[0]=seq[0][0][0]
            else:
                motSpeedList[0]=0
    if len(seq[1])>0:
        if seq[1][0][1]>0:
            motSpeedList[1]=seq[1][0][0]
            seq[1][0][1]-=1
        else:
            seq[1].remove(seq[1][0])
            if len(seq[1])>0:
                motSpeedList[1]=seq[1][0][0]
            else:
                motSpeedList[1]=0
    Time(1)



stepByStepSeqLeft=[10,10,10,3,3,10,3,10,10,10,10,10,10,10]
stepByStepSeqRight=[10,10,10,3,3,3,3,10,10,10,10,10,10,10]
stepByStepSeq=[stepByStepSeqLeft,stepByStepSeqRight]



def stepByStep(stepByStepSeq):
    if len(stepByStepSeq[0])>0:
        motSpeedList[0]=stepByStepSeq[0][0]
        motSpeedList[1]=stepByStepSeq[1][0]
        stepByStepSeq[0].remove(stepByStepSeq[0][0])
        stepByStepSeq[1].remove(stepByStepSeq[1][0])
        Time(1)
        return(stepByStep(stepByStepSeq))
    else:
        return(evaluate())

def evaluate():
    objectif=[0,50,5]
    dist=distancePointPoint(objectif,listeObjets[6])
    return(min(1,1/dist))

def reInit():

    centre=[0,0,5]
    NWSup=[centre[0]-largeur*0.5,centre[1]+longueur*0.5,centre[2]-hauteur*0.5]
    NESup=[centre[0]+largeur*0.5,centre[1]+longueur*0.5,centre[2]-hauteur*0.5]
    SESup=[centre[0]+largeur*0.5,centre[1]-longueur*0.5,centre[2]-hauteur*0.5]
    SWSup=[centre[0]-largeur*0.5,centre[1]-longueur*0.5,centre[2]-hauteur*0.5]
    NWInf=[centre[0]-largeur*0.5,centre[1]+longueur*0.5,centre[2]+hauteur*0.5]
    NEInf=[centre[0]+largeur*0.5,centre[1]+longueur*0.5,centre[2]+hauteur*0.5]
    SEInf=[centre[0]+largeur*0.5,centre[1]-longueur*0.5,centre[2]+hauteur*0.5]
    SWInf=[centre[0]-largeur*0.5,centre[1]-longueur*0.5,centre[2]+hauteur*0.5]

    fixRoueW=[centre[0]-largeur*0.5,centre[1],centre[2]]
    fixRoueE=[centre[0]+largeur*0.5,centre[1],centre[2]]
    listeObjets=[[NWSup,NESup,SESup,SWSup],[NWInf,NEInf,SEInf,SWInf],[NWSup,NESup,NEInf,NWInf],[NESup,SESup,SEInf,NEInf],[SWSup,SESup,SEInf,SWInf],[NWSup,SWSup,SWInf,NWInf],[centre],[fixRoueW],[fixRoueE]]
    ObjetParNom["robot"]=listeObjets
    ObjetParNom["plateformeSup"]=listeObjets[0]
    ObjetParNom["plateformeInf"]=listeObjets[1]
    ObjetParNom["verticaleN"]=listeObjets[2]
    ObjetParNom["verticaleE"]=listeObjets[3]
    ObjetParNom["verticaleS"]=listeObjets[4]
    ObjetParNom["verticaleW"]=listeObjets[5]
    ObjetParNom["centre"]=listeObjets[6]
    ObjetParNom["fixRoue0"]=listeObjets[7]
    ObjetParNom["fixRoue1"]=listeObjets[8]

    global motSpeedList
    motSpeedList=[0,0]
    if afficher:
        update(listeObjets)



#--------------Initialisation---------------

if afficher:
    fig = plt.figure()
    ax = Axes3D(fig)
    plt.ion()
    plt.show()

axlim=100

longueur=15
largeur=10
hauteur=5

centre=[0,0,5]
NWSup=[centre[0]-largeur*0.5,centre[1]+longueur*0.5,centre[2]-hauteur*0.5]
NESup=[centre[0]+largeur*0.5,centre[1]+longueur*0.5,centre[2]-hauteur*0.5]
SESup=[centre[0]+largeur*0.5,centre[1]-longueur*0.5,centre[2]-hauteur*0.5]
SWSup=[centre[0]-largeur*0.5,centre[1]-longueur*0.5,centre[2]-hauteur*0.5]
NWInf=[centre[0]-largeur*0.5,centre[1]+longueur*0.5,centre[2]+hauteur*0.5]
NEInf=[centre[0]+largeur*0.5,centre[1]+longueur*0.5,centre[2]+hauteur*0.5]
SEInf=[centre[0]+largeur*0.5,centre[1]-longueur*0.5,centre[2]+hauteur*0.5]
SWInf=[centre[0]-largeur*0.5,centre[1]-longueur*0.5,centre[2]+hauteur*0.5]

rayonRoues=5.
fixRoueW=[centre[0]-largeur*0.5,centre[1],centre[2]]
fixRoueE=[centre[0]+largeur*0.5,centre[1],centre[2]]


"""
pointsParSupport=15
rayonSupport=2
support0=[]
support1=[]
support2=[]
support3=[]
for i in range(pointsParSupport):
    support0.append(np.add(patte0Inf2[1],[rayonSupport*np.cos(np.pi*2*i/pointsParSupport),rayonSupport*np.sin(np.pi*2*i/pointsParSupport),0]))
    support1.append(np.add(patte1Inf2[1],[rayonSupport*np.cos(np.pi*2*i/pointsParSupport),rayonSupport*np.sin(np.pi*2*i/pointsParSupport),0]))
    support2.append(np.add(patte2Inf2[1],[rayonSupport*np.cos(np.pi*2*i/pointsParSupport),rayonSupport*np.sin(np.pi*2*i/pointsParSupport),0]))
    support3.append(np.add(patte3Inf2[1],[rayonSupport*np.cos(np.pi*2*i/pointsParSupport),rayonSupport*np.sin(np.pi*2*i/pointsParSupport),0]))"""
listeObjets=[[NWSup,NESup,SESup,SWSup],[NWInf,NEInf,SEInf,SWInf],[NWSup,NESup,NEInf,NWInf],[NESup,SESup,SEInf,NEInf],[SWSup,SESup,SEInf,SWInf],[NWSup,SWSup,SWInf,NWInf],[centre],[fixRoueW],[fixRoueE]]


ObjetParNom={}

ObjetParNom["robot"]=listeObjets
ObjetParNom["plateformeSup"]=listeObjets[0]
ObjetParNom["plateformeInf"]=listeObjets[1]
ObjetParNom["verticaleN"]=listeObjets[2]
ObjetParNom["verticaleE"]=listeObjets[3]
ObjetParNom["verticaleS"]=listeObjets[4]
ObjetParNom["verticaleW"]=listeObjets[5]
ObjetParNom["centre"]=listeObjets[6]
ObjetParNom["fixRoue0"]=listeObjets[7]
ObjetParNom["fixRoue1"]=listeObjets[8]

if afficher:
    update(listeObjets)



dt=0.01#dt intervalle de temps en secondes


global motSpeedList
motSpeedList=[mot0Speed,mot1Speed]#0 = gauche
#------------------------------------------
