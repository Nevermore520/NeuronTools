
public class Pair<K extends Comparable<K>,E> implements Comparable<Pair<K, E>> {
		
	K key;
	E value;
	
	public Pair(K key, E value){
		this.key = key;
		this.value = value;
	}
	
	public K getKey(){
		return this.key;
	}
	public void setKey(K key){
		this.key = key;
	}
	public E getValue(){
		return this.value;
	}
	public void setValue(E value){
		this.value = value;
	}
	@Override
	public int compareTo(Pair<K, E> arg0) {
		// TODO Auto-generated method stub
		return this.key.compareTo(arg0.key);
	}
	
	
}
