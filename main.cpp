#include "geofence.h"
#include "class_testing.h"

#if defined(_WIN32) || defined(__linux__)
int main()
{
	test_geofence();
	system("pause");
	return 1;
}
#endif

#if defined(ESP32) || defined(ARDUINO)
#warning "dont forget to add the test_geofence in your code"
#endif