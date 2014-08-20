//  Copyright (c) 2013 Christopher Pugh. All rights reserved.

#ifndef VIRTUOSO_ATTRIBUTEARRAY_MESH_H
#define VIRTUOSO_ATTRIBUTEARRAY_MESH_H

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

#include <sstream>
#include <memory>

#include <Eigen/core>

#include <stdexcept>

#include <array>



class AttributeArray
{

    std::shared_ptr< std::vector<float> > arrayPtr;

    public:

    std::vector<float>& getArray() {return *arrayPtr;}
    const std::vector<float>& getArray()const{return *arrayPtr;}


    std::string name; //name of attribute array

    unsigned int components; //number of components per vertex

    AttributeArray(std::string n, int comps,int vert=0);

    AttributeArray(int vert, std::istream& i);

    void read(int vert,std::istream& in);

    void write(std::ostream& out)const;

    bool operator==(const std::string& str)const{
        return str == name;
    }

    unsigned int numVerts()const{
        return getArray().size() / components;
    }

    bool dataValid()const{
        return !(getArray().size() % components);
    }

    void reserve(unsigned int vertsToReserve){
        getArray().reserve(vertsToReserve * components);
    }

    template <int x>
    struct Inserter{

        std::shared_ptr<std::vector<float> > ptr;
        unsigned int components;


        void insertVec(const Eigen::Matrix<float, x,1>& in){

            unsigned int oldSize = ptr->size();

            ptr->resize( oldSize + components );

            Eigen::Map<Eigen::Matrix<float,  x,1> > map( & ((*ptr)[oldSize]) );

            map = in;


        }



        void vertex(const Eigen::Matrix<float,  x, 1>& in){
           insertVec(in);
        }

        Inserter(AttributeArray& arr)
        :ptr(arr.arrayPtr),
        components(arr.components)
        {

            if(components != x){
                throw std::runtime_error("MISMATCH IN ATTRIBUTE ARRAY AND ARG CT");
            }

        }


    };






};



#endif
