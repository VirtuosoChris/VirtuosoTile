//  Copyright (c) 2013 Christopher Pugh. All rights reserved.

#ifndef VIRTUOSO_MESH_H
#define VIRTUOSO_MESH_H

#include "AttributeArray.h"
#include <Eigen/Geometry>
#include <algorithm>
#include <stdexcept>
namespace Virtuoso{
class Mesh
{

public:

    enum PrimitiveMode {TRIANGLES=0, TRIANGLE_STRIPS, TRIANGLE_FANS, LINES, LINE_LOOP, POLYLINE, POINTS};

    static unsigned int pointsperface(PrimitiveMode m);

    bool indexedmesh;
    int numFaces;
    int numVerts;

    std::vector<unsigned int> indexbuffer;

    std::vector<AttributeArray> attributes;
    PrimitiveMode mode;

    void read(std::istream& in);

    void write(std::ostream& out)const;


    AttributeArray& operator[] (const std::string& in) {

        std::vector<AttributeArray>::iterator it =
        std::find(attributes.begin(), attributes.end(),in);

        if(it == attributes.end()){
            throw std::runtime_error("attribute not present");///\todo throw name too
        }
        return *it;
    }


    void addAttribute(const std::string& att, unsigned int components){

        attributes.push_back(AttributeArray(att, components));

    }

    void removeAttribute(const std::string& att){

        std::vector<AttributeArray>::iterator it = std::find(attributes.begin(), attributes.end(),att);

        attributes.erase(it);

    }

    bool hasAttribute(const std::string& in)const{

        std::vector<AttributeArray>::const_iterator it =
        std::find(attributes.begin(), attributes.end(),in);

        return(it == attributes.end());
    }


    const AttributeArray& operator[] (const std::string& in)const{

        std::vector<AttributeArray>::const_iterator it =
            std::find(attributes.begin(), attributes.end(),in);

        if(it == attributes.end()){
            throw std::runtime_error("attribute not present");///\todo throw name too
        }

        return *it;

    }

    Mesh():indexedmesh(true), numFaces(0), numVerts(0), mode(TRIANGLES) {

    }

    Mesh(std::istream& in){
        read(in);
    }

    void end(){

        if(!attributes.size())return;

        numVerts = attributes[0].numVerts();

        for(unsigned int i = 1; i < attributes.size(); i++){

            if(attributes[i].numVerts() != numVerts){
                throw std::runtime_error("validation failed on end: Mesh has uneven number of verts in attribute arrays");
            }

            if(!attributes[i].dataValid()){
                throw std::runtime_error("validation failed on end: attribute data not along component boundaries");
            }
        }

    }


    void reserve(unsigned int vertsToReserve){
       for(unsigned int i = 0; i < attributes.size(); i++){
            attributes[i].reserve(vertsToReserve);
       }
    }

    void begin(unsigned int additionalVertices){///the additional vertices is a hint to the implementation.  it will function.
        reserve(additionalVertices + numVerts);
    }

    void beginIndices(unsigned int reserveIndices){

        indexbuffer.reserve(reserveIndices + indexbuffer.size() );
    }

    void insertIndices(unsigned int a, unsigned int b, unsigned int c){
        indexedmesh=true;
        indexbuffer.push_back(a);
        indexbuffer.push_back(b);
        indexbuffer.push_back(c);
        numFaces++;
    }

    void endIndices(){
        if(numFaces * 3 !=  indexbuffer.size()){
            throw std::runtime_error("Index buffer size does not match the number of indices indicated by the numfaces parameter");
        }
    }

};


}





#endif
