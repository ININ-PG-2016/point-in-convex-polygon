#include <iostream>
#include <time.h>
#include <vector>

#include "Point2D.h"
#include "Polygon.h"

#include "Geometry.h"
#include "HalfPlaneIntersectionTest.h"
#include "OLogNSlabTest.h"
#include "OLogNPolarSubdivisionTest.h"
#include "O1PolarSubdivisionTest.h"

#include <Windows.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

/*
copy-pasted from msdn documentation
*/
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	using namespace Gdiplus;
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}

int main(int argc, char **argv)
{
	/*Point2D p1(1, 2);
	Point2D p2(2, 1);
	Point2D p3 = p1 + p2;
	p3 *= 2;
	p3 += Point2D(2, 4);
	p3 = p3.normal(Direction::LEFT);*/
	//std::cout << p3.x << " " << p3.y <<std::endl;
	//std::cout << (p3 / 2).dot(Point2D(2, 3)) << std::endl;
	
	srand((unsigned int)time(nullptr));
	const int pointCount = 100000;
	Point2D* points = new Point2D[pointCount];
	bool* inclusion = new bool[pointCount];
	for (int i = 0; i < pointCount; i++)
	{
		points[i].x = ((double)rand() / (RAND_MAX)) * 2 - 1;
		points[i].y = ((double)rand() / (RAND_MAX)) * 2 - 1;
		inclusion[i] = false;
	}
	class Polygon *poly = new class Polygon(10000);
	poly->saveToFile("out.poly");
	O1PolarSubdivisionTest test(*poly);
	test.setWedgesPerOctant(poly->vertices.size() / 8);
	std::cout << "preprocessing" << std::endl;
	test.preprocess();
	std::cout << "testing points" << std::endl;
	long t = clock();
	for (int i = 0; i < pointCount; i++)
		inclusion[i] = test.testPoint(points[i]);
	t = clock() - t;
	std::cout << "done testing - " << t << " ms" << std::endl;
	test.deinit();

	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
	Gdiplus::Bitmap bmp(2500, 2500);
	Gdiplus::Graphics gfx(&bmp);
	for (unsigned int i = 0; i < bmp.GetWidth(); i++)
		for (unsigned int j = 0; j < bmp.GetHeight(); j++)
			bmp.SetPixel(i, j, Gdiplus::Color(255, 255, 255, 255));

	int pointRadius = 2;

	for (int i = 0; i < pointCount; i++)
	{
		Gdiplus::SolidBrush pointBrush(inclusion[i] ? Gdiplus::Color(255, 0, 255, 0) : Gdiplus::Color(255, 255, 0, 0));
		gfx.FillEllipse(&pointBrush, (int)(points[i].x * bmp.GetWidth() / 2 + bmp.GetWidth() / 2) - pointRadius,
			(int)(points[i].y * bmp.GetWidth() / 2 + bmp.GetWidth() / 2) - pointRadius, 2 * pointRadius, 2 * pointRadius);
	}

	Gdiplus::Pen pen(Gdiplus::Color(150, 0, 0, 0), 3);
	for (int i = 0; i < poly->vertices.size() - 1; i++)
		gfx.DrawLine(&pen, (int)(poly->vertices[i].x * bmp.GetWidth() / 2 + bmp.GetWidth() / 2),
		(int)(poly->vertices[i].y * bmp.GetWidth() / 2 + bmp.GetWidth() / 2),
		(int)(poly->vertices[i + 1].x * bmp.GetWidth() / 2 + bmp.GetWidth() / 2),
		(int)(poly->vertices[i + 1].y * bmp.GetWidth() / 2 + bmp.GetWidth() / 2));
	gfx.DrawLine(&pen, (int)(poly->vertices[poly->vertices.size() - 1].x * bmp.GetWidth() / 2 + bmp.GetWidth() / 2),
		(int)(poly->vertices[poly->vertices.size() - 1].y * bmp.GetWidth() / 2 + bmp.GetWidth() / 2),
		(int)(poly->vertices[0].x * bmp.GetWidth() / 2 + bmp.GetWidth() / 2),
		(int)(poly->vertices[0].y * bmp.GetWidth() / 2 + bmp.GetWidth() / 2));

	CLSID  encoderClsid;
	GetEncoderClsid(L"image/png", &encoderClsid);
	bmp.Save(L"polygon.png", &encoderClsid);

	delete poly;
	delete[] points;
	delete[] inclusion;
	//system("DrawPoly.exe");
	std::cin.get();
	return 0;
}