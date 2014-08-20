//  Copyright (c) 2013 Christopher Pugh. All rights reserved.



#include "GPUMesh.h"
#include <stdexcept>

namespace Virtuoso{

GLuint& GPUMesh::operator[](const std::string& in){
    std::vector<std::string>::iterator it =
    std::find(attributenames.begin(), attributenames.end(),in);

    if(it == attributenames.end()){
        throw std::runtime_error("attribute not present");
    }

    int index =  std::distance(attributenames.begin(), it);

    return vbos[index];
}

 
GPUMesh::~GPUMesh()
{

    if(copycount.isUnique()) {

        ///\todo test that they exist first
        glDeleteBuffers(vbos.size(), &vbos[0]);
        
 
        
        glDeleteBuffers(1, & indexbuffer);
    } 
}

    
    


void GPUMesh::initialize(const Mesh& m)
{


    vertexCount = m.numVerts;
    faceCount = m.numFaces;

 
    glGenBuffers(1, & indexbuffer);

    if(  !indexbuffer) {
        throw std::runtime_error("could not allocate VBO  ");
    }

    
 

    //set the postions buffer
    for(unsigned int i = 0; i < m.attributes.size(); i++) {

        GLuint nb = 0;


        glGenBuffers(1, &nb);

        if(!nb)throw std::runtime_error("Could not allocate attribute buffer");

        vbos.push_back(nb);
        attributenames.push_back(m.attributes[i].name);
 
        attributecomponents.push_back(m.attributes[i].components);
 
        
        glBindBuffer(GL_ARRAY_BUFFER, nb);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.attributes[i].components*m.numVerts, &(m.attributes[i].getArray()[0]), GL_STATIC_DRAW);
        glVertexAttribPointer(i, m.attributes[i].components, GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(i);

    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*3*m.numFaces, &(m.indexbuffer[0]), GL_STATIC_DRAW);

}

    
    
 
    
    void GPUMesh::push()
    {
        
        //set the postions buffer
        for(unsigned int i = 0; i < vbos.size(); i++) {
            
            
            glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
           
            glVertexAttribPointer(i, attributecomponents[i], GL_FLOAT, GL_FALSE, 0, 0);
            
            glEnableVertexAttribArray(i);
            
        }
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
 
        glDrawElements(GL_TRIANGLES, 3*faceCount, GL_UNSIGNED_INT, 0);
        
    }
    
    
    
}
