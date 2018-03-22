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
  for (unsigned int i = 0; i < img.height; ++i) {
      for (unsigned int j = 0; j < img.width; ++j) {
          scanf("%hu %hu %hu", &img.pixel[i][j][0],
                               &img.pixel[i][j][1],
                               &img.pixel[i][j][2]);
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

void blur(Image img) { //MUITOS PARAMETROS
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
}

Image rotacionar90direita(Image img) {
    int n_rotacao = 0;
    scanf("%d", &n_rotacao);
    n_rotacao %= 4;
    Image rotacionada;
    for (int contRotacao = 0; contRotacao < n_rotacao; ++contRotacao) {
      rotacionada.width = img.height;
      rotacionada.height = img.width;
      for (unsigned int cont = 0, y = 0; cont < rotacionada.height; ++cont, ++y) {  // y?? x?? olhar codigo antigo
          for (unsigned int contAux = rotacionada.width - 1, x = 0; contAux >= 0; --contAux, ++x) {
              rotacionada.pixel[cont][contAux][0] = img.pixel[x][y][0];
              rotacionada.pixel[cont][contAux][1] = img.pixel[x][y][1];
              rotacionada.pixel[cont][contAux][2] = img.pixel[x][y][2];
          }
      }
    }
    return rotacionada;
}

Image espelhamento(Image img) {
  int horizontal;
  scanf("%d", &horizontal);
  int widthAux = img.width, heightAux = img.height;
  if (horizontal == 1) widthAux /= 2;
  else heightAux /= 2;

  for (int cont = 0; cont < img.height; ++cont) {
      for (int contAux = 0; contAux < img.width; ++contAux) {
          int invertHeight = cont, invertWidth = contAux;

          if (horizontal == 1) invertWidth = img.width - 1 - contAux;
          else invertHeight = img.height - 1 - cont;

          Pixel aux1;
          aux1.r = img.pixel[cont][contAux][0];
          aux1.g = img.pixel[cont][contAux][1];
          aux1.b = img.pixel[cont][contAux][2];

          img.pixel[cont][contAux][0] = img.pixel[invertHeight][invertWidth][0];
          img.pixel[cont][contAux][1] = img.pixel[invertHeight][invertWidth][1];
          img.pixel[cont][contAux][2] = img.pixel[invertHeight][invertWidth][2];

          img.pixel[invertHeight][invertWidth][0] = aux1.r;
          img.pixel[invertHeight][invertWidth][1] = aux1.g;
          img.pixel[invertHeight][invertWidth][2] = aux1.b;
      }
  }
  return img;
}
void inverter_cores(Image img) {
    for (unsigned int cont = 0; cont < img.height; ++cont) {
        for (unsigned int contAux = 0; contAux < img.width; ++contAux) {
            img.pixel[cont][contAux][0] = 255 - img.pixel[cont][contAux][0];
            img.pixel[cont][contAux][1] = 255 - img.pixel[cont][contAux][1];
            img.pixel[cont][contAux][2] = 255 - img.pixel[cont][contAux][2];
        }
    }
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
    int max_color;
    scanf("%u %u %d", &img.width, &img.height, &max_color);

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
                blur(img);
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
                inverter_cores(img);
                break;
            }
            case 7: { // Cortar Imagem
                img = cortar_imagem(img);
                break;
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
