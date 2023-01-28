// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"
#include <list>
#include <algorithm>
#include <iostream>
#include <vector>

struct couleurPoint {
    int r, g, b;
};

int getMinId(std::vector<float> list) {
    int min = 0;

    for (int i = 1; i < list.size(); i++) {
        if (list[i] < list[min]) {
            min = i;
        }
    }

    return min;
}

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgLue2[250];
  int nH, nW, nTaille, K, nbBoucles;
  
  if (argc != 3) 
     {
       printf("Usage: ImageIn.pgm ImageIn2.pgm \n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   sscanf (argv[2],"%s",cNomImgLue2) ;

   OCTET *ImgIn, *ImgIn2;
   
   lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   int nTaille3 = nTaille * 3;
   allocation_tableau(ImgIn, OCTET, nTaille3);
   allocation_tableau(ImgIn2, OCTET, nTaille3);
   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
   lire_image_ppm(cNomImgLue2, ImgIn2, nH * nW);

   float EQM = 0, PSNR;

    for (int i=0; i < nTaille3; i+=3) {
        EQM += pow((ImgIn[i]-ImgIn2[i]), 2) + pow((ImgIn[i+1]-ImgIn2[i+1]), 2) + pow((ImgIn[i+2]-ImgIn2[i+2]), 2);
    }

    EQM/= nTaille3;

    PSNR = 10*log10(pow(255, 2)/EQM);

    std::cout << EQM << " " << PSNR << std::endl;

   free(ImgIn);
   return 1;
}