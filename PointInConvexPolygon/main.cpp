#include <iostream>
#include <time.h>
#include "Point2D.h"
#include "Polygon.h"


#include <vector>
#include "Geometry.h"
#include "OLogNSlabTest.h"

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
	

	class Polygon *poly = new class Polygon(10000);
	poly->saveToFile("out.poly");
	OLogNSlabTest test(*poly);
	test.preprocess();
	//test the points here (not implemented yet)
	test.deinit();

	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
	Gdiplus::Bitmap bmp(1000, 1000);
	Gdiplus::Graphics gfx(&bmp);
	Gdiplus::Pen pen(Gdiplus::Color(255, 255, 0, 0), 1);
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
	//system("DrawPoly.exe");
	return 0;
}