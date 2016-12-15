import java.util.List;


public class GeodesicIgnoreEdgePointsFileTransfer extends FileTransfer{
	// originally Euclidean
	public GeodesicIgnoreEdgePointsFileTransfer(String inputFile){
		this.inputFile = inputFile;
	}

	/**
	 * Compute the Giodesic distance ignore edge points between tree nodes
	 * @param root
	 * @param parentNode
	 * @param distance
	 */
	@Override
	protected void DFS(TreeNode root, TreeNode parentNode, double distance) {
		if(parentNode==null){ // root node
			List<TreeNode> children = root.getChildren();
			for(TreeNode child: children){
				DFS(child, root, distance);
			}
		}else{
			if(root.childrenNum()==0){ // leaf node
				root.setParentNode(parentNode);
				double dist = root.getVal().computeDistance(parentNode.getVal());
				root.setDistance(dist);
			}else if(root.childrenNum()==1){ // point along edges, not tree node
				List<TreeNode> children = root.getChildren();
				for(TreeNode child: children){
					DFS(child,parentNode, distance);
				}
			}else{ // tree node (with more than one child)
				root.setParentNode(parentNode);
				double dist = root.getVal().computeDistance(parentNode.getVal());
				root.setDistance(dist);
				List<TreeNode> children = root.getChildren();
				for(TreeNode child: children){
					DFS(child, root, distance);
				}
			}
		}
	}

}
