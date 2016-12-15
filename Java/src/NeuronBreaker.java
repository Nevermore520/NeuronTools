import java.io.File;
import java.io.FileNotFoundException;
import java.util.LinkedList;
import java.util.List;


public class NeuronBreaker {

	public static void main(String[] args) throws FileNotFoundException {
		// TODO Auto-generated method stub
		List<String> fileNames = new LinkedList<String>();
		final File folder = new File("data/Giorgio_1268/All_Cells/"); //need to change folder for new data
		FileFunctions.listFilesForFolder(folder,fileNames);
		String outputFolder = "data/giorgio_1268_dendrite/";
		FileFunctions.copyAndBreak(fileNames, outputFolder);
	}

}
