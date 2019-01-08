// project2.cpp : Defines the entry point for the console application.
//
#include <fstream>
#include <iterator>
#include <string>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;



struct pixel
{
	unsigned char red, green, blue;
};
struct targa_header {
	char idLength;                 // ID Field (Number of bytes - max 255)
	char colorMapType;               // Colormap Field (0 or 1)
	char datTypeCode;               // Image Type (7 options - color vs. compression)
	short int colorMapOrigin;              // Color Map stuff - first entry index
	short int colorMapLength;                // Color Map stuff - total entries in file
	char colorMapDepth;         // Color Map stuff - number of bits per entry
	short int xOrigin;                      // X-coordinate of origin
	short int yOrigin;                      // Y-coordinate of origin
	short width;                  // Width in Pixels
	short height;                 // Height in Pixels
	char bitsPerPixel;                    // Number of bits per pixel
	char imageDescriptor;                   // Other stuff - scan origin and alpha bits
};



class tgaImageFile {
public:
	targa_header tFileHeader;
	vector<pixel> tFilePixel;
};



void load_tgaFile(const string& fileName, tgaImageFile& tFile)
{
	std::ifstream tgaFile(fileName.c_str(), std::ios_base::binary);
	//if (!tgaFile) return false;
	// Load image header
	tgaFile.read((char*)&tFile.tFileHeader.idLength, sizeof(tFile.tFileHeader.idLength));
	//cout << "ID Length: " << tFile.tFileHeader.idLength << endl;
	tgaFile.read((char*)&tFile.tFileHeader.colorMapType, sizeof(tFile.tFileHeader.colorMapType));
	tgaFile.read((char*)&tFile.tFileHeader.datTypeCode, sizeof(tFile.tFileHeader.datTypeCode));
	tgaFile.read((char*)&tFile.tFileHeader.colorMapOrigin, sizeof(tFile.tFileHeader.colorMapOrigin));
	tgaFile.read((char*)&tFile.tFileHeader.colorMapLength, sizeof(tFile.tFileHeader.colorMapLength));
	tgaFile.read((char*)&tFile.tFileHeader.colorMapDepth, sizeof(tFile.tFileHeader.colorMapDepth));
	tgaFile.read((char*)&tFile.tFileHeader.xOrigin, sizeof(tFile.tFileHeader.xOrigin));
	tgaFile.read((char*)&tFile.tFileHeader.yOrigin, sizeof(tFile.tFileHeader.yOrigin));
	tgaFile.read((char*)&tFile.tFileHeader.width, sizeof(tFile.tFileHeader.width));
	//cout << "Width: " << tFile.tFileHeader.width << endl;
	tgaFile.read((char*)&tFile.tFileHeader.height, sizeof(tFile.tFileHeader.height));
	//cout << "Height: " << tFile.tFileHeader.height << endl;
	tgaFile.read((char*)&tFile.tFileHeader.bitsPerPixel, sizeof(tFile.tFileHeader.bitsPerPixel));
	tgaFile.read((char*)&tFile.tFileHeader.imageDescriptor, sizeof(tFile.tFileHeader.imageDescriptor));
	//system("Pause");
	tFile.tFilePixel.resize(tFile.tFileHeader.width * tFile.tFileHeader.height + 1);  // add one to compensate for 0 index
																					  // Load image data
	for (int i = 1; i <= (tFile.tFileHeader.width * tFile.tFileHeader.height); i++)
	{
		tgaFile.read((char*)&tFile.tFilePixel[i].blue, sizeof(tFile.tFilePixel[i].blue));  // TGA files have BGR instead of RGB
		tgaFile.read((char*)&tFile.tFilePixel[i].green, sizeof(tFile.tFilePixel[i].green));
		tgaFile.read((char*)&tFile.tFilePixel[i].red, sizeof(tFile.tFilePixel[i].red));
		//cout << static_cast<unsigned>(tFile.tFilePixel[i].blue) << ", " << static_cast<unsigned>(tFile.tFilePixel[i].green) << ", " << static_cast<unsigned>(tFile.tFilePixel[i].red) << endl;
		//if (i == 101) system("Pause");  // testing
		//if (i == 20001) system("Pause");
		//if (i == 20101) system("Pause");
	}
	tgaFile.close();
	//return true;
}
void write_tgaFile(const string& fileName, tgaImageFile& tFile)
{
	std::ofstream tgaFile(fileName.c_str(), std::ios_base::binary);
	//if (!tgaFile) return false;
	// Load image header
	tgaFile.write((char*)&tFile.tFileHeader.idLength, sizeof(tFile.tFileHeader.idLength));
	tgaFile.write((char*)&tFile.tFileHeader.colorMapType, sizeof(tFile.tFileHeader.colorMapType));
	tgaFile.write((char*)&tFile.tFileHeader.datTypeCode, sizeof(tFile.tFileHeader.datTypeCode));
	tgaFile.write((char*)&tFile.tFileHeader.colorMapOrigin, sizeof(tFile.tFileHeader.colorMapOrigin));
	tgaFile.write((char*)&tFile.tFileHeader.colorMapLength, sizeof(tFile.tFileHeader.colorMapLength));
	tgaFile.write((char*)&tFile.tFileHeader.colorMapDepth, sizeof(tFile.tFileHeader.colorMapDepth));
	tgaFile.write((char*)&tFile.tFileHeader.xOrigin, sizeof(tFile.tFileHeader.xOrigin));
	tgaFile.write((char*)&tFile.tFileHeader.yOrigin, sizeof(tFile.tFileHeader.yOrigin));
	tgaFile.write((char*)&tFile.tFileHeader.width, sizeof(tFile.tFileHeader.width));
	tgaFile.write((char*)&tFile.tFileHeader.height, sizeof(tFile.tFileHeader.height));
	tgaFile.write((char*)&tFile.tFileHeader.bitsPerPixel, sizeof(tFile.tFileHeader.bitsPerPixel));
	tgaFile.write((char*)&tFile.tFileHeader.imageDescriptor, sizeof(tFile.tFileHeader.imageDescriptor));

	tFile.tFilePixel.resize(tFile.tFileHeader.width * tFile.tFileHeader.height + 1);
	// Load image data
	for (int i = 1; i <= (tFile.tFileHeader.width * tFile.tFileHeader.height); i++)
	{
		tgaFile.write((char*)&tFile.tFilePixel[i].blue, sizeof(tFile.tFilePixel[i].blue));  // TGA files have BGR instead of RGB
		tgaFile.write((char*)&tFile.tFilePixel[i].green, sizeof(tFile.tFilePixel[i].green));
		tgaFile.write((char*)&tFile.tFilePixel[i].red, sizeof(tFile.tFilePixel[i].red));
	}
	tgaFile.close();
	//return true;
}

void multiplyImage(tgaImageFile &tFile, tgaImageFile &tFile2, tgaImageFile &newtFile)
{
	float tempb;
	float tempg;
	float tempr;
	newtFile.tFileHeader = tFile.tFileHeader;
	unsigned int noOfPixels = newtFile.tFileHeader.width * newtFile.tFileHeader.height;
	newtFile.tFilePixel.resize(noOfPixels + 1);

	for (unsigned int i = 1; i <= noOfPixels; i++)
	{
		tempb = ((float) tFile.tFilePixel[i].blue / 255) * ((float) tFile2.tFilePixel[i].blue / 255);
		if (tempb > 1)
		{
			newtFile.tFilePixel[i].blue = 255;
		}
		else
			newtFile.tFilePixel[i].blue = (tempb * 255) + .5;

		tempg = ((float) tFile.tFilePixel[i].green / 255) * ((float) tFile2.tFilePixel[i].green / 255);
		if (tempg > 1)
		{
			newtFile.tFilePixel[i].green = 255;
		}
		else
			newtFile.tFilePixel[i].green = (tempg * 255) + .5;

		tempr = ((float) tFile.tFilePixel[i].red / 255) * ((float) tFile2.tFilePixel[i].red / 255);
		if (tempr > 1)
		{
			newtFile.tFilePixel[i].red = 255;
		}
		else
			newtFile.tFilePixel[i].red = (tempr * 255) + .5;
	}

}

void subtractFile(tgaImageFile &tFile, tgaImageFile &tFile2, tgaImageFile &newtFile)
{
	int tempb;
	int tempg;
    int tempr;
	newtFile.tFileHeader = tFile.tFileHeader;
	unsigned int noOfPixels = newtFile.tFileHeader.width * newtFile.tFileHeader.height;
	newtFile.tFilePixel.resize(noOfPixels + 1);
	for (unsigned int i = 1; i <= noOfPixels; i++)
	{
		tempb = tFile.tFilePixel[i].blue - tFile2.tFilePixel[i].blue;
		if (tempb <= 0)
			newtFile.tFilePixel[i].blue = 0;
		else
			newtFile.tFilePixel[i].blue = tFile.tFilePixel[i].blue - tFile2.tFilePixel[i].blue;

		tempg = tFile.tFilePixel[i].green - tFile2.tFilePixel[i].green;
		if (tempg <= 0)
			newtFile.tFilePixel[i].green = 0;
		else
			newtFile.tFilePixel[i].green = tFile.tFilePixel[i].green - tFile2.tFilePixel[i].green;

		tempr = tFile.tFilePixel[i].red - tFile2.tFilePixel[i].red;
		if (tempr <= 0)
			newtFile.tFilePixel[i].red = 0;
		else
			newtFile.tFilePixel[i].red = tFile.tFilePixel[i].red - tFile2.tFilePixel[i].red;
	}
}

void screenImage(tgaImageFile &tFile, tgaImageFile &tFile2, tgaImageFile &newtFile)
{
	float tempb;
	float tempg;
	float tempr;
	newtFile.tFileHeader = tFile.tFileHeader;
	unsigned int noOfPixels = newtFile.tFileHeader.width * newtFile.tFileHeader.height;
	newtFile.tFilePixel.resize(noOfPixels + 1);

	for (unsigned int i = 1; i <= noOfPixels; i++)
	{
		tempb =( 1 - ((1 - ((float)tFile.tFilePixel[i].blue / 255))) * ((1-((float)tFile2.tFilePixel[i].blue / 255))));
		if (tempb > 1)
		{
			newtFile.tFilePixel[i].blue = 255;
		}
		else
			newtFile.tFilePixel[i].blue = (tempb * 255) + .5;

		tempg = (1 - ((1 - ((float)tFile.tFilePixel[i].green / 255))) * ((1 - ((float)tFile2.tFilePixel[i].green / 255))));
		if (tempg > 1)
		{
			newtFile.tFilePixel[i].green = 255;
		}
		else
			newtFile.tFilePixel[i].green = (tempg * 255) + .5;

		tempr = (1 - ((1 - ((float)tFile.tFilePixel[i].red / 255))) * ((1 - ((float)tFile2.tFilePixel[i].red / 255))));
		if (tempr > 1)
		{
			newtFile.tFilePixel[i].red = 255;
		}
		else
			newtFile.tFilePixel[i].red = (tempr * 255) + .5;
	}
}


void overlayImage(tgaImageFile &tFile, tgaImageFile &tFile2, tgaImageFile &newtFile)
{
	float tempb;
	float tempb2;
	float tempbfinal;
	float tempg;
	float tempg2;
	float tempgfinal;
	float tempr;
	float tempr2;
	float temprfinal;
	newtFile.tFileHeader = tFile.tFileHeader;
	unsigned int noOfPixels = newtFile.tFileHeader.width * newtFile.tFileHeader.height;
	newtFile.tFilePixel.resize(noOfPixels + 1);

	for (unsigned int i = 1; i <= noOfPixels; i++)
	{
		tempb = (float)tFile.tFilePixel[i].blue / 255;
		tempb2 = (float)tFile2.tFilePixel[i].blue / 255;
	 if (tempb2 > .5)
		{
		 tempbfinal = (1 - (2 * (1 - tempb) * (1 - tempb2)));
		 if (tempbfinal < 0)
			 newtFile.tFilePixel[i].blue = 0;
		 else
			 newtFile.tFilePixel[i].blue = ((255 * tempbfinal) + .5);
		}
	 else if (tempb2 <= .5)
	 {
		 tempbfinal = (2 * tempb*tempb2);
		 if (tempbfinal > 1)
			 newtFile.tFilePixel[i].blue = 255;
		 else
			 newtFile.tFilePixel[i].blue = ((255 * tempbfinal) + .5);
	 }
	 tempg = (float)tFile.tFilePixel[i].green / 255;
	 tempg2 = (float)tFile2.tFilePixel[i].green / 255;
	 if (tempg2 > .5)
	 {
		 tempgfinal = (1 - (2 * (1 - tempg) * (1 - tempg2)));
		 if (tempgfinal < 0)
			 newtFile.tFilePixel[i].green = 0;
		 else
			 newtFile.tFilePixel[i].green = ((255 * tempgfinal) + .5);
	 }
	 else if (tempg2 <= .5)
	 {
		 tempgfinal = (2 * tempg*tempg2);
		 if (tempgfinal > 1)
			 newtFile.tFilePixel[i].green = 255;
		 else
			 newtFile.tFilePixel[i].green = ((255 * tempgfinal) + .5);
	 }

	 tempr = (float)tFile.tFilePixel[i].red / 255;
	 tempr2 = (float)tFile2.tFilePixel[i].red / 255;
	 if (tempr2 > .5)
	 {
		 temprfinal = (1 - (2 * (1 - tempr) * (1 - tempr2)));
		 if (temprfinal < 0)
			 newtFile.tFilePixel[i].red = 0;
		 else
			 newtFile.tFilePixel[i].red = ((255 * temprfinal) + .5);
	 }
	 else if (tempr2 <= .5)
	 {
		 temprfinal = (2 * tempr*tempr2);
		 if (tempbfinal > 1)
			 newtFile.tFilePixel[i].red = 255;
		 else
			 newtFile.tFilePixel[i].red = ((255 * temprfinal) + .5);
	 }
	}
}

void addGreen(tgaImageFile &tFile)
{
	unsigned int noOfPixels = tFile.tFileHeader.width * tFile.tFileHeader.height;
	unsigned int temp;
	for (unsigned int i = 1; i <= noOfPixels; i++)
	{
		temp = tFile.tFilePixel[i].green + 200;
		if(temp > 255)
		{
			tFile.tFilePixel[i].green = 255;
		}
		else
		tFile.tFilePixel[i].green = tFile.tFilePixel[i].green + 200;
	}


}

void multiplyRed(tgaImageFile &tFile)
{
	unsigned int noOfPixels = tFile.tFileHeader.width * tFile.tFileHeader.height;
	unsigned int temp;
	for (unsigned int i = 1; i <= noOfPixels; i++)
	{
		temp = tFile.tFilePixel[i].red * 4;
		if (temp > 255)
		{
			tFile.tFilePixel[i].red = 255;

		}
		else
			tFile.tFilePixel[i].red = tFile.tFilePixel[i].red * 4;

		tFile.tFilePixel[i].blue = tFile.tFilePixel[i].blue * 0;
	}
}
void Getlayer(tgaImageFile &tFile, char layerColor)

{
	unsigned int noOfPixels = tFile.tFileHeader.width * tFile.tFileHeader.height;
	tFile.tFilePixel.resize(noOfPixels + 1);
	for (unsigned int i = 1; i <= noOfPixels; i++)
	{
		if (layerColor == 'b')
		{
			tFile.tFilePixel[i].green = tFile.tFilePixel[i].blue;
			tFile.tFilePixel[i].red = tFile.tFilePixel[i].blue;
		}
		else if (layerColor == 'g')
		{
			tFile.tFilePixel[i].red = tFile.tFilePixel[i].green;
			tFile.tFilePixel[i].blue = tFile.tFilePixel[i].green;
		}
		else
		{
			tFile.tFilePixel[i].blue = tFile.tFilePixel[i].red;
			tFile.tFilePixel[i].green = tFile.tFilePixel[i].red;
		}
	}

}

void rotateImage(tgaImageFile &tFile)
{
	pixel tmpPixel;
	unsigned int noOfPixels = tFile.tFileHeader.width * tFile.tFileHeader.height;
	unsigned int currentPixel = 0;
	for (unsigned int i = 1; i <= (noOfPixels / 2); i++)
	{
		tmpPixel = tFile.tFilePixel[noOfPixels - currentPixel];
		tFile.tFilePixel[noOfPixels - currentPixel] = tFile.tFilePixel[i];
		tFile.tFilePixel[i] = tmpPixel;
		currentPixel++;
	}
}
void layerFiles(tgaImageFile &tFile, tgaImageFile &tFile2, tgaImageFile &tFile3, tgaImageFile &newtFile)
{

	newtFile.tFileHeader = tFile.tFileHeader;
	unsigned int noOfPixels = newtFile.tFileHeader.width * newtFile.tFileHeader.height;
	newtFile.tFilePixel.resize(noOfPixels + 1);
	for (unsigned int i = 1; i <= noOfPixels; i++)
	{
		newtFile.tFilePixel[i].blue = tFile.tFilePixel[i].blue;
		newtFile.tFilePixel[i].green = tFile2.tFilePixel[i].green;
		newtFile.tFilePixel[i].red = tFile3.tFilePixel[i].red;

	}
}
//Compare Files for Testing Purposes


int compareImages(tgaImageFile& tFileA, tgaImageFile& tFileB, string testCase) {
//compare headers
	bool headersMatch = false;
	int points = 0;
	bool pixelsMatch = true;
	if (tFileA.tFileHeader.height == tFileB.tFileHeader.height && tFileA.tFileHeader.width == tFileB.tFileHeader.width)
	{
		headersMatch = true;
	}
//compare Pixels
	unsigned int noOfPixels;
	noOfPixels = tFileA.tFileHeader.height * tFileA.tFileHeader.width;
	for (unsigned int i = 1; i <= noOfPixels; i++)
	{
		if (tFileA.tFilePixel[i].blue != tFileB.tFilePixel[i].blue)
			pixelsMatch = false;
		if (tFileA.tFilePixel[i].green != tFileB.tFilePixel[i].green)
			pixelsMatch = false;
		if (tFileA.tFilePixel[i].red != tFileB.tFilePixel[i].red)
			pixelsMatch = false;
	}	
	if(pixelsMatch == true && headersMatch == true)
	{
		points = 11;
		cout << testCase << " test passed " << endl;
	}
	else
	{
		cout << testCase << " test failed" << endl;
	}
	return points;
}


void extraCredit(tgaImageFile& FText, tgaImageFile& FPattern, tgaImageFile& FCar, tgaImageFile& FCircles, tgaImageFile& FResult)
{
	FResult.tFileHeader = FText.tFileHeader;
	FResult.tFileHeader.height = FText.tFileHeader.height * 2;
	FResult.tFileHeader.width = FText.tFileHeader.width * 2;
	unsigned int noOfPixels;
	noOfPixels = FResult.tFileHeader.height * FResult.tFileHeader.width;
	FResult.tFilePixel.resize(noOfPixels + 1);
	unsigned int pixNum = 1;
	unsigned int pixNum2 = 1;

	for (unsigned int i = 1; i <= noOfPixels/2; i++)
	{
		for (int j = 1; j <= FText.tFileHeader.width; j++)
		{
			FResult.tFilePixel[i].blue = FText.tFilePixel[pixNum].blue;
			FResult.tFilePixel[i].green = FText.tFilePixel[pixNum].green;
			FResult.tFilePixel[i].red = FText.tFilePixel[pixNum].red;
			i++;
			pixNum++;
		}
		for (int k = 1; k <= FText.tFileHeader.width; k++)
		{
			FResult.tFilePixel[i].blue = FPattern.tFilePixel[pixNum2].blue;
			FResult.tFilePixel[i].green = FPattern.tFilePixel[pixNum2].green;
			FResult.tFilePixel[i].red = FPattern.tFilePixel[pixNum2].red;
			pixNum2++;
			i++;
		}
		i--;
	}

	pixNum = 1;
	pixNum2 = 1;
	for (unsigned int i = (noOfPixels/2 + 1); i <= noOfPixels; i++)
	{
		for (int j = 1; j <= FText.tFileHeader.width; j++)
		{
			FResult.tFilePixel[i].blue = FCar.tFilePixel[pixNum].blue;
			FResult.tFilePixel[i].green = FCar.tFilePixel[pixNum].green;
			FResult.tFilePixel[i].red = FCar.tFilePixel[pixNum].red;
			i++;
			pixNum++;
		}
		for (int k = 1; k <= FText.tFileHeader.width; k++)
		{
			FResult.tFilePixel[i].blue = FCircles.tFilePixel[pixNum2].blue;
			FResult.tFilePixel[i].green = FCircles.tFilePixel[pixNum2].green;
			FResult.tFilePixel[i].red = FCircles.tFilePixel[pixNum2].red;
			pixNum2++;
			i++;
		}
		i--;
	}




}


int main()
{

	int totalScore = 0; 
	
	//Task 1

	cout << "Running task 1...." << endl;

	//read file
	tgaImageFile imageFileLayer;
	tgaImageFile imageFilePattern;
	tgaImageFile newMultipliedimage;
	tgaImageFile imageFile1b;
	load_tgaFile("./input/layer1.tga", imageFileLayer);
	load_tgaFile("./input/pattern1.tga", imageFilePattern);
	

	//modify file
	multiplyImage(imageFileLayer, imageFilePattern, newMultipliedimage);

	//wite image
	write_tgaFile("./output/task1.tga", newMultipliedimage);
	//comparison test
	load_tgaFile("./output/task1.tga", newMultipliedimage);
	load_tgaFile("./examples/EXAMPLE_part1.tga", imageFile1b);
		
	totalScore = totalScore + compareImages(newMultipliedimage, imageFile1b, "Task 1");

	//task 2

	cout << "Running task 2...." << endl;

	//read file
	tgaImageFile imageFileLayer2;
	tgaImageFile imageFileCar;
	tgaImageFile imageSubtracted;
	tgaImageFile imageFile2b;
	load_tgaFile("./input/layer2.tga", imageFileLayer2);
	load_tgaFile("./input/car.tga", imageFileCar);

	//modify file
	subtractFile(imageFileCar, imageFileLayer2 ,imageSubtracted);

	//write file
	write_tgaFile("./output/task2.tga", imageSubtracted);

	//comparison test
	load_tgaFile("./output/task2.tga", imageSubtracted);
	load_tgaFile("./examples/EXAMPLE_part2.tga", imageFile2b);

	totalScore = totalScore + compareImages(imageSubtracted, imageFile2b, "Task 2");
	
	//task 3
	cout << "Running task 3...." << endl;

	//read file
	tgaImageFile imageFilePattern2;
	tgaImageFile tempMultipliedimage;
	tgaImageFile textImage;
	tgaImageFile newScreenedImage;
	tgaImageFile imageFile3b;
	load_tgaFile("./input/pattern2.tga", imageFilePattern2);
	load_tgaFile("./input/text.tga", textImage);

	//modify file
	multiplyImage(imageFileLayer, imageFilePattern2, tempMultipliedimage);
	screenImage(tempMultipliedimage, textImage, newScreenedImage);

	//write
	write_tgaFile("./output/task3.tga", newScreenedImage);
	
	//compare test 
	load_tgaFile("./output/task3.tga", newScreenedImage);
	load_tgaFile("./examples/EXAMPLE_part3.tga", imageFile3b);

	totalScore = totalScore + compareImages(newScreenedImage, imageFile3b, "Task 3");

	
	//task 4
	cout << "Running task 4...." << endl;
	//read files
	tgaImageFile imageFileCirlces;
	tgaImageFile newMultipliedimage2;
	tgaImageFile newSubractedImage2;
	tgaImageFile imageFile4b;
	load_tgaFile("./input/circles.tga", imageFileCirlces);

	//modify
	multiplyImage(imageFileLayer2, imageFileCirlces, newMultipliedimage2);
	subtractFile(newMultipliedimage2, imageFilePattern2, newSubractedImage2);

	write_tgaFile("./output/task4.tga", newSubractedImage2);
	//comapre test
	load_tgaFile("./output/task4.tga", newSubractedImage2);
	load_tgaFile("./examples/EXAMPLE_part4.tga", imageFile4b);

	totalScore = totalScore + compareImages(newSubractedImage2, imageFile4b, "Task 4");
	
	//task 5
	cout << "Running task 5...." << endl;
	//read files
	tgaImageFile overlayedImage;
	tgaImageFile imageFile5b;

	//modify
	overlayImage(imageFileLayer, imageFilePattern, overlayedImage);

	//write
	write_tgaFile("./output/task5.tga", overlayedImage);

	//compare test
	load_tgaFile("./output/task5.tga", overlayedImage);
	load_tgaFile("./examples/EXAMPLE_part5.tga", imageFile5b);

	totalScore = totalScore + compareImages(overlayedImage, imageFile5b, "Task 5");

	
	//task 6
	cout << "Running task 6...." << endl;

	//read file
	tgaImageFile imageFileAddGreen;
	tgaImageFile imageFile6b;
	load_tgaFile("./input/car.tga", imageFileAddGreen);

	//modify file
	addGreen(imageFileAddGreen);

	//write file
	write_tgaFile("./output/task6.tga", imageFileAddGreen);

	//comapre test
	load_tgaFile("./output/task6.tga", imageFileAddGreen);
	load_tgaFile("./examples/EXAMPLE_part6.tga", imageFile6b);

	totalScore = totalScore + compareImages(imageFileAddGreen, imageFile6b, "Task 6");
	

	//task 7
	cout << "Running task 7...." << endl;

	//read file
	tgaImageFile imageFile7b;
	tgaImageFile imageFileMultRed;
	load_tgaFile("./input/car.tga", imageFileMultRed);

	//modify file
	multiplyRed(imageFileMultRed);

	//write file
	write_tgaFile("./output/task7.tga", imageFileMultRed);

	//comapre test
	load_tgaFile("./output/task7.tga", imageFileMultRed);
	load_tgaFile("./examples/EXAMPLE_part7.tga", imageFile7b);

	totalScore = totalScore + compareImages(imageFileMultRed, imageFile7b, "Task 7");
	
	
	//task 8
	cout << "Running task 8...." << endl;

	//read file 
	tgaImageFile imageFileCarB;
	tgaImageFile imageFileCarG;
	tgaImageFile imageFileCarR;
	tgaImageFile imageFile8r;
	tgaImageFile imageFile8g;
	tgaImageFile imageFile8b;

	load_tgaFile("./input/car.tga", imageFileCarB);
	imageFileCarG = imageFileCarB;
	imageFileCarR = imageFileCarB;

	//modify file
	Getlayer(imageFileCarB, 'b');
	write_tgaFile("./output/part8_b.tga", imageFileCarB);

	Getlayer(imageFileCarG, 'g');
	write_tgaFile("./output/part8_g.tga", imageFileCarG);

	Getlayer(imageFileCarR, 'r');
	write_tgaFile("./output/part8_r.tga", imageFileCarR);

	//compare Files
	load_tgaFile("./examples/EXAMPLE_part8_r.tga", imageFile8r);
	load_tgaFile("./examples/EXAMPLE_part8_g.tga", imageFile8g);
	load_tgaFile("./examples/EXAMPLE_part8_b.tga", imageFile8b);

	
	compareImages(imageFileCarR, imageFile8r, "Task 8 red");
	compareImages(imageFileCarG, imageFile8g, "Task 8 green");
	totalScore = totalScore + compareImages(imageFileCarB, imageFile8b, "Task 8 blue");



	
	//task 9
	cout << "Running task 9...." << endl;

	//Read File
	tgaImageFile imageFileA;
	load_tgaFile("./input/layer_blue.tga", imageFileA);
	tgaImageFile imageFileB;
	load_tgaFile("./input/layer_green.tga", imageFileB);
	tgaImageFile imageFileC;
	load_tgaFile("./input/layer_red.tga", imageFileC);
	tgaImageFile newImageFile;
	tgaImageFile imageFile9b;

	//Modify File
	layerFiles(imageFileA, imageFileB, imageFileC, newImageFile);
	//Write File
	write_tgaFile("./output/task9.tga", newImageFile);

	//compare files
	load_tgaFile("./output/task9.tga", newImageFile);
	load_tgaFile("./examples/EXAMPLE_part9.tga", imageFile9b);

	totalScore = totalScore + compareImages(newImageFile, imageFile9b, "task 9");


	//task 10
	cout << "Running task 10...." << endl;
	//read files
	tgaImageFile imageFileRotate;
	tgaImageFile imageFile10b;
	load_tgaFile("./input/text2.tga", imageFileRotate);

	//Modify
	rotateImage(imageFileRotate);

	//write file
	write_tgaFile("./output/task10.tga", imageFileRotate);

	//compare test
	load_tgaFile("./output/task10.tga", imageFileRotate);
	load_tgaFile("./examples/EXAMPLE_part10.tga", imageFile10b);

	totalScore = totalScore + compareImages(imageFileRotate, imageFile10b, "task 10");
	cout << "You scored " << totalScore << " Points" << endl;
	//Task extra credit
	cout << "Testing Extra Credit" << endl;
	//read file 
	tgaImageFile FileResult;
	tgaImageFile imageFileEC;


	//modify file
	extraCredit(textImage, imageFilePattern, imageFileCar, imageFileCirlces, FileResult);
	write_tgaFile("./output/extracredit.tga", FileResult);
	
	//compare test
	load_tgaFile("./output/extracredit.tga", FileResult);
	load_tgaFile("./examples/EXAMPLE_extraCredit.tga", imageFileEC);

	totalScore = totalScore + compareImages(imageFileRotate, imageFile10b, "Extra Credit Task");
	system("Pause");
	return 0;

}