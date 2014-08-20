//  Copyright (c) 2013 Christopher Pugh. All rights reserved.

#include "Quad.h"

Virtuoso::Quad::Quad(){

        Mesh::addAttribute("position",3);

        Mesh::addAttribute("texcoord",2);

        Mesh::addAttribute("normal",3);

        AttributeArray::Inserter<3> pos((*this)["position"]);

        AttributeArray::Inserter<2> tc((*this)["texcoord"]);

        AttributeArray::Inserter<3> norm((*this)["normal"]);

        Mesh::begin(4);

            //lower left
            tc.vertex({0.0f, 0.0f});
            norm.vertex({0.0f,0.0f,1.0f});
            pos.vertex({-1.0f, -1.0f,0.0});

            //lower right
            tc.vertex({1.0f, 0.0f});    //tex coord
            norm.vertex({0.0f,0.0f,1.0f});	//normal
            pos.vertex({1.0f, -1.0f,0.0});//vert

            //upper right
            tc.vertex({1.0f, 1.0f});//tex coord
            norm.vertex({0.0f,0.0f,1.0f});	//normal
            pos.vertex({1.0f, 1.0f,0.0});//vert

            //upper left
            tc.vertex({0.0f, 1.0f});//tex coord
            norm.vertex({0.0f,0.0f,1.0f});//normal
            pos.vertex({-1.0f, 1.0f,0.0});//vert

        Mesh::end();


        Mesh::beginIndices(6);

            Mesh::insertIndices(0,1,2);
            Mesh::insertIndices(2,3,0);

        Mesh::endIndices();


}
