/* Programm in Modern C++ as I can */
#include <fstream>
#include <regex>

#include "common.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "quad.h"
#include "cone.h"
#include "cylinder.h"

void cornell_box() {
    hittable_list world;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto metal_yello  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light));
    world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    // world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light));
    world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    world.add(box(point3(130, 0, 65), point3(295, 165, 230), white));
    world.add(box(point3(265, 0, 295), point3(430, 330, 460), metal_yello));
    // initialize a cylinder above the box in the front
    world.add(make_cylinder(point3(378, 50, 0), 50, 100, green));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 600;
    cam.samples_per_pixel = 500;
    cam.max_depth         = 50;
    cam.background        = color(0.7, 0.8, 1.0);

    cam.vfov     = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

int main() {
    // Output redirection
    const std::ofstream out("image.ppm");
    std::streambuf* coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());
    //
    // hittable_list world;
    //
    // auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    // auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    // auto material_left   = make_shared<dielectric>(1.50);
    // auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
    // auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);
    //
    // world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    // // world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.2),   0.5, material_center));
    // world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    // world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.4, material_bubble));
    // world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));
    // world.add(make_cylinder(point3(2.5, -0.5, -1.0), 0.5, 1, material_right));     
    // world.add(make_cone(point3(0.0, 0.0, -0.8), 0.5, 0.5, material_right));
    // camera cam;
    //
    // cam.aspect_ratio      = 16.0 / 9.0;
    // cam.image_width       = 400;
    // cam.samples_per_pixel = 100;
    // cam.max_depth         = 50;
    //
    //
    // cam.vfov     = 90;
    // cam.lookfrom = point3(0,0,1);
    // cam.lookat   = point3(0,0,-1);
    // cam.vup      = vec3(0,1,0);
    //
    // cam.render(world);
    cornell_box();
}