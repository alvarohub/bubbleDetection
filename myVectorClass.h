// class vector2D (for the time being, only 2d):


#ifndef vector2D_H
#define vector2D_H

#ifndef DEG_TO_RAD
#define DEG_TO_RAD (PI/180.0)
#endif

#ifndef RAD_TO_DEG
#define RAD_TO_DEG (180.0/PI)
#endif

#ifndef CW
#define CW 1.0
#endif

#ifndef CCW
#define CCW -1.0
#endif

#ifndef PI
#define PI 3.14159265889
#endif

//#include "WProgram.h"
#include "ofMain.h"

class vector2D {

	public:
		
		// Overloaded constructor with parameters:
		vector2D( float _x=0.0f, float _y=0.0f );
		
		// Explicit setting:
		void set( float _x, float _y );
		void set( const vector2D& vec );
		
		// Comparison:
		bool operator==( const vector2D& vec );
		bool operator!=( const vector2D& vec );
		bool match( const vector2D& vec, float tollerance=0.0001 );
	
		// Overloaded operators:
		//
		void	operator=( const vector2D& vec );    // I cannot declare this if we want also operator chaining?
		//vector2D & operator=( const vector2D& vec ); // this is to enable operator chaining (vec1=vec2=vec3). 
		vector2D  operator+( const vector2D& vec ) const;
		vector2D& operator+=( const vector2D& vec );      // why it has an output? for doing vec1=vec2+=vec3? YES!!! (operator chaining). 
		vector2D  operator-( const vector2D& vec ) const;
		vector2D& operator-=( const vector2D& vec );
		vector2D  operator*( const vector2D& vec ) const;
		vector2D& operator*=( const vector2D& vec );
		vector2D  operator/( const vector2D& vec ) const;
		vector2D& operator/=( const vector2D& vec );
	
	//operator overloading for float:
    void 	  operator=( const float f);  // I cannot declare this if we want also operator chaining?
	//vector2D & operator=( const float& val ); // to allow operator chaining
    vector2D  operator+( const float f ) const;
    vector2D& operator+=( const float f );
    vector2D  operator-( const float f ) const;
    vector2D& operator-=( const float f );
    vector2D  operator-() const;
    vector2D  operator*( const float f ) const;
    vector2D& operator*=( const float f );
    vector2D  operator/( const float f ) const;
    vector2D& operator/=( const float f );	
	
	// Distance (between end points of two vector2Ds):
    float distance( const vector2D& pnt) const;
    float squareDistance( const vector2D& pnt ) const;
	
	// Length of vector2D (norm):
    float length() const;
    float squareLength() const; // faster, no sqrt
	
	// Scaling:
	vector2D  getScaled( const float length ) const;
	vector2D& scale( const float length );
	
	// Normalization:
	vector2D  getNormalized() const;
    vector2D& normalize();
	
	// Perpendicular normalized vector2D.
    vector2D  getPerpendicular(int orientation) const;
    vector2D& perpendicular(int orientation);
	
	// Rotation
    vector2D  getRotatedDeg( float angle ) const;
    vector2D  getRotatedRad( float angle ) const;
    vector2D& rotateDeg( float angle );
    vector2D& rotateRad( float angle );
	
	//vector2D product (for 3d vector2Ds - for 2d vector2Ds, something like this is just the "angle" between them):
	//vector2D getvector2DProduct(const vector2D& vec) const;
	//vector2D& vector2DProduct(const vector2D& vec) const;
	
	//Angle (deg) between two vector2Ds (using atan2, so between -180 and 180)
    float angleDeg( const vector2D& vec ) const;
    float angleRad( const vector2D& vec ) const;
	
	//Dot Product:
    float dot( const vector2D& vec ) const;

	
	// =================================================================
	
	// Actual variables:
	float x, y; // or make a class "point"
	
};

/////////////////
// Implementation
/////////////////


inline vector2D::vector2D( float _x, float _y ) {
	x = _x;
	y = _y;
}

inline void vector2D::set( float _x, float _y ) {
	x = _x;
	y = _y;
}

inline void vector2D::set( const vector2D& vec ) {
	x=vec.x;
	y=vec.y;
}

inline bool vector2D::operator==( const vector2D& vec ) {
	return (x == vec.x) && (y == vec.y);
}

inline bool vector2D::operator!=( const vector2D& vec ) {
	return (x != vec.x) || (y != vec.y);
}

inline bool vector2D::match( const vector2D& vec, float tollerance ) {
	return (abs(x - vec.x) < tollerance)
	&& (abs(y - vec.y) < tollerance);
}


/*
inline vector2D & operator=( const vector2D& vec ){ // returning a reference to the vector2D object for allowing operator chaining
	x = vec.x;
	y = vec.y;
	return *this;
}
 */


inline void vector2D::operator=( const vector2D& vec ){
	x = vec.x;
	y = vec.y;
}


inline vector2D vector2D::operator+( const vector2D& vec ) const {
	return vector2D( x+vec.x, y+vec.y);
}

inline vector2D& vector2D::operator+=( const vector2D& vec ) {
	x += vec.x;
	y += vec.y;
	return *this;
}

inline vector2D vector2D::operator-( const vector2D& vec ) const {  
	return vector2D(x-vec.x, y-vec.y);
}

inline vector2D& vector2D::operator-=( const vector2D& vec ) {
	x -= vec.x;
	y -= vec.y;
	return *this;
}

inline vector2D vector2D::operator*( const vector2D& vec ) const {
	return vector2D(x*vec.x, y*vec.y);
}

inline vector2D& vector2D::operator*=( const vector2D& vec ) {
	x*=vec.x;
	y*=vec.y;
	return *this;
}

inline vector2D vector2D::operator/( const vector2D& vec ) const {
	return vector2D( vec.x!=0 ? x/vec.x : x , vec.y!=0 ? y/vec.y : y);
}

inline vector2D& vector2D::operator/=( const vector2D& vec ) {
	vec.x!=0 ? x/=vec.x : x;
	vec.y!=0 ? y/=vec.y : y;
	return *this;
}

//operator overloading for float:
/*
inline vector2D & operator=( const float& val ){
	x = val;
	y = val;
	return *this;
}
 */

inline void vector2D::operator=( const float f){
	x = f;
	y = f;
}


inline vector2D vector2D::operator+( const float f ) const {
	return vector2D( x+f, y+f);
}

inline vector2D& vector2D::operator+=( const float f ) {
	x += f;
	y += f;
	return *this;
}

inline vector2D vector2D::operator-( const float f ) const {
	return vector2D( x-f, y-f);
}

inline vector2D& vector2D::operator-=( const float f ) {
	x -= f;
	y -= f;
	return *this;
}

inline vector2D vector2D::operator-() const {
	return vector2D(-x, -y);
}

inline vector2D vector2D::operator*( const float f ) const {
	return vector2D(x*f, y*f);
}

inline vector2D& vector2D::operator*=( const float f ) {
	x*=f;
	y*=f;
	return *this;
}

inline vector2D vector2D::operator/( const float f ) const {
	//cout << "here" << endl;
	if(f == 0) return vector2D(x, y);
	return vector2D(x/f, y/f);
}

inline vector2D& vector2D::operator/=( const float f ) {
	if(f == 0) return *this;
	x/=f;
	y/=f;
	return *this;
}


inline vector2D vector2D::getScaled( const float length ) const {
	float l = (float)sqrt(x*x + y*y);
	if( l > 0 )
		return vector2D( (x/l)*length, (y/l)*length );
	else
		return vector2D();
}


inline vector2D& vector2D::scale( const float length ) {
	float l = (float)sqrt(x*x + y*y);
	if (l > 0) {
		x = (x/l)*length;
		y = (y/l)*length;
	}
	return *this;
}

// Rotation
//
//

inline vector2D vector2D::getRotatedDeg( float angle ) const {
	float a = (float)(angle*DEG_TO_RAD);
	return vector2D( x*cos(a) - y*sin(a),
					x*sin(a) + y*cos(a) );
}

inline vector2D vector2D::getRotatedRad( float angle ) const {
	float a = angle;
	return vector2D( x*cos(a) - y*sin(a),
					x*sin(a) + y*cos(a) );
}

inline vector2D& vector2D::rotateDeg( float angle ) {
	float a = (float)(angle * DEG_TO_RAD);
	float xrot = x*cos(a) - y*sin(a);
	y = x*sin(a) + y*cos(a);
	x = xrot;
	return *this;
}

inline vector2D& vector2D::rotateRad( float angle ) {
	float a = angle;
	float xrot = x*cos(a) - y*sin(a);
	y = x*sin(a) + y*cos(a);
	x = xrot;
	return *this;
}

inline float vector2D::distance( const vector2D& pnt) const {
	float vx = x-pnt.x;
	float vy = y-pnt.y;
	return (float)sqrt(vx*vx + vy*vy);
}

inline float vector2D::squareDistance( const vector2D& pnt ) const {
	float vx = x-pnt.x;
	float vy = y-pnt.y;
	return vx*vx + vy*vy;
}

// Normalization:
inline vector2D vector2D::getNormalized() const {
	float length = (float)sqrt(x*x + y*y);
	if( length > 0 ) {
		return vector2D( x/length, y/length );
	} else {
		return vector2D();
	}
}

inline vector2D& vector2D::normalize() {
	float length = (float)sqrt(x*x + y*y);
	if( length > 0 ) {
		x /= length;
		y /= length;
	}
	return *this;
}

inline vector2D vector2D::getPerpendicular(int orientation) const {
	float length = (float)sqrt( x*x + y*y );
	if( length > 0 )
		return vector2D( -orientation*(y/length), orientation*x/length );
	else
		return vector2D();
}

inline vector2D& vector2D::perpendicular(int orientation) {
	float length = (float)sqrt( x*x + y*y );
	if( length > 0 ) {
		float _x = x;
		x = -(y/length)*orientation;
		y = _x/length*orientation;
	}
	return *this;
}

// Length (norm of vector2D):
inline float vector2D::length() const {
	return (float)sqrt( x*x + y*y );
}

inline float vector2D::squareLength() const {
	return (float)(x*x + y*y);
}

// Angle between two vector2Ds:
inline float vector2D::angleDeg( const vector2D& vec ) const {
	return (float)(atan2( x*vec.y-y*vec.x, x*vec.x + y*vec.y )*RAD_TO_DEG);
}

inline float vector2D::angleRad( const vector2D& vec ) const {
	return atan2( x*vec.y-y*vec.x, x*vec.x + y*vec.y );
}


#endif
