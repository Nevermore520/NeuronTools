import java.util.ArrayList;
import java.util.List;


public class TreeNode{

	private TreeNode parentNode;
	private Point3D val;
	private List<TreeNode> children;
	private double distance;
	private int index;
	private double radius;
	
	public TreeNode(TreeNode parent, Point3D val){
		this.parentNode = parent;
		//this.parentPoint = parent;
		this.val = val;
		this.children = new ArrayList<TreeNode>();
		this.distance = 0;
		index = 0;
	}
	public TreeNode getParentNode(){
		return this.parentNode;
	}
	public void setParentNode(TreeNode parent){
		this.parentNode = parent;
	}
	
	public Point3D getVal(){
		return this.val;
	}
	
	public int childrenNum(){
		return this.children.size();
	}
	public void addChild(TreeNode child){
		this.children.add(child);
	}
	public List<TreeNode> getChildren(){
		return this.children;
	}
	
	public double getDistance(){
		return this.distance;
	}
	public void setDistance(double distance){
		this.distance = distance;
	}
	public int getIndex(){
		return this.index;
	}
	public void setIndex(int index){
		this.index = index;
	}
	public double getRadius() {
		return radius;
	}
	public void setRadius(double radius) {
		this.radius = radius;
	}
}
