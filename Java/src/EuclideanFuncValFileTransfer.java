
public class EuclideanFuncValFileTransfer extends FileTransfer{
	private TreeNode currentRoot;
	public EuclideanFuncValFileTransfer(String inputFile){
		this.inputFile = inputFile;
		currentRoot = null;
	}

	/**
	 * Compute the Euclidean distance between tree nodes
	 * @param root
	 * @param parentNode
	 */
	@Override
	protected void DFS(TreeNode root, TreeNode parentNode, double distance) {
		// TODO Auto-generated method stub
		if(parentNode==null){
			root.setDistance(0);
			this.currentRoot = root;
			for(TreeNode child: root.getChildren()){
				DFS(child, root, distance);
			}
		}else{
			if(root.childrenNum()==0){
				root.setParentNode(parentNode);
				root.setDistance(root.getVal().computeDistance(this.currentRoot.getVal()));
			}else if(root.childrenNum()==1){
				for(TreeNode child: root.getChildren()){
					DFS(child,parentNode, distance);
				}
			}else{
				root.setParentNode(parentNode);
				root.setDistance( root.getVal().computeDistance(this.currentRoot.getVal()));
				for(TreeNode child: root.getChildren()){
					DFS(child, root, distance);
				}
			}
		}
	}

}
