#include <stdio.h>

typedef struct _pixel {
    // r - > red
    // g -> green
    // b -> blue
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} Pixel;

typedef struct _image {
    // [width][height][rgb]
    // 0 -> r
    // 1 -> g
    // 2 -> b
    unsigned short int pixel[512][512][3];
    unsigned int width;
    unsigned int height;
} Image;

Image readImg (Image img) {
  for (unsigned int cont = 0; cont < img.height; ++cont) {
      for (unsigned int contAux = 0; contAux < img.width; ++contAux) {
          scanf("%hu %hu %hu", &img.pixel[cont][contAux][0],
                               &img.pixel[cont][contAux][1],
                               &img.pixel[cont][contAux][2]);
    }
  }
  return img;
}
Image filtro_sepia(Image img) {
  int cont, contAux;
  for (cont = 0; cont < img.height; ++cont) {
      for (contAux = 0; contAux < img.width; ++contAux) {
          unsigned short int pixel[3];
          pixel[0] = img.pixel[cont][contAux][0];
          pixel[1] = img.pixel[cont][contAux][1];
          pixel[2] = img.pixel[cont][contAux][2];

          int pixelSepia =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
          int menor_r = (255 >  pixelSepia) ? pixelSepia : 255;
          img.pixel[cont][contAux][0] = menor_r;

          pixelSepia =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
          menor_r = (255 >  pixelSepia) ? pixelSepia : 255;
          img.pixel[cont][contAux][1] = menor_r;

          pixelSepia =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
          menor_r = (255 >  pixelSepia) ? pixelSepia : 255;
          img.pixel[cont][contAux][2] = menor_r;
      }
    }
    return img;
}
Image escala_de_cinza(Image img) {
    for (unsigned int cont = 0; cont < img.height; ++cont) {
        for (unsigned int contAux = 0; contAux < img.width; ++contAux) {
            int media = img.pixel[cont][contAux][0] +
                        img.pixel[cont][contAux][1] +
                        img.pixel[cont][contAux][2];
            media /= 3;
            img.pixel[cont][contAux][0] = media;
            img.pixel[cont][contAux][1] = media;
            img.pixel[cont][contAux][2] = media;
        }
    }

    return img;
}

Image blur(Image img) { //MUITOS PARAMETROS
    int tamanho = 0;
    scanf("%d", &tamanho);
    Image pixel;
    for (unsigned int cont = 0; cont < img.height; ++cont) {
        for (unsigned int contAux = 0; contAux < img.width; ++contAux) {
            Pixel media = {0, 0, 0};

            int menor_h = (img.height - 1 > cont + tamanho/2) ? cont + tamanho/2 : img.height - 1;
            int min_w = (img.width - 1 > contAux + tamanho/2) ? contAux + tamanho/2 : img.width - 1;
            for(int newCont = (0 > cont - tamanho/2 ? 0 : cont - tamanho/2); newCont <= menor_h; ++newCont) {
                for(int newContAux = (0 > contAux - tamanho/2 ? 0 : contAux - tamanho/2); newContAux <= min_w; ++newContAux) {
                    media.r += img.pixel[newCont][newContAux][0];
                    media.g += img.pixel[newCont][newContAux][1];
                    media.b += img.pixel[newCont][newContAux][2];
                }
            }
            // printf("%u", media.r)
            media.r /= tamanho * tamanho;
            media.g /= tamanho * tamanho;
            media.b /= tamanho * tamanho;

            img.pixel[cont][contAux][0] = media.r;
            img.pixel[cont][contAux][1] = media.g;
            img.pixel[cont][contAux][2] = media.b;
        }
    }
    return img;
}

Image rotacionar90direita(Image img) {
    Image rotacionada;

    rotacionada.width = img.height;
    rotacionada.height = img.width;
    int quantas_vezes = 0;
    scanf("%d", &quantas_vezes);
    quantas_vezes %= 4;
    for (int j = 0; j < quantas_vezes; ++j) {
      for (unsigned int i = 0, y = 0; i < rotacionada.height; ++i, ++y) {
          for (int j = rotacionada.width - 1, x = 0; j >= 0; --j, ++x) {
              rotacionada.pixel[i][j][0] = img.pixel[x][y][0];
              rotacionada.pixel[i][j][1] = img.pixel[x][y][1];
              rotacionada.pixel[i][j][2] = img.pixel[x][y][2];
        }
      }
    }
    return rotacionada;
}

Image espelhamento(Image img) {
  int horizontal = 0;
  scanf("%d", &horizontal);
  int width = img.width, height = img.height;
  if (horizontal == 1) width /= 2;
  else height /= 2;
  for (int cont = 0; cont < height; ++cont) {
      for (int contAux = 0; contAux < width; ++contAux) {
          int referenceCont = cont, referenceContAux = contAux;

          if (horizontal == 1) referenceContAux = img.width - 1 - contAux;
          else referenceCont = img.height - 1 - cont;

          Pixel pixelAux;
          pixelAux.r = img.pixel[cont][contAux][0];
          pixelAux.g = img.pixel[cont][contAux][1];
          pixelAux.b = img.pixel[cont][contAux][2];

          img.pixel[cont][contAux][0] = img.pixel[referenceCont][referenceContAux][0];
          img.pixel[cont][contAux][1] = img.pixel[referenceCont][referenceContAux][1];
          img.pixel[cont][contAux][2] = img.pixel[referenceCont][referenceContAux][2];

          img.pixel[referenceCont][referenceContAux][0] = pixelAux.r;
          img.pixel[referenceCont][referenceContAux][1] = pixelAux.g;
          img.pixel[referenceCont][referenceContAux][2] = pixelAux.b;
      }
  }
  return img;
}
Image inverter_cores(Image img) {
    for (unsigned int cont = 0; cont < img.height; ++cont) {
        for (unsigned int contAux = 0; contAux < img.width; ++contAux) {
            img.pixel[cont][contAux][0] = 255 - img.pixel[cont][contAux][0];
            img.pixel[cont][contAux][1] = 255 - img.pixel[cont][contAux][1];
            img.pixel[cont][contAux][2] = 255 - img.pixel[cont][contAux][2];
        }
    }
    return img;
}

Image cortar_imagem(Image img) {
    Image cortada;
    int x, y, widthCut, heightCut;
    scanf("%d %d %d %d", &x, &y, &widthCut, &heightCut);

    cortada.width = widthCut;
    cortada.height = heightCut;

    for(int cont = 0; cont < heightCut; ++cont) {
        for(int contAux = 0; contAux < widthCut; ++contAux) {
            cortada.pixel[cont][contAux][0] = img.pixel[cont + y][contAux + x][0];
            cortada.pixel[cont][contAux][1] = img.pixel[cont + y][contAux + x][1];
            cortada.pixel[cont][contAux][2] = img.pixel[cont + y][contAux + x][2];
        }
    }

    return cortada;
}

void showImage (Image img) {
  for (unsigned int cont = 0; cont < img.height; ++cont) {
      for (unsigned int contAux = 0; contAux < img.width; ++contAux) {
          printf("%hu %hu %hu ", img.pixel[cont][contAux][0],
                                 img.pixel[cont][contAux][1],
                                 img.pixel[cont][contAux][2]);

      }
      printf("\n");
  }

}
int main() {
    Image img;
    // read type of image
    char typeImg[4];
    scanf("%s", typeImg);
    // read width height and color of image
    scanf("%u %u", &img.width, &img.height);
    // read all pixels of image
    img = readImg(img);

    int n_opcoes; //VER OQ É
    scanf("%d", &n_opcoes);

    for(int cont = 0; cont < n_opcoes; ++cont) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { // Escala de Cinza
                img = escala_de_cinza(img);
                break;
            }
            case 2: { // Filtro Sepia
                  img = filtro_sepia(img);
                  break;
            }
            case 3: { // Blur
                img = blur(img);
                break;
            }
            case 4: { // Rotacao
                img = rotacionar90direita(img);
                break;
            }
            case 5: { // Espelhamento
              img = espelhamento(img);
              break;
            }
            case 6: { // Inversao de Cores
                img =  inverter_cores(img);
                break;
            }
            case 7: { // Cortar Imagem
                break;
                img = cortar_imagem(img);
            }
        }
    }

    // print type of image
    printf("P3\n");
    // print width height and color of image
    printf("%u %u\n255\n", img.width, img.height);
    // print pixels of image
    showImage(img);
    return 0;
}
