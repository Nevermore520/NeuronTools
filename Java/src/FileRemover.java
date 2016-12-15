import java.io.File;
import java.io.IOException;
import java.util.Arrays;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Set;


public class FileRemover {

	public static void main(String[] args) throws IOException {
		// TODO Auto-generated method stub
		List<String> fileNames = new LinkedList<String>();
		final File folder = new File("data/giorgio_1268_dendrite_3_4/all_cells/"); //need to change folder for new data
		FileFunctions.listFilesForFolder(folder,fileNames);
		Set<Integer> keepIds = new HashSet<Integer>(Arrays.asList(3,4));
		FileFunctions.deleteOtherFile(fileNames, keepIds);
	}

}
