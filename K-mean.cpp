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
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille, K;
  
  if (argc != 4) 
     {
       printf("Usage: ImageIn.pgm ImageOut.pgm K\n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   sscanf (argv[2],"%s",cNomImgEcrite);
   sscanf (argv[3],"%d",&K);

   OCTET *ImgIn, *ImgOut;
   
   lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   int nTaille3 = nTaille * 3;
   allocation_tableau(ImgIn, OCTET, nTaille3);
   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille3);


   std::vector<couleurPoint> listeCentroide;
    listeCentroide.resize(K);

    std::vector<couleurPoint> listePrevCentroide;
    listePrevCentroide.resize(K);

    std::vector<std::vector<couleurPoint>> attache;
    attache.resize(nTaille3);

    std::vector<couleurPoint> moyenne;
    attache.resize(K);

    std::vector<float> distance;
    distance.resize(K);


    //for (int boucle = 0; boucle < nbBoucles; boucle++) {
    
    bool stopBoucle = false;
    int nbBoucle = 0;
    int cptPetisNb = 0;
    
    while (!stopBoucle) {

        if (nbBoucle > 0) {
            for (int i = 0; i < K; i++) {
                if (sqrt(pow(listePrevCentroide[i].r, 2) - pow(listeCentroide[i].r, 2)) < 10 || sqrt(pow(listePrevCentroide[i].g, 2) - pow(listeCentroide[i].g, 2)) < 10 || sqrt(pow(listePrevCentroide[i].b, 2) - pow(listeCentroide[i].b, 2)) < 10 ) {
                    cptPetisNb++;
                }
            }

            if (cptPetisNb > K/2) {
                stopBoucle = true;
            }
        }

        if (!stopBoucle) {

            for (int i = 0; i < K; i++) {
                couleurPoint couleur;
                couleur.r = std::rand()%255;
                couleur.g = std::rand()%255;
                couleur.b = std::rand()%255;
                listeCentroide.at(i) = couleur;
    
            }

            couleurPoint c;
            c.r = 0;
            c.g = 0;
            c.b = 0;

            for (int e = 0; e < K; e++) {
                moyenne.push_back(c);
            }

            int minDist;

            for (int i=0; i < nTaille3; i+=3) {

                float nR = ImgIn[i];
                float nG = ImgIn[i+1];
                float nB = ImgIn[i+2];


                for (int t = 0; t < K; t++) {
                    distance[t] = (sqrt(pow(nR-listeCentroide[t].r, 2) + pow(nG-listeCentroide[t].g, 2) + pow(nB-listeCentroide[t].b, 2)));
                }

                minDist = getMinId(distance);


                couleurPoint c;
                c.r = nR;
                c.g = nG;
                c.b = nB;

                attache[minDist].push_back(c);

                moyenne[minDist].r += ImgIn[i];
                moyenne[minDist].g += ImgIn[i+1];
                moyenne[minDist].b += ImgIn[i+2];

                ImgIn[i] = minDist;
                ImgIn[i+1] = minDist;
                ImgIn[i+2] = minDist;

            }

            for (int i=0; i < nTaille3; i+=3) {

                for (int elt = 0; elt < K; elt++) {

                    if (ImgIn[i] == elt && ImgIn[i+1] == elt && ImgIn[i+2] == elt) {

                        ImgIn[i] = moyenne[elt].r / attache[elt].size(); 
                        ImgIn[i+1] = moyenne[elt].g / attache[elt].size();
                        ImgIn[i+2] = moyenne[elt].b / attache[elt].size();

                        listeCentroide[elt].r = ImgIn[i];
                        listeCentroide[elt].g = ImgIn[i+1];
                        listeCentroide[elt].b = ImgIn[i+2];

                        listePrevCentroide[elt].r = listeCentroide[elt].r;
                        listePrevCentroide[elt].g = listeCentroide[elt].g;
                        listePrevCentroide[elt].b = listeCentroide[elt].b;

                    }
                }

            }

            nbBoucle++;
        }
    }

    std::cout << nbBoucle << std::endl;


    for (int i=0; i < nTaille3; i+=3) {
        if (ImgIn[i] < 0) {
            ImgOut[i] = 0;
        }
        else if (ImgIn[i] > 255) {
            ImgOut[i] = 255;
        }
        else {
            ImgOut[i] = ImgIn[i];
        }

        if (ImgIn[i+1] < 0) {
            ImgOut[i+1] = 0;
        }
        else if (ImgIn[i+1] > 255) {
            ImgOut[i+1] = 255;
        }
        else {
            ImgOut[i+1] = ImgIn[i+1];
        }

        if (ImgIn[i+2] < 0) {
            ImgOut[i+2] = 0;
        } 
        else if (ImgIn[i+2] > 255) {
            ImgOut[i+2] = 255;
        }
        else {
            ImgOut[i+2] = ImgIn[i+2];
        }

    }
    


   ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn);
   return 1;
}