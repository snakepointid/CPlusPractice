//
//  vecComp.h
//  word2vecMac
//
//  Created by 佘昌略  on 2017/2/12.
//  Copyright © 2017年 佘昌略. All rights reserved.
//

#ifndef vecComp_h
#define vecComp_h
#include<cmath>
#include<vector>
using std::vector;
using Vector = vector<float>;
template<class Vector1,class Vector2>
inline float sDot(const Vector1 &x,const Vector2 &y)
{
    int m = x.size();const float *xp = x.data(),*yp = y.data();
    float sum=0.0;
    while (m-->0)
    {
        sum+=(*xp++)*(*yp++);
    }
    return sum;
}
inline void sSaxpy( Vector &x,float g,const Vector &y)
{
    int m = x.size();float *xp = x.data();const float *yp = y.data();
    while (m-->0)(*xp++)+=(*yp++)*g;  
}
inline void sUnit(Vector &x)
{
    float norm = ::sqrt(sDot(x,x));
    if(norm==0) return;
    int m = x.size();float *xp = x.data(); 
    while (m-->0)(*xp++)/=norm;

}
inline float sSum(const Vector &x)
{
    float sum=0.0;
    int m = x.size();const float *xp = x.data(); 
    while (m-->0)sum+=*xp++;
    return sum;

}
#endif /* vecComp_h */
