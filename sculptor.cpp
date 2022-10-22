#include "sculptor.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <vector>

using namespace std;

Sculptor::Sculptor(int _nx, int _ny, int _nz){
    nx = _nx; ny = _ny; nz = _nz;
    r = 1; g= 1; b = 1;
    // Alocação dinâmica da matriz 3d
    v = new Voxel**[nx];
     for (int i =0; i<nx; i++){
      v[i] = new Voxel*[ny];

        for (int j =0; j<ny; j++){
       v[i][j]= new Voxel[nz];
      }
    }
}

Sculptor::~Sculptor(){
    for(int i = 0; i < nx; i++){
        for (int j = 0; j < ny; j++){
            delete[] v[i][j];
        }
    }
    for (int i=0; i <nx; i++){
        delete[] v[i];
    }

    delete[] v;
    //Resetando coordenadas
    nx = 0; ny = 0; nz = 0;
}

void Sculptor::setColor(float r, float g, float b, float a){
    //O usuário define as propriedades do voxel
    Sculptor::r = r; Sculptor::g = g; Sculptor::b = b; Sculptor::a = a;
}

void Sculptor::putVoxel(int x, int y, int z){
    //insere um voxel
    v[x][y][z].isOn = true;
    //repassa cor e transparencia do voxel
    v[x][y][z].r = r; v[x][y][z].g = g; v[x][y][z].b = b; v[x][y][z].a = a;

}

void Sculptor::cutVoxel(int x, int y, int z){
    v[x][y][z].isOn = false;
}

void Sculptor::putBox(int x0, int x1, int y0, int y1, int z0, int z1){
    //Define uma sequencia de voxel
    for(int x = x0; x<x1; x++){
       for (int y = y0; y<y1; y++){
           for (int z = z0; z<z1; z++){
               v[x][y][z].isOn=true;
               v[x][y][z].r = r;
               v[x][y][z].g = g;
               v[x][y][z].b = b;
               v[x][y][z].a = a;
           }
       }
    }
}

void Sculptor::cutBox(int x0, int x1, int y0, int y1, int z0, int z1){

    //Desativa uma sequencia de voxel
    for(int x = x0; x<x1; x++){
       for (int y = y0; y<y1; y++){
           for (int z = z0; z<z1; z++){
               v[x][y][z].isOn = false;
           }
       }
    }
}

void Sculptor::putSphere(int xcenter, int ycenter, int zcenter, int radius){
//Recebe as coordenadas e o raio da esfera

  int i, j, k;

  for( i = -radius; i<=radius; i++){ //pecorre o diametro
      for(j = -radius; j<=radius; j++){
          for(k = -radius; k<=radius; k++){

            if ((i*i+j*j+k*k) < radius*radius){ //condição da esfera
              putVoxel(i+xcenter,j+ycenter,k+zcenter); //adiciona um voxel
            }
          }
      }
  }
}

void Sculptor::cutSphere(int xcenter, int ycenter, int zcenter, int radius){
//Mesma lógica do putSphere, mas ao invés de ativar o voxel, ele desativa
//Desativa a esfera
  int i, j, k;

  for(i = -radius; i<radius; i++){
      for(j = -radius; j<radius; j++){
          for(k = -radius; k<radius; k++){
            if((i*i+j*j+k*k) < radius*radius){
              cutVoxel(i+xcenter,j+ycenter,k+zcenter);
            }
          }
      }
  }
}

void Sculptor::putEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz){
  //Define uma elipsoide
   //recebe as coordenadas das dimensões e coordenadas do centro
   float newx, newy, newz;
   int x, y, z;

    for ( x = 0; x < nx; x++){
        for ( y = 0; y < ny; y++){
            for ( z = 0; z < nz; z++){
            newx = ((float)(x-xcenter)*(float)(x-xcenter))/(rx * rx);
            newy = ((float)(y-ycenter)*(float)(y-ycenter))/(ry * ry);
            newz = ((float)(z-zcenter)*(float)(z-zcenter))/(rz * rz);
//calculo da condição de existencia da elipsoid
            if ((newx + newy + newz) < 1){
                putVoxel(x,y,z);
            }
            }
        }
    }
}

void Sculptor::cutEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz){
 //Desativa a elipsoid

   double newx, newy, newz;
   int x, y, z;

    for ( x = 0; x < nx; x++){
        for ( y = 0; y < ny; y++){
            for ( z = 0; z < nz; z++){
            newx = ((float)(x-xcenter)*(float)(x-xcenter))/(rx * rx);
            newy = ((float)(y-ycenter)*(float)(y-ycenter))/(ry * ry);
            newz = ((float)(z-zcenter)*(float)(z-zcenter))/(rz * rz);

            if ((newx + newy + newz) < 1){
                cutVoxel(x,y,z);
            }
            }
        }
    }
}

void Sculptor::writeOFF( char *filename){
    int qtd_Voxel = 0;
    int ref;
    int  i, j, k, a, b, c; //auxiliares
    float fix = 0.5;
    std::ofstream endArq;
    endArq.open(filename);
    if (endArq.is_open()){
        endArq<<"OFF\n"; //Define off na primeira linha

        // Percorre todas as dimensoes verificando os voxel em exibiçao
        for (i = 0; i < nx; i++){
            for (j = 0; j <ny; j++){
                for (k = 0; k <nz; k++){
                    if(v[i][j][k].isOn == true){
                    qtd_Voxel++; //Guarda a quantidade de voxel ativos
                    }
                }
            }
        }
        endArq<<qtd_Voxel * 8<<" "<<qtd_Voxel * 6 << " " << "0" << "\n";
        // exibe a quantidade total de vertices, faces e arestas

        //Pecorre as posições a, b, c e exibe as coordenadas do voxel
        //No primeiro voxel o fix será o valor de referencia
        for (a = 0; a < nx; a++){
            for (b = 0; b < ny; b++){
                for (c = 0; c < nz; c++){
                    if(v[a][b][c].isOn == true){
                        endArq << a - fix << " " << b + fix << " " << c - fix << "\n" << flush;
                        endArq << a - fix << " " << b - fix << " " << c - fix << "\n" << flush;
                        endArq << a + fix << " " << b - fix << " " << c - fix << "\n" << flush;
                        endArq << a + fix << " " << b + fix << " " << c - fix << "\n" << flush;
                        endArq << a - fix << " " << b + fix << " " << c + fix << "\n" << flush;
                        endArq << a - fix << " " << b - fix << " " << c + fix << "\n" << flush;
                        endArq << a + fix << " " << b - fix << " " << c + fix << "\n" << flush;
                        endArq << a + fix << " " << b + fix << " " << c + fix << "\n" << flush;
                    }
                }
            }
        }

        qtd_Voxel = 0; //Reseta a quantidade de voxel

        // percorre todas as dimensoes
        // Verifica os voxel ativos e exibe as linhas com numeros de faces e as combinacoes que exibem a face
        for (a= 0; a<nx; a++){
            for (b = 0; b<ny; b++){
                for (c= 0; c<nz; c++){
                    if(v[a][b][c].isOn == true){
                    ref = qtd_Voxel * 8;
                    endArq << fixed;

                    // Construir linha que monta as faces a partir do vertices:
                    //Valores exemplos do site quando ref = 0:
                    endArq << "4" << " " << 0+ref << " " << 3+ref << " " << 2+ref << " " << 1+ref << " ";
                    //exibindo propriedades do voxel:
                    endArq << setprecision(2)<<v[a][b][c].r << " " << setprecision(2)<<v[a][b][c].g <<" " << setprecision(2)<<v[a][b][c].b << " " << setprecision(2) << v[a][b][c].a << "\n";

                    endArq << "4" << " " << 4+ref << " " << 5+ref << " " << 6+ref << " " << 7+ref << " ";
                    endArq << setprecision(2)<<v[a][b][c].r << " " << setprecision(2)<<v[a][b][c].g <<" " << setprecision(2)<<v[a][b][c].b << " " << setprecision(2) << v[a][b][c].a << "\n";

                    endArq << "4" << " " << 0+ref << " " << 1+ref << " " << 5+ref << " " << 4+ref << " ";
                    endArq << setprecision(2)<<v[a][b][c].r << " " << setprecision(2)<<v[a][b][c].g <<" " << setprecision(2)<<v[a][b][c].b << " " << setprecision(2) << v[a][b][c].a << "\n";

                    endArq << "4" << " " << 0+ref << " " << 4+ref << " " << 7+ref << " " << 3+ref << " ";
                    endArq << setprecision(2)<<v[a][b][c].r << " " << setprecision(2)<<v[a][b][c].g <<" " << setprecision(2)<<v[a][b][c].b << " " << setprecision(2) << v[a][b][c].a << "\n";

                    endArq << "4" << " " << 3+ref << " " << 7+ref << " " << 6+ref << " " << 2+ref << " ";
                    endArq << setprecision(2)<<v[a][b][c].r << " " << setprecision(2)<<v[a][b][c].g <<" " << setprecision(2)<<v[a][b][c].b << " " << setprecision(2) << v[a][b][c].a << "\n";

                    endArq << "4" << " " << 1+ref << " " << 2+ref << " " << 6+ref << " " << 5+ref << " ";
                    endArq << setprecision(2)<<v[a][b][c].r << " " << setprecision(2)<<v[a][b][c].g <<" " << setprecision(2)<<v[a][b][c].b << " " << setprecision(2) << v[a][b][c].a << "\n";

                    qtd_Voxel++; //incrementa a qtd de voxels
                    }
                }
            }
        }
    }
    else{
    cout << "Erro ao abrir arquivo de texto.";
    }

    endArq.close();
}
