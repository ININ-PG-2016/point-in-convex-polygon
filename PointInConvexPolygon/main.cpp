#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include <chrono>
#include <string>

#include "Point2D.h"
#include "Polygon.h"

#include "Geometry.h"
#include "HalfPlaneIntersectionTest.h"
#include "OLogNSlabTest.h"
#include "OLogNPolarSubdivisionTest.h"
#include "O1PolarSubdivisionTest.h"
#include "O1SlabTest.h"
#include "CrossProductTest.h"

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

int main2(int argc, char **argv)
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
	const int pointCount = 10000000;
	Point2D* points = new Point2D[pointCount];
	bool* inclusion = new bool[pointCount];
	for (int i = 0; i < pointCount; i++)
	{
		points[i].x = ((double)rand() / (RAND_MAX)) * 2 - 1;
		points[i].y = ((double)rand() / (RAND_MAX)) * 2 - 1;
		inclusion[i] = false;
	}
	//class Polygon *poly = new class Polygon("1000000.poly");
	class Polygon *poly = new class Polygon(500000);
	poly->saveToFile("out.poly");

	if (poly->vertices.size() == 0)
	{
		exit(1);
	}

	O1SlabTest test(*poly);
	//test.setMaxSlabCount(poly->vertices.size());
	//test.setWedgesPerOctant(poly->vertices.size() / 8);
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
		if(!inclusion[i])
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
	/*std::cout << "Done. Press enter to continue." << std::endl;
	std::cin.get();*/
	return 0;
}

void draw(wchar_t* filename, class Polygon* poly, Point2D* points, bool* inclusion, int pointCount)
{
	/*Gdiplus::Bitmap bmp(2500, 2500);
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
	bmp.Save(filename, &encoderClsid);*/
}

long test(PointInConvexPolygonTest* test, Point2D* points, bool* inclusion, int pointCount)
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	test->preprocess();
	for (int i = 0; i < pointCount; i++)
	{
		inclusion[i] = test->testPoint(points[i]);
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	test->deinit();
	int inside = 0;
	int outside = 0;
	for (int i = 0; i < pointCount; i++)
	{
		if (inclusion[i])
			inside++;
		else
			outside++;
	}
	std::cout << "Inside: " << inside << ", Outside: " << outside << std::endl;
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}

int main(int argc, char **argv)
{
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	srand((unsigned int)time(nullptr));
	const int pointCount = 10000000;
	Point2D* points = new Point2D[pointCount];
	bool* inclusion = new bool[pointCount];
	for (int i = 0; i < pointCount; i++)
	{
		points[i].x = ((double)rand() / (RAND_MAX)) * 2 - 1;
		points[i].y = ((double)rand() / (RAND_MAX)) * 2 - 1;
	}

	std::vector<int> pointCounts = {10000, 100000, 1000000, 10000000, 100000000};
	std::vector<int> polygonSizes = {10, 100, 1000, 10000, 100000, 1000000};
	
	for (int i = 0; i < polygonSizes.size() - 1; i++)
	{
		std::string filename = std::string("") + std::to_string(polygonSizes[i]) + std::string(".poly");
		class Polygon* poly = new class Polygon((char*)filename.c_str());
		std::string edgesString = std::string("Edges = ") + std::to_string(polygonSizes[i]);
		std::string testName;

		testName = "HPI, " + edgesString;
		if (polygonSizes[i] < 100000)
		{
			std::ofstream file(testName + ".csv");
			for (int j = 0; j < pointCounts.size() - 2; j++)
			{
				std::cout << testName << std::endl;
				HalfPlaneIntersectionTest t(*poly);
				file << pointCounts[j] << ";" << test(&t, points, inclusion, pointCounts[j]) << ";" << std::endl;
				std::string picName = testName + ", N = " + std::to_string(pointCounts[j]) + ".png";
				draw((wchar_t*)std::wstring(picName.begin(), picName.end()).c_str(), poly, points, inclusion, pointCounts[j]);
			}
			file.close();
		}

		testName = "CP, " + edgesString;
		if (polygonSizes[i] < 100000)
		{
			std::ofstream file(testName + ".csv");
			for (int j = 0; j < pointCounts.size() - 2; j++)
			{
				std::cout << testName << std::endl;
				CrossProductTest t(*poly);
				file << pointCounts[j] << ";" << test(&t, points, inclusion, pointCounts[j]) << ";" << std::endl;
				std::string picName = testName + ", N = " + std::to_string(pointCounts[j]) + ".png";
				draw((wchar_t*)std::wstring(picName.begin(), picName.end()).c_str(), poly, points, inclusion, pointCounts[j]);
			}
			file.close();
		}

		{
			testName = "O(LogN) slabs, " + edgesString;
			std::ofstream file(testName + ".csv");
			for (int j = 0; j < pointCounts.size() - 1; j++)
			{
				std::cout << testName << std::endl;
				OLogNSlabTest t(*poly);
				file << pointCounts[j] << ";" << test(&t, points, inclusion, pointCounts[j]) << ";" << std::endl;
				std::string picName = testName + ", N = " + std::to_string(pointCounts[j]) + ".png";
				draw((wchar_t*)std::wstring(picName.begin(), picName.end()).c_str(), poly, points, inclusion, pointCounts[j]);
			}
			file.close();
		}

		{
			testName = "O(LogN) polar, " + edgesString;
			std::ofstream file(testName + ".csv");
			for (int j = 0; j < pointCounts.size() - 1; j++)
			{
				std::cout << testName << std::endl;
				OLogNPolarSubdivisionTest t(*poly);
				file << pointCounts[j] << ";" << test(&t, points, inclusion, pointCounts[j]) << ";" << std::endl;
				std::string picName = testName + ", N = " + std::to_string(pointCounts[j]) + ".png";
				draw((wchar_t*)std::wstring(picName.begin(), picName.end()).c_str(), poly, points, inclusion, pointCounts[j]);
			}
			file.close();
		}

		{
			testName = "O(1) polar, M = sqrt(N), " + edgesString;
			std::ofstream file(testName + ".csv");
			for (int j = 0; j < pointCounts.size() - 1; j++)
			{
				std::cout << testName << std::endl;
				O1PolarSubdivisionTest t(*poly);
				t.setWedgesPerOctant(sqrt(poly->vertices.size()) / 8);
				file << pointCounts[j] << ";" << test(&t, points, inclusion, pointCounts[j]) << ";" << std::endl;
				std::string picName = testName + ", N = " + std::to_string(pointCounts[j]) + ".png";
				draw((wchar_t*)std::wstring(picName.begin(), picName.end()).c_str(), poly, points, inclusion, pointCounts[j]);
			}
			file.close();
		}

		{
			testName = "O(1) polar, M = N x 0.25, " + edgesString;
			std::ofstream file(testName + ".csv");
			for (int j = 0; j < pointCounts.size() - 1; j++)
			{
				std::cout << testName << std::endl;
				O1PolarSubdivisionTest t(*poly);
				t.setWedgesPerOctant((poly->vertices.size() / 4) / 8);
				file << pointCounts[j] << ";" << test(&t, points, inclusion, pointCounts[j]) << ";" << std::endl;
				std::string picName = testName + ", N = " + std::to_string(pointCounts[j]) + ".png";
				draw((wchar_t*)std::wstring(picName.begin(), picName.end()).c_str(), poly, points, inclusion, pointCounts[j]);
			}
			file.close();
		}

		{
			testName = "O(1) polar, M = N x 0.5, " + edgesString;
			std::ofstream file(testName + ".csv");
			for (int j = 0; j < pointCounts.size() - 1; j++)
			{
				std::cout << testName << std::endl;
				O1PolarSubdivisionTest t(*poly);
				t.setWedgesPerOctant((poly->vertices.size() / 2) / 8);
				file << pointCounts[j] << ";" << test(&t, points, inclusion, pointCounts[j]) << ";" << std::endl;
				std::string picName = testName + ", N = " + std::to_string(pointCounts[j]) + ".png";
				draw((wchar_t*)std::wstring(picName.begin(), picName.end()).c_str(), poly, points, inclusion, pointCounts[j]);
			}
			file.close();
		}

		{
			testName = "O(1) polar, M = N, " + edgesString;
			std::ofstream file(testName + ".csv");
			for (int j = 0; j < pointCounts.size() - 1; j++)
			{
				std::cout << testName << std::endl;
				O1PolarSubdivisionTest t(*poly);
				t.setWedgesPerOctant(poly->vertices.size() / 8);
				file << pointCounts[j] << ";" << test(&t, points, inclusion, pointCounts[j]) << ";" << std::endl;
				std::string picName = testName + ", N = " + std::to_string(pointCounts[j]) + ".png";
				draw((wchar_t*)std::wstring(picName.begin(), picName.end()).c_str(), poly, points, inclusion, pointCounts[j]);
			}
			file.close();
		}

		{
			testName = "O(1) polar, M = N x 2, " + edgesString;
			std::ofstream file(testName + ".csv");
			for (int j = 0; j < pointCounts.size() - 1; j++)
			{
				std::cout << testName << std::endl;
				O1PolarSubdivisionTest t(*poly);
				t.setWedgesPerOctant((poly->vertices.size() * 2) / 8);
				file << pointCounts[j] << ";" << test(&t, points, inclusion, pointCounts[j]) << ";" << std::endl;
				std::string picName = testName + ", N = " + std::to_string(pointCounts[j]) + ".png";
				draw((wchar_t*)std::wstring(picName.begin(), picName.end()).c_str(), poly, points, inclusion, pointCounts[j]);
			}
			file.close();
		}

		{
			testName = "O(1) polar, M = N x 4, " + edgesString;
			std::ofstream file(testName + ".csv");
			for (int j = 0; j < pointCounts.size() - 1; j++)
			{
				std::cout << testName << std::endl;
				O1PolarSubdivisionTest t(*poly);
				t.setWedgesPerOctant((poly->vertices.size() * 4) / 8);
				file << pointCounts[j] << ";" << test(&t, points, inclusion, pointCounts[j]) << ";" << std::endl;
				std::string picName = testName + ", N = " + std::to_string(pointCounts[j]) + ".png";
				draw((wchar_t*)std::wstring(picName.begin(), picName.end()).c_str(), poly, points, inclusion, pointCounts[j]);
			}
			file.close();
		}

		{
			testName = "O(1) slabs, M = sqrt(N), " + edgesString;
			std::ofstream file(testName + ".csv");
			for (int j = 0; j < pointCounts.size() - 1; j++)
			{
				std::cout << testName << std::endl;
				O1SlabTest t(*poly);
				t.setMaxSlabCount(sqrt(poly->vertices.size()));
				file << pointCounts[j] << ";" << test(&t, points, inclusion, pointCounts[j]) << ";" << std::endl;
				std::string picName = testName + ", N = " + std::to_string(pointCounts[j]) + ".png";
				draw((wchar_t*)std::wstring(picName.begin(), picName.end()).c_str(), poly, points, inclusion, pointCounts[j]);
			}
			file.close();
		}

		{
			testName = "O(1) slabs, M = N x 0.25, " + edgesString;
			std::ofstream file(testName + ".csv");
			for (int j = 0; j < pointCounts.size() - 1; j++)
			{
				std::cout << testName << std::endl;
				O1SlabTest t(*poly);
				t.setMaxSlabCount(poly->vertices.size() / 4);
				file << pointCounts[j] << ";" << test(&t, points, inclusion, pointCounts[j]) << ";" << std::endl;
				std::string picName = testName + ", N = " + std::to_string(pointCounts[j]) + ".png";
				draw((wchar_t*)std::wstring(picName.begin(), picName.end()).c_str(), poly, points, inclusion, pointCounts[j]);
			}
			file.close();
		}

		{
			testName = "O(1) slabs, M = N x 0.5, " + edgesString;
			std::ofstream file(testName + ".csv");
			for (int j = 0; j < pointCounts.size() - 1; j++)
			{
				std::cout << testName << std::endl;
				O1SlabTest t(*poly);
				t.setMaxSlabCount(poly->vertices.size() / 2);
				file << pointCounts[j] << ";" << test(&t, points, inclusion, pointCounts[j]) << ";" << std::endl;
				std::string picName = testName + ", N = " + std::to_string(pointCounts[j]) + ".png";
				draw((wchar_t*)std::wstring(picName.begin(), picName.end()).c_str(), poly, points, inclusion, pointCounts[j]);
			}
			file.close();
		}

		{
			testName = "O(1) slabs, M = N, " + edgesString;
			std::ofstream file(testName + ".csv");
			for (int j = 0; j < pointCounts.size() - 1; j++)
			{
				std::cout << testName << std::endl;
				O1SlabTest t(*poly);
				t.setMaxSlabCount(poly->vertices.size());
				file << pointCounts[j] << ";" << test(&t, points, inclusion, pointCounts[j]) << ";" << std::endl;
				std::string picName = testName + ", N = " + std::to_string(pointCounts[j]) + ".png";
				draw((wchar_t*)std::wstring(picName.begin(), picName.end()).c_str(), poly, points, inclusion, pointCounts[j]);
			}
			file.close();
		}

		{
			testName = "O(1) slabs, M = N x 2, " + edgesString;
			std::ofstream file(testName + ".csv");
			for (int j = 0; j < pointCounts.size() - 1; j++)
			{
				std::cout << testName << std::endl;
				O1SlabTest t(*poly);
				t.setMaxSlabCount(poly->vertices.size() * 2);
				file << pointCounts[j] << ";" << test(&t, points, inclusion, pointCounts[j]) << ";" << std::endl;
				std::string picName = testName + ", N = " + std::to_string(pointCounts[j]) + ".png";
				draw((wchar_t*)std::wstring(picName.begin(), picName.end()).c_str(), poly, points, inclusion, pointCounts[j]);
			}
			file.close();
		}

		{
			testName = "O(1) slabs, M = N x 4, " + edgesString;
			std::ofstream file(testName + ".csv");
			for (int j = 0; j < pointCounts.size() - 1; j++)
			{
				std::cout << testName << std::endl;
				O1SlabTest t(*poly);
				t.setMaxSlabCount(poly->vertices.size() * 4);
				file << pointCounts[j] << ";" << test(&t, points, inclusion, pointCounts[j]) << ";" << std::endl;
				std::string picName = testName + ", N = " + std::to_string(pointCounts[j]) + ".png";
				draw((wchar_t*)std::wstring(picName.begin(), picName.end()).c_str(), poly, points, inclusion, pointCounts[j]);
			}
			file.close();
		}

		delete poly;
	}
	delete[] points;
	delete[] inclusion;
}