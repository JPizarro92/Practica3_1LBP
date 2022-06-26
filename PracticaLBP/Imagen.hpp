#include <iostream>
#include <cstdlib>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp> 

#include "LBPDescriptor.hpp"

using namespace cv;
using namespace std;

class Imagen
{
public:
	Mat original;
	Mat imgCIELab;
	Mat canal_gray_scale;
	Mat canal_b;
	Mat canal_g;
	Mat canal_r;
	vector<int> histo_b;
	vector<int> histo_g;
	vector<int> histo_r;
	void generarCanales();
	void guardarHistograma(string);
	void calculaHistogramas();
private:

};

void Imagen::generarCanales() {
	vector<Mat> canales;
	cvtColor(original, imgCIELab, COLOR_BGR2Lab);
	split(imgCIELab, canales);
	canal_b = canales[0];
	canal_g = canales[1];
	canal_r = canales[2];
}

void Imagen::guardarHistograma(string documento){
	
	ofstream file;
	file.open(documento, std::fstream::in | std::fstream::out | std::fstream::app);

	string histoB = "Canal B: ";
	for (int i = 0; i < histo_b.size(); i++)
	{
		if(i<255)
			histoB = histoB + to_string(histo_b[i]) + ";";
		else
			histoB = histoB + to_string(histo_b[i]);
	}
	string histoG = "Canal G: ";
	for (int i = 0; i < histo_g.size(); i++)
	{
		if (i < 255)
			histoG = histoG + to_string(histo_g[i]) + ";";
		else
			histoG = histoG + to_string(histo_g[i]);
	}
	string histoR = "Canal R: ";
	for (int i = 0; i < histo_r.size(); i++)
	{
		if (i < 255)
			histoR = histoR + to_string(histo_r[i]) + ";";
		else
			histoR = histoR + to_string(histo_r[i]) + "\n";
	}

	file << histoB;
	file << histoG;
	file << histoR;
	file.close();
}

void Imagen::calculaHistogramas() {
	LBPDescriptor* lbp = new LBPDescriptor();
	histo_b = lbp->histogramaLBP(canal_b);
	histo_g = lbp->histogramaLBP(canal_g);
	histo_r = lbp->histogramaLBP(canal_r);
	delete lbp;
}