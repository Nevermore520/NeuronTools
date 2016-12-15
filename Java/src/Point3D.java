import java.util.LinkedList;
import java.util.List;


public class Point3D {
	float x;
	float y;
	float z;
	public Point3D(float x, float y, float z){
		this.x = x;
		this.y = y;
		this.z = z;
	}
	public double computeDistance(Point3D p){
		return Math.sqrt((this.x-p.x)*(this.x-p.x)+(this.y-p.y)*(this.y-p.y)+(this.z-p.z)*(this.z-p.z));
	}
	public List<Float> getPosition(){
		List<Float> position = new LinkedList<Float>();
		position.add(x);
		position.add(y);
		position.add(z);
		return position;
	}
}
