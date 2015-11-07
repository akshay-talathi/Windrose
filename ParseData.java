import java.io.File;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import ucar.ma2.Array;
import ucar.ma2.Index;
import ucar.nc2.NetcdfFile;
import ucar.nc2.Variable;
import java.nio.file.Files;
import java.nio.file.Paths;
public class ParseData {

	
	//Get Float Attribute From File
	@SuppressWarnings("deprecation")
	public static double[] getDoubleAttribute(String fileName,String directory,String attribute) {	
		double[]data=new double[0];
		try {	


			directory=directory+"/obs/"+fileName.substring(0,4)+"/"+fileName.substring(4,6)+"-"+fileName.substring(6,8)+"/"+fileName;			
			NetcdfFile ncfile = null;
			ncfile = NetcdfFile.open(directory);
			Variable var = null;
			List<Variable> variableList = ncfile.getVariables();
			Iterator<Variable> varIterator = variableList.iterator();
			while (varIterator.hasNext()) 
			{	
				var = varIterator.next();
				if ((var.getName()).equalsIgnoreCase(attribute)) {
					Array arr = var.read();
					int[] shape = arr.getShape();
					Index index = arr.getIndex();
					data=new double[shape[0]];
					for (int i = 0; i < shape[0]; i++) 
						data[i]=arr.getDouble(index.set(i));
					break;
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return data;
	}

	public static void deleteFile(String fileName,String directory)throws Exception
	{

		directory=directory+"/obs/"+fileName.substring(0,4)+"/"+fileName.substring(4,6)+"-"+fileName.substring(6,8)+"/"+fileName.substring(0,fileName.length()-3);
		java.nio.file.Path path=Paths.get(directory);
		Files.delete(path);
	}



	@SuppressWarnings("deprecation")
	public static String[] getStringAttribute(String fileName,String directory,String attribute) {	
		String[]data=new String[0];
		try {	


			directory=directory+"/obs/"+fileName.substring(0,4)+"/"+fileName.substring(4,6)+"-"+fileName.substring(6,8)+"/"+fileName;			
			NetcdfFile ncfile = null;
			ncfile = NetcdfFile.open(directory);
			Variable var = null;
			List<Variable> variableList = ncfile.getVariables();
			Iterator<Variable> varIterator = variableList.iterator();
			while (varIterator.hasNext()) 
			{	
				var = varIterator.next();
				if ((var.getName()).equalsIgnoreCase(attribute)) 
				{
					Array arr = var.read();
					int[] shape = arr.getShape();
					Index index = arr.getIndex();
					data=new String[shape[0]];
					for ( int i=0; i<shape[0]; i++) 
					{
						StringBuffer stationIdString=new StringBuffer();
						for ( int j=0; j<shape[1]; j++) 
						{
							char dval = arr.getChar(index.set(i,j));
							stationIdString.append(dval+"");
						}
						data[i]=stationIdString.toString().replaceAll("\\u0000", "" ) ;
					}
					break;
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return data;
	}




	// Get Size OF Array
	@SuppressWarnings("deprecation")
	public static int getSize(String fileName,String directory,String attribute) {	
		float[]data=new float[0];
		try {				
			NetcdfFile ncfile = null;
			ncfile = NetcdfFile.open(getPath(fileName,directory));
			Variable var = null;
			List<Variable> variableList = ncfile.getVariables();
			Iterator<Variable> varIterator = variableList.iterator();
			while (varIterator.hasNext()) 
			{	
				var = varIterator.next();
				if ((var.getName()).equalsIgnoreCase(attribute)) {
					Array arr = var.read();
					return arr.getShape()[0];
				}
			}
		}catch(Exception e)
		{
			e.printStackTrace();
		}
		return 0;
	}

	//Get Float Attribute From File
	@SuppressWarnings("deprecation")
	public static float[] getFloatAttribute(String fileName,String directory,String attribute) {
		float[]data=new float[0];
		try {				
			NetcdfFile ncfile = null;
			ncfile = NetcdfFile.open(getPath(fileName,directory));
			Variable var = null;
			List<Variable> variableList = ncfile.getVariables();
			Iterator<Variable> varIterator = variableList.iterator();
			while (varIterator.hasNext()) 
			{	
				var = varIterator.next();
				if ((var.getName()).equalsIgnoreCase(attribute)) {
					Array arr = var.read();
					
	
					int[] shape = arr.getShape();
					Index index = arr.getIndex();
					data=new float[shape[0]];
					for (int i = 0; i < shape[0]; i++) 
						data[i]=arr.getFloat(index.set(i));
					break;
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return data;
	}
	



	public static String getPath(String fileName, String directory){
	
		ArrayList<File> files=listFiles(directory, new ArrayList<>());
		for(File file:files)
		{
			if(file.getName().endsWith(fileName))
				return file.getAbsolutePath();
		}
		return null;
	}
		
	//List Files in a Directory
	public static ArrayList<File> listFiles(String directoryName,ArrayList<File> files) {
		File directory = new File(directoryName);
		File[] fList = directory.listFiles();
		for (File file : fList) {
			if (file.isFile()) {
				files.add(file);
			} else if (file.isDirectory()) {
				files.addAll(listFiles(file.getAbsolutePath(), new ArrayList<File>()));
			}
		}
		return files;
	}

	
}
