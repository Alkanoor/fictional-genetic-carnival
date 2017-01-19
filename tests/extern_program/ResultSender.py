#!/usr/bin/python
# -*- coding: utf-8 -*-

from testRobotSimple import*
import sys

max = 50
nbit = 8
maxbit = 2**(nbit-1)
coeff = 1.*max/maxbit

sequence = open('data_in','rb').read().replace('\n','').split(";")
for i, elt in enumerate(sequence):
    print(i,elt)

instructions = [[] for elt in sequence]
for i, elt in enumerate(sequence):
    n = int(len(elt)/8)
    instructions[i] = [elt[j*8:(j+1)*8] for j in range(n)]

instructions = [[(int(elt2,2) - maxbit)*coeff for elt2 in elt] for elt in instructions]

with open("data", "w+") as fichier:
    for j, elt in enumerate(instructions):
        reInit()
        droite = [elt[2*i] for i in range(len(elt)/2)]
        gauche = [elt[(2*i)+1] for i in range(len(elt)/2)]
        liste = [droite, gauche]
        fichier.write(str(stepByStep(liste))+'\n')
