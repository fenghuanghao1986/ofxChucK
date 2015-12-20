//------------------------------------------------------------------------------
// name: ofck.h
// desc: openframeworks <=> ChucK bridge; ChucK-side bindings
//
// @author  Ge Wang
// @author  Tim O'Brien
// @author  Kitty Shi
// @author  Madeline Huberth
//
// @date    Fall 2015
//------------------------------------------------------------------------------
#ifndef __OF_CK_H__
#define __OF_CK_H__


#include "chuck_dl.h"
#include "x-vector3d.h"


// the query function for ofck
DLL_QUERY ofck_query( Chuck_DL_Query * QUERY );




//------------------------------------------------------------------------------
// class VREntity
//------------------------------------------------------------------------------
class VREntity
{
public:
    // constructor
    VREntity();

public:
    // add child
    void addChild( VREntity * child );

public:
    // sync data with chuck
    void syncFromChucK();
    // get chuck-side object
    Chuck_Object * chuckObject() { return m_chuckObject; }
    // allocate the chuck object
    bool initChucKSideObject();
    
public:
    // update
    virtual void update( double delta );
    // render
    virtual void render();

public:
    // update all, cascading to children
    void updateAll( double delta );
    // render all, cascading to children
    void renderAll();
    
public:
    // location
    Vector3D loc;
    // orientation
    Vector3D ori;
    // scaling
    Vector3D sca;
    // color
    Vector3D col;
    // alpha value
    float alpha;

protected:
    // applies translation, rotation, etc
    void applyTransforms();
    // pop
    void popTransforms();

protected:
    // child nodes in the scene graph
    std::vector<VREntity *> children;

protected:
    // how chuck sees this entity (chuck type: VREntity)
    Chuck_Object * m_chuckObject;
};





//------------------------------------------------------------------------------
// name: class ofckdb
// desc: ofck database
//------------------------------------------------------------------------------
class OFCKDB
{
public:
    // constructor
    OFCKDB();

public:
    // associate a int value with a key
    t_CKINT setInt( const std::string & key, t_CKINT value );
    // get a int value associated with a key
    t_CKINT getInt( const std::string & key );
    // associate a float value with a key
    t_CKFLOAT setFloat( const std::string & key, t_CKFLOAT value );
    // get a float value associated with a key
    t_CKFLOAT getFloat( const std::string & key );
    // associate a vec3 value with a key
    t_CKVEC3 setVec3( const std::string & key, t_CKVEC3 value );
    // get a vec3 value associated with a key
    t_CKVEC3 getVec3( const std::string & key );
    // associate a vec4 value with a key
    t_CKVEC4 setVec4( const std::string & key, t_CKVEC4 value );
    // get a vec3 value associated with a key
    t_CKVEC4 getVec4( const std::string & key );
    // associate a string value with a key
    std::string setString( const std::string & key, const std::string & value );
    // get a vec3 value associated with a key
    std::string getString( const std::string & key );
    // associate an VREntity with a key
    VREntity * setObject( const std::string & key, VREntity * e );
    // get an object associated with a key
    VREntity * getObject( const std::string & key );
    
public:
    // instance
    static OFCKDB * instance() {
        // check
        if( ourInstance == NULL ) { ourInstance = new OFCKDB; }
        return ourInstance;
    }

public:
    // static instance
    static OFCKDB * ourInstance;

public:
    // string to int map
    std::map<std::string, t_CKINT> string2int;
    // string to float map
    std::map<std::string, t_CKFLOAT> string2float;
    // string to float map
    std::map<std::string, t_CKVEC3> string2vec3;
    // string to float map
    std::map<std::string, t_CKVEC4> string2vec4;
    // string to string map
    std::map<std::string, std::string> string2string;
    // string to entity map
    std::map<std::string, VREntity *> string2entity;

    // display sync event is broadcast right before each new graphics frame
    Chuck_Event displaySync;
    // event buffer for queue broadcat
    CBufferSimple * m_eventBuffer;
};




#endif