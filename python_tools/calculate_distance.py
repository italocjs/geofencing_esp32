from math import radians, sin, cos, sqrt, atan2

def haversine_distance(point1, point2):
    # Radius of Earth in meters
    R = 6371000.0
    # Convert latitude and longitude from degrees to radians
    lat1, lon1 = radians(point1[0]), radians(point1[1])
    lat2, lon2 = radians(point2[0]), radians(point2[1])
    # Calculate change in coordinates
    dlat = lat2 - lat1
    dlon = lon2 - lon1
    # Haversine formula
    a = sin(dlat / 2)**2 + cos(lat1) * cos(lat2) * sin(dlon / 2)**2
    c = 2 * atan2(sqrt(a), sqrt(1 - a))
    # Calculate distance
    distance = R * c
    return distance

# Coordinates of the two points
point1 = (-23.207486, -45.907859)
point2 = (-23.211250, -45.906183)

# Calculate distance using haversine formula
distance_meters = haversine_distance(point1, point2)
distance_meters #result:  452.22873765131664


from geopy.distance import geodesic
# Coordinates of the two points
point1 = (-23.207486, -45.907859)
point2 = (-23.211250, -45.906183)

# Calculate the distance using geopy's geodesic function
distance = geodesic(point1, point2).meters
distance #result: 450.77551416860337