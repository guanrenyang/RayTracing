#ifndef CONE_H
#define CONE_H

#include "common.h"

#include "hittable.h"
#include "hittable_list.h" 
#include "cylinder.h"

class cone : public hittable {
public:
    point3 vertex;    
    double height;
    double radius;
    shared_ptr<material> mat;

    cone(const point3& vertex, double height, double radius, shared_ptr<material> mat)
        : vertex(vertex), height(height), radius(radius), mat(mat) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        double tan_theta = radius / height;
        auto a = tan_theta * tan_theta * r.direction().y() * r.direction().y() - r.direction().x() * r.direction().x() - r.direction().z() * r.direction().z();
        auto b = 2.0 * (r.direction().y() * (r.origin().y() - vertex.y()) * tan_theta * tan_theta - r.direction().x() * (r.origin().x() - vertex.x()) - r.direction().z() * (r.origin().z() - vertex.z()));
        auto c = tan_theta * tan_theta * (r.origin().y() - vertex.y()) * (r.origin().y() - vertex.y()) - (r.origin().x() - vertex.x()) * (r.origin().x() - vertex.x()) - (r.origin().z() - vertex.z()) * (r.origin().z() - vertex.z());

        auto discriminant = b * b - 4 * a * c;
        if (discriminant < 0) {
            return false;
        }

        auto sqrtd = sqrt(discriminant);

        auto root = (-b - sqrtd) / (2 * a);
        if (!ray_t.surrounds(root) || !is_interior(r.at(root))) {
            root = (-b + sqrtd) / (2 * a);
            if (!ray_t.surrounds(root) || !is_interior(r.at(root))) {
                return false;
            }
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = rec.p - (vertex - vec3(0, (rec.p - vertex).length() * (height * height + radius *radius) / height, 0));
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }

    bool is_interior(const point3 &hit_point) const {
        if ((hit_point.y() < vertex.y() - height) || hit_point.y() > vertex.y()) {
            return false;
        }
        return true;
    }
};
 
inline shared_ptr<hittable_list> make_cone(const point3 &vertex, double height, double radius, shared_ptr<material> mat) {
    auto cone_list = make_shared<hittable_list>();
    auto cone_ptr = make_shared<cone>(vertex, height, radius, mat);
    auto round_ptr = make_shared<round_y>(vertex - vec3(0, height, 0), radius, mat);
    cone_list->add(cone_ptr);
    cone_list->add(round_ptr);
    return cone_list;
}
#endif