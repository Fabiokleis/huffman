import cv2

# Carrega a imagem
imagem = cv2.imread('wind_waker.bmp')

# Verifica se a imagem foi carregada corretamente
if imagem is not None:
    
    # Percorre as células do primeiro grid 4x4
    grid = imagem[0:4, 0:4]
    print(grid)
    cv2.imshow('Grid 4x4', grid)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
else:
    print('Erro ao carregar a imagem.')