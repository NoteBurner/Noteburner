#include <iostream>

#include <chrono>
using namespace std;

class Point {
	public:
		Point(float x = 0.0,float y = 0.0):_x(x),_y(y){}
		virtual float x(){return _x;}
		virtual float z(){return 0.0;}
		~Point(){}
	private:
		float _x,_y;
};

class Point3d :  public  Point {
	public:
		Point3d(float x = 0.0 , float y = 0.0, float z = 0.0):Point(x,y),_z(z){}

	protected:
		float _z;
};

class Vertex :  public virtual Point {
	public:
		Vertex(Vertex * next = 0 ):_next(next){}
		Vertex & operator=(const Vertex & v){
			Point::operator=(v);
			_next = v._next;
			return *this;
		}
	protected:
		Vertex *  _next;
};

// class Vertex3D :  public  Point3d,public Vertex {
// 	public:
// 		Vertex3D(){}
// 		Vertex3D & operator=(const Vertex3D & v){
// 			Point::operator=(v);
// 			Point3d::operator=(v);
// 			Vertex::operator=(v);
// 			return *this;
// 		}
	
// };


template <typename Ret,class Fx,class... Args>
Ret Time_Record(Fx func,Args... args){
	
	// 获取程序开始时间点
    auto start_time = std::chrono::high_resolution_clock::now();

    // 在这里放置你的代码，进行计算或处理
	Ret ret = 	func(args...);
    // 获取程序结束时间点
    auto end_time = std::chrono::high_resolution_clock::now();

    // 计算时间差
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    // 输出运行时间
    std::cout << "程序运行时间: " << duration << " 微秒" << std::endl;
	return ret;
}
template <typename Fx,class... Args>
void Time_Record(Fx func,Args... args){
	
	// 获取程序开始时间点
    auto start_time = std::chrono::high_resolution_clock::now();

    // 在这里放置你的代码，进行计算或处理
		func(args...);
    // 获取程序结束时间点
    auto end_time = std::chrono::high_resolution_clock::now();

    // 计算时间差
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    // 输出运行时间
    std::cout << "程序运行时间: " << duration << " 微秒" << std::endl;
	
}
class Y {
		public:
			Y(){}
			bool operator==(const Y&) const {return 1;}
	};
	class X {
		public:
			X(){}
			operator Y() const{ return Y();}
			X getValue(){return X();}
		
	};

void _main(){
	
	Point point;
	switch (int(point.x())){
		case -1:
			cout <<"-1"<< endl;
		return;
		case 0:
		cout <<"0"<< endl;
		return;
		case 1:
		cout <<"1"<< endl;
		return;
		default:
		cout <<"default"<< endl;
		return;
	}
	
}

Point v1;
int v2;

int main(int argc,char ** argv){
	Point knots[10] = {
		Point(),
		Point(1.0,1.0)
	};
	//Time_Record(_m1ain);
	//printf("address of v1 = %p\naddress of v2 = %p",&v1,&v2);
		
	return 0;
}
