#include "particle_utils.h"

double ps::generate_random_number_in_range(double begin, double end)
{
	double rand_num = (double)rand() / RAND_MAX;
	double random_range = begin + rand_num * (end - begin);
	return random_range;
}

//enum class ps::intersect_type
//{
//	disjoint, // no intersection (0)
//	intersect, // in unique point I0 (1)
//	overlap // in segment from I0 to I1 (2)
//};
//
//ps::intersect_type ps::line_line_intersection(ps::vector3d start1, ps::vector3d end1, ps::vector3d start2, ps::vector3d end2)
//{
//	vector3d u = end1 - start1;
//	vector3d v = end2 - start2;
//	vector3d w = start1 - start2;
//	double D = u[0]*v[1] - u[1]*v[0];
//
//	vector3d i0, i1;
//
//	 // test if  they are parallel (includes either being a point)
//	if (std::abs(D) < 0.00000001 ) 
//	{           
//		// S1 and S2 are parallel
//		if (u[0] * w[1] - u[1] * w[0] != 0 || v[0] * w[1] - v[1] * w[0] != 0)
//		{
//			return ps::intersect_type::disjoint; // they are NOT collinear
//		}
//		// they are collinear or degenerate
//		// check if they are degenerate  points
//		float du = u.norm_squared();
//		float dv = v.norm_squared();
//		if (du == 0 && dv == 0) 
//		{ // both segments are points
//			if (start1 != start2) // they are distinct  points
//			{
//				return ps::intersect_type::disjoint;
//			}
//			i0 = start1; // they are the same point
//			return ps::intersect_type::intersect;
//		}
//		if (du == 0) 
//		{                     // S1 is a single point
//			if (inSegment(start1, S2) == 0)  // but is not in S2
//			{
//				return ps::intersect_type::disjoint;
//			}
//			i0 = start1;
//			return ps::intersect_type::intersect;
//		}
//		if (dv == 0) 
//		{                     // S2 a single point
//			if (inSegment(start2, S1) == 0)  // but is not in S1
//			{
//				return ps::intersect_type::disjoint;
//			}
//			i0 = start2;
//			return ps::intersect_type::intersect;
//		}
//		//// they are collinear segments - get  overlap (or not)
//		//float t0, t1;                    // endpoints of S1 in eqn for S2
//		//Vector w2 = S1.P1 - S2.P0;
//		//if (v.x != 0) {
//		//	t0 = w.x / v.x;
//		//	t1 = w2.x / v.x;
//		//}
//		//else {
//		//	t0 = w.y / v.y;
//		//	t1 = w2.y / v.y;
//		//}
//		//if (t0 > t1) {                   // must have t0 smaller than t1
//		//	float t = t0; t0 = t1; t1 = t;    // swap if not
//		//}
//		//if (t0 > 1 || t1 < 0) {
//		//	return 0;      // NO overlap
//		//}
//		//t0 = t0<0 ? 0 : t0;               // clip to min 0
//		//t1 = t1>1 ? 1 : t1;               // clip to max 1
//		//if (t0 == t1) {                  // intersect is a point
//		//	*I0 = S2.P0 + t0 * v;
//		//	return 1;
//		//}
//	
//		//// they overlap in a valid subsegment
//		//*I0 = S2.P0 + t0 * v;
//		//*I1 = S2.P0 + t1 * v;
//		//return 2;
//	}
//	
//	//// the segments are skew and may intersect in a point
//	//// get the intersect parameter for S1
//	//float     sI = perp(v, w) / D;
//	//if (sI < 0 || sI > 1)                // no intersect with S1
//	//	return 0;
//	//
//	//// get the intersect parameter for S2
//	//float     tI = perp(u, w) / D;
//	//if (tI < 0 || tI > 1)                // no intersect with S2
//	//	return 0;
//	//
//	//*I0 = S1.P0 + sI * u;                // compute S1 intersect point
//	//return 1;
//}

//#define SMALL_NUM   0.00000001 // anything that avoids division overflow
//// dot product (3D) which allows vector operations in arguments
//#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)
//#define perp(u,v)  ((u).x * (v).y - (u).y * (v).x)  // perp product  (2D)
//
//
//
//// intersect2D_2Segments(): find the 2D intersection of 2 finite segments
////    Input:  two finite segments S1 and S2
////    Output: *I0 = intersect point (when it exists)
////            *I1 =  endpoint of intersect segment [I0,I1] (when it exists)
////    Return: 0=disjoint (no intersect)
////            1=intersect  in unique point I0
////            2=overlap  in segment from I0 to I1
//int
//intersect2D_2Segments(Segment S1, Segment S2, Point* I0, Point* I1)
//{
//	Vector    u = S1.P1 - S1.P0;
//	Vector    v = S2.P1 - S2.P0;
//	Vector    w = S1.P0 - S2.P0;
//	float     D = perp(u, v);
//
//	// test if  they are parallel (includes either being a point)
//	if (fabs(D) < SMALL_NUM) {           // S1 and S2 are parallel
//		if (perp(u, w) != 0 || perp(v, w) != 0) {
//			return 0;                    // they are NOT collinear
//		}
//		// they are collinear or degenerate
//		// check if they are degenerate  points
//		float du = dot(u, u);
//		float dv = dot(v, v);
//		if (du == 0 && dv == 0) {            // both segments are points
//			if (S1.P0 != S2.P0)         // they are distinct  points
//				return 0;
//			*I0 = S1.P0;                 // they are the same point
//			return 1;
//		}
//		if (du == 0) {                     // S1 is a single point
//			if (inSegment(S1.P0, S2) == 0)  // but is not in S2
//				return 0;
//			*I0 = S1.P0;
//			return 1;
//		}
//		if (dv == 0) {                     // S2 a single point
//			if (inSegment(S2.P0, S1) == 0)  // but is not in S1
//				return 0;
//			*I0 = S2.P0;
//			return 1;
//		}
//		// they are collinear segments - get  overlap (or not)
//		float t0, t1;                    // endpoints of S1 in eqn for S2
//		Vector w2 = S1.P1 - S2.P0;
//		if (v.x != 0) {
//			t0 = w.x / v.x;
//			t1 = w2.x / v.x;
//		}
//		else {
//			t0 = w.y / v.y;
//			t1 = w2.y / v.y;
//		}
//		if (t0 > t1) {                   // must have t0 smaller than t1
//			float t = t0; t0 = t1; t1 = t;    // swap if not
//		}
//		if (t0 > 1 || t1 < 0) {
//			return 0;      // NO overlap
//		}
//		t0 = t0<0 ? 0 : t0;               // clip to min 0
//		t1 = t1>1 ? 1 : t1;               // clip to max 1
//		if (t0 == t1) {                  // intersect is a point
//			*I0 = S2.P0 + t0 * v;
//			return 1;
//		}
//
//		// they overlap in a valid subsegment
//		*I0 = S2.P0 + t0 * v;
//		*I1 = S2.P0 + t1 * v;
//		return 2;
//	}
//
//	// the segments are skew and may intersect in a point
//	// get the intersect parameter for S1
//	float     sI = perp(v, w) / D;
//	if (sI < 0 || sI > 1)                // no intersect with S1
//		return 0;
//
//	// get the intersect parameter for S2
//	float     tI = perp(u, w) / D;
//	if (tI < 0 || tI > 1)                // no intersect with S2
//		return 0;
//
//	*I0 = S1.P0 + sI * u;                // compute S1 intersect point
//	return 1;
//}
////===================================================================
//
//
//
//// inSegment(): determine if a point is inside a segment
////    Input:  a point P, and a collinear segment S
////    Return: 1 = P is inside S
////            0 = P is  not inside S
//int
//inSegment(Point P, Segment S)
//{
//	if (S.P0.x != S.P1.x) {    // S is not  vertical
//		if (S.P0.x <= P.x && P.x <= S.P1.x)
//			return 1;
//		if (S.P0.x >= P.x && P.x >= S.P1.x)
//			return 1;
//	}
//	else {    // S is vertical, so test y  coordinate
//		if (S.P0.y <= P.y && P.y <= S.P1.y)
//			return 1;
//		if (S.P0.y >= P.y && P.y >= S.P1.y)
//			return 1;
//	}
//	return 0;
//}
