from xml.etree import ElementTree as ET

# Function to read XML from file
def read_xml_from_file(file_path):
    with open(file_path, 'r', encoding='utf-8') as file:
        return ET.parse(file).getroot()

# Read XML data from file
root = read_xml_from_file("google_earth.xml")

# Initialize an empty list to store the placemarks
placemarks = []

# XML namespaces
namespaces = {'kml': 'http://www.opengis.net/kml/2.2'}

# Iterate over all "Placemark" elements
for placemark in root.findall(".//kml:Placemark", namespaces=namespaces):
    # Initialize an empty dictionary to store the placemark details
    placemark_details = {}
    
    # Get the name of the placemark
    name = placemark.find("kml:name", namespaces=namespaces)
    if name is not None:
        placemark_details['name'] = name.text
    
    # Get the latitude and longitude of the placemark
    coordinates = placemark.find(".//kml:coordinates", namespaces=namespaces)
    if coordinates is not None:
        coords = coordinates.text.split(",")
        if len(coords) >= 2:
            placemark_details['longitude'] = coords[0]
            placemark_details['latitude'] = coords[1]
    
    # Add the placemark details to the list
    placemarks.append(placemark_details)

print('GeoFence geoFence;')
for placemark in placemarks:
    #it the placemark contains p or P, it is an point that should be added to the geofence
    if placemark['name'].find("p") != -1 or placemark['name'].find("P") != -1:
        print("geoFence.addPoint(%0.6f, %0.6f); //%s" % (float(placemark['longitude']), float(placemark['latitude']), placemark['name']))
    
    #it the placemark contains t or T, it is an point that should used to test if it is inside or outside the geofence
    if placemark['name'].find("t") != -1 or placemark['name'].find("T") != -1:
        print("Point %s(%0.6f, %0.6f); //%s" % (placemark['name'], float(placemark['longitude']), float(placemark['latitude']), placemark['name']))      
        print('geoFence.isInside(%s) ? printf("%s is inside the geofence.\\n") : printf("%s is outside the geofence.\\n");' % (placemark['name'], placemark['name'], placemark['name']))

    #implement the function for an polygon here
print('return 0;')
