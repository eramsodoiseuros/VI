//
//  Scene.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#include "scene.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "primitive.hpp"
#include "mesh.hpp"
#include "Phong.hpp"
#include "AreaLight.hpp"

#include <iostream>
#include <set>
#include <vector>

using namespace tinyobj;

static void PrintInfo (const ObjReader myObj) {
    const tinyobj::attrib_t attrib = myObj.GetAttrib();
    const std::vector<tinyobj::shape_t> shapes = myObj.GetShapes();
    const std::vector<tinyobj::material_t> materials = myObj.GetMaterials();
    std::cout << "# of vertices  : " << (attrib.vertices.size() / 3) << std::endl;
    std::cout << "# of normals   : " << (attrib.normals.size() / 3) << std::endl;
    std::cout << "# of texcoords : " << (attrib.texcoords.size() / 2)
              << std::endl;

    std::cout << "# of shapes    : " << shapes.size() << std::endl;
    std::cout << "# of materials : " << materials.size() << std::endl;
    
    // Iterate shapes
    auto it_shape = shapes.begin();
    for ( ; it_shape != shapes.end() ; it_shape++) {
        // assume each face has 3 vertices
        // std::cout << "Processing shape " << it_shape->name << std::endl;
        // iterate faces
        // assume each face has 3 vertices
        auto it_vertex = it_shape->mesh.indices.begin();
        for ( ; it_vertex != it_shape->mesh.indices.end() ; ) {
            // process 3 vertices
            for (int v=0 ; v<3 ; v++) {
                // std::cout << it_vertex->vertex_index;
                it_vertex++;
            }
            // std::cout << std::endl;
        }
        // std::cout << std::endl;
        
        // printf("There are %lu material indexes\n", it_shape->mesh.material_ids.size());
    }
    
}

/*
 Use tiny load to load .obj scene descriptions
 https://github.com/tinyobjloader/tinyobjloader
 */

bool Scene::Load (const std::string &fname) {
    ObjReader myObjReader;

    if (!myObjReader.ParseFromFile(fname)) {
        std::cout << "Failed to load .obj file!";
        return false;
    }

    PrintInfo (myObjReader);

    const tinyobj::attrib_t attrib = myObjReader.GetAttrib();
    const std::vector<shape_t> shapes = myObjReader.GetShapes();
    const std::vector<material_t> materials = myObjReader.GetMaterials();

    for (auto mat = materials.begin(); mat != materials.end(); mat++){
        auto *m = new Phong();

        m->Ka = RGB(mat->ambient[0],mat->ambient[1], mat->ambient[2]);
        m->Kd = RGB(mat->diffuse[0],mat->diffuse[1], mat->diffuse[2]);
        m->Ks = RGB(mat->specular[0],mat->specular[1], mat->specular[2]);
        m->Kt = RGB(mat->transmittance[0],mat->transmittance[1], mat->transmittance[2]);
        m->Ns = mat->shininess;
        this->BRDFs.push_back(m);
        this->numBRDFs++;
    }

    // Load shapes
    for (auto shp = shapes.begin(); shp != shapes.end() ; shp++) {

        // for each shape create a primitive (Mesh + material_ndx)

        auto *prim = new Primitive();
        Mesh *mesh = new Mesh();
        prim->g = mesh;

        // iterate shapes
        for (auto ver = shp->mesh.indices.begin(); ver != shp->mesh.indices.end();) {

            // Create a new face for the mesh
            Face *face = new Face();

            face->bb.min = Point(0,0,0);
            face->bb.max = Point(0,0,0);

            // Loop over vertices in the face.
            for (size_t v = 0; v < 3; v++) {

                // access to vertex
                tinyobj::real_t vx = attrib.vertices[3*ver->vertex_index];
                tinyobj::real_t vy = attrib.vertices[3*ver->vertex_index+1];
                tinyobj::real_t vz = attrib.vertices[3*ver->vertex_index+2];

                Point ver_now = Point(vx,vy,vz);
                int index = mesh->get_index(ver_now);
                if(index != -1) face->vert_ndx[v] = index;
                else {
                    mesh->vertices.push_back(ver_now);
                    mesh->numVertices++;
                    face->vert_ndx[v] = mesh->numVertices-1;
                }
                face->bb.update(ver_now);
                mesh->bb.update(ver_now);

                ver++;
            }

            Point v0 = mesh->vertices[face->vert_ndx[0]];
            Point v1 = mesh->vertices[face->vert_ndx[1]];
            Point v2 = mesh->vertices[face->vert_ndx[2]];

            Vector edge1 = Vector(v1.X - v0.X,
                                  v1.Y - v0.Y,
                                  v1.Z - v0.Z);
            Vector edge2 = Vector(v2.X - v0.X,
                                  v2.Y - v0.Y,
                                  v2.Z - v0.Z);

            Vector normal = edge1.cross(edge2);
            normal.normalize();

            face->geoNormal = normal;

            mesh->faces.push_back(*face);
            mesh->numFaces++;
        }

        prim->material_ndx = shp->mesh.material_ids[0];
        prims.push_back(prim);
        numPrimitives++;
    }

    return true;
}

bool Scene::trace (Ray r, Intersection *isect) {
    Intersection curr_isect;
    bool intersection = false;

    if (numPrimitives==0) return false;

    // iterate over all primitives
    for (auto prim_itr = prims.begin() ; prim_itr != prims.end() ; prim_itr++) {
        if ((*prim_itr)->g->intersect(r, &curr_isect)) {
            if (!intersection) { // first intersection
                intersection = true;
                *isect = curr_isect;
                isect->f = BRDFs[(*prim_itr)->material_ndx];
            }
            else if (curr_isect.depth < isect->depth) {
                *isect = curr_isect;
                isect->f = BRDFs[(*prim_itr)->material_ndx];
            }
        }
    }

    isect->isLight = false; // download new intersection.hpp
    for (auto & light : lights) {
        if (light->type == AREA_LIGHT) {
            AreaLight *al = (AreaLight *)light;
            if (al->gem->intersect(r, &curr_isect)) {
                if (!intersection) { // first intersection
                    intersection = true;
                    *isect = curr_isect;
                    isect->isLight = true;
                    isect->Le = al->L();
                }
                else if (curr_isect.depth < isect->depth) {
                    *isect = curr_isect;
                    isect->isLight = true;
                    isect->Le = al->L();
                }
            }
        }
    }
    return intersection;
}

// checks whether a point on a light source (distance maxL) is visible
bool Scene::visibility (Ray s, const float maxL) {
    bool visible = true;
    Intersection curr_isect;

    if (numPrimitives==0) return true;

    // iterate over all primitives while visible
    for (auto prim_itr = prims.begin() ; prim_itr != prims.end() && visible ; prim_itr++) {
        if ((*prim_itr)->g->intersect(s, &curr_isect)) {
            if (curr_isect.depth < maxL) {
                visible = false;
            }
        }
    }
    return visible;
}

