/**
 * @file class_testing.h
 * @author Italo Soares (italocjs@live.com)
 * @brief This file contains the tests for the geofence class. its been tested under windows, linux, esp32 and arduino.
 * @version 0.1
 * @date 2023-09-07
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include "geofence.h"

#if defined(ESP32) || defined(ARDUINO)
#include "Arduino.h"
#endif

/**
 * @brief Test the geofence with 4 points, the geofence is a random neigborhood in Brazil.
 *
 * @return int
 */
bool test_geofence_4points()
{
	printf("test_geofence_4points()\n");
	// geofence defined near Simova, has 4 points defining the polygon shape
	GeoFence geoFence;

	// Define the vertices of the polygon
	geoFence.add_point(-23.207486, -45.907859);    // simova p1
	geoFence.add_point(-23.209189, -45.909029);    // simova p2
	geoFence.add_point(-23.211687, -45.909443);    // simova p3
	geoFence.add_point(-23.212556, -45.902455);    // simova p4

	// Check if test points are inside or outside the geofence
	GPS_Coordinate testPoint1(-23.209565, -45.907350);    // must return true (its inside)
	GPS_Coordinate testPoint2(-23.211250, -45.906183);    // must return true (its inside)
	GPS_Coordinate testPoint3(-23.210104, -45.904434);    // must return false (its outside)
	GPS_Coordinate testPoint4(-23.214471, -45.906442);    // must return false (its outside)

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

bool test_fence_distance()
{
	printf("test_fence_distance()\n");
	GeoFence fence;
	fence.add_point(-23.207486, -45.907859);                   // p1
	fence.add_point(-23.209189, -45.909029);                   // p2
	fence.add_point(-23.211687, -45.909443);                   // p3
	fence.add_point(-23.212556, -45.902455);                   // p4
	GPS_Coordinate test_coordinate(-23.214471, -45.906442);    // test coordinate outside the fence

	double acceptable_error = 5;               // in meters
	double gmaps_distance_to_fence = 265.0;    // obtained on google earth
	double lib_distance_to_fence = fence.distance_to_boundary(test_coordinate, false);
	double lib_dtf_error = abs(gmaps_distance_to_fence - lib_distance_to_fence);
	printf("\tgmaps_distance_to_fence: %0.2fm, lib_distance_to_fence: %0.2fm, error: %0.2fm, acceptable_error: %0.2f\n",
	       gmaps_distance_to_fence, lib_distance_to_fence, lib_dtf_error, acceptable_error);

	double gmaps_distance_to_nearest_vertice = 435.01;    // obtained on google earth
	double lib_distance_to_nearest_vertice = GeoFence::boundary_vertice_to_coordinate_distance(fence.boundary_coordinates, test_coordinate);
	double lib_dtnv_error = abs(gmaps_distance_to_nearest_vertice - lib_distance_to_nearest_vertice);
	printf("\tgmaps_distance_to_nearest_vertice: %0.2fm, lib_distance_to_nearest_vertice: %0.2fm, error: %0.2fm, acceptable_error: %0.2f\n",
	       gmaps_distance_to_nearest_vertice, lib_distance_to_nearest_vertice, lib_dtnv_error, acceptable_error);

	if (lib_dtf_error > acceptable_error)
	{
		printf("\tgmaps_distance_to_fence() failed, error above %0.2f\n", acceptable_error);
		return 0;
	}

	if (lib_dtnv_error > acceptable_error)
	{
		printf("\tgmaps_distance_to_nearest_vertice() failed, error above %0.2f\n", acceptable_error);
		return 0;
	}

	printf("\ttest_fence_distance() passed.\n");
	return 1;
}

bool test_geofence_99points()
{
	printf("test_geofence_99points()\n");
	GeoFence geoFence;
	geoFence.add_point(-45.930582, -23.195937);      // p1 point 1
	geoFence.add_point(-45.931122, -23.196960);      // p1 point 2
	geoFence.add_point(-45.932497, -23.197128);      // p1 point 3
	geoFence.add_point(-45.933112, -23.197460);      // p1 point 4
	geoFence.add_point(-45.933307, -23.198011);      // p1 point 5
	geoFence.add_point(-45.933770, -23.198606);      // p1 point 6
	geoFence.add_point(-45.933917, -23.199494);      // p1 point 7
	geoFence.add_point(-45.934150, -23.200621);      // p1 point 8
	geoFence.add_point(-45.934922, -23.201239);      // p1 point 9
	geoFence.add_point(-45.936356, -23.201291);      // p1 point 10
	geoFence.add_point(-45.937248, -23.201334);      // p1 point 11
	geoFence.add_point(-45.937954, -23.202035);      // p1 point 12
	geoFence.add_point(-45.938150, -23.203679);      // p1 point 13
	geoFence.add_point(-45.938141, -23.204431);      // p1 point 14
	geoFence.add_point(-45.939238, -23.205125);      // p1 point 15
	geoFence.add_point(-45.940772, -23.205427);      // p1 point 16
	geoFence.add_point(-45.941977, -23.206435);      // p1 point 17
	geoFence.add_point(-45.942081, -23.207604);      // p1 point 18
	geoFence.add_point(-45.941863, -23.208611);      // p1 point 19
	geoFence.add_point(-45.942389, -23.209561);      // p1 point 20
	geoFence.add_point(-45.942705, -23.210153);      // p1 point 21
	geoFence.add_point(-45.944603, -23.210111);      // p1 point 22
	geoFence.add_point(-45.946429, -23.209758);      // p1 point 23
	geoFence.add_point(-45.947746, -23.209533);      // p1 point 24
	geoFence.add_point(-45.948782, -23.209742);      // p1 point 25
	geoFence.add_point(-45.949126, -23.210693);      // p1 point 26
	geoFence.add_point(-45.948613, -23.211423);      // p1 point 27
	geoFence.add_point(-45.948111, -23.212579);      // p1 point 28
	geoFence.add_point(-45.948448, -23.213811);      // p1 point 29
	geoFence.add_point(-45.949035, -23.214606);      // p1 point 30
	geoFence.add_point(-45.950244, -23.215557);      // p1 point 31
	geoFence.add_point(-45.951322, -23.215949);      // p1 point 32
	geoFence.add_point(-45.952534, -23.215599);      // p1 point 33
	geoFence.add_point(-45.952986, -23.215449);      // p1 point 34
	geoFence.add_point(-45.953347, -23.216031);      // p1 point 35
	geoFence.add_point(-45.953834, -23.216133);      // p1 point 36
	geoFence.add_point(-45.953708, -23.216827);      // p1 point 37
	geoFence.add_point(-45.954291, -23.218421);      // p1 point 38
	geoFence.add_point(-45.954262, -23.219410);      // p1 point 39
	geoFence.add_point(-45.955055, -23.220920);      // p1 point 40
	geoFence.add_point(-45.955697, -23.221096);      // p1 point 41
	geoFence.add_point(-45.956628, -23.220178);      // p1 point 42
	geoFence.add_point(-45.957604, -23.219200);      // p1 point 43
	geoFence.add_point(-45.958779, -23.219078);      // p1 point 44
	geoFence.add_point(-45.959978, -23.219161);      // p1 point 45
	geoFence.add_point(-45.961018, -23.219807);      // p1 point 46
	geoFence.add_point(-45.961323, -23.220700);      // p1 point 47
	geoFence.add_point(-45.961077, -23.221952);      // p1 point 48
	geoFence.add_point(-45.960814, -23.223498);      // p1 point 49
	geoFence.add_point(-45.960420, -23.223539);      // p1 point 50
	geoFence.add_point(-45.960473, -23.222251);      // p1 point 51
	geoFence.add_point(-45.960762, -23.220826);      // p1 point 52
	geoFence.add_point(-45.960684, -23.219987);      // p1 point 53
	geoFence.add_point(-45.959043, -23.219296);      // p1 point 54
	geoFence.add_point(-45.957568, -23.219497);      // p1 point 55
	geoFence.add_point(-45.956651, -23.220767);      // p1 point 56
	geoFence.add_point(-45.955765, -23.221607);      // p1 point 57
	geoFence.add_point(-45.954810, -23.221344);      // p1 point 58
	geoFence.add_point(-45.953896, -23.219650);      // p1 point 59
	geoFence.add_point(-45.953875, -23.218428);      // p1 point 60
	geoFence.add_point(-45.953439, -23.217163);      // p1 point 61
	geoFence.add_point(-45.952663, -23.216373);      // p1 point 62
	geoFence.add_point(-45.951965, -23.216108);      // p1 point 63
	geoFence.add_point(-45.951271, -23.216297);      // p1 point 64
	geoFence.add_point(-45.949934, -23.215875);      // p1 point 65
	geoFence.add_point(-45.948822, -23.215176);      // p1 point 66
	geoFence.add_point(-45.948060, -23.214206);      // p1 point 67
	geoFence.add_point(-45.947611, -23.212483);      // p1 point 68
	geoFence.add_point(-45.947853, -23.211760);      // p1 point 69
	geoFence.add_point(-45.948348, -23.211108);      // p1 point 70
	geoFence.add_point(-45.948650, -23.210533);      // p1 point 71
	geoFence.add_point(-45.948365, -23.209929);      // p1 point 72
	geoFence.add_point(-45.947487, -23.209894);      // p1 point 73
	geoFence.add_point(-45.945726, -23.210243);      // p1 point 74
	geoFence.add_point(-45.943948, -23.210722);      // p1 point 75
	geoFence.add_point(-45.942791, -23.210577);      // p1 point 76
	geoFence.add_point(-45.941839, -23.209708);      // p1 point 77
	geoFence.add_point(-45.941452, -23.208573);      // p1 point 78
	geoFence.add_point(-45.941596, -23.207369);      // p1 point 79
	geoFence.add_point(-45.941429, -23.206227);      // p1 point 80
	geoFence.add_point(-45.940245, -23.205566);      // p1 point 81
	geoFence.add_point(-45.938971, -23.205246);      // p1 point 82
	geoFence.add_point(-45.937762, -23.204880);      // p1 point 83
	geoFence.add_point(-45.937619, -23.204049);      // p1 point 84
	geoFence.add_point(-45.937713, -23.203284);      // p1 point 85
	geoFence.add_point(-45.937648, -23.202293);      // p1 point 86
	geoFence.add_point(-45.937228, -23.201722);      // p1 point 87
	geoFence.add_point(-45.936145, -23.201570);      // p1 point 88
	geoFence.add_point(-45.934826, -23.201536);      // p1 point 89
	geoFence.add_point(-45.933788, -23.200818);      // p1 point 90
	geoFence.add_point(-45.933475, -23.199884);      // p1 point 91
	geoFence.add_point(-45.933298, -23.198600);      // p1 point 92
	geoFence.add_point(-45.932640, -23.197828);      // p1 point 93
	geoFence.add_point(-45.931838, -23.197502);      // p1 point 94
	geoFence.add_point(-45.931160, -23.197357);      // p1 point 95
	geoFence.add_point(-45.930429, -23.196960);      // p1 point 96
	geoFence.add_point(-45.930396, -23.196418);      // p1 point 97
	geoFence.add_point(-45.930144, -23.196026);      // p1 point 98
	geoFence.add_point(-45.930582, -23.195937);      // p1 point 99
	GPS_Coordinate test1(-45.930756, -23.196812);    // test1 - inside
	GPS_Coordinate test2(-45.932583, -23.198608);    // test2 - outside
	GPS_Coordinate test3(-45.937060, -23.201438);    // test3 - inside

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

/**
 * @brief Test the geofence with 4 points, the geofence is a random neigborhood in Brazil.
 *
 * @return int
 */
bool test_geofence_norway_450points()
{
	printf("test_geofence_norway_450points()\n");
	// geofence defined near Simova, has 4 points defining the polygon shape
	GeoFence norway_fence;
	/* #region  */
	GeoFence geoFence;
	norway_fence.add_point(4.659663, 61.594989);                        // norway_poligon point 1
	norway_fence.add_point(4.751647, 61.508207);                        // norway_poligon point 2
	norway_fence.add_point(4.947209, 61.500731);                        // norway_poligon point 3
	norway_fence.add_point(4.909767, 61.399444);                        // norway_poligon point 4
	norway_fence.add_point(4.659075, 61.312203);                        // norway_poligon point 5
	norway_fence.add_point(4.790644, 61.195218);                        // norway_poligon point 6
	norway_fence.add_point(4.569032, 61.073245);                        // norway_poligon point 7
	norway_fence.add_point(4.527652, 60.961004);                        // norway_poligon point 8
	norway_fence.add_point(4.720628, 60.954154);                        // norway_poligon point 9
	norway_fence.add_point(4.655579, 60.838637);                        // norway_poligon point 10
	norway_fence.add_point(4.700814, 60.749922);                        // norway_poligon point 11
	norway_fence.add_point(4.750474, 60.671255);                        // norway_poligon point 12
	norway_fence.add_point(4.774379, 60.584226);                        // norway_poligon point 13
	norway_fence.add_point(4.861259, 60.448702);                        // norway_poligon point 14
	norway_fence.add_point(4.937524, 60.383045);                        // norway_poligon point 15
	norway_fence.add_point(4.991695, 60.271354);                        // norway_poligon point 16
	norway_fence.add_point(4.984457, 60.256030);                        // norway_poligon point 17
	norway_fence.add_point(5.030851, 60.128750);                        // norway_poligon point 18
	norway_fence.add_point(5.067020, 59.980669);                        // norway_poligon point 19
	norway_fence.add_point(5.021013, 59.883034);                        // norway_poligon point 20
	norway_fence.add_point(5.118655, 59.776819);                        // norway_poligon point 21
	norway_fence.add_point(5.132807, 59.672996);                        // norway_poligon point 22
	norway_fence.add_point(5.133118, 59.651368);                        // norway_poligon point 23
	norway_fence.add_point(5.138289, 59.640065);                        // norway_poligon point 24
	norway_fence.add_point(5.167042, 59.545345);                        // norway_poligon point 25
	norway_fence.add_point(5.129263, 59.353732);                        // norway_poligon point 26
	norway_fence.add_point(5.154681, 59.296859);                        // norway_poligon point 27
	norway_fence.add_point(5.158175, 59.171137);                        // norway_poligon point 28
	norway_fence.add_point(5.329142, 59.137872);                        // norway_poligon point 29
	norway_fence.add_point(5.412522, 59.153815);                        // norway_poligon point 30
	norway_fence.add_point(5.548792, 59.115114);                        // norway_poligon point 31
	norway_fence.add_point(5.561715, 59.109724);                        // norway_poligon point 32
	norway_fence.add_point(5.600668, 58.966420);                        // norway_poligon point 33
	norway_fence.add_point(5.554977, 58.856006);                        // norway_poligon point 34
	norway_fence.add_point(5.536524, 58.806186);                        // norway_poligon point 35
	norway_fence.add_point(5.505740, 58.745672);                        // norway_poligon point 36
	norway_fence.add_point(5.469468, 58.749669);                        // norway_poligon point 37
	norway_fence.add_point(5.531181, 58.708864);                        // norway_poligon point 38
	norway_fence.add_point(5.588036, 58.606392);                        // norway_poligon point 39
	norway_fence.add_point(5.668218, 58.565297);                        // norway_poligon point 40
	norway_fence.add_point(5.752646, 58.525617);                        // norway_poligon point 41
	norway_fence.add_point(5.823889, 58.507440);                        // norway_poligon point 42
	norway_fence.add_point(5.879396, 58.462840);                        // norway_poligon point 43
	norway_fence.add_point(5.976155, 58.401262);                        // norway_poligon point 44
	norway_fence.add_point(6.059828, 58.376086);                        // norway_poligon point 45
	norway_fence.add_point(6.128909, 58.352600);                        // norway_poligon point 46
	norway_fence.add_point(6.207975, 58.343184);                        // norway_poligon point 47
	norway_fence.add_point(6.324855, 58.313811);                        // norway_poligon point 48
	norway_fence.add_point(6.379969, 58.276456);                        // norway_poligon point 49
	norway_fence.add_point(6.516049, 58.242547);                        // norway_poligon point 50
	norway_fence.add_point(6.556323, 58.204937);                        // norway_poligon point 51
	norway_fence.add_point(6.610987, 58.167413);                        // norway_poligon point 52
	norway_fence.add_point(6.567728, 58.113152);                        // norway_poligon point 53
	norway_fence.add_point(6.686822, 58.075192);                        // norway_poligon point 54
	norway_fence.add_point(6.786177, 58.072308);                        // norway_poligon point 55
	norway_fence.add_point(6.835399, 58.069921);                        // norway_poligon point 56
	norway_fence.add_point(6.978466, 58.022697);                        // norway_poligon point 57
	norway_fence.add_point(7.051811, 57.980402);                        // norway_poligon point 58
	norway_fence.add_point(7.123131, 58.009732);                        // norway_poligon point 59
	norway_fence.add_point(7.210542, 58.034950);                        // norway_poligon point 60
	norway_fence.add_point(7.305035, 58.014771);                        // norway_poligon point 61
	norway_fence.add_point(7.430865, 58.007670);                        // norway_poligon point 62
	norway_fence.add_point(7.523906, 58.014478);                        // norway_poligon point 63
	norway_fence.add_point(7.650726, 57.979898);                        // norway_poligon point 64
	norway_fence.add_point(7.715431, 58.046374);                        // norway_poligon point 65
	norway_fence.add_point(7.819844, 58.068700);                        // norway_poligon point 66
	norway_fence.add_point(7.897681, 58.073377);                        // norway_poligon point 67
	norway_fence.add_point(7.961773, 58.101123);                        // norway_poligon point 68
	norway_fence.add_point(7.979648, 58.137186);                        // norway_poligon point 69
	norway_fence.add_point(8.101603, 58.143264);                        // norway_poligon point 70
	norway_fence.add_point(8.144254, 58.127626);                        // norway_poligon point 71
	norway_fence.add_point(8.294898, 58.183382);                        // norway_poligon point 72
	norway_fence.add_point(8.416094, 58.258503);                        // norway_poligon point 73
	norway_fence.add_point(8.530966, 58.292027);                        // norway_poligon point 74
	norway_fence.add_point(8.616865, 58.339233);                        // norway_poligon point 75
	norway_fence.add_point(8.689376, 58.367161);                        // norway_poligon point 76
	norway_fence.add_point(8.753112, 58.405920);                        // norway_poligon point 77
	norway_fence.add_point(8.820927, 58.438275);                        // norway_poligon point 78
	norway_fence.add_point(8.951983, 58.472807);                        // norway_poligon point 79
	norway_fence.add_point(8.966832, 58.480218);                        // norway_poligon point 80
	norway_fence.add_point(9.016735, 58.540027);                        // norway_poligon point 81
	norway_fence.add_point(9.093112, 58.580527);                        // norway_poligon point 82
	norway_fence.add_point(9.193288, 58.630856);                        // norway_poligon point 83
	norway_fence.add_point(9.251050, 58.683590);                        // norway_poligon point 84
	norway_fence.add_point(9.303202, 58.725626);                        // norway_poligon point 85
	norway_fence.add_point(9.381190, 58.781005);                        // norway_poligon point 86
	norway_fence.add_point(9.429792, 58.789045);                        // norway_poligon point 87
	norway_fence.add_point(9.525615, 58.809111);                        // norway_poligon point 88
	norway_fence.add_point(9.566561, 58.843152);                        // norway_poligon point 89
	norway_fence.add_point(9.618803, 58.877386);                        // norway_poligon point 90
	norway_fence.add_point(9.662352, 58.909088);                        // norway_poligon point 91
	norway_fence.add_point(9.693381, 58.957984);                        // norway_poligon point 92
	norway_fence.add_point(9.770758, 58.976874);                        // norway_poligon point 93
	norway_fence.add_point(9.847401, 58.961372);                        // norway_poligon point 94
	norway_fence.add_point(9.910914, 58.955486);                        // norway_poligon point 95
	norway_fence.add_point(9.916329, 58.958487);                        // norway_poligon point 96
	norway_fence.add_point(9.990279, 58.954753);                        // norway_poligon point 97
	norway_fence.add_point(10.068009, 58.975537);                       // norway_poligon point 98
	norway_fence.add_point(10.127691, 59.000813);                       // norway_poligon point 99
	norway_fence.add_point(10.171350, 59.010416);                       // norway_poligon point 100
	norway_fence.add_point(10.210870, 59.018824);                       // norway_poligon point 101
	norway_fence.add_point(10.241592, 59.035796);                       // norway_poligon point 102
	norway_fence.add_point(10.309203, 59.050682);                       // norway_poligon point 103
	norway_fence.add_point(10.335071, 59.106273);                       // norway_poligon point 104
	norway_fence.add_point(10.404885, 59.048515);                       // norway_poligon point 105
	norway_fence.add_point(10.449796, 59.054177);                       // norway_poligon point 106
	norway_fence.add_point(10.488369, 59.060808);                       // norway_poligon point 107
	norway_fence.add_point(10.614395, 58.894305);                       // norway_poligon point 108
	norway_fence.add_point(10.683794, 58.912309);                       // norway_poligon point 109
	norway_fence.add_point(10.806854, 58.921662);                       // norway_poligon point 110
	norway_fence.add_point(10.854243, 58.943068);                       // norway_poligon point 111
	norway_fence.add_point(10.949634, 58.969379);                       // norway_poligon point 112
	norway_fence.add_point(11.000758, 58.973785);                       // norway_poligon point 113
	norway_fence.add_point(11.091950, 59.004257);                       // norway_poligon point 114
	norway_fence.add_point(11.130161, 59.061403);                       // norway_poligon point 115
	norway_fence.add_point(11.172484, 59.085252);                       // norway_poligon point 116
	norway_fence.add_point(11.244554, 59.095157);                       // norway_poligon point 117
	norway_fence.add_point(11.337611, 59.105170);                       // norway_poligon point 118
	norway_fence.add_point(11.443474, 59.051120);                       // norway_poligon point 119
	norway_fence.add_point(11.467294, 58.968302);                       // norway_poligon point 120
	norway_fence.add_point(11.469650, 58.907351);                       // norway_poligon point 121
	norway_fence.add_point(11.570610, 58.893027);                       // norway_poligon point 122
	norway_fence.add_point(11.633465, 58.921955);                       // norway_poligon point 123
	norway_fence.add_point(11.722912, 59.017391);                       // norway_poligon point 124
	norway_fence.add_point(11.770496, 59.101306);                       // norway_poligon point 125
	norway_fence.add_point(11.782007, 59.175437);                       // norway_poligon point 126
	norway_fence.add_point(11.846335, 59.285030);                       // norway_poligon point 127
	norway_fence.add_point(11.784665, 59.410330);                       // norway_poligon point 128
	norway_fence.add_point(11.771776, 59.493287);                       // norway_poligon point 129
	norway_fence.add_point(11.691313, 59.605485);                       // norway_poligon point 130
	norway_fence.add_point(11.859040, 59.653035);                       // norway_poligon point 131
	norway_fence.add_point(11.921554, 59.686776);                       // norway_poligon point 132
	norway_fence.add_point(11.933656, 59.713319);                       // norway_poligon point 133
	norway_fence.add_point(11.902190, 59.845613);                       // norway_poligon point 134
	norway_fence.add_point(11.964989, 59.886998);                       // norway_poligon point 135
	norway_fence.add_point(11.969607, 59.886232);                       // norway_poligon point 136
	norway_fence.add_point(12.146484, 59.882618);                       // norway_poligon point 137
	norway_fence.add_point(12.345283, 59.973496);                       // norway_poligon point 138
	norway_fence.add_point(12.460187, 60.083498);                       // norway_poligon point 139
	norway_fence.add_point(12.551052, 60.202596);                       // norway_poligon point 140
	norway_fence.add_point(12.504143, 60.331922);                       // norway_poligon point 141
	norway_fence.add_point(12.558972, 60.366566);                       // norway_poligon point 142
	norway_fence.add_point(12.611012, 60.419725);                       // norway_poligon point 143
	norway_fence.add_point(12.587570, 60.552589);                       // norway_poligon point 144
	norway_fence.add_point(12.592258, 60.551804);                       // norway_poligon point 145
	norway_fence.add_point(12.572854, 60.637480);                       // norway_poligon point 146
	norway_fence.add_point(12.476022, 60.694767);                       // norway_poligon point 147
	norway_fence.add_point(12.420061, 60.788876);                       // norway_poligon point 148
	norway_fence.add_point(12.328929, 60.862923);                       // norway_poligon point 149
	norway_fence.add_point(12.302268, 60.923764);                       // norway_poligon point 150
	norway_fence.add_point(12.237477, 61.006143);                       // norway_poligon point 151
	norway_fence.add_point(12.331241, 61.042257);                       // norway_poligon point 152
	norway_fence.add_point(12.518779, 61.052724);                       // norway_poligon point 153
	norway_fence.add_point(12.585569, 61.057167);                       // norway_poligon point 154
	norway_fence.add_point(12.646317, 61.083137);                       // norway_poligon point 155
	norway_fence.add_point(12.722072, 61.145176);                       // norway_poligon point 156
	norway_fence.add_point(12.766326, 61.214992);                       // norway_poligon point 157
	norway_fence.add_point(12.769602, 61.219593);                       // norway_poligon point 158
	norway_fence.add_point(12.833526, 61.301601);                       // norway_poligon point 159
	norway_fence.add_point(12.872972, 61.349093);                       // norway_poligon point 160
	norway_fence.add_point(12.850190, 61.489167);                       // norway_poligon point 161
	norway_fence.add_point(12.840554, 61.490750);                       // norway_poligon point 162
	norway_fence.add_point(12.625361, 61.543868);                       // norway_poligon point 163
	norway_fence.add_point(12.530408, 61.569501);                       // norway_poligon point 164
	norway_fence.add_point(12.381956, 61.595917);                       // norway_poligon point 165
	norway_fence.add_point(12.246232, 61.663707);                       // norway_poligon point 166
	norway_fence.add_point(12.145564, 61.751400);                       // norway_poligon point 167
	norway_fence.add_point(12.158418, 61.823737);                       // norway_poligon point 168
	norway_fence.add_point(12.215412, 61.943060);                       // norway_poligon point 169
	norway_fence.add_point(12.264656, 62.066290);                       // norway_poligon point 170
	norway_fence.add_point(12.287856, 62.160314);                       // norway_poligon point 171
	norway_fence.add_point(12.292993, 62.244381);                       // norway_poligon point 172
	norway_fence.add_point(12.296310, 62.249008);                       // norway_poligon point 173
	norway_fence.add_point(12.294865, 62.339316);                       // norway_poligon point 174
	norway_fence.add_point(12.213517, 62.434368);                       // norway_poligon point 175
	norway_fence.add_point(12.156635, 62.533343);                       // norway_poligon point 176
	norway_fence.add_point(12.097778, 62.606848);                       // norway_poligon point 177
	norway_fence.add_point(12.119142, 62.699050);                       // norway_poligon point 178
	norway_fence.add_point(12.149039, 62.802996);                       // norway_poligon point 179
	norway_fence.add_point(12.082671, 62.890731);                       // norway_poligon point 180
	norway_fence.add_point(12.109368, 62.966944);                       // norway_poligon point 181
	norway_fence.add_point(12.204451, 62.988742);                       // norway_poligon point 182
	norway_fence.add_point(12.183890, 63.054148);                       // norway_poligon point 183
	norway_fence.add_point(12.096607, 63.114144);                       // norway_poligon point 184
	norway_fence.add_point(12.049737, 63.206982);                       // norway_poligon point 185
	norway_fence.add_point(11.964867, 63.279582);                       // norway_poligon point 186
	norway_fence.add_point(11.973423, 63.283502);                       // norway_poligon point 187
	norway_fence.add_point(12.048509, 63.327150);                       // norway_poligon point 188
	norway_fence.add_point(12.103947, 63.391173);                       // norway_poligon point 189
	norway_fence.add_point(12.161217, 63.438506);                       // norway_poligon point 190
	norway_fence.add_point(12.195031, 63.492371);                       // norway_poligon point 191
	norway_fence.add_point(12.175483, 63.570029);                       // norway_poligon point 192
	norway_fence.add_point(12.215568, 63.613092);                       // norway_poligon point 193
	norway_fence.add_point(12.429247, 63.731835);                       // norway_poligon point 194
	norway_fence.add_point(12.567951, 63.854332);                       // norway_poligon point 195
	norway_fence.add_point(12.670770, 63.929958);                       // norway_poligon point 196
	norway_fence.add_point(12.802638, 64.014078);                       // norway_poligon point 197
	norway_fence.add_point(12.904582, 64.050776);                       // norway_poligon point 198
	norway_fence.add_point(13.146586, 64.089213);                       // norway_poligon point 199
	norway_fence.add_point(13.240916, 64.097723);                       // norway_poligon point 200
	norway_fence.add_point(13.532792, 64.053666);                       // norway_poligon point 201
	norway_fence.add_point(13.706231, 64.043237);                       // norway_poligon point 202
	norway_fence.add_point(13.968894, 64.008874);                       // norway_poligon point 203
	norway_fence.add_point(14.153670, 64.177054);                       // norway_poligon point 204
	norway_fence.add_point(14.156739, 64.466485);                       // norway_poligon point 205
	norway_fence.add_point(14.056433, 64.478840);                       // norway_poligon point 206
	norway_fence.add_point(13.977337, 64.481414);                       // norway_poligon point 207
	norway_fence.add_point(13.856747, 64.522367);                       // norway_poligon point 208
	norway_fence.add_point(13.753065, 64.557374);                       // norway_poligon point 209
	norway_fence.add_point(13.659859, 64.587469);                       // norway_poligon point 210
	norway_fence.add_point(13.817513, 64.730085);                       // norway_poligon point 211
	norway_fence.add_point(13.843913, 64.726092);                       // norway_poligon point 212
	norway_fence.add_point(14.002183, 64.858698);                       // norway_poligon point 213
	norway_fence.add_point(14.203634, 65.013165);                       // norway_poligon point 214
	norway_fence.add_point(14.304929, 65.102940);                       // norway_poligon point 215
	norway_fence.add_point(14.352403, 65.216813);                       // norway_poligon point 216
	norway_fence.add_point(14.433919, 65.274423);                       // norway_poligon point 217
	norway_fence.add_point(14.503193, 65.317945);                       // norway_poligon point 218
	norway_fence.add_point(14.499593, 65.407663);                       // norway_poligon point 219
	norway_fence.add_point(14.553953, 65.536177);                       // norway_poligon point 220
	norway_fence.add_point(14.492549, 65.672698);                       // norway_poligon point 221
	norway_fence.add_point(14.606838, 65.772910);                       // norway_poligon point 222
	norway_fence.add_point(14.631459, 65.816827);                       // norway_poligon point 223
	norway_fence.add_point(14.572116, 66.117883);                       // norway_poligon point 224
	norway_fence.add_point(15.012678, 66.143213);                       // norway_poligon point 225
	norway_fence.add_point(15.468673, 66.273253);                       // norway_poligon point 226
	norway_fence.add_point(15.397963, 66.488162);                       // norway_poligon point 227
	norway_fence.add_point(15.643636, 66.599962);                       // norway_poligon point 228
	norway_fence.add_point(16.118775, 66.928510);                       // norway_poligon point 229
	norway_fence.add_point(16.424461, 67.050253);                       // norway_poligon point 230
	norway_fence.add_point(16.420687, 67.221206);                       // norway_poligon point 231
	norway_fence.add_point(16.108976, 67.433856);                       // norway_poligon point 232
	norway_fence.add_point(16.121929, 67.438286);                       // norway_poligon point 233
	norway_fence.add_point(16.182427, 67.496491);                       // norway_poligon point 234
	norway_fence.add_point(16.461608, 67.530405);                       // norway_poligon point 235
	norway_fence.add_point(16.825975, 67.932997);                       // norway_poligon point 236
	norway_fence.add_point(17.284554, 68.144245);                       // norway_poligon point 237
	norway_fence.add_point(17.912154, 67.971088);                       // norway_poligon point 238
	norway_fence.add_point(18.216244, 68.217135);                       // norway_poligon point 239
	norway_fence.add_point(18.186124, 68.512990);                       // norway_poligon point 240
	norway_fence.add_point(18.401921, 68.544184);                       // norway_poligon point 241
	norway_fence.add_point(18.629681, 68.503414);                       // norway_poligon point 242
	norway_fence.add_point(19.884283, 68.354068);                       // norway_poligon point 243
	norway_fence.add_point(20.245373, 68.472678);                       // norway_poligon point 244
	norway_fence.add_point(19.975336, 68.543406);                       // norway_poligon point 245
	norway_fence.add_point(20.247492, 68.695553);                       // norway_poligon point 246
	norway_fence.add_point(20.332251, 68.887807);                       // norway_poligon point 247
	norway_fence.add_point(20.112923, 69.002533);                       // norway_poligon point 248
	norway_fence.add_point(20.493580, 69.040129);                       // norway_poligon point 249
	norway_fence.add_point(20.694376, 69.113590);                       // norway_poligon point 250
	norway_fence.add_point(21.067216, 69.058601);                       // norway_poligon point 251
	norway_fence.add_point(21.028466, 69.220967);                       // norway_poligon point 252
	norway_fence.add_point(21.301379, 69.305813);                       // norway_poligon point 253
	norway_fence.add_point(21.586614, 69.268794);                       // norway_poligon point 254
	norway_fence.add_point(21.967429, 69.110896);                       // norway_poligon point 255
	norway_fence.add_point(22.224760, 68.943210);                       // norway_poligon point 256
	norway_fence.add_point(22.361180, 68.852298);                       // norway_poligon point 257
	norway_fence.add_point(22.392964, 68.722315);                       // norway_poligon point 258
	norway_fence.add_point(22.682179, 68.711974);                       // norway_poligon point 259
	norway_fence.add_point(23.117439, 68.644870);                       // norway_poligon point 260
	norway_fence.add_point(23.257866, 68.662410);                       // norway_poligon point 261
	norway_fence.add_point(23.706879, 68.712554);                       // norway_poligon point 262
	norway_fence.add_point(23.823626, 68.814794);                       // norway_poligon point 263
	norway_fence.add_point(24.255365, 68.780301);                       // norway_poligon point 264
	norway_fence.add_point(24.492136, 68.682750);                       // norway_poligon point 265
	norway_fence.add_point(24.938820, 68.565056);                       // norway_poligon point 266
	norway_fence.add_point(25.123027, 68.632183);                       // norway_poligon point 267
	norway_fence.add_point(25.151515, 68.735786);                       // norway_poligon point 268
	norway_fence.add_point(25.296754, 68.850297);                       // norway_poligon point 269
	norway_fence.add_point(25.680286, 68.896913);                       // norway_poligon point 270
	norway_fence.add_point(25.814971, 69.014127);                       // norway_poligon point 271
	norway_fence.add_point(25.752982, 69.109077);                       // norway_poligon point 272
	norway_fence.add_point(25.813439, 69.252929);                       // norway_poligon point 273
	norway_fence.add_point(25.860348, 69.346375);                       // norway_poligon point 274
	norway_fence.add_point(25.903040, 69.490719);                       // norway_poligon point 275
	norway_fence.add_point(25.986007, 69.580518);                       // norway_poligon point 276
	norway_fence.add_point(26.016736, 69.717026);                       // norway_poligon point 277
	norway_fence.add_point(26.305565, 69.821993);                       // norway_poligon point 278
	norway_fence.add_point(26.477019, 69.912153);                       // norway_poligon point 279
	norway_fence.add_point(26.693092, 69.950029);                       // norway_poligon point 280
	norway_fence.add_point(27.007924, 69.930938);                       // norway_poligon point 281
	norway_fence.add_point(27.432856, 69.993246);                       // norway_poligon point 282
	norway_fence.add_point(27.798718, 70.101125);                       // norway_poligon point 283
	norway_fence.add_point(27.950873, 70.017578);                       // norway_poligon point 284
	norway_fence.add_point(28.182252, 69.940671);                       // norway_poligon point 285
	norway_fence.add_point(28.484516, 69.827698);                       // norway_poligon point 286
	norway_fence.add_point(28.897990, 69.730057);                       // norway_poligon point 287
	norway_fence.add_point(29.170121, 69.676684);                       // norway_poligon point 288
	norway_fence.add_point(29.225580, 69.627867);                       // norway_poligon point 289
	norway_fence.add_point(29.313565, 69.489724);                       // norway_poligon point 290
	norway_fence.add_point(29.006723, 69.295033);                       // norway_poligon point 291
	norway_fence.add_point(28.871062, 69.222952);                       // norway_poligon point 292
	norway_fence.add_point(28.869140, 69.099782);                       // norway_poligon point 293
	norway_fence.add_point(29.074611, 69.014863);                       // norway_poligon point 294
	norway_fence.add_point(29.235108, 69.100344);                       // norway_poligon point 295
	norway_fence.add_point(29.298271, 69.225641);                       // norway_poligon point 296
	norway_fence.add_point(29.320338, 69.275888);                       // norway_poligon point 297
	norway_fence.add_point(29.476505, 69.333264);                       // norway_poligon point 298
	norway_fence.add_point(29.648871, 69.340898);                       // norway_poligon point 299
	norway_fence.add_point(29.828514, 69.418846);                       // norway_poligon point 300
	norway_fence.add_point(30.021135, 69.431248);                       // norway_poligon point 301
	norway_fence.add_point(30.160969, 69.525584);                       // norway_poligon point 302
	norway_fence.add_point(30.145490, 69.631444);                       // norway_poligon point 303
	norway_fence.add_point(30.148259, 69.680170);                       // norway_poligon point 304
	norway_fence.add_point(30.401273, 69.621985);                       // norway_poligon point 305
	norway_fence.add_point(30.656170, 69.522873);                       // norway_poligon point 306
	norway_fence.add_point(30.840345, 69.543454);                       // norway_poligon point 307
	norway_fence.add_point(30.917716, 69.613875);                       // norway_poligon point 308
	norway_fence.add_point(30.923176, 69.718860);                       // norway_poligon point 309
	norway_fence.add_point(30.862246, 69.787062);                       // norway_poligon point 310
	norway_fence.add_point(29.841944, 69.940697);                       // norway_poligon point 311
	norway_fence.add_point(29.833009, 69.936243);                       // norway_poligon point 312
	norway_fence.add_point(30.017965, 70.046964);                       // norway_poligon point 313
	norway_fence.add_point(29.990917, 70.033624);                       // norway_poligon point 314
	norway_fence.add_point(29.974913, 70.020967);                       // norway_poligon point 315
	norway_fence.add_point(30.433063, 70.103265);                       // norway_poligon point 316
	norway_fence.add_point(30.569130, 70.206411);                       // norway_poligon point 317
	norway_fence.add_point(31.136499, 70.265811);                       // norway_poligon point 318
	norway_fence.add_point(31.158146, 70.280576);                       // norway_poligon point 319
	norway_fence.add_point(31.173317, 70.296921);                       // norway_poligon point 320
	norway_fence.add_point(31.084427, 70.445279);                       // norway_poligon point 321
	norway_fence.add_point(30.502884, 70.583382);                       // norway_poligon point 322
	norway_fence.add_point(29.969739, 70.799286);                       // norway_poligon point 323
	norway_fence.add_point(29.626548, 70.778274);                       // norway_poligon point 324
	norway_fence.add_point(29.589800, 70.770125);                       // norway_poligon point 325
	norway_fence.add_point(28.941824, 70.959730);                       // norway_poligon point 326
	norway_fence.add_point(27.964641, 71.165330);                       // norway_poligon point 327
	norway_fence.add_point(27.075201, 71.164698);                       // norway_poligon point 328
	norway_fence.add_point(26.442500, 70.943293);                       // norway_poligon point 329
	norway_fence.add_point(25.353436, 70.529774);                       // norway_poligon point 330
	norway_fence.add_point(25.367024, 70.545407);                       // norway_poligon point 331
	norway_fence.add_point(25.483331, 70.650925);                       // norway_poligon point 332
	norway_fence.add_point(25.852591, 70.827352);                       // norway_poligon point 333
	norway_fence.add_point(26.167073, 71.015575);                       // norway_poligon point 334
	norway_fence.add_point(26.145479, 71.195747);                       // norway_poligon point 335
	norway_fence.add_point(25.630514, 71.201360);                       // norway_poligon point 336
	norway_fence.add_point(25.154194, 71.148127);                       // norway_poligon point 337
	norway_fence.add_point(24.750365, 71.117612);                       // norway_poligon point 338
	norway_fence.add_point(24.207971, 71.134335);                       // norway_poligon point 339
	norway_fence.add_point(23.730775, 70.957557);                       // norway_poligon point 340
	norway_fence.add_point(23.178610, 70.862025);                       // norway_poligon point 341
	norway_fence.add_point(22.365325, 70.757543);                       // norway_poligon point 342
	norway_fence.add_point(21.620006, 70.415506);                       // norway_poligon point 343
	norway_fence.add_point(21.125032, 70.318215);                       // norway_poligon point 344
	norway_fence.add_point(20.687261, 70.311408);                       // norway_poligon point 345
	norway_fence.add_point(20.094504, 70.349983);                       // norway_poligon point 346
	norway_fence.add_point(19.036200, 70.233094);                       // norway_poligon point 347
	norway_fence.add_point(18.540554, 69.998448);                       // norway_poligon point 348
	norway_fence.add_point(18.029715, 69.732089);                       // norway_poligon point 349
	norway_fence.add_point(17.618978, 69.608954);                       // norway_poligon point 350
	norway_fence.add_point(17.315196, 69.427385);                       // norway_poligon point 351
	norway_fence.add_point(16.904345, 69.409808);                       // norway_poligon point 352
	norway_fence.add_point(16.881836, 69.192164);                       // norway_poligon point 353
	norway_fence.add_point(16.753712, 69.070325);                       // norway_poligon point 354
	norway_fence.add_point(16.518281, 69.052836);                       // norway_poligon point 355
	norway_fence.add_point(16.160103, 68.946212);                       // norway_poligon point 356
	norway_fence.add_point(15.955862, 68.966010);                       // norway_poligon point 357
	norway_fence.add_point(15.872030, 69.019816);                       // norway_poligon point 358
	norway_fence.add_point(16.198144, 69.216190);                       // norway_poligon point 359
	norway_fence.add_point(16.253423, 69.297458);                       // norway_poligon point 360
	norway_fence.add_point(16.008555, 69.305654);                       // norway_poligon point 361
	norway_fence.add_point(15.793192, 69.165826);                       // norway_poligon point 362
	norway_fence.add_point(15.607253, 69.072632);                       // norway_poligon point 363
	norway_fence.add_point(15.491089, 68.955800);                       // norway_poligon point 364
	norway_fence.add_point(15.396792, 68.915787);                       // norway_poligon point 365
	norway_fence.add_point(15.282959, 68.946371);                       // norway_poligon point 366
	norway_fence.add_point(15.210033, 68.986431);                       // norway_poligon point 367
	norway_fence.add_point(15.146000, 69.025248);                       // norway_poligon point 368
	norway_fence.add_point(14.973236, 68.969198);                       // norway_poligon point 369
	norway_fence.add_point(14.876914, 68.895468);                       // norway_poligon point 370
	norway_fence.add_point(14.746207, 68.879016);                       // norway_poligon point 371
	norway_fence.add_point(14.484018, 68.782315);                       // norway_poligon point 372
	norway_fence.add_point(14.419346, 68.685981);                       // norway_poligon point 373
	norway_fence.add_point(14.500182, 68.622931);                       // norway_poligon point 374
	norway_fence.add_point(14.653536, 68.602542);                       // norway_poligon point 375
	norway_fence.add_point(14.665439, 68.522351);                       // norway_poligon point 376
	norway_fence.add_point(14.740179, 68.474894);                       // norway_poligon point 377
	norway_fence.add_point(14.523501, 68.440396);                       // norway_poligon point 378
	norway_fence.add_point(14.366230, 68.382953);                       // norway_poligon point 379
	norway_fence.add_point(14.061196, 68.348674);                       // norway_poligon point 380
	norway_fence.add_point(13.917599, 68.356258);                       // norway_poligon point 381
	norway_fence.add_point(13.583764, 68.213776);                       // norway_poligon point 382
	norway_fence.add_point(13.245849, 68.116229);                       // norway_poligon point 383
	norway_fence.add_point(13.094326, 68.079945);                       // norway_poligon point 384
	norway_fence.add_point(12.934646, 67.911799);                       // norway_poligon point 385
	norway_fence.add_point(12.774696, 67.696075);                       // norway_poligon point 386
	norway_fence.add_point(13.181463, 67.891614);                       // norway_poligon point 387
	norway_fence.add_point(13.564318, 68.042132);                       // norway_poligon point 388
	norway_fence.add_point(14.040884, 68.053942);                       // norway_poligon point 389
	norway_fence.add_point(14.086071, 68.070242);                       // norway_poligon point 390
	norway_fence.add_point(14.511293, 68.139847);                       // norway_poligon point 391
	norway_fence.add_point(15.121232, 68.209521);                       // norway_poligon point 392
	norway_fence.add_point(15.650472, 68.195232);                       // norway_poligon point 393
	norway_fence.add_point(15.212398, 68.050878);                       // norway_poligon point 394
	norway_fence.add_point(15.200349, 68.048825);                       // norway_poligon point 395
	norway_fence.add_point(15.157650, 68.036098);                       // norway_poligon point 396
	norway_fence.add_point(14.704085, 67.908643);                       // norway_poligon point 397
	norway_fence.add_point(14.673029, 67.886837);                       // norway_poligon point 398
	norway_fence.add_point(14.657109, 67.870403);                       // norway_poligon point 399
	norway_fence.add_point(14.510862, 67.749075);                       // norway_poligon point 400
	norway_fence.add_point(14.805903, 67.607732);                       // norway_poligon point 401
	norway_fence.add_point(14.542274, 67.402784);                       // norway_poligon point 402
	norway_fence.add_point(14.332777, 67.457782);                       // norway_poligon point 403
	norway_fence.add_point(13.826771, 67.411698);                       // norway_poligon point 404
	norway_fence.add_point(12.761525, 66.623069);                       // norway_poligon point 405
	norway_fence.add_point(12.225190, 66.213956);                       // norway_poligon point 406
	norway_fence.add_point(11.999565, 65.962924);                       // norway_poligon point 407
	norway_fence.add_point(11.902591, 65.768756);                       // norway_poligon point 408
	norway_fence.add_point(11.758691, 65.639474);                       // norway_poligon point 409
	norway_fence.add_point(11.902619, 65.513603);                       // norway_poligon point 410
	norway_fence.add_point(11.959388, 65.402420);                       // norway_poligon point 411
	norway_fence.add_point(11.464959, 65.046953);                       // norway_poligon point 412
	norway_fence.add_point(11.178284, 65.072380);                       // norway_poligon point 413
	norway_fence.add_point(10.804836, 65.074402);                       // norway_poligon point 414
	norway_fence.add_point(10.796822, 65.075389);                       // norway_poligon point 415
	norway_fence.add_point(10.560195, 64.974372);                       // norway_poligon point 416
	norway_fence.add_point(10.639948, 64.910774);                       // norway_poligon point 417
	norway_fence.add_point(10.620051, 64.901704);                       // norway_poligon point 418
	norway_fence.add_point(10.598336, 64.889976);                       // norway_poligon point 419
	norway_fence.add_point(10.596505, 64.887320);                       // norway_poligon point 420
	norway_fence.add_point(10.561317, 64.796554);                       // norway_poligon point 421
	norway_fence.add_point(10.939747, 64.754861);                       // norway_poligon point 422
	norway_fence.add_point(10.939589, 64.676891);                       // norway_poligon point 423
	norway_fence.add_point(10.592419, 64.475540);                       // norway_poligon point 424
	norway_fence.add_point(10.222802, 64.250899);                       // norway_poligon point 425
	norway_fence.add_point(10.012283, 64.037940);                       // norway_poligon point 426
	norway_fence.add_point(9.691604, 63.841259);                        // norway_poligon point 427
	norway_fence.add_point(9.206483, 63.663900);                        // norway_poligon point 428
	norway_fence.add_point(9.103018, 63.696403);                        // norway_poligon point 429
	norway_fence.add_point(8.811973, 63.828291);                        // norway_poligon point 430
	norway_fence.add_point(8.668576, 63.893581);                        // norway_poligon point 431
	norway_fence.add_point(8.279677, 63.679486);                        // norway_poligon point 432
	norway_fence.add_point(8.407621, 63.598987);                        // norway_poligon point 433
	norway_fence.add_point(8.240844, 63.497160);                        // norway_poligon point 434
	norway_fence.add_point(8.043488, 63.487649);                        // norway_poligon point 435
	norway_fence.add_point(7.954568, 63.500376);                        // norway_poligon point 436
	norway_fence.add_point(7.737934, 63.314340);                        // norway_poligon point 437
	norway_fence.add_point(7.710621, 63.205041);                        // norway_poligon point 438
	norway_fence.add_point(7.438542, 63.050100);                        // norway_poligon point 439
	norway_fence.add_point(7.061528, 63.015201);                        // norway_poligon point 440
	norway_fence.add_point(6.705932, 62.820967);                        // norway_poligon point 441
	norway_fence.add_point(6.331490, 62.719845);                        // norway_poligon point 442
	norway_fence.add_point(6.044335, 62.588639);                        // norway_poligon point 443
	norway_fence.add_point(5.656987, 62.343589);                        // norway_poligon point 444
	norway_fence.add_point(5.503549, 62.331723);                        // norway_poligon point 445
	norway_fence.add_point(5.270612, 62.280199);                        // norway_poligon point 446
	norway_fence.add_point(5.027184, 62.120765);                        // norway_poligon point 447
	norway_fence.add_point(4.879312, 61.951797);                        // norway_poligon point 448
	norway_fence.add_point(4.875594, 61.943295);                        // norway_poligon point 449
	norway_fence.add_point(4.717120, 61.789033);                        // norway_poligon point 450
	norway_fence.add_point(4.667328, 61.701617);                        // norway_poligon point 451
	norway_fence.add_point(4.659663, 61.594989);                        // norway_poligon point 452
	GPS_Coordinate test_point1_outside_norway(15.942879, 65.067013);    // test_point1_outside_norway
	GPS_Coordinate test_point2_outside_norway(4.671328, 56.694215);     // test_point2_outside_norway
	GPS_Coordinate test_point3_outside_norway(14.216710, 69.797403);    // test_point3_outside_norway
	GPS_Coordinate test_point4_inside_norway(23.850579, 69.621210);     // test_point4_inside_norway
	GPS_Coordinate test_point5_inside_norway(13.432925, 65.413087);     // test_point5_inside_norway
	GPS_Coordinate test_point6_inside_norway(8.358762, 60.468781);      // test_point6_inside_norway
	/* #endregion */

	// test the test points
	bool testPoint1 = norway_fence.is_inside(test_point1_outside_norway);
	bool testPoint2 = norway_fence.is_inside(test_point2_outside_norway);
	bool testPoint3 = norway_fence.is_inside(test_point3_outside_norway);
	bool testPoint4 = norway_fence.is_inside(test_point4_inside_norway);
	bool testPoint5 = norway_fence.is_inside(test_point5_inside_norway);
	bool testPoint6 = norway_fence.is_inside(test_point6_inside_norway);

	// check if the test points are correct
	printf("\ttestPoint1 is inside the geofence: %s, expected false\n", testPoint1 ? "true" : "false");
	printf("\ttestPoint2 is inside the geofence: %s, expected false\n", testPoint2 ? "true" : "false");
	printf("\ttestPoint3 is inside the geofence: %s, expected false\n", testPoint3 ? "true" : "false");
	printf("\ttestPoint4 is inside the geofence: %s, expected true\n", testPoint4 ? "true" : "false");
	printf("\ttestPoint5 is inside the geofence: %s, expected true\n", testPoint5 ? "true" : "false");
	printf("\ttestPoint6 is inside the geofence: %s, expected true\n", testPoint6 ? "true" : "false");

	// check if the test points are correct
	if (!testPoint1 && !testPoint2 && !testPoint3 && testPoint4 && testPoint5 && testPoint6)
	{
		printf("\test_geofence_norway_450points() passed.\n");
		return 1;
	}
	printf("\test_geofence_norway_450points() failed.\n");
	return 0;
}

#include "class_testing.h"
#include "geofence.h"

bool test_geofence()
{
#if defined(ESP32) || defined(ARDUINO)
	unsigned long start_time = millis();
#endif
	bool failed = false;
	failed = (!test_geofence_4points()) ? true : failed;
	failed = (!test_geofence_99points()) ? true : failed;
	failed = (!test_fence_distance()) ? true : failed;
	failed = (!test_geofence_norway_450points()) ? true : failed;

	if (failed)
	{
#if defined(ESP32) || defined(ARDUINO)
		printf("some tests failed, test duration %lu ms\n", millis() - start_time);
#else
		printf("some tests failed.\n");
#endif
		return 0;
	}
#if defined(ESP32) || defined(ARDUINO)
	printf("all tests passed, test duration %lu ms\n", millis() - start_time);
#else
	printf("all tests passed.\n");
#endif
	return 1;
}