import numpy as np
import os
import matplotlib.pyplot as plt

def readConf(filename):
    folder = "/home/aquisicao/Documentos/PET_SYS/ROOT/CorFac/"
    filename = folder+filename
    output = np.zeros((8,8))
    with open(filename, 'r') as file:
        linhas = file.readlines()
        for i,linha in enumerate(linhas):
             l = linha.split(" ")
             l.pop(-1)
             l= np.asarray(l)
             output[i,:]=l

    return output

def writeConf(detectores):
    folder = "/home/aquisicao/Documentos/PET_SYS/ROOT/CorFac/"
    for key,value in detectores.items():
        nome = folder+"detector-"+key+".detconf"
        with open(nome,'w') as file:
            for row in range (value.shape[0]):
                for col in range (value.shape[1]):
                    out = str(value[row,col])+" "
                    file.write(out)
                
                file.write("\n")

if __name__ == "__main__":
    arquivos =[]
    for arquivo in os.listdir("/home/aquisicao/Documentos/PET_SYS/ROOT/CorFac"):
        if arquivo.find("BG")!=-1:
            arquivos.append(arquivo)

    separador = "groupdetector-"
    
    detectores = {
                    '0': [],
                    '1': []
                    #'2': [],
                    #'3': [],
    }
    
    for arquivo in arquivos:
        nome = arquivo.split(sep=separador)
        detector = nome[-1]
        detector = detector.split(sep=".")[0]
        mat = readConf(arquivo)
        mat = mat/mat.max()
        detectores[detector].append(mat)

    for key,value in detectores.items():
        data = np.zeros((8,8))
        count = 1
        for v in value:
            data+=v
            count +=1
        data = data/count
        detectores[key]=data

    writeConf(detectores)

    #b = readConf("/home/aquisicao/Documentos/PET_SYS/ROOT/CorFac/BG1-CorrFac_groupdetector-0.detconf")
