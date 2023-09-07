from xml.etree import ElementTree as ET

debug_placemarks = False # Set to True to print out the placemarks
print_cpp_program = True # Set to True to print out the C++ program using the sample class

def read_xml_from_file(file_path):
    with open(file_path, 'r', encoding='utf-8') as file:
        return ET.parse(file).getroot()

root = read_xml_from_file("google_earth.xml")
placemarks = []
namespaces = {'kml': 'http://www.opengis.net/kml/2.2'}

# Iterate over all "Placemark" elements
for placemark in root.findall(".//kml:Placemark", namespaces=namespaces):
    placemark_details = {}

    # Get the name of the placemark
    name = placemark.find("kml:name", namespaces=namespaces)
    if name is not None:
        placemark_details['name'] = name.text
    
    # Check if it's a point
    coordinates = placemark.find(".//kml:coordinates", namespaces=namespaces)
    if coordinates is not None:
        coords = coordinates.text.split(",")
        if len(coords) >= 2:
            # Format longitude and latitude to 6 decimal places
            placemark_details['longitude'] = "{:.6f}".format(float(coords[0]))
            placemark_details['latitude'] = "{:.6f}".format(float(coords[1]))
    
    # Check if it's a polygon
    polygon_coordinates = placemark.find(".//kml:Polygon//kml:coordinates", namespaces=namespaces)
    if polygon_coordinates is not None:
        # Initialize an empty list to store the polygon's coordinates
        polygon_coords = []
        for coord_set in polygon_coordinates.text.split():
            coords = coord_set.split(",")
            if len(coords) >= 2:
                # Format longitude and latitude to 6 decimal places
                longitude = "{:.6f}".format(float(coords[0]))
                latitude = "{:.6f}".format(float(coords[1]))
                polygon_coords.append((longitude, latitude))
        placemark_details['polygon_coordinates'] = polygon_coords
    
    # Add the placemark details to the list
    placemarks.append(placemark_details)

# uncomment this section if you want to see the output
#
if debug_placemarks:
    print('\nDEBUG PLACEMARKS\n')
    for placemark in placemarks:
        print(f"Placemark: {placemark.get('name', 'Unnamed')}")
        if 'longitude' in placemark and 'latitude' in placemark:
            print(f"  Point: ({placemark['longitude']}, {placemark['latitude']})")
        
        polygon_coords = placemark.get('polygon_coordinates')
        if polygon_coords:
            print("  Polygon Coordinates:")
            for longitude, latitude in polygon_coords:
                print(f"    ({longitude}, {latitude})")
            print(f"  Total Coordinates in Polygon: {len(polygon_coords)}")

if print_cpp_program:
    print('\nCPP PROGRAM OUTPUT\n')
    print('GeoFence geoFence;')
    for placemark in placemarks:
        polygon_coords = placemark.get('polygon_coordinates')
        if polygon_coords:
            counter = 0
            for longitude, latitude in polygon_coords:
                counter = counter + 1
                print("geoFence.addPoint(%0.6f, %0.6f); //%s point %s" % (float(longitude), float(latitude),placemark['name'],  str(counter)))
        
        elif placemark['name'].find("p") != -1 or placemark['name'].find("P") != -1:
            print("geoFence.addPoint(%0.6f, %0.6f); //marker %s" % (float(placemark['longitude']), float(placemark['latitude']), placemark['name']))
        
        #test point needs to create an object and then compare its inside or not
        elif placemark['name'].find("t") != -1 or placemark['name'].find("T") != -1:
            print("Point %s(%0.6f, %0.6f); //%s" % (placemark['name'], float(placemark['longitude']), float(placemark['latitude']), placemark['name']))      
            #print('geoFence.isInside(%s) ? printf("%s is inside the geofence.\\n") : printf("%s is outside the geofence.\\n");' % (placemark['name'], placemark['name'], placemark['name']))
    print('return 0;')