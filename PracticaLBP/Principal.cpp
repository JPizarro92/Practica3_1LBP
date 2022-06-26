#include <iostream>
#include <dirent.h>


#include <opencv2/core/core.hpp> // Contiene las definiciones base de matrices y estructuras
#include <opencv2/highgui/highgui.hpp> // Interfaz gráfica de usuario
#include <opencv2/imgproc/imgproc.hpp> // Procesamiento de imágenes
#include <opencv2/imgcodecs/imgcodecs.hpp> // Códecs para leer los distintos formatos de imágenes (JPG, PNG, etc.)
#include <opencv2/video/video.hpp> // Lectura de vídeos
#include <opencv2/videoio/videoio.hpp> // Lectura y escritura de videos
#include <opencv2/objdetect/objdetect.hpp> // Para realizar detección de objetos

#include "Imagen.hpp"


string carpetas[3] = { "clase1/","clase2/", "test/"};
int ancho = 350;
int alto = 225;

vector<string> getArchivos(string ruta) {
	vector<string>archivos;
	DIR* direccion;
	struct dirent* elementos;
	if (direccion = opendir(ruta.c_str())) {
		while (elementos = readdir(direccion)) {
			string elemento = elementos->d_name;
			if (elemento != "." && elemento != ".." && elemento.length() > 2) {
				//cout << elemento << endl;
				archivos.push_back(elemento);
			}
		}
	}
	closedir(direccion);
	return archivos;
}

vector<Imagen> cargarImagenes(string carpeta, vector<string> files, string documento) {
	vector<Imagen>	imgs;
	for (int i = 0; i < files.size(); i++) {
		Imagen nImagen;
		Mat imgAux = imread(carpeta + files[i]);
		resize(imgAux, imgAux, Size(ancho, alto), INTER_LINEAR);
		nImagen.original = imgAux;
		nImagen.generarCanales();
		nImagen.calculaHistogramas();
		nImagen.guardarHistograma(documento);
		imgs.push_back(nImagen);
	}
	return imgs;
}

double calcularDistanciaEuclidia(Imagen &obj1, Imagen &obj2) {
	double dis_euclidea = 0;
	double resulB = 0;
	double resulG = 0;
	double resulR = 0;

	for (int i = 0; i < 255; i++) {
		resulB = pow((obj1.histo_b.at(i) - obj2.histo_b.at(i)), 2) + resulB;
		resulG = pow((obj1.histo_g.at(i) - obj2.histo_g.at(i)), 2) + resulG;
		resulR = pow((obj1.histo_r.at(i) - obj2.histo_r.at(i)), 2) + resulR;
	}
	dis_euclidea = sqrt(resulB) + sqrt(resulG) + sqrt(resulR);
	return dis_euclidea;
}

int main(int argc, char* argv[]) {

	vector<string> clase1 = getArchivos(carpetas[0]);
	vector<string> clase2 = getArchivos(carpetas[1]);
	vector<string> test = getArchivos(carpetas[2]);
	
	vector<Imagen> imgsClase1 = cargarImagenes(carpetas[0], clase1, "Clase1.txt");
	vector<Imagen> imgsClase2 = cargarImagenes(carpetas[1], clase2, "Clase2.txt");
	vector<Imagen> imgsTest = cargarImagenes(carpetas[2], test, "Test.txt");

	cout << "**********************************************" << endl;
	cout << "\t\t\Clase 1" << endl;
	cout << "**********************************************" << endl;
	for (int i = 0; i < clase1.size()-1 ; i++) {
		for (int j = i + 1; j < clase1.size(); j++) {
			double distancia = calcularDistanciaEuclidia(imgsClase1[i], imgsClase1[j]);
			cout << clase1[i] << " vs " << clase1[j] << "\t\tDistancia: " << distancia << endl;
			cout << endl;
		}
	}
	
	cout << "**********************************************" << endl;
	cout << "\t\t\tClase 2" << endl;
	cout << "**********************************************" << endl;
	for (int i = 0; i < clase2.size() - 1; i++) {
		for (int j = i + 1; j < clase2.size(); j++) {
			double distancia = calcularDistanciaEuclidia(imgsClase2[i], imgsClase2[j]);
			cout << clase2[i] << " vs " << clase2[j] << "\t\t Distancia: " << distancia << endl;
			cout << endl;
		}
	}
	
	cout << "**********************************************" << endl;
	cout << "\t NuevaClasificación 2" << endl;
	cout << "**********************************************" << endl;

	double resul[2][2] = {(0,0),(0,0)};
	
	for (int i = 0; i < imgsTest.size(); i++)
	{
		for (int j = 0; j < clase1.size(); j++) {
			double distancia = calcularDistanciaEuclidia(imgsTest[i], imgsClase1[j]);
			cout << "----------------------------------------------------------" << endl;
			cout << test[i] << " vs " << clase1[j] << "\t\t Distancia: " << distancia << endl;
			if (j == 0) {
				resul[i][0] = distancia;
			}
			else {
				if (distancia < resul[i][0])
					resul[i][0] = distancia;
					resul[i][1] = (double)(i+1.0);
			}
			cout << endl;
		}
		for (int j = 0; j < clase2.size(); j++) {
			double distancia = calcularDistanciaEuclidia(imgsTest[i], imgsClase2[j]);
			cout << "----------------------------------------------------------" << endl;
			cout << test[i] << " vs " << clase2[j] << "\t\t Distancia: " << distancia << endl;
			
			if (distancia < resul[i][0])
				resul[i][0] = distancia;
				resul[i][1] = (double)(i + 1.0);
			
			cout << endl;
		}
	}
	cout << "----------------------------------------------------------" << endl;
	cout << "********************************" << endl;
	cout << "\t Resultados" << endl;
	cout << "********************************" << endl;

	for (int i = 0; i < imgsTest.size(); i++)
	{
		cout << test[i] << " pertece a: Clase " << resul[i][1] << "\t\t Distancia menor" << resul[i][0] << endl;
	}
	


	return 0;
}