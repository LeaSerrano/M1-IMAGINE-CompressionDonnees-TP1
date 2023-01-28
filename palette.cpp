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

bool listContains(int nr, int ng, int nb, std::vector<couleurPoint> listCouleur) {
    for (int i = 0; i < listCouleur.size(); i++) {
        if (nr == listCouleur[i].r && ng == listCouleur[i].g && nb == listCouleur[i].b) {
            return true;
        }
    }

    return false;
}

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250], cNomImgEcrite2[250];
  int nH, nW, nTaille, K;
  
  if (argc != 5) 
     {
       printf("Usage: ImageIn.pgm ImageOut1.pgm ImageOut2.pgm K \n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   sscanf (argv[2],"%s",cNomImgEcrite);
   sscanf (argv[3],"%s",cNomImgEcrite2);
   sscanf (argv[4],"%i",&K);

   OCTET *ImgIn, *ImgOut, *ImgOut2;
   
   lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;

   int nTaille2563 = K * 3;
  
   int nTaille3 = nTaille * 3;
   allocation_tableau(ImgIn, OCTET, nTaille3);
   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille3);
   allocation_tableau(ImgOut2, OCTET, nTaille2563);

    std::vector<couleurPoint> listCouleur;

    for (int i=0; i < nTaille3; i+=3) {
        float nR = ImgIn[i];
        float nG = ImgIn[i+1];
        float nB = ImgIn[i+2];

        couleurPoint c;
        c.r = nR;
        c.g = nG;
        c.b = nB;

        if (i == 0) {
            listCouleur.push_back(c);
        }

        else if (!listContains(nR, nG, nB, listCouleur)) {
            listCouleur.push_back(c);
        }

    }

    std::vector<int> listCouleurNB;

    for (int i = 0; i < listCouleur.size(); i++) {

        if (i == 0) {
            listCouleurNB.push_back(0);
        }
        else {
            listCouleurNB.push_back(255/i);
        }
    }

    for (int i=0; i < nTaille3; i+=3) {
        float nR = ImgIn[i];
        float nG = ImgIn[i+1];
        float nB = ImgIn[i+2];

        couleurPoint c;
        c.r = nR;
        c.g = nG;
        c.b = nB;

        for (int j = 0; j < listCouleurNB.size(); j++) {
            if (listCouleur[j].r == nR && listCouleur[j].g == nG && listCouleur[j].b == nB) {

                ImgOut[i] = listCouleurNB[j];
                ImgOut[i+1] = listCouleurNB[j];
                ImgOut[i+2] = listCouleurNB[j];
            }
        }
    }
    
    /*for (int i = 0; i < nTaille3; i+=3) {
        double index = (double)i/(nTaille3/K);

        std::cout << floor(index) << std::endl;

        ImgOut2[i] = listCouleur[floor(index)].r;
        ImgOut2[i+1] = listCouleur[floor(index)].g;
        ImgOut2[i+2] = listCouleur[floor(index)].b;
    }*/

    /*for (int i = 0; i < nTaille2563; i+=3) {
        double index = (double)i/(nTaille3/K);

        std::cout << floor(index) << std::endl;

        std::cout << i << " " << i/3 << std::endl;

        ImgOut2[i] = listCouleur[i/3].r;
        ImgOut2[i+1] = listCouleur[i/3].g;
        ImgOut2[i+2] = listCouleur[i/3].b;
    }*/

    for (int i=0; i < K*3; i+=3){ 

        std::cout << i << " " << i/3 << std::endl;
        ImgOut2[i] = listCouleur[i/3].r;
        ImgOut2[i+1] = listCouleur[i/3].g;
        ImgOut2[i+2] = listCouleur[i/3].b;
   } 


   ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
   ecrire_image_ppm(cNomImgEcrite2, ImgOut2,  16, 16);
   free(ImgIn); free(ImgOut); free(ImgOut2);
   return 1;
}