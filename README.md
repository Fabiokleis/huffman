# huffman
implementacao da arvore de huffman.

## makefile

### build
gera o arquivo binario huff:
```shell
make
```

### clean
remove todos os arquivos compilados:
```shell
make clean
```

### run
```shell
./huff
```
para checar o arquivo gerado `code.h4k`:
```shell
xxd -b code.h4k
```


### resultados

`resultado.bmp` grid 4x4 e 12 cores kmeans:


![image](https://github.com/user-attachments/assets/3977b154-d580-442d-8847-dcef59ff0088)

(1 - (((82 + 224) / 1024) / 2.8)) * 100 = 89.32756696428571

89.32% de compressão

![image](https://raw.githubusercontent.com/Fabiokleis/huffman/refs/heads/main/wind_waker.bmp)
![image](https://raw.githubusercontent.com/Fabiokleis/huffman/refs/heads/main/resultado.bmp)
