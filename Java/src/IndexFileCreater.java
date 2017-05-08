import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;


public class IndexFileCreater {

	public static void main(String[] args) throws FileNotFoundException, UnsupportedEncodingException {
		// TODO Auto-generated method stub
		List<String> fileNames = new LinkedList<String>();
		final File folder = new File("data/rec-data-ChCs_area_layer_axon/all_cells/");
		//new File("data/Zhao_trees/All_Cells");//new File("data/FourClassDataHippoBreak"); //need to change folder for new data
		FileFunctions.listFilesForFolder(folder,fileNames);
		String output = "data/indexFile.txt";
		//printIndexFile(fileNames,output);
		String outputFolder = "data/SWC/";
		//FileFunctions.deleteComment(fileNames,outputFolder);
		String outputAbsPath = "data/rec-data-ChCs_area_layer_axon/index_file/rec-data-ChCs_area_layer_axon_index_justin.txt";
		//TODO change function if using Justin's code
		//printIndexFileAbsolutePath(fileNames,outputAbsPath);
		String extension = ".ASC.swc";
		printIndexFileFitJustinPath(fileNames,outputAbsPath, extension);
	}
	
	private static void printIndexFile(List<String> fileNames,String output) throws FileNotFoundException, UnsupportedEncodingException{
		PrintWriter writer = new PrintWriter(output, "UTF-8");
		for(int i=0;i<fileNames.size();i++){
			File next = new File(fileNames.get(i));
			writer.println((i+1)+" "+next.getName());
		}
		writer.close();
	}
	
	private static void printIndexFileAbsolutePath(List<String> fileNames,String output) throws FileNotFoundException, UnsupportedEncodingException{
		PrintWriter writer = new PrintWriter(output, "UTF-8");
		for(int i=0;i<fileNames.size();i++){
			writer.println((i+1)+" "+fileNames.get(i));
		}
		writer.close();
	}
	
	private static String getLongestCommonPrefix(List<String> fileNames){
		if(fileNames.size() == 0) return "";
		String prefix = "";
		int index = 0;
		int minLength = fileNames.get(0).length();
		for(String name: fileNames){
			minLength = Math.min(minLength, name.length());
		}
		while(index < minLength){
			char nextChar = fileNames.get(0).charAt(index);
			boolean match = true;
			for(String names: fileNames){
				if(names.charAt(index) != nextChar){
					match = false;
					break;
				}
			}
			if(!match) break;
			prefix += nextChar;
			index++;
		}
		return prefix;
	}
	
	private static void printIndexFileFitJustinPath(List<String> fileNames,String output, String extension) throws FileNotFoundException, UnsupportedEncodingException{
		PrintWriter writer = new PrintWriter(output, "UTF-8");
		String flag = "Original_data";
		String commonPrefix = getLongestCommonPrefix(fileNames);
		System.out.println(commonPrefix);
		for(int i=0;i<fileNames.size();i++){
			String name = fileNames.get(i);
			// name = name.substring(name.indexOf(flag)+flag.length()+1);
			name = name.substring(commonPrefix.length());
			name = name.substring(0, name.indexOf(extension));
			String entry = (i+1)+" "+name;
			writer.println(entry);
		}
		writer.close();
	}

}
