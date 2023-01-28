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

    std::vector<std::vector<int>> listeCentroide;
    listeCentroide.resize(256);

    for (int i = 0; i < 256; i++) {
        listeCentroide.at(i) = {std::rand()%255, std::rand()%255, std::rand()%255};

        //std::cout << listeCentroide.at(i) << " " << listeCentroide.at(i+1) << " " << listeCentroide.at(i+2) << std::endl;
    }

    std::vector<std::vector<couleurPoint>> listeAttacheCentroide;
    listeAttacheCentroide.resize(nTaille3);

    std::vector<std::vector<float>> listeMoyenneCentroide;
    listeMoyenneCentroide.resize(nTaille3);

    //for (int elt = 0; elt < 3; elt++) {
        couleurPoint moyenneC1 = {0, 0, 0};
        couleurPoint moyenneC2 = {0, 0, 0};

        for (int i=0; i < nTaille3; i+=3) {
            float nR = ImgIn[i];
            float nG = ImgIn[i+1];
            float nB = ImgIn[i+2];

            float minDist;

            for (int j = 0; j < 256; j+=3) {
                float distActuelle;

                if (j == 0) {
                    minDist = sqrt(pow(nR-listeCentroide.at(j)[0], 2) + pow(nG-listeCentroide.at(j)[1], 2) + pow(nB-listeCentroide.at(j)[2], 2));
                }
                else {
                    distActuelle = sqrt(pow(nR-listeCentroide.at(j)[0], 2) + pow(nG-listeCentroide.at(j)[1], 2) + pow(nB-listeCentroide.at(j)[2], 2));
                    
                    if (distActuelle < minDist) {
                        minDist = distActuelle;
                    }

                }
            }

            couleurPoint c;
            c.r = nR;
            c.g = nG;
            c.b = nB;

            listeAttacheCentroide.at(minDist).push_back(c);

            listeMoyenneCentroide[minDist][i] += ImgIn[i];
            listeMoyenneCentroide[minDist][i+1] += ImgIn[i+1];
            listeMoyenneCentroide[minDist][i+2] += ImgIn[i+2];

            std::cout << "test" << std::endl;

            ImgIn[i] = minDist;
            ImgIn[i+1] = minDist;
            ImgIn[i+2] = minDist;
        }

        for (int i=0; i < nTaille3; i+=3) {
            int cpt = 0;

            if (ImgIn[i] == cpt && ImgIn[i+1] == cpt && ImgIn[i+2] == cpt) {
                ImgIn[i] = listeMoyenneCentroide[cpt][i] / listeAttacheCentroide[cpt].size(); 
                ImgIn[i+1] = listeMoyenneCentroide[cpt][i+1] / listeAttacheCentroide[cpt].size();
                ImgIn[i+2] = listeMoyenneCentroide[cpt][i+2] / listeAttacheCentroide[cpt].size();

                listeCentroide[i][0] = ImgIn[i];
                listeCentroide[i][1] = ImgIn[i+1];
                listeCentroide[i][2] = ImgIn[i+2];

            }

            cpt++;

        }
    //}


    for (int i=0; i < nTaille3; i+=3) {
        ImgOut[i] = ImgIn[i];
        ImgOut[i+1] = ImgIn[i+1];
        ImgOut[i+2] = ImgIn[i+2];
    }
    


   ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn);
   return 1;
}