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

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille;
  
  if (argc != 3) 
     {
       printf("Usage: ImageIn.pgm ImageOut.pgm \n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   sscanf (argv[2],"%s",cNomImgEcrite);

   OCTET *ImgIn, *ImgOut;
   
   lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   int nTaille3 = nTaille * 3;
   allocation_tableau(ImgIn, OCTET, nTaille3);
   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille3);
   

    couleurPoint C1 = {139,127,115};
    couleurPoint C2 = {70,41,23};

    std::vector<couleurPoint> attacheC1;
    std::vector<couleurPoint> attacheC2;

    for (int elt = 0; elt < 3; elt++) {
        couleurPoint moyenneC1 = {0, 0, 0};
        couleurPoint moyenneC2 = {0, 0, 0};

        for (int i=0; i < nTaille3; i+=3) {
            float nR = ImgIn[i];
            float nG = ImgIn[i+1];
            float nB = ImgIn[i+2];

            float distC1 = sqrt(pow(nR-C1.r, 2) + pow(nG-C1.g, 2) + pow(nB-C1.b, 2));
            float distC2 = sqrt(pow(nR-C2.r, 2) + pow(nG-C2.g, 2) + pow(nB-C2.b, 2));

            if (distC1 < distC2) {
                couleurPoint c1;
                c1.r = nR;
                c1.g = nG;
                c1.b = nB;

                attacheC1.push_back(c1);

                moyenneC1.r += ImgIn[i];
                moyenneC1.g += ImgIn[i+1];
                moyenneC1.b += ImgIn[i+2];

                ImgIn[i] = 0;
                ImgIn[i+1] = 0;
                ImgIn[i+2] = 0;
            }
            else {
                couleurPoint c2;
                c2.r = nR;
                c2.g = nG;
                c2.b = nB;

                attacheC2.push_back(c2);

                moyenneC2.r += ImgIn[i];
                moyenneC2.g += ImgIn[i+1];
                moyenneC2.b += ImgIn[i+2];

                ImgIn[i] = 1;
                ImgIn[i+1] = 1;
                ImgIn[i+2] = 1;
            }
        }

        for (int i=0; i < nTaille3; i+=3) {

            if (ImgIn[i] == 0 && ImgIn[i+1] == 0 && ImgIn[i+2] == 0) {
                ImgIn[i] = moyenneC1.r / attacheC1.size(); 
                ImgIn[i+1] = moyenneC1.g / attacheC1.size();
                ImgIn[i+2] = moyenneC1.b / attacheC1.size();

                C1.r = ImgIn[i];
                C1.g = ImgIn[i+1];
                C1.b = ImgIn[i+2];

            }

            else if (ImgIn[i] == 1 && ImgIn[i+1] == 1 && ImgIn[i+2] == 1)  {
                ImgIn[i] = moyenneC2.r / attacheC2.size(); 
                ImgIn[i+1] = moyenneC2.g / attacheC2.size();
                ImgIn[i+2] = moyenneC2.b / attacheC2.size();

                C2.r = ImgIn[i];
                C2.g = ImgIn[i+1];
                C2.b = ImgIn[i+2];

            }

        }
    }


    for (int i=0; i < nTaille3; i+=3) {
        ImgOut[i] = ImgIn[i];
        ImgOut[i+1] = ImgIn[i+1];
        ImgOut[i+2] = ImgIn[i+2];
    }
    


   ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn);
   return 1;
}