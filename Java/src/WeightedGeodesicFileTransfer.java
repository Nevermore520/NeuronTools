import java.util.List;
import java.util.Set;


public class WeightedGeodesicFileTransfer extends FileTransfer{
	
	public WeightedGeodesicFileTransfer(String inputFile){
		this.inputFile = inputFile;
	}
	
	// Compute volume for frustum given its radius and height.
	private double frustumVolume(double bottomRadius, double topRadius, double height){
		return Math.PI * (bottomRadius * bottomRadius + bottomRadius * topRadius + topRadius * topRadius) * height / 3 / 10; // Divide 10 to avoid overflow
	}

	/**
	 * Compute weighted Geodesic distance along edges between tree nodes
	 * @param root
	 * @param parentNode
	 * @param distance
	 */
	@Override
	protected void DFS(TreeNode root, TreeNode parentNode, double distance) {
		if(parentNode==null){ // root node
			List<TreeNode> children = root.getChildren();
			for(TreeNode child: children){
				double height = root.getVal().computeDistance(child.getVal());
				DFS(child,root, frustumVolume(root.getRadius(), child.getRadius(), height));
			}
		}else{
			if(root.childrenNum()==0){ // leaf node
				root.setParentNode(parentNode);
				root.setDistance(distance);
			}else if(root.childrenNum()==1){ // edge point
				List<TreeNode> children = root.getChildren();
				for(TreeNode child: children){
					double height = root.getVal().computeDistance(child.getVal());
					DFS(child,parentNode, frustumVolume(root.getRadius(), child.getRadius(), height) + distance);
				}
			}else{ // tree node
				root.setParentNode(parentNode);
				root.setDistance(distance);
				List<TreeNode> children = root.getChildren();
				for(TreeNode child: children){
					double height = root.getVal().computeDistance(child.getVal());
					DFS(child,root, frustumVolume(root.getRadius(), child.getRadius(), height));
				}
			}
		}
	}

}
