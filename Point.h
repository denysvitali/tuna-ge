//
// Created by dvitali on 17.11.18.
//

#ifndef TUNA_GE_POINT_H
#define TUNA_GE_POINT_H
namespace tunage {
    class Point {
    public:
        Point(float x, float y, float z);
    private:
        float m_x;
        float m_y;
        float m_z;
    };
}
#endif //TUNA_GE_POINT_H
