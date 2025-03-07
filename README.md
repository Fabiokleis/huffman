# huffman
implementacao da arvore de huffman e kmeans para compressao e descompressao lossy de imagens.

## makefile

### build
gera o arquivo binario `huff` e `descompactador`:
```shell
make
```

### clean
remove todos os arquivos compilados:
```shell
make clean
```

### run
compacta um arquivo .bmp gerando 2 arquivos de saida, `code.h4k` e `tree.bin`:
```shell
./huff
```
descompacta decodificando os arquivos `tree.bin` e `code.h4k`, e escreve um arquivo `resultado.bmp`:
```shell
./descompactador
```


### resultados

`resultado.bmp` grid 4x4 e 12 cores kmeans:


![image](https://github.com/user-attachments/assets/3977b154-d580-442d-8847-dcef59ff0088)

(1 - (((82 + 224) / 1024) / 2.8)) * 100 = 89.32756696428571

89.32% de compressão

![image](https://raw.githubusercontent.com/Fabiokleis/huffman/refs/heads/main/wind_waker.bmp)
![image](https://raw.githubusercontent.com/Fabiokleis/huffman/refs/heads/main/resultado.bmp)
