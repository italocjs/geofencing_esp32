#include <iostream>
#include <vector>
#include <math.h>

#define IMPL_M_PI 3.14159265358979323846264338327950288

class Point
{
public:
	float latitude;
	float longitude;

	Point(float lat, float lon) : latitude(lat), longitude(lon) {}
};

/**
 * @brief This class help to create a polygon geofence, it can support as many points as your stack can hold.  Tested with 99 points.
 *
 * The is_inside() function implements the point-in-polygon algorithm, which determines whether a given point is inside a polygon or outside
 * it. The algorithm checks the number of times a horizontal line drawn through the point intersects with the edges of the polygon. If the
 * number of intersections is odd, the point is inside the polygon; otherwise, it's outside.
 *
 * Here's a step-by-step explanation of the logic used in the is_inside() function:
 * - The function takes a point p (with latitude and longitude) as its argument.
 * - It initializes some variables: numVertices (number of vertices in the polygon), j (index of the previous vertex), and inside (a flag to
 *     track whether the point is inside or outside the polygon).
 * - The algorithm loops through each pair of consecutive vertices of the polygon (edges). For each edge defined by vertices i and j, it
 *     performs the following checks:
 *      a. It checks if the current edge crosses the horizontal line at the latitude of the given point p.
 *      b. If the above condition is met, it checks if the longitude of the point p is to the left of the intersection point of the edge
 * with the horizontal line. If it is, the point lies inside the polygon.
 *      c. The inside flag is toggled when the edge crosses the horizontal line and the longitude condition is met. After looping through
 * all edges, the inside flag is returned. If it's true, the point is inside the polygon; otherwise, it's outside.
 *
 * In simpler terms, the algorithm calculates how many times the polygon's edges cross the horizontal line through the given point. If this
 * count is odd, the point is inside the polygon. If it's even, the point is outside.
 *
 * This algorithm works for convex and concave polygons, but it assumes that the polygon's vertices are ordered in a counterclockwise
 * manner. For more complex cases, such as self-intersecting polygons, additional checks might be needed.
 *
 * Keep in mind that this algorithm assumes a 2D plane and doesn't account for the curvature of the Earth's surface when dealing with GPS
 * coordinates. For accurate geographic calculations, a more sophisticated library that considers the Earth's geometry is recommended.
 *
 */
class GeoFence
{
private:
	std::vector<Point> points;
	/**
	 * @brief Convert degrees to radians
	 *
	 * @param degrees
	 * @return double
	 */
	static double degrees_to_radians(double degrees) { return degrees * IMPL_M_PI / 180.0; }

public:
	/**
	 * @brief Add a point to the geofence, takes latitude and longitude in decimal degrees as parameters.
	 *
	 * @param lat decimal latitude
	 * @param lon decimal longitude
	 */
	void add_point(float lat, float lon) { points.emplace_back(lat, lon); }

	/**
	 * @brief Check if a point is inside the geofence (the geofence is created by adding points to it)
	 *
	 * @param p
	 * @param debug
	 * @return true
	 * @return false
	 */
	bool is_inside(const Point &p, bool debug = false)
	{
		int numVertices = points.size();
		int j = numVertices - 1;
		bool inside = false;
		static int counter_of_calls = 0;
		counter_of_calls++;

		for (int i = 0; i < numVertices; i++)
		{
			if ((points[i].latitude < p.latitude && points[j].latitude >= p.latitude) ||
				(points[j].latitude < p.latitude && points[i].latitude >= p.latitude))
			{
				if (points[i].longitude + (p.latitude - points[i].latitude) / (points[j].latitude - points[i].latitude) *
											  (points[j].longitude - points[i].longitude) <
					p.longitude)
				{
					inside = !inside;
				}
			}
			j = i;
		}

		if (debug)
			(inside) ? printf("inside geofence, %d vertices.\n", numVertices) : printf("outside geofence, %d vertices.\n", numVertices);

		return inside;
	}

	/**
	 * @brief Calculate the distance between two points, this function uses approximate values for the radius of the earth instead of an geoid model for faster calculation.
	 *
	 * @param coordinate1
	 * @param coordinate2
	 * @param debug
	 * @return value in meters
	 */
	static double calculate_distance(Point coordinate1, Point coordinate2, bool debug = false)
	{
		double lat1, lon1, lat2, lon2;
		lat1 = degrees_to_radians(coordinate1.latitude);
		lon1 = degrees_to_radians(coordinate1.longitude);
		lat2 = degrees_to_radians(coordinate2.latitude);
		lon2 = degrees_to_radians(coordinate2.longitude);

		// Haversine formula to calculate distance
		double dlat = lat2 - lat1;
		double dlon = lon2 - lon1;
		double a = sin(dlat / 2) * sin(dlat / 2) + cos(lat1) * cos(lat2) * sin(dlon / 2) * sin(dlon / 2);
		double c = 2 * atan2(sqrt(a), sqrt(1 - a));

		// Calculate the distance
		double RADIUS_OF_EARTH = 6371.0; // Radius in kilometers
		double distance = RADIUS_OF_EARTH * c;

		// Convert distance to meters
		distance = distance * 1000;

		if (debug)
			printf("distance: %f meters\n", distance);
		return distance;
	}
};

/**
 * @brief Test the geofence with 4 points, the geofence is a random neigborhood in Brazil.
 *
 * @return int
 */
int test_geofence_4points()
{
	printf("test_geofence_4points()\n");
	// geofence defined near Simova, has 4 points defining the polygon shape
	GeoFence geoFence;

	// Define the vertices of the polygon
	geoFence.add_point(-23.207486, -45.907859); // simova p1
	geoFence.add_point(-23.209189, -45.909029); // simova p2
	geoFence.add_point(-23.211687, -45.909443); // simova p3
	geoFence.add_point(-23.212556, -45.902455); // simova p4

	// Check if test points are inside or outside the geofence
	Point testPoint1(-23.209565, -45.907350); // must return true (its inside)
	Point testPoint2(-23.211250, -45.906183); // must return true (its inside)
	Point testPoint3(-23.210104, -45.904434); // must return false (its outside)
	Point testPoint4(-23.214471, -45.906442); // must return false (its outside)

	bool testPoint1_isInside = geoFence.is_inside(testPoint1);
	bool testPoint2_isInside = geoFence.is_inside(testPoint2);
	bool testPoint3_isInside = geoFence.is_inside(testPoint3);
	bool testPoint4_isInside = geoFence.is_inside(testPoint4);

	// print the results
	printf("\ttestPoint1 is inside the geofence: %s\n", testPoint1_isInside ? "true" : "false");
	printf("\ttestPoint2 is inside the geofence: %s\n", testPoint2_isInside ? "true" : "false");
	printf("\ttestPoint3 is inside the geofence: %s\n", testPoint3_isInside ? "true" : "false");
	printf("\ttestPoint4 is inside the geofence: %s\n", testPoint4_isInside ? "true" : "false");

	if (testPoint1_isInside && testPoint2_isInside && !testPoint3_isInside && !testPoint4_isInside)
	{
		printf("\ttest_geofence_4points() passed.\n");
		return 1;
	}
	printf("\ttest_geofence_4points() failed.\n");
	return 0;
}

int test_geofence_99points()
{
	printf("test_geofence_99points()\n");
	GeoFence geoFence;
	geoFence.add_point(-45.930582, -23.195937); // p1 point 1
	geoFence.add_point(-45.931122, -23.196960); // p1 point 2
	geoFence.add_point(-45.932497, -23.197128); // p1 point 3
	geoFence.add_point(-45.933112, -23.197460); // p1 point 4
	geoFence.add_point(-45.933307, -23.198011); // p1 point 5
	geoFence.add_point(-45.933770, -23.198606); // p1 point 6
	geoFence.add_point(-45.933917, -23.199494); // p1 point 7
	geoFence.add_point(-45.934150, -23.200621); // p1 point 8
	geoFence.add_point(-45.934922, -23.201239); // p1 point 9
	geoFence.add_point(-45.936356, -23.201291); // p1 point 10
	geoFence.add_point(-45.937248, -23.201334); // p1 point 11
	geoFence.add_point(-45.937954, -23.202035); // p1 point 12
	geoFence.add_point(-45.938150, -23.203679); // p1 point 13
	geoFence.add_point(-45.938141, -23.204431); // p1 point 14
	geoFence.add_point(-45.939238, -23.205125); // p1 point 15
	geoFence.add_point(-45.940772, -23.205427); // p1 point 16
	geoFence.add_point(-45.941977, -23.206435); // p1 point 17
	geoFence.add_point(-45.942081, -23.207604); // p1 point 18
	geoFence.add_point(-45.941863, -23.208611); // p1 point 19
	geoFence.add_point(-45.942389, -23.209561); // p1 point 20
	geoFence.add_point(-45.942705, -23.210153); // p1 point 21
	geoFence.add_point(-45.944603, -23.210111); // p1 point 22
	geoFence.add_point(-45.946429, -23.209758); // p1 point 23
	geoFence.add_point(-45.947746, -23.209533); // p1 point 24
	geoFence.add_point(-45.948782, -23.209742); // p1 point 25
	geoFence.add_point(-45.949126, -23.210693); // p1 point 26
	geoFence.add_point(-45.948613, -23.211423); // p1 point 27
	geoFence.add_point(-45.948111, -23.212579); // p1 point 28
	geoFence.add_point(-45.948448, -23.213811); // p1 point 29
	geoFence.add_point(-45.949035, -23.214606); // p1 point 30
	geoFence.add_point(-45.950244, -23.215557); // p1 point 31
	geoFence.add_point(-45.951322, -23.215949); // p1 point 32
	geoFence.add_point(-45.952534, -23.215599); // p1 point 33
	geoFence.add_point(-45.952986, -23.215449); // p1 point 34
	geoFence.add_point(-45.953347, -23.216031); // p1 point 35
	geoFence.add_point(-45.953834, -23.216133); // p1 point 36
	geoFence.add_point(-45.953708, -23.216827); // p1 point 37
	geoFence.add_point(-45.954291, -23.218421); // p1 point 38
	geoFence.add_point(-45.954262, -23.219410); // p1 point 39
	geoFence.add_point(-45.955055, -23.220920); // p1 point 40
	geoFence.add_point(-45.955697, -23.221096); // p1 point 41
	geoFence.add_point(-45.956628, -23.220178); // p1 point 42
	geoFence.add_point(-45.957604, -23.219200); // p1 point 43
	geoFence.add_point(-45.958779, -23.219078); // p1 point 44
	geoFence.add_point(-45.959978, -23.219161); // p1 point 45
	geoFence.add_point(-45.961018, -23.219807); // p1 point 46
	geoFence.add_point(-45.961323, -23.220700); // p1 point 47
	geoFence.add_point(-45.961077, -23.221952); // p1 point 48
	geoFence.add_point(-45.960814, -23.223498); // p1 point 49
	geoFence.add_point(-45.960420, -23.223539); // p1 point 50
	geoFence.add_point(-45.960473, -23.222251); // p1 point 51
	geoFence.add_point(-45.960762, -23.220826); // p1 point 52
	geoFence.add_point(-45.960684, -23.219987); // p1 point 53
	geoFence.add_point(-45.959043, -23.219296); // p1 point 54
	geoFence.add_point(-45.957568, -23.219497); // p1 point 55
	geoFence.add_point(-45.956651, -23.220767); // p1 point 56
	geoFence.add_point(-45.955765, -23.221607); // p1 point 57
	geoFence.add_point(-45.954810, -23.221344); // p1 point 58
	geoFence.add_point(-45.953896, -23.219650); // p1 point 59
	geoFence.add_point(-45.953875, -23.218428); // p1 point 60
	geoFence.add_point(-45.953439, -23.217163); // p1 point 61
	geoFence.add_point(-45.952663, -23.216373); // p1 point 62
	geoFence.add_point(-45.951965, -23.216108); // p1 point 63
	geoFence.add_point(-45.951271, -23.216297); // p1 point 64
	geoFence.add_point(-45.949934, -23.215875); // p1 point 65
	geoFence.add_point(-45.948822, -23.215176); // p1 point 66
	geoFence.add_point(-45.948060, -23.214206); // p1 point 67
	geoFence.add_point(-45.947611, -23.212483); // p1 point 68
	geoFence.add_point(-45.947853, -23.211760); // p1 point 69
	geoFence.add_point(-45.948348, -23.211108); // p1 point 70
	geoFence.add_point(-45.948650, -23.210533); // p1 point 71
	geoFence.add_point(-45.948365, -23.209929); // p1 point 72
	geoFence.add_point(-45.947487, -23.209894); // p1 point 73
	geoFence.add_point(-45.945726, -23.210243); // p1 point 74
	geoFence.add_point(-45.943948, -23.210722); // p1 point 75
	geoFence.add_point(-45.942791, -23.210577); // p1 point 76
	geoFence.add_point(-45.941839, -23.209708); // p1 point 77
	geoFence.add_point(-45.941452, -23.208573); // p1 point 78
	geoFence.add_point(-45.941596, -23.207369); // p1 point 79
	geoFence.add_point(-45.941429, -23.206227); // p1 point 80
	geoFence.add_point(-45.940245, -23.205566); // p1 point 81
	geoFence.add_point(-45.938971, -23.205246); // p1 point 82
	geoFence.add_point(-45.937762, -23.204880); // p1 point 83
	geoFence.add_point(-45.937619, -23.204049); // p1 point 84
	geoFence.add_point(-45.937713, -23.203284); // p1 point 85
	geoFence.add_point(-45.937648, -23.202293); // p1 point 86
	geoFence.add_point(-45.937228, -23.201722); // p1 point 87
	geoFence.add_point(-45.936145, -23.201570); // p1 point 88
	geoFence.add_point(-45.934826, -23.201536); // p1 point 89
	geoFence.add_point(-45.933788, -23.200818); // p1 point 90
	geoFence.add_point(-45.933475, -23.199884); // p1 point 91
	geoFence.add_point(-45.933298, -23.198600); // p1 point 92
	geoFence.add_point(-45.932640, -23.197828); // p1 point 93
	geoFence.add_point(-45.931838, -23.197502); // p1 point 94
	geoFence.add_point(-45.931160, -23.197357); // p1 point 95
	geoFence.add_point(-45.930429, -23.196960); // p1 point 96
	geoFence.add_point(-45.930396, -23.196418); // p1 point 97
	geoFence.add_point(-45.930144, -23.196026); // p1 point 98
	geoFence.add_point(-45.930582, -23.195937); // p1 point 99
	Point test1(-45.930756, -23.196812);		// test1
	Point test2(-45.932583, -23.198608);		// test2
	Point test3(-45.937060, -23.201438);		// test3

	bool testPoint1_isInside = geoFence.is_inside(test1);
	bool testPoint2_isInside = geoFence.is_inside(test2);
	bool testPoint3_isInside = geoFence.is_inside(test3);

	// print the results
	printf("\ttest1 is inside the geofence: %s \n", testPoint1_isInside ? "true" : "false");
	printf("\ttest2 is inside the geofence: %s \n", testPoint2_isInside ? "true" : "false");
	printf("\ttest3 is inside the geofence: %s \n", testPoint3_isInside ? "true" : "false");

	if (testPoint1_isInside && !testPoint2_isInside && testPoint3_isInside)
	{
		printf("\ttest_geofence_99points() passed.\n");
		return 1;
	}
	printf("\ttest_geofence_99points() failed.\n");
	return 0;
}

// function to test the calculate_distance function

/**
 * @brief Test the calculate_distance function, this function uses approximate values for the radius of the earth instead of an geoid model for faster calculation.
 * Set tolerable_error to your desire. this class does not use geoide models, so there will be a difference when comparing to GoogleEarth measurements.
 */
int test_calculate_distance()
{
	printf("test_calculate_distance()\n");
	double tolerable_error = 5.0; // how many meters of error is acceptable? Remember that this class does not use geoide models, so there will be a difference when comparing to GoogleEarth measurements.
	double distance_reference = 452.22873765131664; // in meters using using the Haversine formula (python).
	//double distance_reference = 450.77551416860337; // in meters using Geodesic model for more precision (python).

	GeoFence geoFence;
	Point coordinate_01(-23.207486, -45.907859);
	Point coordinate_02(-23.211250, -45.906183);
	double distance = geoFence.calculate_distance(coordinate_01, coordinate_02);

	// check if distance is within the tolerable error
	double difference = fabs(distance - distance_reference);
	printf("\tCalculated distance: %f, reference distance %f, difference %f\n", distance, distance_reference, difference);

	if (difference < tolerable_error)
	{
		printf("\ttest_calculate_distance() passed\n");
		return 1;
	}
	printf("\ttest_calculate_distance() failed\n");
	return 0;
}

int main()
{
	printf("Running tests...\n");
	test_geofence_4points();
	test_geofence_99points();
	test_calculate_distance();
	return 1;
}