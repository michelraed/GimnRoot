#!/home/gimn6/miniconda3/envs/ia/bin/python

import argparse
import uproot as up

def generate_conf(chipIDs:list,porta:list,hmin:int,hmax:int,positioning:str,cristal:str,ftrim:float)->None:
    '''
        Esta função irá gerar o arquivo de configuração a ser utilizado no software do sistema PETSYS
            chipIDs : vetor contendo todos os chipIDs respectivos aos detectores ativos durante a aquisição
            porta: vetor contendo todas as portas em utilização durante o momento da quisicao
            hmin: valor minimo dos histogramas
            hmax: valor maximo dos histogramas 
            positioning: o metodo refere-se ao tipo de avaliação da posição a ser utilizado, no momento temos 
            implementado somente o metodo de anger
            cristal: tipo de cristal a  ser utilizado o qual pode ser monolitico ou segmentado
            ftrim: parametro de otimização do metodo proposto por bonifacio
        
    '''

    #cria o cabeçalio
    header = ["port","chipID","inCoincidence","algorithm","type","hMin","hMax","ftrim","name\n"]
    head = "\t".join(header) # junta o mesmo
    #abre o arquivo para se salvar a configuração
    with open("configuracao.conf",'w') as file:
        file.write(head)
        for i,porta in enumerate(portas):
            line = f"{porta}\t{chipIDs[i]}\t15\t{positioning}\t{cristal}\t{hmin}\t{hmax}\t{ftrim}\tdetector-{i+1}\n"
            file.write(line)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='''
        Esta função irá gerar o arquivo de configuração a ser utilizado no software do sistema PETSYS
            chipIDs : vetor contendo todos os chipIDs respectivos aos detectores ativos durante a aquisição
            porta: vetor contendo todas as portas em utilização durante o momento da quisicao
            hmin: valor minimo dos histogramas
            hmax: valor maximo dos histogramas 
            positioning: o metodo refere-se ao tipo de avaliação da posição a ser utilizado, no momento temos 
            implementado somente o metodo de anger
            cristal: tipo de cristal a  ser utilizado o qual pode ser monolitico ou segmentado
            ftrim: parametro de otimização do metodo proposto por bonifacio''')


    parser.add_argument('--file', type=str, help='Arquivo de grupos para geração do arquivo de configuração')
    parser.add_argument('--hmin', type=float, default=0.0, help='Valor mínimo dos histogramas')
    parser.add_argument('--hmax', type=float, default=1.0, help='Valor máximo dos histogramas')
    parser.add_argument('--positioning', type=str, default='Anger', help='Algoritmo de posicionamento utilizado')
    parser.add_argument('--crystal', type=str, default='seg', help='tipo de cristal :  mono , seg')
    parser.add_argument('--ftrim', type=float, default=0.25, help='parametro de ajuste para o posicionamento do bonifacio')
    
    args = parser.parse_args()

    file_name = args.file
    hmin = args.hmin
    hmax = args.hmax
    positioning = args.positioning
    crystal = args.crystal
    ftrim = args.ftrim

    if file_name is not None:
        file = up.open(file_name) 
        data = file['data']
        dados = data.arrays(['mh_n','mh_j','energy','time','channelID'],library='pd')
        dados['chipID'] = dados['channelID']//64
        dados['portID'] = dados['channelID']//128
        portas = dados['portID'].unique()
        chipIDs = dados['chipID'].unique()
        generate_conf(chipIDs,portas,hmin,hmax,positioning,crystal,ftrim)

    else : 
        print(" Nenhum arquivo foi selecionado")