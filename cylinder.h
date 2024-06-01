#ifndef CYLINDER_H
#define CYLINDER_H

#include "common.h"

#include "hittable.h"
#include "hittable_list.h"

class round_y : public hittable {
public:
    round_y(const point3 &center, double radius, shared_ptr<material> mat) : center(center), radius(radius), mat(mat) {
        normal = vec3(0, 1, 0);
    }
    bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
        if (fabs(r.direction().y()) < 1e-8) {
            return false;
        }
        auto t = (center.y() - r.origin().y()) / r.direction().y();
        if(!ray_t.surrounds(t))
            return false;
        
        auto intersection = r.at(t);
        vec3 planar_hitpt_vector = intersection - center;
        if (planar_hitpt_vector.length() > radius) {
            return false;
        }
        
        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.set_face_normal(r, normal);

        return true;
    }
private:
    point3 center;
    double radius;
    vec3 normal;
    shared_ptr<material> mat;
};

class cylinder : public hittable {
public:
    cylinder(const point3 &floor_center, double radius, double height, shared_ptr<material> mat) : floor_center(floor_center), radius(radius), height(height), mat(mat) {}
    bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
        auto a = r.direction().x() * r.direction().x() + r.direction().z() * r.direction().z();
        auto b = 2.0 * r.direction().x() * (r.origin().x() - floor_center.x()) + 2.0 * r.direction().z() * (r.origin().z() - floor_center.z());
        auto c = r.origin().x() * r.origin().x() + floor_center.x() * floor_center.x() - 2.0 * r.origin().x() * floor_center.x() + r.origin().z() * r.origin().z() + floor_center.z() * floor_center.z() - 2.0 * r.origin().z() * floor_center.z() - radius * radius;

        auto discriminant = b * b - 4 * a * c; 
        if (discriminant < 0) {
            return false;
        }
        
        auto sqrtd = sqrt(discriminant);

        auto root = (-b-sqrtd) / (2*a);
        if(!ray_t.surrounds(root) || !is_interior(r.at(root).y())) {
            root = (-b+sqrtd) / (2*a);
            if(!ray_t.surrounds(root) || !is_interior(r.at(root).y())) {
                return false;
            }
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - floor_center) * vec3({1, 0, 1});
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }

    bool is_interior(const double& hit_point_y) const {
        if (hit_point_y < floor_center.y() || (hit_point_y > floor_center.y() + height)) {
            return false;
        }
        return true;
    }
private:
    point3 floor_center;
    double radius;
    double height;
    shared_ptr<material> mat;
};

inline shared_ptr<hittable_list> make_cylinder(const point3 &floor_center, double radius, double height, shared_ptr<material> mat) {
    auto cylinder_ptr = make_shared<hittable_list>();
    cylinder_ptr->add(make_shared<cylinder>(floor_center, radius, height, mat));
    cylinder_ptr->add(make_shared<round_y>(floor_center, radius, mat));
    cylinder_ptr->add(make_shared<round_y>(floor_center + vec3(0, height, 0), radius, mat));
    return cylinder_ptr;
}
#endif