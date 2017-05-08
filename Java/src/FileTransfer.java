import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Scanner;
import java.util.Set;


public abstract class FileTransfer {
	static final int STARTINDEX = 1;
	static int INDEX = 0;
	protected String inputFile;
	protected abstract void DFS(TreeNode root, TreeNode parentNode, double distance);
	
	public void printDensityCount(String outputFile) throws FileNotFoundException, UnsupportedEncodingException{
		List<Pair<Double, Integer>> list = GetDensityCount();
		PrintWriter pw = new PrintWriter(outputFile, "UTF-8");
		pw.println(list.size());
		for(Pair<Double, Integer> p: list){
			pw.println(p.getKey()+" "+p.getValue());
		}
		pw.close();
	}
	
	private List<Pair<Double, Integer>> GetDensityCount() throws FileNotFoundException{
		List<Pair<Double, Integer>> list = new ArrayList<Pair<Double, Integer>>();
		
		Map<Integer, TreeNode> treeMap = GetTreeMap();
		
		TreeNode root = treeMap.get(STARTINDEX);
		
		for(TreeNode node: treeMap.values()){
			node.setDistance(node.getVal().computeDistance(root.getVal()));
		}
		
		List<TreeNode> nodeList = new ArrayList<TreeNode>(treeMap.values());
		Collections.sort(nodeList, new Comparator<TreeNode>(){
			@Override
			public int compare(TreeNode n1, TreeNode n2) {
				// TODO Auto-generated method stub
				if(n1.getDistance()<n2.getDistance()) return -1;
				else if(n1.getDistance() == n2.getDistance()) return 0;
				else return 1;
			}
		});
		
		Set<TreeNode> visited = new HashSet<TreeNode>();
		list.add(new Pair<Double, Integer>(0.0, 0));
		for(TreeNode node: nodeList){
			Pair<Double, Integer> tail = list.get(list.size()-1);
			
			int outCount = 0;
			int inCount = 0;
			if(node.getParentNode()!=null && visited.contains(node.getParentNode())){
				inCount++;
			}else if(node.getParentNode()!=null){
				outCount++;
			}
			for(TreeNode child: node.getChildren()){
				if(!visited.contains(child)){
					outCount++;
				}else{
					inCount++;
				}
			}
			
			if(node.getDistance() == tail.getKey()){
				tail.setValue(tail.getValue()+outCount-inCount);
			}else{
				Pair<Double, Integer> newTail = new Pair<Double, Integer>(node.getDistance(),tail.getValue()+outCount-inCount);
				list.add(newTail);
			}
			visited.add(node);
		}
		return list;
	}
	
	/**
	 * 
	 * @return map of pair of (index, TreeNode)
	 * @throws FileNotFoundException
	 */
	private Map<Integer, TreeNode> GetTreeMap() throws FileNotFoundException{
		File fp = new File(inputFile);
		Scanner input = new Scanner(fp);
		int index, structID, parentIndex;
		float x, y, z, radius;
		Map<Integer, TreeNode> allNodes = new HashMap<Integer, TreeNode>();
		//jump to the first line of valid data
		while(!input.hasNextInt()){
			input.nextLine();
		}
		while(input.hasNext()){
			index = input.nextInt();
			structID = input.nextInt();
			x = input.nextFloat();
			y = input.nextFloat();
			z = input.nextFloat();
			radius = input.nextFloat();
			parentIndex = input.nextInt();
			
			Point3D p = new Point3D(x,y,z);
			TreeNode currentNode = new TreeNode(null, p);
			currentNode.setRadius(radius);
			if(parentIndex >= STARTINDEX && allNodes.containsKey(parentIndex)){
				TreeNode parent = allNodes.get(parentIndex);
				currentNode.setParentNode(parent);
				parent.addChild(currentNode);
			}
			allNodes.put(index, currentNode);
		}
		input.close();
		return allNodes;
	}
	private TreeNode GetTreeRoot() throws FileNotFoundException{
		return GetTreeMap().get(FileTransfer.STARTINDEX);
	}
	public void ChangeFileFormat(String outputFile) throws FileNotFoundException, UnsupportedEncodingException{
		
		TreeNode root = GetTreeRoot();
		DFS(root, null, 0);
		
		List<List<Float>> points = new ArrayList<List<Float>>();
		List<List<Integer>> connectivity = new ArrayList<List<Integer>>();
		List<Double> distance = new ArrayList<Double>();
		INDEX = 0;
		BuildData(points, connectivity, distance, root);
		
		PrintWriter writer = new PrintWriter(outputFile, "UTF-8");
		writer.println(points.size()+" "+connectivity.size());
		for(int i=0;i<points.size();i++){
			List<Float> point = points.get(i);
			writer.println(point.get(0)+" "+point.get(1)+" "+point.get(2));
		}
		for(int i=0;i<connectivity.size();i++){
			List<Integer> connect = connectivity.get(i);
			writer.println(connect.get(0)+" "+connect.get(1)+" "+distance.get(i));
		}
		writer.close();
	}
	
	// ChangeFileFormatWithFuncVal is used for computing function value as Euclidean distance to the root node.
	public void ChangeFileFormatWithFuncVal(String outputFile) throws FileNotFoundException, UnsupportedEncodingException{
		TreeNode root = GetTreeRoot();
		DFS(root, null, 0);
		List<List<Float>> points = new ArrayList<List<Float>>();
		List<List<Integer>> connectivity = new ArrayList<List<Integer>>();
		List<Double> distance = new ArrayList<Double>();
		distance.add(root.getDistance()); // The function value of root node is 0.
		INDEX = 0;
		BuildData(points, connectivity, distance, root);
		
		PrintWriter writer = new PrintWriter(outputFile, "UTF-8");
		writer.println(points.size()+" "+connectivity.size());
		for(int i=0;i<points.size();i++){
			List<Float> point = points.get(i);
			writer.println(point.get(0)+" "+point.get(1)+" "+point.get(2));
		}
		for(int i=0;i<connectivity.size();i++){
			List<Integer> connect = connectivity.get(i);
			writer.println(connect.get(0)+" "+connect.get(1));
		}
		for(int i=0;i<distance.size();i++){
			writer.println(distance.get(i));
		}
		writer.close();
	}
	
	private void BuildData(List<List<Float>> points, List<List<Integer>> connectivity, List<Double> distance, TreeNode root){
		if(root.getParentNode()==null){
			root.setIndex(INDEX);
			INDEX++;
			points.add(root.getVal().getPosition());
			List<TreeNode> children = root.getChildren();
			for(TreeNode child:children){
				BuildData(points, connectivity, distance, child);
			}
		}else{
			if(root.childrenNum()!=1){
				root.setIndex(INDEX);
				INDEX++;
				points.add(root.getVal().getPosition());
				List<Integer> connect = new ArrayList<Integer>();
				connect.add(root.getParentNode().getIndex());
				connect.add(root.getIndex());
				connectivity.add(connect);
				distance.add(root.getDistance());
				if(root.childrenNum()>1){
					List<TreeNode> children = root.getChildren();
					for(TreeNode child: children){
						BuildData(points, connectivity, distance, child);
					}
				}
			}else{
				List<TreeNode> children = root.getChildren();
				for(TreeNode child: children){
					BuildData(points, connectivity, distance, child);
				}
			}
		}
	}
}
