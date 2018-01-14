//
// Created by huangxin on 2018/1/14.
//

#ifndef ANDROLUA1_LUAJAVA_H
#define ANDROLUA1_LUAJAVA_H


#include <jni.h>
#include <stdio.h>
#include <stdlib.h>

#include "../lua/lua.h"
#include "../lua/lualib.h"
#include "../lua/lauxlib.h"


/* Constant that is used to index the JNI Environment */
#define LUAJAVAJNIENVTAG      "__JNIEnv"
/* Defines wheter the metatable is of a java Object */
#define LUAJAVAOBJECTIND      "__IsJavaObject"
/* Defines the lua State Index Property Name */
#define LUAJAVASTATEINDEX     "LuaJavaStateIndex"
/* Index metamethod name */
#define LUAINDEXMETAMETHODTAG "__index"
/* Garbage collector metamethod name */
#define LUAGCMETAMETHODTAG    "__gc"
/* Call metamethod name */
#define LUACALLMETAMETHODTAG  "__call"
/* Constant that defines where in the metatable should I place the function name */
#define LUAJAVAOBJFUNCCALLED  "__FunctionCalled"



static jclass    throwable_class      = NULL;
static jmethodID get_message_method   = NULL;
static jclass    java_function_class  = NULL;
static jmethodID java_function_method = NULL;
static jclass    luajava_api_class    = NULL;
static jclass    java_lang_class      = NULL;


/***************************************************************************
*
* $FC Function objectIndex
*
* $ED Description
*    Function to be called by the metamethod __index of the java object
*
* $EP Function Parameters
*    $P L - lua State
*    $P Stack - Parameters will be received by the stack
*
* $FV Returned Value
*    int - Number of values to be returned by the function
*
*$. **********************************************************************/

static int objectIndex( lua_State * L );


/***************************************************************************
*
* $FC Function objectIndexReturn
*
* $ED Description
*    Function returned by the metamethod __index of a java Object. It is
*    the actual function that is going to call the java method.
*
* $EP Function Parameters
*    $P L - lua State
*    $P Stack - Parameters will be received by the stack
*
* $FV Returned Value
*    int - Number of values to be returned by the function
*
*$. **********************************************************************/

static int objectIndexReturn( lua_State * L );


/***************************************************************************
*
* $FC Function classIndex
*
* $ED Description
*    Function to be called by the metamethod __index of the java class
*
* $EP Function Parameters
*    $P L - lua State
*    $P Stack - Parameters will be received by the stack
*
* $FV Returned Value
*    int - Number of values to be returned by the function
*
*$. **********************************************************************/

static int classIndex( lua_State * L );


/***************************************************************************
*
* $FC Function GC
*
* $ED Description
*    Function to be called by the metamethod __gc of the java object
*
* $EP Function Parameters
*    $P L - lua State
*    $P Stack - Parameters will be received by the stack
*
* $FV Returned Value
*    int - Number of values to be returned by the function
*
*$. **********************************************************************/

static int gc( lua_State * L );


/***************************************************************************
*
* $FC Function javaBindClass
*
* $ED Description
*    Implementation of lua function luajava.BindClass
*
* $EP Function Parameters
*    $P L - lua State
*
* $FV Returned Value
*    int - Number of values to be returned by the function
*
*$. **********************************************************************/

static int javaBindClass( lua_State * L );

/***************************************************************************
*
* $FC Function createProxy
*
* $ED Description
*    Implementation of lua function luajava.createProxy.
*    Transform a lua table into a java class that implements a list
*  of interfaces
*
* $EP Function Parameters
*    $P L - lua State
*    $P Stack - Parameters will be received by the stack
*
* $FV Returned Value
*    int - Number of values to be returned by the function
*
*$. **********************************************************************/

static int createProxy( lua_State * L );

/***************************************************************************
*
* $FC Function javaNew
*
* $ED Description
*    Implementation of lua function luajava.new
*
* $EP Function Parameters
*    $P L - lua State
*    $P Stack - Parameters will be received by the stack
*
* $FV Returned Value
*    int - Number of values to be returned by the function
*
*$. **********************************************************************/

static int javaNew( lua_State * L );


/***************************************************************************
*
* $FC Function javaNewInstance
*
* $ED Description
*    Implementation of lua function luajava.newInstance
*
* $EP Function Parameters
*    $P L - lua State
*    $P Stack - Parameters will be received by the stack
*
* $FV Returned Value
*    int - Number of values to be returned by the function
*
*$. **********************************************************************/

static int javaNewInstance( lua_State * L );


/***************************************************************************
*
* $FC Function javaLoadLib
*
* $ED Description
*    Implementation of lua function luajava.loadLib
*
* $EP Function Parameters
*    $P L - lua State
*    $P Stack - Parameters will be received by the stack
*
* $FV Returned Value
*    int - Number of values to be returned by the function
*
*$. **********************************************************************/

static int javaLoadLib( lua_State * L );


/***************************************************************************
/*
* $FC pushJavaObject
*
* $ED Description
*    Function to create a lua proxy to a java object
*
* $EP Function Parameters
*    $P L - lua State
*    $P javaObject - Java Object to be pushed on the stack
*
* $FV Returned Value
*    int - Number of values to be returned by the function
*
*$. **********************************************************************/

static int pushJavaObject( lua_State * L , jobject javaObject );


/***************************************************************************
*
* $FC pushJavaClass
*
* $ED Description
*    Function to create a lua proxy to a java class
*
* $EP Function Parameters
*    $P L - lua State
*    $P javaObject - Java Class to be pushed on the stack
*
* $FV Returned Value
*    int - Number of values to be returned by the function
*
*$. **********************************************************************/

static int pushJavaClass( lua_State * L , jobject javaObject );


/***************************************************************************
*
* $FC isJavaObject
*
* $ED Description
*    Returns 1 is given index represents a java object
*
* $EP Function Parameters
*    $P L - lua State
*    $P idx - index on the stack
*
* $FV Returned Value
*    int - Boolean.
*
*$. **********************************************************************/

static int isJavaObject( lua_State * L , int idx );


/***************************************************************************
*
* $FC getStateFromCPtr
*
* $ED Description
*    Returns the lua_State from the CPtr Java Object
*
* $EP Function Parameters
*    $P L - lua State
*    $P cptr - CPtr object
*
* $FV Returned Value
*    int - Number of values to be returned by the function.
*
*$. **********************************************************************/

static lua_State * getStateFromCPtr( JNIEnv * env , jobject cptr );


/***************************************************************************
*
* $FC luaJavaFunctionCall
*
* $ED Description
*    function called by metamethod __call of instances of JavaFunctionWrapper
*
* $EP Function Parameters
*    $P L - lua State
*    $P Stack - Parameters will be received by the stack
*
* $FV Returned Value
*    int - Number of values to be returned by the function.
*
*$. **********************************************************************/

static int luaJavaFunctionCall( lua_State * L );


/***************************************************************************
*
* $FC pushJNIEnv
*
* $ED Description
*    function that pushes the jni environment into the lua state
*
* $EP Function Parameters
*    $P env - java environment
*    $P L - lua State
*
* $FV Returned Value
*    void
*
*$. **********************************************************************/

static void pushJNIEnv( JNIEnv * env , lua_State * L );


/***************************************************************************
*
* $FC getEnvFromState
*
* $ED Description
*    auxiliar function to get the JNIEnv from the lua state
*
* $EP Function Parameters
*    $P L - lua State
*
* $FV Returned Value
*    JNIEnv * - JNI environment
*
*$. **********************************************************************/

static JNIEnv * getEnvFromState( lua_State * L );


#endif //ANDROLUA1_LUAJAVA_H
