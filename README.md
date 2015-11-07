# Windrose

### Application
1. Created Climatological Windrose Generation as a CMPE 275 class project.
2. Used Java Native Interface (JNI) framework to enable Java code running in a JVM to call and be called by C++ code
3. JNI helped in interfacing Java and C++ code.
4. Parsed nedcdf file data using Java JNI.
5. Divided the retrived data points into bins based on the wind speed, direction and station ID.
6. Presented the output in 6*16 matrix.

### Development
1. To get attributes of the netcdf file, we created getFloatAttribute method in Java.
2. Called getFloatAttribute() method from C++ code GetStaticMethodID() method to initialize the class and to find method in Java class.
3. getFloatData function calls getFloatAttribute method and retrieves float data. 
4. getSize function calls getSize method in Java to retrieve size of the netcdf file.
5. getStringData function calls getStringAttribute method and retrieves String data.
6. The output of this step generates three arrays; one for wind speed, wind direction and station ID.
7. The calcBins function performs processing of wind speed, wind direction and station ID data and segments data points into bins.

### Running application
Please find commnad files in the repository directory.
