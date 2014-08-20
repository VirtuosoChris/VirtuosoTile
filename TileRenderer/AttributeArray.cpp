//  Copyright (c) 2013 Christopher Pugh. All rights reserved.

#include "AttributeArray.h"

//constructor to construct an attribute array.
AttributeArray::AttributeArray(std::string n, int comps,int vert)
:
arrayPtr(new std::vector<float>()),
name(n),
components(comps)
{
    (*arrayPtr).reserve(comps*vert);
}
AttributeArray::AttributeArray( int vert, std::istream& i)
:
arrayPtr(new std::vector<float>()),
name(""),
components(0)
{
    read(vert,i);
}

void AttributeArray::read(int vertices,std::istream& in)
{

     std::vector<float>& array = *arrayPtr;

    unsigned int namelength;

    in.read((char*) &namelength, sizeof(unsigned int));

    std::vector<char> c_str(namelength,' ');

    in.read((char*)&(c_str[0]), sizeof(char)*namelength);

    name = std::string(&c_str[0]);

    in.read((char*)&components, sizeof(unsigned int));

    array.resize(components*vertices);

    in.read((char*) &array[0], vertices*components*sizeof(float));


}


void AttributeArray::write(std::ostream& out)const
{

    std::vector<float>& array = *arrayPtr;

    unsigned int namelength = name.size()+1;
    out.write((char*)&namelength, sizeof(unsigned int));
    out.write((const char*)name.c_str(), sizeof(char)*(namelength));
    out.write((const char*)&components, sizeof(unsigned int));
    out.write((char*)&array[0], numVerts()*components*sizeof(float));
}


