import java.io.File;
import java.io.FileNotFoundException;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;


public class FTMain {
	
	private static boolean checkParameter(String inputFolder, String outputFolder, int controller){
		if(inputFolder.length() == 0) return false;
		if(outputFolder.length() == 0) return false;
		if(controller < 0) return false;
		return true;
	}
	public static void main(String[] args) throws FileNotFoundException, UnsupportedEncodingException {
		if(args.length != 3){
			System.out.println("usage: file_transfer <original swc file folder> <output file folder> <file transfer function type>");
			System.out.println("<file transfer function type> is a number:");
			System.out.println("0: Geodesic distance along neuron tree edges from tree nodes to root");
			System.out.println("1: Euclidean distance along neuron tree edges from tree nodes to root");
			System.out.println("2: Weighted Geodesic distance along neuron tree edges from tree nodes to root");
			System.out.println("3: Euclidean distance from tree nodes to root");
			System.out.println("4: Density count at each tree node from root to farthest tree node to root");
			System.exit(0);
		}
		String inputFolder = args[0];
		String outputFolder = args[1];
		int controller = Integer.valueOf(args[2]);
		
		if(!checkParameter(inputFolder, outputFolder, controller)){
			System.out.println("error with argument");
			System.exit(0);
		}
		
		if(outputFolder.lastIndexOf("/") != outputFolder.length() - 1){
			outputFolder += "/"; // outputFolder should end with '/'
		}
		
		final File folder = new File(inputFolder); //need to change folder for new data
		List<String> fileNames = new ArrayList<String>();
		FileFunctions.listFilesForFolder(folder,fileNames);
		FileTransfer fileTransfer;
		if(controller == 0){ // Geodesic distance along neuron tree edges from tree nodes to root
			System.out.println("Geodesic distance along neuron tree edges from tree nodes to root");
			for(int i=0;i<fileNames.size();i++){
				String inputFile = fileNames.get(i);
				File next = new File(inputFile);
				String outputFile = outputFolder + (i + 1) + "_" + next.getName();
				fileTransfer = new GeodesicFileTransfer(inputFile);
				fileTransfer.ChangeFileFormat(outputFile);
			}
		}else if(controller == 1){ // Euclidean distance along neuron tree edges from tree nodes to root
			System.out.println("Euclidean distance along neuron tree edges from tree nodes to root");
			for(int i=0;i<fileNames.size();i++){
				String inputFile = fileNames.get(i);
				File next = new File(inputFile);
				String outputFile = outputFolder + (i + 1) + "_" + next.getName();
				fileTransfer = new GeodesicIgnoreEdgePointsFileTransfer(inputFile);
				fileTransfer.ChangeFileFormat(outputFile);
			}
		}else if(controller == 2){ // Weighted Geodesic distance along neuron tree edges from tree nodes to root
			System.out.println("Weighted Geodesic distance along neuron tree edges from tree nodes to root");
			for(int i=0;i<fileNames.size();i++){
				String inputFile = fileNames.get(i);
				File next = new File(inputFile);
				String outputFile = outputFolder + (i + 1) + "_" + next.getName();
				fileTransfer = new WeightedGeodesicFileTransfer(inputFile);
				fileTransfer.ChangeFileFormat(outputFile);
			}
		}else if(controller == 3){ // Euclidean distance from tree nodes to root
			System.out.println("Euclidean distance from tree nodes to root");
			for(int i=0;i<fileNames.size();i++){
				String inputFile = fileNames.get(i);
				File next = new File(inputFile);
				String outputFile = outputFolder + (i + 1) + "_" + next.getName();
				fileTransfer = new EuclideanFuncValFileTransfer(inputFile);
				fileTransfer.ChangeFileFormatWithFuncVal(outputFile);
			}
		}else if(controller == 4){ // Density count at each tree node from root to farthest tree node to root
			System.out.println("Density count at each tree node from root to farthest tree node to root");
			for(int i=0;i<fileNames.size();i++){
				String inputFile = fileNames.get(i);
				File next = new File(inputFile);
				String outputFile = outputFolder + (i + 1) + "_" + next.getName();
				fileTransfer = new GeodesicFileTransfer(inputFile);
				fileTransfer.printDensityCount(outputFile);
			}
		}else{
			System.out.println("<file transfer function type> should be in {0,1,2,3,4}");
			System.exit(0);
		}
//		for(int i=0;i<fileNames.size();i++){
//			//System.out.println("process file: "+fileNames.get(i));
//			String inputFile = fileNames.get(i);
//			//fileTransfer = new EuclideanFileTransfer(inputFile);
//			//fileTransfer = new WeightedGeodesicFileTransfer(inputFile);
//			fileTransfer = new GeodesicFileTransfer(inputFile);
//			File next = new File(inputFile);
//			if(controller == 0){
//				// perform file transfer function
//				//String outputFile = "data/output/modified_"+next.getName(); // need to change folder for new data
//				//String outputFile = "data/output/modified_geodesic_"+next.getName();
//				//String outputFile = "data/FourClassHippoBreakOutput/Geodesic/"+(i+1)+"_Geodesic_"+next.getName();
//				//String outputFile = "data/Zhao_trees/output_Euclidean/"+(i+1)+"_Euclidean_"+next.getName();	// i+1 is the index of this file
//				//String outputFile = "data/Zhao_trees/output_Euclidean_BigClass/five_big_class/"+(i+1)+"_Euclidean_"+next.getName();	// i+1 is the index of this file
//				String outputFile = "data/Giorgio_1268_dendrite_3_4/output_weighted_geodesic/" + (i + 1) + "_Euclidean_" + next.getName();
//				//String outputFile = "data/Giorgio_trees reclassed version/transfered_data/"+(i+1)+"_Euclidean_"+next.getName();	// i+1 is the index of this file
//				//fileTransfer = new GeodesicFileTransfer(inputFile);
//				fileTransfer.ChangeFileFormat(outputFile);
//			}else if(controller == 1){
//				// perform density count function
//				String densityOutputFile = "data/Giorgio_trees reclassed version/output_density/"+(i+1)+"_density_"+next.getName();
//				fileTransfer.printDensityCount(densityOutputFile);
//			}else if(controller == 2){
//				// perform file transfer function with function value defined as Euclidean distance to root node.
//				fileTransfer = new EuclideanFuncValFileTransfer(inputFile);
//				String outputFile = "data/FourClassHippoBreakOutput/output_euclidean_with_func_val/" + (i + 1) + "_Euclidean_with_func_val_" + next.getName();
//				fileTransfer.ChangeFileFormatWithFuncVal(outputFile);
//			}
//		}
	}
}
