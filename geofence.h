#pragma once
#include <vector>
#include <cmath>    // Include cmath for math functions and M_PI
#include <limits>   // Include limits for numeric_limits
#include <cstdio>   // Include cstdio for printf

// Detect environment and include appropriate headers
#if defined(_WIN32) || defined(__linux__)
#include <math.h>
#include <iostream>
#define IMPL_M_PI 3.14159265358979323846    // same as esp32
// define the esp_err_t for windows and linux
#elif defined(ESP32) || defined(ARDUINO)
#include <math.h>   // Ensure M_PI is defined
#define IMPL_M_PI M_PI
#else
#warning "Unknown environment, please check your build environment."
#endif

class GPS_Coordinate
{
   public:
	float latitude;
	float longitude;

	GPS_Coordinate(float lat, float lon) : latitude(lat), longitude(lon) {}
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
	/**
	 * @brief Convert degrees to radians
	 *
	 * @param degrees
	 * @return double
	 */
	static double degrees_to_radians(double degrees) { return degrees * IMPL_M_PI / 180.0; }

   public:
	std::vector<GPS_Coordinate> boundary_coordinates;

	static double haversineDistance(const GPS_Coordinate &a, const GPS_Coordinate &b)
	{
		const double R = 6371.0;    // Radius of Earth in km
		double dlat = (b.latitude - a.latitude) * IMPL_M_PI / 180.0;
		double dlon = (b.longitude - a.longitude) * IMPL_M_PI / 180.0;
		double lat1 = a.latitude * IMPL_M_PI / 180.0;
		double lat2 = b.latitude * IMPL_M_PI / 180.0;

		double d = sin(dlat / 2) * sin(dlat / 2) + sin(dlon / 2) * sin(dlon / 2) * cos(lat1) * cos(lat2);
		double c = 2 * atan2(sqrt(d), sqrt(1 - d));
		return R * c;
	}

	static double boundary_vertice_to_coordinate_distance(const std::vector<GPS_Coordinate> &boundary, GPS_Coordinate &coordinates)
	{
		double minDistance = std::numeric_limits<double>::max();
		for (const auto &bound : boundary)
		{
			double distance = haversineDistance(coordinates, bound);
			if (distance < minDistance)
			{
				minDistance = distance;
			}
		}
		return minDistance * 1000;    // convert km to meters
	}

	double distance_to_boundary(const GPS_Coordinate &p, bool debug = false)
	{
		double min_distance = std::numeric_limits<double>::max();

		int numVertices = boundary_coordinates.size();
		for (int i = 0; i < numVertices; i++)
		{
			GPS_Coordinate A = boundary_coordinates[i];
			GPS_Coordinate B = boundary_coordinates[(i + 1) % numVertices];    // Next point, with wrap-around

			// Calculate distance from point P to line segment AB
			double distance = calculate_distance_to_segment(A, B, p);

			if (distance < min_distance)
			{
				min_distance = distance;
			}
		}

		if (debug) printf("Minimum distance to boundary: %f meters\n", min_distance);

		return min_distance;
	}

	static double calculate_distance_to_segment(GPS_Coordinate A, GPS_Coordinate B, GPS_Coordinate P)
	{
		// First, find the nearest point on the line AB to point P
		double latA = degrees_to_radians(A.latitude);
		double lonA = degrees_to_radians(A.longitude);
		double latB = degrees_to_radians(B.latitude);
		double lonB = degrees_to_radians(B.longitude);
		double latP = degrees_to_radians(P.latitude);
		double lonP = degrees_to_radians(P.longitude);

		// Vector from A to B
		double Ax = cos(latA) * cos(lonA);
		double Ay = cos(latA) * sin(lonA);
		double Az = sin(latA);
		double Bx = cos(latB) * cos(lonB);
		double By = cos(latB) * sin(lonB);
		double Bz = sin(latB);

		// Vector from A to P
		double Px = cos(latP) * cos(lonP);
		double Py = cos(latP) * sin(lonP);
		double Pz = sin(latP);

		// Calculate the nearest point on the line AB to point P
		double t = ((Px - Ax) * (Bx - Ax) + (Py - Ay) * (By - Ay) + (Pz - Az) * (Bz - Az)) /
		           ((Bx - Ax) * (Bx - Ax) + (By - Ay) * (By - Ay) + (Bz - Az) * (Bz - Az));

		// Limit t to the range [0, 1] to stay within the segment AB
		if (t < 0) t = 0;
		if (t > 1) t = 1;

		// Calculate the nearest point Q on the line segment AB
		double Qx = Ax + t * (Bx - Ax);
		double Qy = Ay + t * (By - Ay);
		double Qz = Az + t * (Bz - Az);

		// Calculate the distance from P to Q
		double distance = sqrt((Px - Qx) * (Px - Qx) + (Py - Qy) * (Py - Qy) + (Pz - Qz) * (Pz - Qz));

		// Convert the distance to meters using the Earth's radius
		double RADIUS_OF_EARTH = 6371.0;    // Radius in kilometers
		distance = distance * RADIUS_OF_EARTH * 1000;

		return distance;
	}

	static double radians_to_degrees(double radians) { return radians * 180.0 / IMPL_M_PI; }

	/**
	 * @brief Add a point to the geofence, takes latitude and longitude in decimal degrees as parameters.
	 *
	 * @param lat decimal latitude
	 * @param lon decimal longitude
	 */
	void add_point(float lat, float lon) { boundary_coordinates.emplace_back(lat, lon); }

	/**
	 * @brief Check if a point is inside the geofence (the geofence is created by adding points to it)
	 *
	 * @param p
	 * @param debug
	 * @return true
	 * @return false
	 */
	bool is_inside(const GPS_Coordinate &p, bool debug = false)
	{
		int numVertices = boundary_coordinates.size();
		int j = numVertices - 1;
		bool inside = false;
		static int counter_of_calls = 0;
		counter_of_calls++;

		for (int i = 0; i < numVertices; i++)
		{
			if ((boundary_coordinates[i].latitude < p.latitude && boundary_coordinates[j].latitude >= p.latitude) ||
			    (boundary_coordinates[j].latitude < p.latitude && boundary_coordinates[i].latitude >= p.latitude))
			{
				if (boundary_coordinates[i].longitude + (p.latitude - boundary_coordinates[i].latitude) /
				                                            (boundary_coordinates[j].latitude - boundary_coordinates[i].latitude) *
				                                            (boundary_coordinates[j].longitude - boundary_coordinates[i].longitude) <
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
	 * @brief Calculate the distance between two points, this function uses approximate values for the radius of the earth instead of an
	 * geoid model for faster calculation.
	 *
	 * @param coordinate1
	 * @param coordinate2
	 * @param debug
	 * @return value in meters
	 */
	static double distance_between_coordinates(GPS_Coordinate coordinate1, GPS_Coordinate coordinate2, bool debug = false)
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
		double RADIUS_OF_EARTH = 6371.0;    // Radius in kilometers
		double distance = RADIUS_OF_EARTH * c;

		// Convert distance to meters
		distance = distance * 1000;

		if (debug) printf("distance: %f meters\n", distance);
		return distance;
	}
};
