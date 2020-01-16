#include <iostream>
#include <Utilities.h>
#include <Steady3D.h>
#include <StreamlineTracer.h>
#include <StreamSurfaceTracer.h>

#include "Steady3D_Bernard.h"

using namespace std;


enum TestCases {
	SingleStreamLine_TestCase = 0,
	MultipleStreamlines_TestCase,
	StreamSurfaceWithSeedingCurve_TestCase
};

void testSingleStreamline();
void testMultipleStreamline();
void testStreamSurfaceWithSeedingCurve();

/* Generate a single streamline from a predefined seeding point*/
void testSingleStreamline() {
	std::cout << "Start testSingleStreamline()..." << std::endl;
	// Step 1: Load a vector field. In this example, we use the Bernard dataset
	Steady3D_Bernard bernardVectorField("../../sample_data/bernard.raw");

	// Step 2: Initialize the streamline tracer
	StreamlineTracer streamlineTracer(bernardVectorField);
	streamlineTracer.setForward(true);
	streamlineTracer.setStepSize(0.1);
	int streamline_length = 50;

	// Step 3: Create a seeding point
	Vector3f sampleSeedingPoint(2.51969, 0.548387, 0.444444);

	// Step 4: Trace the streamline
	vector<Vector3f> streamline = streamlineTracer.trace(sampleSeedingPoint, streamline_length);

	// Step 5: Save the streamline to a vtk file
	vector<vector<Vector3f>> outputLines;
	outputLines.push_back(streamline);
	Util::saveLinesToVTKFile(outputLines, "../../output/singleStreamline.vtk");

	std::cout << "Finish testSingleStreamline()!" << std::endl;
	
}

/* Generate multiple streamlines from a seeding curve*/
void testMultipleStreamline() {

	std::cout << "Start testMultipleStreamline()..." << std::endl;
	// Step 1: Load a vector field. In this example, we use the Bernard dataset
	Steady3D_Bernard bernardVectorField("../../sample_data/bernard.raw");

	// Step 2: Initialize the streamline tracer
	StreamlineTracer streamlineTracer(bernardVectorField);
	streamlineTracer.setForward(true);
	streamlineTracer.setStepSize(0.1);
	int streamline_length = 50;

	// Step 3: Create a seeding curve with 4 points
	vector<Vector3f> seedingCurve;
	seedingCurve.push_back(Vector3f(2.51969, 0.548387, 0.444444));
	seedingCurve.push_back(Vector3f(2.51432, 0.504238, 0.486014));
	seedingCurve.push_back(Vector3f(2.50522, 0.478441, 0.534027));
	seedingCurve.push_back(Vector3f(2.49611, 0.452904, 0.581806));

	// Step 4: Generate streamlines 
	vector<vector<Vector3f>> allStreamlines;
	for (int i = 0; i < seedingCurve.size(); i++) {
		vector<Vector3f> streamline = streamlineTracer.trace(seedingCurve[i], streamline_length);
		allStreamlines.push_back(streamline);
	}
	

	// Step 4: Save the streamlines to a vtk file
	
	Util::saveLinesToVTKFile(allStreamlines, "../../output/multipleStreamline.vtk");

	std::cout << "Finish testMultipleStreamline()!" << std::endl;

}

/* Generate a surface given a seeding curve*/
void testStreamSurfaceWithSeedingCurve() {
	std::cout << "Start testStreamSurfaceWithSeedingCurve()..." << std::endl;
	// Step 1: Load a vector field. In this example, we use the Bernard dataset
	Steady3D_Bernard bernardVectorField("../../sample_data/bernard.raw");

	// Step 2: Initialize the streamline tracer
	StreamSurfaceTracer streamSurfaceTracer(bernardVectorField);
	streamSurfaceTracer.setForward(true);
	streamSurfaceTracer.setStepSize(0.1);
	int streamline_length = 50;

	// Step 3: Create a seeding curve with 4 points
	vector<Vector3f> seedingCurve;
	seedingCurve.push_back(Vector3f(2.51969, 0.548387, 0.444444));
	seedingCurve.push_back(Vector3f(2.51432, 0.504238, 0.486014));
	seedingCurve.push_back(Vector3f(2.50522, 0.478441, 0.534027));
	seedingCurve.push_back(Vector3f(2.49611, 0.452904, 0.581806));

	// Step 4: Generate a stream surface 
	vector<Vector3f> vertices;
	vector<Vector3i> faces;
	streamSurfaceTracer.trace(seedingCurve, streamline_length, vertices, faces);


	// Step 4: Save the streamlines to a vtk file

	Util::outputSurfaceToOBJ(vertices, faces, "../../output/streamsurface.obj");

	std::cout << "Finish testStreamSurfaceWithSeedingCurve()!" << std::endl;
}

int main(int argc, char **argv)
{

	for (int testCase = 0; testCase < 3; testCase++) {

		switch (testCase)
		{
		case SingleStreamLine_TestCase:
			testSingleStreamline();
			break;
		case MultipleStreamlines_TestCase:
			testMultipleStreamline();
			break;
		case StreamSurfaceWithSeedingCurve_TestCase:
			testStreamSurfaceWithSeedingCurve();
			break;
		}
	}
	
	system("PAUSE");
	return 0;
}