import java.util.List;
import java.util.Set;


public class GeodesicFileTransfer extends FileTransfer{
	
	public GeodesicFileTransfer(String inputFile){
		this.inputFile = inputFile;
	}

	/**
	 * Compute Geodesic distance along tree edges between tree nodes
	 * @param root
	 * @param parentNode
	 * @param distance
	 */
	@Override
	protected void DFS(TreeNode root, TreeNode parentNode, double distance) {
		if(parentNode==null){ // root node
			List<TreeNode> children = root.getChildren();
			for(TreeNode child: children){
				double dist = root.getVal().computeDistance(child.getVal());
				DFS(child,root, dist);
			}
		}else{
			if(root.childrenNum()==0){ // leaf node
				root.setParentNode(parentNode);
				root.setDistance(distance);
			}else if(root.childrenNum()==1){ // edge point
				List<TreeNode> children = root.getChildren();
				for(TreeNode child: children){
					double dist = root.getVal().computeDistance(child.getVal())+distance;
					DFS(child,parentNode, dist);
				}
			}else{ // tree node
				root.setParentNode(parentNode);
				root.setDistance(distance);
				List<TreeNode> children = root.getChildren();
				for(TreeNode child: children){
					double dist = root.getVal().computeDistance(child.getVal());
					DFS(child,root, dist);
				}
			}
		}
	}

}
